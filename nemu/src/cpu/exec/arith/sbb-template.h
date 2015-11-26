#include "cpu/exec/template-start.h"

#define instr sbb

static void do_execute() {
    int32_t tmp = op_src->val, step = 32 - 8 * DATA_BYTE;
    tmp = (tmp << step >> step) + cpu.eflags.CF;
    OPERAND_W(op_dest, op_dest->val - tmp);
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
