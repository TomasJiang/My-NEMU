#include "cpu/exec/template-start.h"

#define instr cmovge

static void do_execute() {
    // Move if greater or equal (SF=OF).
    if(cpu.eflags.SF == cpu.eflags.OF)
        OPERAND_W(op_dest, op_src->val);
	print_asm_template2();
}

make_instr_helper(r2rm)

#undef instr

#include "cpu/exec/template-end.h"
