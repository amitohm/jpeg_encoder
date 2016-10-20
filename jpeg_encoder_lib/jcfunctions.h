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
GLOBAL(UINT8) jc_put_markers(jcinfo_t *jc_info);
/* jcmarker.c */

/* jcread_asm.s */
GLOBAL(UINT8) jc_read_400(jcstruct_t *s,UINT8 toggle);
GLOBAL(UINT8) jc_read_444(jcstruct_t *s,UINT8 toggle);
GLOBAL(UINT8) jc_read_444_420(jcstruct_t *s,UINT8 toggle);
GLOBAL(UINT8) jc_read_420(jcstruct_t *s,UINT8 toggle);
GLOBAL(UINT8) jc_read_422(jcstruct_t *s,UINT8 toggle);
GLOBAL(UINT8) jc_read_422_420(jcstruct_t *s,UINT8 toggle);
GLOBAL(UINT8) jc_read_uyvy(jcstruct_t *s,UINT8 toggle);
GLOBAL(UINT8) jc_read_rgb24(jcstruct_t *s,UINT8 toggle);
GLOBAL(UINT8) jc_read_rgb24_420(jcstruct_t *s,UINT8 toggle);
GLOBAL(UINT8) jc_read_mb(jcstruct_t *s,UINT8 toggle);
/* jcread_asm.s */

/* jcmdma.c */
GLOBAL(void) jc_setup_mdma_400(jcinfo_t *jc_info);
GLOBAL(UINT8) jc_mdma_400(jcstruct_t *s,UINT8 toggle);
GLOBAL(void) jc_setup_mdma_444(jcinfo_t *jc_info);
GLOBAL(UINT8) jc_mdma_444(jcstruct_t *s,UINT8 toggle);
GLOBAL(void) jc_setup_mdma_420(jcinfo_t *jc_info);
GLOBAL(UINT8) jc_mdma_420(jcstruct_t *s,UINT8 toggle);
GLOBAL(void) jc_setup_mdma_422(jcinfo_t *jc_info);
GLOBAL(UINT8) jc_mdma_422(jcstruct_t *s,UINT8 toggle);
GLOBAL(void) jc_setup_mdma_uyvy(jcinfo_t *jc_info);
GLOBAL(UINT8) jc_mdma_uyvy(jcstruct_t *s,UINT8 toggle);
GLOBAL(void) jc_dma_start(dma_descriptor_t *src_ptr,dma_descriptor_t *dst_ptr);
GLOBAL(void) jc_wait_for_dma(void);
/* jcmdma.c */

/* jcwrite_asm.s */
GLOBAL(void) jc_memset(void *buf,INT32 val,UINT32 size);
GLOBAL(void) jc_put_8_bits(jcstruct_t *s,UINT8 val);
GLOBAL(void) jc_put_16_bits(jcstruct_t *s,UINT16 val);
GLOBAL(void) jc_put_32_bits(jcstruct_t *s,UINT32 val);
/* jcwrite_asm.s */

/* jchuffman_asm.s */
GLOBAL(void) jc_empty_huffbuf(jcstruct_t *s);
GLOBAL(UINT8) jc_huffman_seq(jcstruct_t *s,jchuffman_t *htbl,INT16 *block);
GLOBAL(void) jc_finish_huffman(jcstruct_t *s);
/* jchuffman_asm.s */

/* jchuffman.c */
GLOBAL(UINT8) jc_huffman_seq_dummy(jcstruct_t *s,INT16 *block,UINT32 *dc_count,UINT32 *ac_count);
GLOBAL(UINT8) jc_gen_huff_table(jcinfo_t *jc_info,UINT8 table_selector);
/* jchuffman.c */

/* quantization and zig-zag */
/* jcdct_asm.s */
GLOBAL(void) jc_dct(INT16 *temp_buffer,INT16 *jpeg_coeff,INT16 *macro_block);
GLOBAL(void) jc_lshift(UINT8 *buffer,INT16 *temp_buffer,INT16 mcu_cols);
GLOBAL(void) jc_qz(INT16 *temp_buffer,INT16 *macro_block,UINT16 *quantization_table);
/* jcdct_asm.s */

/* jcencode.c */
GLOBAL(UINT8) jc_sequential_dummy(jcinfo_t *jc_info,UINT8 toggle);
GLOBAL(UINT8) jc_sequential(jcinfo_t *jc_info,UINT8 toggle);
/* jcencode.c */

/* jcadvanceptr.c*/
GLOBAL(void) jc_advance_ptr_block_400(jcstruct_t *s,UINT16 mcu_cols);
GLOBAL(void) jc_advance_ptr_row_400(jcstruct_t *s,UINT16 mcu_rows);
GLOBAL(void) jc_advance_ptr_block_444(jcstruct_t *s,UINT16 mcu_cols);
GLOBAL(void) jc_advance_ptr_row_444(jcstruct_t *s,UINT16 mcu_rows);
GLOBAL(void) jc_advance_ptr_block_420(jcstruct_t *s,UINT16 mcu_cols);
GLOBAL(void) jc_advance_ptr_row_420(jcstruct_t *s,UINT16 mcu_rows);
GLOBAL(void) jc_advance_ptr_block_422(jcstruct_t *s,UINT16 mcu_cols);
GLOBAL(void) jc_advance_ptr_row_422(jcstruct_t *s,UINT16 mcu_rows);
GLOBAL(void) jc_advance_ptr_block_uyvy(jcstruct_t *s,UINT16 mcu_cols);
GLOBAL(void) jc_advance_ptr_row_uyvy(jcstruct_t *s,UINT16 mcu_rows);
GLOBAL(void) jc_advance_ptr_block_rgb24(jcstruct_t *s,UINT16 mcu_cols);
GLOBAL(void) jc_advance_ptr_row_rgb24(jcstruct_t *s,UINT16 mcu_rows);
GLOBAL(void) jc_advance_ptr_block_mb(jcstruct_t *s,UINT16 mcu_cols);
GLOBAL(void) jc_advance_ptr_row_mb(jcstruct_t *s,UINT16 mcu_rows);
/* jcadvanceptr.c*/

#endif /* file's endif */
