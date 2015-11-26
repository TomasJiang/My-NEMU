#include "cpu/exec/template-start.h"

#define instr cmps

make_helper(concat3(instr, _, SUFFIX)) {
    // cmp S2, S1 <=> S1 - S2
    // cmp SRC, DESt <=> DEST : SRC
    //
    // cmp edi, esi

    DATA_TYPE lval = MEM_R(REG(R_ESI));
	DATA_TYPE rval = MEM_R(REG(R_EDI));
	DATA_TYPE result = lval - rval;

	// OF, SF, ZF, CF, and PF
    uint32_t step = DATA_BYTE * 8 - 1;
	unsigned sf = 0x1 & (rval >> step);
    unsigned df = 0x1 & (lval >> step);
	unsigned rf = 0x1 & (result >> step);

	cpu.eflags.OF = (!df && sf && rf) || (df && !sf && !rf);
	cpu.eflags.SF = rf;
	cpu.eflags.ZF = (result == 0);
	cpu.eflags.CF = (uint32_t)(lval) < (uint32_t)(rval);

	result = 0xff & result;
	unsigned count;
	for(count = 0; result; ++count)
		result &= (result - 1);
	cpu.eflags.PF = !(count % 2);

    REG(R_EDI) = (-2 * cpu.eflags.DF + 1) * DATA_BYTE;
    REG(R_ESI) = (-2 * cpu.eflags.DF + 1) * DATA_BYTE;

    print_asm("cmps");
    return 1;
}

#include "cpu/exec/template-end.h"
