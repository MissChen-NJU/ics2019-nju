#include "cpu/instr.h"

static void instr_execute_1op()
{
    opr_dest.type = OPR_MEM;
    opr_dest.sreg = SREG_DS; //data section;
    opr_dest.addr = cpu.esp;
    operand_read(&opr_dest);
    opr_src.val=opr_dest.val;
    operand_write(&opr_dest);
    cpu.esp+=32/8;
}

make_instr_impl_1op(pop, r, v);
make_instr_impl_1op(pop, rm, v);
make_instr_impl_1op(pop, i, b);
make_instr_impl_1op(pop, i, v);
