#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute () {
    // cmp S2, S1 <=> S1 - S2
    // cmp SRC, DESt <=> DEST : SRC
	DATA_TYPE rval = -op_src->val;
	DATA_TYPE result = op_dest->val + rval;

    // cmp 0x80000000, 0
    // Wrong CF = 1, PF = 1, SF = 1

	// OF, SF, ZF, CF, and PF
	unsigned df = 0x1 & (op_dest->val >> 31);
	unsigned sf = 0x1 & (rval >> 31);
    unsigned srcf = 0x1 & (op_src->val >> 31);
	unsigned rf = 0x1 & (result >> 31);

	cpu.eflags.OF = (df && sf && !rf) || (!df && !sf && rf) || !(sf ^ srcf);
	cpu.eflags.SF = rf;
	cpu.eflags.ZF = (result == 0);
	cpu.eflags.CF = 0x1 & (((df && sf) || ((df || sf) && !rf)) ^ 0x1);

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
