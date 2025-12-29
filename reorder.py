#!/usr/bin/env python3 -u

import sys
import os

# Usage: find src/DETHRACE -name "*.c" | xargs ./reorder.py

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
    codegen_gvs = []
    while True:
        line = codegen_f.readline()

        if ';' in line or '{' in line:
            in_gvs = True
        
        if line.strip() == '':
            if in_gvs == True:
                in_gvs = False
                break
        
        if in_gvs == True:
            codegen_gvs.append(line)

    i = 0
    lines = f.readlines()
    while True:
        line = lines[i]
        if ';' in line or '{' in line:
            for codegen_gv in codegen_gvs:
                lines.insert(i, codegen_gv)
                i = i + 1
            lines.insert(i, "\n\n// END OF NEW ORDERING\n\n")
            break
        i = i + 1



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

        



