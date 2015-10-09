#include "cpu/exec/helper.h"

make_helper(call_rel32) {
	// Push:
	// ESP = ESP - 4;
	// (SS:ESP) = (SOURCE);
	//
	// Push(EIP);
	// EIP = EIP + rel32;
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.eip);
	cpu.eip += instr_fetch(cpu.eip + 1, 4);
	print_asm("call 0x%x", cpu.eip+5);
	return 5;
}
