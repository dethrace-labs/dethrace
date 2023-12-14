// Input, output variables
// =======================

in vec3 v_frag_pos;
in vec3 v_normal;
in vec2 v_tex_coord;

// this is actually an integer palette index. We define it as a float to allow opengl to
// interpolate the value across each face.
in float v_color;

// 256 color mode, so each calculated color is a palette index.
out uint out_palette_index;

// Uniform variables
// =================

// colour_buffer is an image which contains the current content of the framebuffer
uniform usampler2D u_colour_buffer;
uniform uint u_viewport_height;

uniform vec4 u_clip_planes[6];
uniform int u_clip_plane_count;

uniform uint u_material_flags;
uniform mat2x3 u_material_uv_transform;
uniform usampler2D u_material_texture_pixelmap;
uniform uint u_material_texture_enabled;

// material_blend_table is a 256x256 image which encodes 256 values of blending between new color and current color in framebuffer
uniform uint u_material_blend_enabled;
uniform usampler2D u_material_blend_table;

// material_shade_table is a 256px-wide image which encodes material_shade_table_height lit shades for each color
uniform usampler2D u_material_shade_table;

// how many lit shades are in the material_shade_table
uniform uint u_material_shade_table_height;

// For non-textured materials, defines the starting palette index
uniform uint u_material_index_base;
// For non-textured materials, defines the range of lit shades
uniform uint u_material_index_range;

// material_flags values
const uint BR_MATF_LIGHT = 1u;
const uint BR_MATF_PRELIT = 2u;

void main(void) {

    for(int i = 0; i < u_clip_plane_count; i++) {
        // calculate signed plane-vertex distance
        vec4 v4 = vec4(v_frag_pos.xyz, 1);
        float d = dot(u_clip_planes[i], v4);
        if (d < 0.0) {
            discard;
        }
    }

    if (u_material_texture_enabled == 0u) {
        // force palette index, no texture lookup
        out_palette_index = u_material_index_base;

        if ((u_material_flags & BR_MATF_LIGHT) != 0u) {
            // TODO: lighting calculations based on https://rr2000.cwaboard.co.uk/R4/BRENDER/TEBK_43.HTM#0
        }
    }
    else {
        // calculate texture uv coordinates
        vec2 sample_coord = vec3(v_tex_coord.xy, 1) * u_material_uv_transform;
        uint texel = texture(u_material_texture_pixelmap, sample_coord.xy).r;

        // color 0 is always transparent
        if (texel == 0u) {
            discard;
        }

        if ((u_material_flags & BR_MATF_LIGHT) != 0u) {
            if ((u_material_flags & BR_MATF_PRELIT) != 0u) {
                // BR_MATF_PRELIT means the light value comes from the vertex color attribute
                uint calculated_lit_value = uint(v_color) / u_material_shade_table_height;
                out_palette_index = texelFetch(u_material_shade_table, ivec2(texel, calculated_lit_value), 0).r;
            } else {
                // TODO: lighting calculations based on https://rr2000.cwaboard.co.uk/R4/BRENDER/TEBK_43.HTM#0
                uint calculated_lit_value = 0u;
                out_palette_index = texelFetch(u_material_shade_table, ivec2(texel, calculated_lit_value), 0).r;
            }
        } else {
            // no lighting
            out_palette_index = texel;
        }
    }

    if (u_material_blend_enabled == 1u) {
        // u_colour_buffer is upside down from opengl perspective. We need to sample it upside down.
        int i = int(gl_FragCoord.x);
        int i2 = int(u_viewport_height) - int(gl_FragCoord.y);
        ivec2 coords = ivec2(int(gl_FragCoord.x), i2);
        uint current_framebuffer_color = texelFetch(u_colour_buffer, coords, 0).r;

        // uint current_framebuffer_color = texelFetch(u_colour_buffer, ivec2(gl_FragCoord.x, int(u_viewport_height - gl_FragCoord.y)), 0).r;
        out_palette_index = texelFetch(u_material_blend_table, ivec2(out_palette_index, current_framebuffer_color), 0).r;
    }

    // HACK: Pick another black color instead of palette index 0 so we can detect which pixels have been drawn this frame in FlushBuffers
    if (out_palette_index == 0u) {
        out_palette_index = 240u;
    }
}
