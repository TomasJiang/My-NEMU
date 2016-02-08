#include "cpu/exec/helper.h"

make_helper(je_rel_b) {
    int32_t tmp = instr_fetch(cpu.eip + 1, 1);
    tmp = tmp << 24 >> 24;
    uint32_t target = cpu.eip + tmp;
	if(cpu.eflags.ZF) {
		cpu.eip = target;
	}
	print_asm("je 0x%x", target + 2);
	return 2;
}

make_helper(je_rel_w) {
    int32_t tmp = instr_fetch(cpu.eip + 2, 2);
    tmp = tmp << 16 >> 16;
    uint32_t target = cpu.eip + tmp;
	if(cpu.eflags.ZF) {
		cpu.eip = target;
	}
	print_asm("je 0x%x", target + 4);
	return 3;
}

make_helper(je_rel_l) {
    int32_t tmp = instr_fetch(cpu.eip + 2, 4);
    // Log("tmp = 0x%x", tmp);
    uint32_t target = cpu.eip + tmp;
	if(cpu.eflags.ZF) {
		cpu.eip = target;
	}
	print_asm("je 0x%x", target + 6);
	return 5;
}

make_helper_v(je_rel)
