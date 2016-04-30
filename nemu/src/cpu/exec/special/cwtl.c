#include "cpu/exec/helper.h"

make_helper(cwtl) {
    int32_t temp = reg_l(R_EAX);
    temp = (temp << 16 >> 16);
    reg_l(R_EAX) = temp;
    return 1;
}
