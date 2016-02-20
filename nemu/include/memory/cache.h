#ifndef __CACHE_H__
#define __CACHE_H__

// cache parameters
#define CC_WIDTH      16
#define CC_SET_WIDTH  3
#define CC_BLOCK_WIDTH      6

#define CC_ROW_WIDTH  (CC_WIDTH - CC_SET_WIDTH - CC_BLOCK_WIDTH)
#define CC_SIZE       (1 << CC_WIDTH)
#define CC_SET_SIZE   (1 << CC_SET_WIDTH)
#define CC_ROW_SIZE   (1 << CC_ROW_WIDTH)
#define CC_BLOCK_SIZE       (1 << CC_BLOCK_WIDTH)
#define CC_SET_MASK   (((1 << CC_SET_WIDTH) - 1) << CC_BLOCK_WIDTH)
#define CC_BLOCK_MASK  ((1 << CC_BLOCK_WIDTH) - 1)

typedef struct cache_block {
    bool     valid;
    uint32_t tag;
    uint8_t  block[CC_BLOCK_SIZE];

    // int  (*read ) (struct cache_block *this, uint32_t addr);
    // void (*write) (struct cache_block *this, uint32_t addr, uint32_t data);
} cache_block;

cache_block cache[CC_SET_SIZE][CC_ROW_SIZE];

void init_cache();
uint32_t cache_read(uint32_t addr, size_t len);
void cache_write(uint32_t addr, size_t len, uint32_t data);


#endif
