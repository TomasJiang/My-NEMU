#include "cpu/exec/template-start.h"

#define instr movs

make_helper(concat3(instr, _, SUFFIX)) {
    Log("esi = 0x%x, edi = 0x%x", REG(R_ESI), REG(R_EDI));
    Log("Mem($esi) = 0x%x", MEM_R(REG(R_ESI)));
    MEM_W(REG(R_EDI), MEM_R(REG(R_ESI)));
    int flag = cpu.eflags.DF;
    cpu.edi += (-2 * flag + 1) * DATA_BYTE;
    cpu.esi += (-2 * flag + 1) * DATA_BYTE;
    print_asm(str(instr));
    return 1;
}

#include "cpu/exec/template-end.h"
