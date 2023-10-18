/*
 *	$Id: fsys.h,v 1.26 1997/09/24 18:48:31 shepperd Exp $
 *
 *		Copyright 1996,1997 Atari Games, Corp.
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 */
#ifndef _FSYS_H_
#define _FSYS_H_

#include <sys/types.h>		/* need size_t */
#include <qio.h>		/* need struct qio_ioq */

/*

This file describes the on disk structure for the AGC filesystem.

NOTE: LBA (Logical Block Address) 0 is not used in this filesystem and remains
reserved for future use. LBA of 0, when appearing in retrieval pointers etc.,
is reserved as a flag to indicate "end of list", etc.

There are FSYS_MAX_ALTS copies of what is called the "home block" on the disk.
Each home block is one sector long (512 bytes). The home block contains some
details about the volume (see FsysHomeBlock below) plus some pointers (LBA's)
to the file headers for index.sys. There is also FSYS_MAX_ALTS copies of
index.sys on the disk and there is a pointer to the header of each of those
files in each home block. The Home Blocks reside at known (fixed) LBA's on the
volume which are determined by the definition of the macros FSYS_HB_ALG() for
the home blocks. At volume mount time, all home blocks can be read (if
possible) and verified that they contain identical information.

The contents of the index file can be thought of as a linear array of arrays
(groups) of FSYS_MAX_ALTS LBA's with the first entry in each LBA group
pointing to the file header for a file and each (optional) entry containing an
LBA to a file header for a duplicate copy of the file. The low level file open
routine will try each of the file headers in order until it successfully reads
one. There are three reserved entries in the index file:

Entry 0 in index.sys points to itself (index.sys)
Entry 1 in index.sys points the the free list file
Entry 2 in index.sys points to the root level directory.

Each file on the disk (including index.sys) has an associated file header. A
file header is one sector long (512 bytes) and contains details about the file
(see FsysHeader below). Among those details are an array of "retrieval
pointers" (see FsysRetPtr below). A retrieval pointer contains the physical
starting disk LBA, a number of sectors and a compile time optional repeat and
skip counts. As the filesystem accesses the file, it begins at the first
retrieval pointer and reads n contigious sectors begining at the specified LBA
then optionally skips m contigious sectors and repeats this cycle 'repeat
count' times. Once the first retriveal pointer is exhausted, it continues with
the next until all retrieval pointers have been exhausted. A starting LBA of 0
in a retrieval pointer indicates the end of the list. If the file needs more
retrieval pointers than what will fit in a file header, additional retrieval
pointers will be placed in subsequent extension file header(s). There may be
instances where duplicate file headers contain identical retrieval pointers
(pointing the the same place on disk for the file's data) allowing for errors
in reading the file to be acceptable, but directory errors while opening the
file to be recoverable. Likewise, there may be instances where there will be
different retrieval pointers in each of the duplicate file headers allowing
for recovery of both directory and file read errors.

A directory is a plain file. The contents of a directory is a stream of bytes
consisting of 3 bytes of file ID (FID, which is simply the index into the
index.sys file), 1 byte of "generation number" followed by 1 byte of string
length ("string length" includes the trailing null and a length of 0 means a
length of 256) and a null terminated ASCII string representing the filename.
The "generation number" needs to match the one found in the file header for
the directory entry to be valid. The first entry in a directory contains an
index to the parent directory file with the name "..". The second entry
contains an index to itself and the name ".". 

***************************************************************************/
/**************************************************************************

The following is the list of features that the file system may have. The
definition of FSYS_FEATURES is the composite of all the avilable features
compiled into the code of this system. That is to say, space in data
structures in the file headers, etc. will have been reserved (for features
that require them) for those bits listed in FSYS_FEATURES.

The definition of FSYS_OPTIONS lists which of the features also have code
installed in the system to handle those features.

The purpose of the two separate items is so potentially one could initialise
(and/or use) a disk with a filesystem created on one host (perhaps a PC) and
be able to use it on a system with different filesystem feature list. Maybe
the s/w could cope, then again, maybe not.

 **************************************************************************

SKIP_REPEAT - is an idea where the retrieval pointers contain, in
addition to "start" and "nblocks", a "skip" and a "repeat" parameter.
The "skip" parameter says how many sectors to skip and the "repeat" says
how many times to repeat the "nblocks"/"skip" pair. The idea behind this
scheme is to allow for files to be "interleaved" with one another. This is
a feature that could prove useful if playing a movie and streaming audio,
for example. It complicates the internals of the filesystem quite a bit
to implement this feature, especially with the auto fail-over to alternate
copies of files, etc. For this reason, I provided very little code to support
the feature at this time. Implementation of this feature is left as an exercise
for the reader.
*/

