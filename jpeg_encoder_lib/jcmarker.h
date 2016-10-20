#ifndef _JCMARKER_H_
#define _JCMARKER_H_

#include "jcdefinitions.h"

#define		SOF0_MARKER		0xC0
#define		SOF1_MARKER		0xC1
#define		SOF2_MARKER		0xC2
#define		SOF3_MARKER		0xC3

#define		SOF5_MARKER		0xC5
#define		SOF6_MARKER		0xC6
#define		SOF7_MARKER		0xC7

#define		JPG_MARKER		0xC8
#define		SOF9_MARKER		0xC9
#define		SOF10_MARKER	0xCA
#define		SOF11_MARKER	0xCB

#define		SOF13_MARKER	0xCD
#define		SOF14_MARKER	0xCE
#define		SOF15_MARKER	0xCF

#define		DHT_MARKER		0xC4

#define		DAC_MARKER		0xCC

#define		RST0_MARKER		0xD0
#define		RST1_MARKER		0xD1
#define		RST2_MARKER		0xD2
#define		RST3_MARKER		0xD3
#define		RST4_MARKER		0xD4
#define		RST5_MARKER		0xD5
#define		RST6_MARKER		0xD6
#define		RST7_MARKER		0xD7

#define		SOI_MARKER		0xD8
#define		EOI_MARKER		0xD9
#define		SOS_MARKER		0xDA
#define		DQT_MARKER		0xDB
#define		DNL_MARKER		0xDC
#define		DRI_MARKER		0xDD
#define		DHP_MARKER		0xDE
#define		EXP_MARKER		0xDF

#define		APP0_MARKER		0xE0
#define		APP1_MARKER		0xE1
#define		APP2_MARKER		0xE2
#define		APP3_MARKER		0xE3
#define		APP4_MARKER		0xE4
#define		APP5_MARKER		0xE5
#define		APP6_MARKER		0xE6
#define		APP7_MARKER		0xE7
#define		APP8_MARKER		0xE8
#define		APP9_MARKER		0xE9
#define		APP10_MARKER	0xEA
#define		APP11_MARKER	0xEB
#define		APP12_MARKER	0xEC
#define		APP13_MARKER	0xED
#define		APP14_MARKER	0xEE
#define		APP15_MARKER	0xEF

#define		JPG0_MARKER		0xF0
#define		JPG1_MARKER		0xF1
#define		JPG2_MARKER		0xF2
#define		JPG3_MARKER		0xF3
#define		JPG4_MARKER		0xF4
#define		JPG5_MARKER		0xF5
#define		JPG6_MARKER		0xF6
#define		JPG7_MARKER		0xF7
#define		JPG8_MARKER		0xF8
#define		JPG9_MARKER		0xF9
#define		JPG10_MARKER	0xFA
#define		JPG11_MARKER	0xFB
#define		JPG12_MARKER	0xFC
#define		JPG13_MARKER	0xFD

#define		COM_MARKER		0xFE

#define		TEM_MARKER		0x01

LOCAL(void) jc_put_next_marker(jcstruct_t *s,UINT8 val);
LOCAL(UINT8) jc_put_dqt_marker(jcinfo_t *jc_info);
LOCAL(UINT8) jc_put_dht_marker(jcinfo_t *jc_info);
LOCAL(UINT8) jc_put_sof_marker(jcinfo_t *jc_info);
LOCAL(UINT8) jc_put_sos_marker(jcinfo_t *jc_info);

#endif
