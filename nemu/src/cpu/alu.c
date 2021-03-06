#include "cpu/cpu.h"
#include "stdlib.h"
void set_CF_add(uint32_t result,uint32_t src,size_t data_size){
    result=sign_ext(result&(0xFFFFFFFF>>(32-data_size)),data_size);
    src=sign_ext(src&(0xFFFFFFFF>>(32-data_size)),data_size);
    cpu.eflags.CF=result<src;
}
void set_CF_adc(uint32_t result,uint32_t src,size_t data_size){
    result=sign_ext(result&(0xFFFFFFFF>>(32-data_size)),data_size);
    src=sign_ext(src&(0xFFFFFFFF>>(32-data_size)),data_size);
    cpu.eflags.CF=((result<src)&&(cpu.eflags.CF==0))||((result<=src)&&(cpu.eflags.CF==1));
}

void set_CF_sub(uint32_t dest,uint32_t src,size_t data_size){
    dest=sign_ext(dest&(0xFFFFFFFF>>(32-data_size)),data_size);
    src=sign_ext(src&(0xFFFFFFFF>>(32-data_size)),data_size);
    cpu.eflags.CF=(dest<src);
}
void set_CF_sbb(uint32_t dest,uint32_t src,size_t data_size){
    dest=sign_ext(dest&(0xFFFFFFFF>>(32-data_size)),data_size);
    src=sign_ext(src&(0xFFFFFFFF>>(32-data_size)),data_size);
    if(cpu.eflags.CF==0) cpu.eflags.CF=(dest<src);
    else if(cpu.eflags.CF==1) cpu.eflags.CF=(dest<=src);
}
void set_CFandOF_mul(uint64_t result,size_t data_size){
    if((result>>data_size)==0){
        cpu.eflags.CF=0;
        cpu.eflags.OF=0;    
    }
    else{
        cpu.eflags.CF=1;
        cpu.eflags.OF=1;
    } 
}
void set_ZF(uint32_t result,size_t data_size){
    result=result&(0xFFFFFFFF>>(32-data_size));
    cpu.eflags.ZF=(result==0);
}
void set_SF(uint32_t result,size_t data_size){
    result=sign_ext(result&(0xFFFFFFFF>>(32-data_size)),data_size);
    cpu.eflags.SF=sign(result);
}
void set_PF(uint32_t result){
    int32_t i,count=0;
    result=result&0xFF;
    for(i=0;i<8;i++){
        if(result%2==1){
            count++;
        }
        result=result>>1;
    }
    if(count%2==0) {cpu.eflags.PF=1;}
    else cpu.eflags.PF=0;
}
void set_OF_add(uint32_t result, uint32_t src,uint32_t dest,size_t data_size){
    switch(data_size){
        case 8:
            result=sign_ext(result&0xFF,8);//"and operation" and shift to signed 32bits; 
            src=sign_ext(src&0xFF,8);
            dest=sign_ext(dest&0xFF,8);
            break;
        case 16:   
            result=sign_ext(result&0xFFFF,16);
            src=sign_ext(src&0xFFFF,16);
            dest=sign_ext(dest&0xFFFF,16);
            break; 
        default:break;
    }
    if(sign(src)==sign(dest)){
        if(sign(src)!=sign(result))
            cpu.eflags.OF=1;
        else
            cpu.eflags.OF=0;
    }
    else{
        cpu.eflags.OF=0;
    }
}
void set_OF_sub(uint32_t result, uint32_t src,uint32_t dest,size_t data_size){
    switch(data_size){
        case 8:
            result=sign_ext(result&0xFF,8);//"and operation" and shift to signed 32bits; 
            src=sign_ext(src&0xFF,8);
            dest=sign_ext(dest&0xFF,8);
            break;
        case 16:   
            result=sign_ext(result&0xFFFF,16);
            src=sign_ext(src&0xFFFF,16);
            dest=sign_ext(dest&0xFFFF,16);
            break; 
        default:break;
    }
    if(sign(dest)!=sign(src)){
        if(sign(dest)!=sign(result))
            cpu.eflags.OF=1;
        else
            cpu.eflags.OF=0;
    }
    else{
        cpu.eflags.OF=0;
    }
}
uint32_t alu_add(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_add(src, dest, data_size);
#else
	uint32_t res=0;
    res=dest+src;
    set_CF_add(res,src,data_size); 
    set_PF(res);
    set_ZF(res,data_size);
    set_SF(res,data_size);
    set_OF_add(res,src,dest,data_size);
    return res&(0xFFFFFFFF>>(32-data_size));//shifting and "and opration"
#endif
}

uint32_t alu_adc(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_adc(src, dest, data_size);
#else
	uint32_t res=0;
    res=dest+src+cpu.eflags.CF;
    set_CF_adc(res,src,data_size); 
    set_PF(res);
    set_ZF(res,data_size);
    set_SF(res,data_size);
    set_OF_add(res,src,dest,data_size);
    return res&(0xFFFFFFFF>>(32-data_size));
#endif
}
uint32_t alu_sub(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sub(src, dest, data_size);
#else
    uint32_t res=0;
    res=dest-src;
    set_CF_sub(dest,src,data_size); 
    set_PF(res);
    set_ZF(res,data_size);
    set_SF(res,data_size);
    set_OF_sub(res,src,dest,data_size);
    return res&(0xFFFFFFFF>>(32-data_size));
#endif
}

