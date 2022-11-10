#version 140
#extension GL_ARB_explicit_attrib_location : require
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

out uint palette_index;

uniform mat3x2 uv_transform;
//uniform vec3 uv_transform[2];
uniform usampler2D pixels;
uniform usampler2D shade_table;
uniform int palette_index_override = -1;
uniform int light_value = -1;
uniform vec4 clip_planes[6];
uniform int clip_plane_count = 0;

void main() {

    for(int i=0; i<clip_plane_count; i++) {
        // calculate signed plane-vertex distance
        vec4 v4 = vec4(FragPos.x, FragPos.y, FragPos.z, 1);
        float d = dot(clip_planes[i], v4);
        if(d < 0.0) discard;
    }

    if (palette_index_override == -1) {
        //vec3 sample_coord = mat3(pixels_transform[0], pixels_transform[1], vec3(0, 0, 1)) * vec3(TexCoord.xy, 1);
        //vec3 sample_coord = vec3(TexCoord.xy, 1) * mat3(uv_transform[0], uv_transform[1], vec3(0, 0, 1));
        vec3 sample_coord = vec2(TexCoord.xy) * uv_transform;
        uint texel = texture(pixels, sample_coord.xy).x;
        if (light_value == -1) {
            palette_index = texel;
        } else {
            palette_index = texelFetch(shade_table, ivec2(texel, light_value), 0).x;
        }
        if (palette_index == 0u) {
            discard;
        }
    }
    else {
        palette_index = uint(palette_index_override);
    }
}
