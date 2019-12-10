# Watcom symbol dump codegen tools

## codegen.py

Takes an [exedump](https://github.com/jeff-1amstudios/open-watcom-v2/tree/master/bld/exedump) output file and generates skeleton "c" project files containing functions, structs, enums and global variables.

Was used to generate the first commits of [src](../../src) directory.

Example output:
```c

typedef enum tPowerup_type {
    ePowerup_dummy = 0,
    ePowerup_instantaneous = 1,
    ePowerup_timed = 2,
    ePowerup_whole_race = 3
} tPowerup_type;

struct tPedestrian_action {
    float danger_level;
    float percentage_chance;
    int number_of_bearings;
    int number_of_sounds;
    int sounds[3];
    tBearing_sequence sequences[7];
    float initial_speed;
    float looping_speed;
    tU32 reaction_time;
};

// Offset: 9548
// Size: 1603
// EAX: c
void CalcEngineForce(tCar_spec *c, br_scalar dt) {
    br_scalar torque;
    br_scalar ts;
    br_scalar ts2;
    br_scalar brake_temp;
    int sign;
    tS32 temp_for_swap;
}
```

### Usage

1. Concatenate the executable file and the symbol file
```
cat carma1.exe dethrace.sym > carma_with_symbols.exe
```

2. Execute wdump and pipe the output to file
```
wdump -Daglmt carma_with_symbols.exe > dump.txt
```

3. Execute the codegen tool
```
codegen.py dump.txt
```

You now have the C skeleton files in `./_generated`.


## split-dump.sh
Takes a [exedump](https://github.com/jeff-1amstudios/open-watcom-v2/tree/master/bld/exedump) output file and splits it into a single file per module for easier debugging.

