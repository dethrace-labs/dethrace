import re
import sys

SEGMENT_BASES = {
    '0001': 0x401000,
    '0002': 0x504000,
    '0003': 0x507000,
}

def parse_symbol_table(filename):
    symbols = {}
    in_symbol_table = False
    with open(filename) as f:
        for line in f:
            # Detect start of symbol table
            if 'Address' in line and 'Publics by Value' in line:
                in_symbol_table = True
                continue
            if in_symbol_table:
                # Match lines like: 0001:00000000       AdjustSkid
                m = re.match(r'\s*([0-9A-Fa-f]{4}):([0-9A-Fa-f]{8})\s+(\w+)', line)
                if m:
                    segment = m.group(1)
                    offset = int(m.group(2), 16)
                    name = m.group(3)
                    base = SEGMENT_BASES.get(segment, 0)
                    address = offset + base
                    symbols[name] = (f'0x{address:X}').lower()
    return symbols

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <inputfile>")
        sys.exit(1)
    filename = sys.argv[1]
    for name, addr in parse_symbol_table(filename):
        print(f'{name}: {addr}')

def extract_function_name(line):
    # Match function definitions at the start of a line (not indented), including those with function pointer arguments
    m = re.match(r'^([a-zA-Z_][\w\s\*]*?)\s+([a-zA-Z_]\w*)\s*\((?:[^()]*|\([^()]*\))*\)\s*\{', line)
    if m:
        return m.group(2)
    return None

def extract_global_var_name(line):
    # Match variable declarations at the start of a line (not indented)
    # e.g., int foo = 5; or char* gDamage_names[] = { or tU32 gPanel_flic_data_length[2]; or br_matrix34 gSheer_mat = { or void (*gAdd_proc)(tNet_game_details*);
    m = re.match(r'^[a-zA-Z_][\w\s\*]*\s+([a-zA-Z_]\w*)\s*(=.*)?;', line)
    if m:
        return m.group(1)
    # Match array initializations like char* gDamage_names[] = {
    m = re.match(r'^[a-zA-Z_][\w\s\*]*\s+([a-zA-Z_]\w*)\s*\[.*\]\s*=\s*\{', line)
    if m:
        return m.group(1)
    # Match array declarations like tU32 gPanel_flic_data_length[2];
    m = re.match(r'^[a-zA-Z_][\w\s\*]*\s+([a-zA-Z_]\w*)\s*\[.*\]\s*;', line)
    if m:
        return m.group(1)
    # Match struct/typedef/union initializations like br_matrix34 gSheer_mat = {
    m = re.match(r'^[a-zA-Z_][\w\s\*]*\s+([a-zA-Z_]\w*)\s*=\s*\{', line)
    if m:
        return m.group(1)
    # Match function pointer declarations like void (*gAdd_proc)(tNet_game_details*);
    m = re.match(r'^[a-zA-Z_][\w\s\*]*\(\*([a-zA-Z_]\w*)\)\s*\([^)]*\)\s*;', line)
    if m:
        return m.group(1)
    return None

def extract_static_var_name(line):
    m = re.match(r'^\s*static\s+(?:[\w\*\s]+\s+)(\w+)\s*(=.*)?;', line)
    if m:
        return m.group(1)
    return None


def is_function_def(line):
    return extract_function_name(line) is not None

def is_global_var(line):
    return (
        extract_global_var_name(line) is not None
        and not is_function_def(line)
        and not line.strip().startswith('//')
        and not line.strip().startswith('/*')
        and line.strip() != ''
    )

def main(filename):
    map_syms = parse_symbol_table("/Users/jeff/Downloads/carma/Carma_Win95.map")
    with open(filename) as f:
        lines = f.readlines()

    output = []
    brace_level = 0
    last_ignore_seen = 0
    current_function = ''
    unmatched = []

    for i, line in enumerate(lines):
        if line == "}\n":
            brace_level = 1

        if line.strip() == '// UNUSED':
            last_ignore_seen = i

        prev_line = output[-1] if output else ''

        # Only consider top-level (global) scope
        if brace_level == 0:
            func_name = extract_function_name(line)
            var_name = extract_global_var_name(line)

            if func_name:
                current_function = func_name
                addr = map_syms.get(func_name)
                annotation = f'// FUNCTION: CARM95 {addr}\n'
                if not prev_line.strip().startswith('// FUNCTION: CARM95'):
                    if addr == None and last_ignore_seen != i - 1:
                        unmatched.append(func_name)
                    else:
                        output.append(annotation)
            elif var_name:
                addr = map_syms.get(var_name)
                annotation = f'// GLOBAL: CARM95 {addr}\n'
                if not prev_line.strip().startswith('// GLOBAL: CARM95'):
                    if addr == None and last_ignore_seen != i - 1:
                        unmatched.append(var_name)
                    else:
                        output.append(annotation)
        else:
            static_var_name = extract_static_var_name(line)

            if static_var_name:
                qname = current_function + '_' + static_var_name
                addr = map_syms.get(qname)
                annotation = f'    // GLOBAL: CARM95 {addr}\n'
                if '// GLOBAL: CARM95' not in prev_line:
                    if addr == None and last_ignore_seen != i - 1:
                        unmatched.append(qname)
                    else:
                      output.append(annotation)

        output.append(line)
        # Update brace level
        brace_level += line.count('{') - line.count('}')


    with open(filename, 'w') as f:
        f.writelines(output)

    print("Unmatched in", filename)
    print(unmatched)

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("Usage: python annotate_c_globals.py <file.c>")
        sys.exit(1)

    main(sys.argv[1])
