#include "cpu/exec/helper.h"

// if less
// => SF != OF

make_helper(jl_rel_b) {
    int32_t k = instr_fetch(cpu.eip + 1, 1);
	swaddr_t temp = cpu.eip + (k << 24 >> 24);
	if(cpu.eflags.SF != cpu.eflags.OF) {
		cpu.eip = temp;
	}
	print_asm("jl 0x%x", temp + 2);
	return 2;
}

