#include "jcerror.h"
#include "jcfunctions.h"

/*******************************************************************************************/

GLOBAL(void) jc_empty_huffbuf(jcstruct_t *s)
{
	UINT8 code;
	
	while(s->huffbits>=8)
	{
		s->huffbits -= 8;
		code = ((s->huffcode)>>(s->huffbits));
		code &= 0xFF;
		
		*(s->output_buf++) = code;
		if(code==0xFF)
			*(s->output_buf++) = 0;
	}
	
	return;
}

/*******************************************************************************************/

GLOBAL(UINT8) jc_huffman_seq(jcstruct_t *s,jchuffman_t *htbl,INT16 *block)
{
	INT16 i,temp,temp1,temp2;
	UINT32 r,nbits,code;
	
	UINT8  *dct_nbits;
	UINT8  *huffbits_table;
	UINT16 *huffcode_table;
	
	dct_nbits = s->dct_nbits;
	
	//DC
	huffbits_table = htbl->huffbits_dc;
	huffcode_table = htbl->huffcode_dc;
	
	temp = temp1 = block[0];
	
	if(temp<0)
	{
		temp = -temp;
		temp1--;
	}
	
	temp2 = temp>>6;
	if(temp2>0)
	{
		temp = temp2;
		temp2 = 6;
	}
	nbits = dct_nbits[temp] + temp2;
	
	if(nbits>11)//check if nbits is > 11
		return ERROR_INVALID_DCT_COEFF;
	
	temp = huffbits_table[nbits];
	code = huffcode_table[nbits];
	
	s->huffbits += temp;
	
	code = code&((1L<<temp) - 1);
	
	s->huffcode <<= temp;
	
	s->huffcode |= code;
	
	if(s->huffbits>16)
		jc_empty_huffbuf(s);
	
	s->huffbits += nbits;
	
	code = temp1&((1L<<nbits) - 1);
	
	s->huffcode <<= nbits;
	
	s->huffcode |= code;
	
	if(s->huffbits>16)
		jc_empty_huffbuf(s);
	
	//AC
	huffbits_table = htbl->huffbits_ac;
	huffcode_table = htbl->huffcode_ac;
	
	r = 0;
	
	for(i=1;i<64;i++)
	{
		temp = temp1 = block[i];
		
		if(temp==0)
		{
			r++;
		}
		else
		{
			while(r>15)
			{
				nbits = huffbits_table[0xF0];
				code = huffcode_table[0xF0];
				
				s->huffbits += nbits;
				
				code = code&((1L<<nbits) - 1);
				
				s->huffcode <<= nbits;
				
				s->huffcode |= code;
				
				if(s->huffbits>16)
					jc_empty_huffbuf(s);
				
				r -= 16;
			}
			
			if(temp<0)
			{
				temp = -temp;
				temp1--;
			}
			
			temp2 = temp>>6;
			if(temp2>0)
			{
				temp = temp2;
				temp2 = 6;
			}
			nbits = dct_nbits[temp] + temp2;
			
			if(nbits>10)//check if nbits is > 10
				return ERROR_INVALID_DCT_COEFF;
			
			temp = (r<<4) + nbits;
			
			code = huffcode_table[temp];
			temp = huffbits_table[temp];
			
			s->huffbits += temp;
			
			code = code&((1L<<temp) - 1);
			
			s->huffcode <<= temp;
			
			s->huffcode |= code;
			
			if(s->huffbits>16)
				jc_empty_huffbuf(s);
			
			s->huffbits += nbits;
			
			code = temp1&((1L<<nbits) - 1);
			
			s->huffcode <<= nbits;
			
			s->huffcode |= code;
			
			if(s->huffbits>16)
				jc_empty_huffbuf(s);
			
			r = 0;
		}
	}
	
	if(r>0)
	{
		nbits = huffbits_table[0];
		code = huffcode_table[0];
		
		s->huffbits += nbits;
		
		code = code&((1L<<nbits) - 1);
		
		s->huffcode <<= nbits;
		
		s->huffcode |= code;
		
		if(s->huffbits>16)
			jc_empty_huffbuf(s);
	}
	
    return JC_ENCODE_SUCCESS;
}

/*******************************************************************************************/

GLOBAL(void) jc_finish_huffman(jcstruct_t *s)
{
	UINT32 huffbits,huffcode;
	UINT32 nbits,code;
	
	if(s->huffbits>0)
	{
		huffbits = s->huffbits;
		huffcode = s->huffcode;
		
		nbits = ((huffbits + 7)>>3)<<3;
		nbits -= huffbits;
		huffbits += nbits;
		
		code = ((1L<<nbits) - 1);
		
		huffcode <<= nbits;
		huffcode  |= code;
		
		while(huffbits)
		{
			huffbits -= 8;
			code = huffcode>>huffbits;
			code &= 0xFF;
			
			*(s->output_buf++) = code;
			if(code==0xFF)
				*(s->output_buf++) = 0;
		}
	}
	
	return;
}

/*******************************************************************************************/

