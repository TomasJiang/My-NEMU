#include "cpu/exec/helper.h"

make_helper(cdq) {
    if((int32_t)cpu.eax < 0)
        cpu.edx = 0xffffffff;
    else
        cpu.edx = 0;
    return 1;
}