#define FSYS_FEATURES_SKIP_REPEAT	0x0001

/**************************************************************************
All relevant details about a file are contained in the file header, including
the retrieval pointer set. Since the file header is exactly one sector, there
is a potential problem of there not being enough room to hold all the file's
retrieval pointers. To correct for this shortcoming, there is an "extension"
file header that contains mainly additional retrieval pointers. At this time I
did not provide any code to create or read extension file headers. Use of
extension file headers complicates the internals of the filesystem slightly
and I assumed (perhaps incorrectly) that there would be no files in a
production system with more than 20 retrieval pointers (the number that will
fit in the fileheader with the skip/repeat feature turned off). Adding code to
support extension file headers is left as an exercise for the reader.
*/

#define FSYS_FEATURES_EXTENSION_HEADER	0x0002

/**************************************************************************
There may come a time where we will want an "access" and "backup" time
recorded with each file. At the present time, there is no code to support them.
*/

#define FSYS_FEATURES_ABTIME		0x0004

/**************************************************************************
There may come a time where we will want a "modified" time recorded with each
file. The field for this time is always present in the file header, but the
code that updates it may or may not be included in the filesystem.
*/

#define FSYS_FEATURES_CMTIME		0x0008

/**************************************************************************
There may come a time where we will want a list of "permissions" recorded with
each file to allow/disallow access based on some criteria. At the present time
there is no support for this feature.
*/

#define FSYS_FEATURES_PERMS		0x0010

/**************************************************************************
As described above, the following two variables are to be defined to the list
of features and options desired in this filesystem. The default is to include
nothing of substance. The definitions may be overridden by lines in config.mac.
*/

#ifndef FSYS_FEATURES
# define FSYS_FEATURES	(FSYS_FEATURES_CMTIME)
#endif

#ifndef FSYS_OPTIONS
# define FSYS_OPTIONS	(FSYS_FEATURES_CMTIME)
#endif

/**************************************************************************
A slightly faster and much more compact filesystem code can be generated if it
is known in advance that there will be no file creation/deletion, etc. This is
referred to as a READ_ONLY file system, although individual files may be
updated in place as long as they are not extended (attempts to extend a file
on a read-only filesystem will result in an error). Updates to the eof and
time fields of a file header are allowed if the FSYS_UPD_FH variable is set
otherwise no fileheader updates are performed either. The default is to
produce a read only filesystem, with updates to only the eof and time fields
of the file header allowed. Set FSYS_TIGHT_MEM if you need to have a r/w
filesystem with conservative memory usage.
*/

#ifndef FSYS_READ_ONLY
# define FSYS_READ_ONLY 1
#endif

#ifndef FSYS_UPD_FH
# define FSYS_UPD_FH 0
#endif

#ifndef FSYS_TIGHT_MEM
# if FSYS_READ_ONLY
#  define FSYS_TIGHT_MEM 1
# else
#  define FSYS_TIGHT_MEM 0
# endif
#endif

#if !FSYS_READ_ONLY
# undef FSYS_UPD_FH
# define FSYS_UPD_FH 1
#endif

/**************************************************************************
FSYS_MAX_VOLUMES defines how many concurrent volumes will be supported with
this code. The default is 1. There are some additional checks to ensure the
value is within a reasonable range. NUM_HDRIVES is a value set (optionally and
externally) to the maximum number of hard drives that the system supports.
As written, the filessystem supports only one volume per harddrive.
*/

#ifndef NUM_HDRIVES
# define NUM_HDRIVES		1
#endif

#ifndef FSYS_MAX_VOLUMES
# define FSYS_MAX_VOLUMES	(NUM_HDRIVES)	/* number of concurrent volumes supported */
#endif

