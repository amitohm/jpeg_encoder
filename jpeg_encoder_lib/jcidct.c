#include "jcfunctions.h"

#define COS_4_PI_16	0x5A82	//C4
#define COS_6_PI_16	0x30FB	//C6
#define COS_2_PI_16	0x7641	//C2
#define COS_7_PI_16	0x18F8	//C7
#define COS_1_PI_16	0x7D8A	//C1
#define COS_5_PI_16	0x471C	//C5
#define COS_3_PI_16	0x6A6D	//C3

GLOBAL(void) jc_dct(short *data,short *coeff,short *buffer)
{
    unsigned short i,j;
    int a0,a1;
    int e0,e1,e2,e3,e4,e5,e6,e7;
    short *data_ptr,*buffer_ptr;

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

	    buffer[0]  = (short)(e0&0xFFFF);
	    buffer[8]  = (short)(e4&0xFFFF);
	    buffer[16] = (short)(e2&0xFFFF);
	    buffer[24] = (short)(e6&0xFFFF);
	    buffer[32] = (short)(e1&0xFFFF);
	    buffer[40] = (short)(e5&0xFFFF);
	    buffer[48] = (short)(e3&0xFFFF);
	    buffer[56] = (short)(e7&0xFFFF);

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

GLOBAL(void) jc_lshift(unsigned char *buffer,short *temp_buffer,short mcu_cols)
{
    unsigned char i,j;
    short val;

    for(i=0;i<8;i++)
    {
	for(j=0;j<8;j++)
	{
	    val = (short)(buffer[j]);
	    temp_buffer[j] = (val - 128);
	}
	buffer += mcu_cols;
	temp_buffer += 8;
    }

    return;
}

/*******************************************************************************************/

GLOBAL(void) jc_qz(short *temp_buffer,short *macro_block,unsigned short *quantization_table)
{
    int temp,qval;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[0] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[1]= (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[5] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[6] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[14] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[15] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[27] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);;
    temp = QMULTIPLY(temp,qval);
    macro_block[28]= (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[2] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[4] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[7] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[13] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[16] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[26] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[29] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[42] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[3] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[8] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[12] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[17] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[25] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[30] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[41] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[43] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[9] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[11] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[18] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[24] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[31] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[40] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[44] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[53] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[10] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[19] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[23] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[32] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[39] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[45] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[52] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[54] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[20] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[22] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[33] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[38] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[46] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[51] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[55] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[60] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[21] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[34] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[37] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[47] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[50] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[56] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[59] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[61] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[35] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[36] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[48] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[49] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[57] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[58] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[62] = (short)temp;

    qval = (int)(*quantization_table++);
    temp = (int)(*temp_buffer++);
    temp = QMULTIPLY(temp,qval);
    macro_block[63] = (short)temp;

    return;
}

/*******************************************************************************************/
