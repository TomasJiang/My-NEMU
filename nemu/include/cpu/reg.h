#ifndef __REG_H__
#define __REG_H__

#include "common.h"

enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };

// Sreg: a segment register. The segment register bit assignments are
// ES=0, CS=1, SS=2, DS=3, FS=4, and GS=5. (i386 manual p248)
enum { R_ES, R_CS, R_SS, R_DS, R_FS, R_GS };


/* TODO: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */

typedef struct {
    union {
        struct {
            unsigned index: 13;
            unsigned ti   : 1;
            unsigned rpl  : 2;
        };
        uint16_t selector;
    };
    unsigned hidden_selector : 32;
} sreg;


typedef struct {
	union
	{
		union {
			uint32_t _32;
			uint16_t _16;
			uint8_t _8[2];
		} gpr[8];

		/* Do NOT change the order of the GPRs' definitions. */

		struct
		{
			uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
		};
	};

	struct {
		unsigned CF: 1;
		unsigned PAD1: 1;
		unsigned PF: 1;
		unsigned PAD2: 3;
		unsigned ZF: 1;
		unsigned SF: 1;
		unsigned PAD3: 1;
		unsigned IF: 1;
		unsigned DF: 1;
		unsigned OF: 1;
		unsigned PAD4: 20;
	} eflags;

	swaddr_t eip;

    union {
        struct {
            uint32_t PE:  1;
            uint32_t MP:  1;
            uint32_t EM:  1;
            uint32_t TS:  1;
            uint32_t ET:  1;
            uint32_t PAD: 26;
            uint32_t PG:  1;
        };
        uint32_t val;
    } cr0;

    struct {
        unsigned base:  32;
        unsigned limit: 16;
    } gdtr;

    union {

        struct {
            sreg es, cs, ss, ds, fs, gs;
        };

        sreg sregs[6];
    };

} CPU_state;

extern CPU_state cpu;

void init_cr0();

static inline int check_reg_index(int index) {
	assert(index >= 0 && index < 8);
	return index;
}

static inline int check_sreg_index(int index) {
    assert(index >= 0 && index < 6);
    return index;
}

#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)
#define reg_w(index) (cpu.gpr[check_reg_index(index)]._16)
#define reg_b(index) (cpu.gpr[check_reg_index(index) & 0x3]._8[index >> 2])

#define  SREG(index) (cpu.sregs[check_sreg_index(index)])
#define  SREG_VAL(index) (cpu.sregs[check_sreg_index(index)].selector)
#define  SREG_NAME(index) (sregs_name[check_sreg_index(index)])

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];
extern const char *sregs_name[];

#endif