#if FSYS_MAX_VOLUMES > NUM_HDRIVES
# undef FSYS_MAX_VOLUMES
# define FSYS_MAX_VOLUMES NUM_HDRIVES
#endif

#if FSYS_MAX_VOLUMES <= 0
# error Need to set FSYS_MAX_VOLUMES > 0
#endif

/**************************************************************************
FSYS_MAX_ALTS - defines how many duplicate files are to be supported by this
filesystem. The home blocks, index file, free_list file, file headers and
directory files are always automatically duplicated FSYS_MAX_ALTS times. User
files (ordinary files) are duplicated up to a max of FSYS_MAX_ALTS (selected
by the user as each file is created). Automatic failover to an alternate copy
is performed should there be a read error in one of the other copies. The
default is set to 3.
*/

#ifndef FSYS_MAX_ALTS
# define FSYS_MAX_ALTS	3		/* maximum number of alternate files */
#endif

/**************************************************************************
FSYS_DEFAULT_EXTEND sets the default number of sectors a file is extended.
Writing past the end of file (or more exactly, past the end of allocation)
will cause the file to be extended (an additional retrieval pointer added to
its list of retrieval pointers). Increasing a file's allocation by number of
sectors other than 1 is done for effeciency's sake.
*/

#ifndef FSYS_DEFAULT_EXTEND
# define FSYS_DEFAULT_EXTEND	10	/* default number of sectors to extend file */
#endif

/**************************************************************************
FSYS_DEFAULT_DIR_EXTEND sets the default number of sectors a directory is
extended. Writing past the end of file (or more exactly, past the end of
allocation) will cause the file to be extended (an additional retrieval
pointer added to its list of retrieval pointers). Increasing a file's
allocation by number of sectors other than 1 is done for effeciency's sake.
*/

#ifndef FSYS_DEFAULT_DIR_EXTEND
# define FSYS_DEFAULT_DIR_EXTEND 50	/* default number of sectors to extend directory */
#endif

/**************************************************************************
FSYS_HB_ALG - defines the algorithm with which the filesystem code can use to
find a specific home block. The default is to use the ratio of FSYS_MAX_ALTS
and the maximum LBA on the volume rounded to a sector on a 256 sector boundary
plus 1. 

NOTE: This algorithm also sets the position of the index file headers and
the relative positions of duplicate files.
*/

#ifndef FSYS_HB_ALG			/* Home block location algorithm */
# define FSYS_HB_ALG(idx, max) (((((max)*(idx))/FSYS_MAX_ALTS)&-256)+1)
#endif

#ifndef FSYS_CLUSTER_SIZE
# define FSYS_CLUSTER_SIZE	1	/* blocks per cluster */
#endif

#ifndef FSYS_DIR_HASH_SIZE
# define FSYS_DIR_HASH_SIZE	31	/* number of entries in directory hash table */
    					/* (works best if it is a prime number) */
#endif

#define FSYS_VERSION_HB_MAJOR	1	/* home block major version */
#define FSYS_VERSION_HB_MINOR	2	/* home block minor version */
/* Home block version history:
 *	Versions prior to 1.2 have garbage in the freelist file's copies 2 and 3.
 */
#define FSYS_VERSION_FH_MAJOR	1	/* file header major version */
#define FSYS_VERSION_FH_MINOR	2	/* file header minor version */
/* File header version history:
 *	Versions prior to 1.2 have garbage in the flags field.
 */
#define FSYS_VERSION_EFH_MAJOR	1	/* extension file header major version */
#define FSYS_VERSION_EFH_MINOR	1	/* extension file header minor version */

#if FSYS_REPEAT_SKIP
#define FSYS_VERSION_RP_MAJOR	2	/* retrieval pointer major version */
#else
#define FSYS_VERSION_RP_MAJOR	1	/* retrieval pointer major version */
#endif
#define FSYS_VERSION_RP_MINOR	1	/* retrieval pointer minor version */

#define FSYS_ID_HOME	0xFEEDF00D
#define FSYS_ID_INDEX	0xF00DFACE
#define FSYS_ID_HEADER	0xC0EDBABE
#define FSYS_ID_VOLUME	(('V'<<24) | ('O'<<16) | ('L'<<8) | 'M')

