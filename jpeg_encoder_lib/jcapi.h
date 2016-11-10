#ifndef _JCAPI_H_
#define _JCAPI_H_

#include "jcdefinitions.h"

LOCAL(unsigned char) jc_init_data_buffers(jcinfo_t *jc_info);
LOCAL(void) jc_set_default_quantization_table(jcinfo_t *jc_info);
LOCAL(void) jc_set_default_huffman_table(jcinfo_t *jc_info);

#endif

