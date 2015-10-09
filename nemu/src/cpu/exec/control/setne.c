#include "cpu/decode/modrm.h"
#include "cpu/helper.h"

make_helper(setne_rm_b) {
	read_ModR_M(eip, op_src, op_dest);
	return 2;
}
