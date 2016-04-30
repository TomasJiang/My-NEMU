#include "cpu/exec/template-start.h"

#define instr cmovp

static void do_execute() {
    // Move if not sign (SF=0).
    if(cpu.eflags.PF == 1)
        //myth OPERAND_W(op_dest, op_src->val);
        OPERAND_W(op_src, op_dest->val);
	print_asm_template2();
}

make_instr_helper(r2rm)

#undef instr

#include "cpu/exec/template-end.h"
