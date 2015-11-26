#include "cpu/exec/template-start.h"

#define instr movsx

static void do_execute() {
    int32_t tmp = op_src->val;
    int32_t step = 32 - 8 * DATA_BYTE;
    tmp = tmp << step >> step;
#if DATA_BYTE == 1
    if(ops_decoded.is_data_size_16)
        reg_w(op_dest->reg) = tmp;
    else
        reg_l(op_dest->reg) = tmp;
#elif DATA_BYTE == 2
    reg_l(op_dest->reg) = tmp;
#endif
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
