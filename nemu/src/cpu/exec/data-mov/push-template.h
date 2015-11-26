#include "cpu/exec/template-start.h"

#define instr push

static void do_execute () {
    // IF OperandsSize = 16
    // THEN
    //  ESP <- ESP - 2;
    //  (SS:ESP) <- (SOURCE); (* word assignment *)
    // ELSE
	//  ESP = ESP - 4;
	//  (SS:ESP) <- (SOURCE); (* dword assignment *)
#if DATA_BYTE == 2
	cpu.esp -= 2;
	swaddr_write(cpu.esp, 2, op_src->val);
#else
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, op_src->val);
#endif
	print_asm_template1();
}

make_instr_helper(i)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
make_instr_helper(rm)
#endif

#include "cpu/exec/template-end.h"
