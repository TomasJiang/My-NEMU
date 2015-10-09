#include "cpu/exec/helper.h"
#include "atlas.h"

void print_operand(Operand *op) {
	Log("ADD PRINTOP:\n");
	switch(op->type) {
		case OP_TYPE_REG:
			printf("type: OP_TYPE_REG\n");
			printf("reg = 0x%x\n", op->reg);
		case OP_TYPE_MEM:
			printf("type: OP_TYPE_MEM\n");
			printf("reg = 0x%x\n", op->addr);
		case OP_TYPE_IMM:
			printf("type: OP_TYPE_IMM\n");
			printf("reg = 0x%x\n", op->imm);
	}
	printf("size = %u", op->size);
	printf("val = 0x%x\n", op->val);
	return;
}
