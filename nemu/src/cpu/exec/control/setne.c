#include "cpu/decode/modrm.h"
#include "cpu/helper.h"

make_helper(setne_rm_b) {
	/*
	read_ModR_M(eip, op_src, op_dest);
	if(!cpu.eflags.ZF)
		REG(op_dest->reg) = op_dest->val;
		*/
	return 2;
}
