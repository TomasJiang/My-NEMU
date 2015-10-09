#include "cpu/exec/helper.h"

make_helper(call_rel32) {
	// Push(EIP);
	// EIP = EIP + rel32;
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, eip);
	eip = eip + instr_fetch(eip + 1, 4);
	return 5;
}
