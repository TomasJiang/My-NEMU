#include "cpu/exec/template-start.h"

#define instr movz

void print_operand(Operand *op);

static void do_execute() {
	printf("reg: %x\n", REG(op_dest->reg));
	printf("op_src:\n");
	print_operand(op_src);
	printf("op_dest:\n");
	print_operand(op_dest);

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

	printf("reg: %x\n", REG(op_dest->reg));
	printf("op_src:\n");
	print_operand(op_src);
	printf("op_dest:\n");
	print_operand(op_dest);

	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
