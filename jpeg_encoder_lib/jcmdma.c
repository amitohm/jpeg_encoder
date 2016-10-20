#include "jcfunctions.h"

GLOBAL(void) jc_setup_mdma_400(jcinfo_t *jc_info)
{
	jcdata_l1_t *jcdata_l1;
	jcstruct_t *s;
	dma_descriptor_t *dma_ptr;
	
	jcdata_l1 = (jcdata_l1_t *)(jc_info->l1_buf);
	
	s = &(jcdata_l1->s);
	
    dma_ptr = &(jcdata_l1->Y_sptr);
    s->dma_sptr = dma_ptr;
    
	jcdata_l1->Y_sptr.next_descpriptor_ptr = NULL;
	jcdata_l1->Y_sptr.dma_config = 0x0031;
	jcdata_l1->Y_sptr.xmodify = 1;
	
    dma_ptr = &(jcdata_l1->Y_dptr);
    s->dma_dptr = dma_ptr;
    
	jcdata_l1->Y_dptr.next_descpriptor_ptr = NULL;
	jcdata_l1->Y_dptr.dma_config = 0x00B3;
	jcdata_l1->Y_dptr.xmodify = 1;
	
	return;
}

GLOBAL(UINT8) jc_mdma_400(jcstruct_t *s,UINT8 toggle)
{
	UINT16 mcu_cols,mcu_rows;
	
	dma_descriptor_t *dma_ptr;
	
	mcu_cols = s->mcu_cols[s->lc_flag];
	mcu_rows = s->mcu_rows[s->lr_flag];
	
	dma_ptr = s->dma_sptr;
	dma_ptr->start_address =  (s->ptr[0]);
	dma_ptr->xcount = mcu_cols;
	dma_ptr->ycount = mcu_rows;
	dma_ptr->ymodify = (s->number_of_samples_per_line - mcu_cols + 1);
	
	dma_ptr = s->dma_dptr;
	dma_ptr->start_address =  (s->bufferY[toggle]);
	dma_ptr->xcount = mcu_cols;
	dma_ptr->ycount = mcu_rows;
	dma_ptr->ymodify = (UINT16)(8 - mcu_cols + 1);
	
	jc_dma_start((s->dma_sptr),(s->dma_dptr));
	
	TOGGLE(toggle);
	
	return toggle;
}

GLOBAL(void) jc_setup_mdma_444(jcinfo_t *jc_info)
{
	jcdata_l1_t *jcdata_l1;
	jcstruct_t *s;
	dma_descriptor_t *dma_ptr;
	
	jcdata_l1 = (jcdata_l1_t *)(jc_info->l1_buf);
	
	s = &(jcdata_l1->s);
	
    dma_ptr = &(jcdata_l1->Y_sptr);
    s->dma_sptr = dma_ptr;
    
    dma_ptr = &(jcdata_l1->Cb_sptr);
	jcdata_l1->Y_sptr.next_descpriptor_ptr = dma_ptr;
	jcdata_l1->Y_sptr.dma_config = 0x7931;
	jcdata_l1->Y_sptr.xmodify = 1;
	
    dma_ptr = &(jcdata_l1->Cr_sptr);
	jcdata_l1->Cb_sptr.next_descpriptor_ptr = dma_ptr;
	jcdata_l1->Cb_sptr.dma_config = 0x7931;
	jcdata_l1->Cb_sptr.xmodify = 1;
	
	jcdata_l1->Cr_sptr.next_descpriptor_ptr = NULL;
	jcdata_l1->Cr_sptr.dma_config = 0x0031;
	jcdata_l1->Cr_sptr.xmodify = 1;
	
    dma_ptr = &(jcdata_l1->Y_dptr);
    s->dma_dptr = dma_ptr;
    
    dma_ptr = &(jcdata_l1->Cb_dptr);
	jcdata_l1->Y_dptr.next_descpriptor_ptr = dma_ptr;
	jcdata_l1->Y_dptr.dma_config = 0x7933;
	jcdata_l1->Y_dptr.xmodify = 1;
	
    dma_ptr = &(jcdata_l1->Cr_dptr);
	jcdata_l1->Cb_dptr.next_descpriptor_ptr = dma_ptr;
	jcdata_l1->Cb_dptr.dma_config = 0x7933;
	jcdata_l1->Cb_dptr.xmodify = 1;
	
	jcdata_l1->Cr_dptr.next_descpriptor_ptr = NULL;
	jcdata_l1->Cr_dptr.dma_config = 0x00B3;
	jcdata_l1->Cr_dptr.xmodify = 1;
	
	return;
}

