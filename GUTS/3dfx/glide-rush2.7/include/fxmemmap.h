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
** $Revision: 2 $
** $Date: 6/19/96 4:20p $
*/

/* DeviceIoControl service table control codes for communication with */
/* fxmemmap vxd. The codes here must match those in fxmemmap.asm. */

#define   DIOC_CLOSEHANDLE     -1     
#define   DIOC_GETVERSIONDWORD  0   
#define   GETAPPVERSIONDWORD    1   
#define   GETLINEARADDR         2   
#define   GETLDTSELECTOR        3
#define   FREELDTSELECTOR       4
#define   GETLINEARADDR_AUTO    5	/* Watches for & handles PASSTHRU, */
					/* Otherwise identical to 2 */
