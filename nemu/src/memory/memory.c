#include "common.h"
#include "memory/l1_cache.h"
//#include "memory/segmentation.h"
//#include "memory/page.h"
#include "cpu/reg.h"
#include "x86-inc/mmu.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
    // Log("hwaddr_read: addr = 0x%x", addr);
    return L1_cache_read(addr, len);
	// uint32_t res = dram_read(addr, len) & (~0u >> ((4 - len) << 3));
    // return res;
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
    // Log("addr = 0x%x, len = %d, data = 0x%x", addr, len, data);
    L1_cache_write(addr, len, data);
	// dram_write(addr, len, data);
}

hwaddr_t page_translate(lnaddr_t addr, size_t len) {
    LinearAddress lnaddr;
    lnaddr.val = addr;
    Log("addr = 0x%x", addr);
    Log("lnaddr.offset = 0x%x", lnaddr.offset);
    Log("lnaddr.page   = 0x%x", lnaddr.page);
    Log("lnaddr.dir    = 0x%x", lnaddr.dir);
    PDE *pdir_entry_addr   = (PDE *)((cpu.cr3.page_directory_base << 12) + (lnaddr.dir << 2));
    Assert(pdir_entry_addr->present, "Page directory entry's present bit is 0.");
    PTE *ptable_entry_addr = (PTE *)((pdir_entry_addr->page_frame << 12) + (lnaddr.page << 2));
    Assert(ptable_entry_addr->present, "Page table entry's present bit is 0.");
    addr = (ptable_entry_addr->page_frame << 12) + lnaddr.offset;
    return addr;
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
#ifdef IA32_PAGE
    if (cpu.cr0.PE && cpu.cr0.PG)
        addr = page_translate(addr);
#endif
	return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
#ifdef IA32_PAGE
    if (cpu.cr0.PE && cpu.cr0.PG)
        addr = page_translate(addr);
#endif
	hwaddr_write(addr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len, uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
#ifdef IA32_SEG
    if (cpu.cr0.PE)
        addr = seg_translate(addr, sreg);
#endif
	return lnaddr_read(addr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data, uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
#ifdef IA32_SEG
    if (cpu.cr0.PE)
        addr = seg_translate(addr, sreg);
#endif
	lnaddr_write(addr, len, data);
}

