#include "cpu/cpu.h"
#include "memory/memory.h"

// translate from linear address to physical address
paddr_t page_translate(laddr_t laddr)
{
#ifndef TLB_ENABLED
#ifdef IA32_PAGE
	uint32_t dir = (laddr >> 22)&0x3ff;
	uint32_t page = (laddr >> 12) & 0x3ff;
	uint32_t offset = laddr & 0xfff;
	

	PDE *page_DIR_ENTRY = (PDE *)((cpu.cr3.pdtr << 12) + (uint32_t)hw_mem);
	page_DIR_ENTRY += dir;
	assert(page_DIR_ENTRY->present == 1);
	PTE *page_TBL_ENTRY = (PTE *)((page_DIR_ENTRY->page_frame << 12) + (uint32_t)hw_mem);
	page_TBL_ENTRY += page;
	assert(page_TBL_ENTRY->present == 1);

	return (page_TBL_ENTRY->page_frame << 12) | (laddr & 0xFFF);
#else
	printf("\nPlease implement page_translate()\n");
	assert(0);
#endif
#else
	return tlb_read(laddr) | (laddr & PAGE_MASK);
#endif
}
