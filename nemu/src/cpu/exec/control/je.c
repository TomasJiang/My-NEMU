#include "cpu/exec/helper.h"

make_helper(je_rel_b) {
    uint8_t tmp = cpu.eip + instr_fetch(cpu.eip + 1, 1);
    uint32_t target = (cpu.eip & 0xffffff00) + tmp;
	if(cpu.eflags.ZF) {
		cpu.eip = target;
	}
	print_asm("je 0x%x", target + 2);
	return 2;
}
