#include "jcerror.h"
#include "jcfunctions.h"
#include "jcapi.h"

/*******************************************************************************************/

GLOBAL(void) jc_init_encode(jcinfo_t *jc_info)
{
	jc_info->cols = jc_info->rows = jc_info->jc_progress_indicator = 0;
	
	jc_info->jc_copy_method = jc_info->jc_terminate = 
	jc_info->jc_quality_factor = 0;
	
	jc_info->jc_input_buf = jc_info->jc_output_buf = NULL;
	jc_info->jc_input_buf_size = jc_info->jc_output_buf_size = 0;
	
	jc_info->opaque = NULL;
	
	jc_info->l1_buf = jc_info->l3_buf = NULL;
	
    jc_info->l1_buf_size = sizeof(jcdata_l1_t);
    
    jc_info->l3_buf_size = sizeof(jcdata_l3_t);
	
	return;
}

/*******************************************************************************************/

GLOBAL(void) add_quantization_tables(jcinfo_t *jc_info,UINT8 table_selector,const UINT8 *qtbl)
{
	UINT8 *quantization_table;
	UINT32 i;
	
	jcdata_l1_t *jcdata_l1;
	jcdata_l3_t *jcdata_l3;
	jcstruct_t *s;
	
	jcdata_l1 = (jcdata_l1_t *)(jc_info->l1_buf);
	jcdata_l3 = (jcdata_l3_t *)(jc_info->l3_buf);
	
	s = &(jcdata_l1->s);
	
	if(table_selector>2)
	{
		//Invalid table_selector value
		return;
	}
	
	//is used to identify if qtable is user defined or default
	//if add_quantization_tables() is called from set_defaults()
	//then it is set back to 0 else it remains 1
	s->user_defined_qtbl = 1;
	
	quantization_table = jcdata_l3->qtbl[table_selector] = jcdata_l3->quantization_table[table_selector];
	
	for(i=0;i<64;i++)
	{
		quantization_table[i] = qtbl[i];
	}
	
	return;
}

/*******************************************************************************************/

LOCAL(void) jc_set_default_quantization_table(jcinfo_t *jc_info)
{
	static const UINT8 std_luminance_qtbl[64] =		{
											16,  11,  10,  16,  24,  40,  51,  61,
											12,  12,  14,  19,  26,  58,  60,  55,
											14,  13,  16,  24,  40,  57,  69,  56,
											14,  17,  22,  29,  51,  87,  80,  62,
											18,  22,  37,  56,  68, 109, 103,  77,
											24,  35,  55,  64,  81, 104, 113,  92,
											49,  64,  78,  87, 103, 121, 120, 101,
											72,  92,  95,  98, 112, 100, 103,  99
										};
	
	static const UINT8 std_chrominance_qtbl[64] =	{
											17,  18,  24,  47,  99,  99,  99,  99,
											18,  21,  26,  66,  99,  99,  99,  99,
											24,  26,  56,  99,  99,  99,  99,  99,
											47,  66,  99,  99,  99,  99,  99,  99,
											99,  99,  99,  99,  99,  99,  99,  99,
											99,  99,  99,  99,  99,  99,  99,  99,
											99,  99,  99,  99,  99,  99,  99,  99,
											99,  99,  99,  99,  99,  99,  99,  99
										};
	
	jcdata_l1_t *jcdata_l1;
	jcstruct_t *s;
	
	jcdata_l1 = (jcdata_l1_t *)(jc_info->l1_buf);
	
	s = &(jcdata_l1->s);
	
	add_quantization_tables(jc_info,QTBL_LUMINANCE_Y,std_luminance_qtbl);
	
	add_quantization_tables(jc_info,QTBL_CHROMINANCE_CB,std_chrominance_qtbl);
	
	s->user_defined_qtbl = 0;
	
	return;
}

/*******************************************************************************************/

