#pragma once

namespace mpp::sift::detail::shader_source
{
    constexpr auto screen_vert = R"(#version 320 es
#ifdef GL_ES
    precision highp float;
#endif
out vec2 vs_uv;
void main()
{
    gl_Position = vec4(mix(-1.f, 3.f, float(gl_VertexID & 0x1)), mix(-1.f, 3.f, float((gl_VertexID >> 1) & 0x1)), 0.f, 1.f);
    vs_uv = ((gl_Position.xy+1.0f)*0.5f);
}
)";
    constexpr auto gauss_blur_frag = R"(#version 320 es
#ifdef GL_ES
    precision highp float;
#endif
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
    float nom = exp(-(inner * inner / 2.f));
    return nom / (sigma * sqrt_2_pi);
}

void main()
{
    ivec2 uv = ivec2(gl_FragCoord.xy);
    float gval = gaussian(u_sigma, 0.f);
    vec4 color = gval * texelFetch(in_texture, uv, u_mip);
    ivec2 off = ivec2(0, 0);
    ivec2 tsize = ivec2(textureSize(in_texture, u_mip).xy);
    for(int i=1; i<int(6.f*u_sigma); ++i)
    {
        off[u_dir] = i;
        ivec2 pos_uv = ivec2(mod(vec2(uv + off), vec2(tsize)));
        ivec2 neg_uv = ivec2(mod(vec2(uv + tsize - off), vec2(tsize)));
        gval = gaussian(u_sigma, float(i));
        color += gval * (texelFetch(in_texture, pos_uv, u_mip) 
            + texelFetch(in_texture, neg_uv, u_mip));
    }
    out_color = color;
}
)";
    constexpr auto difference_frag = R"(#version 320 es
#ifdef GL_ES
    precision highp float;
#endif
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
    constexpr auto orientation_vert = R"(#version 320 es
#ifdef GL_ES
    precision highp float;
#endif
layout(location = 0) in vec4 feature;
layout(location = 1) in int octave;

out vec4 ft_feature;
flat out ivec2 ft_pixel;
flat out int ft_scale;
flat out int ft_octave;

void main()
{
    ft_feature = feature;
    ft_pixel = ivec2(round(feature.xy));
    ft_scale = int(round(feature.z));
    ft_octave = octave;
}
)";
    constexpr auto descriptor_comp = R"(#version 320 es
layout(local_size_x = 32) in;
struct in_feature_t
{
    float x;
    float y;   
    float sigma;
    float scale;
    int octave;
    float orientation; // angle in radians
    vec2 _pad;
};
struct feature_t
{
    float x;
    float y;   
    float sigma;
    float scale;

    int octave;
    float orientation; // angle in radians
    vec2 _pad;

    float descriptor[128];
};
layout(std430, binding = 0) restrict readonly buffer InFeatures {
    in_feature_t in_features[];
};

layout(std430, binding = 1) restrict buffer OutFeatures {
    feature_t out_features[];
};
uniform sampler2D u_textures[16];
const float pi = 3.141592653587;
float gaussian(float sigma, float diff)
{
    const float sqrt_2_pi = 2.50662827463f;
    float inner = diff/sigma;
    float nom = exp(-(inner * inner / 2.f));
    return nom / (sigma * sqrt_2_pi);
}

