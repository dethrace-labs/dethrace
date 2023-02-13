#version 140
#extension GL_ARB_explicit_attrib_location : require

in vec3 v_frag_pos;
in vec3 v_normal;
in vec2 v_tex_coord;

layout(origin_upper_left) in vec4 gl_FragCoord;

out uint out_palette_index;

uniform mat2x3 u_texture_coords_transform;
uniform usampler2D u_texture_pixelmap;
uniform usampler2D u_shade_table;
uniform usampler2D u_blend_table;
uniform usampler2D u_current_framebuffer;
uniform int u_palette_index_override = -1;
uniform int u_light_value = -1;
uniform vec4 u_clip_planes[6];
uniform int u_clip_plane_count = 0;
uniform int u_blend_enabled = 0;
uniform int u_viewport_height;

void main() {

    for(int i = 0; i < u_clip_plane_count; i++) {
        // calculate signed plane-vertex distance
        vec4 v4 = vec4(v_frag_pos.x, v_frag_pos.y, v_frag_pos.z, 1);
        float d = dot(u_clip_planes[i], v4);
        if (d < 0.0) discard;
    }

    if (u_palette_index_override >= 0) {
        // force palette index, no texture lookup
        out_palette_index = uint(u_palette_index_override);
    } else {
        // calculate texture uv coordinates
        vec2 sample_coord = vec3(v_tex_coord.xy, 1) * u_texture_coords_transform;
        uint texel = texture(u_texture_pixelmap, sample_coord.xy).r;
        if (u_light_value >= 0) {
            // shade_table is a 256x256 image which encodes 256 lit shades for each color
            out_palette_index = texelFetch(u_shade_table, ivec2(texel, u_light_value), 0).r;
        } else {
            // no shadetable
            out_palette_index = texel;
        }

        if (u_blend_enabled == 1 && out_palette_index != 0u) {
            // blend_table is a 256x256 image which encodes 256 values of blending between texture and existing screen pixel for each color
            // current_framebuffer is upside down from opengl perspective. We need to sample it upside down.
            uint fb_color = texelFetch(u_current_framebuffer, ivec2(gl_FragCoord.xy), 0).r;
            uint blended_color = texelFetch(u_blend_table, ivec2(out_palette_index, fb_color), 0).r;
            uint old_out = out_palette_index;
            out_palette_index = blended_color;
            
        }
    }
    // color 0 is always transparent
    if (out_palette_index == 0u) {
        discard;
    }
}
