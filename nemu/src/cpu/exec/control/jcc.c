#include "cpu/exec/helper.h"

// if above
// => CF = 0 and ZF = 0

#define instr ja
#define cond (!cpu.eflags.CF && !cpu.eflags.ZF)
#include "jcc-template.h"
#undef cond
#undef instr

