#include "jcfunctions.h"

//*******************************************************************************************//

GLOBAL(unsigned char) jc_read_400(jcstruct_t *s,unsigned char toggle)
{
	unsigned short mcu_cols,mcu_rows;
	unsigned int i,j,offset;
	unsigned char *ptr0;
	unsigned char *ptrY;
	
	mcu_cols = s->mcu_cols[s->lc_flag];
	mcu_rows = s->mcu_rows[s->lr_flag];
	
	ptr0 = s->ptr[0];
	
	ptrY = s->bufferY[toggle];
	
	offset = (s->number_of_samples_per_line);
	
	for(i=0;i<mcu_rows;i++)
	{
		for(j=0;j<mcu_cols;j++)
		{
			ptrY[(i<<3)+j]  = ptr0[j];
		}
		ptr0 += offset;
	}
	
	TOGGLE(toggle);
	
	return toggle;
}

//*******************************************************************************************//

GLOBAL(unsigned char) jc_read_444(jcstruct_t *s,unsigned char toggle)
{
	unsigned short mcu_cols,mcu_rows;
	unsigned int i,j,offset;
	unsigned char *ptr0,*ptr1,*ptr2;
	unsigned char *ptrY,*ptrCb,*ptrCr;
	
	mcu_cols = s->mcu_cols[s->lc_flag];
	mcu_rows = s->mcu_rows[s->lr_flag];
	
	ptr0 = s->ptr[0];
	ptr1 = s->ptr[1];
	ptr2 = s->ptr[2];
	
	ptrY = s->bufferY[toggle];
	ptrCb = s->bufferCb[toggle];
	ptrCr = s->bufferCr[toggle];
	
	offset = (s->number_of_samples_per_line);
	
	for(i=0;i<mcu_rows;i++)
	{
		for(j=0;j<mcu_cols;j++)
		{
			ptrY[(i<<3)+j]  = ptr0[j];
			ptrCb[(i<<3)+j] = ptr1[j];
			ptrCr[(i<<3)+j] = ptr2[j];
		}
		ptr0 += offset;
		ptr1 += offset;
		ptr2 += offset;
	}
	
	TOGGLE(toggle);
	
	return toggle;
}

//*******************************************************************************************//

GLOBAL(unsigned char) jc_read_444_420(jcstruct_t *s,unsigned char toggle)
{
	unsigned short mcu_cols,mcu_rows;
	unsigned int i,j,offset;
	unsigned char *ptr0,*ptr1,*ptr2;
	unsigned char *ptrY,*ptrCb,*ptrCr;
	
	mcu_cols = s->mcu_cols[s->lc_flag];
	mcu_rows = s->mcu_rows[s->lr_flag];
	
	ptr0 = s->ptr[0];
	ptr1 = s->ptr[1];
	ptr2 = s->ptr[2];
	
	ptrY = s->bufferY[toggle];
	ptrCb = s->bufferCb[toggle];
	ptrCr = s->bufferCr[toggle];
	
	offset = (s->number_of_samples_per_line);
	
	for(i=0;i<mcu_rows;i++)
	{
		for(j=0;j<mcu_cols;j++)
		{
			ptrY[(i<<4)+j]  = ptr0[j];
		}
		ptr0 += offset;
	}
	
	mcu_cols >>= 1;
	mcu_rows >>= 1;
	
	offset <<= 1;
	
	for(i=0;i<mcu_rows;i++)
	{
		for(j=0;j<mcu_cols;j++)
		{
			ptrCb[(i<<3)+j] = ptr1[(j<<1)];
			ptrCr[(i<<3)+j] = ptr2[(j<<1)];
		}
		ptr1 += offset;
		ptr2 += offset;
	}
	
	TOGGLE(toggle);
	
	return toggle;
}

//*******************************************************************************************//