GLOBAL(UINT8) jc_mdma_444(jcstruct_t *s,UINT8 toggle)
{
	UINT16 number_of_samples_per_line,mcu_cols,mcu_rows,offset;
	
	dma_descriptor_t *dma_ptr;
	
	number_of_samples_per_line = s->number_of_samples_per_line;
	
	mcu_cols = s->mcu_cols[s->lc_flag];
	mcu_rows = s->mcu_rows[s->lr_flag];
	
	offset = (number_of_samples_per_line - mcu_cols + 1);
	
	dma_ptr = s->dma_sptr;
	dma_ptr->start_address =  (s->ptr[0]);
	dma_ptr->xcount = mcu_cols;
	dma_ptr->ycount = mcu_rows;
	dma_ptr->ymodify = offset;
	
	dma_ptr = dma_ptr->next_descpriptor_ptr;
	dma_ptr->start_address =  (s->ptr[1]);
	dma_ptr->xcount = mcu_cols;
	dma_ptr->ycount = mcu_rows;
	dma_ptr->ymodify = offset;
	
	dma_ptr = dma_ptr->next_descpriptor_ptr;
	dma_ptr->start_address =  (s->ptr[2]);
	dma_ptr->xcount = mcu_cols;
	dma_ptr->ycount = mcu_rows;
	dma_ptr->ymodify = offset;
	
	dma_ptr = s->dma_dptr;
	dma_ptr->start_address =  (s->bufferY[toggle]);
	dma_ptr->xcount = mcu_cols;
	dma_ptr->ycount = mcu_rows;
	dma_ptr->ymodify = (UINT16)(8 - mcu_cols + 1);
	
	dma_ptr = dma_ptr->next_descpriptor_ptr;
	dma_ptr->start_address =  (s->bufferCb[toggle]);
	dma_ptr->xcount = mcu_cols;
	dma_ptr->ycount = mcu_rows;
	dma_ptr->ymodify = (UINT16)(8 - mcu_cols + 1);
	
	dma_ptr = dma_ptr->next_descpriptor_ptr;
	dma_ptr->start_address =  (s->bufferCr[toggle]);
	dma_ptr->xcount = mcu_cols;
	dma_ptr->ycount = mcu_rows;
	dma_ptr->ymodify = (UINT16)(8 - mcu_cols + 1);
	
	jc_dma_start((s->dma_sptr),(s->dma_dptr));
	
	TOGGLE(toggle);
	
	return toggle;
}

GLOBAL(void) jc_setup_mdma_420(jcinfo_t *jc_info)
{
	jcdata_l1_t *jcdata_l1;
	jcstruct_t *s;
	dma_descriptor_t *dma_ptr;
	
	jcdata_l1 = (jcdata_l1_t *)(jc_info->l1_buf);
	
	s = &(jcdata_l1->s);
	
    dma_ptr = &(jcdata_l1->Y_sptr);
    s->dma_sptr = dma_ptr;
    
    dma_ptr = &(jcdata_l1->Cb_sptr);
	jcdata_l1->Y_sptr.next_descpriptor_ptr = dma_ptr;
	jcdata_l1->Y_sptr.dma_config = 0x7931;
	jcdata_l1->Y_sptr.xmodify = 1;
	
    dma_ptr = &(jcdata_l1->Cr_sptr);
	jcdata_l1->Cb_sptr.next_descpriptor_ptr = dma_ptr;
	jcdata_l1->Cb_sptr.dma_config = 0x7931;
	jcdata_l1->Cb_sptr.xmodify = 1;
	
	jcdata_l1->Cr_sptr.next_descpriptor_ptr = NULL;
	jcdata_l1->Cr_sptr.dma_config = 0x0031;
	jcdata_l1->Cr_sptr.xmodify = 1;
	
    dma_ptr = &(jcdata_l1->Y_dptr);
    s->dma_dptr = dma_ptr;
    
    dma_ptr = &(jcdata_l1->Cb_dptr);
	jcdata_l1->Y_dptr.next_descpriptor_ptr = dma_ptr;
	jcdata_l1->Y_dptr.dma_config = 0x7933;
	jcdata_l1->Y_dptr.xmodify = 1;
	
    dma_ptr = &(jcdata_l1->Cr_dptr);
	jcdata_l1->Cb_dptr.next_descpriptor_ptr = dma_ptr;
	jcdata_l1->Cb_dptr.dma_config = 0x7933;
	jcdata_l1->Cb_dptr.xmodify = 1;
	
	jcdata_l1->Cr_dptr.next_descpriptor_ptr = NULL;
	jcdata_l1->Cr_dptr.dma_config = 0x00B3;
	jcdata_l1->Cr_dptr.xmodify = 1;
	
	return;
}

