#include "jcerror.h"
#include "jcfunctions.h"

GLOBAL(UINT8) jc_sequential_dummy(jcinfo_t *jc_info,UINT8 toggle)
{
	UINT8  err_val;
	UINT8  components_in_scan;
	UINT8  horizontal_sampling_factor,vertical_sampling_factor;
	INT16  temp;
	UINT16 i,j,k,l,m;
	UINT16 horizontal_mcus,vertical_mcus;
	UINT16 mcu_rows,mcu_cols;
	UINT32 nmcu;
	UINT8  *buffer;
	UINT16 *qtbl;
	INT16  *macro_block,*jpeg_coeff,*temp_buffer;
	UINT32 *dc_count,*ac_count;
	
	UINT8 (* read_format)(jcstruct_t *s,UINT8 toggle);
	void  (* advance_ptr_block)(jcstruct_t *s,UINT16 mcu_cols);
	void  (* advance_ptr_row)(jcstruct_t *s,UINT16 mcu_rows);
	
	jcdata_l1_t *jcdata_l1;
	jcdata_l3_t *jcdata_l3;
	jcstruct_t *s;
	
	jcdata_l1 = (jcdata_l1_t *)(jc_info->l1_buf);
	jcdata_l3 = (jcdata_l3_t *)(jc_info->l3_buf);
	
	s = &(jcdata_l1->s);
	
	jpeg_coeff = s->jpeg_coeff;
	
	macro_block = s->macro_block;
	
	temp_buffer = s->temp_buffer;
	
	read_format = s->read_format;
	advance_ptr_block = s->advance_ptr_block;
	advance_ptr_row = s->advance_ptr_row;
	
	s->huffbits = 0;
	s->huffcode = 0;
	
	s->decoded_mcus = 0;
	
	horizontal_mcus = s->horizontal_mcus;
	vertical_mcus = s->vertical_mcus;
	
	nmcu = (horizontal_mcus*vertical_mcus) - 1;
	
	components_in_scan = s->components_in_scan;
	
	horizontal_sampling_factor = s->horizontal_sampling_factor;
	vertical_sampling_factor = s->vertical_sampling_factor;
	
	k = l = m = 0;
	
	for(i=1;i<=vertical_mcus;i++)
	{
		s->lr_flag = (i == vertical_mcus) ? 1 : 0;
		
		mcu_rows = s->mcu_rows[s->lr_flag];
		
		for(j=1;j<=horizontal_mcus;j++)
		{
			s->lc_flag = (j == horizontal_mcus) ? 1 : 0;
			
			mcu_cols = s->mcu_cols[s->lc_flag];
			
			(*advance_ptr_block)(s,mcu_cols);
			
			if(s->lc_flag)
			{
				(*advance_ptr_row)(s,mcu_rows);
				s->lr_flag = (i == (vertical_mcus-1)) ? 1 : 0;
			}
			
			s->lc_flag = (j == (horizontal_mcus-1)) ? 1 : 0;
			if(nmcu!=0)
			{
				toggle = (*read_format)(s,toggle);
			}
			else
			{
				TOGGLE(toggle);
			}
			
			buffer = s->bufferY[toggle];
			
			qtbl = s->qtbl[0];
			
			dc_count = jcdata_l3->dc_count[0];
			ac_count = jcdata_l3->ac_count[0];
			
			mcu_cols = (8*horizontal_sampling_factor);
			
			for(l=0;l<vertical_sampling_factor;l++)
			{
				for(m=0;m<horizontal_sampling_factor;m++)
				{
					jc_lshift(buffer,temp_buffer,mcu_cols);
					jc_dct(temp_buffer,jpeg_coeff,macro_block);
					jc_qz(temp_buffer,macro_block,qtbl);
					
					temp = macro_block[0];
					macro_block[0] = temp - s->last_dc_val[0];
					s->last_dc_val[0] = temp;
					err_val = jc_huffman_seq_dummy(s,macro_block,dc_count,ac_count);
					if(err_val != JC_ENCODE_SUCCESS)
						return err_val;
					
					buffer += 8;
				}
				buffer += (56*horizontal_sampling_factor);//8*7=56
			}
			
			mcu_cols = 8;
			
			dc_count = jcdata_l3->dc_count[1];
			ac_count = jcdata_l3->ac_count[1];
			
			for(k=1;k<components_in_scan;k++)
			{
				//horizontal_sampling_factor is always 1 for U.
				//vertical_sampling_factor is always 1 for V.
				
				buffer = (k==1) ? (s->bufferCb[toggle]) : (s->bufferCr[toggle]);
				
				qtbl = s->qtbl[k];
				
				jc_lshift(buffer,temp_buffer,mcu_cols);
				jc_dct(temp_buffer,jpeg_coeff,macro_block);
				jc_qz(temp_buffer,macro_block,qtbl);
				
				temp = macro_block[0];
				macro_block[0] = temp - s->last_dc_val[k];
				s->last_dc_val[k]= temp;
				err_val = jc_huffman_seq_dummy(s,macro_block,dc_count,ac_count);
				if(err_val != JC_ENCODE_SUCCESS)
					return err_val;
			}
			
			s->decoded_mcus++;
			
			jc_info->jc_progress_indicator = ((s->decoded_mcus * s->inverse_total_mcus)>>20) + 1;
			
			if(jc_info->jc_terminate==JC_TERMINATE_ENCODE)
			{
				return JC_TERMINATED;
			}
			
			nmcu--;
		}
	}
	
	return JC_ENCODE_SUCCESS;
}

