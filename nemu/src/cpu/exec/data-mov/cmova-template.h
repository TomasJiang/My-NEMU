#include "cpu/exec/template-start.h"

#define instr cmova

static void do_execute() {
    // Move if above (CF=0 and ZF=0)
    if(cpu.eflags.CF == 0 && cpu.eflags.ZF == 0)
        // OPERAND_W(op_dest, op_src->val);
        OPERAND_W(op_src, op_dest->val);
	print_asm(str(instr) str(SUFFIX) " %s, %s", op_dest->str, op_src->str);
}

make_instr_helper(r2rm)

#undef instr

#include "cpu/exec/template-end.h"
