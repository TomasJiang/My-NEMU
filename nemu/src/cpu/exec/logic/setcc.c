#include "cpu/exec/helper.h"

#define instr setb
#define cond (cpu.eflags.CF)
#include "setcc-template.h"
#undef cond
#undef instr

#define instr sete
#define cond (cpu.eflags.ZF)
#include "setcc-template.h"
#undef cond
#undef instr

#define instr setne
#define cond (!cpu.eflags.ZF)
#include "setcc-template.h"
#undef cond
#undef instr

#define instr seto
#define cond (cpu.eflags.OF)
#include "setcc-template.h"
#undef cond
#undef instr

#define instr setp
#define cond (cpu.eflags.PF)
#include "setcc-template.h"
#undef cond
#undef instr

#define instr sets
#define cond (cpu.eflags.SF)
#include "setcc-template.h"
#undef cond
#undef instr

