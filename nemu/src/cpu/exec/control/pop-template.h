#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute () {
	// IF OperandSize = 16
	//    THEN
	//		DEST = (SS:ESP); (* copy a word *)
	//		ESP =  ESP + 2;
	//    ELSE (* OperandSize = 32 *)
	//		DEST = (SS:ESP); (* copy a dword *)
	//		ESP = ESP + 4;
	//    FI;
	// FI;

    uint32_t tmp = swaddr_read(cpu.esp, DATA_BYTE);
	OPERAND_W(op_src, tmp);
	cpu.esp += DATA_BYTE;
	print_asm_template1();
}

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
