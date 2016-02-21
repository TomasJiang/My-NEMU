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

make_helper(mov_cr02r) {
    int len = read_ModR_M(cpu.eip, op_dest, op_src);
    Log("len = %d", len);
    Log("eax = 0x%x", cpu.eax);
    Log("op_dest = %x", op_dest->reg);

    return 1;
}
make_helper(mov_r2cr0) {
    return 1;
}

make_helper_v(mov_i2r)
make_helper_v(mov_i2rm)
make_helper_v(mov_r2rm)
make_helper_v(mov_rm2r)
make_helper_v(mov_a2moffs)
make_helper_v(mov_moffs2a)
