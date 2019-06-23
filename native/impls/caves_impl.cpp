#include "caves_impl.hpp"
#include <opengl/mygl_glfw.hpp>
#include <program_state.hpp>
#include <iostream>
#include <glm/ext.hpp>
#include <random>
#include <impls/caves_impl/marching_cubes.hpp>
#include <platform/opengl.hpp>

namespace mpp
{
    caves_impl::caves_impl() {
        use_environment<opengl_environment>();
    }
    void caves_impl::on_setup(program_state& state)
    {
        glfwWindowHint(GLFW_SAMPLES, 8);
    }
    void caves_impl::init_mc()
    {
        glCreateBuffers(1, &_mc_buf_case_faces);
        glNamedBufferStorage(_mc_buf_case_faces, std::size(case_faces) * sizeof(case_faces[0]), std::data(case_faces), {});

        glCreateBuffers(1, &_mc_buf_edge_connections);
        glNamedBufferStorage(_mc_buf_edge_connections, std::size(edge_connections) * sizeof(edge_connections[0]), std::data(edge_connections), {});

        _mc_program = glCreateProgram();

        const auto mc_vs = glCreateShader(GL_VERTEX_SHADER);
        const auto mc_gs = glCreateShader(GL_GEOMETRY_SHADER);
        const auto mc_fs = glCreateShader(GL_FRAGMENT_SHADER);

        constexpr auto mc_vs_src = R"(#version 460 core
void main() { 
const ivec3 tex_size = ivec3(64, 64, 64);
const uint vid = uint(gl_VertexID);
const uint id_z = vid / (tex_size.x * tex_size.y);
const uint id_xy = vid % (tex_size.x * tex_size.y);
const uint id_y = id_xy / tex_size.x;
const uint id_x = id_xy % tex_size.x;
gl_Position = vec4(id_x, id_y, id_z, 1); 
}
)";
        constexpr auto mc_gs_src = R"(#version 460 core
layout(points) in;
layout(triangle_strip, max_vertices = 15) out;
layout(binding = 0) restrict readonly buffer CaseFaces { int case_faces[256]; };

struct edge_set_t { uint connections[5]; };
layout(binding = 1) restrict readonly buffer EdgeConnections { edge_set_t edge_connections[256]; };
layout(r8ui, binding = 0) restrict readonly uniform uimage3D point_image;
layout(location = 0) uniform mat4 view_projection;
layout(location = 0) out vec4 gs_pos;
layout(location = 1) out vec3 gs_normal;

void main()
{
    const float scaling = 1.f/16.f;
    const vec4 base_pos = gl_in[0].gl_Position;
    const ivec3 gid = ivec3(base_pos.xyz);

	vec3 voxel_vertices[8];
	voxel_vertices[0] = vec3(1, 0, 0);
	voxel_vertices[1] = vec3(1, 0, 1);
	voxel_vertices[2] = vec3(1, 1, 1);
	voxel_vertices[3] = vec3(1, 1, 0);
	voxel_vertices[4] = vec3(0, 0, 0);
	voxel_vertices[5] = vec3(0, 0, 1);
	voxel_vertices[6] = vec3(0, 1, 1);
	voxel_vertices[7] = vec3(0, 1, 0);

    const uint a = imageLoad(point_image, ivec3(gid.x+1, gid.y, gid.z)).r;
    const uint b = imageLoad(point_image, ivec3(gid.x+1, gid.y, gid.z+1)).r;
    const uint c = imageLoad(point_image, ivec3(gid.x+1, gid.y+1, gid.z+1)).r;
    const uint d = imageLoad(point_image, ivec3(gid.x+1, gid.y+1, gid.z)).r;
    const uint e = imageLoad(point_image, ivec3(gid.x, gid.y, gid.z)).r;
    const uint f = imageLoad(point_image, ivec3(gid.x, gid.y, gid.z+1)).r;
    const uint g = imageLoad(point_image, ivec3(gid.x, gid.y+1, gid.z+1)).r;
    const uint h = imageLoad(point_image, ivec3(gid.x, gid.y+1, gid.z)).r;

    const uint mc_case = ((a&1)<<0) | 
        ((b&1) << 1) |
        ((c&1) << 2) |
        ((d&1) << 3) |
        ((e&1) << 4) |
        ((f&1) << 5) |
        ((g&1) << 6) |
        ((h&1) << 7);

    const int poly_count = case_faces[mc_case];
    const edge_set_t edges = edge_connections[mc_case];
    for(int i=0; i<poly_count; ++i)
    {
        const uint indices = edges.connections[i];
        vec3 vertices[3];
        {
            const uint first = indices & 0x7;
            const uint second = (indices >> 3) & 0x7;
            vertices[0] = mix(voxel_vertices[first], voxel_vertices[second], 0.5);
        }
        {
            const uint first = (indices >> 6) & 0x7;
            const uint second = (indices >> 9) & 0x7;
            vertices[1] = mix(voxel_vertices[first], voxel_vertices[second], 0.5);
        }
        {
            const uint first = (indices >> 12) & 0x7;
            const uint second = (indices >> 15) & 0x7;
            vertices[2] = mix(voxel_vertices[first], voxel_vertices[second], 0.5);
        }

        gs_normal = normalize(cross(normalize(vertices[1] - vertices[0]), normalize(vertices[2] - vertices[0])));
        gs_pos = vec4(scaling * (base_pos.xyz + vertices[0]), 1);
        gl_Position = view_projection * gs_pos;
        EmitVertex();
        gs_pos = vec4(scaling * (base_pos.xyz + vertices[1]), 1);
        gl_Position = view_projection * gs_pos;
        EmitVertex();
        gs_pos = vec4(scaling * (base_pos.xyz + vertices[2]), 1);
        gl_Position = view_projection * gs_pos;
        EmitVertex();
        EndPrimitive();
    }
}
)";
        constexpr auto mc_fs_src = R"(#version 460 core
