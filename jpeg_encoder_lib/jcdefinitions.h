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
	UINT8  *start_address;
	UINT16 dma_config;
	UINT16 xcount;
	UINT16 xmodify;
	UINT16 ycount;
	UINT16 ymodify;
};

typedef struct jchuffman_t
{
	UINT8  *huffbits_dc;
	UINT8  *huffbits_ac;
	UINT16 *huffcode_dc;
	UINT16 *huffcode_ac;
} jchuffman_t;

typedef struct jcstruct_t jcstruct_t;

struct jcstruct_t
{
	UINT8 write_headers;
	UINT8 write_data;
	UINT8 user_defined_qtbl;
	UINT8 components_in_scan;
	UINT8 components_in_frame;
	
	UINT8 lc_flag;//last mcu column flag
	UINT8 lr_flag;//last mcu row flag
	UINT8 mcu_rows[2];
	UINT8 mcu_cols[2];
	
	UINT8 horizontal_sampling_factor;
	UINT8 vertical_sampling_factor;
	
	UINT8 qtbl_selector[3];
	
	UINT16 number_of_samples_per_line;
	UINT16 number_of_lines;
	
	UINT16 horizontal_mcus;
	UINT16 vertical_mcus;
	
	INT16 last_dc_val[3];
	
	UINT32 huffcode;
	UINT32 huffbits;
	
	UINT32 inverse_total_mcus;
	UINT32 decoded_mcus;
	
	UINT8 *output_buf;
	
	UINT8 *dct_nbits;
	
	UINT8 *ptr[3];
	
	UINT8 *bufferY[2];
	UINT8 *bufferCb[2];
	UINT8 *bufferCr[2];
	
	INT16 *rgb_coeff;
	
	INT16 *jpeg_coeff;
	
	INT16 *macro_block;
	
	INT16 *temp_buffer;
	
	UINT16 *qtbl[3];
	
	UINT8 (* read_format)(jcstruct_t *s,UINT8 toggle);
	
	void (* advance_ptr_block)(jcstruct_t *s,UINT16 number);
	void (* advance_ptr_row)(jcstruct_t *s,UINT16 number);
	
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
	
	INT32  rgb_coeff[4];
	INT32  jpeg_coeff[4];
	
	INT32  macro_block[32];//to ensure buffer is 4 aligned
	
	INT32  temp_buffer[32];//to ensure buffer is 4 aligned
	
	UINT32 huffbits_dc[2][5];//2 rows of 20 UINT8 (17 reqd.)
	UINT32 huffbits_ac[2][64];//2 rows of 256 UINT8
	UINT32 huffcode_dc[2][9];//2 rows of 18 UINT16 (17 reqd.)
	UINT32 huffcode_ac[2][128];//2 rows of 256 UINT16.
	
	UINT32 dct_nbits[16];//stores nbits for dct values ranging from 0 - 64
	
	UINT32 bufferY[2][64];//bufferY = 256*2bytes.
	UINT32 bufferCb[2][16];//bufferCb = 64*2bytes.
	UINT32 bufferCr[2][16];//bufferCr = 64*2bytes.
	
	//MAX quantization tables possible = 4 ---> according to standard - CYMK color plane
	//MAX quantization tables possible for seq. jpg images = 3 - YCbCr color plane
	UINT32 quantization_table[3][32];//to ensure buffer is 4 aligned
	
	jcstruct_t s;
	
	jchuffman_t luminance_htbl;
	jchuffman_t chrominance_htbl;
	
} jcdata_l1_t;

typedef struct jcdata_l3_t
{
	// non L1
	UINT8 nval_tbl_dc[2][17];
	UINT8 nval_tbl_ac[2][17];
	UINT8 val_tbl_dc[2][16];
	UINT8 val_tbl_ac[2][256];
	
	UINT8 bits[33];
	UINT8 codesize[257];
	
	UINT8 quantization_table[3][64];
	
	INT16 others[257];
	
	UINT32 dc_count[2][257];
	UINT32 ac_count[2][257];
	
	//UINT32  bufferY[2][160];//bufferY = 640*2bytes.
	
	UINT8 *qtbl[3];
	
} jcdata_l3_t;

#endif