enum filesys {
    FSYS_INDEX_INDEX=0,		/* first after generation # are ptrs to index.sys */
    FSYS_INDEX_FREE,		/* next are ptrs to freemap.sys */
    FSYS_INDEX_ROOT,		/* next are ptrs to root directory */
    FSYS_INDEX_BOOT,		/* next is the boot image */
    FSYS_TYPE_EMPTY=0,		/* file header is unused */
    FSYS_TYPE_INDEX,		/* file is an index file */
    FSYS_TYPE_DIR,		/* file is a directory */
    FSYS_TYPE_LINK,		/* file is a symlink */
    FSYS_TYPE_FILE		/* file is plain file */
};

/* Description of volume home block */

typedef struct home_block {
    unsigned long id;			/* block ID type. s/b FEEDF00D */
    unsigned short hb_minor;		/* home block minor version */
    unsigned short hb_major;		/* home block major version */
    unsigned short hb_size;		/* size in bytes of home block struct */
    unsigned short fh_minor;		/* file header minor version */
    unsigned short fh_major;		/* file header major version */
    unsigned short fh_size;		/* size in bytes of file header struct */
    unsigned short fh_ptrs;		/* number of retrieval pointers in a file header */
    unsigned short efh_minor;		/* extension file header minor version */
    unsigned short efh_major;		/* extension file header major version */
    unsigned short efh_size;		/* size in bytes of extension file header struct */
    unsigned short efh_ptrs;		/* number of retrieval pointers in an extension file header */
    unsigned short rp_minor;		/* retrieval pointer minor version */
    unsigned short rp_major;		/* retrieval pointer major version */
    unsigned short rp_size;		/* size in bytes of retrieval pointer struct */
    unsigned short cluster;		/* blocks per cluster */
    unsigned short maxalts;		/* number of alternates on this volume */
    unsigned long def_extend;		/* default number of clusters to extend files */
    unsigned long ctime;		/* volume creation date/time */
    unsigned long mtime;		/* volume modification date/time */
    unsigned long atime;		/* volume access date/time */
    unsigned long btime;		/* volume backup date/time */
    unsigned long chksum;		/* home block checksum */
    unsigned long features;		/* file system features */
    unsigned long options;		/* file system options */
    unsigned long index[FSYS_MAX_ALTS]; /* up to n ptrs to index.sys files */
} FsysHomeBlock;

/* Description of retrieval pointer */

typedef struct file_retptr {
    unsigned long start;		/* starting lba */
    unsigned long nblocks;		/* number of contigious clusters */
# if FSYS_REPEAT_SKIP
    unsigned long repeat;		/* number of times to repeat the nblocks/skip pair */
    unsigned long skip;			/* number of clusters to skip */
# endif
} FsysRetPtr;

/* Description of file header */

typedef struct file_header {
    unsigned long id;			/* file header type */
    unsigned long size;			/* file size in bytes */
    unsigned long clusters;		/* number of clusters allocated for this file */
    unsigned char generation;		/* file's generation number */
    unsigned char type;			/* file type (see above for types) */
    unsigned short flags;		/* spare (to fill out to longword) */
#if (FSYS_FEATURES&FSYS_FEATURES_EXTENSION_HEADER)
    unsigned long extension;		/* index to file header extension */
# define FSYS_FHEADER_EXT	1
#else
# define FSYS_FHEADER_EXT	0
#endif
#if (FSYS_FEATURES&FSYS_FEATURES_CMTIME)
    unsigned long ctime;		/* file creation time */
    unsigned long mtime;		/* file modification time */
# define FSYS_FHEADER_CMTIME	2
#else
# define FSYS_FHEADER_CMTIME	0
#endif
#if (FSYS_FEATURES&FSYS_FEATURES_ABTIME)
    unsigned long atime;		/* file access time */
    unsigned long btime;		/* file backup time */
# define FSYS_FHEADER_ABTIME	2
#else
# define FSYS_FHEADER_ABTIME	0
#endif
#if (FSYS_FEATURES&FSYS_FEATURES_PERMS)
    unsigned long owner;		/* file owner (for future use) */
    unsigned long perms;		/* file permissions (for future use) */
# define FSYS_FHEADER_PERMS	2
#else
# define FSYS_FHEADER_PERMS	0
#endif
#define FSYS_FHEADER_MEMBS (4+FSYS_FHEADER_EXT+FSYS_FHEADER_CMTIME+FSYS_FHEADER_ABTIME+FSYS_FHEADER_PERMS)
/*
 * Set this to the maximum number of retrieval pointers in a file header. It is set
 * such that the sizeof(struct file_header) is <= 512. The FSYS_FHEADER_MEMBS in the
 * expression below is the number of long's appearing ahead of this member.
 */
#define FSYS_MAX_FHPTRS	(((512-sizeof(long)*FSYS_FHEADER_MEMBS)/sizeof(FsysRetPtr))/FSYS_MAX_ALTS)
/*
 */
    FsysRetPtr pointers[FSYS_MAX_ALTS][FSYS_MAX_FHPTRS]; /* retrieval pointers */
} FsysHeader;

