#include "jcfunctions.h"

#define COS_4_PI_16	0x5A82	//C4
#define COS_6_PI_16	0x30FB	//C6
#define COS_2_PI_16	0x7641	//C2
#define COS_7_PI_16	0x18F8	//C7
#define COS_1_PI_16	0x7D8A	//C1
#define COS_5_PI_16	0x471C	//C5
#define COS_3_PI_16	0x6A6D	//C3

GLOBAL(void) jc_dct(INT16 *data,INT16 *coeff,INT16 *buffer)
{
	UINT16 i,j;
	INT32 a0,a1;
	INT32 e0,e1,e2,e3,e4,e5,e6,e7;
	INT16 *data_ptr,*buffer_ptr;
	
	data_ptr = data;
	buffer_ptr = buffer;
	
	for(i=0;i<2;i++)
	{
		for(j=0;j<8;j++)
		{
			e0 = ((data[0] + data[7]) + 1)>>1;
			e7 = ((data[0] - data[7]) + 1)>>1;
			e1 = ((data[1] + data[6]) + 1)>>1;
			e6 = ((data[1] - data[6]) + 1)>>1;
			
			e2 = ((data[2] + data[5]) + 1)>>1;
			e5 = ((data[2] - data[5]) + 1)>>1;
			e3 = ((data[3] + data[4]) + 1)>>1;
			e4 = ((data[3] - data[4]) + 1)>>1;
			
			a0 = e0 + e3;	a1 = e0 - e3;
			e0 = a0;		e3 = a1;
			a0 = e1 + e2;	a1 = e1 - e2;
			e1 = a0;		e2 = a1;
			
			a0 = a1 = (e6*COS_4_PI_16);
			a0 = a0 + (e5*COS_4_PI_16);
			a1 = a1 - (e5*COS_4_PI_16);
			e5 = (a1 + 0x4000)>>15;
			e6 = (a0 + 0x4000)>>15;
			
			a0 = a1 = (e0*COS_4_PI_16);
			a0 = a0 + (e1*COS_4_PI_16);
			a1 = a1 - (e1*COS_4_PI_16);
			e1 = (a1 + 0x4000)>>15;
			e0 = (a0 + 0x4000)>>15;
			
			a0 = (e3*COS_6_PI_16);
			a1 = (e2*COS_6_PI_16);
			a0 = a0 - (e2*COS_2_PI_16);
			a1 = a1 + (e3*COS_2_PI_16);
			e2 = (a1 + 0x4000)>>15;
			e3 = (a0 + 0x4000)>>15;
			
			a0 = e4 + e5;	a1 = e4 - e5;
			e4 = a0;		e5 = a1;
			a0 = e7 + e6;	a1 = e7 - e6;
			e7 = a0;		e6 = a1;
			
			a0 = (e7*COS_7_PI_16);
			a1 = (e4*COS_7_PI_16);
			a0 = a0 - (e4*COS_1_PI_16);
			a1 = a1 + (e7*COS_1_PI_16);
			e4 = (a1 + 0x4000)>>15;
			e7 = (a0 + 0x4000)>>15;
			
			a0 = (e6*COS_3_PI_16);
			a1 = (e5*COS_3_PI_16);
			a0 = a0 - (e5*COS_5_PI_16);
			a1 = a1 + (e6*COS_5_PI_16);
			e5 = (a1 + 0x4000)>>15;
			e6 = (a0 + 0x4000)>>15;
			
			buffer[0]  = (INT16)(e0&0xFFFF);
			buffer[8]  = (INT16)(e4&0xFFFF);
			buffer[16] = (INT16)(e2&0xFFFF);
			buffer[24] = (INT16)(e6&0xFFFF);
			buffer[32] = (INT16)(e1&0xFFFF);
			buffer[40] = (INT16)(e5&0xFFFF);
			buffer[48] = (INT16)(e3&0xFFFF);
			buffer[56] = (INT16)(e7&0xFFFF);
			
			buffer += 1;//next column
			data += 8;//next row
		}
		//swap pointers
		data = buffer_ptr;
		buffer = data_ptr;
	}
	
	return;
}

/*******************************************************************************************/

GLOBAL(void) jc_lshift(UINT8 *buffer,INT16 *temp_buffer,INT16 mcu_cols)
{
	UINT8 i,j;
	INT16 val;
	
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			val = (INT16)(buffer[j]);
			temp_buffer[j] = (val - 128);
		}
		buffer += mcu_cols;
		temp_buffer += 8;
	}
	
	return;
}

/*******************************************************************************************/

GLOBAL(void) jc_qz(INT16 *temp_buffer,INT16 *macro_block,UINT16 *quantization_table)
{
	INT32 temp,qval;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[0] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[1]= (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[5] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[6] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[14] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[15] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[27] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);;
	temp = QMULTIPLY(temp,qval);
	macro_block[28]= (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[2] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[4] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[7] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[13] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[16] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[26] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[29] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[42] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[3] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[8] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[12] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[17] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[25] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[30] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[41] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[43] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[9] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[11] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[18] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[24] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[31] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[40] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[44] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[53] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[10] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[19] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[23] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[32] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[39] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[45] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[52] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[54] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[20] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[22] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[33] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[38] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[46] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[51] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[55] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[60] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[21] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[34] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[37] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[47] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[50] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[56] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[59] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[61] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[35] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[36] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[48] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[49] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[57] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[58] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[62] = (INT16)temp;
	
	qval = (INT32)(*quantization_table++);
	temp = (INT32)(*temp_buffer++);
	temp = QMULTIPLY(temp,qval);
	macro_block[63] = (INT16)temp;
	
	return;
}

/*******************************************************************************************/
