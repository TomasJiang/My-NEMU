#include "cpu/exec/template-start.h"

#define instr cmov

static void do_execute() {
    // Move if above (CF=0 and ZF=0)
    if(cpu.eflags.CF == 0 && cpu.eflags.ZF == 0)
        OPERAND_W(op_dest, op_src->val);
	print_asm_template2();
}

make_instr_helper(r2rm)

#include "cpu/exec/template-end.h"
