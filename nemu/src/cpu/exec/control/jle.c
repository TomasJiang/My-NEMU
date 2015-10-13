#include "cpu/exec/helper.h"

make_helper(jle_rel8) {
	swaddr_t temp = cpu.eip + instr_fetch(cpu.eip + 1, 1);
	if(cpu.eflags.ZF && cpu.eflags.SF != cpu.eflags.OF) {
		cpu.eip = temp;
	}
	print_asm("jle 0x%x", temp + 2);
	return 2;
}
