#include "cpu/exec/template-start.h"

#define instr movs

static void do_execute() {
    int32_t tmp = op_src->val;
    int32_t step = 32 - 8 * DATA_BYTE;
    tmp = tmp << step >> step;
    OPERAND_W(op_dest, tmp);
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
