#include "cpu/exec/helper.h"

make_helper(jne_rel8) {
	if(!cpu.eflags.ZF) {
		int32_t temp = instr_fetch(cpu.eip + 1, 1);
		cpu.eip += temp << 24 >> 24;
	}
	print_asm("jne 0x%x", cpu.eip + 2);

	return 2;
}
