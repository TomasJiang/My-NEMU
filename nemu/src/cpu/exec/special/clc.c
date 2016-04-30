#include "cpu/exec/helper.h"

make_helper(clc) {
    cpu.eflags.CF = 0;
    return 1;
}
