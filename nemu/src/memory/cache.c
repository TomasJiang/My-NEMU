#include <stdlib.h>
#include "common.h"
#include "memory/cache.h"

void init_cache() {
    Log("init_cache");
    int i, j;
    for (i = 0; i < CC_SET_SIZE; ++i)
        for (j = 0; j < CC_ROW_SIZE; ++j)
            cache[i][j].valid = false;
}

uint32_t  dram_read(hwaddr_t, size_t);
uint32_t dram_write(hwaddr_t, size_t, uint32_t);

void dram_read_block(uint32_t addr, uint8_t *buf) {
    int i;
    for (i = 0; i < CB_SIZE; ++i) {
        buf[i] = dram_read(addr & (~CB_BLOCK_MASK + i), 1);
    }
}

uint32_t find_row(uint32_t set_num) {
    uint32_t i;
    for (i = 0; i < CC_ROW_SIZE; ++i) {
        if (!cache[set_num][i].valid) {
            return i;
        }
    }

    // choose one to replace
    // srand(time(0));
    return rand() % CC_ROW_SIZE;
}

void find_row_write(uint8_t *buf, uint32_t set_num, uint32_t tag) {
    uint32_t row_num = find_row(set_num);
    int i;
    for (i = 0; i < CB_SIZE; ++i) {
        cache[set_num][row_num].block[i] = buf[i];
    }
    cache[set_num][row_num].tag   = tag;
    cache[set_num][row_num].valid = true;
}

uint32_t cache_read(uint32_t addr, size_t len) {
    Log("cache_read: addr = 0x%x, len = %d", addr, len);
    uint32_t tag     = addr >> (CB_WIDTH + CC_SET_WIDTH);
    uint32_t set_num = (addr & CC_SET_MASK) >> CB_WIDTH;
    uint32_t offset  = addr & CB_BLOCK_MASK;
    int i;
    for (i = 0; i < CC_ROW_SIZE; ++i) {
        if (cache[set_num][i].valid && cache[set_num][i].tag == tag) {
            return cache[set_num][i].block[offset];
        }
    }

    // missed
    Log("missed");
    uint8_t buf[2 * CB_SIZE];
    dram_read_block(addr & ~CB_BLOCK_MASK, buf);
    find_row_write(buf, set_num, tag);

    if (offset + len > CB_SIZE) {
        dram_read_block((addr & ~CB_BLOCK_MASK) + CB_SIZE, buf + CB_SIZE);
        find_row_write(buf + CB_SIZE, set_num, tag);
    }

    return unalign_rw(buf + offset, 4);
}

void cache_write(uint32_t addr, size_t len, uint32_t data) {
    Log("cache_write: addr = 0x%x, len = %d, data = %u", addr, len, data);
    uint32_t tag     = addr >> (CB_WIDTH + CC_SET_WIDTH);
    uint32_t set_num = (addr & CC_SET_MASK) >> CB_WIDTH;
    uint32_t offset  = addr & CB_BLOCK_MASK;
    int i;
    for (i = 0; i < CC_ROW_SIZE; ++i) {
        if (cache[set_num][i].valid && cache[set_num][i].tag == tag) {
            cache[set_num][i].block[offset] = data;
            break;
        }
    }
    dram_write(addr, len, data);
}
