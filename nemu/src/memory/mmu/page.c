#include "cpu/cpu.h"
#include "memory/memory.h"

// translate from linear address to physical address
paddr_t page_translate(laddr_t laddr)
{
#ifndef TLB_ENABLED
	#ifdef IA32_PAGE
		PDE *page_DIR_ENTRY = (PDE *)((cpu.cr3.pdtr << 12) + (uint32_t)hw_mem);
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
