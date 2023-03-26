#version 140
#extension GL_ARB_explicit_attrib_location : require

in vec3 v_frag_pos;
in vec3 v_normal;
in vec2 v_tex_coord;
in float v_color;

out uint out_palette_index;

uniform usampler2D u_colour_buffer;
uniform vec4 u_clip_planes[6];
uniform int u_clip_plane_count = 0;
uniform int u_viewport_height;

uniform uint u_material_flags;
uniform mat2x3 u_material_uv_transform;
uniform usampler2D u_material_texture_pixelmap;
uniform int u_material_texture_enabled;
uniform int u_material_blend_enabled = 0;
uniform usampler2D u_material_blend_table;
uniform usampler2D u_material_shade_table;
uniform uint u_material_index_base;
uniform uint u_material_index_range;


const uint BR_MATF_LIGHT = 1u;
const uint BR_MATF_PRELIT = 2u;

void main() {

    uint x = u_material_index_range;
    for(int i = 0; i < u_clip_plane_count; i++) {
        // calculate signed plane-vertex distance
        vec4 v4 = vec4(v_frag_pos.x, v_frag_pos.y, v_frag_pos.z, 1);
        float d = dot(u_clip_planes[i], v4);
        if (d < 0.0) discard;
    }

    if (u_material_texture_enabled == 0) {
        // force palette index, no texture lookup
        out_palette_index = u_material_index_base;
    } else {
        // calculate texture uv coordinates
        vec2 sample_coord = vec3(v_tex_coord.xy, 1) * u_material_uv_transform;
        uint texel = texture(u_material_texture_pixelmap, sample_coord.xy).r;

        // color 0 is always transparent
        if (texel == 0u) {
            discard;
        }
        
        if ((u_material_flags & BR_MATF_LIGHT) != 0u && u_material_index_range > 1u) {
            if ((u_material_flags & BR_MATF_PRELIT) != 0u) {
                // shade_table is a 256px-wide image which encodes 256 lit shades for each color
                uint u_v_color = uint(v_color);
                float ranged_color = u_material_index_range * (float(u_v_color) / 256.0);
                ranged_color = 0;
                uint row = u_material_index_base + uint(ranged_color);
                out_palette_index = texelFetch(u_material_shade_table, ivec2(texel, row), 0).r;
                //if (out_palette_index != texel) {
                //    out_palette_index = 255u;
                //}
            } else {
                // shade_table is a 256x256 image which encodes 256 lit shades for each color
                out_palette_index = texelFetch(u_material_shade_table, ivec2(texel, 0), 0).r;
                out_palette_index = 255u;
            }
        } else {
            // no shadetable
            out_palette_index = texel;
        }

        if (u_material_blend_enabled == 1 && out_palette_index != 0u) {
            // blend_table is a 256x256 image which encodes 256 values of blending between texture and existing screen pixel for each color
            // u_colour_buffer is upside down from opengl perspective. We need to sample it upside down.
            uint fb_color = texelFetch(u_colour_buffer, ivec2(gl_FragCoord.x, u_viewport_height - gl_FragCoord.y), 0).r;
            uint blended_color = texelFetch(u_material_blend_table, ivec2(out_palette_index, fb_color), 0).r;
            out_palette_index = blended_color;
        }
    }
    if (out_palette_index == 0u) {
        out_palette_index = 240u;
    }
    
}
