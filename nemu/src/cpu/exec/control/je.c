#include "cpu/exec/helper.h"

make_helper(je_rel8) {
	swaddr_t temp = cpu.eip + instr_fetch(cpu.eip + 1, 1);
	if(cpu.eflags.ZF) {
		cpu.eip = temp;
	}
	print_asm("je 0x%x", temp);
	return 2;
}
