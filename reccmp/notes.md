# Decomp notes

The original code generally prefers:

1. ">" instead of "<=" and vice versa
2. Generally nested if statements instead of early returns
3. For loops like `(i = 0, ptr = start_of_array; condition; i++, ptr++)`

In a for loop, a pattern lile this indicates something like

```asm
jne 0x5
jmp -0x10a
```

```
if (1) {
    continue;
}
```

Pulling a local variable into a compiler-generated variable, then a set of cmp/jmp's pointing to code
locations above means a switch. The ordering of the cmp/jmp appears to be numerically sorted, regardless of
the ordering in the origina C code

For example, this came from a switch statement like

```
switch (version) {

    case '2':
    case '3':
    case '4':
        ...
    case '1':
        ...
        break;
    }
```

```
.text:0041F567 loc_41F567:                             ; CODE XREF: ReadMechanicsData+680↑j
.text:0041F567                 cmp     [ebp+var_1A8], 31h ; '1'
.text:0041F56E                 jz      loc_41F4F2
.text:0041F574                 cmp     [ebp+var_1A8], 32h ; '2'
.text:0041F57B                 jl      loc_41F593
.text:0041F581                 cmp     [ebp+var_1A8], 34h ; '4'
.text:0041F588                 jle     loc_41F480
.text:0041F58E                 jmp     $+5
```
