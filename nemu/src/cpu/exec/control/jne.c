#include "cpu/exec/helper.h"

make_helper(jne_rel_b) {
	swaddr_t temp = cpu.eip + instr_fetch(cpu.eip + 1, 1);
	if(!cpu.eflags.ZF) {
		cpu.eip = temp;
	}
	print_asm("jne 0x%x", temp + 2);
	return 2;
}
