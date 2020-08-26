#!/usr/bin/env python3 -u

# Used to discover the key referenced in `KeyIsDown` calls.
# Input: keymap index
# Output: keycode index

import sys

input = sys.argv[1]
values = input.split('\n')
i = 0
for v in values:
    v = v.replace(',', '')
    print(v.strip(), '=', i, ',', '// ', hex(i))
    i = i + 1



