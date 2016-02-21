#include "cpu/exec/helper.h"

make_helper(ljmp) {
    uint32_t lword = instr_fetch(eip + 1, 4);
    uint16_t word  = instr_fetch(eip + 5, 2);
    Log("lword = 0x%x, word = 0x%x", lword, word);
    return 6;
}
