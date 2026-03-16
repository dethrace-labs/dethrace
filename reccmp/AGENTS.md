# Matching assembly

We want to generate assembly that matches the original retail binary. Each function has an annotation describing the memory location of this function in the original binary.

## Rules

- You cannot declare new variables or rename, reorder or remove their declarations
- You cannot change the function signature
- We want c code that would be as close to what the original developers wrote, so avoid `volatile` casts and unnatural c code
- You cannot change any code except in the function that we are trying to match
- If the stack size is different (the sub esp, [size]), then abort trying to match
- Don't add unecessary casts
- Don't add goto statements or goto labels
- Don't look back at previous commits. This code has never matched, so no point in doing this.
- Don't add code that isn't x86/x64 compatible. Don't assume a pointer is 4 bytes. The original binary is x86 only, but our recompiled code needs to work correctly on 64 bit.
- Reversed compare order changes will generally resolve by themselves - leave these till last. They are normally not fixable.
- Inspect the corresponding `build_msvc42/*.asm` function before editing so local variable slots (`[ebp-*]`) are mapped correctly.
- Classify asm diffs before editing: compare-order-only diffs vs semantic/codegen diffs. Prioritize semantic/codegen diffs first.
- In asm diffs, prioritize instruction presence/absence (`+`/`-` lines) before jump-target/offset changes. Jump addresses often self-correct after shape mismatches are fixed.
- When only one semantic/codegen mismatch remains, make one minimal edit targeting that mismatch and rerun reccmp before any other refactor.
- Do not refactor loop/control-flow shape to chase compare-order-only diffs.
- If a trial edit decreases match percentage, revert immediately and return to the last higher-percentage version for that exact trial.
- Do not discard an entire strategy family after one regression; test close companion variants (for example `default: return` vs `default: break` + tail return, and flat vs nested `if/else` scaffolding) before abandoning it.

## Stack variable slots

Each .c file that we work on has a corresponding .asm file in `build_msvc42` which you can read to discover the local variable slots so that you can discover that [ebp-4] is "i" for example. You _are_ allowed to swap variable _usage_ (not declarations) to match the original variable slots.

## To see what assembly is different

The `reccmp/asm-match.sh` script should be invoked with a single argument `asm-match.sh [function memory location]`. It will compile our code and then compare this function to the original. The output will show assembly-level diff.

- "+" lines show where we have code that the original doesnt have, so we should try and remove it
- "-" lines show code that is in the retail binary but not in our recompiled binary, so we should try and add it
- often, the code is just moved, so we would see the same line twice with a "-" in one and a "+" in another

Continue making changes and running the command until it shows a 100% match or you run out of ideas.

## Reccmp Matching Heuristics (important)

- Prefer original control-flow shape over logical cleanup/safety refactors.
- If asm diff shows extra post-loop compare/jump blocks (for example `+mov`/`+cmp` after a `for` loop), avoid `break`-then-check-index patterns; try early `return` in-loop.
- Avoid `do { ... } while (0)` wrappers in target functions unless proven necessary; they often add synthetic jump blocks.
- Do not "fix" leaks or unreachable branches in matching work. Binary-faithful behavior wins over code quality.
- Keep odd or redundant locals and branches if they help codegen, even when logically unnecessary.
- Treat reccmp `+` and `-` lines as assembly-shape guidance, not direct C line add/remove instructions.
- For matching tasks, make one control-flow change at a time and rerun reccmp after each change.
- For enum-based `switch` matching, `default:` can change jump-table targets even when behavior is equivalent.
- A `default: break;` often creates a separate shared break block; retail may instead route missing/unhandled entries directly to function epilogue.
- If diffs show extra or missing epilogue-adjacent blocks (`mov eax`, `pop`, `leave`, `ret`), prioritize matching return-site structure and `if/else` nesting before tuning jump targets.

## Aborting

In some cases, a match is not possible due to compiler entropy. This is more common with floating point instructions. For example:

```
0x404f73	fsub dword ptr [eax + ecx + 8]
0x404f77	fstp dword ptr [ebp - 0x1118]
+fld dword ptr [ebp - 0x111c] (opponent.c:2383)
+fmul dword ptr [ebp - 0x111c]
0x404f7d	fld dword ptr [ebp - 0x1118]
0x404f83	fmul dword ptr [ebp - 0x1118]
+faddp st(1)
0x404f89	fld dword ptr [ebp - 0x1120]
0x404f8f	fmul dword ptr [ebp - 0x1120]
0x404f95	-faddp st(1)
0x404f97	-fld dword ptr [ebp - 0x111c]
0x404f9d	-fmul dword ptr [ebp - 0x111c]
0x404fa3	faddp st(1)
0x404fa5	call __CIsqrt (FUNCTION)
```

We are generally unable to resolve a diff like this, so resolve the other diffs in the function, then abort.
