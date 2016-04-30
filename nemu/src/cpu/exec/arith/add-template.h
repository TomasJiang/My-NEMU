#include "cpu/exec/template-start.h"

#define instr add

static void do_execute() {
	DATA_TYPE result = op_dest->val + op_src->val;
	OPERAND_W(op_dest, result);

	// OF, SF, ZF, CF, and PF
	unsigned df = 0x1 & (op_dest->val >> 31);
	unsigned sf = 0x1 & (op_src->val >> 31);
	unsigned rf = 0x1 & (result >> 31);

	cpu.eflags.OF = (df && sf && !rf) || (!df && !sf && rf);
	cpu.eflags.SF = rf;
	cpu.eflags.ZF = (result == 0);
    Log("result = %d, ZF = %d", result, cpu.eflags.ZF);
	cpu.eflags.CF = (df && sf) || ((df || sf) && !rf);

	result = 0xff & result;
	unsigned count;
	for(count = 0; result; ++count)
		result &= (result - 1);
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
