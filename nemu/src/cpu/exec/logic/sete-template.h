#include "cpu/exec/template-start.h"

#define instr sete

static void do_execute () {
    // Set byte if equal (ZF=1)
    // IF condition THEN r/m8 ← 1 ELSE r/m8 ← 0; FI;

	OPERAND_W(op_src, cpu.eflags.ZF);
	print_asm_template1();
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"
