#include "cpu/exec/helper.h"

make_helper(call_rel32) {
	// Push:
	// ESP = ESP - 4;
	// (SS:ESP) = (SOURCE);
	//
	// Push(EIP);
	// EIP = EIP + rel32;
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.eip + 4);
	cpu.eip += instr_fetch(cpu.eip + 1, 4);
	print_asm("call 0x%x", cpu.eip + 5);
	return 5;
}


#define DATA_BYTE 2
#include "call-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "call-template.h"
#undef DATA_BYTE

make_helper_v(call_rm)