void main()
{
    if(int(gl_GlobalInvocationID.x) > in_features.length())
        return;

    int gid = int(gl_GlobalInvocationID.x);
    in_feature_t ft = in_features[gid];
    ivec2 px = ivec2(round(ft.x), round(ft.y));
    int ft_scale = int(round(ft.sigma));
    int octave = ft.octave;
    float orientation = ft.orientation;

    out_features[gid].x = ft.x;
    out_features[gid].y = ft.y;
    out_features[gid].sigma = ft.sigma;
    out_features[gid].scale = ft.scale;
    out_features[gid].octave = ft.octave;
    out_features[gid].orientation = ft.orientation;
    for(int i=0; i < 128; ++i)
        out_features[gid].descriptor[i] = 0.f;

    // iterate over frames.
    for (int fy = -2; fy < 2; ++fy)
    {
        for (int fx = -2; fx < 2; ++fx)
        {
            int fdx = fx * 4;
            int fdy = fy * 4;

            // iterate over frame elements
            for (int ex = 0; ex < 4; ++ex)
            {
                for (int ey = 0; ey < 4; ++ey)
                {
                    int x = px.x + fdx + ex;
                    int y = px.y + fdy + ey;
            
                    float tpx = texelFetch(u_textures[ft_scale], ivec2(x+1, y), octave).r;
                    float tnx = texelFetch(u_textures[ft_scale], ivec2(x-1, y), octave).r;
                    float tpy = texelFetch(u_textures[ft_scale], ivec2(x, y+1), octave).r;
                    float tny = texelFetch(u_textures[ft_scale], ivec2(x, y-1), octave).r;
                    float xdiff = tpx - tnx;
                    float ydiff = tpy - tny;

                    float mag = sqrt(xdiff * xdiff + ydiff * ydiff);
                    float theta = atan(ydiff, xdiff) + pi; // normalize to [0, 2*PI]
                    float g = gaussian(2.5f, length(vec2(float(ex) - 1.5f, float(ey) - 1.5f)));

                    // compute angle difference to dominant orientation. In range rad[0, 2*pi] (deg[0, 360])
                    float angle_diff = mod((orientation - theta) + 3.f * pi, 2.f * pi);
                    int angle_index = int(floor((angle_diff / (2.f * pi)) * 8.f));
        
                    int didx = (fx + 2) + (fy + 2) * 4 + angle_index * 4 * 4;
                    out_features[gid].descriptor[didx] += g * mag;
                }
            }
        }
    }
    out_features[gid]._pad[0] = 0.f;
    out_features[gid]._pad[1] = 0.f;
}

)";

    constexpr auto orientation_geom = R"(#version 320 es
#ifdef GL_ES
    precision highp float;
#endif
layout(points) in;
layout (points, max_vertices = 1) out;

in vec4 ft_feature[];
flat in ivec2 ft_pixel[];
flat in int ft_scale[];
flat in int ft_octave[];

uniform sampler2D u_textures[16];
const float pi = 3.141592653587;

out vec4 feature;
out int octave;
out float orientation;
out ivec2 pad2;

float gaussian(float sigma, float diff)
{
    const float sqrt_2_pi = 2.50662827463f;
    float inner = diff/sigma;
    float nom = exp(-(inner * inner / 2.f));
    return nom / (sigma * sqrt_2_pi);
}

void main()
{
    feature = ft_feature[0];
    octave = ft_octave[0];
    pad2 = ivec2(0, 0);

    // Compute orientation
    ivec2 px = ivec2(ft_pixel[0].x >> octave, ft_pixel[0].y >> octave);
    ivec2 tsize = textureSize(u_textures[ft_scale[0]], octave);
    const int window_size_half = 5;
    const int window_width = window_size_half + window_size_half + 1;

    // Discard features where the window does not fit inside the image.
    if (px.x - window_size_half <= 0 || px.x + window_size_half >= tsize.x - 1 || px.y - window_size_half <= 0 || px.y + window_size_half > tsize.y - 1)
        return;

    // Subdivide 360 degrees into 36 bins of 10 degrees.
    // Then compute a gaussian- and magnitude-weighted orientation histogram.
    float mag_max = 0.f;
    float ang_max = pi / 2.f;
    const int slices = 24;
    float step = (2.f * pi) / float(slices);

    vec2 vectors[slices];
    for(int i=0; i<slices; ++i) vectors[i] = vec2(0, 0);

    for (int win_y = -window_size_half; win_y <= window_size_half; ++win_y)
    {
        for (int win_x = -window_size_half; win_x <= window_size_half; ++win_x)
        {
            int x = px.x + win_x;
            int y = px.y + win_y;
            
            float tpx = texelFetch(u_textures[ft_scale[0]], ivec2(x+1, y), octave).r;
            float tnx = texelFetch(u_textures[ft_scale[0]], ivec2(x-1, y), octave).r;
            float tpy = texelFetch(u_textures[ft_scale[0]], ivec2(x, y+1), octave).r;
            float tny = texelFetch(u_textures[ft_scale[0]], ivec2(x, y-1), octave).r;

            float xdiff = tpx - tnx;
            float ydiff = tpy - tny;

            float g = gaussian(1.83f, length(vec2(float(win_x) + 0.5f, float(win_y) + 0.5f)));

            float angle = mod(atan(ydiff, xdiff) + pi, 2.f * pi);
            vec2 mag = vec2(g * xdiff, g * ydiff);

            // Put into multiple bins to smooth out the histogram (b-1, b, b+1)
            int bin = (int(angle / step) + slices - 1) % slices;
            vectors[bin] += mag;
            bin = (bin + 1) % slices;
            vectors[bin] += mag;
            bin = (bin + 1) % slices;
            vectors[bin] += mag;
        }
    }

    vec2 it = vec2(0);
    for(int i=0; i<slices; ++i)
    {
        vec2 v = vectors[i];
        if(dot(v, v) > dot(it, it))
        {
            it = v;
        }
    }

    if (dot(it, it) > 0.00015f)
    {
        orientation = atan(it.x, it.y);

        EmitVertex();
        EndPrimitive();
    }
}
)";

    constexpr auto filter_vert = R"(#version 320 es
