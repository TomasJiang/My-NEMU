#include <stdlib.h>
#include "common.h"
#include "memory/cache.h"

void init_cache() {
    // Log("init_cache");
    // Log("CC_SIZE\t\t%u", CC_SIZE);
    // Log("CC_SET_SIZE\t%u", CC_SET_SIZE);
    // Log("CC_ROW_SIZE\t%u", CC_ROW_SIZE);
    // Log("CB_SIZE\t\t%u", CB_SIZE);
    // Log("CC_SET_MASK\t%x", CC_SET_MASK);
    // Log("CB_BLOCK_MASK\t%x", CB_BLOCK_MASK);
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
        buf[i] = dram_read((addr & ~CB_BLOCK_MASK) + i, 1);
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
    // Log("row_num = %u", row_num);
    int i;
    for (i = 0; i < CB_SIZE; ++i) {
        //Log("i = %d", i);
        cache[set_num][row_num].block[i] = buf[i];
    }
    // Log("tag = %u", tag);
    cache[set_num][row_num].tag   = tag;
    cache[set_num][row_num].valid = true;
}

uint32_t cache_read(uint32_t addr, size_t len) {
    Assert(len == 1 || len == 2 || len == 4, "cache read not 1/2/4");
    // Log("cache_read: addr = 0x%x, len = %d", addr, len);

    bool is_hit = false;
    uint8_t  buf[2 * CB_SIZE];
    uint32_t tag     = addr >> (CB_WIDTH + CC_SET_WIDTH);
    // Log("tag = 0x%x", tag);
    uint32_t set_num = (addr & CC_SET_MASK) >> CB_WIDTH;
    // Log("set_num = 0x%x", set_num);
    uint32_t offset  = addr & CB_BLOCK_MASK;
    // Log("addr = 0x%x, CB_BLOCK_MASK = 0x%x", addr, CB_BLOCK_MASK);
    // Log("offset  = 0x%x", offset);


    int i;
    for (i = 0; i < CC_ROW_SIZE; ++i) {
        if (cache[set_num][i].valid && cache[set_num][i].tag == tag) {
            // Log("hit");
            is_hit = true;
            memcpy(buf, cache[set_num][i].block, CB_SIZE);
        }
    }
    if (!is_hit) {
        // Log("missed");
        dram_read_block(addr & ~CB_BLOCK_MASK, buf);
        find_row_write(buf, set_num, tag);
    }


    if (offset + len > CB_SIZE) {
        set_num = (set_num + 1) % CC_SET_SIZE;
        is_hit = false;
        for (i = 0; i < CC_ROW_SIZE; ++i) {
            if (cache[set_num][i].valid && cache[set_num][i].tag == tag) {
                // Log("hit");
                is_hit = true;
                memcpy(buf + CB_SIZE, cache[set_num][i].block, CB_SIZE);
            }
        }

        if (!is_hit) {
            // Log("missed");
            dram_read_block((addr & ~CB_BLOCK_MASK) + CB_SIZE, buf + CB_SIZE);
            find_row_write(buf + CB_SIZE, set_num, tag);
        }
    }

    // uint32_t res = unalign_rw(buf + offset, 4) & (~0u >> ((4 - len) << 3));
    uint32_t res = unalign_rw(buf + offset, 4);
    // Log("res = 0x%x", res);
    return res & (~0u >> ((4 - len) << 3));
}

void cache_write(uint32_t addr, size_t len, uint32_t data) {
    // Log("cache_write: addr = 0x%x, len = %d, data = 0x%x", addr, len, data);
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
