#include "cpu/intr.h"
#include "cpu/instr.h"

make_instr_func(int_)
{
    OPERAND rel;
    rel.data_size=8;
    rel.type=OPR_IMM;
    rel.addr=eip+1;
    rel.sreg=SREG_CS;
    operand_read(&rel);
    print_asm_1("int","",2,&rel);
    raise_sw_intr(rel.val);
    return 0;
}