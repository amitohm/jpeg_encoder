#include "jcerror.h"
#include "jcfunctions.h"
#include "jcmarker.h"

/*******************************************************************************************/

LOCAL(void) jc_put_next_marker(jcstruct_t *s,UINT8 val)
{
	*(s->output_buf++) = 0xFF;
	*(s->output_buf++) = val;
	
	return;
}

/*******************************************************************************************/

GLOBAL(UINT8) jc_put_markers(jcinfo_t *jc_info)
{
	UINT8 err_val,toggle,i;
	UINT32 max,min,size;
	
	jcdata_l1_t *jcdata_l1;
	jcstruct_t *s;
	
	jcdata_l1 = (jcdata_l1_t *)(jc_info->l1_buf);
	
	s = &(jcdata_l1->s);
	
	switch(s->write_headers)
	{
	case JC_WRITE_HEADERS:
		jc_put_next_marker(s,(UINT8)SOI_MARKER);
		
		err_val = jc_put_sof_marker(jc_info);
		if(err_val != JC_ENCODE_SUCCESS)
		{
			return err_val;
		}
		
		err_val = jc_put_dqt_marker(jc_info);
		if(err_val != JC_ENCODE_SUCCESS)
		{
			return err_val;
		}
		
		if(jc_info->jc_optimize==JC_OPTIMIZE_HUFFMAN_TABLES)
		{
			//pre-read mcu to speed up encoding
			s->lc_flag = (s->horizontal_mcus == 1) ? 1 : 0;
			s->lr_flag = (s->vertical_mcus == 1) ? 1 : 0;
			toggle = (*s->read_format)(s,0);
			
			jc_sequential_dummy(jc_info,toggle);
			
			for(i=0;i<4;i++)
			{
				err_val = jc_gen_huff_table(jc_info,i);
				if(err_val!=JC_ENCODE_SUCCESS)
					return err_val;
			}
			
			switch(jc_info->jc_input_format)
			{
			case JC_INPUT_YUV400:
				s->ptr[0] = jc_info->jc_input_buf;
				s->ptr[1] = NULL;
				s->ptr[2] = NULL;
				break;
			
			case JC_INPUT_YUV444:
				size = (jc_info->cols*jc_info->rows);				
				s->ptr[0] = jc_info->jc_input_buf;
				s->ptr[1] = (UINT8 *)((UINT32)(s->ptr[0]) + size);
				s->ptr[2] = (UINT8 *)((UINT32)(s->ptr[1]) + size);
				break;
			
			case JC_INPUT_YUV420:
				size = (jc_info->cols*jc_info->rows);
				s->ptr[0] = jc_info->jc_input_buf;
				s->ptr[1] = (UINT8 *)((UINT32)(s->ptr[0]) + size);
				size >>= 2;
				s->ptr[2] = (UINT8 *)((UINT32)(s->ptr[1]) + size);
				break;
			
			case JC_INPUT_UYVY:
				s->ptr[1] = jc_info->jc_input_buf;
				s->ptr[0] = (UINT8 *)((UINT32)(s->ptr[1]) + 1);
				s->ptr[2] = (UINT8 *)((UINT32)(s->ptr[1]) + 2);
				break;
			
			case JC_INPUT_YUV422:
				size = (jc_info->cols*jc_info->rows);
				s->ptr[0] = jc_info->jc_input_buf;
				s->ptr[1] = (UINT8 *)((UINT32)(s->ptr[0]) + size);
				size >>= 1;
				s->ptr[2] = (UINT8 *)((UINT32)(s->ptr[1]) + size);
				break;
			
			case JC_INPUT_RGB24:
				s->ptr[0] = jc_info->jc_input_buf;
				s->ptr[1] = NULL;
				s->ptr[2] = NULL;
				break;
			
			case JC_INPUT_MB:
				s->ptr[0] = jc_info->jc_input_buf;
				s->ptr[1] = NULL;
				s->ptr[2] = NULL;
				break;
			
			default:
				return ERROR_UNSUPPORTED_IMAGE_FORMAT;
			}
		}
		
		err_val = jc_put_dht_marker(jc_info);
		if(err_val != JC_ENCODE_SUCCESS)
		{
			return err_val;
		}
		
		s->write_data = JC_WRITE_SOS;
		
		err_val = JC_WRITE_HEADERS_SUCCESS;
		
		break;
	
	case JC_ENCODE:
		err_val = jc_put_sos_marker(jc_info);
		if(err_val != JC_ENCODE_SUCCESS)
		{
			return err_val;
		}
		
		jc_put_next_marker(s,(UINT8)EOI_MARKER);
		
		err_val = JC_ENCODE_SUCCESS;
		
		break;
	}
	
	min = (UINT32)(jc_info->jc_output_buf);
	max = (UINT32)(s->output_buf);
	
	jc_info->jc_output_buf_size = (max - min);
	
	return err_val;
}

