#ifndef _JCFUNCTIONS_H_
#define _JCFUNCTIONS_H_

#include "jcdefinitions.h"

#define		ABS(X)			((X) < (0) ? (-X): (X))

#define		MIN(X,Y)		((X) < (Y) ? (X) : (Y))

#define		MAX(X,Y)		((X) > (Y) ? (X) : (Y))

#define		QINVERSE(X)		(((1L<<15)-1)/(X))

#define		QMULTIPLY(X,Y)	((X) < (0) ? (-((((-X)*(Y))+(0x4000))>>15)) : ((((X)*(Y))+(0x4000))>>15))

#define		TOGGLE(X)		do{(X++);(X) = (2 - (X));}while(0)

/* jcmarker.c */
unsigned char jc_put_markers(jcinfo_t *jc_info);
/* jcmarker.c */

/* jcread_asm.s */
unsigned char jc_read_400(jcstruct_t *s,unsigned char toggle);
unsigned char jc_read_444(jcstruct_t *s,unsigned char toggle);
unsigned char jc_read_444_420(jcstruct_t *s,unsigned char toggle);
unsigned char jc_read_420(jcstruct_t *s,unsigned char toggle);
unsigned char jc_read_422(jcstruct_t *s,unsigned char toggle);
unsigned char jc_read_422_420(jcstruct_t *s,unsigned char toggle);
unsigned char jc_read_uyvy(jcstruct_t *s,unsigned char toggle);
unsigned char jc_read_rgb24(jcstruct_t *s,unsigned char toggle);
unsigned char jc_read_rgb24_420(jcstruct_t *s,unsigned char toggle);
unsigned char jc_read_mb(jcstruct_t *s,unsigned char toggle);
/* jcread_asm.s */

/* jcwrite_asm.s */
void jc_memset(void *buf,int val,unsigned int size);
void jc_put_8_bits(jcstruct_t *s,unsigned char val);
void jc_put_16_bits(jcstruct_t *s,unsigned short val);
void jc_put_32_bits(jcstruct_t *s,unsigned int val);
/* jcwrite_asm.s */

/* jchuffman_asm.s */
void jc_empty_huffbuf(jcstruct_t *s);
unsigned char jc_huffman_seq(jcstruct_t *s,jchuffman_t *htbl,short *block);
void jc_finish_huffman(jcstruct_t *s);
/* jchuffman_asm.s */

/* jchuffman.c */
unsigned char jc_huffman_seq_dummy(jcstruct_t *s,short *block,unsigned int *dc_count,unsigned int *ac_count);
unsigned char jc_gen_huff_table(jcinfo_t *jc_info,unsigned char table_selector);
/* jchuffman.c */

/* quantization and zig-zag */
/* jcdct_asm.s */
void jc_dct(short *temp_buffer,short *jpeg_coeff,short *macro_block);
void jc_lshift(unsigned char *buffer,short *temp_buffer,short mcu_cols);
void jc_qz(short *temp_buffer,short *macro_block,unsigned short *quantization_table);
/* jcdct_asm.s */

/* jcencode.c */
unsigned char jc_sequential_dummy(jcinfo_t *jc_info,unsigned char toggle);
unsigned char jc_sequential(jcinfo_t *jc_info,unsigned char toggle);
/* jcencode.c */

/* jcadvanceptr.c*/
void jc_advance_ptr_block_400(jcstruct_t *s,unsigned short mcu_cols);
void jc_advance_ptr_row_400(jcstruct_t *s,unsigned short mcu_rows);
void jc_advance_ptr_block_444(jcstruct_t *s,unsigned short mcu_cols);
void jc_advance_ptr_row_444(jcstruct_t *s,unsigned short mcu_rows);
void jc_advance_ptr_block_420(jcstruct_t *s,unsigned short mcu_cols);
void jc_advance_ptr_row_420(jcstruct_t *s,unsigned short mcu_rows);
void jc_advance_ptr_block_422(jcstruct_t *s,unsigned short mcu_cols);
void jc_advance_ptr_row_422(jcstruct_t *s,unsigned short mcu_rows);
void jc_advance_ptr_block_uyvy(jcstruct_t *s,unsigned short mcu_cols);
void jc_advance_ptr_row_uyvy(jcstruct_t *s,unsigned short mcu_rows);
void jc_advance_ptr_block_rgb24(jcstruct_t *s,unsigned short mcu_cols);
void jc_advance_ptr_row_rgb24(jcstruct_t *s,unsigned short mcu_rows);
void jc_advance_ptr_block_mb(jcstruct_t *s,unsigned short mcu_cols);
void jc_advance_ptr_row_mb(jcstruct_t *s,unsigned short mcu_rows);
/* jcadvanceptr.c*/

#endif /* file's endif */
