#include "cpu/exec/helper.h"

make_helper(lgdt) {
    uint32_t gdtdesc = instr_fetch(cpu.eip + 3, 4);
    uint32_t limit = instr_fetch(gdtdesc, 2);
    uint32_t base  = instr_fetch(gdtdesc + 2, 4);
    cpu.gdtr.base  = base;
    cpu.gdtr.limit = limit;
    Log("limit = 0x%x, base = 0x%x", limit, base);
    print_asm("lgdt 0x%x", gdtdesc);
    return 6;
}