/*******************************************************************************************/

LOCAL(UINT8) jc_put_sof_marker(jcinfo_t *jc_info)
{
	UINT8 byte,sample_precision,components_in_frame,component_selector;
	UINT16 segment_length;
	UINT16 number_of_lines, number_of_samples_per_line;
	UINT16 horizontal_sampling_factor, vertical_sampling_factor;
	UINT16 horizontal_mcus, vertical_mcus;
	UINT32 last_mcu_cols, last_mcu_rows;
	UINT16 *quantization_table, *prev_quantization_table;
	
	jcdata_l1_t *jcdata_l1;
	jcdata_l3_t *jcdata_l3;
	jcstruct_t *s;
	
	jcdata_l1 = (jcdata_l1_t *)(jc_info->l1_buf);
	jcdata_l3 = (jcdata_l3_t *)(jc_info->l3_buf);
	
	s = &(jcdata_l1->s);
	
	jc_put_next_marker(s,(UINT8)SOF0_MARKER);
	
	segment_length = (2 + 1 + 2 + 2 + 1 + (3*s->components_in_frame));
	jc_put_16_bits(s,segment_length);
	
	sample_precision = 8;
	jc_put_8_bits(s,sample_precision);
	
	number_of_lines = s->number_of_lines = jc_info->rows;
	if(number_of_lines==0)
	{
		return ERROR_INVALID_NUMBER_OF_LINES;
	}
	jc_put_16_bits(s,number_of_lines);
	
	number_of_samples_per_line = s->number_of_samples_per_line = jc_info->cols;
	if(number_of_samples_per_line==0)
	{
		return ERROR_INVALID_SAMPLES_PER_LINE;
	}
	jc_put_16_bits(s,number_of_samples_per_line);
	
	components_in_frame = s->components_in_frame;
	jc_put_8_bits(s,components_in_frame);
	
	prev_quantization_table = quantization_table = NULL;
	for(component_selector=0;component_selector<components_in_frame;component_selector++)
	{
		byte = component_selector + 1;
		jc_put_8_bits(s,byte);
		
		if(component_selector==0)
		{
			horizontal_sampling_factor = s->horizontal_sampling_factor;
			vertical_sampling_factor = s->vertical_sampling_factor;
		}
		else
		{
			horizontal_sampling_factor = vertical_sampling_factor = 1;
		}
		
		byte = (UINT8)((horizontal_sampling_factor<<4) + vertical_sampling_factor);
		jc_put_8_bits(s,byte);
		
		byte = s->qtbl_selector[component_selector];
		jc_put_8_bits(s,byte);
	}
	
	horizontal_mcus = s->horizontal_mcus = (number_of_samples_per_line + (8*s->horizontal_sampling_factor) - 1) / (8*s->horizontal_sampling_factor);
	vertical_mcus = s->vertical_mcus = (number_of_lines + (8*s->vertical_sampling_factor) - 1) / (8*s->vertical_sampling_factor);
	
	last_mcu_cols = number_of_samples_per_line - ((horizontal_mcus-1)*s->horizontal_sampling_factor*8);
	last_mcu_rows = number_of_lines - ((vertical_mcus-1)*s->vertical_sampling_factor*8);
	
	s->mcu_cols[0] = (8 * s->horizontal_sampling_factor);
	s->mcu_rows[0] = (8 * s->vertical_sampling_factor);
	
	s->mcu_cols[1] = last_mcu_cols;
	s->mcu_rows[1] = last_mcu_rows;
	
	s->inverse_total_mcus = 104857600 - 1;
	
	s->inverse_total_mcus = (s->inverse_total_mcus)/(horizontal_mcus * vertical_mcus);
	
	return JC_ENCODE_SUCCESS;
}

/*******************************************************************************************/