/*******************************************************************************************/

GLOBAL(UINT8) jc_sequential(jcinfo_t *jc_info,UINT8 toggle)
{
	UINT8  err_val;
	UINT8  components_in_scan;
	UINT8  horizontal_sampling_factor,vertical_sampling_factor;
	INT16  temp;
	UINT16 i,j,k,l,m;
	UINT16 horizontal_mcus,vertical_mcus;
	UINT16 mcu_rows,mcu_cols;
	UINT32 nmcu;
	UINT8  *buffer;
	UINT16 *qtbl;
	INT16  *macro_block,*jpeg_coeff,*temp_buffer;
	
	UINT8 (* read_format)(jcstruct_t *s,UINT8 toggle);
	void  (* advance_ptr_block)(jcstruct_t *s,UINT16 mcu_cols);
	void  (* advance_ptr_row)(jcstruct_t *s,UINT16 mcu_rows);
	
	jcdata_l1_t *jcdata_l1;
	jcstruct_t *s;
	jchuffman_t *htbl;
	
	jcdata_l1 = (jcdata_l1_t *)(jc_info->l1_buf);
	
	s = &(jcdata_l1->s);
	
	jpeg_coeff = s->jpeg_coeff;
	
	macro_block = s->macro_block;
	
	temp_buffer = s->temp_buffer;
	
	read_format = s->read_format;
	advance_ptr_block = s->advance_ptr_block;
	advance_ptr_row = s->advance_ptr_row;
	
	s->huffbits = 0;
	s->huffcode = 0;
	
	s->decoded_mcus = 0;
	
	horizontal_mcus = s->horizontal_mcus;
	vertical_mcus = s->vertical_mcus;
	
	nmcu = (horizontal_mcus*vertical_mcus) - 1;
	
	components_in_scan = s->components_in_scan;
	
	horizontal_sampling_factor = s->horizontal_sampling_factor;
	vertical_sampling_factor = s->vertical_sampling_factor;
	
	k = l = m = 0;
	
	for(i=1;i<=vertical_mcus;i++)
	{
		s->lr_flag = (i == vertical_mcus) ? 1 : 0;
		
		mcu_rows = s->mcu_rows[s->lr_flag];
		
		for(j=1;j<=horizontal_mcus;j++)
		{
			s->lc_flag = (j == horizontal_mcus) ? 1 : 0;
			
			mcu_cols = s->mcu_cols[s->lc_flag];
			
			(*advance_ptr_block)(s,mcu_cols);
			
			if(s->lc_flag)
			{
				(*advance_ptr_row)(s,mcu_rows);
				s->lr_flag = (i == (vertical_mcus-1)) ? 1 : 0;
			}
			
			s->lc_flag = (j == (horizontal_mcus-1)) ? 1 : 0;
			if(nmcu!=0)
			{
				toggle = (*read_format)(s,toggle);
			}
			else
			{
				TOGGLE(toggle);
			}
			
			buffer = s->bufferY[toggle];
			
			qtbl = s->qtbl[0];
			htbl = s->htbl[0];
			
			mcu_cols = (8*horizontal_sampling_factor);
			
			for(l=0;l<vertical_sampling_factor;l++)
			{
				for(m=0;m<horizontal_sampling_factor;m++)
				{
					jc_lshift(buffer,temp_buffer,mcu_cols);
					jc_dct(temp_buffer,jpeg_coeff,macro_block);
					jc_qz(temp_buffer,macro_block,qtbl);
					
					temp = macro_block[0];
					macro_block[0] = temp - s->last_dc_val[0];
					s->last_dc_val[0] = temp;
					err_val = jc_huffman_seq(s,htbl,macro_block);
					if(err_val != JC_ENCODE_SUCCESS)
						return err_val;
					
					buffer += 8;
				}
				buffer += (56*horizontal_sampling_factor);//8*7=56
			}
			
			mcu_cols = 8;
			
			for(k=1;k<components_in_scan;k++)
			{
				//horizontal_sampling_factor is always 1 for U.
				//vertical_sampling_factor is always 1 for V.
				
				buffer = (k==1) ? (s->bufferCb[toggle]) : (s->bufferCr[toggle]);
				
				qtbl = s->qtbl[k];
				htbl = s->htbl[k];
				
				jc_lshift(buffer,temp_buffer,mcu_cols);
				jc_dct(temp_buffer,jpeg_coeff,macro_block);
				jc_qz(temp_buffer,macro_block,qtbl);
				
				temp = macro_block[0];
				macro_block[0] = temp - s->last_dc_val[k];
				s->last_dc_val[k]= temp;
				err_val = jc_huffman_seq(s,htbl,macro_block);
				if(err_val != JC_ENCODE_SUCCESS)
					return err_val;
			}
			
			s->decoded_mcus++;
			
			jc_info->jc_progress_indicator = ((s->decoded_mcus * s->inverse_total_mcus)>>20) + 1;
			
			if(jc_info->jc_terminate==JC_TERMINATE_ENCODE)
			{
				return JC_TERMINATED;
			}
			
			nmcu--;
		}
	}
	
	return JC_ENCODE_SUCCESS;
}

/*******************************************************************************************/