#if (FSYS_FEATURES&FSYS_FEATURES_EXTENSION_HEADER)
/* Description of extension file header */

typedef struct efile_header {
    unsigned long id;			/* file header type */
    unsigned long head;			/* pointer to first header in list */
    unsigned long extension;		/* ptr to next file header extension */
/*
 * Set this to the maximum number of retrieval pointers in an extension file header.
 * It is set such that the sizeof(struct efile_header) is <= 512. The '3' in the
 * expression below is the number of long's appearing ahead of this member.
 */
#define FSYS_MAX_EFHPTRS	(((512-sizeof(long)*3)/sizeof(FsysRetPtr))/FSYS_MAX_ALTS)
/*
 */
    FsysRetPtr pointers[FSYS_MAX_ALTS][FSYS_MAX_EFHPTRS]; /* retrieval pointers */
} FsysEHeader;

#else
# define FSYS_MAX_EFHPTRS	0
#endif

typedef struct ram_rp {
# if !FSYS_READ_ONLY
    struct ram_rp *next;		/* pointer to next block of these */
# endif
    FsysRetPtr *rptrs;			/* pointer to array of retrieval pointers */
    unsigned int rptrs_size:12;		/* size of array of pointers */
    unsigned int num_rptrs:12;		/* number of active pointers in array */
    unsigned int mallocd:1;		/* rptrs is an individually malloc'd area */
    unsigned int filler:7;		/* pad it to a longword boundary */
} FsysRamRP;

#define RAMFH_VALID		1	/* fh is valid */
#define RAMFH_NEW		2	/* fh is new */
#define RAMFH_UPD		4	/* add to dirty at close */

typedef struct ram_fileheader {		/* RAM based file header */
    unsigned long clusters;		/* file allocation in sectors */
    unsigned long size;			/* size of file in bytes */
# if (FSYS_FEATURES&FSYS_OPTIONS&FSYS_FEATURES_CMTIME)
    unsigned long ctime;		/* creation time of this file */
    unsigned long mtime;		/* modification time on this file */
# endif
# if (FSYS_FEATURES&FSYS_OPTIONS&FSYS_FEATURES_ABTIME)
    unsigned long atime;
    unsigned long btime;
# endif
# if (FSYS_FEATURES&FSYS_OPTIONS&FSYS_OPTIONS_PERMS)
    unsigned long perms;		/* file access permissions */
    unsigned long owner;		/* who owns this file */
# endif
    struct fsys_dirent **directory;	/* pointer to directory hash table if this is a directory */
    int active_rp;			/* which of the following ramrp's is currently in use */
    FsysRamRP ramrp[FSYS_MAX_ALTS];	/* retrieval pointers */
    unsigned char generation;		/* generation number on this file */
    unsigned char valid;		/* signals rfh is valid */
    unsigned short def_extend;		/* amount to extend file by default */
} FsysRamFH;

/* Description of filesystem I/O argument block */

