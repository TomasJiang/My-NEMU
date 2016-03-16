#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute() {
    cpu.eip = op_src->val - ops_decoded.instr_len;
    // Log("instr_len = %u", ops_decoded.instr_len);
    print_asm("jmp 0x%x", op_src->val);
    //Log("eip = 0x%x", cpu.eip);
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"
