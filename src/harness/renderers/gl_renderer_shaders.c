
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
                    "uniform sampler2D ourTexture;\n"
                    "layout (location = 0) out vec4 FragColor;\n"

                    "void main()\n"
                    "{\n"
                    "   FragColor = texture(ourTexture, TexCoord);\n"
                    "   FragColor.a = 1.0;\n"
                    "}\n\0";

const char* fs_2d_trans = "#version 330 core\n"
                          "in vec2 TexCoord;\n"
                          "uniform sampler2D ourTexture;\n"
                          "layout (location = 0) out vec4 FragColor;\n"

                          "void main()\n"
                          "{\n"
                          "   FragColor = texture(ourTexture, TexCoord);\n"
                          "   FragColor.a = 1.0;\n"
                          "   if (FragColor.r == 0 && FragColor.g == 0 && FragColor.b == 0) {\n"
                          "     discard;\n"
                          "   }\n"
                          "}\n\0";

const char* fs_3d = "#version 330 core\n"
                    "in vec3 fragmentColor;\n"
                    "out vec4 color;\n"
                    "void main()\n"
                    "{\n"
                    "   color = vec4(fragmentColor, 1);\n"
                    "}\n\0";

const char* vs_3d = "#version 330 core\n"
                    "layout (location = 0) in vec3 aPos;\n"
                    "layout (location = 1) in vec3 aColor;\n"
                    "out vec3 fragmentColor;\n"
                    "uniform mat4 MVP;\n"
                    "void main()\n"
                    "{\n"
                    "   gl_Position = MVP * vec4(aPos, 1);\n"
                    "   fragmentColor = aColor;\n"
                    "}\0";