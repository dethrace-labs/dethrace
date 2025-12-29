#!/usr/bin/env python3 -u

import sys
import os

# Usage: find src/DETHRACE -name "*.h" | xargs ./extern.py

path = 'DETHRACE/'
codegen_path = '/Users/jeff/temp/c1_output2/DETHRACE/source'

for file_name in sys.argv[1:]:
    # print (file_name)
    # continue

    f = open(file_name, 'r')
    
    just_file_name = file_name[file_name.index(path)+len(path):]
    
    try:
        codegen_f = open(codegen_path + '/' + just_file_name, 'r')
    except:
        print('WARNING could not load file', codegen_path + '/' + just_file_name)
        continue

    in_gvs = False
    stripped_existing = False
    codegen_gvs = []
    while True:
        line = codegen_f.readline()

        if line.startswith('extern'):
            codegen_gvs.append(line)

        if line == '':
            break

    appended = False
    lines = []
    while True:
        line = f.readline()

        if line == '':
            break

        # first function decl
        if not appended:
            if (line != '' and line[0].isalpha() and '(' in line and line.endswith(');\n')
                or line == '#endif\n'):
                if stripped_existing == True:
                    print('pop')
                    lines.pop()  # remove the extra newline
                for codegen_gv in codegen_gvs:
                    lines.append(codegen_gv)
                lines.append('\n')
                appended = True

        if not line.startswith('extern'):
            lines.append(line)
        else:
            stripped_existing = True

    print(lines)
    # in_function = False
    # lines = []
    
    # while True:
    #     line = f.readline()
    #     if line == '':
    #         break

    #     if line[0].isalpha() and '(' in line and line[-2] == '{':
    #         in_function = True
            
    outF = open(file_name, "w")
    outF.writelines(lines)
    outF.close()

        



