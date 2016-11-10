#ifndef _JCDEFINITIONS_H_
#define _JCDEFINITIONS_H_

#include "../test_app/jpegenc.h"

#define LOCAL(type)		static type

#define GLOBAL(type)	type

////////////////////////
// internal to encoder
////////////////////////

#define		TRUE			1
#define		FALSE			0

// encode modes for the jpeg encoder
#define		JC_WRITE_SOS		0
#define		JC_WRITE_DATA		1

#define		JC_WRITE_HEADERS 	1
#define		JC_ENCODE	 		0

#define		NBITS			8
#define		MIN_HUFFBITS	16	//32-16
#define		MAX_HUFFBITS	32

#define MAX_BLOCKS_PER_MCU		12


//#define		JC_INPUT_YUV400		0
//#define 		JC_INPUT_YUV444 	1
//#define		JC_INPUT_YUV420		2
//#define		JC_INPUT_YUV422		3
//#define		JC_INPUT_UYVY		4
//#define	 	JC_INPUT_RGB24		5
#define			JC_INPUT_MB			11

// DMA descriptor large descriptor list mode
typedef struct dma_descriptor_t dma_descriptor_t;

struct dma_descriptor_t
{
	dma_descriptor_t *next_descpriptor_ptr;
	unsigned char  *start_address;
	unsigned short dma_config;
	unsigned short xcount;
	unsigned short xmodify;
	unsigned short ycount;
	unsigned short ymodify;
};

typedef struct jchuffman_t
{
	unsigned char  *huffbits_dc;
	unsigned char  *huffbits_ac;
	unsigned short *huffcode_dc;
	unsigned short *huffcode_ac;
} jchuffman_t;

typedef struct jcstruct_t jcstruct_t;

struct jcstruct_t
{
	unsigned char write_headers;
	unsigned char write_data;
	unsigned char user_defined_qtbl;
	unsigned char components_in_scan;
	unsigned char components_in_frame;
	
	unsigned char lc_flag;//last mcu column flag
	unsigned char lr_flag;//last mcu row flag
	unsigned char mcu_rows[2];
	unsigned char mcu_cols[2];
	
	unsigned char horizontal_sampling_factor;
	unsigned char vertical_sampling_factor;
	
	unsigned char qtbl_selector[3];
	
	unsigned short number_of_samples_per_line;
	unsigned short number_of_lines;
	
	unsigned short horizontal_mcus;
	unsigned short vertical_mcus;
	
	short last_dc_val[3];
	
	unsigned int huffcode;
	unsigned int huffbits;
	
	unsigned int inverse_total_mcus;
	unsigned int decoded_mcus;
	
	unsigned char *output_buf;
	
	unsigned char *dct_nbits;
	
	unsigned char *ptr[3];
	
	unsigned char *bufferY[2];
	unsigned char *bufferCb[2];
	unsigned char *bufferCr[2];
	
	short *rgb_coeff;
	
	short *jpeg_coeff;
	
	short *macro_block;
	
	short *temp_buffer;
	
	unsigned short *qtbl[3];
	
	unsigned char (* read_format)(jcstruct_t *s,unsigned char toggle);
	
	void (* advance_ptr_block)(jcstruct_t *s,unsigned short number);
	void (* advance_ptr_row)(jcstruct_t *s,unsigned short number);
	
	jchuffman_t *htbl[3];
	
	dma_descriptor_t *dma_sptr;
	dma_descriptor_t *dma_dptr;
};

typedef struct jcdata_l1_t
{
	dma_descriptor_t Y_sptr;
	dma_descriptor_t Cb_sptr;
	dma_descriptor_t Cr_sptr;
	
	dma_descriptor_t Y_dptr;
	dma_descriptor_t Cb_dptr;
	dma_descriptor_t Cr_dptr;
	
	int  rgb_coeff[4];
	int  jpeg_coeff[4];
	
	int  macro_block[32];//to ensure buffer is 4 aligned
	
	int  temp_buffer[32];//to ensure buffer is 4 aligned
	
	unsigned int huffbits_dc[2][5];//2 rows of 20 unsigned char (17 reqd.)
	unsigned int huffbits_ac[2][64];//2 rows of 256 unsigned char
	unsigned int huffcode_dc[2][9];//2 rows of 18 unsigned short (17 reqd.)
	unsigned int huffcode_ac[2][128];//2 rows of 256 unsigned short.
	
	unsigned int dct_nbits[16];//stores nbits for dct values ranging from 0 - 64
	
	unsigned int bufferY[2][64];//bufferY = 256*2bytes.
	unsigned int bufferCb[2][16];//bufferCb = 64*2bytes.
	unsigned int bufferCr[2][16];//bufferCr = 64*2bytes.
	
	//MAX quantization tables possible = 4 ---> according to standard - CYMK color plane
	//MAX quantization tables possible for seq. jpg images = 3 - YCbCr color plane
	unsigned int quantization_table[3][32];//to ensure buffer is 4 aligned
	
	jcstruct_t s;
	
	jchuffman_t luminance_htbl;
	jchuffman_t chrominance_htbl;
	
} jcdata_l1_t;

typedef struct jcdata_l3_t
{
	// non L1
	unsigned char nval_tbl_dc[2][17];
	unsigned char nval_tbl_ac[2][17];
	unsigned char val_tbl_dc[2][16];
	unsigned char val_tbl_ac[2][256];
	
	unsigned char bits[33];
	unsigned char codesize[257];
	
	unsigned char quantization_table[3][64];
	
	short others[257];
	
	unsigned int dc_count[2][257];
	unsigned int ac_count[2][257];
	
	//unsigned int  bufferY[2][160];//bufferY = 640*2bytes.
	
	unsigned char *qtbl[3];
	
} jcdata_l3_t;

#endif
