#pragma once

namespace mpp::sift::detail::shader_source
{
    constexpr auto screen_vert = R"(#version 330 core
out vec2 vs_uv;
void main()
{
    gl_Position = vec4(mix(-1.f, 3.f, float(gl_VertexID & 0x1)), mix(-1.f, 3.f, float((gl_VertexID >> 1) & 0x1)), 0.f, 1.f);
    vs_uv = ((gl_Position.xy+1)*0.5f);
}
)";
    constexpr auto gauss_blur_frag = R"(#version 330 core
in vec2 vs_uv;
uniform int u_mip;
uniform int u_dir;
uniform float u_sigma;
uniform sampler2D in_texture;
layout(location = 0) out vec4 out_color;

float gaussian(float sigma, float diff)
{
    const float sqrt_2_pi = 2.50662827463f;
    float inner = diff/sigma;
    float nom = exp(-(inner * inner / 2));
    return nom / (sigma * sqrt_2_pi);
}

void main()
{
    ivec2 uv = ivec2(gl_FragCoord.xy);
    float gval = gaussian(u_sigma, 0.f);
    vec4 color = gval * texelFetch(in_texture, uv, u_mip);
    ivec2 off = ivec2(0, 0);
    ivec2 tsize = ivec2(textureSize(in_texture, u_mip).xy);
    for(int i=1; i<int(6*u_sigma); ++i)
    {
        off[u_dir] = i;
        ivec2 pos_uv = ivec2(mod(uv + off, tsize));
        ivec2 neg_uv = ivec2(mod(uv + tsize - off, tsize));
        gval = gaussian(u_sigma, float(i));
        color += gval * (texelFetch(in_texture, pos_uv, u_mip) 
            + texelFetch(in_texture, neg_uv, u_mip));
    }
    out_color = color;
}
)";
    constexpr auto difference_frag = R"(#version 330 core
in vec2 vs_uv;
uniform sampler2D u_current_tex;
uniform sampler2D u_previous_tex;
layout(location = 0) out vec4 color;
void main()
{
    ivec2 px = ivec2(gl_FragCoord.xy);
    color = abs(texelFetch(u_current_tex, px, 0) - texelFetch(u_previous_tex, px, 0));
}
)";
    constexpr auto filter_frag = R"(#version 330 core
in vec2 vs_uv;
uniform sampler2D u_previous_tex;
uniform sampler2D u_current_tex;
uniform sampler2D u_next_tex;
uniform sampler2D u_feature_tex;
uniform int u_scale;
uniform int u_mip;
uniform int u_border;
layout(location = 0) out vec4 color;

ivec2 tsize;
ivec2 tcl(ivec2 px)
{
    return clamp(px, ivec2(0), tsize - ivec2(1));
}

