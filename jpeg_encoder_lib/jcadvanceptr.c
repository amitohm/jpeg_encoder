#include "jcfunctions.h"

//*******************************************************************************************//

GLOBAL(void) jc_advance_ptr_block_400(jcstruct_t *s,UINT16 mcu_cols)
{
    s->ptr[0] += (mcu_cols);
	
	return;
}

//*******************************************************************************************//

GLOBAL(void) jc_advance_ptr_row_400(jcstruct_t *s,UINT16 mcu_rows)
{
	UINT32 offset;
	
	offset = ((mcu_rows-1)*(s->number_of_samples_per_line));
	
    s->ptr[0] += offset;
	
	return;
}

//*******************************************************************************************//

GLOBAL(void) jc_advance_ptr_block_444(jcstruct_t *s,UINT16 mcu_cols)
{
    s->ptr[0] += (mcu_cols);
	s->ptr[1] += (mcu_cols);
	s->ptr[2] += (mcu_cols);
	
	return;
}

//*******************************************************************************************//

GLOBAL(void) jc_advance_ptr_row_444(jcstruct_t *s,UINT16 mcu_rows)
{
	UINT32 offset;
	
	offset = ((mcu_rows-1)*(s->number_of_samples_per_line));
	
    s->ptr[0] += offset;
	s->ptr[1] += offset;
	s->ptr[2] += offset;
	
	return;
}

//*******************************************************************************************//

GLOBAL(void) jc_advance_ptr_block_420(jcstruct_t *s,UINT16 mcu_cols)
{
    s->ptr[0] += (mcu_cols);
	mcu_cols >>= 1;
	s->ptr[1] += (mcu_cols);
	s->ptr[2] += (mcu_cols);
	
	return;
}

//*******************************************************************************************//

GLOBAL(void) jc_advance_ptr_row_420(jcstruct_t *s,UINT16 mcu_rows)
{
	UINT32 offset;
	
	offset = ((mcu_rows-1)*(s->number_of_samples_per_line));
	
    s->ptr[0] += offset;
	mcu_rows >>= 1;
	offset = ((mcu_rows-1)*(s->number_of_samples_per_line))>>1;
	s->ptr[1] += offset;
	s->ptr[2] += offset;
	
	return;
}

//*******************************************************************************************//

GLOBAL(void) jc_advance_ptr_block_422(jcstruct_t *s,UINT16 mcu_cols)
{
    s->ptr[0] += (mcu_cols);
	mcu_cols >>= 1;
	s->ptr[1] += (mcu_cols);
	s->ptr[2] += (mcu_cols);
	
	return;
}

//*******************************************************************************************//

GLOBAL(void) jc_advance_ptr_row_422(jcstruct_t *s,UINT16 mcu_rows)
{
	UINT32 offset;
	
	offset = ((mcu_rows-1)*(s->number_of_samples_per_line));
	
    s->ptr[0] += offset;
	offset = ((mcu_rows-1)*(s->number_of_samples_per_line))>>1;
	s->ptr[1] += offset;
	s->ptr[2] += offset;
	
	return;
}

//*******************************************************************************************//

GLOBAL(void) jc_advance_ptr_block_uyvy(jcstruct_t *s,UINT16 mcu_cols)
{
	mcu_cols <<= 1;
    s->ptr[0] += (mcu_cols);
	s->ptr[1] += (mcu_cols);
	s->ptr[2] += (mcu_cols);
	
	return;
}

//*******************************************************************************************//

GLOBAL(void) jc_advance_ptr_row_uyvy(jcstruct_t *s,UINT16 mcu_rows)
{
	UINT32 offset;
	
	offset = (2*(mcu_rows-1)*(s->number_of_samples_per_line));
	
    s->ptr[0] += offset;
	s->ptr[1] += offset;
	s->ptr[2] += offset;
	
	return;
}

//*******************************************************************************************//

GLOBAL(void) jc_advance_ptr_block_rgb24(jcstruct_t *s,UINT16 mcu_cols)
{
    s->ptr[0] += (3*mcu_cols);
	
	return;
}

//*******************************************************************************************//

GLOBAL(void) jc_advance_ptr_row_rgb24(jcstruct_t *s,UINT16 mcu_rows)
{
	UINT32 offset;
	
	offset = (3*(mcu_rows-1)*(s->number_of_samples_per_line));
	
    s->ptr[0] += offset;
	
	return;
}

//*******************************************************************************************//

GLOBAL(void) jc_advance_ptr_block_mb(jcstruct_t *s,UINT16 mcu_cols)
{
    s->ptr[0] += 384;
	
	return;
}

//*******************************************************************************************//

GLOBAL(void) jc_advance_ptr_row_mb(jcstruct_t *s,UINT16 mcu_rows)
{
    UINT32 offset;
    
    offset = ((45 - (s->horizontal_mcus))*384);
    s->ptr[0] += offset;
    
	return;
}

//*******************************************************************************************//
