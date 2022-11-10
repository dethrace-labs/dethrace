#version 140
#extension GL_ARB_explicit_attrib_location : require
in vec2 TexCoord;
uniform usampler2D pixels;

uniform sampler2D palette;
layout (location = 0) out vec4 FragColor;

void main() {
   uint palette_index = texture(pixels, TexCoord.xy).x;
  vec4 texel = texelFetch(palette, ivec2(palette_index, 0), 0);
  FragColor = texel;
}