void main()
{
    ivec2 px = ivec2(gl_FragCoord.xy);
    tsize = ivec2(textureSize(u_current_tex, u_mip));
    if(any(lessThan(px, ivec2(u_border))) || any(greaterThan(px, tsize - ivec2(u_border + 1))) || texelFetch(u_feature_tex, px, u_mip).r == 0)
    {
        color = vec4(0, 0, 0, 1);
        return;
    }

    float d = texelFetch(u_current_tex, px, u_mip).r;
   /* const float threshold = 0.0012f;
    if(d < threshold)
    {
        color = vec4(0, 0, 0, 1);
        return;
    }*/

    float xval_p = texelFetch(u_current_tex, tcl(px + ivec2(1, 0)), u_mip).r;
    float xval_n = texelFetch(u_current_tex, tcl(px + ivec2(-1, 0)), u_mip).r;
    float yval_p = texelFetch(u_current_tex, tcl(px + ivec2(0, 1)), u_mip).r;
    float yval_n = texelFetch(u_current_tex, tcl(px + ivec2(0, -1)), u_mip).r;
    float sval_p = texelFetch(u_next_tex, tcl(px), u_mip).r;
    float sval_n = texelFetch(u_previous_tex, tcl(px), u_mip).r;

    float xval_p_yval_p = texelFetch(u_current_tex, tcl(px + ivec2(1, 1)), u_mip).r;
    float xval_p_yval_n = texelFetch(u_current_tex, tcl(px + ivec2(1, -1)), u_mip).r;
    float xval_n_yval_p = texelFetch(u_current_tex, tcl(px + ivec2(-1, 1)), u_mip).r;
    float xval_n_yval_n = texelFetch(u_current_tex, tcl(px + ivec2(-1, -1)), u_mip).r;

    float xval_p_sval_p = texelFetch(u_next_tex, tcl(px + ivec2(1, 0)), u_mip).r;
    float xval_p_sval_n = texelFetch(u_previous_tex, tcl(px + ivec2(1, 0)), u_mip).r;
    float xval_n_sval_p = texelFetch(u_next_tex, tcl(px + ivec2(-1, 0)), u_mip).r;
    float xval_n_sval_n = texelFetch(u_previous_tex, tcl(px + ivec2(-1, 0)), u_mip).r;

    float sval_p_yval_p = texelFetch(u_next_tex, tcl(px + ivec2(0, 1)), u_mip).r;
    float sval_p_yval_n = texelFetch(u_next_tex, tcl(px + ivec2(0, -1)), u_mip).r;
    float sval_n_yval_p = texelFetch(u_previous_tex, tcl(px + ivec2(0, 1)), u_mip).r;
    float sval_n_yval_n = texelFetch(u_previous_tex, tcl(px + ivec2(0, -1)), u_mip).r;

    vec3 gradient = vec3(
        xval_p - xval_n,
        yval_p - yval_n,
        sval_p - sval_n
    );
    mat3 hessian;
    hessian[0][0] = (xval_p + xval_n) - 2 * d;
    hessian[0][1] = hessian[1][0] = (xval_p_yval_p + xval_n_yval_n - xval_p_yval_n - xval_p_yval_n) / 4.f;
    hessian[1][1] = (yval_p + yval_n) - 2 * d;
    hessian[0][2] = hessian[2][0] = (xval_p_sval_p + xval_n_sval_n - xval_p_sval_n - xval_n_sval_p) / 4.f;
    hessian[2][2] = (sval_p + sval_n) - 2 * d;
    hessian[1][2] = hessian[2][1] = (sval_p_yval_p + sval_n_yval_n - sval_n_yval_p - sval_p_yval_n) / 4.f;

    // if any offset values are greater than 0.5, discard the point.
    vec3 interpolated = inverse(hessian) * gradient;
    bool offset_lt_half = all(lessThan(abs(interpolated), vec3(0.5)));

    // if the ratio of the eigen values of the upper left 2x2 hessian matrix are greater than a given threshold, discard them as they lie on an edge.
    float eigen_val_1 = hessian[0][0] - hessian[0][1];
    float eigen_val_2 = hessian[0][0] + hessian[0][1];
    bool eigen_values_valid = min(eigen_val_1, eigen_val_2) / max(eigen_val_1, eigen_val_2) < 0.5f;

    if(offset_lt_half && eigen_values_valid)
    {
        vec3 step_size = vec3(1 << u_mip, 1 << u_mip, 1.f);
        vec3 start_point = vec3(vec2(px), u_scale);
        vec3 final_point = (start_point + interpolated) * step_size;

        float lobe = float(1 << (u_mip + 1)) * (u_scale + interpolated.z + 1) + 1;
        float scale = 1.2f / 3.f * lobe;
        color = vec4(final_point, scale);
    }
    else
    {
        color = vec4(0, 0, 0, 1);
        return;
    }
}
)";
    constexpr auto orientation_frag = R"(#version 330 core
in vec2 vs_uv;

const int kernel_half_size = 7;
const int kernel_size = kernel_half_size + kernel_half_size + 1;
const int kernel_max_count = kernel_size * kernel_size;
const float pi = 3.141592653587f;

#define arr_def(TYPE, NAME, SIZE) TYPE NAME[SIZE]; int ptr_##NAME = 0;
#define arr_push(NAME, VALUE) NAME[ptr_##NAME++] = VALUE;
#define arr_len(NAME) (ptr_##NAME)

uniform int u_mip;
uniform int u_scale;
uniform int u_orientation_slices;
uniform float u_orientation_magnitude_threshold;
uniform sampler2D u_images[16];

layout(location = 0) out vec4 color;

float gaussian(float sigma, float diff)
{
    float sqrt_2_pi = 2.50662827463f;
    float inner = diff/sigma;
    float nom = exp(-(inner * inner / 2));
    return nom / (sigma * sqrt_2_pi);
}

