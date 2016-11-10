#ifndef _JPEGENC_H_
#define _JPEGENC_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifndef NULL
#define NULL (void*)0
#endif

#define 	JC_WRITE_HEADERS_SUCCESS	100
#define 	JC_ENCODE_SUCCESS 			0

#define		JC_TERMINATED 				105

#define		INPUT_BUF_SIZE				131072	//128k

// switch between MDMA and core-copy
#define 	JC_MDMA_COPY		1
#define 	JC_CORE_COPY 		0

//select input format
#define		JC_INPUT_YUV400		0
#define 	JC_INPUT_YUV444 	1
#define		JC_INPUT_YUV420		2
#define		JC_INPUT_YUV422		3
#define		JC_INPUT_UYVY		4
#define 	JC_INPUT_RGB24		5

//jpeg optimize huffman tables
#define JC_DEFAULT_HUFFMAN_TABLES	0
#define JC_OPTIMIZE_HUFFMAN_TABLES	1

//enable subsampling - JPEG encoder output will be 420
#define JC_DISABLE_SUBSAMPLING		0
#define JC_ENABLE_SUBSAMPLING		1

//jpeg encode terminate
#define JC_TERMINATE_ENCODE		1

//Do Not Change these defines
//used internally for table selection
///////////////////////////////////////
#define		QTBL_LUMINANCE_Y		0
#define		QTBL_CHROMINANCE_CB		1
#define		QTBL_CHROMINANCE_CR		2

#define		HTBL_DC_LUMINANCE		0
#define		HTBL_AC_LUMINANCE		1
#define		HTBL_DC_CHROMINANCE		2
#define		HTBL_AC_CHROMINANCE		3
///////////////////////////////////////

typedef struct jcinfo_t
{
    unsigned char jc_copy_method;

    unsigned char jc_terminate;

    //select input image_format
    unsigned char jc_input_format;

    //specify quality factor (0 ... 100)
    unsigned char jc_quality_factor;

    unsigned char jc_optimize;

    unsigned char jc_subsample;

    unsigned int jc_progress_indicator;

    //dimensions
    unsigned int cols;
    unsigned int rows;

    unsigned char  *jc_input_buf;
    unsigned int jc_input_buf_size;

    unsigned char  *jc_output_buf;
    unsigned int jc_output_buf_size;

    unsigned int l1_buf_size;
    void *l1_buf;
    unsigned int l3_buf_size;
    void *l3_buf;

    void *opaque;//used for user defined structs,variables,etc

} jcinfo_t;

void jc_init_encode(jcinfo_t *jc_info);

void jc_set_defaults(jcinfo_t *jc_info);

unsigned char jc_write_headers(jcinfo_t *jc_info);

unsigned char jc_encode(jcinfo_t *jc_info);

void add_huffman_tables(jcinfo_t *jc_info,unsigned char table_selector,const unsigned char *nval,const unsigned char *val);

void add_quantization_tables(jcinfo_t *jc_info,unsigned char table_selector,const unsigned char *qtbl);

#endif
