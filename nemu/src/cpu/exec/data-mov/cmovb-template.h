#include "cpu/exec/template-start.h"

#define instr cmovb

static void do_execute() {
    // Move if not sign (SF=0).
    if(cpu.eflags.CF == 1)
        OPERAND_W(op_dest, op_src->val);
	print_asm_template2();
}

make_instr_helper(r2rm)

#undef instr

#include "cpu/exec/template-end.h"
