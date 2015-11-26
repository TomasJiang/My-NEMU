#include "cpu/exec/helper.h"

make_helper(jmp_rel_b) {
	cpu.eip += instr_fetch(cpu.eip + 1, 1);
	print_asm("jmp 0x%x", cpu.eip + 2);
	return 2;
}

make_helper(jmp_rel_w) {
	cpu.eip += instr_fetch(cpu.eip + 1, 2);
	print_asm("jmp 0x%x", cpu.eip + 3);
	return 3;
}

make_helper(jmp_rel_l) {
	cpu.eip += instr_fetch(cpu.eip + 1, 4);
	print_asm("jmp 0x%x", cpu.eip + 5);
	return 5;
}

#define DATA_BYTE 1
#include "jmp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jmp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jmp-template.h"
#undef DATA_BYTE

make_helper_v(jmp_rel)
make_helper_v(jmp_rm)
