#include "cpu/exec/helper.h"

make_helper(stc) {
    cpu.eflags.CF = 1;
    return 1;
}