GLOBAL(UINT8) jc_huffman_seq_dummy(jcstruct_t *s,INT16 *block,UINT32 *dc_count,UINT32 *ac_count)
{
	INT16 i,temp,temp1,temp2;
	UINT32 r,nbits;
	
	UINT8  *dct_nbits;
	
	dct_nbits = s->dct_nbits;
	
	//DC
	temp = temp1 = block[0];
	
	if(temp<0)
	{
		temp = -temp;
		temp1--;
	}
	
	temp2 = temp>>6;
	if(temp2>0)
	{
		temp = temp2;
		temp2 = 6;
	}
	nbits = dct_nbits[temp] + temp2;
	
	if(nbits>11)//check if nbits is > 11
		return ERROR_INVALID_DCT_COEFF;
	
	dc_count[nbits]++;
	
	//AC
	r = 0;
	
	for(i=1;i<64;i++)
	{
		temp = temp1 = block[i];
		
		if(temp==0)
		{
			r++;
		}
		else
		{
			while(r>15)
			{
				ac_count[0xF0]++;
				r -= 16;
			}
			
			if(temp<0)
			{
				temp = -temp;
				temp1--;
			}
			
			temp2 = temp>>6;
			if(temp2>0)
			{
				temp = temp2;
				temp2 = 6;
			}
			nbits = dct_nbits[temp] + temp2;
			
			if(nbits>10)//check if nbits is > 10
				return ERROR_INVALID_DCT_COEFF;
			
			temp = (r<<4) + nbits;
			
			ac_count[temp]++;
			
			r = 0;
		}
	}
	
	if(r>0)
	{
		ac_count[0]++;
	}
	
    return JC_ENCODE_SUCCESS;
}

/*******************************************************************************************/

GLOBAL(UINT8) jc_gen_huff_table(jcinfo_t *jc_info,UINT8 table_selector)
{
	INT16 i,j;
	INT16 c1,c2;
	UINT32 min;
	
	UINT8 *nval_tbl,*val_tbl;
	UINT8 *bits,*codesize;
	INT16 *others;
	UINT32 *count;
	
	jcdata_l3_t *jcdata_l3;
	
	jcdata_l3 = (jcdata_l3_t *)(jc_info->l3_buf);
	
	/* This algorithm is explained in section K.2 of the JPEG standard */
	
	bits = jcdata_l3->bits;
	codesize = jcdata_l3->codesize;
	others = jcdata_l3->others;
	
	jc_memset(bits,0,(sizeof(UINT8)*33));
	jc_memset(codesize,0,(sizeof(UINT8)*257));
	jc_memset(others,-1,(sizeof(INT16)*257));
	
	switch(table_selector)
	{
	case HTBL_DC_LUMINANCE:
		count = jcdata_l3->dc_count[0];
		nval_tbl = jcdata_l3->nval_tbl_dc[0];
		val_tbl = jcdata_l3->val_tbl_dc[0];
		break;
	
	case HTBL_AC_LUMINANCE:
		count = jcdata_l3->ac_count[0];
		nval_tbl = jcdata_l3->nval_tbl_ac[0];
		val_tbl = jcdata_l3->val_tbl_ac[0];
		break;
	
	case HTBL_DC_CHROMINANCE:
		count = jcdata_l3->dc_count[1];
		nval_tbl = jcdata_l3->nval_tbl_dc[1];
		val_tbl = jcdata_l3->val_tbl_dc[1];
		break;
	
	case HTBL_AC_CHROMINANCE:
		count = jcdata_l3->ac_count[1];
		nval_tbl = jcdata_l3->nval_tbl_ac[1];
		val_tbl = jcdata_l3->val_tbl_ac[1];
		break;
	}
	
	count[256] = 1;
	
	do
	{
		c1 = -1;
		min = 1<<31;
		for(i=0;i<=256;i++)
		{
			if((count[i]!=0)&&(count[i]<=min))
			{
				min = count[i];
				c1 = i;
			}
		}
		
		c2 = -1;
		min = 1<<31;
		for(i=0;i<=256;i++)
		{
			if((count[i]!=0)&&(count[i]<=min)&&(i!=c1))
			{
				min = count[i];
				c2 = i;
			}
		}
		
		if(c2<0)
			break;
		
		count[c1] += count[c2];
		count[c2] = 0;
		
		codesize[c1]++;
		while(others[c1]>=0)
		{
			c1 = others[c1];
			codesize[c1]++;
		}
		
		others[c1] = c2;
		
		codesize[c2]++;
		while(others[c2]>=0)
		{
			c2 = others[c2];
			codesize[c2]++;
		}
		
	} while(1);
	
	for(i=0;i<=256;i++)
	{
		if(codesize[i]!=0)
		{
			if(codesize[i] > 32)
				return ERROR_HUFFCODE_OVERFLOW;
			bits[codesize[i]]++;
		}
	}
	
	for(i=32;i>16;i--)
	{
		while(bits[i]>0)
		{
			j = i - 2;
			
			while(bits[j]==0)
				j--;
			
			bits[i] -= 2;
			bits[i-1] += 1;
			bits[j+1] += 2;
			bits[j] -= 1;
		}
	}
	
	while(bits[i]==0)
		i--;
	bits[i]--;
	
	for(i=0;i<=16;i++)
	{
		nval_tbl[i] = bits[i];
	}
	
	min = 0;
	for(i=1;i<=32;i++)
	{
		for(j=0;j<=255;j++)
		{
			if(codesize[j]==i)
			{
				val_tbl[min] = (UINT8)j;
				min++;
			}
		}
	}
	
	return JC_ENCODE_SUCCESS;
}

/*******************************************************************************************/
