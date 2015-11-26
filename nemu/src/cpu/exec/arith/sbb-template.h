#include "cpu/exec/template-start.h"

#define instr sbb

static void do_execute() {
    int32_t tmp = op_src->val, step = 32 - 8 * DATA_BYTE, result;
    tmp = (tmp << step >> step) + cpu.eflags.CF;
    result = op_dest->val - tmp;
    OPERAND_W(op_dest, result);

    unsigned sf = 0x1 & (tmp >> 31);
    unsigned df = 0x1 & (op_dest->val >> 31);
    unsigned rf = 0x1 & (result >> 31);

    cpu.eflags.OF = (!df && sf && rf) || (df && !sf && !rf);
    cpu.eflags.SF = rf;
    cpu.eflags.ZF = (result == 0);
    cpu.eflags.CF = (uint32_t)(op_dest->val) < (uint32_t)(op_src->val);

    result = 0xff & result;
    unsigned cnt;
    for(cnt = 0; result; ++cnt)
        result &= (result - 1);
    cpu.eflags.PF = !(cnt % 2);

    print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
