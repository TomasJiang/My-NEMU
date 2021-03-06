#include "cpu/exec/template-start.h"

#define instr cmove

static void do_execute() {
    // Move if equal (ZF=1).
    if(cpu.eflags.ZF == 1)
        //OPERAND_W(op_dest, op_src->val);
        // myth
        OPERAND_W(op_src, op_dest->val);
	print_asm(str(instr) str(SUFFIX) " %s, %s", op_dest->str, op_src->str);
}

make_instr_helper(r2rm)

#undef instr

#include "cpu/exec/template-end.h"