GLOBAL(UINT8) jc_mdma_420(jcstruct_t *s,UINT8 toggle)
{
	UINT16 number_of_samples_per_line,mcu_cols,mcu_rows,offset;
	UINT16 mcu_cols_2,mcu_rows_2;
	
	dma_descriptor_t *dma_ptr;
	
	number_of_samples_per_line = s->number_of_samples_per_line;
	
	mcu_cols = s->mcu_cols[s->lc_flag];
	mcu_rows = s->mcu_rows[s->lr_flag];
	
	offset = (number_of_samples_per_line - mcu_cols + 1);
	
	dma_ptr = s->dma_sptr;
	dma_ptr->start_address =  (s->ptr[0]);
	dma_ptr->xcount = mcu_cols;
	dma_ptr->ycount = mcu_rows;
	dma_ptr->ymodify = offset;
	
	number_of_samples_per_line >>= 1;
	mcu_cols_2 = mcu_cols>>1;
	mcu_rows_2 = mcu_rows>>1;
	
	offset = (number_of_samples_per_line - mcu_cols_2 + 1);
	
	dma_ptr = dma_ptr->next_descpriptor_ptr;
	dma_ptr->start_address =  (s->ptr[1]);
	dma_ptr->xcount = mcu_cols_2;
	dma_ptr->ycount = mcu_rows_2;
	dma_ptr->ymodify = offset;
	
	dma_ptr = dma_ptr->next_descpriptor_ptr;
	dma_ptr->start_address =  (s->ptr[2]);
	dma_ptr->xcount = mcu_cols_2;
	dma_ptr->ycount = mcu_rows_2;
	dma_ptr->ymodify = offset;
	
	dma_ptr = s->dma_dptr;
	dma_ptr->start_address =  (s->bufferY[toggle]);
	dma_ptr->xcount = mcu_cols;
	dma_ptr->ycount = mcu_rows;
	dma_ptr->ymodify = (UINT16)(16 - mcu_cols + 1);
	
	dma_ptr = dma_ptr->next_descpriptor_ptr;
	dma_ptr->start_address =  (s->bufferCb[toggle]);
	dma_ptr->xcount = mcu_cols_2;
	dma_ptr->ycount = mcu_rows_2;
	dma_ptr->ymodify = (UINT16)(8 - mcu_cols_2 + 1);
	
	dma_ptr = dma_ptr->next_descpriptor_ptr;
	dma_ptr->start_address =  (s->bufferCr[toggle]);
	dma_ptr->xcount = mcu_cols_2;
	dma_ptr->ycount = mcu_rows_2;
	dma_ptr->ymodify = (UINT16)(8 - mcu_cols_2 + 1);
	
	jc_dma_start((s->dma_sptr),(s->dma_dptr));
	
	TOGGLE(toggle);
	
	return toggle;
}

GLOBAL(void) jc_setup_mdma_422(jcinfo_t *jc_info)
{
	jcdata_l1_t *jcdata_l1;
	jcstruct_t *s;
	dma_descriptor_t *dma_ptr;
	
	jcdata_l1 = (jcdata_l1_t *)(jc_info->l1_buf);
	
	s = &(jcdata_l1->s);
	
    dma_ptr = &(jcdata_l1->Y_sptr);
    s->dma_sptr = dma_ptr;
    
    dma_ptr = &(jcdata_l1->Cb_sptr);
	jcdata_l1->Y_sptr.next_descpriptor_ptr = dma_ptr;
	jcdata_l1->Y_sptr.dma_config = 0x7931;
	jcdata_l1->Y_sptr.xmodify = 1;
	
    dma_ptr = &(jcdata_l1->Cr_sptr);
	jcdata_l1->Cb_sptr.next_descpriptor_ptr = dma_ptr;
	jcdata_l1->Cb_sptr.dma_config = 0x7931;
	jcdata_l1->Cb_sptr.xmodify = 1;
	
	jcdata_l1->Cr_sptr.next_descpriptor_ptr = NULL;
	jcdata_l1->Cr_sptr.dma_config = 0x0031;
	jcdata_l1->Cr_sptr.xmodify = 1;
	
    dma_ptr = &(jcdata_l1->Y_dptr);
    s->dma_dptr = dma_ptr;
    
    dma_ptr = &(jcdata_l1->Cb_dptr);
	jcdata_l1->Y_dptr.next_descpriptor_ptr = dma_ptr;
	jcdata_l1->Y_dptr.dma_config = 0x7933;
	jcdata_l1->Y_dptr.xmodify = 1;
	
    dma_ptr = &(jcdata_l1->Cr_dptr);
	jcdata_l1->Cb_dptr.next_descpriptor_ptr = dma_ptr;
	jcdata_l1->Cb_dptr.dma_config = 0x7933;
	jcdata_l1->Cb_dptr.xmodify = 1;
	
	jcdata_l1->Cr_dptr.next_descpriptor_ptr = NULL;
	jcdata_l1->Cr_dptr.dma_config = 0x00B3;
	jcdata_l1->Cr_dptr.xmodify = 1;
	
	return;
}

