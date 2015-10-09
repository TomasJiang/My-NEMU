#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "push-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "push-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "push-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper(push_m_w) {
	cpu.esp -= 2;
	swaddr_write(cpu.esp, 2, instr_fetch(eip + 1, 2));
	return 3;
}

make_helper(push_m_l) {
	cpu.esp -= 2;
	swaddr_write(cpu.esp, 4, instr_fetch(eip + 1, 4));
	return 5;
}

make_helper_v(push_i)
make_helper_v(push_r)
make_helper_v(push_m)
