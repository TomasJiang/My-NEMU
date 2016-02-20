#ifndef __CACHE_H__
#define __CACHE_H__

// cache parameters
#define L1_CC_WIDTH         16
#define L1_CC_SET_WIDTH     3
#define L1_CC_BLOCK_WIDTH   6

#define L1_CC_ROW_WIDTH  (L1_CC_WIDTH - L1_CC_SET_WIDTH - L1_CC_BLOCK_WIDTH)
#define L1_CC_SIZE       (1 << L1_CC_WIDTH)
#define L1_CC_SET_SIZE   (1 << L1_CC_SET_WIDTH)
#define L1_CC_ROW_SIZE   (1 << L1_CC_ROW_WIDTH)
#define L1_CC_BLOCK_SIZE (1 << L1_CC_BLOCK_WIDTH)
#define L1_CC_SET_MASK   (((1 << L1_CC_SET_WIDTH) - 1) << L1_CC_BLOCK_WIDTH)
#define L1_CC_BLOCK_MASK  ((1 << L1_CC_BLOCK_WIDTH) - 1)

#define L2_CC_WIDTH         22
#define L2_CC_SET_WIDTH     4
#define L2_CC_BLOCK_WIDTH   6

#define L2_CC_ROW_WIDTH  (L2_CC_WIDTH - L2_CC_SET_WIDTH - L2_CC_BLOCK_WIDTH)
#define L2_CC_SIZE       (1 << L2_CC_WIDTH)
#define L2_CC_SET_SIZE   (1 << L2_CC_SET_WIDTH)
#define L2_CC_ROW_SIZE   (1 << L2_CC_ROW_WIDTH)
#define L2_CC_BLOCK_SIZE (1 << L2_CC_BLOCK_WIDTH)
#define L2_CC_SET_MASK   (((1 << L2_CC_SET_WIDTH) - 1) << L2_CC_BLOCK_WIDTH)
#define L2_CC_BLOCK_MASK  ((1 << L2_CC_BLOCK_WIDTH) - 1)

typedef struct cache_block {
    bool     valid;
    uint32_t tag;
    uint8_t  block[L1_CC_BLOCK_SIZE];
} cache_block;

cache_block L1_cache[L1_CC_SET_SIZE][L1_CC_ROW_SIZE];
cache_block L2_cache[L2_CC_SET_SIZE][L2_CC_ROW_SIZE];

void init_L1_cache();
uint32_t L1_cache_read(uint32_t addr, size_t len);
void L1_cache_write(uint32_t addr, size_t len, uint32_t data);

void init_L2_cache();
uint32_t L2_cache_read(uint32_t addr, size_t len);
void L2_cache_write(uint32_t addr, size_t len, uint32_t data);

#endif
