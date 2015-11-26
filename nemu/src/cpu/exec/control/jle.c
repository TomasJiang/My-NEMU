#include "cpu/exec/helper.h"

make_helper(jle_rel8) {
    // if less or equal
    // => ZF = 1 or SF != OF

	swaddr_t temp = cpu.eip + (instr_fetch(cpu.eip + 1, 1) << 24 >> 24);
    Log("jle addr = %u\n", temp);
	if(cpu.eflags.ZF || cpu.eflags.SF != cpu.eflags.OF) {
		cpu.eip = temp;
	}
	print_asm("jle 0x%x", temp + 2);
	return 2;
}
