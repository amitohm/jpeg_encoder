#include "jcfunctions.h"

/*******************************************************************************************/

void jc_memset(void *buf, int val,unsigned int size)
{
    memset(buf,val,size);
    return;
}

/*******************************************************************************************/

void jc_put_8_bits(jcstruct_t *s,unsigned char val)
{
    *(s->output_buf++) = val;
    return;
}

/*******************************************************************************************/

void jc_put_16_bits(jcstruct_t *s,unsigned short val)
{
    *(s->output_buf++) = ((val>>8)&0xFF);
    *(s->output_buf++) = (val&0xFF);
    return;
}

/*******************************************************************************************/

void jc_put_32_bits(jcstruct_t *s,unsigned int val)
{
    *(s->output_buf++) = ((val>>24)&0xFF);
    *(s->output_buf++) = ((val>>16)&0xFF);
    *(s->output_buf++) = ((val>>8)&0xFF);
    *(s->output_buf++) = (val&0xFF);
    return;
}

/*******************************************************************************************/