typedef struct fsys_qio {
    QioIOQ our_ioq;			/* !!!This must be the first member!!! for fsys I/O we use a different IOQ */
    struct act_q astq;			/* place to use to queue up operation */
    FsysRamRP *ramrp;			/* pointer to list of retrieval pointers */
    int state;				/* read state */
    unsigned long sector;		/* user's relative sector number */
    unsigned char *buff;		/* pointer to user's buffer */
    int total;				/* total bytes xferred */
    int count;				/* number of sectors to read/write */
    int u_len;				/* user's byte count */
    unsigned char *o_buff;		/* original buffer pointer */
    int o_len;				/* original length */
    unsigned long o_where;		/* original relative sector number */
    int o_which;			/* which of the ALTS we are writing */
    int o_iostatus;			/* remember the last error, if any */
    void (*compl)(struct qio_ioq *);	/* pointer to completion routine */
    QioFile *fsys_fp;			/* pointer to file sys's QioFile */
    QioIOQ *callers_ioq;		/* remember ptr to caller's IOQ */
} FsysQio;

typedef struct fsys_dirent {
    struct fsys_dirent *next;		/* ptr to next entry */
    const char *name;			/* ptr to null terminated name of file */
    int fid:24;				/* index into index.sys file to file header */
    int generation:8;
} FsysDirEnt;

#ifndef FSYS_SYNC_ERRLOG
# define FSYS_SYNC_ERRLOG		(20)	/* assume a log of 20 entries */
#endif
#ifndef FSYS_SYNC_TIMER
# define FSYS_SYNC_TIMER		(500000) /* sync automatically runs every 500000 microseconds */
#endif
#ifndef FSYS_SYNC_BUFFSIZE
# if FSYS_TIGHT_MEM
#  define FSYS_SYNC_BUFFSIZE		(512)
# else
#  define FSYS_SYNC_BUFFSIZE		(4096)
# endif
#endif
#define FSYS_SYNC_BUSY_NONTIMER		(1)
#define FSYS_SYNC_BUSY_TIMER		(2)
#define FSYS_SYNC_UPD_INDEX		(1)
#define FSYS_SYNC_UPD_FREE		(2)

typedef struct fsys_sync {
    FsysQio our_ioq;			/* !!!MUST BE FIRST MEMBER!!! an IOQ for sync to use */
    struct fsys_volume *vol;		/* ptr to volume */
    struct tq sync_t;			/* entry for sync timer function */
    int status;				/* return status from last sync */
    unsigned long errcnt;		/* total number of errors encountered during syncs */
    unsigned long errlog[FSYS_SYNC_ERRLOG]; /* error history log */
    int err_in;				/* index into errlog to place next entry */
    FsysRamRP ramrp;			/* fake ram retrieval pointer */
    FsysRetPtr rptr;			/* fake retrieval pointer used to r/w the file header */
    int buffer_size;			/* size of buffers in bytes */
    unsigned long *buffers;		/* virtual mem ptr to following buffer */
    unsigned long *output;		/* adjusted non-cached pointer to output buffer */
    int state;				/* current sync state */
    int substate;			/* sync state's substate */
    int alts;				/* which of the alternate files we are reading/writing */
    int busy;				/* flag indicating sync is currently executing and how is was started */
    int sects;				/* saved sector count */
    int start;				/* saved starting sector */
} FsysSyncT;

/* Description of mounted volume */

