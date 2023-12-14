layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_uv;
layout (location = 3) in float a_color;

out vec3 v_frag_pos;
out vec3 v_normal;
out vec2 v_tex_coord;
out float v_color;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform uint u_material_flags;
uniform mat4 u_normal_matrix;

// material_flags values
const uint BR_MATF_ENVIRONMENT_L = 0x10u;

// https://rr2000.cwaboard.co.uk/R4/BRENDER/TEBK_43.HTM#HEADING331
// https://www.clicktorelease.com/blog/creating-spherical-environment-mapping-shader/
void do_spherical_environment_map(void) {
    mat4 view_model = u_view * u_model;
    vec3 e = normalize( vec3( view_model * vec4( a_pos, 1.0 ) ) );
    vec3 n = normalize( mat3(u_normal_matrix) * a_normal );
    vec3 r = reflect( e, n );
    float m = 2. * sqrt( pow( r.x, 2. ) + pow( r.y, 2. ) + pow( r.z + 1., 2. ) );
    v_tex_coord = r.xy / m + .5;
}

void main(void) {
    v_frag_pos = vec3(u_model * vec4(a_pos, 1.0));
    v_normal = a_normal;
    v_tex_coord = a_uv;
    v_color = a_color;
    gl_Position = u_projection * u_view * vec4(v_frag_pos, 1.0);

    if ((u_material_flags & BR_MATF_ENVIRONMENT_L) != 0u) {
        do_spherical_environment_map();
    }
}
