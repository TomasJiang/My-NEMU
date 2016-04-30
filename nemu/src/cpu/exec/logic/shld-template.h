#include "cpu/exec/template-start.h"

#define instr shld

#if DATA_BYTE == 2 || DATA_BYTE == 4
static void do_execute () {
	DATA_TYPE in = op_dest->val;
	DATA_TYPE out = op_src->val;

	uint8_t count = reg_b(R_CL);
	count &= 0x1f;
	while(count != 0) {
		out <<= 1;
		out |= (in & 1) << ((DATA_BYTE << 3) - 1);
		in <<= 1;
		count --;
	}

	OPERAND_W(op_src, out);

	print_asm("shld" str(SUFFIX) " %%cl,%s,%s", op_dest->str, op_src->str);
}

make_helper(concat(shld_, SUFFIX)) {
	int len = concat(decode_rm2r_, SUFFIX) (eip + 1);
	op_dest->val = REG(op_dest->reg);
	do_execute();
	return len + 1;
}
#endif

#undef instr
#include "cpu/exec/template-end.h"