typedef struct fsys_volume {
    FsysQio reader;			/* !!!This absolutely must be the first member !!! */
    unsigned long id;			/* distinguishing marker for this struct */
    QioMutex mutex;			/* I/O mutex for this volume */
    int iofd;				/* FD to use to do I/O */
    volatile int status;		/* volume mount status */
    volatile int state;			/* volume mount state */
    int substate;			/* mount substate */
    unsigned long maxlba;		/* max lba for this volume (logical limit) */
    unsigned long maxblocks;		/* max lba for this disk (physical limit) */
    unsigned long total_free_clusters;	/* number of available clusters */
    unsigned long total_alloc_clusters;	/* number of used clusters */
    unsigned long index_lbas[FSYS_MAX_ALTS]; /* lbas to index.sys file headers */
    unsigned long *index;		/* index.sys file contents */
    FsysRamFH *files;			/* ptr to ram copy of file headers */
#if FSYS_TIGHT_MEM
    FsysRetPtr *rp_pool;		/* ptr to preallocated pool of retrieval pointers */
    int rp_pool_size;			/* number of entries in remaining in pool */
#endif
    int files_ffree;			/* first free element in files */
    int files_elems;			/* number of elements in files */
    FsysRetPtr *free;			/* freemap.sys file contents */
    int free_ffree;			/* first free element in freemap */
    int free_elems;			/* size of freemap in elements */
# if !FSYS_READ_ONLY
    unsigned long *index_bits;		/* pointer to array of bits, one bit per sector in index */
    int index_bits_elems;		/* number of elements in index_bits */
    int free_start;			/* first element in freelist that has been updated */
    unsigned long *unused;		/* ptr to list of unused (deleted) fid's */
    int unused_ffree;			/* first free element in unused */
    int unused_elems;			/* number of elements in unused */
# endif
# if FSYS_UPD_FH
    FsysSyncT sync_work;		/* work area for sync code */
    unsigned long *dirty;		/* list of FID's to update */
    int dirty_ffree;			/* first free entry in dirty */
    int dirty_elems;			/* number of entries in dirty */
# endif
    int files_indx;			/* current 'files' index (tmp used by mount) */
    int rw_amt;				/* amt to r/w (tmp used by mount) */
    unsigned long *contents;		/* pointer to where to read/write (tmp used by mount) */
    unsigned long *buff;		/* pointer to non-cacned sector buffer (tmp used by mount) */
    int buff_size;			/* size of buffer in bytes (tmp used by mount) */
    FsysRamRP tmpramrp;			/* retrieval pointer (tmp used by mount) */
    FsysRetPtr tmprp;			/* retrieval pointer (tmp used by mount) */
# if FSYS_UMOUNT
    void **freemem;			/* record of malloc's (tmp used by mount) */
    int freem_elems;			/* number of items in freemem (tmp used by mount) */
    int freem_indx;			/* next available item in freemem (tmp used by mount) */
# endif
    struct act_q tmpq;			/* place to use with prc_q_ast (tmp used by mount) */
} FsysVolume;

/* Description of volume init parameters */

typedef struct fsys_initv {
    int cluster;		/* number of sectors per cluster */
    int index_sectors;		/* number of sectors for initial index file allocation */
    int free_sectors;		/* number of sectors for initial freemap file allocation */
    int root_sectors;		/* number of sectors for initial root directory allocation */
    int def_extend;		/* number of sectors to extend files by default */
} FsysInitVol;

typedef struct fsys_opent {
    QioOpenSpc spc;		/* This must be first (note: not a pointer) */
    int fid;			/* FID of file to open (set to FID of open'ed file) */
    int parent;			/* FID of parent */
    size_t alloc;		/* file allocation size in bytes */
    size_t eof;			/* position of eof marker in bytes */
    int placement;		/* which area of disk to place file */
    int copies;			/* number of copies of file to make */
    int def_extend;		/* amount to extend file by default */
    int mkdir;			/* .ne. if to create a directory */
    unsigned long ctime;	/* time to assign for creation */
    unsigned long mtime;	/* time to assign for modification */
    unsigned long btime;	/* time to assign for backup */
    unsigned long atime;	/* time to assign for access */
} FsysOpenT;

/* Additional flags that can be set in the mode field and that reside in the */
/* mode field of the QioFile struct */

#if FSYS_MAX_ALTS > 3
# error ** Need to fix definition of FSYS_OPNCPY_x
#endif
#define FSYS_OPNCPY_V	30			/* bit position for number */
#define FSYS_OPNCPY_M	(3<<FSYS_OPNCPY_V)	/* mask for copy numbers (0=no copy) */
#define O_OPNCPY	(1<<FSYS_OPNCPY_V)	/* open only the copy spec'd in FsysOpenT.copies */

typedef struct lookupfile_t {
    FsysVolume *vol;	/* volume on which to look for file */
    FsysRamFH *top;	/* directory in which to start the search */
    const char *path;	/* pointer to path/filename */
    const char *fname;	/* filename only portion of above */
    FsysDirEnt *dir;	/* pointer to directory entry where file found */
    FsysRamFH *file;	/* pointer to found file */
    FsysRamFH *owner;	/* pointer to directory that owns the file */
    int depth;		/* number of directories traversed to reach this file */
} FsysLookUpFileT;

typedef struct dirent {
    int filler;
} FsysFDirEnt;

typedef struct statfs {
    int filler;
} FsysStatFS;

