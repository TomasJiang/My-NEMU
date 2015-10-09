#include "cpu/exec/helper.h"

make_helper(je_rel8) {
	Log("eip = 0x%x", cpu.eip);
	if(cpu.eflags.ZF) {
		cpu.eip += instr_fetch(cpu.eip + 1, 1);
		Log("eip = 0x%x", cpu.eip);
	}
	return 2;
}
