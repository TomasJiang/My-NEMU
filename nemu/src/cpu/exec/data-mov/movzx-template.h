#include "cpu/exec/template-start.h"

#define instr movz

static void do_execute() {
    OPERAND_W(op_dest, op_src->val);
    /*
#if DATA_BYTE == 1
	if(ops_decoded.is_data_size_16) {
		REG(op_dest->reg) = REG(op_dest->reg) >> 16 << 16;
		REG(op_dest->reg) += op_src->val;
	} else
		REG(op_dest->reg) = op_src->val;
#endif
#if DATA_BYTE == 2
	REG(op_dest->reg) = op_src->val;
#endif

*/
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
