#include "cpu/helper.h"

make_helper(ret) {
	// EIP = Pop()
	// 
	// Pop:
	// DEST = (SS:ESP); (* copy a dword *)
	// ESP = ESP + 4;
	
	eip = cpu.esp;
	cpu.esp += 4;
	return 1;
}
