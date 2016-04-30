#include "cpu/exec/template-start.h"

#define instr movs

make_helper(concat3(instr, _, SUFFIX)) {
    Log("copy from 0x%x to 0x%x: %x", reg_l(R_ESI), reg_l(R_EDI), MEM_R(REG(R_ESI)));
    MEM_W(reg_l(R_EDI), MEM_R(reg_l(R_ESI)));
    int flag = cpu.eflags.DF;
    cpu.edi += (-2 * flag + 1) * DATA_BYTE;
    cpu.esi += (-2 * flag + 1) * DATA_BYTE;
    print_asm(str(instr));
    return 1;
}

#include "cpu/exec/template-end.h"
