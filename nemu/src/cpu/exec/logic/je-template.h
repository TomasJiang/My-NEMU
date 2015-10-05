#include "cpu/exec/template-start.h"

#define instr je

static void do_execute() {
	if(cpu.eflags.ZF == 1) {
	cpu.eip += instr_fetch(cpu.eip + 1, DATA_BYTE);
	if(DATA_BYTE == 2)
		cpu.eip &= 0xffff;
	}
}

make_instr_helper(i2a)

#include "cpu/exec/template-end.h"