struct direct {
    const char *name;
    unsigned long fid;
};

typedef struct dir {
    QioIOQ ioq;			/* place to allow us to manage these entries */
    FsysVolume *vol;
    const char *dirname;	/* filename of directory */
    FsysDirEnt **hash;		/* pointer to hash table */
    FsysDirEnt *current;	/* pointer to next entry to read */
    int entry;			/* hash table entry currently processing */
    int state;			/* internal state */
} DIR;

typedef struct fsys_fhioctl {
    int fields;			/* bit mask indicating which of the following are valid */
    U32 ctime;			/* creation time */
    U32 mtime;			/* modified time */
    U32 atime;			/* access time */
    U32 btime;			/* backup time */
    size_t alloc;		/* file allocation in bytes */
    size_t size;		/* file size in bytes */
    size_t pos;			/* current file pointer in bytes */
    size_t def_extend;		/* default file extension in bytes */
    int copies;			/* number of copies maintained */
    int fid;			/* FID of file */
    int dir;			/* .ne. if file is directory */
} FsysFHIoctl;

#define FSYS_FHFIELDS_CTIME	0x01	/* FsysFHIoctl.fields bits */
#define FSYS_FHFIELDS_MTIME	0x02
#define FSYS_FHFIELDS_ATIME	0x04
#define FSYS_FHFIELDS_BTIME	0x08
#define FSYS_FHFIELDS_ALLOC	0x10
#define FSYS_FHFIELDS_SIZE	0x20
#define FSYS_FHFIELDS_DEFEXT	0x40
#define FSYS_FHFIELDS_COPIES	0x80

#define FSYS_IOC_GETFH	1	/* get file header contents */
#define FSYS_IOC_SETFH	2	/* set (some) file header contents */

/************************************************************
 * fsys_qmount - a volume mount primitive. This function will 
 * perform the necessary steps to mount a volume and load the
 * index, freelist and root directory into cache. The function
 * is expected to operate as an AST (action) routine and therefore
 * will complete asychronously from the calling program.
 * This function is used internally by the filesystem and not
 * expected to be used by the casual user.
 * 
 * At entry:
 *	vol - pointer to volume argument list. The list must be in
 *		static memory (i.e., not on the stack)
 * At exit:
 *	returns nothing. The function queue's input which will
 *	complete asynchronously from the calling program.
 */
extern void fsys_qmount(struct qio_ioq *vol);

/************************************************************
 * fsys_mountw - volume mount. This function queue's the volume
 * mount primitive and waits for its completion.
 * 
 * At entry:
 *	where - null terminated string with name of physical device 
 *	what - null terminated string with name of virtual device
 *
 * At exit:
 *	returns 0 if success or one of FSYS_MOUNT_xxx if error.
 */
extern int fsys_mountw(const char *where, const char *what);

/************************************************************
 * fsys_initfs - initialize volume. This function writes all the
 * necessary sectors to disk to initialize a volume for use by the
 * filesystem.
 * 
 * At entry:
 *	what - null terminated string naming physcal device to write
 *	v - pointer to struct with init params:
 *		->cluster - must be 1 at this time
 *		->index_sectors - # of sectors to allocate to index file
 *		->free_sectors - # of sectors to allocate to freelist file
 *		->root_sectors - # of sectors to allocate to root directory
 *
 * At exit:
 *	returns 0 if success or one of FSYS_INIT_xxx if error.
 */
extern int fsys_initfs(const char *what, FsysInitVol *v);

/************************************************************
 * fsys_init - initialize filesystem internals. This function must
 * be called once at boot.
 * 
 * At entry:
 *	no requirements.
 *
 * At exit:
 *	returns nothing.
 */
extern void fsys_init(void);

/*********************************************************************
 * fsys_sync_delay - set period at which the sync task runs.
 *
 * At entry:
 *	time - number of microseconds between runs of Sync task
 *
 * At exit:
 *	returns previous value of sync delay. Requeues the sync
 *	task at its new rate. Also forces a sync on each volume
 *	in the system. If the parameter 'time' is 0, the delay 
 *	is set to the value assigned to the cpp variable
 *	FSYS_SYNC_TIMER.
 */

extern int fsys_sync_delay(int time);

#endif		/* _FSYS_H_ */
