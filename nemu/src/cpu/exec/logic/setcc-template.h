#define DATA_BYTE 1
#include "cpu/exec/template-start.h"


static void do_execute () {
	OPERAND_W(op_src, cond);
	print_asm_template1();
}

make_instr_helper(rm)


#include "cpu/exec/template-end.h"
#undef DATA_BYTE
