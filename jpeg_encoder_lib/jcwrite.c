#include "jcfunctions.h"

/*******************************************************************************************/

GLOBAL(void) jc_memset(void *buf,INT32 val,UINT32 size)
{
	memset(buf,val,size);
	
	return;
}

/*******************************************************************************************/

GLOBAL(void) jc_put_8_bits(jcstruct_t *s,UINT8 val)
{
	*(s->output_buf++) = val;
	
	return;
}

/*******************************************************************************************/

GLOBAL(void) jc_put_16_bits(jcstruct_t *s,UINT16 val)
{
	*(s->output_buf++) = ((val>>8)&0xFF);
	*(s->output_buf++) = (val&0xFF);
	
	return;
}

/*******************************************************************************************/

GLOBAL(void) jc_put_32_bits(jcstruct_t *s,UINT32 val)
{
	*(s->output_buf++) = ((val>>24)&0xFF);
	*(s->output_buf++) = ((val>>16)&0xFF);
	*(s->output_buf++) = ((val>>8)&0xFF);
	*(s->output_buf++) = (val&0xFF);
	
	return;
}

/*******************************************************************************************/
