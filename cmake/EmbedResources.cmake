# EmbedResources.cmake
# Jeff / 1amstudios.com

####################################################################################################

function(write_header output_file_name)
    file(WRITE "${output_file_name}" "
#include <string.h>

typedef struct embedded_resource_t {
    const char *name;
    int len;
    const char *content;
} embedded_resource_t;

")
endfunction()

function(embed_resource resource_file_name source_file_name variable_name)
    # hex functions by amir-saniyan https://gist.github.com/amir-saniyan/de99cee82fa9d8d615bb69f3f53b6004

    file(READ "${SOURCE_DIR}/${resource_file_name}" hex_content HEX)
    string(REPEAT "[0-9a-f]" 32 pattern)
    string(REGEX REPLACE "(${pattern})" "\\1\n" content "${hex_content}")
    string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1, " content "${content}")
    string(REGEX REPLACE ", $" "" content "${content}")
    set(array_definition "static const char ${variable_name}[] =\n{\n${content}\n};\n")
    file(APPEND "${source_file_name}" "${array_definition}")
endfunction()


function(embed_resource_ref resource_file_name source_file_name variable_name)
    file(APPEND "${source_file_name}" "  { .name = \"${resource_file_name}\", .len = sizeof(${variable_name}), .content = ${variable_name} },\n")
endfunction()

function (write_footer output_file_name)
    file(APPEND "${output_file_name}" "
};

embedded_resource_t *get_embedded_resource_by_name(char *name) {
  unsigned int i;
  for (i = 0; i < sizeof(embedded_resources) / sizeof(embedded_resources[0]); i++) {
    if (strcmp(name, embedded_resources[i].name) == 0) {
        return &embedded_resources[i];
    }
  }
  return NULL;
}
")

endfunction()

####################################################################################################

if(NOT DEFINED RESOURCES_GLOB)
    message(FATAL_ERROR "RESOURCES_GLOB is not set")
endif(NOT DEFINED RESOURCES_GLOB)

file(GLOB resource_files RELATIVE ${SOURCE_DIR} "${SOURCE_DIR}/${RESOURCES_GLOB}")

write_header("embedded_resources.h")

# Add binary content as char array
foreach(elem ${resource_files})
    message("Embedding ${elem}")
    string(REPLACE "." "_" generated_name "${elem}")
    string(REPLACE "/" "_" generated_name "${generated_name}")
    list(APPEND variable_names "${generated_name}")
    embed_resource("${elem}" "embedded_resources.h" "${generated_name}")
endforeach()

# Add lookup table from original filename to content
file(APPEND "embedded_resources.h" "
embedded_resource_t embedded_resources[] = {
")

foreach(elem ${resource_files})
    string(REPLACE "." "_" generated_name "${elem}")
    string(REPLACE "/" "_" generated_name "${generated_name}")
    embed_resource_ref("${elem}" "embedded_resources.h" "${generated_name}")
endforeach()

write_footer("embedded_resources.h")
