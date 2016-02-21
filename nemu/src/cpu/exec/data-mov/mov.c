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
    // TODO
    cpu.eax = cpu.cr0.val;
    return 2;
}
make_helper(mov_r2cr0) {
    // TODO
    cpu.cr0.val = cpu.eax;
    return 2;
}

make_helper_v(mov_i2r)
make_helper_v(mov_i2rm)
make_helper_v(mov_r2rm)
make_helper_v(mov_rm2r)
make_helper_v(mov_a2moffs)
make_helper_v(mov_moffs2a)
