#include "dr_types.h"
#include "br_types.h"
// Offset: 10
// Size: 118
void BrFailure(char *s, ...);

// Offset: 138
// Size: 118
void BrWarning(char *s, ...);

// Offset: 264
// Size: 132
void BrFatal(char *name, int line, char *s, ...);

// Offset: 406
// Size: 95
void _BrAssert(char *condition, char *file, unsigned int line);

// Offset: 512
// Size: 95
void _BrUAssert(char *condition, char *file, unsigned int line);

