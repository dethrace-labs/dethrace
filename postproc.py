#!/usr/bin/env python3 -u

import sys
import os

def get_codegen_function_body(codegen_lines, function_header):
    print('looking for', function_header)
    for i in range(len(codegen_lines)):
        #print(codegen_lines[i].strip())
        if function_header in codegen_lines[i]:
            lines = []
            #lines.append('// IDA:' + codegen_lines[i-1][6:] + '\n')
            j = 0
            while True:
                j = j + 1
                if codegen_lines[i + j][0] == '}':
                    return lines
                lines.append(codegen_lines[i+j] + '\n')
                
                
                
    print('failed to find', function_header.strip())

def get_codegen_function_ida(codegen_lines, function_header):
    print('looking for', function_header)
    for i in range(len(codegen_lines)):
        if function_header in codegen_lines[i]:
            return '// IDA:' + codegen_lines[i-1][6:] + '\n'
                
    print('failed to find', function_header.strip())


path = 'BRSRC13'
replace_path = 'BRSRC13'

for file_name in sys.argv[1:]:
    f = open(file_name, 'r')
    
    just_file_name = file_name[file_name.index(path)+len(path):]
    print(just_file_name)
    try:
        f2 = open('/Users/jeff/code/c1_codegen/' + replace_path + '/' + just_file_name, 'r')
    except:
        print('WARNING could not load file', just_file_name)
        continue
    codegen_lines = f2.read().splitlines()

    in_function = False
    lines = []
    
    while True:
        line = f.readline()
        if line == '':
            break

        if line[0].isalpha() and '(' in line and line[-2] == '{':
            in_function = True
            function_lines = 0
            function_text = line[0:line.index('(')+1]

            codegen_result = get_codegen_function_ida(codegen_lines, function_text)
            if codegen_result is not None:
                lines.append(codegen_result)
            seen_emptyline = False
            seen_not_implemented = False
        if line[0] == '}' and in_function == True:
            in_function = False
            if seen_not_implemented == True:
                codegen_result = get_codegen_function_body(codegen_lines, function_text)
                if codegen_result is not None:
                    lines = lines[:-function_lines+1]
                    lines.extend(codegen_result)
                lines.append('    NOT_IMPLEMENTED();\n')

        if in_function:
            function_lines = function_lines + 1
            if line == '\n':
                seen_emptyline = True
            if line.strip() == 'NOT_IMPLEMENTED();':
                seen_not_implemented = True
        lines.append(line)

    outF = open(file_name, "w")
    outF.writelines(lines)
    outF.close()

        