GLOBAL(void) add_huffman_tables(jcinfo_t *jc_info,UINT8 table_selector,const UINT8 *nval,const UINT8 *val)
{
	UINT8 nsymbol;
	UINT32 l,l1;
	
	UINT8 *nval_tbl;
	UINT8 *val_tbl;
	
	jcdata_l3_t *jcdata_l3;
	
	jcdata_l3 = (jcdata_l3_t *)(jc_info->l3_buf);
	
	if(table_selector>3)
	{
		//Invalid table_selector value
		return;
	}
	
	switch(table_selector)
	{
	case HTBL_DC_LUMINANCE:
		nval_tbl = jcdata_l3->nval_tbl_dc[0];
		val_tbl = jcdata_l3->val_tbl_dc[0];
		break;
	
	case HTBL_AC_LUMINANCE:
		nval_tbl = jcdata_l3->nval_tbl_ac[0];
		val_tbl = jcdata_l3->val_tbl_ac[0];
		break;
	
	case HTBL_DC_CHROMINANCE:
		nval_tbl = jcdata_l3->nval_tbl_dc[1];
		val_tbl = jcdata_l3->val_tbl_dc[1];
		break;
	
	case HTBL_AC_CHROMINANCE:
		nval_tbl = jcdata_l3->nval_tbl_ac[1];
		val_tbl = jcdata_l3->val_tbl_ac[1];
		break;
	}
	
	l1 = 0;
	for(l=0;l<=16;l++)
	{
		nsymbol = nval_tbl[l] = nval[l];
		
		while(nsymbol--)
		{
			val_tbl[l1] = val[l1];
			l1++;
		}
	}
	
	return;
}

/*******************************************************************************************/

LOCAL(void) jc_set_default_huffman_table(jcinfo_t *jc_info)
{
	//nval --> no. of values per bit length
	//e.g : bit length			nval		val
	//			0(NP)			0			X
	//			1				0			X
	//			2				1			0
	//			3				5			1,2,3,4,5
	//			4				1			6
	//			5				1			7
	//			6				1			8
	//			7				1			9
	//			8				1			10
	//			9				1			11
	
	static const UINT8 nval_dc_luminance[17] = 
	{ /* 0-base */ 0, 0, 1, 5, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 };
	
	static const UINT8 val_dc_luminance[] = 
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	
	static const UINT8 nval_dc_chrominance[17] = 
    { /* 0-base */ 0, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 };
	
	static const UINT8 val_dc_chrominance[] = 
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	
	static const UINT8 nval_ac_luminance[17] = 
    { /* 0-base */ 0, 0, 2, 1, 3, 3, 2, 4, 3, 5, 5, 4, 4, 0, 0, 1, 0x7d };
	
	static const UINT8 val_ac_luminance[] = 
    { 0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12,
      0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
      0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08,
      0x23, 0x42, 0xb1, 0xc1, 0x15, 0x52, 0xd1, 0xf0,
      0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0a, 0x16,
      0x17, 0x18, 0x19, 0x1a, 0x25, 0x26, 0x27, 0x28,
      0x29, 0x2a, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
      0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
      0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
      0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
      0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
      0x7a, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
      0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
      0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
      0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6,
      0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5,
      0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4,
      0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2,
      0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea,
      0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
      0xf9, 0xfa };
	
	static const UINT8 nval_ac_chrominance[17] = 
    { /* 0-base */ 0, 0, 2, 1, 2, 4, 4, 3, 4, 7, 5, 4, 4, 0, 1, 2, 0x77 };
	
	static const UINT8 val_ac_chrominance[] = 
    { 0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21,
      0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71,
      0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91,
      0xa1, 0xb1, 0xc1, 0x09, 0x23, 0x33, 0x52, 0xf0,
      0x15, 0x62, 0x72, 0xd1, 0x0a, 0x16, 0x24, 0x34,
      0xe1, 0x25, 0xf1, 0x17, 0x18, 0x19, 0x1a, 0x26,
      0x27, 0x28, 0x29, 0x2a, 0x35, 0x36, 0x37, 0x38,
      0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
      0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
      0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
      0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
      0x79, 0x7a, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
      0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96,
      0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5,
      0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4,
      0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3,
      0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2,
      0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda,
      0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9,
      0xea, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
      0xf9, 0xfa };
	
	
	add_huffman_tables(jc_info,HTBL_DC_LUMINANCE,nval_dc_luminance,val_dc_luminance);
	
	add_huffman_tables(jc_info,HTBL_AC_LUMINANCE,nval_ac_luminance,val_ac_luminance);
	
	add_huffman_tables(jc_info,HTBL_DC_CHROMINANCE,nval_dc_chrominance,val_dc_chrominance);
	
	add_huffman_tables(jc_info,HTBL_AC_CHROMINANCE,nval_ac_chrominance,val_ac_chrominance);
	
	return;
}

/*******************************************************************************************/