layout(location = 0) in vec4 gs_pos;
layout(location = 1) in vec3 gs_normal;
layout(location = 0) out vec4 fs_color;
void main()
{
    const vec3 ldir = normalize(vec3(1, 1, 1));

    fs_color = vec4((gs_pos.rgb) * max(dot(gl_FrontFacing ? gs_normal : -gs_normal, ldir), 0.1f), 1) ;
}
)";
        glShaderSource(mc_vs, 1, &mc_vs_src, nullptr);
        glShaderSource(mc_gs, 1, &mc_gs_src, nullptr);
        glShaderSource(mc_fs, 1, &mc_fs_src, nullptr);
        glCompileShader(mc_vs);
        glCompileShader(mc_gs);
        glCompileShader(mc_fs);

        glAttachShader(_mc_program, mc_vs);
        glAttachShader(_mc_program, mc_gs);
        glAttachShader(_mc_program, mc_fs);
        glLinkProgram(_mc_program);

        int did_compile;
        int info_log_length;
        std::string info_log;
        glGetProgramiv(_mc_program, GL_LINK_STATUS, &did_compile);
        glGetProgramiv(_mc_program, GL_INFO_LOG_LENGTH, &info_log_length);
        info_log.resize(info_log_length, '\0');
        glGetProgramInfoLog(_mc_program, info_log_length, &info_log_length, info_log.data());
        std::cout << "MC Program Link Status:\n";
        std::cout << info_log << '\n';

        glCreateVertexArrays(1, &_mc_vao);
    }

    void caves_impl::on_start(program_state& state) {
        _texture_size = { 64, 64, 64 };
        init_mc();

        glCreateTextures(GL_TEXTURE_3D, 1, &_texture_front);
        glCreateTextures(GL_TEXTURE_3D, 1, &_texture_back);
        glTextureStorage3D(_texture_front, 1, GL_R8UI, _texture_size.x, _texture_size.y, _texture_size.z);
        glTextureStorage3D(_texture_back, 1, GL_R8UI, _texture_size.x, _texture_size.y, _texture_size.z);

        std::vector<char> data(_texture_size.x * _texture_size.y * _texture_size.z);
        for (auto& d : data)
            d = _dist(_rng);
        glTextureSubImage3D(_texture_front, 0, 0, 0, 0, _texture_size.x, _texture_size.y, _texture_size.z, GL_RED_INTEGER, GL_UNSIGNED_BYTE, data.data());
        glTextureSubImage3D(_texture_back, 0, 0, 0, 0, _texture_size.x, _texture_size.y, _texture_size.z, GL_RED_INTEGER, GL_UNSIGNED_BYTE, data.data());

        auto simcs = glCreateShader(GL_COMPUTE_SHADER);
        constexpr auto simcs_src = R"(#version 460 core
layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;
layout(r8ui, binding = 0) uniform readonly uimage3D texture_front;
layout(r8ui, binding = 1) uniform writeonly uimage3D texture_back;
layout(location = 0) uniform vec2 cutoffs;
void main()
{
    const ivec3 tsize = ivec3(64, 64, 64);
    const ivec3 gid = ivec3(gl_GlobalInvocationID.xyz);
    uint c = imageLoad(texture_front, gid.xyz).x;

    if(gid.x == 0 || gid.y == 0 || gid.z == 0 || gid.x == tsize.x-1 || gid.y == tsize.y-1 || gid.z == tsize.z-1)
    {
        imageStore(texture_back, gid.xyz, uvec4(0));
        return;
    }

    uint neighs[26];
    int neigh = 0;
    for (int nx = -1; nx <= 1; ++nx)
    {
        for (int ny = -1; ny <= 1; ++ny)
        {
            for (int nz = -1; nz <= 1; ++nz)
            {
                if ((nx != 0 || ny != 0 || nz != 0) && gid.x + nx > 0 && gid.x + nx < tsize.x-1 && gid.y + ny > 0 && gid.y + ny < tsize.y-1 && gid.z + nz > 0 && gid.z + nz < tsize.z-1)
                {
                    neighs[neigh++] = imageLoad(texture_front, gid.xyz + ivec3(nx, ny, nz)).x;
                }
            }
        }
    }

    float nval = 0.f;
    for (int i = 0; i < neigh; ++i)
    {
        if (neighs[i] == 1)
            nval += 1.f;
    }

    
    if (c == 0 && nval > float(neigh) * cutoffs.x)
        c = 1;
    else if (c == 1 && nval < float(neigh) * cutoffs.y)
        c = 0;
    imageStore(texture_back, gid.xyz, uvec4(c, c, c, c));
}
)";
        glShaderSource(simcs, 1, &simcs_src, nullptr);
        glCompileShader(simcs);

        glCreateVertexArrays(1, &_vao);
        glEnableVertexArrayAttrib(_vao, 0);
        glVertexArrayAttribFormat(_vao, 0, 4, GL_FLOAT, false, 0);
        glVertexArrayAttribBinding(_vao, 0, 0);
        glCreateBuffers(1, &_vbo);
        glNamedBufferStorage(_vbo, _texture_size.x * _texture_size.y * _texture_size.z * sizeof(glm::vec4), nullptr, GL_DYNAMIC_STORAGE_BIT);

        const auto meshcs = glCreateShader(GL_COMPUTE_SHADER);
        constexpr auto meshcs_src = R"(#version 460 core
layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;
layout(r8ui, binding = 0) uniform readonly uimage3D texture_front;
layout(binding = 0) writeonly buffer VBO
{
    vec4 vbo[];
};
void main()
{
    const ivec3 tsize = ivec3(64);
    ivec3 gid = ivec3(gl_GlobalInvocationID.xyz);
    const uint c = imageLoad(texture_front, gid).r;
    if(c != 0)
        vbo[gid.z * tsize.y * tsize.x + gid.y * tsize.x + gid.x] = vec4(vec3(gid), 1);
    else
        vbo[gid.z * tsize.y * tsize.x + gid.y * tsize.x + gid.x] = vec4(0.f, 0.f, 0.f, 1);
}
)";
        glShaderSource(meshcs, 1, &meshcs_src, nullptr);
        glCompileShader(meshcs);


        _sim_program = glCreateProgram();
        glAttachShader(_sim_program, simcs);
        glLinkProgram(_sim_program);
        _mesh_program = glCreateProgram();
        glAttachShader(_mesh_program, meshcs);
        glLinkProgram(_mesh_program);

        int len;
        std::string log;

        glGetProgramiv(_sim_program, GL_INFO_LOG_LENGTH, &len);
        log.resize(len, '\0');
        glGetProgramInfoLog(_sim_program, len, &len, log.data());
        std::cout << log << '\n';
        glGetProgramiv(_mesh_program, GL_INFO_LOG_LENGTH, &len);
        log.resize(len, '\0');
        glGetProgramInfoLog(_mesh_program, len, &len, log.data());
        std::cout << log << '\n';

        auto vs = glCreateShader(GL_VERTEX_SHADER);
        constexpr auto vs_src = R"(#version 460 core