#ifdef GL_ES
    precision highp float;
#endif
uniform sampler2D u_current_tex;
uniform int u_mip;
flat out ivec2 pixel;
void main()
{
    ivec2 ts = ivec2(textureSize(u_current_tex, u_mip));
    pixel = ivec2(gl_VertexID % ts.x, gl_VertexID / ts.x);
}
)";

    constexpr auto filter_geom = R"(#version 320 es
#extension GL_EXT_geometry_shader : enable
#ifdef GL_ES
    precision highp float;
#endif
layout(points) in;
layout (points, max_vertices = 1) out;

flat in ivec2 pixel[];
uniform sampler2D u_previous_tex;
uniform sampler2D u_current_tex;
uniform sampler2D u_next_tex;
uniform sampler2D u_feature_tex;
uniform int u_scale;
uniform int u_mip;
uniform int u_border;

out vec4 feature;
out int octave;
out ivec3 pad3;

ivec2 tsize;
ivec2 tcl(ivec2 px)
{
    return clamp(px, ivec2(0), tsize - ivec2(1));
}

void main()
{
    ivec2 px = ivec2(pixel[0]);
    tsize = ivec2(textureSize(u_current_tex, u_mip));
//#define texelFetch_(T, U, M) (textureLod((T), vec2(U) / vec2(tsize), float(M)))
#define texelFetch_(T, U, M) (texelFetch(T, U, M))

    if(any(lessThan(px, ivec2(u_border))) || any(greaterThan(px, tsize - ivec2(u_border + 1))) || all(equal(texelFetch_(u_feature_tex, px, u_mip), vec4(0, 0, 0, 1))))
    {
        return;
    }

    float d = texelFetch_(u_current_tex, px, u_mip).r;
   /* const float threshold = 0.0012f;
    if(d < threshold)
    {
        color = vec4(0, 0, 0, 1);
        return;
    }*/

    float xval_p = texelFetch_(u_current_tex, tcl(px + ivec2(1, 0)), u_mip).r;
    float xval_n = texelFetch_(u_current_tex, tcl(px + ivec2(-1, 0)), u_mip).r;
    float yval_p = texelFetch_(u_current_tex, tcl(px + ivec2(0, 1)), u_mip).r;
    float yval_n = texelFetch_(u_current_tex, tcl(px + ivec2(0, -1)), u_mip).r;
    float sval_p = texelFetch_(u_next_tex, tcl(px), u_mip).r;
    float sval_n = texelFetch_(u_previous_tex, tcl(px), u_mip).r;

    float xval_p_yval_p = texelFetch_(u_current_tex, tcl(px + ivec2(1, 1)), u_mip).r;
    float xval_p_yval_n = texelFetch_(u_current_tex, tcl(px + ivec2(1, -1)), u_mip).r;
    float xval_n_yval_p = texelFetch_(u_current_tex, tcl(px + ivec2(-1, 1)), u_mip).r;
    float xval_n_yval_n = texelFetch_(u_current_tex, tcl(px + ivec2(-1, -1)), u_mip).r;

    float xval_p_sval_p = texelFetch_(u_next_tex, tcl(px + ivec2(1, 0)), u_mip).r;
    float xval_p_sval_n = texelFetch_(u_previous_tex, tcl(px + ivec2(1, 0)), u_mip).r;
    float xval_n_sval_p = texelFetch_(u_next_tex, tcl(px + ivec2(-1, 0)), u_mip).r;
    float xval_n_sval_n = texelFetch_(u_previous_tex, tcl(px + ivec2(-1, 0)), u_mip).r;

    float sval_p_yval_p = texelFetch_(u_next_tex, tcl(px + ivec2(0, 1)), u_mip).r;
    float sval_p_yval_n = texelFetch_(u_next_tex, tcl(px + ivec2(0, -1)), u_mip).r;
    float sval_n_yval_p = texelFetch_(u_previous_tex, tcl(px + ivec2(0, 1)), u_mip).r;
    float sval_n_yval_n = texelFetch_(u_previous_tex, tcl(px + ivec2(0, -1)), u_mip).r;

    vec3 gradient = vec3(
        xval_p - xval_n,
        yval_p - yval_n,
        sval_p - sval_n
    );
    mat3 hessian;
    hessian[0][0] = (xval_p + xval_n) - 2.f * d;
    hessian[0][1] = hessian[1][0] = (xval_p_yval_p + xval_n_yval_n - xval_p_yval_n - xval_p_yval_n) / 4.f;
    hessian[1][1] = (yval_p + yval_n) - 2.f * d;
    hessian[0][2] = hessian[2][0] = (xval_p_sval_p + xval_n_sval_n - xval_p_sval_n - xval_n_sval_p) / 4.f;
    hessian[2][2] = (sval_p + sval_n) - 2.f * d;
    hessian[1][2] = hessian[2][1] = (sval_p_yval_p + sval_n_yval_n - sval_n_yval_p - sval_p_yval_n) / 4.f;

    // if any offset values are greater than 0.5, discard the point.
    vec3 interpolated = inverse(hessian) * gradient;
    bool offset_lt_half = all(lessThan(abs(interpolated), vec3(0.5)));

    // if the ratio of the eigen values of the upper left 2x2 hessian matrix are greater than a given threshold, discard them as they lie on an edge.
    float eigen_val_1 = hessian[0][0] - hessian[0][1];
    float eigen_val_2 = hessian[0][0] + hessian[0][1];
    bool eigen_values_valid = min(eigen_val_1, eigen_val_2) / max(eigen_val_1, eigen_val_2) < 0.7f;

    if(offset_lt_half && eigen_values_valid)
    {
        vec3 step_size = vec3(1 << u_mip, 1 << u_mip, 1.f);
        vec3 start_point = vec3(vec2(px), u_scale);
        vec3 final_point = (start_point + interpolated) * step_size;

        float lobe = float(1 << (u_mip + 1)) * (float(u_scale) + interpolated.z + 1.f) + 1.f;
        float scale = 1.2f / 3.f * lobe;
        feature = vec4(final_point, scale);
        octave = u_mip;
        pad3 = ivec3(0, 0, 0);
        EmitVertex();
        EndPrimitive();
    }
}
)";

    constexpr auto filter_frag = R"(#version 320 es
