#include <stdlib.h>
#include "common.h"
#include "misc.h"
#include "memory/l2_cache.h"

uint32_t  dram_read(hwaddr_t, size_t);
uint32_t dram_write(hwaddr_t, size_t, uint32_t);

void init_L2_cache() {
    // Log("init_L2_cache");
    // Log("L2_CC_SIZE\t%u", L2_CC_SIZE);
    // Log("L2_CC_SET_SIZE\t%u", L2_CC_SET_SIZE);
    // Log("L2_CC_ROW_SIZE\t%u", L2_CC_ROW_SIZE);
    // Log("L2_CC_BLOCK_SIZE\t%u", L2_CC_BLOCK_SIZE);
    // Log("L2_CC_SET_MASK\t%x", L2_CC_SET_MASK);
    // Log("L2_CC_BLOCK_MASK\t%x", L2_CC_BLOCK_MASK);
    int i, j;
    for (i = 0; i < L2_CC_SET_SIZE; ++i)
        for (j = 0; j < L2_CC_ROW_SIZE; ++j) {
            L2_cache[i][j].valid = false;
            L2_cache[i][j].dirty = false;
        }
}

static void read_block(uint32_t addr, uint8_t *buf) {
    int i;
    for (i = 0; i < L2_CC_BLOCK_SIZE; ++i) {
        buf[i] = dram_read((addr & ~L2_CC_BLOCK_MASK) + i, 1);
    }
}

static uint32_t find_row(uint32_t set_num) {
    uint32_t i;
    for (i = 0; i < L2_CC_ROW_SIZE; ++i) {
        if (!L2_cache[set_num][i].valid) {
            return i;
        }
    }

    return rand() % L2_CC_ROW_SIZE;
}

static void find_row_write(uint8_t *buf, uint32_t set_num, uint32_t tag) {
    uint32_t row_num = find_row(set_num);
    if (L2_cache[set_num][row_num].valid
            && L2_cache[set_num][row_num].dirty) {
        // write back
        uint32_t addr = ((((tag << L2_CC_SET_WIDTH)
                        + set_num) << L2_CC_ROW_WIDTH) + row_num) << L2_CC_BLOCK_WIDTH;
        int i;
        for (i = 0; i < L2_CC_BLOCK_SIZE; ++i) {
            dram_write(addr + i, 1, buf[i]);
        }
    }
    memcpy(L2_cache[set_num][row_num].block, buf, L2_CC_BLOCK_SIZE);
    L2_cache[set_num][row_num].tag   = tag;
    L2_cache[set_num][row_num].valid = true;
    L2_cache[set_num][row_num].dirty = false;
}

// static void print_buf(uint8_t *buf) {
//     int i;
//     for (i = 0; i < L2_CC_BLOCK_SIZE; ++i) {
//         printf("%02x ", buf[i]);
//     }
//     printf("\n");
// }

static void L2_cache_read_prime(uint32_t addr, uint8_t *buf, uint32_t set_num, uint32_t tag) {
    bool is_hit = false;
    int i;
    for (i = 0; i < L2_CC_ROW_SIZE; ++i) {
        if (L2_cache[set_num][i].valid && L2_cache[set_num][i].tag == tag) {
            // Log("hit");
            is_hit = true;
            memcpy(buf, L2_cache[set_num][i].block, L2_CC_BLOCK_SIZE);
        }
    }
    if (!is_hit) {
        // Log("missed");
        read_block(addr & ~L2_CC_BLOCK_MASK, buf);
        find_row_write(buf, set_num, tag);
    }
}

uint32_t L2_cache_read(uint32_t addr, size_t len) {
    Assert(len == 1 || len == 2 || len == 4, "L2_cache read not 1/2/4");
    // Log("L2_cache_read: addr = 0x%x, len = %d", addr, len);

    uint8_t  buf[2 * L2_CC_BLOCK_SIZE];
    uint32_t tag     = addr >> (L2_CC_BLOCK_WIDTH + L2_CC_SET_WIDTH);
    uint32_t set_num = (addr & L2_CC_SET_MASK) >> L2_CC_BLOCK_WIDTH;
    uint32_t offset  = addr & L2_CC_BLOCK_MASK;

    L2_cache_read_prime(addr, buf, set_num, tag);
    if (offset + len > L2_CC_BLOCK_SIZE) {
        L2_cache_read_prime(addr + L2_CC_BLOCK_SIZE, // next block in memory
                buf + L2_CC_BLOCK_SIZE, (set_num + 1) % L2_CC_SET_SIZE, tag);
    }

    uint32_t res = unalign_rw(buf + offset, 4);
    return res & (~0u >> ((4 - len) << 3));
}

static void L2_cache_write_prime(uint32_t addr, uint8_t *buf, uint8_t *mask, uint32_t set_num, uint32_t tag) {
    // Log("addr = 0x%x", addr);
    bool is_hit = false;
    int i;
    for (i = 0; i < L2_CC_ROW_SIZE; ++i) {
        if (L2_cache[set_num][i].valid && L2_cache[set_num][i].tag == tag) {
            // Log("hit");
            is_hit = true;
            memcpy_with_mask(L2_cache[set_num][i].block, buf, L2_CC_BLOCK_SIZE, mask);
            L2_cache[set_num][i].dirty = true;
        }
    }
    if (!is_hit) { // write allocate
        // Log("missed");
        // print_buf(mask);
        int i;
        for (i = 0; i < L2_CC_BLOCK_SIZE; ++i) {
            if (mask[i]) {
                // uint32_t temp = (addr & ~L2_CC_BLOCK_MASK) + i;
                // Log("addr = 0x%x, buf[%d] = %02x", temp, i, buf[i]);
                dram_write((addr & ~L2_CC_BLOCK_MASK) + i, 1, buf[i]);
            }
        } // write dram
        read_block(addr, buf);
        find_row_write(buf, set_num, tag);
        // write cache
    }
}

void L2_cache_write(uint32_t addr, size_t len, uint32_t data) {
    // Log("L2_cache_write: addr = 0x%x, len = %d, data = 0x%x", addr, len, data);
    uint32_t tag     = addr >> (L2_CC_BLOCK_WIDTH + L2_CC_SET_WIDTH);
    uint32_t set_num = (addr & L2_CC_SET_MASK) >> L2_CC_BLOCK_WIDTH;
    uint32_t offset  = addr & L2_CC_BLOCK_MASK;

    uint8_t  buf[2 * L2_CC_BLOCK_SIZE];
    uint8_t mask[2 * L2_CC_BLOCK_SIZE];
    memset(mask, 0, 2 * L2_CC_BLOCK_SIZE);
    *(uint32_t *)(buf + offset) = data;
    memset(mask + offset, 1, len);

    // print_buf(buf);
    // print_buf(mask);

    L2_cache_write_prime(addr, buf, mask, set_num, tag);
    if (offset + len > L2_CC_BLOCK_SIZE) {
        L2_cache_write_prime(addr + L2_CC_BLOCK_SIZE,
                buf + L2_CC_BLOCK_SIZE, mask, (set_num + 1) % L2_CC_SET_SIZE, tag);
    }
}