LOCAL(UINT8) jc_put_dqt_marker(jcinfo_t *jc_info)
{
	UINT8 byte;
	
	static const UINT8 jpeg_natural_order[64] =	{
										 0,  1,  8,  16, 9,  2, 3,  10,
										17, 24, 32, 25, 18, 11,  4,  5,
										12, 19, 26, 33, 40, 48, 41, 34,
										27, 20, 13,  6,  7, 14, 21, 28,
										35, 42, 49, 56, 57, 50, 43, 36,
										29, 22, 15, 23, 30, 37, 44, 51,
										58, 59, 52, 45, 38, 31, 39, 46,
										53, 60, 61, 54, 47, 55, 62, 63
									};
	
	UINT16 i;
	UINT16 segment_length;
	UINT16 precision, table_selector;
	INT32  quality_factor,inverse,qval;
	UINT8  *quantization_table;
	UINT16 *qtbl,*prev_qtbl;
	
	jcdata_l1_t *jcdata_l1;
	jcdata_l3_t *jcdata_l3;
	jcstruct_t *s;
	
	jcdata_l1 = (jcdata_l1_t *)(jc_info->l1_buf);
	jcdata_l3 = (jcdata_l3_t *)(jc_info->l3_buf);
	
	s = &(jcdata_l1->s);
	
	quality_factor = jc_info->jc_quality_factor;
	if(s->user_defined_qtbl)
		quality_factor = 50;
	quality_factor = MAX(1,quality_factor);
	quality_factor = MIN(100,quality_factor);
	jc_info->jc_quality_factor = quality_factor;
	
	inverse = QINVERSE(100);
	
	if(quality_factor<50)
	{
		quality_factor = (5000/quality_factor);
	}
	else
	{
		quality_factor = (200 - 2*quality_factor);
	}
	
	precision = 0;//8 bit samples
	
	prev_qtbl = NULL;
	
	for(table_selector=0;table_selector<(s->components_in_scan);table_selector++)
	{
		quantization_table = jcdata_l3->qtbl[table_selector];
		if(quantization_table==NULL)
			break;
		
		switch(table_selector)
		{
		case QTBL_LUMINANCE_Y:
			s->qtbl_selector[0] = QTBL_LUMINANCE_Y;
			s->qtbl[0] = (UINT16 *)(jcdata_l1->quantization_table[table_selector]);
			break;
		
		case QTBL_CHROMINANCE_CB:
			s->qtbl_selector[1] = QTBL_CHROMINANCE_CB;
			s->qtbl_selector[2] = QTBL_CHROMINANCE_CB;
			s->qtbl[1] = (UINT16 *)(jcdata_l1->quantization_table[table_selector]);
			s->qtbl[2] = s->qtbl[1];
			break;
		
		case QTBL_CHROMINANCE_CR:
			s->qtbl_selector[2] = QTBL_CHROMINANCE_CR;
			s->qtbl[2] = (UINT16 *)(jcdata_l1->quantization_table[table_selector]);
			break;
		}
		
		qtbl = s->qtbl[table_selector];
		
		if(qtbl==prev_qtbl)
		{
			//Same tables for Cb and Cr
			break;
		}
		
		jc_put_next_marker(s,(UINT8)DQT_MARKER);
		
		segment_length = (64 + 1 + 2);
		jc_put_16_bits(s,segment_length);
		
		byte = (UINT8)((precision<<4) + table_selector);
		jc_put_8_bits (s,byte);
		
		for(i=0;i<64;i++)
		{
			qval = (quantization_table[jpeg_natural_order[i]]);
			qval = QMULTIPLY(((quality_factor*qval) + 50),inverse);
			//qval = (((qval*quality_factor) + 50)*inverse)>>15;
			qval = MAX(1,qval);
			qval = MIN(255,qval);
			jc_put_8_bits(s,(UINT8)qval);
			
			qval = QINVERSE(qval);
			qtbl[jpeg_natural_order[i]] = (UINT16)qval;
		}
		
		prev_qtbl = qtbl;
	}
	
	return JC_ENCODE_SUCCESS;
}

/*******************************************************************************************/

