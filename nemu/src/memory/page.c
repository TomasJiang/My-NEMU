#include "common.h"
#include "cpu/reg.h"
#include "memory/page.h"


hwaddr_t page_translation(lnaddr_t addr) {
    PTA page_translation_addr;
    page_translation_addr.val = addr;
    Log("addr = 0x%x", addr);
    Log("PTA.offset = 0x%x", page_translation_addr.offset);
    Log("PTA.page   = 0x%x", page_translation_addr.page);
    Log("PTA.dir    = 0x%x", page_translation_addr.dir);
    PDE *pdir_entry_addr   = (PDE *)((cpu.cr3.PDBR << 12) + (page_translation_addr.dir << 2));
    PTE *ptable_entry_addr = (PTE *)((pdir_entry_addr->page_frame << 12) + (page_translation_addr.page << 2));
    addr = (ptable_entry_addr->page_frame << 12) + page_translation_addr.offset;
    return addr;
}
