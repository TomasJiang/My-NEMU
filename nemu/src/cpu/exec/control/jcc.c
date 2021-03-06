#include "cpu/exec/helper.h"


#define instr ja
#define cond (!cpu.eflags.CF && !cpu.eflags.ZF)
#include "jcc-template.h"
#undef cond
#undef instr

#define instr jbe
#define cond (cpu.eflags.CF || cpu.eflags.ZF)
#include "jcc-template.h"
#undef cond
#undef instr

#define instr je
#define cond (cpu.eflags.ZF)
#include "jcc-template.h"
#undef cond
#undef instr

#define instr jg
#define cond (!cpu.eflags.ZF && cpu.eflags.SF == cpu.eflags.OF)
#include "jcc-template.h"
#undef cond
#undef instr

#define instr jge
#define cond (cpu.eflags.ZF || cpu.eflags.SF == cpu.eflags.OF)
#include "jcc-template.h"
#undef cond
#undef instr

#define instr jl
#define cond (cpu.eflags.SF != cpu.eflags.OF)
#include "jcc-template.h"
#undef cond
#undef instr

#define instr jle
#define cond (cpu.eflags.ZF || cpu.eflags.SF != cpu.eflags.OF)
#include "jcc-template.h"
#undef cond
#undef instr

#define instr jnc
#define cond (!cpu.eflags.CF)
#include "jcc-template.h"
#undef cond
#undef instr

#define instr jne
#define cond (!cpu.eflags.ZF)
#include "jcc-template.h"
#undef cond
#undef instr

#define instr jno
#define cond (!cpu.eflags.OF)
#include "jcc-template.h"
#undef cond
#undef instr

#define instr jnp
#define cond (!cpu.eflags.PF)
#include "jcc-template.h"
#undef cond
#undef instr

#define instr jns
#define cond (!cpu.eflags.SF)
#include "jcc-template.h"
#undef cond
#undef instr

#define instr js
#define cond (cpu.eflags.SF)
#include "jcc-template.h"
#undef cond
#undef instr

