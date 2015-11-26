#include "cpu/exec/helper.h"

make_helper(cdq) {
    if(cpu.eax < 0)
        cpu.eax = 0x0FFFFFFF;
    else
        cpu.edx = 0;
    return 1;
}
