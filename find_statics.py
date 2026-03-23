import os
import re

def find_unannotated_statics(dir_path):
    unannotated = []
    for root, dirs, files in os.walk(dir_path):
        for file in files:
            if file.endswith('.c'):
                file_path = os.path.join(root, file)
                with open(file_path, 'r') as f:
                    lines = f.readlines()
                    for i, line in enumerate(lines):
                        stripped = line.strip()
                        if stripped.startswith('static ') or (stripped.startswith('const static ') or stripped.startswith('static const ')):
                            # Check if indented
                            if line.startswith(' ') or line.startswith('\t'):
                                # It's inside a function (likely)
                                # Check preceding lines for annotation
                                annotated = False
                                # Look back up to 2 lines
                                for j in range(1, 3):
                                    if i - j >= 0:
                                        prev_line = lines[i - j].strip()
                                        if "// GLOBAL: CARM95" in prev_line or "// LCL: CARM95" in prev_line or "// OFFSET:" in prev_line:
                                            annotated = True
                                            break
                                        if prev_line == "":
                                            continue
                                        # If we find something else, stop looking further? 
                                        # Actually, let's be more lenient.
                                
                                if not annotated:
                                    unannotated.append((file_path, i + 1, line.strip()))
    return unannotated

results = find_unannotated_statics('src/DETHRACE/common')
for file_path, line_num, line_content in results:
    print(f"{file_path}:{line_num}: {line_content}")