void main()
{
    ivec2 px = ivec2(gl_FragCoord.xy);
    ivec2 tsize = textureSize(u_images[u_scale], u_mip);

    // Discard features where the window does not fit inside the image.
    if (px.x - kernel_half_size <= 0 || px.x + kernel_half_size >= tsize.x - 1 || px.y - kernel_half_size <= 0 || px.y + kernel_half_size > tsize.y - 1)
    {
        color = vec4(-100.f, 0, 0, 1.f);    
        return;
    }

    arr_def(float, angles, kernel_max_count);
    arr_def(vec2, vectors, kernel_max_count);
    vec4 feat = texelFetch(u_images[u_scale], ivec2(px.x, px.y), u_mip);
    vec2 rdloc = feat.xy;
    float rdscale = feat.z;

    for (int win_y = -kernel_half_size; win_y <= kernel_half_size; ++win_y)
    {
        for (int win_x = -kernel_half_size; win_x <= kernel_half_size; ++win_x)
        {
            int x = int(round(rdloc.x)) + win_x;
            int y = int(round(rdloc.y)) + win_y;

            #define rd_img(X, Y) texelFetch(u_images[int(round(rdscale))], ivec2(X + 1, Y), u_mip).r

            float xdiff = rd_img(x + 1, y) - rd_img(x - 1, y);
            float ydiff = rd_img(x, y + 1) - rd_img(x, y - 1);
            float mag = sqrt(xdiff * xdiff + ydiff * ydiff);

            float g = gaussian(2.5f, length(vec2(win_x + 0.5f, win_y + 0.5f)));
            arr_push(angles, atan(ydiff, xdiff));
            arr_push(vectors, vec2(g * xdiff, g * ydiff));
        }
    }

    float mag_max = 0;
    float ang_max = pi/2.f;
    int slices = 36;
    int step = int((2 * pi) / slices);
    for (int angle = 0; angle < slices; ++angle)
    {
        float min_angle = step * angle - pi;
        float max_angle = min_angle + pi / 3.f;

        vec2 sum = vec2(0, 0);
        for (int i = 0; i < arr_len(angles); ++i)
        {
            float ang_mod = mod(angles[i] + 2 * pi, 2 * pi);
            if (ang_mod >= min_angle && ang_mod <= max_angle)
            {
                sum += vectors[i];
            }
        }

        float length2 = dot(sum, sum);
        if (length2 > mag_max)
        {
            mag_max = length2;
            ang_max = atan(sum.y, sum.x);
        }
    }
    if (mag_max > 0.0002f)
    {
        color.r = ang_max;
    }
    else
    {
        color = vec4(-100.f, 0, 0, 1.f);
    }
}
)";

    constexpr auto maximize_frag = R"(#version 330 core
in vec2 vs_uv;
uniform sampler2D u_previous_tex;
uniform sampler2D u_current_tex;
uniform sampler2D u_next_tex;
uniform int u_mip;
uniform int u_scale;
uniform int u_neighbors; // 0 both, 1 only next, -1 only prev
layout(location = 0) out vec4 color;
void main()
{
    ivec2 px = ivec2(gl_FragCoord.xy);
    // look at neighbors
    //float max_val = -1.f/0.f;
    //float min_val = 1.f/0.f;
    ivec2 tsize = ivec2(textureSize(u_current_tex, u_mip).xy);
    float val_curr = texelFetch(u_current_tex, px, u_mip).r;
    float cmax_val = -1.f/0.f;
    float cmin_val = 1.f/0.f;
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            if(x == 0 && y == 0)
                continue;
            ivec2 pos_uv = ivec2(clamp(px + ivec2(x, y), ivec2(0, 0), tsize - 1));
            float vprev = u_neighbors == 1 ? -(1.f/0.f) : texelFetch(u_previous_tex, pos_uv, u_mip).r;
            float vcurr = texelFetch(u_current_tex, pos_uv, u_mip).r;
            float vnext = u_neighbors == -1 ? -(1.f/0.f) : texelFetch(u_next_tex, pos_uv, u_mip).r;
            
            float maxval = max(vprev, max(vcurr, vnext));
            float minval = min(vprev, min(vcurr, vnext));
            if(maxval > cmax_val)
            {
                cmax_val = maxval;
            }
            if(minval < cmin_val)
            {
                cmin_val = minval;
            }
        }
    }
    
    // is extremum
    if(cmax_val < val_curr || cmin_val > val_curr)
        color = vec4(px.x, px.y, u_scale, 1);
    else
        discard;
}
)";
}