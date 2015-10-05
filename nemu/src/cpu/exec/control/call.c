#include "cpu/exec/helper.h"

make_helper(call_rel32) {
	// Push(EIP);
	// EIP = EIP + rel32;
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.eip);
	cpu.eip = cpu.eip + instr_fetch(cpu.eip + 1, 4);
	return 5;
}
