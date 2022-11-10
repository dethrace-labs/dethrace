# EmbedResource.cmake

####################################################################################################
# original function by amir-saniyan https://gist.github.com/amir-saniyan/de99cee82fa9d8d615bb69f3f53b6004
function(embed_resource resource_file_name source_file_name variable_name)

if(EXISTS "${source_file_name}")
    if("${source_file_name}" IS_NEWER_THAN "${resource_file_name}")
        return()
    endif()
endif()

message("Updating ${resource_file_name} embeded resource")

file(READ "${resource_file_name}" hex_content HEX)

string(REPEAT "[0-9a-f]" 32 pattern)
string(REGEX REPLACE "(${pattern})" "\\1\n" content "${hex_content}")
string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1, " content "${content}")
string(REGEX REPLACE ", $" "" content "${content}")
set(array_definition "static const char ${variable_name}[] =\n{\n${content}\n};")
set(source "// Auto generated file.\n${array_definition}\n")
file(WRITE "${source_file_name}" "${source}")

endfunction()
####################################################################################################


if(NOT DEFINED SOURCE_DIR)
    message(FATAL_ERROR "SOURCE_DIR is not set")
endif(NOT DEFINED SOURCE_DIR)

if(NOT DEFINED FILE)
    message(FATAL_ERROR "FILE is not set")
endif(NOT DEFINED FILE)

string(REPLACE "." "_" generated_name "${FILE}")
string(REPLACE "/" "_" generated_name "${generated_name}")
string(TOUPPER "${generated_name}" generated_name)
embed_resource("${SOURCE_DIR}/${FILE}" "${FILE}.h" "${generated_name}")
