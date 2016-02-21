#include "cpu/exec/helper.h"

make_helper(ljmp) {
    uint32_t lword = instr_fetch(eip + 1, 4);
    uint16_t word  = instr_fetch(eip + 5, 2);
    cpu.cs.selector  = word;
    cpu.eip = lword;
	print_asm("ljmp $0x%x, $0x%x", word, lword);
    return 6;
}
