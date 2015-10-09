#include "cpu/exec/helper.h"

make_helper(call_rel32) {
	// Push:
	// ESP = ESP - 4;
	// (SS:ESP) = (SOURCE);
	//
	// Push(EIP);
	// EIP = EIP + rel32;
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, eip);
	Log("eip = 0x%08x", eip);
	eip = eip + instr_fetch(eip + 1, 4);
	Log("eip = 0x%08x", eip);
	return 5;
}
