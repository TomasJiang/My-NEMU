#include "cpu/exec/helper.h"

// if above
// => CF = 0 and ZF = 0

make_helper(ja_rel_b) {
    int32_t k = instr_fetch(cpu.eip + 1, 1);
	swaddr_t temp = cpu.eip + (k << 24 >> 24);
	if(!cpu.eflags.CF && !cpu.eflags.ZF) {
		cpu.eip = temp;
	}
	print_asm("ja 0x%x", temp + 2);
	return 2;
}

make_helper(ja_rel_w) {
    int32_t k = instr_fetch(cpu.eip + 1, 2);
	swaddr_t temp = cpu.eip + (k << 16 >> 16);
	if(!cpu.eflags.SF && !cpu.eflags.ZF) {
		cpu.eip = temp;
	}
	print_asm("ja 0x%x", temp + 4);
	return 3;
}

make_helper(ja_rel_l) {
    int32_t k = instr_fetch(cpu.eip + 1, 4);
    Log("%d\t0x%x", k, k);
	swaddr_t temp = cpu.eip + k;
	if(!cpu.eflags.SF && !cpu.eflags.ZF) {
		cpu.eip = temp;
	}
	print_asm("ja 0x%x", temp + 6);
	return 5;
}

make_helper_v(ja_rel)