layout(location = 0) in vec4 in_pos;
layout(location = 0) uniform mat4 view_proj;
layout(location = 1) uniform float base_point_size;
layout(location = 0) out vec3 vs_pos;
void main() {
    gl_Position = view_proj * vec4(0.025f * in_pos.xyz, 1);
    gl_PointSize = base_point_size / gl_Position.w;
    vs_pos = 0.01f * in_pos.xyz / (1.f+gl_Position.w / 3.f);
}
)";
        glShaderSource(vs, 1, &vs_src, nullptr);
        glCompileShader(vs);
        int did_compile = 0;
        int info_log_length = 0;
        std::string info_log;

        glGetShaderiv(vs, GL_COMPILE_STATUS, &did_compile);
        glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &info_log_length);
        info_log.resize(info_log_length, '\0');
        glGetShaderInfoLog(vs, info_log_length, &info_log_length, info_log.data());
        std::cout << "Shader Compile Status:\n";
        std::cout << info_log << '\n';
        if (!did_compile)
            throw std::runtime_error("Did not compile");

        auto fs = glCreateShader(GL_FRAGMENT_SHADER);
        constexpr auto fs_src = R"(#version 460 core
layout(location = 0) in vec3 vs_pos;
layout(location = 0) out vec4 fs_color;
void main()
{
    fs_color = vec4(vs_pos, 1);
}
)";
        glShaderSource(fs, 1, &fs_src, nullptr);
        glCompileShader(fs);

        glGetShaderiv(fs, GL_COMPILE_STATUS, &did_compile);
        glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &info_log_length);
        info_log.resize(info_log_length, '\0');
        glGetShaderInfoLog(fs, info_log_length, &info_log_length, info_log.data());
        std::cout << "Shader Compile Status:\n";
        std::cout << info_log << '\n';
        if (!did_compile)
            throw std::runtime_error("Did not compile");

        _draw_program = glCreateProgram();
        glAttachShader(_draw_program, vs);
        glAttachShader(_draw_program, fs);
        glLinkProgram(_draw_program);

        glGetProgramiv(_draw_program, GL_LINK_STATUS, &did_compile);
        glGetProgramiv(_draw_program, GL_INFO_LOG_LENGTH, &info_log_length);
        info_log.resize(info_log_length, '\0');
        glGetProgramInfoLog(_draw_program, info_log_length, &info_log_length, info_log.data());
        std::cout << "Program Link Status:\n";
        std::cout << info_log << '\n';

        if (!did_compile)
            throw std::runtime_error("Did not link");

        glDetachShader(_draw_program, vs);
        glDetachShader(_draw_program, fs);
        glDeleteShader(vs);
        glDeleteShader(fs);

        _camera.set_axis_smoothing(0.7f);
        _camera.set_rotate_smoothing(0.7f);
        _camera.set_position(glm::vec3(3, 3, 3));
        _camera.look_at(glm::vec3(0, 0, 0));
    }
    void caves_impl::on_update(program_state & state, seconds delta) {
        _acc_time += delta;
        const auto delta_millis = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(delta);
        const auto gcc = glfwGetCurrentContext();
        double cposx, cposy;
        glfwGetCursorPos(gcc, &cposx, &cposy);
        glEnable(GL_PROGRAM_POINT_SIZE);

        if (!ImGui::GetIO().WantCaptureKeyboard && !ImGui::GetIO().WantTextInput)
        {
            _camera.input_axis_x(float(glfwGetKey(gcc, GLFW_KEY_D) - glfwGetKey(gcc, GLFW_KEY_A)));
            _camera.input_axis_y(float(glfwGetKey(gcc, GLFW_KEY_E) - glfwGetKey(gcc, GLFW_KEY_Q)));
            _camera.input_axis_z(float(glfwGetKey(gcc, GLFW_KEY_W) - glfwGetKey(gcc, GLFW_KEY_S)));
            if (glfwGetKey(gcc, GLFW_KEY_LEFT_CONTROL))
                _camera.input_speed_factor(0.6f);
            else if (glfwGetKey(gcc, GLFW_KEY_LEFT_SHIFT))
                _camera.input_speed_factor(9.f);
            else
                _camera.input_speed_factor(3.f);
        }

        if (!ImGui::GetIO().WantCaptureMouse && glfwGetMouseButton(gcc, GLFW_MOUSE_BUTTON_LEFT))
        {
            _camera.input_rotate_h(float(cposx - _last_curpos.x));
            _camera.input_rotate_v(float(cposy - _last_curpos.y));

            glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else {
            glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        _last_curpos = { cposx, cposy };
        _camera.update(delta_millis);
        glClear(GL_COLOR_BUFFER_BIT);

        if (_running && _acc_time > std::chrono::milliseconds(30))
        {
            glUseProgram(_sim_program);
            glUniform2f(0, _min_cutoff, _max_cutoff);
            glBindImageTexture(0, _texture_front, 0, true, 0, GL_READ_ONLY, GL_R8UI);
            glBindImageTexture(1, _texture_back, 0, true, 0, GL_WRITE_ONLY, GL_R8UI);
            glDispatchCompute(64 / 8, 64 / 8, 64 / 8);
            glCopyImageSubData(_texture_back, GL_TEXTURE_3D, 0, 0, 0, 0, _texture_front, GL_TEXTURE_3D, 0, 0, 0, 0, _texture_size.x, _texture_size.y, _texture_size.z);

            _acc_time = { 0 };
        }

        glUseProgram(_mesh_program);
        glBindImageTexture(0, _texture_front, 0, true, 0, GL_READ_ONLY, GL_R8UI);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, _vbo);
        glDispatchCompute(64 / 8, 64 / 8, 64 / 8);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        int viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        const glm::mat4 view_proj =
            glm::perspectiveFov(glm::radians(60.f), float(viewport[2]), float(viewport[3]), 0.01f, 100.f) *
            _camera.view_matrix();
       /* glUseProgram(_draw_program);
        glProgramUniformMatrix4fv(_draw_program, 0, 1, false, value_ptr(view_proj));
        glProgramUniform1f(_draw_program, 1, _base_point_size);
        glBindVertexArray(_vao);
        glBindVertexBuffer(0, _vbo, 0, sizeof(glm::vec4));
        glDrawArrays(GL_POINTS, 0, _texture_size.x * _texture_size.y * _texture_size.z);*/


        //layout(binding = 0) restrict readonly buffer CaseFaces { int case_faces[256]; };
        //struct edge_set_t { ivec4 connections[5]; };
        //layout(binding = 1) restrict readonly buffer EdgeConnections { edge_set_t edge_connections[256]; };
        //layout(r8ui, binding = 0) restrict readonly uimage2D point_image;
        //layout(location = 0) uniform mat4 view_projection;
        glUseProgram(_mc_program);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, _mc_buf_case_faces);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, _mc_buf_edge_connections);
        glBindImageTexture(0, _texture_front, 0, true, 0, GL_READ_ONLY, GL_R8UI);
        glUniformMatrix4fv(0, 1, false, value_ptr(view_proj));
        glDrawArrays(GL_POINTS, 0, (_texture_size.x-1) * (_texture_size.y - 1) * (_texture_size.z - 1));

        if (ImGui::Begin("Settings"))
        {
            ImGui::Checkbox("Run", &_running);
            ImGui::DragInt("Radius", &_radius, 0.1f, 0, 1000);
            ImGui::DragFloat("Point Size", &_base_point_size, 0.01f, 0.f, 100.f);
            ImGui::DragFloat("Min Cutoff", &_min_cutoff, 0.01f, 0.f, _max_cutoff);
            ImGui::DragFloat("Max Cutoff", &_max_cutoff, 0.01f, _min_cutoff, 1.f);
            if (ImGui::Button("Reset"))
            {
                std::vector<char> data(_texture_size.x * _texture_size.y * _texture_size.z);
                for (auto& d : data)
                    d = _dist(_rng);
                glTextureSubImage3D(_texture_front, 0, 0, 0, 0, _texture_size.x, _texture_size.y, _texture_size.z, GL_RED_INTEGER, GL_UNSIGNED_BYTE, data.data());
                glTextureSubImage3D(_texture_back, 0, 0, 0, 0, _texture_size.x, _texture_size.y, _texture_size.z, GL_RED_INTEGER, GL_UNSIGNED_BYTE, data.data());
            }
            ImGui::End();
        }
    }
    void caves_impl::on_end(program_state & state) {

    }
}