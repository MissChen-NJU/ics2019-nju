#include "cpu/instr.h"

static void instr_excute_1op()
{
    operand_read(&opr_src);
    cpu.esp=cpu.esp-data_size/8;//top point<-top point-2;
    opr_dest.type=opr_MEM;
    opr_dest.sreg=SREG_DS;//data section;
    opr_dest.addr=cpu.esp;
    opr_dest.val=opr_src.val;
    operand_write(&opr_dest);
}

make_instr_impl_1op(push,r,v);
make_instr_impl_1op(push,rm,v);
make_instr_impl_1op(push,i,v)

make_instr_func(push_i_b)
{
    int len=1;
    len++;
    
    opr_src.data_size=8;
    opr_src.type=OPR_IMM;
    opr_src.sreg=SREG_CS;
    opr_src.addr=eip+1;
    print_asm_1("push","b",len,&opr_src);
    cpu.esp=cpu.esp-data_size/8;

    oprand_read(&opr_src);
    opr_dest.val=sign_ext(opr_src.val,8);
    opr_dest.data_size=data_size;
    opr_dest.type=opr_MEM;
    opr_dest.sreg=SREG_DS;//data section;
    opr_dest.addr=cpu.esp;
    opr_dest.val=opr_src.val;
    operand_write(&opr_dest);

    return len;
}
