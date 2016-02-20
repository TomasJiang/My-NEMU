#ifndef __L2_CACHE_H__
#define __L2_CACHE_H__

// cache parameters
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

typedef struct L2_cache_block {
    bool     valid;
    bool     dirty;
    uint32_t tag;
    uint8_t  block[L2_CC_BLOCK_SIZE];
} L2_cache_block;

L2_cache_block L2_cache[L2_CC_SET_SIZE][L2_CC_ROW_SIZE];

void init_L2_cache();
uint32_t L2_cache_read(uint32_t addr, size_t len);
void L2_cache_write(uint32_t addr, size_t len, uint32_t data);

#endif
