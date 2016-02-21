#include "cpu/exec/helper.h"

make_helper(lgdt) {
    uint16_t gdtdesc = instr_fetch(cpu.eip + 3, 4);
    Log("gdtdesc = 0x%x", gdtdesc);
    // uint32_t base  = instr_fetch(cpu.eip + 4, 4);
    // cpu.gdtr.base  = base;
    // cpu.gdtr.limit = limit;
    // Log("limit = 0x%x, base = 0x%x", limit, base);
    return 6;
}
