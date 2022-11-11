#version 140
#extension GL_ARB_explicit_attrib_location:require

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_color;
layout (location = 2) in vec2 a_uv;

out vec2 v_tex_coord;

void main() {
   gl_Position = vec4(a_pos, 1.0);
   v_tex_coord = a_uv;
}
