#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute() {
    cpu.eip = op_src->val-2;
    Log("DATA_BYTE = %d", DATA_BYTE);
    Log("op_src->val = 0x%x", op_src->val);
    Log("op_src2->val = 0x%x", op_src2->val);
    Log("op_dest->val = 0x%x", op_dest->val);
    Log("instr_len = %u", ops_decoded.instr_len);
    Log("%s", op_src->str);
    Log("%u", op_src->size);
    Log("%x", ops_decoded.opcode);
    print_asm("jmp 0x%x", op_src->val);
    //Log("eip = 0x%x", cpu.eip);
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"
