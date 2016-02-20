#ifndef __CACHE_H__
#define __CACHE_H__

// cache parameters
#define L1_CC_WIDTH      16
#define L1_CC_SET_WIDTH  3
#define L1_CC_BLOCK_WIDTH      6

#define L1_CC_ROW_WIDTH  (L1_CC_WIDTH - L1_CC_SET_WIDTH - L1_CC_BLOCK_WIDTH)
#define L1_CC_SIZE       (1 << L1_CC_WIDTH)
#define L1_CC_SET_SIZE   (1 << L1_CC_SET_WIDTH)
#define L1_CC_ROW_SIZE   (1 << L1_CC_ROW_WIDTH)
#define L1_CC_BLOCK_SIZE       (1 << L1_CC_BLOCK_WIDTH)
#define L1_CC_SET_MASK   (((1 << L1_CC_SET_WIDTH) - 1) << L1_CC_BLOCK_WIDTH)
#define L1_CC_BLOCK_MASK  ((1 << L1_CC_BLOCK_WIDTH) - 1)

typedef struct cache_block {
    bool     valid;
    uint32_t tag;
    uint8_t  block[L1_CC_BLOCK_SIZE];

    // int  (*read ) (struct cache_block *this, uint32_t addr);
    // void (*write) (struct cache_block *this, uint32_t addr, uint32_t data);
} cache_block;

cache_block cache[L1_CC_SET_SIZE][L1_CC_ROW_SIZE];

void init_cache();
uint32_t cache_read(uint32_t addr, size_t len);
void cache_write(uint32_t addr, size_t len, uint32_t data);


#endif
