#include "cpu/exec/template-start.h"

#define instr movz

void print_operand(Operand *op);

static void do_execute() {
	printf("op_src:\n");
	print_operand(op_src);
	printf("op_dest:\n");
	print_operand(op_dest);

#if DATA_BYTE == 1
	if(ops_decoded.is_data_size_16)
		op_dest->size = 2;
	else
		op_dest->size = 4;
#endif
#if DATA_BYTE == 2
	op_dest->size = 4; 
#endif

	printf("op_src:\n");
	print_operand(op_src);
	printf("op_dest:\n");
	print_operand(op_dest);

	OPERAND_W(op_dest, op_src->val);
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
