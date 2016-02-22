#ifndef __PAGE_H__
#define __PAGE_H__

typedef union {
    struct {
        uint32_t P               : 1;
        uint32_t R_W             : 1;
        uint32_t U_S             : 1;
        uint32_t PAD0            : 2;
        uint32_t A               : 1;
        uint32_t D               : 1;
        uint32_t PAD1            : 2;
        uint32_t AVAIL           : 3;
        uint32_t PAGE_FRAME_ADDR : 20;
    };
    uint32_t val;
} PageTableEntry;

hwaddr_t page_translate(lnaddr_t lnaddr);

#endif
