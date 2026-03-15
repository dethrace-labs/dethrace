# Matching assembly

We want to generate assembly that matches the original retail binary.

Each function has an annotation describing the memory location of this function in the original binary.

## Rules

- You cannot declare new variables or rename, reorder or remove their declarations
- You cannot change the function signature
- We want c code that would be as close to what the original developers wrote, so avoid `volatile` casts and unnatural c code
- You cannot change any code except in the function that we are trying to match
- If the stack size is different (the sub esp, [size]), then abort trying to match
- Don't add unecessary casts
- Don't add goto statements or goto labels
- Don't use any other tools except `dr-reccmp`

## Stack variable slots

Each .c file that we work on has a corresponding .asm file in build\*msvc42 which you can read to discover the local variable slots so that you can discover that [ebp-4] is "i" for example. You _are_ allowed to swap variable _usage_ (not declarations) to match the original variable slots.

## To see what assembly is different

The `asm-match.sh` script should be invoked with a single argument `asm-match.sh [function memory location]`. It will compile our code and then compare this function to the original. The output will show assembly-level diff.

- "+" lines show where we have code that the original doesnt have, so we should try and remove it
- "-" lines show code that is in the retail binary but not in our recompiled binary, so we should try and add it
- often, the code is just moved, so we would see the same line twice with a "-" in one and a "+" in another

Continue making changes and running the command until it shows a 100% match or you run out of ideas.

## Aborting

Abort if the only difference is a swapped ordering of a compare and small associated jmp difference. For example:

```
0x448cdb	-mov eax, dword ptr [ebp - 4]
0x448cde	-cmp dword ptr [gNumber_of_net_players (DATA)], eax
0x448ce4	-jle 0x69
+mov eax, dword ptr [gNumber_of_net_players (DATA)]
+cmp dword ptr [ebp - 4], eax
+jge 0x69
...
0x448d4e	-jmp -0x7b
+jmp -0x7a (network.c:1754)
```

This is caused by compiler entropy and not something you will be able to resolve.
