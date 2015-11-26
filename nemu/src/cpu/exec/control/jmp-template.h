#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute() {
    Log("jmp_rm");
    cpu.eip = op_src->val;
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"
