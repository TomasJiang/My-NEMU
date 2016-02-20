#include "common.h"
#include "memory/cache.h"

void init_cache() {
    int i, j;
    for (i = 0; i < CC_SET_SIZE; ++i)
        for (j = 0; j < CC_ROW_SIZE; ++j)
            cache[i][j].valid = false;
}

uint32_t dram_read(hwaddr_t, size_t);

uint32_t cache_read(uint32_t addr, size_t len, bool *success) {
    Log("addr = 0x%x, len = %d", addr, len);
    *success = false;
    uint32_t tag = addr >> (CB_WIDTH + CC_SET_WIDTH);
    uint32_t set_num = (addr & CC_SET_MASK) >> CB_WIDTH;
    uint32_t offset = addr & CB_ADDR_MASK;
    int i;
    for (i = 0; i < CC_ROW_SIZE; ++i) {
        if (cache[set_num][i].valid && cache[set_num][i].tag == tag) {
            *success = true;
            return cache[set_num][i].block[offset];
        }
    }
    return 0;
    // return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
}
