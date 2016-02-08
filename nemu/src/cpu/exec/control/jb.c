#include "cpu/exec/helper.h"

// if below
// => CF = 1

make_helper(jb_rel_b) {
    int32_t k = instr_fetch(cpu.eip + 1, 1);
	swaddr_t temp = cpu.eip + (k << 24 >> 24);
	if(!cpu.eflags.CF && !cpu.eflags.ZF) {
		cpu.eip = temp;
	}
	print_asm("jb 0x%x", temp + 2);
	return 2;
}

make_helper(jb_rel_w) {
    int32_t k = instr_fetch(cpu.eip + 2, 2);
	swaddr_t temp = cpu.eip + (k << 16 >> 16);
	if(!cpu.eflags.SF && !cpu.eflags.ZF) {
		cpu.eip = temp;
	}
	print_asm("jb 0x%x", temp + 4);
	return 3;
}

make_helper(jb_rel_l) {
    int32_t k = instr_fetch(cpu.eip + 2, 4);
	swaddr_t temp = cpu.eip + k;
	if(!cpu.eflags.SF && !cpu.eflags.ZF) {
		cpu.eip = temp;
	}
	print_asm("jb 0x%x", temp + 6);
	return 5;
}

make_helper_v(jb_rel)