uint32_t alu_sbb(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sbb(src, dest, data_size);
#else
	uint32_t res=0;
    res=dest-(src+cpu.eflags.CF);
    set_CF_sbb(dest,src,data_size); 
    set_PF(res);
    set_ZF(res,data_size);
    set_SF(res,data_size);
    set_OF_sub(res,src,dest,data_size);
    return res&(0xFFFFFFFF>>(32-data_size));
#endif
}

uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mul(src, dest, data_size);
#else
	uint64_t res=((uint64_t)dest)*((uint64_t)src);
    set_CFandOF_mul(res,data_size); 
    return res&(0xFFFFFFFFFFFFFFFF>>(64-2*(data_size)));
#endif
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imul(src, dest, data_size);
#else
    
	int64_t res=((int64_t)dest)*((int64_t)src);
    return res;//&(0xFFFFFFFFFFFFFFFF>>(64-2*(data_size)))
#endif
}

// need to implement alu_mod before testing
uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_div(src, dest, data_size);
#else
	uint32_t res=dest/src;
    if(src==0) abort();
    return res;//&(0xFFFFFFFF>>(32-data_size));
#endif
}

// need to implement alu_imod before testing
int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_idiv(src, dest, data_size);
#else
	int32_t res=dest/src;
    if(src==0) abort();
    return res;//&(0xFFFFFFFF>>(32-data_size));
#endif
}

uint32_t alu_mod(uint64_t src, uint64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mod(src, dest);
#else
	uint32_t res=dest/src;
    if(src==0) abort();
    uint32_t remain=dest-res*src;
    return remain;
#endif
}

int32_t alu_imod(int64_t src, int64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imod(src, dest);
#else
	int32_t res=dest/src;
    if(src==0) abort();
    int32_t remain=dest-res*src;
    return remain;
#endif
}

uint32_t alu_and(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_and(src, dest, data_size);
#else
	uint32_t res=0;
    res=src&dest;
    cpu.eflags.CF=0;
    set_PF(res);
    set_ZF(res,data_size);
    set_SF(res,data_size);
    cpu.eflags.OF=0;
    return res&(0xFFFFFFFF>>(32-data_size));
#endif
}

uint32_t alu_xor(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_xor(src, dest, data_size);
#else
	uint32_t res=0;
    res=src^dest;
    cpu.eflags.CF=0;
    set_PF(res);
    set_ZF(res,data_size);
    set_SF(res,data_size);
    cpu.eflags.OF=0;
    return res&(0xFFFFFFFF>>(32-data_size));
#endif
}

uint32_t alu_or(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_or(src, dest, data_size);
#else
	uint32_t res=0;
    res=src|dest;
    cpu.eflags.CF=0;
    set_PF(res);
    set_ZF(res,data_size);
    set_SF(res,data_size);
    cpu.eflags.OF=0;
    return res&(0xFFFFFFFF>>(32-data_size));
#endif
}

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shl(src, dest, data_size);
#else
    dest=dest&(0xFFFFFFFF>>(32-data_size));
	uint32_t temp=0;
    temp=src;
    uint32_t res=0;
    res=dest;
    while(temp!=0)
    {
        res=alu_add(res,res,data_size);
        temp=temp-1;
    }    
    set_PF(res);
    set_ZF(res,data_size);
    set_SF(res,data_size);
    if(src==1) cpu.eflags.OF=sign(res);
    return res&(0xFFFFFFFF>>(32-data_size));
#endif
}
uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sal(src, dest, data_size);
#else
    dest=dest&(0xFFFFFFFF>>(32-data_size));
	uint32_t temp=0;
    temp=src;
    uint32_t res=0;
    res=dest;
    while(temp!=0)
    {
        res=alu_add(res,res,data_size);
        temp=temp-1;
    }    
    set_PF(res);
    set_ZF(res,data_size);
    set_SF(res,data_size);
    if(src==1) cpu.eflags.OF=sign(res);
    return res&(0xFFFFFFFF>>(32-data_size));
#endif
}
uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sar(src, dest, data_size);
#else
    uint32_t res=0;
	uint32_t temp=src;
    int32_t mid=sign_ext(dest&0xFFFFFFFF>>(32-data_size),data_size);//expand with sign; 
    while(temp!=0){
        cpu.eflags.CF=mid%2;
        mid=mid>>1;
        temp=temp-1;
    }
    mid=mid&(0xFFFFFFFF>>(32-data_size));
    res=res|mid;
    set_PF(res);
    set_ZF(res,data_size);
    set_SF(res,data_size);
    return res&(0xFFFFFFFF>>(32-data_size));
#endif
}
uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shr(src, dest, data_size);
#else
    dest=dest&(0xFFFFFFFF>>(32-data_size));
	uint32_t temp=0;
    temp=src;
    uint32_t res=0;
    res=dest;
    while(temp!=0)
    {
        cpu.eflags.CF=(sign_ext(res&(0xFFFFFFFF>>(32-data_size)),data_size))%2;
        res=alu_div(2,res,data_size);
        temp=temp-1;
    }    
    set_PF(res);
    set_ZF(res,data_size);
    set_SF(res,data_size);
    if(src==1) cpu.eflags.OF=sign(res);
    return res&(0xFFFFFFFF>>(32-data_size));
#endif
}

