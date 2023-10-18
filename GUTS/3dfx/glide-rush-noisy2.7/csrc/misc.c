#include <3dfx.h>
#include <glidesys.h>
#include <fxpci.h>
#include <glide.h>
#include <fxglide.h>

extern char *getenv(const char *);

/* PUBLIC DATA  */

const PciRegister PCI_VENDOR_ID       = { 0x0,  2, READ_ONLY };
const PciRegister PCI_DEVICE_ID       = { 0x2,  2, READ_ONLY };
const PciRegister PCI_COMMAND         = { 0x4,  2, READ_WRITE };
const PciRegister PCI_STATUS          = { 0x6,  2, READ_WRITE };
const PciRegister PCI_REVISION_ID     = { 0x8,  1, READ_ONLY };
const PciRegister PCI_CLASS_CODE      = { 0x9,  3, READ_ONLY };
const PciRegister PCI_CACHE_LINE_SIZE = { 0xC,  1, READ_WRITE };
const PciRegister PCI_LATENCY_TIMER   = { 0xD,  1, READ_WRITE };
const PciRegister PCI_HEADER_TYPE     = { 0xE,  1, READ_ONLY };
const PciRegister PCI_BIST            = { 0xF,  1, READ_WRITE };
const PciRegister PCI_BASE_ADDRESS_0  = { 0x10, 4, READ_WRITE };
const PciRegister PCI_BASE_ADDRESS_1  = { 0x14, 4, READ_WRITE };
const PciRegister PCI_BASE_ADDRESS_2  = { 0x18, 4, READ_WRITE };
const PciRegister PCI_BASE_ADDRESS_3  = { 0x1C, 4, READ_WRITE };
const PciRegister PCI_BASE_ADDRESS_4  = { 0x20, 4, READ_WRITE };
const PciRegister PCI_BASE_ADDRESS_5  = { 0x24, 4, READ_WRITE };
    /* 0x28->3B - Reserved */
const PciRegister PCI_INTERRUPT_LINE  = { 0x3C, 1, READ_WRITE };
const PciRegister PCI_INTERRUPT_PIN   = { 0x3D, 1, READ_ONLY };
const PciRegister PCI_MIN_GNT         = { 0x3E, 1, READ_ONLY };
const PciRegister PCI_MAX_LAT         = { 0x3F, 1, READ_ONLY };

static FxBool         libraryInitialized;
char *pciErrorString;

unsigned long sst_memfifo_min;

extern int put_pci_config_reg(int devnum, unsigned long addr, int val);
extern int get_pci_config_reg(int devnum, unsigned long addr);

FxBool pciGetConfigData( PciRegister reg, FxU32 device_number, FxU32 *data ) {

    if ( reg.rwFlag == WRITE_ONLY ) {
	pciErrorString = "Cannot read a WRITE_ONLY register.\n";
	return FXFALSE;
    }

    if (data) {
	*data = get_pci_config_reg(device_number, (reg.regAddress>>2));
	if (reg.sizeInBytes <= 1) *data &= 0xFF;
	else if (reg.sizeInBytes <= 2) *data &= 0xFFFF;
    }
    return FXTRUE;
}

FxBool pciSetConfigData( PciRegister reg, FxU32 device_number, FxU32 *data ) {

    if ( reg.rwFlag == READ_ONLY ) {
	pciErrorString = "Cannot write to a READ_ONLY register.\n";
	return FXFALSE;
    }

    put_pci_config_reg(device_number,  (reg.regAddress>>2), *data);

    return FXTRUE;
}

/*
** sst1InitGetenv():
**  Getenv() for INIT routines.
**
**  Returns the value from getenv().
**
*/
char *sst1InitGetenv(char *string)
{
    return getenv(string);
}

#if !NOISY && !NO_INIT_PRINTF
void INIT_PRINTF(const char *ctl, ...) {
    return;
}

void INIT_INFO(const char *ctl, ...) {
    return;
}
#endif

FxBool pciClose( void ) {
    libraryInitialized = FXFALSE;
    return FXTRUE;
}

/* return current time in seconds (floating point)	*/
float fxTime(void)
{
    return 0.0;
}

/* returns elapsed time in seconds */
float timer(int flag)
{
    static float starttime,endtime;

    if (flag == 0)
	starttime = endtime = fxTime();
    else
	endtime = fxTime();
    return endtime - starttime;
}

/* lcl_strupr
**
** Summary: UNPUBLISHED - upper case the provided string in place.
**          Provided because strupr isn't provided in all clib implementations.
*/
void lcl_strupr( char *str )
{
    FxU32 index;
    for ( index = 0; index < strlen( str ); index++ )
    {
		if ( str[index] >= 'a' && str[index] <= 'z' )
		{
	    	str[index] -= 'a' - 'A';
		}
    }
}

void agcBufferSwap(int interval) {
#ifdef GETGC
  GETGC;
#endif
  grSstStatus();
  grSstWriteConfigAll( SSTR_SWAPBUFFERCMD, interval);
  grSstStatus();
}