GLOBAL(void) jc_set_defaults(jcinfo_t *jc_info)
{
	// INIT //
	jc_memset(jc_info->l1_buf, 0, jc_info->l1_buf_size);
	jc_memset(jc_info->l3_buf, 0, jc_info->l3_buf_size);
	
	jc_set_default_quantization_table(jc_info);
	
	jc_set_default_huffman_table(jc_info);
	
	jc_info->jc_quality_factor = 75;//default value
	
	return;
}

/*******************************************************************************************/

LOCAL(UINT8) jc_init_data_buffers(jcinfo_t *jc_info)
{
	INT16  l,l1;
	UINT32 size;
	UINT8  *dct_nbits;
	INT16  *jpeg_coeff,*rgb_coeff;
	
	jcdata_l1_t *jcdata_l1;
	jcstruct_t *s;
	jchuffman_t *htbl;
	
	jc_info->jc_terminate = 0;
	
	jcdata_l1 = (jcdata_l1_t *)(jc_info->l1_buf);
	
	s = &(jcdata_l1->s);
	
	s->write_headers = JC_WRITE_HEADERS;
	
	switch(jc_info->jc_input_format)
	{
	case JC_INPUT_YUV400:
		s->horizontal_sampling_factor = s->vertical_sampling_factor = 1;
		
		s->read_format = jc_read_400;
		s->advance_ptr_block = jc_advance_ptr_block_400;
		s->advance_ptr_row = jc_advance_ptr_row_400;
		
		s->bufferY[0] = (UINT8 *)(jcdata_l1->bufferY[0]);
		s->bufferY[1] = (UINT8 *)(jcdata_l1->bufferY[1]);
		
		s->ptr[0] = jc_info->jc_input_buf;
		s->ptr[1] = NULL;
		s->ptr[2] = NULL;
		break;
	
	case JC_INPUT_YUV444:
		if(jc_info->jc_subsample==JC_ENABLE_SUBSAMPLING)
		{
			s->horizontal_sampling_factor = s->vertical_sampling_factor = 2;
			
			s->read_format = jc_read_444_420;
		}
		else
		{
			s->horizontal_sampling_factor = s->vertical_sampling_factor = 1;
			
			s->read_format = jc_read_444;
		}
		
		s->advance_ptr_block = jc_advance_ptr_block_444;
		s->advance_ptr_row = jc_advance_ptr_row_444;
		
		s->bufferY[0] = (UINT8 *)(jcdata_l1->bufferY[0]);
		s->bufferY[1] = (UINT8 *)(jcdata_l1->bufferY[1]);
		
		size = (jc_info->cols*jc_info->rows);
		
		s->ptr[0] = jc_info->jc_input_buf;
		s->ptr[1] = (UINT8 *)((UINT32)(s->ptr[0]) + size);
		s->ptr[2] = (UINT8 *)((UINT32)(s->ptr[1]) + size);
		break;
	
	case JC_INPUT_YUV420:
		if(jc_info->cols&0x1)
			return ERROR_ODD_COLUMNS_NOT_SUPPORTED;
		
		if(jc_info->rows&0x1)
			return ERROR_ODD_ROWS_NOT_SUPPORTED;
		
		s->horizontal_sampling_factor = s->vertical_sampling_factor = 2;
		
		s->read_format = jc_read_420;
		s->advance_ptr_block = jc_advance_ptr_block_420;
		s->advance_ptr_row = jc_advance_ptr_row_420;
		
		s->bufferY[0] = (UINT8 *)(jcdata_l1->bufferY[0]);
		s->bufferY[1] = (UINT8 *)(jcdata_l1->bufferY[1]);
		
		size = (jc_info->cols*jc_info->rows);
		
		s->ptr[0] = jc_info->jc_input_buf;
		s->ptr[1] = (UINT8 *)((UINT32)(s->ptr[0]) + size);
		size >>= 2;
		s->ptr[2] = (UINT8 *)((UINT32)(s->ptr[1]) + size);
		break;
	
	case JC_INPUT_UYVY:
		if(jc_info->cols&0x1)
			return ERROR_ODD_COLUMNS_NOT_SUPPORTED;
		
		if(jc_info->rows&0x1)
			return ERROR_ODD_ROWS_NOT_SUPPORTED;
		
		s->horizontal_sampling_factor = s->vertical_sampling_factor = 2;
		
		s->read_format = jc_read_uyvy;
		s->advance_ptr_block = jc_advance_ptr_block_uyvy;
		s->advance_ptr_row = jc_advance_ptr_row_uyvy;
		
		s->bufferY[0] = (UINT8 *)(jcdata_l1->bufferY[0]);
		s->bufferY[1] = (UINT8 *)(jcdata_l1->bufferY[1]);
		
		s->ptr[1] = jc_info->jc_input_buf;
		s->ptr[0] = (UINT8 *)((UINT32)(s->ptr[1]) + 1);
		s->ptr[2] = (UINT8 *)((UINT32)(s->ptr[1]) + 2);
		break;
	
	case JC_INPUT_YUV422:
		if(jc_info->cols&0x1)
			return ERROR_ODD_COLUMNS_NOT_SUPPORTED;
		
		if(jc_info->jc_subsample==JC_ENABLE_SUBSAMPLING)
		{
			s->horizontal_sampling_factor = s->vertical_sampling_factor = 2;
			
			s->read_format = jc_read_422_420;
		}
		else
		{
			s->horizontal_sampling_factor = 2;
			s->vertical_sampling_factor = 1;
			
			s->read_format = jc_read_422;
		}
		
		s->advance_ptr_block = jc_advance_ptr_block_422;
		s->advance_ptr_row = jc_advance_ptr_row_422;
		
		s->bufferY[0] = (UINT8 *)(jcdata_l1->bufferY[0]);
		s->bufferY[1] = (UINT8 *)(jcdata_l1->bufferY[1]);
		
		size = (jc_info->cols*jc_info->rows);
		s->ptr[0] = jc_info->jc_input_buf;
		s->ptr[1] = (UINT8 *)((UINT32)(s->ptr[0]) + size);
		size >>= 1;
		s->ptr[2] = (UINT8 *)((UINT32)(s->ptr[1]) + size);
		break;
	
	case JC_INPUT_RGB24:
		if(jc_info->jc_subsample==JC_ENABLE_SUBSAMPLING)
		{
			s->horizontal_sampling_factor = s->vertical_sampling_factor = 2;
			
			s->read_format = jc_read_rgb24_420;
		}
		else
		{
			s->horizontal_sampling_factor = s->vertical_sampling_factor = 1;
			
			s->read_format = jc_read_rgb24;
		}		
		s->advance_ptr_block = jc_advance_ptr_block_rgb24;
		s->advance_ptr_row = jc_advance_ptr_row_rgb24;
		
		s->bufferY[0] = (UINT8 *)(jcdata_l1->bufferY[0]);
		s->bufferY[1] = (UINT8 *)(jcdata_l1->bufferY[1]);
		
		s->ptr[0] = jc_info->jc_input_buf;
		s->ptr[1] = NULL;
		s->ptr[2] = NULL;
		break;
	
	case JC_INPUT_MB:
		s->horizontal_sampling_factor = s->vertical_sampling_factor = 2;
		
		s->read_format = jc_read_mb;
		s->advance_ptr_block = jc_advance_ptr_block_mb;
		s->advance_ptr_row = jc_advance_ptr_row_mb;
		
		s->bufferY[0] = (UINT8 *)(jcdata_l1->bufferY[0]);
		s->bufferY[1] = (UINT8 *)(jcdata_l1->bufferY[1]);
		
		s->ptr[0] = jc_info->jc_input_buf;
		s->ptr[1] = NULL;
		s->ptr[2] = NULL;
		break;
	
	default:
		return ERROR_UNSUPPORTED_IMAGE_FORMAT;
	}
	
	s->bufferCb[0] = (UINT8 *)(jcdata_l1->bufferCb[0]);
	s->bufferCb[1] = (UINT8 *)(jcdata_l1->bufferCb[1]);
	
	s->bufferCr[0] = (UINT8 *)(jcdata_l1->bufferCr[0]);
	s->bufferCr[1] = (UINT8 *)(jcdata_l1->bufferCr[1]);
	
	//s->sof_detected = 0;
	//s->input_buf_size = 0;
	//s->restart_interval = 0;
	//s->huffbuf = 0;
	//s->huffbits = 0;
	//s->hardwiredone = 1;//FIXED never change
	//s->dma_done = 1;//FIXED never change
	
	s->output_buf = jc_info->jc_output_buf;
	
	s->macro_block = (INT16 *)(jcdata_l1->macro_block);
	
	s->temp_buffer = (INT16 *)(jcdata_l1->temp_buffer);
	
	//assign buffer for huffman tables
	htbl = &(jcdata_l1->luminance_htbl);
	htbl->huffbits_dc = (UINT8 *)(jcdata_l1->huffbits_dc[0]);
	htbl->huffbits_ac = (UINT8 *)(jcdata_l1->huffbits_ac[0]);
	htbl->huffcode_dc = (UINT16 *)(jcdata_l1->huffcode_dc[0]);
	htbl->huffcode_ac = (UINT16 *)(jcdata_l1->huffcode_ac[0]);
	
	htbl = &(jcdata_l1->chrominance_htbl);
	htbl->huffbits_dc = (UINT8 *)(jcdata_l1->huffbits_dc[1]);
	htbl->huffbits_ac = (UINT8 *)(jcdata_l1->huffbits_ac[1]);
	htbl->huffcode_dc = (UINT16 *)(jcdata_l1->huffcode_dc[1]);
	htbl->huffcode_ac = (UINT16 *)(jcdata_l1->huffcode_ac[1]);
	
	if(jc_info->jc_input_format==JC_INPUT_YUV400)
		s->components_in_frame = s->components_in_scan = 1;
	else
		s->components_in_frame = s->components_in_scan = 3;
	
	//INIT DCT NBITS TABLE
	dct_nbits = s->dct_nbits = (UINT8 *)(jcdata_l1->dct_nbits);
	
	l = l1 = 1;
	dct_nbits[0] = 0;
	for(;l<=6;l++)
	{
		size = (1<<(l-1));
		while(size--)
		{
			dct_nbits[l1] = (UINT8)l;
			l1++;
		}
	}
	
	// INIT JPEG & RGB COEFFICIENTS //
	jpeg_coeff = s->jpeg_coeff = (INT16 *)(jcdata_l1->jpeg_coeff);
	
	jpeg_coeff[0] = (INT16)(0x5A82);//C4
	jpeg_coeff[1] = (INT16)(0x5A82);//C4
	jpeg_coeff[2] = (INT16)(0x30FB);//cos(3pi/8)  C6
	jpeg_coeff[3] = (INT16)(0x7641);//cos(pi/8)   C2
	jpeg_coeff[4] = (INT16)(0x18F8);//cos(7pi/16)
	jpeg_coeff[5] = (INT16)(0x7D8A);//cos(pi/16)
	jpeg_coeff[6] = (INT16)(0x471C);//cos(5pi/16)
	jpeg_coeff[7] = (INT16)(0x6A6D);//cos(3pi/16)
	
	rgb_coeff = s->rgb_coeff = (INT16 *)(jcdata_l1->rgb_coeff);
	
	rgb_coeff[0] = (INT16)(0x2645);// 0.299
	rgb_coeff[1] = (INT16)(0x4B22);// 0.587
	rgb_coeff[2] = (INT16)(0x0E97);// 0.114
	rgb_coeff[3] = (INT16)(0xEA5F);//-0.169
	rgb_coeff[4] = (INT16)(0x4000);// 0.500
	rgb_coeff[5] = (INT16)(0xF5A2);//-0.081
	rgb_coeff[6] = (INT16)(0xD5A2);//-0.331
	rgb_coeff[7] = (INT16)(0xCA5F);//-0.419
	
	// INIT //
	
    return JC_ENCODE_SUCCESS;
}

