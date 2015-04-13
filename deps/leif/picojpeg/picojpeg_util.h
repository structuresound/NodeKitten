//
//  picojpeg_util.h
//  NodeKittenX
//
//  Created by Leif Shackelford on 2/22/15.
//  Copyright (c) 2015 structuresound. All rights reserved.
//

#ifndef __NodeKittenX__picojpeg_util__
#define __NodeKittenX__picojpeg_util__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "picojpeg.h"

unsigned char pjpeg_need_bytes_callback(unsigned char* pBuf, unsigned char buf_size, unsigned char *pBytes_actually_read, void *pCallback_data);

uint8_t *pjpeg_load_from_file(const char *pFilename, unsigned *x, unsigned *y, int *comps, pjpeg_scan_type_t *pScan_type, int reduce);

#endif /* defined(__NodeKittenX__picojpeg_util__) */
