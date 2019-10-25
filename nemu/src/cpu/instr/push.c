#include "cpu/instr.h"

make_instr_func(push)
{
    OPERAND reg;
    reg.type=OPR_REG;
    reg.addr=opcode&0x07;
    reg.data_size=data_size;

    int len=1;

    operand_read(&reg);
    reg.val=reg.val-2;
    operand_write(&reg);

    return len;
}