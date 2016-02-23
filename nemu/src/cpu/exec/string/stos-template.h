#include "cpu/exec/template-start.h"

#define instr stos

make_helper(concat3(instr, _, SUFFIX)) {
    // Log("$edi = 0x%x, $es = 0x%x", REG(R_EDI), cpu.es.selector);
    MEM_W(REG(R_EDI), REG(R_EAX), R_ES);
    int flag = cpu.eflags.DF;
    cpu.edi += (-2 * flag + 1) * DATA_BYTE;
    print_asm(str(instr));
    return 1;
}

#include "cpu/exec/template-end.h"