GLOBAL(unsigned char) jc_read_420(jcstruct_t *s,unsigned char toggle)
{
	unsigned short mcu_cols,mcu_rows;
	unsigned int i,j,offset;
	unsigned char *ptr0,*ptr1,*ptr2;
	unsigned char *ptrY,*ptrCb,*ptrCr;
	
	mcu_cols = s->mcu_cols[s->lc_flag];
	mcu_rows = s->mcu_rows[s->lr_flag];
	
	ptr0 = s->ptr[0];
	ptr1 = s->ptr[1];
	ptr2 = s->ptr[2];
	
	ptrY = s->bufferY[toggle];
	ptrCb = s->bufferCb[toggle];
	ptrCr = s->bufferCr[toggle];
	
	offset = (s->number_of_samples_per_line);
	
	for(i=0;i<mcu_rows;i++)
	{
		for(j=0;j<mcu_cols;j++)
		{
			ptrY[(i<<4)+j]  = ptr0[j];
		}
		ptr0 += offset;
	}
	
	mcu_cols >>= 1;
	mcu_rows >>= 1;
	
	offset >>= 1;
	
	for(i=0;i<mcu_rows;i++)
	{
		for(j=0;j<mcu_cols;j++)
		{
			ptrCb[(i<<3)+j] = ptr1[j];
			ptrCr[(i<<3)+j] = ptr2[j];
		}
		ptr1 += offset;
		ptr2 += offset;
	}
	
	TOGGLE(toggle);
	
	return toggle;
}

//*******************************************************************************************//

GLOBAL(unsigned char) jc_read_422(jcstruct_t *s,unsigned char toggle)
{
	unsigned short mcu_cols,mcu_rows;
	unsigned int i,j,offset;
	unsigned char *ptr0,*ptr1,*ptr2;
	unsigned char *ptrY,*ptrCb,*ptrCr;
	
	mcu_cols = s->mcu_cols[s->lc_flag];
	mcu_rows = s->mcu_rows[s->lr_flag];
	
	ptr0 = s->ptr[0];
	ptr1 = s->ptr[1];
	ptr2 = s->ptr[2];
	
	ptrY = s->bufferY[toggle];
	ptrCb = s->bufferCb[toggle];
	ptrCr = s->bufferCr[toggle];
	
	offset = (s->number_of_samples_per_line);
	
	for(i=0;i<mcu_rows;i++)
	{
		for(j=0;j<mcu_cols;j++)
		{
			ptrY[(i<<4)+j]  = ptr0[j];
		}
		ptr0 += offset;
	}
	
	mcu_cols >>= 1;
	
	offset >>= 1;
	
	for(i=0;i<mcu_rows;i++)
	{
		for(j=0;j<mcu_cols;j++)
		{
			ptrCb[(i<<3)+j] = ptr1[j];
			ptrCr[(i<<3)+j] = ptr2[j];
		}
		ptr1 += offset;
		ptr2 += offset;
	}
	
	TOGGLE(toggle);
	
	return toggle;
}

//*******************************************************************************************//

GLOBAL(unsigned char) jc_read_422_420(jcstruct_t *s,unsigned char toggle)
{
	unsigned short mcu_cols,mcu_rows;
	unsigned int i,j,offset;
	unsigned char *ptr0,*ptr1,*ptr2;
	unsigned char *ptrY,*ptrCb,*ptrCr;
	
	mcu_cols = s->mcu_cols[s->lc_flag];
	mcu_rows = s->mcu_rows[s->lr_flag];
	
	ptr0 = s->ptr[0];
	ptr1 = s->ptr[1];
	ptr2 = s->ptr[2];
	
	ptrY = s->bufferY[toggle];
	ptrCb = s->bufferCb[toggle];
	ptrCr = s->bufferCr[toggle];
	
	offset = (s->number_of_samples_per_line);
	
	for(i=0;i<mcu_rows;i++)
	{
		for(j=0;j<mcu_cols;j++)
		{
			ptrY[(i<<4)+j]  = ptr0[j];
		}
		ptr0 += offset;
	}
	
	mcu_cols >>= 1;
	mcu_rows >>= 1;
	
	for(i=0;i<mcu_rows;i++)
	{
		for(j=0;j<mcu_cols;j++)
		{
			ptrCb[(i<<3)+j] = ptr1[j];
			ptrCr[(i<<3)+j] = ptr2[j];
		}
		ptr1 += offset;
		ptr2 += offset;
	}
	
	TOGGLE(toggle);
	
	return toggle;
}