GLOBAL(UINT8) jc_mdma_422(jcstruct_t *s,UINT8 toggle)
{
	UINT16 number_of_samples_per_line,mcu_cols,mcu_rows,offset;
	UINT16 mcu_cols_2;
	
	dma_descriptor_t *dma_ptr;
	
	number_of_samples_per_line = s->number_of_samples_per_line;
	
	mcu_cols = s->mcu_cols[s->lc_flag];
	mcu_rows = s->mcu_rows[s->lr_flag];
	
	offset = (number_of_samples_per_line - mcu_cols + 1);
	
	dma_ptr = s->dma_sptr;
	dma_ptr->start_address =  (s->ptr[0]);
	dma_ptr->xcount = mcu_cols;
	dma_ptr->ycount = mcu_rows;
	dma_ptr->ymodify = offset;
	
	number_of_samples_per_line >>= 1;
	mcu_cols_2 = mcu_cols>>1;
	
	offset = (number_of_samples_per_line - mcu_cols_2 + 1);
	
	dma_ptr = dma_ptr->next_descpriptor_ptr;
	dma_ptr->start_address =  (s->ptr[1]);
	dma_ptr->xcount = mcu_cols_2;
	dma_ptr->ycount = mcu_rows;
	dma_ptr->ymodify = offset;
	
	dma_ptr = dma_ptr->next_descpriptor_ptr;
	dma_ptr->start_address =  (s->ptr[2]);
	dma_ptr->xcount = mcu_cols_2;
	dma_ptr->ycount = mcu_rows;
	dma_ptr->ymodify = offset;
	
	dma_ptr = s->dma_dptr;
	dma_ptr->start_address =  (s->bufferY[toggle]);
	dma_ptr->xcount = mcu_cols;
	dma_ptr->ycount = mcu_rows;
	dma_ptr->ymodify = (UINT16)(16 - mcu_cols + 1);
	
	dma_ptr = dma_ptr->next_descpriptor_ptr;
	dma_ptr->start_address =  (s->bufferCb[toggle]);
	dma_ptr->xcount = mcu_cols_2;
	dma_ptr->ycount = mcu_rows;
	dma_ptr->ymodify = (UINT16)(8 - mcu_cols_2 + 1);
	
	dma_ptr = dma_ptr->next_descpriptor_ptr;
	dma_ptr->start_address =  (s->bufferCr[toggle]);
	dma_ptr->xcount = mcu_cols_2;
	dma_ptr->ycount = mcu_rows;
	dma_ptr->ymodify = (UINT16)(8 - mcu_cols_2 + 1);
	
	jc_dma_start((s->dma_sptr),(s->dma_dptr));
	
	TOGGLE(toggle);
	
	return toggle;
}

