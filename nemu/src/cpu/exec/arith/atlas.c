#include "cpu/exec/helper.h"
#include "atlas.h"

void print_operand(Operand *op) {
	Log("ADD PRINTOP:\n");
	switch(op->type) {
		case OP_TYPE_REG:
			Log("type: OP_TYPE_REG\n");
			Log("reg = 0x%x\n", op->reg);
		case OP_TYPE_MEM:
			Log("type: OP_TYPE_MEM\n");
			Log("reg = 0x%x\n", op->addr);
		case OP_TYPE_IMM:
			Log("type: OP_TYPE_IMM\n");
			Log("reg = 0x%x\n", op->imm);
	}
	Log("size = %u", op->size);
	Log("val = 0x%x\n", op->val);
	return;
}
