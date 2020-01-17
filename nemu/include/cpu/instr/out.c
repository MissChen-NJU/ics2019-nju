#include "cpu/instr.h"
#include "device/port_io.h"

make_instr_func(out_b)
{
    print_asm_0("out","",1);
    pio_write(cpu.edx,1,cpu.eax);
    return 1;
}

make_instr_func(out_v)
{
    print_asm_0("out",(data_size==16)?"w":"l",1);
    pio_write(cpu.edx,data_szie/8,cpu.eax);
    return 1;
}