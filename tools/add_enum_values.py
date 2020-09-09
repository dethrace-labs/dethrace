#!/usr/bin/env python3 -u

import sys

input = sys.argv[1]
values = input.split('\n')
i = 0
for v in values:
    v = v.replace(',', '')
    print(v.strip(), '=', i, ',', '// ', hex(i))
    i = i + 1



