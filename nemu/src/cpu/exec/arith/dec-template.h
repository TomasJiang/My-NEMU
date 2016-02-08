#include "cpu/exec/template-start.h"

#define instr dec

static void do_execute () {
    Log("dec");
	DATA_TYPE result = op_src->val - 1;
	OPERAND_W(op_src, result);

	/* TODO: Update EFLAGS. */
    uint32_t step = DATA_BYTE * 8 - 1;
    unsigned sf = 0x1 & (op_src->val >> step);
    unsigned df = 0x1 & (op_dest->val >> step);
    unsigned rf = 0x1 & (result >> step);

    cpu.eflags.OF = (!df && sf && rf) || (df && !sf && !rf);
    cpu.eflags.SF = rf;
    cpu.eflags.ZF = (result == 0);

    result = 0xff & result;
    unsigned count;
    for(count = 0; result; ++count)
        result &= (result - 1);
    cpu.eflags.PF = !(count % 2);

	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
