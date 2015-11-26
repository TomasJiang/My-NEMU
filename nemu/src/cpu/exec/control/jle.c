#include "cpu/exec/helper.h"

make_helper(jle_rel8) {
    // if less or equal
    // => ZF = 1 or SF != OF

    int32_t k = instr_fetch(cpu.eip + 1, 1);
    Log("k = %x\n", k);
	swaddr_t temp = cpu.eip + (k << 24 >> 24);
    Log("jle addr = %x\n", temp);
	if(cpu.eflags.ZF || cpu.eflags.SF != cpu.eflags.OF) {
		cpu.eip = temp;
	}
	print_asm("jle 0x%x", temp + 2);
	return 2;
}
