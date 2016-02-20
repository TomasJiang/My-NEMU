#include <stdlib.h>
#include "common.h"
#include "misc.h"
#include "memory/cache.h"

uint32_t  dram_read(hwaddr_t, size_t);
uint32_t dram_write(hwaddr_t, size_t, uint32_t);

void init_L1_cache() {
    // Log("init_L1_cache");
    // Log("L1_CC_SIZE\t\t%u", L1_CC_SIZE);
    // Log("L1_CC_SET_SIZE\t%u", L1_CC_SET_SIZE);
    // Log("L1_CC_ROW_SIZE\t%u", L1_CC_ROW_SIZE);
    // Log("L1_CC_BLOCK_SIZE\t\t%u", CB_SIZE);
    // Log("L1_CC_SET_MASK\t%x", L1_CC_SET_MASK);
    // Log("L1_CC_BLOCK_MASK\t%x", L1_CC_BLOCK_MASK);
    int i, j;
    for (i = 0; i < L1_CC_SET_SIZE; ++i)
        for (j = 0; j < L1_CC_ROW_SIZE; ++j)
            L1_cache[i][j].valid = false;
}

static void dram_read_block(uint32_t addr, uint8_t *buf) {
    int i;
    for (i = 0; i < L1_CC_BLOCK_SIZE; ++i) {
        buf[i] = dram_read((addr & ~L1_CC_BLOCK_MASK) + i, 1);
    }
}

static uint32_t find_row(uint32_t set_num) {
    uint32_t i;
    for (i = 0; i < L1_CC_ROW_SIZE; ++i) {
        if (!L1_cache[set_num][i].valid) {
            return i;
        }
    }

    // choose one to replace
    // srand(time(0));
    return rand() % L1_CC_ROW_SIZE;
}

static void find_row_write(uint8_t *buf, uint32_t set_num, uint32_t tag) {
    uint32_t row_num = find_row(set_num);
    memcpy(L1_cache[set_num][row_num].block, buf, L1_CC_BLOCK_SIZE);
    L1_cache[set_num][row_num].tag   = tag;
    L1_cache[set_num][row_num].valid = true;
}

// static void print_buf(uint8_t *buf) {
//     int i;
//     for (i = 0; i < L1_CC_BLOCK_SIZE; ++i) {
//         printf("%02x ", buf[i]);
//     }
//     printf("\n");
// }

static void cache_read_prime(uint32_t addr, uint8_t *buf, uint32_t set_num, uint32_t tag) {
    bool is_hit = false;
    int i;
    for (i = 0; i < L1_CC_ROW_SIZE; ++i) {
        if (L1_cache[set_num][i].valid && L1_cache[set_num][i].tag == tag) {
            // Log("hit");
            is_hit = true;
            memcpy(buf, L1_cache[set_num][i].block, L1_CC_BLOCK_SIZE);
        }
    }
    if (!is_hit) {
        // Log("missed");
        dram_read_block(addr & ~L1_CC_BLOCK_MASK, buf);
        find_row_write(buf, set_num, tag);
    }
}

uint32_t L1_cache_read(uint32_t addr, size_t len) {
    Assert(len == 1 || len == 2 || len == 4, "L1_cache read not 1/2/4");
    // Log("L1_cache_read: addr = 0x%x, len = %d", addr, len);

    uint8_t  buf[2 * L1_CC_BLOCK_SIZE];
    uint32_t tag     = addr >> (L1_CC_BLOCK_WIDTH + L1_CC_SET_WIDTH);
    uint32_t set_num = (addr & L1_CC_SET_MASK) >> L1_CC_BLOCK_WIDTH;
    uint32_t offset  = addr & L1_CC_BLOCK_MASK;

    cache_read_prime(addr, buf, set_num, tag);
    if (offset + len > L1_CC_BLOCK_SIZE) {
        cache_read_prime(addr + L1_CC_BLOCK_SIZE, // next block in memory
                buf + L1_CC_BLOCK_SIZE, (set_num + 1) % L1_CC_SET_SIZE, tag);
    }


    uint32_t res = unalign_rw(buf + offset, 4);
    return res & (~0u >> ((4 - len) << 3));
}

static void cache_write_prime(uint32_t addr, uint8_t *buf, uint8_t *mask, uint32_t set_num, uint32_t tag) {
    int i;
    for (i = 0; i < L1_CC_ROW_SIZE; ++i) {
        if (L1_cache[set_num][i].valid && L1_cache[set_num][i].tag == tag) {
            memcpy_with_mask(L1_cache[set_num][i].block, buf, L1_CC_BLOCK_SIZE, mask);
        }
    }
}

void L1_cache_write(uint32_t addr, size_t len, uint32_t data) {
    // Log("L1_cache_write: addr = 0x%x, len = %d, data = 0x%x", addr, len, data);
    uint32_t tag     = addr >> (L1_CC_BLOCK_WIDTH + L1_CC_SET_WIDTH);
    uint32_t set_num = (addr & L1_CC_SET_MASK) >> L1_CC_BLOCK_WIDTH;
    uint32_t offset  = addr & L1_CC_BLOCK_MASK;

    uint8_t  buf[2 * L1_CC_BLOCK_SIZE];
    uint8_t mask[2 * L1_CC_BLOCK_SIZE];
    memset(mask, 0, 2 * L1_CC_BLOCK_SIZE);
    *(uint32_t *)(buf + offset) = data;
    memset(mask + offset, 1, len);

    cache_write_prime(addr, buf, mask, set_num, tag);
    if (offset + len > L1_CC_BLOCK_SIZE) {
        cache_write_prime(addr + L1_CC_BLOCK_SIZE,
                buf + L1_CC_BLOCK_SIZE, mask, (set_num + 1) % L1_CC_SET_SIZE, tag);
    }

    dram_write(addr, len, data);
}
