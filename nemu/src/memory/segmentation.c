#include "common.h"
#include "cpu/reg.h"
#include "memory/segmentation.h"

uint32_t lnaddr_read(lnaddr_t addr, size_t len);

lnaddr_t seg_translate(swaddr_t swaddr, size_t len, uint8_t sreg) {
    Log("swaddr = 0x%x", swaddr);
    if (!cpu.cr0.PE)
        return swaddr;
    uint32_t segdesc_addr = cpu.gdtr.base + cpu.cs.index;
    // Log("addr = 0x%x", segdesc_addr);

    SegDesc *segdesc = (SegDesc *)malloc(sizeof(SegDesc));
    segdesc->val[0] = lnaddr_read(segdesc_addr, 4);
    segdesc->val[1] = lnaddr_read(segdesc_addr + 4, 4);
    // int i;
    // for(i = 0; i < 2; ++i)
    //     Log("segdesc: 0x%08x", segdesc->val[i]);

    uint32_t lnaddr = swaddr + (segdesc->BASE_31_24 << 24) +
        (segdesc->BASE_23_16 << 16) + segdesc->BASE_15_0;
    Log("lnaddr = 0x%x", lnaddr);
    return lnaddr;
}

