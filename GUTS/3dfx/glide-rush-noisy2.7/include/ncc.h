#ifndef __NCC_H__
#define __NCC_H__

/*
** Copyright (c) 1995, 3Dfx Interactive, Inc.
** All Rights Reserved.
**
** This is UNPUBLISHED PROPRIETARY SOURCE CODE of 3Dfx Interactive, Inc.;
** the contents of this file may not be disclosed to third parties, copied or
** duplicated in any form, in whole or in part, without the prior written
** permission of 3Dfx Interactive, Inc.
**
** RESTRICTED RIGHTS LEGEND:
** Use, duplication or disclosure by the Government is subject to restrictions
** as set forth in subdivision (c)(1)(ii) of the Rights in Technical Data
** and Computer Software clause at DFARS 252.227-7013, and/or in similar or
** successor clauses in the FAR, DOD or NASA FAR Supplement. Unpublished  -
** rights reserved under the Copyright Laws of the United States.
**
** $Revision: 1.1 $
** $Date: 1995/05/18 13:35:48 $
*/

/* translation tables for decompressing NCC format */
typedef struct {
    unsigned char yRGB[16];
    int iRGB[4][3];
    int qRGB[4][3];
} NccTable;

extern void yiq2rgb(NccTable *ncc, unsigned char *c888, unsigned char yiq);
extern void nccYIQtoRGB(unsigned int *array, long length);
extern unsigned char *nccRGBtoYIQ(const int xSize, const int ySize,
				const unsigned int *from, float percent);
#endif	/* !__NCC_H__ */
