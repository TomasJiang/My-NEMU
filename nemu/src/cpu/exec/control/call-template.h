#include "cpu/exec/template-start.h"

#define instr call

static void do_execute() {
    // Push(EIP);
    // EIP = [r/m32];
    cpu.esp -= 4;
    swaddr_write(cpu.esp, 4, cpu.eip + 4, R_SS);
    cpu.eip = op_src->val-2;
    print_asm("call 0x%x", op_src->val);
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"
