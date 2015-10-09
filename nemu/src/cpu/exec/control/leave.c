#include "cpu/exec/helper.h"

make_helper(leave) {
	// leave:
	// ESP = EBP
	// EBP = Pop()
	//
	// Pop:
	// DEST = (SS:ESP); (* copy a dword *)
	// ESP = ESP + 4;
	cpu.esp = cpu.ebp;
	cpu.ebp = cpu.esp;
	cpu.esp += 4;
	print_asm("leave");
	return 1;
}
