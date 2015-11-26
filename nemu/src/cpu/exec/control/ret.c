#include "cpu/exec/helper.h"

make_helper(ret) {
	// EIP = Pop()
	//
	// Pop:
	// DEST = (SS:ESP); (* copy a dword *)
	// ESP = ESP + 4;

	cpu.eip = swaddr_read(cpu.esp, 4);
	cpu.esp += 4;
	print_asm("ret");
	return 1;
}

make_helper(ret_i) {
    uint32_t imm = instr_fetch(eip + 1, 2);
    Log("imm = %u", imm);
	cpu.eip = swaddr_read(cpu.esp, 4);
	cpu.esp += 4 + imm;
	print_asm("ret $0x%u", imm);
    return 3;
}
