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
**
** $Revision: 2 $ 
** $Date: 12/27/95 7:45p $ 
**
*/

#ifndef _SST1_PCI_H_
#define _SST1_PCI_H_

#ifndef _FXPCI_H_
#include <fxpci.h>
#endif

extern const PciRegister PCI_SST1_INIT_ENABLE; /* 0x40 */
extern const PciRegister PCI_SST1_BUS_SNOOP_0; /* 0x44 */
extern const PciRegister PCI_SST1_BUS_SNOOP_1; /* 0x48 */
extern const PciRegister PCI_SST1_CFG_STATUS;  /* 0x4C */

#endif