/*******************************************************************************************/

GLOBAL(UINT8) jc_write_headers (jcinfo_t *jc_info)
{
	UINT8  err_val;
	
	jcdata_l1_t *jcdata_l1;
	jcstruct_t *s;
	
	jcdata_l1 = (jcdata_l1_t *)(jc_info->l1_buf);
	
	s = &(jcdata_l1->s);
	
	jc_info->jc_output_buf_size = 0;
	jc_info->jc_progress_indicator = 0;
	
	err_val = s->user_defined_qtbl;
	
	// INIT //
	jc_memset(jc_info->l1_buf, 0, jc_info->l1_buf_size);
	
	s->user_defined_qtbl = err_val;
	
	err_val = jc_init_data_buffers(jc_info);
	if(err_val!=JC_ENCODE_SUCCESS)
	{
		return err_val;
	}
	
	err_val = jc_put_markers(jc_info);
	
	return err_val;
}

/*******************************************************************************************/

GLOBAL(UINT8) jc_encode (jcinfo_t *jc_info)
{
	UINT8 err_val;
	
	jcdata_l1_t *jcdata_l1;
	jcstruct_t *s;
	
	jcdata_l1 = (jcdata_l1_t *)(jc_info->l1_buf);
	
	s = &(jcdata_l1->s);
	
	jc_info->jc_progress_indicator = 0;
	s->write_headers = JC_ENCODE;
	
	err_val = jc_put_markers(jc_info);
	
	return err_val;
}

/*******************************************************************************************/
