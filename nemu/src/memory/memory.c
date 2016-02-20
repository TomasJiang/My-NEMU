#include "common.h"
#include "memory/cache.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
    // Log("hwaddr_read: addr = 0x%x", addr);
    return cache_read(addr, len);
	// uint32_t res = dram_read(addr, len) & (~0u >> ((4 - len) << 3));
    // Log("res = 0x%02x", res);
    // return res;
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
    // Log("addr = 0x%x, len = %d, data = 0x%x", addr, len, data);
    cache_write(addr, len, data);
	// dram_write(addr, len, data);
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	hwaddr_write(addr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	return lnaddr_read(addr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_write(addr, len, data);
}

