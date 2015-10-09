#include "cpu/exec/template-start.h"

#define instr and

static void do_execute () {
	DATA_TYPE result = op_dest->val & op_src->val;
	OPERAND_W(op_dest, result);

	// CF = 0, OF = 0; PF, SF, and ZF
	cpu.eflags.CF = 0;
	cpu.eflags.OF = 0;
	cpu.eflags.ZF = result == 0;
	cpu.eflags.SF = 0x1 & (result >> 31);

	int count;
	for(count = 0; result; count++)
		result &= result - 1;
	cpu.eflags.PF = !(count % 2);

	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
