#!/usr/bin/env python3
"""Extract stack slot definitions for a given function from MSVC assembly files."""

import argparse
import glob
import re
import subprocess
import sys
from pathlib import Path
from typing import Optional


def find_function_file(function_name: str, build_dir: Path) -> Optional[Path]:
    """Find which .asm file contains the given function."""
    search_pattern = f"_{function_name}.*PROC"
    asm_files = glob.glob(str(build_dir / "*.asm"))

    if not asm_files:
        return None

    try:
        result = subprocess.run(
            ["grep", "-l", search_pattern] + asm_files,
            capture_output=True,
            text=True,
        )

        if result.returncode == 0 and result.stdout.strip():
            # Return first matching file
            return Path(result.stdout.strip().split('\n')[0])

        return None
    except Exception:
        return None


def extract_stack_slots(asm_file: Path, function_name: str) -> list[str]:
    """Extract stack slot definitions for the given function."""
    proc_pattern = f"_{function_name}"
    slot_pattern = re.compile(r'^(_[\w$]+) = (-?\d+)$')

    with open(asm_file, 'r') as f:
        lines = f.readlines()

    # Find the PROC line
    proc_line_idx = None
    for idx, line in enumerate(lines):
        if proc_pattern in line:
            proc_line_idx = idx
            break

    if proc_line_idx is None:
        return []

    # Read backwards from PROC line to collect stack slots
    stack_slots = []
    for idx in range(proc_line_idx - 1, -1, -1):
        line = lines[idx].strip()

        # Stop conditions
        if "_TEXT SEGMENT" in line:
            break
        if "PROC" in line and proc_pattern not in line:
            break

        # Check if line matches stack slot pattern
        match = slot_pattern.match(line)
        if match:
            var_name = match.group(1)
            offset = int(match.group(2))

            # Remove leading underscore and trailing $ from variable name
            clean_var_name = var_name.lstrip('_').rstrip('$')

            # Convert to hex format like [ebp - 0xc] or [ebp + 0x8]
            if offset < 0:
                hex_str = f"[ebp - {hex(-offset)}]"
            else:
                hex_str = f"[ebp + {hex(offset)}]"

            stack_slots.append((offset, f"{clean_var_name} = {hex_str}"))
        elif line and not line.startswith(';'):
            # Non-empty, non-comment line that doesn't match pattern - stop
            # But allow empty lines and comments to continue
            if not (line == "" or line.startswith(";")):
                break

    # Sort by offset (highest first, i.e., positive offsets first, then negative from highest to lowest)
    stack_slots.sort(key=lambda x: x[0], reverse=True)

    # Return just the formatted strings
    return [slot[1] for slot in stack_slots]


def main():
    parser = argparse.ArgumentParser(
        description="Extract stack slot definitions for a function from MSVC assembly"
    )
    parser.add_argument("function_name", help="Name of the function (without leading underscore)")
    args = parser.parse_args()

    build_dir = Path("build_msvc42")

    if not build_dir.exists():
        print(f"Error: {build_dir} directory not found", file=sys.stderr)
        return 1

    # Find which file contains the function
    asm_file = find_function_file(args.function_name, build_dir)

    if asm_file is None:
        print(f"Error: Function '{args.function_name}' not found in {build_dir}", file=sys.stderr)
        return 1

    # Extract stack slots
    stack_slots = extract_stack_slots(asm_file, args.function_name)

    # Output to stdout
    for slot in stack_slots:
        print(slot)

    return 0


if __name__ == "__main__":
    sys.exit(main())
