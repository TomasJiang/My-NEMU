#include "cpu/exec/helper.h"

make_helper(lgdt) {
    uint16_t limit = instr_fetch(cpu.eip + 1, 2);
    uint32_t base  = instr_fetch(cpu.eip + 3, 4);
    Log("limit = 0x%x, base = 0x%x", limit, base);
    return 7;
}
