#include "cpu/exec/helper.h"

// if greater
// => ZF = 0 and SF = OF

make_helper(jg_rel_b) {
    int32_t k = instr_fetch(cpu.eip + 1, 1);
	swaddr_t temp = cpu.eip + (k << 24 >> 24);
	if(!cpu.eflags.OF && cpu.eflags.SF == cpu.eflags.OF) {
		cpu.eip = temp;
	}
	print_asm("jg 0x%x", temp + 2);
	return 2;
}

make_helper(jg_rel_w) {
    int32_t k = instr_fetch(cpu.eip + 1, 2);
	swaddr_t temp = cpu.eip + (k << 16 >> 16);
	if(!cpu.eflags.OF && cpu.eflags.SF == cpu.eflags.OF) {
		cpu.eip = temp;
	}
	print_asm("jg 0x%x", temp + 4);
	return 3;
}

make_helper(jg_rel_l) {
    int32_t k = instr_fetch(cpu.eip + 1, 4);
	swaddr_t temp = cpu.eip + k;
	if(!cpu.eflags.OF && cpu.eflags.SF == cpu.eflags.OF) {
		cpu.eip = temp;
	}
	print_asm("jg 0x%x", temp + 6);
	return 5;
}

make_helper_v(jg_rel)
