#include "cpu/instr.h"

static void instr_excute_2op()
{
    operand_read(&opr_src);
    operand_read(&opr_dest);
    opr_src.val=sign_ext(opr_src.val,opr_src.data_size);
    opr_dest.val=sign_ext(opr_dest.val,opr_dest.data_size);
    opr_dest.val=alu_sub(opr_src.val,opr_dest.val,data_size);
}

make_instr_impl_2op(sub,r,rm,b)
make_instr_impl_2op(sub,r,rm,v)
make_instr_impl_2op(sub,rm,r,b)
make_instr_impl_2op(sub,rm,r,v)
make_instr_impl_2op(sub,i,rm,b)
make_instr_impl_2op(sub,i,rm,v)
make_instr_impl_2op(sub,r,rm,bv)


