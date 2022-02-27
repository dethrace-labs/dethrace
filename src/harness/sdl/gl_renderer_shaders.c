
const char* vs_2d = "#version 330 core\n"
                    "layout (location = 0) in vec3 aPos;\n"
                    "layout (location = 1) in vec3 aColor;\n"
                    "layout (location = 2) in vec2 aTexCoord;\n"
                    "out vec2 TexCoord;\n"
                    "void main()\n"
                    "{\n"
                    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                    "   TexCoord = aTexCoord;\n"
                    "}\0";

const char* fs_2d = "#version 330 core\n"
                    "in vec2 TexCoord;\n"
                    "uniform usampler2D pixels;\n"
                    "uniform sampler2D palette;\n"
                    "layout (location = 0) out vec4 FragColor;\n"

                    "void main()\n"
                    "{\n"
                    "   uint palette_index = texture(pixels, TexCoord.xy).x;\n"
                    "  vec4 texel = texelFetch(palette, ivec2(palette_index, 0), 0);\n"
                    "  FragColor = texel;\n"
                    "}\n\0";

const char* vs_3d = "#version 330 core\n"
                    "layout (location = 0) in vec3 aPos;\n"
                    "layout (location = 1) in vec3 aNormal;\n"
                    "layout (location = 2) in vec2 aUV;\n"
                    "\n"
                    "out vec3 FragPos;\n"
                    "out vec3 Normal;\n"
                    "out vec2 TexCoord;\n"
                    "\n"
                    "uniform mat4 model;\n"
                    "uniform mat4 view;\n"
                    "uniform mat4 projection;\n"
                    "void main()\n"
                    "{\n"
                    "   FragPos = vec3(model * vec4(aPos, 1.0));\n"
                    "   Normal = aNormal;\n"
                    "   TexCoord = aUV;\n"
                    "   gl_Position = projection * view * vec4(FragPos, 1.0);\n"
                    "}\0";

const char* fs_3d = "#version 330 core\n"
                    "in vec3 Normal;\n"
                    "in vec3 FragPos;\n"
                    "in vec2 TexCoord;\n"
                    "\n"
                    "out uint palette_index;\n"
                    "\n"
                    "uniform usampler2D pixels;\n"
                    "uniform usampler2D shade_table;\n"
                    "uniform int palette_index_override = -1;\n"
                    "uniform int light_value = -1;\n"
                    "uniform vec4 clip_planes[6];\n"
                    "uniform int clip_plane_count = 0;\n"
                    "void main()\n"
                    "{\n"
                    "  for(int i=0; i<clip_plane_count; i++) {\n"
                    "      // calculate signed plane-vertex distance\n"
                    "      vec4 v4 = vec4(FragPos.x, FragPos.y, FragPos.z, 1);\n"
                    "      float d = dot(clip_planes[i], v4);\n"
                    "      if(d < 0.0) discard;\n"
                    "  }\n"
                    "    if (palette_index_override == -1) {"
                    "       uint texel = texture(pixels, TexCoord.xy).x;\n"
                    "       if (light_value == -1) {\n"
                    "           palette_index = texel;\n"
                    "       } else {\n"
                    "           palette_index = texelFetch(shade_table, ivec2(texel, light_value), 0).x;\n"
                    "       }\n"
                    "       if (palette_index == 0u) {\n"
                    "           discard;\n"
                    "       }\n"
                    "    }\n"
                    "    else {\n"
                    "        palette_index = uint(palette_index_override);\n"
                    "    }\n"
                    "}\n\0";