LOCAL(UINT8) jc_put_dht_marker(jcinfo_t *jc_info)
{
	UINT8 byte,table_selector;
	UINT8 l,l1 ;
	UINT8 nsymbol,table_class,table_identifier;
	UINT16 segment_length,code;
	
	UINT8 *huffbits,*nval_tbl,*val_tbl;
	UINT16 *huffcode;
	
	jcdata_l1_t *jcdata_l1;
	jcdata_l3_t *jcdata_l3;
	jcstruct_t *s;
	jchuffman_t *htbl;
	
	jcdata_l1 = (jcdata_l1_t *)(jc_info->l1_buf);
	jcdata_l3 = (jcdata_l3_t *)(jc_info->l3_buf);
	
	s = &(jcdata_l1->s);
	
	for(table_selector=0;table_selector<4;table_selector++)
	{
		if((table_selector&0x2)==0)
		{
			s->htbl[0] = &(jcdata_l1->luminance_htbl);
			htbl = s->htbl[0];
		}
		else
		{
			s->htbl[1] = &(jcdata_l1->chrominance_htbl);
			s->htbl[2] = s->htbl[1];
			htbl = s->htbl[1];
		}
		
		if((table_selector&0x1)==0)
		{
			huffbits = htbl->huffbits_dc;
			huffcode = htbl->huffcode_dc;
		}
		else
		{
			huffbits = htbl->huffbits_ac;
			huffcode = htbl->huffcode_ac;
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
		
		segment_length = 0;
		for(l=1;l<=16;l++)
		{
			segment_length += nval_tbl[l];
		}
		
		if(segment_length>256)
			return ERROR_INVALID_NUMBER_OF_HUFFMAN_CODES;
		
		jc_put_next_marker(s,(UINT8)DHT_MARKER);
		
		segment_length = (2 + 1 + 16 + segment_length);
		jc_put_16_bits(s,segment_length);
		
		table_class = (table_selector&0x1);
		table_identifier = ((table_selector&0x2)>>1);
		byte = ((table_class<<4) + table_identifier);
		jc_put_8_bits(s,byte);
		
		for(l=1;l<=16;l++)
		{
			byte = nval_tbl[l];
			jc_put_8_bits(s,byte);
		}
		
		segment_length -= (2 + 1 + 16);
		for(l=0;l<segment_length;l++)
		{
			byte = val_tbl[l];
			jc_put_8_bits(s,byte);
		}
		
		code = 0;
		l1 = 0;
		for(l=1;l<=16;l++)
		{
			nsymbol = nval_tbl[l];
			
			while(nsymbol--)
			{
				byte = val_tbl[l1];
				huffbits[byte] = l;
				huffcode[byte] = code;
				code++;
				l1++;
			}
			code <<= 1;
		}
	}
	
	return JC_ENCODE_SUCCESS;
}

/*******************************************************************************************/

LOCAL(UINT8) jc_put_sos_marker(jcinfo_t *jc_info)
{
	UINT8 toggle;
	UINT8 byte,components_in_scan,component_selector;
	UINT8 dc_table_selector, ac_table_selector;
	UINT8 Ss, Se;
	UINT8 Al, Ah;
	UINT16 segment_length;
	
	jcdata_l1_t *jcdata_l1;
	jcstruct_t *s;
	
	jcdata_l1 = (jcdata_l1_t *)(jc_info->l1_buf);
	
	s = &(jcdata_l1->s);
	
	//pre-read mcu to speed up encoding
	s->lc_flag = (s->horizontal_mcus == 1) ? 1 : 0;
	s->lr_flag = (s->vertical_mcus == 1) ? 1 : 0;
	toggle = (*s->read_format)(s,0);
	
	switch(s->write_data)
	{
	case JC_WRITE_SOS:
		jc_put_next_marker(s,(UINT8)SOS_MARKER);
		
		components_in_scan = s->components_in_scan;
		
		segment_length = (2 + 1 + (2*components_in_scan) + 3);
		jc_put_16_bits(s,segment_length);
		
		jc_put_8_bits(s,components_in_scan);
		
		for(component_selector=0;component_selector<components_in_scan;component_selector++)
		{
			byte = component_selector + 1;
			jc_put_8_bits(s,byte);
			
			if(component_selector==0)
			{//For luminance
				dc_table_selector = ac_table_selector = 0;
			}
			else
			{//For chrominance
				dc_table_selector = ac_table_selector = 1;
			}
			byte = ((dc_table_selector<<4) + ac_table_selector);
			jc_put_8_bits(s,byte);
		}
		
		Ss = 0;
		jc_put_8_bits(s,Ss);
		
		Se = 63;
		jc_put_8_bits(s,Se);
		
		Ah = Al = 0;
		byte = ((Ah<<4) + Al);
		jc_put_8_bits(s,byte);
		
		/* initializations */
		s->last_dc_val[0] = 0;
		s->last_dc_val[1] = 0;
		s->last_dc_val[2] = 0;
		
		s->write_data = JC_WRITE_DATA;
	
	case JC_WRITE_DATA:
		byte = jc_sequential(jc_info,toggle);
		if(byte!=JC_ENCODE_SUCCESS)
			return byte;
		
		jc_finish_huffman(s);
		
		break;
	}
	
	return JC_ENCODE_SUCCESS;
}

/*******************************************************************************************/
