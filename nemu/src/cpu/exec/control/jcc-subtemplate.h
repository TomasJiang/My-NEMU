#include "cpu/exec/template-start.h"

make_helper(concat3(instr, _, SUFFIX)) {
    int32_t k = instr_fetch(cpu.eip + 1, DATA_BYTE);
    uint32_t step = 32 - (8 * DATA_BYTE);
    swaddr_t temp = cpu.eip + (k << step >> step);
    Log("DATA_BYTE = %u, k = %u, step = %u, temp = %u", DATA_BYTE, k, step, temp);
    if(cond) {
        cpu.eip = temp;
    }
    print_asm(str(instr) " 0x%x", temp + 2 + (DATA_BYTE / 2) * 2);
    return 1 + DATA_BYTE;
}

#include "cpu/exec/template-end.h"
