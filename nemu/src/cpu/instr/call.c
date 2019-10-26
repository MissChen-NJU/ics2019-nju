#include "cpu/instr.h"

make_instr_func(call_naer)
{
    OPERAND rel,mem;
    rel.data_size=data_szie;
    rel.type=OPR_IMM;
    rel.sreg=SREG_CS;
    rel.addr=cpu.eip+1;
    operand_read(&rel);
    print_asm_1("call","",1+data_size/8,&rel);
    cpu.esp=cpu.esp-data_size/8;
    mem.data_szie=data_szie;
    mem.type=OPR_MEM;
    mem.sreg=SREG_DS;
    mem.addr=cpu.esp;
    mem.val=cpu.eip+1+data_size/8;
    operand_write(&mem);
    int offset=sign_ext(rel.val,data_size);
    cpu.eip=cpu.eip+offset+1+data_size/8;
    return 0;

}