#include "cpu/cpu.h"
#include "memory/memory.h"

// translate from linear address to physical address
paddr_t page_translate(laddr_t laddr)
{
#ifndef TLB_ENABLED
#ifdef IA32_PAGE
	/*uint32_t dir = laddr >> 22;
		uint32_t page = (laddr >> 12) & 0x3ff;
		uint32_t offset = laddr & 0xfff;
		PDE *pdir = (PDE *)(hw_mem + (cpu.cr3.pdtr << 12) + (dir << 2));
		PTE *ptable = (PTE *)(hw_mem + (pdir->page_frame << 12) + (page << 2));
		assert(pdir->present == 1);
		assert(ptable->present == 1);
		return (ptable->page_frame << 12) + offset;*/
	PDE *page_DIR_ENTRY = (PDE *)((cpu.cr3.PDBR << 12) + (uint32_t)hw_mem);
	page_DIR_ENTRY += ((laddr >> 22) & 0x3FF);
	assert(page_DIR_ENTRY->present == 1);
	PTE *page_TBL_ENTRY = (PTE *)((page_DIR_ENTRY->page_frame << 12) + (uint32_t)hw_mem);
	page_TBL_ENTRY += ((laddr >> 12) & 0x3FF);
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
