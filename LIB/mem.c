#include "stdtypes.h"
#include "strings.h"
#include "error.h"
#include "mem.h"
#include "hdrive.h"

U32		*mem_pool;
int		mem_count,mem_lock_count,MEM_BYTES;

/*********************************************************************
 *********************************************************************/
void	*AllocFile(const char *dir,const char *name)
{
void	*base;
int		size;

	base = GetMemBase();
	size = RdFile(dir,name,BytesFree(),base);
	AllocMem(size);
	return base;
}

/*********************************************************************
 *********************************************************************/
void	*AllocMem(int amount)
{
void	*base;

	if (amount & 7) amount += 8 - (amount & 7);
	base = (void *) &mem_pool[mem_count / 4];
	mem_count += amount;

	if (mem_count >= MEM_BYTES) FatalMsg("AllocMem: failed (%d) bytes",amount);

	return base;
}

/*********************************************************************
 *********************************************************************/
void	*GetMemBase()
{
	return (void *) &mem_pool[mem_count / 4];
}

/*********************************************************************
 *********************************************************************/
int		BytesFree()
{
	return(MEM_BYTES - mem_count);
}

/*********************************************************************
 *********************************************************************/
void	FreeAllMem()
{
	mem_count = 0;
}


/*********************************************************************
 *********************************************************************/
void	FreeUnlockedMem()
{
	mem_count = mem_lock_count;
}


/*********************************************************************
 *********************************************************************/
void	LockMem()
{
	mem_lock_count = mem_count;
}

/*********************************************************************
 *********************************************************************/
void	InitMemHandler()
{
int		top;
extern	U32	bss_end[];

	MEM_BYTES = (U32) &top - (U32) bss_end - 16384;
	mem_pool = bss_end;
}

/*********************************************************************
 *********************************************************************/
void	InitMemHandler2(U32 first_avail_mem, U32 max_stack_bytes)
{
extern	U32	INIT_SP[];

	MEM_BYTES = (U32)&INIT_SP - max_stack_bytes - first_avail_mem;
	mem_pool = (U32 *)first_avail_mem;
	mem_count = 0;
}