GLOBAL(void) jc_setup_mdma_uyvy(jcinfo_t *jc_info)
{
	jcdata_l1_t *jcdata_l1;
	jcstruct_t *s;
	dma_descriptor_t *dma_ptr;
	
	jcdata_l1 = (jcdata_l1_t *)(jc_info->l1_buf);
	
	s = &(jcdata_l1->s);
	
    dma_ptr = &(jcdata_l1->Y_sptr);
    s->dma_sptr = dma_ptr;
    
    dma_ptr = &(jcdata_l1->Cb_sptr);
	jcdata_l1->Y_sptr.next_descpriptor_ptr = dma_ptr;
	jcdata_l1->Y_sptr.dma_config = 0x7931;
	jcdata_l1->Y_sptr.xmodify = 2;
	
    dma_ptr = &(jcdata_l1->Cr_sptr);
	jcdata_l1->Cb_sptr.next_descpriptor_ptr = dma_ptr;
	jcdata_l1->Cb_sptr.dma_config = 0x7931;
	jcdata_l1->Cb_sptr.xmodify = 4;
	
	jcdata_l1->Cr_sptr.next_descpriptor_ptr = NULL;
	jcdata_l1->Cr_sptr.dma_config = 0x0031;
	jcdata_l1->Cr_sptr.xmodify = 4;
	
    dma_ptr = &(jcdata_l1->Y_dptr);
    s->dma_dptr = dma_ptr;
    
    dma_ptr = &(jcdata_l1->Cb_dptr);
	jcdata_l1->Y_dptr.next_descpriptor_ptr = dma_ptr;
	jcdata_l1->Y_dptr.dma_config = 0x7933;
	jcdata_l1->Y_dptr.xmodify = 1;
	
    dma_ptr = &(jcdata_l1->Cr_dptr);
	jcdata_l1->Cb_dptr.next_descpriptor_ptr = dma_ptr;
	jcdata_l1->Cb_dptr.dma_config = 0x7933;
	jcdata_l1->Cb_dptr.xmodify = 1;
	
	jcdata_l1->Cr_dptr.next_descpriptor_ptr = NULL;
	jcdata_l1->Cr_dptr.dma_config = 0x00B3;
	jcdata_l1->Cr_dptr.xmodify = 1;
	
	return;
}

GLOBAL(UINT8) jc_mdma_uyvy(jcstruct_t *s,UINT8 toggle)
{
	UINT16 number_of_samples_per_line,mcu_cols,mcu_rows,offset;
	UINT16 mcu_cols_2,mcu_rows_2;
	
	dma_descriptor_t *dma_ptr;
	
	number_of_samples_per_line = s->number_of_samples_per_line;
	
	mcu_cols = s->mcu_cols[s->lc_flag];
	mcu_rows = s->mcu_rows[s->lr_flag];
	
	offset = 2*(number_of_samples_per_line - mcu_cols + 1);
	
	dma_ptr = s->dma_sptr;
	dma_ptr->start_address =  (s->ptr[0]);
	dma_ptr->xcount = mcu_cols;
	dma_ptr->ycount = mcu_rows;
	dma_ptr->ymodify = offset;
	
	mcu_cols_2 = mcu_cols>>1;
	mcu_rows_2 = mcu_rows>>1;
	
	offset = 4*(number_of_samples_per_line - mcu_cols_2 + 1);
	
	dma_ptr = dma_ptr->next_descpriptor_ptr;
	dma_ptr->start_address =  (s->ptr[1]);
	dma_ptr->xcount = mcu_cols_2;
	dma_ptr->ycount = mcu_rows_2;
	dma_ptr->ymodify = offset;
	
	dma_ptr = dma_ptr->next_descpriptor_ptr;
	dma_ptr->start_address =  (s->ptr[2]);
	dma_ptr->xcount = mcu_rows_2;
	dma_ptr->ycount = mcu_rows_2;
	dma_ptr->ymodify = offset;
	
	dma_ptr = s->dma_dptr;
	dma_ptr->start_address =  (s->bufferY[toggle]);
	dma_ptr->xcount = mcu_cols;
	dma_ptr->ycount = mcu_rows;
	dma_ptr->ymodify = (UINT16)(16 - mcu_cols + 1);
	
	dma_ptr = dma_ptr->next_descpriptor_ptr;
	dma_ptr->start_address =  (s->bufferCb[toggle]);
	dma_ptr->xcount = mcu_cols_2;
	dma_ptr->ycount = mcu_rows_2;
	dma_ptr->ymodify = (UINT16)(8 - mcu_cols_2 + 1);
	
	dma_ptr = dma_ptr->next_descpriptor_ptr;
	dma_ptr->start_address =  (s->bufferCr[toggle]);
	dma_ptr->xcount = mcu_cols_2;
	dma_ptr->ycount = mcu_rows_2;
	dma_ptr->ymodify = (UINT16)(8 - mcu_cols_2 + 1);
	
	jc_dma_start((s->dma_sptr),(s->dma_dptr));
	
	TOGGLE(toggle);
	
	return toggle;
}
