#include "cpu/exec/helper.h"

make_helper(jmp_rel8) {
	cpu.eip += instr_fetch(cpu.eip + 1, 1);
	print_asm_template1();
	return 2;
}
