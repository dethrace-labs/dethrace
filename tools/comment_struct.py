#!/usr/bin/env python3 -u

f = open("struct_decl", 'r')
f2 = open("struct_def", 'r')

line = f.readline()
line2 = f2.readline()

print(line, end='')
while True:
        line = f.readline()
        line2 = f2.readline()
        if line == '':
            print('error, didnt see end of struct')
            break
        if line2 == '':
            print('error, files should have same number of lines')
            break
        
        if '};' in line:
            print(line)
            break

        line = line.rstrip()
        line2 = line2.strip()

        comment_index = line.find('//')
        if comment_index > 0:
            line = line[:comment_index]

        parts = line2.split(' ')
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

        

        print(line, '\t\t//', name)