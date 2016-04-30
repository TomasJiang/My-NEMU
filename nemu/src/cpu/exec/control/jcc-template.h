
#define DATA_BYTE 1
#include "jcc-subtemplate.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jcc-subtemplate.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jcc-subtemplate.h"
#undef DATA_BYTE

make_helper_v(instr)
