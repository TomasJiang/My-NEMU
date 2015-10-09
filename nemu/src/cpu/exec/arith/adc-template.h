#include "cpu/exec/template-start.h"

#define instr adc

static void do_execute () {

	DATA_TYPE result = op_dest->val + op_src->val + cpu.eflags.CF;
	OPERAND_W(op_src, result);

	// OF, SF, ZF, AF, CF and PF
	unsigned df = 0x1 & (op_dest->val >> 31);
	unsigned sf = 0x1 & (op_src->val >> 31);
	unsigned rf = 0x1 & (result >> 31);

	cpu.eflags.OF = (df && sf && !rf) || (!df && !sf && rf);
	cpu.eflags.SF = rf;
	cpu.eflags.ZF = (result == 0);
	cpu.eflags.CF = (df && sf) || ((df || sf) && !rf);

	result = 0xff & result;
	unsigned count;
	for(count = 0; result; ++count)
		result &= (result - 1);
	cpu.eflags.PF = !(count % 2);

	print_asm_template2();
}

make_instr_helper(r2rm)

#include "cpu/exec/template-end.h"
