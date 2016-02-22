#include "common.h"
#include "cpu/reg.h"
#include "memory/segmentation.h"

uint32_t lnaddr_read(lnaddr_t addr, size_t len);

/* If the destination operand is a segment register (DS, ES, FS,
 * GS, or SS), the source operand must be a valid segment selector.
 * In protected mode, moving a segment selector into a segment
 * register automatically causes the segment descriptor information
 * associated with that segment selector to be loaded into the
 * hidden (shadow) part of the segment register. While loading this
 * information, the segment selector and segment descriptor
 * information is validated (see the "Operation" algorithm below).
 * The segment descriptor data is obtained from the GDT or LDT entry
 * for the specified segment selector.
 */

lnaddr_t seg_translate(swaddr_t swaddr, size_t len, uint8_t sreg) {
    Log("swaddr = 0x%x", swaddr);
    if (!cpu.cr0.PE)
        return lnaddr_read(swaddr, len);
    Log("segment");
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