//*******************************************************************************************//

GLOBAL(unsigned char) jc_read_uyvy(jcstruct_t *s,unsigned char toggle)
{
	unsigned short mcu_cols,mcu_rows;
	unsigned int i,j,offset;
	unsigned char *ptr0,*ptr1,*ptr2;
	unsigned char *ptrY,*ptrCb,*ptrCr;
	
	mcu_cols = s->mcu_cols[s->lc_flag];
	mcu_rows = s->mcu_rows[s->lr_flag];
	
	ptr0 = s->ptr[0];
	ptr1 = s->ptr[1];
	ptr2 = s->ptr[2];
	
	ptrY = s->bufferY[toggle];
	ptrCb = s->bufferCb[toggle];
	ptrCr = s->bufferCr[toggle];
	
	offset = 2*(s->number_of_samples_per_line);
	
	for(i=0;i<mcu_rows;i++)
	{
		for(j=0;j<mcu_cols;j++)
		{
			ptrY[(i<<4)+j]  = ptr0[(j<<1)];
		}
		ptr0 += offset;
	}
	
	mcu_cols >>= 1;
	mcu_rows >>= 1;
	
	offset <<= 1;
	
	for(i=0;i<mcu_rows;i++)
	{
		for(j=0;j<mcu_cols;j++)
		{
			ptrCb[(i<<3)+j] = ptr1[(j<<2)];
			ptrCr[(i<<3)+j] = ptr2[(j<<2)];
		}
		ptr1 += offset;
		ptr2 += offset;
	}
	
	TOGGLE(toggle);
	
	return toggle;
}

//*******************************************************************************************//

GLOBAL(unsigned char) jc_read_rgb24(jcstruct_t *s,unsigned char toggle)
{
	unsigned short mcu_cols,mcu_rows;
	int t,r,g,b;
	unsigned int i,j,offset;
	unsigned char *ptr,*ptrY,*ptrCb,*ptrCr;
	short *rgb_coeff;
	
	rgb_coeff = s->rgb_coeff;
	
	mcu_cols = s->mcu_cols[s->lc_flag];
	mcu_rows = s->mcu_rows[s->lr_flag];
	
	ptr = s->ptr[0];
	
	ptrY = s->bufferY[toggle];
	ptrCb = s->bufferCb[toggle];
	ptrCr = s->bufferCr[toggle];
	
	offset = 3*(s->number_of_samples_per_line);
	
	for(i=0;i<mcu_rows;i++)
	{
		for(j=0;j<mcu_cols;j++)
		{
			r = ptr[(3*j) + 0];
			g = ptr[(3*j) + 1];
			b = ptr[(3*j) + 2];
			
			t = (r*rgb_coeff[0]) + (g*rgb_coeff[1]) + (b*rgb_coeff[2]);
			t >>= 15;
			t = MIN(255,t);
			t = MAX(0,t);
			ptrY[(i<<3)+j] = (unsigned char)t;
			
			t = (r*rgb_coeff[3]) + (g*rgb_coeff[6]) + (b*rgb_coeff[4]);
			t >>= 15;
			t += 128;
			t = MIN(255,t);
			t = MAX(0,t);
			ptrCb[(i<<3)+j] = (unsigned char)t;
			
			t = (r*rgb_coeff[4]) + (g*rgb_coeff[7]) + (b*rgb_coeff[5]);
			t >>= 15;
			t += 128;
			t = MIN(255,t);
			t = MAX(0,t);
			ptrCr[(i<<3)+j] = (unsigned char)t;
		}
		ptr += offset;
	}
	
	TOGGLE(toggle);
	
	return toggle;
}

