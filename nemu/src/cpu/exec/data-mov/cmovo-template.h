#include "cpu/exec/template-start.h"

#define instr cmovo

static void do_execute() {
    // Move if above (CF=0 and ZF=0)
    if(cpu.eflags.OF == 1)
        OPERAND_W(op_src, op_dest->val);
	print_asm(str(instr) str(SUFFIX) " %s, %s", op_dest->str, op_src->str);
}

make_instr_helper(r2rm)

#undef instr

#include "cpu/exec/template-end.h"
