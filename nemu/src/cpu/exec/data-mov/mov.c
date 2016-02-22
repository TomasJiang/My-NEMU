#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "mov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "mov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "mov-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */
int read_ModR_M(swaddr_t eip, Operand *rm, Operand *reg);

make_helper(mov_cr2r) {
    decode_r2rm_l(cpu.eip + 2);
    // Log(" op_src->reg = 0x%x",  op_src->reg);
    // Log("op_dest->reg = 0x%x", op_dest->reg);
    switch (op_dest->reg) {
        case 0:
            reg_l(op_src->reg) = cpu.cr0.val;
            print_asm("mov %%cr0, %s", op_src->str);
            break;
        case 3:
            reg_l(op_src->reg) = cpu.cr3.val;
            print_asm("mov %%cr3, %s", op_src->str);
            break;
        default:
            panic("error");
    }

    return 2;
}

make_helper(mov_r2cr) {
    decode_r2rm_l(cpu.eip + 2);
    // Log(" op_src->reg = 0x%x",  op_src->reg);
    // Log("op_dest->reg = 0x%x", op_dest->reg);
    switch (op_src->reg) {
        case 0:
            cpu.cr0.val = op_dest->val;
            print_asm("mov %s, %%cr0", op_dest->str);
            break;
        case 3:
            cpu.cr3.val = op_dest->val;
            print_asm("mov %s, %%cr3", op_dest->str);
            break;
        default:
            panic("error");
    }
    return 2;
}

make_helper(mov_r2sreg) {
    decode_r2rm_l(cpu.eip + 1);
    // Log("op_dest->reg = 0x%x", op_dest->reg);
    // Log("op_dest->val = 0x%x", op_dest->val);
    // Log(" op_src->reg = 0x%x",  op_src->reg);
    SREG_VAL(op_src->reg) = op_dest->val;
    print_asm("mov %s, %%%s", op_dest->str, SREG_NAME(op_src->reg));
    return 2;
}

make_helper_v(mov_i2r)
make_helper_v(mov_i2rm)
make_helper_v(mov_r2rm)
make_helper_v(mov_rm2r)
make_helper_v(mov_a2moffs)
make_helper_v(mov_moffs2a)
