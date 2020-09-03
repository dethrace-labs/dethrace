#!/usr/bin/env python3 -u

# Used to discover the key referenced in `KeyIsDown` calls.
# Input: keymap index
# Output: keycode index

import os
import sys

f = open(os.environ['DETHRACE_ROOT_DIR'] + '/data/KEYMAP_0.TXT')
content = f.readlines()
f.close()

keymap_code = int(sys.argv[1])
print(content[keymap_code].strip())



