#include "cpu/exec/helper.h"

make_helper(jne_rel8) {
	int32_t temp = instr_fetch(cpu.eip + 1, 1);
	temp = temp << 24 >> 24;
	if(!cpu.eflags.ZF) {
		cpu.eip += temp;
	}
	print_asm("jne 0x%x", cpu.eip + temp);

	return 2;
}
