#include "cpu/exec/helper.h"

make_helper(jne_rel_b) {
    int32_t tmp = instr_fetch(cpu.eip + 1, 1);
    tmp = tmp << 24 >> 24;
    uint32_t target = cpu.eip + tmp;
	if(!cpu.eflags.ZF) {
		cpu.eip = target;
	}
	print_asm("jne 0x%x", target + 2);
	return 2;
}
