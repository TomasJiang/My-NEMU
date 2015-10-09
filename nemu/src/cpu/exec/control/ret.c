#include "cpu/exec/helper.h"

make_helper(ret) {
	// EIP = Pop()
	// 
	// Pop:
	// DEST = (SS:ESP); (* copy a dword *)
	// ESP = ESP + 4;
	
	cpu.eip = cpu.esp;
	cpu.esp += 4;
	print_asm("ret");
	return 1;
}
