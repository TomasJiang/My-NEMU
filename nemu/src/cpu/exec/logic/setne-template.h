#include "cpu/exec/template-start.h"

#define instr setne

static void do_execute () {
	// IF condition THEN r/m8 = 1 ELSE r/m= 0; FI;

	OPERAND_W(op_src, !cpu.eflags.ZF);
	print_asm_template1();
}

make_helper

#include "cpu/exec/template-end.h"
