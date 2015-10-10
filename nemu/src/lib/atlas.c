#include "cpu/exec/helper.h"
#include "atlas.h"

void print_operand(Operand *op) {
	Log("ADD PRINTOP:");
	switch(op->type) {
		case OP_TYPE_REG:
			Log("type: OP_TYPE_REG");
			Log("reg = 0x%x", op->reg);
			break;
		case OP_TYPE_MEM:
			Log("type: OP_TYPE_MEM");
			Log("addr = 0x%x", op->addr);
			break;
		case OP_TYPE_IMM:
			Log("type: OP_TYPE_IMM");
			Log("imm = 0x%x", op->imm);
			break;
	}
	Log("size = %u", op->size);
	Log("val = 0x%x", op->val);
	return;
}