#ifdef GL_ES
    precision highp float;
#endif
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
//#define texelFetch_(T, U, M) (textureLod((T), vec2(U) / vec2(tsize), float(M)))
#define texelFetch_(T, U, M) (texelFetch(T, U, M))
    if(any(lessThan(px, ivec2(u_border))) || any(greaterThan(px, tsize - ivec2(u_border + 1))) || texelFetch_(u_feature_tex, px, u_mip).r == 0.f)
    {
        color = vec4(0, 0, 0, 1);
        return;
    }

    float d = texelFetch_(u_current_tex, px, u_mip).r;
   /* const float threshold = 0.0012f;
    if(d < threshold)
    {
        color = vec4(0, 0, 0, 1);
        return;
    }*/

    float xval_p = texelFetch_(u_current_tex, tcl(px + ivec2(1, 0)), u_mip).r;
    float xval_n = texelFetch_(u_current_tex, tcl(px + ivec2(-1, 0)), u_mip).r;
    float yval_p = texelFetch_(u_current_tex, tcl(px + ivec2(0, 1)), u_mip).r;
    float yval_n = texelFetch_(u_current_tex, tcl(px + ivec2(0, -1)), u_mip).r;
    float sval_p = texelFetch_(u_next_tex, tcl(px), u_mip).r;
    float sval_n = texelFetch_(u_previous_tex, tcl(px), u_mip).r;

    float xval_p_yval_p = texelFetch_(u_current_tex, tcl(px + ivec2(1, 1)), u_mip).r;
    float xval_p_yval_n = texelFetch_(u_current_tex, tcl(px + ivec2(1, -1)), u_mip).r;
    float xval_n_yval_p = texelFetch_(u_current_tex, tcl(px + ivec2(-1, 1)), u_mip).r;
    float xval_n_yval_n = texelFetch_(u_current_tex, tcl(px + ivec2(-1, -1)), u_mip).r;

    float xval_p_sval_p = texelFetch_(u_next_tex, tcl(px + ivec2(1, 0)), u_mip).r;
    float xval_p_sval_n = texelFetch_(u_previous_tex, tcl(px + ivec2(1, 0)), u_mip).r;
    float xval_n_sval_p = texelFetch_(u_next_tex, tcl(px + ivec2(-1, 0)), u_mip).r;
    float xval_n_sval_n = texelFetch_(u_previous_tex, tcl(px + ivec2(-1, 0)), u_mip).r;

    float sval_p_yval_p = texelFetch_(u_next_tex, tcl(px + ivec2(0, 1)), u_mip).r;
    float sval_p_yval_n = texelFetch_(u_next_tex, tcl(px + ivec2(0, -1)), u_mip).r;
    float sval_n_yval_p = texelFetch_(u_previous_tex, tcl(px + ivec2(0, 1)), u_mip).r;
    float sval_n_yval_n = texelFetch_(u_previous_tex, tcl(px + ivec2(0, -1)), u_mip).r;

    vec3 gradient = vec3(
        xval_p - xval_n,
        yval_p - yval_n,
        sval_p - sval_n
    );
    mat3 hessian;
    hessian[0][0] = (xval_p + xval_n) - 2.f * d;
    hessian[0][1] = hessian[1][0] = (xval_p_yval_p + xval_n_yval_n - xval_p_yval_n - xval_p_yval_n) / 4.f;
    hessian[1][1] = (yval_p + yval_n) - 2.f * d;
    hessian[0][2] = hessian[2][0] = (xval_p_sval_p + xval_n_sval_n - xval_p_sval_n - xval_n_sval_p) / 4.f;
    hessian[2][2] = (sval_p + sval_n) - 2.f * d;
    hessian[1][2] = hessian[2][1] = (sval_p_yval_p + sval_n_yval_n - sval_n_yval_p - sval_p_yval_n) / 4.f;

    // if any offset values are greater than 0.5, discard the point.
    vec3 interpolated = inverse(hessian) * gradient;
    bool offset_lt_half = all(lessThan(abs(interpolated), vec3(0.5)));

    // if the ratio of the eigen values of the upper left 2x2 hessian matrix are greater than a given threshold, discard them as they lie on an edge.
    float eigen_val_1 = hessian[0][0] - hessian[0][1];
    float eigen_val_2 = hessian[0][0] + hessian[0][1];
    bool eigen_values_valid = min(eigen_val_1, eigen_val_2) / max(eigen_val_1, eigen_val_2) < 0.7f;

    if(offset_lt_half && eigen_values_valid)
    {
        vec3 step_size = vec3(1 << u_mip, 1 << u_mip, 1.f);
        vec3 start_point = vec3(vec2(px), u_scale);
        vec3 final_point = (start_point + interpolated) * step_size;

        float lobe = float(1 << (u_mip + 1)) * (float(u_scale) + interpolated.z + 1.f) + 1.f;
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

    constexpr auto maximize_frag = R"(#version 320 es
#ifdef GL_ES
    precision highp float;
#endif
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

//#define texelFetch_(T, U, M) (textureLod((T), vec2(U) / vec2(tsize), float(M)))
#define texelFetch_(T, U, M) (texelFetch(T, U, M))

    float val_curr = texelFetch_(u_current_tex, px, u_mip).r;
    float cmax_val = -1.f/0.f;
    float cmin_val = 1.f/0.f;
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            if(x == 0 && y == 0)
                continue;
            ivec2 pos_uv = ivec2(clamp(px + ivec2(x, y), ivec2(0, 0), tsize - 1));
            float vprev = u_neighbors == 1 ? -(1.f/0.f) : texelFetch_(u_previous_tex, pos_uv, u_mip).r;
            float vcurr = texelFetch_(u_current_tex, pos_uv, u_mip).r;
            float vnext = u_neighbors == -1 ? -(1.f/0.f) : texelFetch_(u_next_tex, pos_uv, u_mip).r;
            
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
    constexpr auto transform_feedback_reduce_vert = R"(#version 320 es
#ifdef GL_ES
    precision highp float;
#endif
uniform sampler2D u_texture;
uniform int u_mip;
flat out vec4 vs_pos;
void main()
{
    ivec2 ts = ivec2(textureSize(u_texture, u_mip));
#define texelFetch_(T, U, M) (textureLod((T), vec2(U) / vec2(ts), float(M)))
    vs_pos = texelFetch(u_texture, ivec2(gl_VertexID % ts.x, gl_VertexID / ts.x), u_mip);
}
)";

    constexpr auto transform_feedback_reduce_geom = R"(#version 320 es
#extension GL_EXT_geometry_shader : enable
#ifdef GL_ES
    precision highp float;
#endif
layout(points) in;
layout (points, max_vertices = 1) out;
flat in vec4 vs_pos[];
uniform int u_mip;
out vec4 feature_values;
out int octave;
out vec3 skip_c;

void main()
{
    if(any(notEqual(vs_pos[0].xyz, vec3(0, 0, 0))))
    {
        feature_values = vs_pos[0];
        octave = u_mip;
        skip_c = vec3(0, 0, 0);
        EmitVertex();
        EndPrimitive();
    }
}
)";
}