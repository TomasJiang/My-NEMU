#include "cpu/exec/template-start.h"

#define instr push

static void do_execute () {
	// ESP = ESP - 4;
	// (SS:ESP) = (SOURCE); (* dword assignment *)
#if DATA_BYTE == 2
	cpu.esp -= 2;
#else
	cpu.esp -= 4;
#endif
	swaddr_write(cpu.esp, DATA_BYTE, op_src->val);

	print_asm_template1();
}

make_instr_helper(i)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
make_instr_helper(rm)
#endif

#include "cpu/exec/template-end.h"
