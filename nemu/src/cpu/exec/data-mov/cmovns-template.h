#include "cpu/exec/template-start.h"

#define instr cmovns

static void do_execute() {
    // Move if not sign (SF=0).
    if(cpu.eflags.SF == 0)
        OPERAND_W(op_dest, op_src->val);
	print_asm_template2();
}

make_instr_helper(r2rm)

#undef instr

#include "cpu/exec/template-end.h"
