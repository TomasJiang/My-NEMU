#include "cpu/exec/template-start.h"

#define instr test

static void do_execute () {
	DATA_TYPE result = op_dest->val & op_src->val;

	// OF = 0, CF = 0; SF, ZF, and PF
	cpu.eflags.OF = 0;
	cpu.eflags.CF = 0;
	cpu.eflags.SF = 0x1 & (result >> 31);
	cpu.eflags.ZF = (result == 0);

	int count;
	result = (result & 0xff);
	for(count = 0; result; count++)
		result &= (result - 1);
	cpu.eflags.PF = !(count % 2);

	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)

#include "cpu/exec/template-end.h"
