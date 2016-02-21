#include "common.h"
#include "cpu/reg.h"
#include "memory/l1_cache.h"
#include "memory/segmentation.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

/* Memory accessing interfaces */

static lnaddr_t seg_translate(swaddr_t addr, size_t len, uint8_t sreg) {
    if (!cpu.cr0.PE)
        return addr;
    Log("GDTR.BASE = 0x%x", cpu.gdtr.base);
    SegDesc *segdesc = (SegDesc *)(cpu.gdtr.base + cpu.cs.index);
    Log("0x%x", segdesc->LIMIT_15_0);
    return segdesc->BASE_15_0;
}

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
    // Log("hwaddr_read: addr = 0x%x", addr);
    return L1_cache_read(addr, len);
	// uint32_t res = dram_read(addr, len) & (~0u >> ((4 - len) << 3));
    // return res;
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
    // Log("addr = 0x%x, len = %d, data = 0x%x", addr, len, data);
    L1_cache_write(addr, len, data);
	// dram_write(addr, len, data);
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	hwaddr_write(addr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len, uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
    lnaddr_t lnaddr = seg_translate(addr, len, sreg);
	return lnaddr_read(lnaddr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_write(addr, len, data);
}