//*******************************************************************************************//

GLOBAL(unsigned char) jc_read_rgb24_420(jcstruct_t *s,unsigned char toggle)
{
	unsigned short mcu_cols,mcu_rows;
	int t,r,g,b;
	unsigned int i,j,offset;
	unsigned char *ptr,*ptrY,*ptrCb,*ptrCr;
	short *rgb_coeff;
	
	rgb_coeff = s->rgb_coeff;
	
	mcu_cols = s->mcu_cols[s->lc_flag];
	mcu_rows = s->mcu_rows[s->lr_flag];
	
	ptr = s->ptr[0];
	
	ptrY = s->bufferY[toggle];
	ptrCb = s->bufferCb[toggle];
	ptrCr = s->bufferCr[toggle];
	
	offset = 3*(s->number_of_samples_per_line);
	
	for(i=0;i<mcu_rows;i++)
	{
		for(j=0;j<mcu_cols;j++)
		{
			r = ptr[(3*j) + 0];
			g = ptr[(3*j) + 1];
			b = ptr[(3*j) + 2];
			
			t = (r*rgb_coeff[0]) + (g*rgb_coeff[1]) + (b*rgb_coeff[2]);
			t >>= 15;
			t = MIN(255,t);
			t = MAX(0,t);
			ptrY[(i<<4)+j] = (unsigned char)t;
		}
		ptr += offset;
	}
	
	mcu_cols >>= 1;
	mcu_rows >>= 1;
	
	ptr = s->ptr[0];
	
	for(i=0;i<mcu_rows;i++)
	{
		for(j=0;j<mcu_cols;j++)
		{
			r = ptr[(6*j) + 0];
			g = ptr[(6*j) + 1];
			b = ptr[(6*j) + 2];
			
			t = (r*rgb_coeff[3]) + (g*rgb_coeff[6]) + (b*rgb_coeff[4]);
			t >>= 15;
			t += 128;
			t = MIN(255,t);
			t = MAX(0,t);
			ptrCb[(i<<3)+j] = (unsigned char)t;
			
			t = (r*rgb_coeff[4]) + (g*rgb_coeff[7]) + (b*rgb_coeff[5]);
			t >>= 15;
			t += 128;
			t = MIN(255,t);
			t = MAX(0,t);
			ptrCr[(i<<3)+j] = (unsigned char)t;
		}
		ptr += offset;
	}
	
	TOGGLE(toggle);
	
	return toggle;
}

//*******************************************************************************************//

GLOBAL(unsigned char) jc_read_mb(jcstruct_t *s,unsigned char toggle)
{
	unsigned short mcu_cols,mcu_rows;
	unsigned int i,j;
	unsigned char *ptr,*ptrY,*ptrCb,*ptrCr;
	
	mcu_cols = s->mcu_cols[s->lc_flag];
	mcu_rows = s->mcu_rows[s->lr_flag];
	
	ptr = s->ptr[0];
	
	ptrY = s->bufferY[toggle];
	ptrCb = s->bufferCb[toggle];
	ptrCr = s->bufferCr[toggle];
	
	for(i=0;i<mcu_rows;i++)
	{
		for(j=0;j<mcu_cols;j++)
		{
			ptrY[(i*mcu_cols) + j] = *ptr++;
		}
	}
	
	mcu_cols >>= 1;
	mcu_rows >>= 1;
	
	for(i=0;i<mcu_rows;i++)
	{
		for(j=0;j<mcu_cols;j++)
		{
			ptrCb[(i*mcu_cols) + j] = *ptr++;
		}
	}

	for(i=0;i<mcu_rows;i++)
	{
		for(j=0;j<mcu_cols;j++)
		{
			ptrCr[(i*mcu_cols) + j] = *ptr++;
		}
	}	
	
	TOGGLE(toggle);
	
	return toggle;
}

//*******************************************************************************************//
