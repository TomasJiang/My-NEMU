#include "cpu/exec/template-start.h"

#define instr shld

#if DATA_BYTE == 2 || DATA_BYTE == 4
static void do_execute () {
	DATA_TYPE in = op_dest->val; // eax low
	DATA_TYPE out = op_src->val; // edx high

	uint8_t count = reg_b(R_CL);
	count &= 0x3f;
    printf("\n");
    Log("count = %u, in(eax) = 0x%x, out(edx) = 0x%x", count, in, out);
	while(count != 0) {
		out <<= 1;
		out |= (in >> ((DATA_BYTE << 3) - 1)) & 0x1;
		in <<= 1;
		count --;
	}

    Log("out = 0x%x", out);
	OPERAND_W(op_src, out);

    // dest eax; src edx
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
