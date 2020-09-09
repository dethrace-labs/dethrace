#!/usr/bin/env python3 -u

# Used to comment each line of a struct variable with the name of the associated field.
# For example:
# tInterface_spec s = {
#     0,		// initial_imode
#     0,		// first_opening_flic
# };
#
# Usage: 
# `tools/comment_struct_var.py 'tInterface_spec s = {
#     0,
#     0,
# };`

import sys

struct_var = sys.argv[1]
lines = struct_var.split('\n')

var_header = lines[0]
tokens = var_header.split(' ')
struct_name = tokens[0]

f = open("src/DETHRACE/dr_types.h", 'r')

match_line = 'typedef struct {0} {{'.format(struct_name)

found = False
while True:
    l = f.readline()
    if l == "":
        break

    if l.strip() == match_line:
        found = True
        break

if found == False:
    print('Couldnt find struct in dr_types.h. Looking for', match_line)
    sys.exit(1)

print(var_header.strip())

for i in range(1, len(lines)):
    var_line = lines[i]
    def_line = f.readline().strip()

    if '};' in var_line:
        print(var_line)
        break

    # comment_index = line.find('//')
    # if comment_index > 0:
    #     line = line[:comment_index]

    parts = def_line.split(' ')
    name = parts[1]
    array_index = name.find('[')
    if array_index > 0:
        name = name[0: array_index]

    if name.startswith('(*'):
        name = name[2:]
        paren_index = name.find(')')
        if paren_index > 0:
            name = name[0:paren_index]
    
    if ';' in name:
        name = name[:-1]

    print(var_line, '\t\t//', name)
