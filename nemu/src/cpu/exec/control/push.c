#include "cpu/exec/helper.h"

make_helper(push_r32) {
	// ESP = ESP - 4;
	// (SS:ESP) = (SOURCE); (* dword assignment *)
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, instr_fetch(eip + 1, 4));
	return 1;
}
