#include "cpu/cpu.h"
#include "memory/memory.h"

// return the linear address from the virtual address and segment selector
uint32_t segment_translate(uint32_t offset, uint8_t sreg)
{
	/* TODO: perform segment translation from virtual address to linear address
	 * by reading the invisible part of the segment register 'sreg'
	 */
	#ifdef IA32_SEG
		uint32_t base=cpu.segReg[sreg].base;
		base=base+offset;
		return base;
	#else 
		return 0;
	#endif
}

// load the invisible part of a segment register
void load_sreg(uint8_t sreg)
{
	/* TODO: load the invisibile part of the segment register 'sreg' by reading the GDT.
	 * The visible part of 'sreg' should be assigned by mov or ljmp already.
	 */
	#ifdef IA32_SEG
		uint32_t addr=(uint32_t)hw_mem+8*cpu.segReg[sreg].index+cpu.gdtr.base;
		SegDesc* segdesc=(void*)addr;
		uint32_t privilege_level=segdesc->privilege_level;
		uint32_t base=(segdesc->base_31_24<<24)+(segdesc->base_23_16<<16)+segdesc->base_15_0;
		uint32_t limit=(segdesc->limit_19_16<<16)+segdesc->limit_15_0;
		assert(base==0);
		assert(limit==0xfffff);
		assert(segdesc->granularity==1);
		cpu.segReg[sreg].privilege_level=privilege_level;
		cpu.segReg[sreg].base=base;
		cpu.segReg[sreg].limit=limit;
	#endif
}
