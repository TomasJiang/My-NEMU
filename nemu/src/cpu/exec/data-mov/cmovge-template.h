#include "cpu/exec/template-start.h"

#define instr cmovge

static void do_execute() {
    // Move if greater or equal (SF=OF).
    if(cpu.eflags.SF == cpu.eflags.OF)
        OPERAND_W(op_src, op_dest->val);
	print_asm(str(instr) str(SUFFIX) " %s, %s", op_dest->str, op_src->str);
}

make_instr_helper(r2rm)

#undef instr

#include "cpu/exec/template-end.h"
