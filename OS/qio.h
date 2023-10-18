/*
 * $Id: qio.h,v 1.32 1997/10/04 20:35:23 shepperd Exp $
 *
 *		Copyright 1996 Atari Games, Corp.
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 */
#if !defined(_QIO_H_) && !defined(QIO_ERR_MSG) && !defined(QIO_FACILITY_MSG)
#define _QIO_H_

#ifndef _OS_PROTO_H_
# error You need to #include "os_proto.h" before including qio.h
#endif
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#if HOST_BOARD
#include <reent.h>
#endif

#ifndef QIO_MAX_FILES
# define QIO_MAX_FILES		64	/* set to the maximum number of files open at once */
#endif

#ifndef QIO_MAX_DEVICES
# define QIO_MAX_DEVICES	8	/* set to the maximum number of devices in system */
#endif

#ifndef QIO_ASTLVL
# define QIO_ASTLVL		0	/* File system and QIO AST's are always the highest priority */
#endif

#ifndef QIO_CACHE_LINE_SIZE
# define QIO_CACHE_LINE_SIZE	32
#endif

#ifndef QIO_ALIGN
# define QIO_ALIGN(x, amt) (((U32)(x)+((amt)-1))&-(amt))
#endif

#ifndef QIO_MK_NONCACHE
# if HOST_BOARD
#  define QIO_MK_NONCACHE(x) ((U32)(x)|0x20000000)
# else
#  define QIO_MK_NONCACHE(x) ((U32)(x))
# endif
#endif

#ifndef QIO_PHYS
# define QIO_PHYS(x) ((U32)(x)&0x1FFFFFFF)
#endif

/********************************************************************************
QIO_FNAME_SEPARATOR - defines the character used to delimit device and directory
names from filenames. The default is '/'.
*/

#ifndef QIO_FNAME_SEPARATOR
# define QIO_FNAME_SEPARATOR	'/'
#endif

/**************************************************************************
The QIO subsystem uses a re-enterant malloc, et al. to get memory to hold its
data structures, buffers, etc. (Look in mallocr.c for these functions). These
functions allocate memory from a private pool reserved explicitly to the
subsystem. A single memory pool is used for all QIO operations and the
filesystem(s) claim memory from this pool as well. QIO_POOL_SIZE sets the size
(in bytes) of this pool. The default is 256k allowing for approximately 1000
files in a single filesystem volume. A writeable filesystem takes much more
memory than a read only system unless FSYS_TIGHT_MEM is set non-zero. Setting
FSYS_TIGHT_MEM to non-zero makes the filesystem take roughly 15% more memory
than a read only system.

NOTE: on a READ_ONLY filesystem, once the volume has been mounted, no further
malloc's are performed. One may check the residual value of of the static
variable 'free_size' in fsys.c after all mounts have completed and adjust the
value assigned to QIO_POOL_SIZE to retrieve some unused memory if desired.

*/

#ifndef QIO_POOL_SIZE
# define QIO_POOL_SIZE (256*1024)	/* amount of memory filesystem can use */
#endif

#if QIO_LOCAL_DEFINES
#  ifndef n_elts
#   define n_elts(x) (sizeof(x)/sizeof((x)[0]))
#  endif
# if HOST_BOARD
#  include <reent.h>
extern struct _reent *qio_reent;
#  include <stdlib.h>
#  if MALLOC_DEBUG
extern void *guts_malloc_r( struct _reent *rep, int amt, const char *file, int lineno );
extern void *guts_realloc_r( struct _reent *rep, void *old, int amt, const char *file, int lineno );
extern void *guts_calloc_r( struct _reent *rep, int amt, int size, const char *file, int lineno);
extern void guts_free_r( struct _reent *rep, void *old, const char *file, int lineno);
#   define QIOmalloc(x) guts_malloc_r((void *)qio_reent, x, __FILE__, __LINE__)
#   define QIOrealloc(x, y) guts_realloc_r((void *)qio_reent, x, y, __FILE__, __LINE__)
#   define QIOcalloc(x, y) guts_calloc_r((void *)qio_reent, x, y, __FILE__, __LINE__)
#   define QIOfree(x) guts_free_r((void *)qio_reent, x, __FILE__, __LINE__)
#  else
#   define QIOmalloc(x) _malloc_r((void *)qio_reent, x)
#   define QIOrealloc(x, y) _realloc_r((void *)qio_reent, x, y)
#   define QIOcalloc(x, y) _calloc_r((void *)qio_reent, x, y)
#   define QIOfree(x) _free_r((void *)qio_reent, x)
#  endif
# else
#  define QIOmalloc(x) malloc(x)
#  define QIOrealloc(x, y) realloc(x, y)
#  define QIOcalloc(x, y) calloc(x, y)
#  define QIOfree(x) free(x)
# endif
#endif		/* QIO_LOCAL_DEFINES */

/*
 * The following is the definition of the QioIOQ struct, a pointer to which is
 * required by nearly all qio functions. 
 */

typedef struct qio_ioq {
/*
The following 3 members are required by the list manager (qio_getioq() and
qio_freeioq()). If the QioIOQ was obtained via a qio_getioq(), these members
MUST NOT be altered, cloned to another QioIOQ, etc. If the QioIOQ is not
managed by the qio list manager (qio_getioq() and qio_freeioq(), i.e.
allocated from bss, stack or by a user provided list manager) the contents of
these 3 fields is not significant.

* WARNING * DANGER * WARNING * DANGER * WARNING * DANGER * WARNING * DANGER *
THESE 3 MEMBERS MUST APPEAR FIRST IN THE STRUCTURE. DO NOT MOVE
THEM AND DO NOT CHANGE THE ORDER OF THESE MEMBERS.

*/
    struct qio_ioq **owner;	/* pointer to pointer of head of freelist */
    struct qio_ioq **head;	/* pointer to pointer of head of list to which this element belongs */
    struct qio_ioq *next;	/* next guy in the list */

/* The 'file' member holds the file descriptor of the file to be processed
 * by the qio. This field is a input to all qio functions except qio_open
 * which uses it as an output (qio_open sets this field to the FD of the newly
 * opened file). Except for qio_open, no qio function alters this field.
 */
    int file;			/* fd of file to access */

/* The 'timeout' member holds the I/O timeout amount in microseconds. The
 * underlying subsystem may not provide timeouts as resolute as microseconds
 * (Phoenix class systems will only timeout to the nearest millisecond, for
 * example), so value set here is only an approximate timeout.
 * This field is only an input and no qio functions will alter it.
 * Set this field to 0 if no timeout is to be used.
 */
    int timeout;		/* I/O timeout amount in microseconds */
/*
 * The following two members are set by the qio function to the status
 * and I/O count respectively (or other values as defined by the specific
 * qio function). These members are output only (set by the qio function)
 * so their contents at entry to a qio call is not relevant unless
 * specified otherwise by the specific qio function.
 */
    volatile size_t iostatus;	/* I/O completion status */
    volatile size_t iocount;	/* I/O xfer count or FD after qio_open */

/* The 'complete' member holds a pointer to a user completion routine.
 * This member is only an input and no qio function will alter this member.
 * The contents of this member must always be a valid 'pointer to function'
 * or 0 (NULL) if no completion routine is to be called.
 * The completion routine (if provided) will be called with a single
 * parameter which is a pointer to this struct, the same pointer as used
 * in the qio function call.
 */
    void (*complete)(struct qio_ioq *);	/* user's completion routine */
/*
 * The following two members may be used by the user for any purposes.
 * qio functions neither reference nor modify the contents of these 2 members.
 */
    void *user;			/* user's field to do whatever s/he wants */
    void *user2;		/* user's field to do whatever s/he wants */
/*
 * The remaining members are reserved for use by the qio code itself. The
 * contents of these members at entry to a qio function is not significant
 * unless specified otherwise by specific qio functions (i.e. private is used
 * by qio_opendir, qio_readdir, etc).
 * The contents of these members at entry to the user completion routine is
 * not specified, but the pparamx and iparamx members will probably contain
 * the parameters passed to the qio function (pointers go into pparamx,
 * non-pointers go into iparamx).
 */
    void *private;
    void *private2;
    struct act_q aq;
    struct tq timer;
    void *pparam0;
    void *pparam1;
    void *pparam2;
    long iparam0;
    long iparam1;
    long iparam2;
} QioIOQ;

typedef struct qio_mutex {
    struct act_q *current;	/* which mutex is currently being used */
    struct act_q *waiting;	/* list of those waiting to use this mutex */
    struct act_q *tail;		/* last guy in the list */
} QioMutex;    

typedef struct qio_device {
    const char *name;		/* name of device */
    int name_len;		/* length of name string */
    const struct qio_file_ops *fio_ops; /* list of FIO operations allowed on device */
    QioMutex *mutex;		/* pointer to mutex for device */
    int unit;			/* unit number for device */
    void *private;		/* value specific to device */
} QioDevice;

/* Bits in the flags member of the qio_file struct:
 */
#define QIO_FFLAG_CLOSING	0x0001	/* close pending */
#define QIO_FFLAG_SOCKET	0x0002	/* file is a socket */
#define QIO_FFLAG_CANCEL	0x0004	/* cancel pending */

typedef struct qio_file {
    int mode;			/* access mode */
    unsigned long pos;		/* current position in file of next byte to r/w */
    unsigned long size;		/* size of file in bytes */
    unsigned long flags;	/* misc flags */
    unsigned long gen;		/* generation number */
    const QioDevice *dvc;	/* ptr to device that this file is on */
    void *private;		/* reserved for use by network, qio and fsys */
    struct qio_file *next;	/* when free, pointer to next free. When busy, ptr to next dvc */
    QioMutex mutex;		/* mutex to use with this file */
} QioFile;
        
typedef struct open_spc {
    const char *path;
    int mode;
} QioOpenSpc;

typedef struct qio_file_ops {
    int (*lseek) (QioIOQ *ioq, size_t where, int whence);
    int (*read) (QioIOQ *ioq, void *buf, int len);
    int (*write) (QioIOQ *ioq, const void *buf, int len);  
    int (*ioctl) (QioIOQ *ioq, unsigned int cmd, void *arg);
    int (*open) (QioIOQ *ioq, const char *name);
    int (*close) (QioIOQ *ioq);
    int (*delete) (QioIOQ *ioq, const char *path);
    int (*fsync) (QioIOQ *ioq);
    int (*mkdir) (QioIOQ *ioq, const char *path, int mode);
    int (*rmdir) (QioIOQ *ioq, const char *path);
    int (*rename) (QioIOQ *ioq, const char *old, const char *new);
    int (*truncate) (QioIOQ *ioq);
    int (*statfs) (QioIOQ *ioq, const char *path, void *stat);
    int (*fstat) (QioIOQ *ioq, struct stat *stat);
    int (*cancel) (QioIOQ *ioq);
    int (*isatty) (QioIOQ *ioq);
    int (*readwpos) (QioIOQ *ioq, size_t where, void *buf, int len);
    int (*writewpos) (QioIOQ *ioq, size_t where, const void *buf, int len);  
    int (*opendir) (QioIOQ *ioq, void **dpp, const char *name);
    int (*seekdir) (QioIOQ *ioq, void *dirp);
    int (*telldir) (QioIOQ *ioq, void *dirp);
    int (*rewdir) (QioIOQ *ioq, void *dirp);
    int (*readdir) (QioIOQ *ioq, void *dirp, void *directp);
    int (*closedir) (QioIOQ *ioq, void *dirp);
} QioFileOps;

/* The following functions are used by device drivers, although their
 * use is not restricted to same. Extreme care must be used if using
 * these functions outside a device driver.
 */

/************************************************************
 * qio_getmutex - get mutex and switch to AST level.
 * 
 * At entry:
 *	mutex - pointer to mutex struct
 *	func - ptr to AST function
 *	ioq - pointer to function's argument, a QioIOQ
 *
 * At exit:
 *	returns 0 if success or one of QIO_MUTEX_xxx if not.
 *
 * The function is queued at the specified AST level. If the mutex is
 * busy, the function is put on the end of mutex's wait queue and will
 * be executed only after the mutex is free'd.
 */
extern int qio_getmutex(QioMutex *mutex, void (*func)(QioIOQ *), QioIOQ *ioq);

/************************************************************
 * qio_freemutex - free a previously claimed mutex.
 * 
 * At entry:
 *	mutex - pointer to mutex struct
 *	ioq - pointer the function's argument, a QioIOQ, that
 *		was used in the qio_getmutex(). This parameter
 *		is used only to validate that the 'free' is
 *		being performed by the same function that
 *		claimed the mutex. The contents pointed to by
 *		this parameter is not referenced.
 *
 * At exit:
 *	returns 0 if success or one of QIO_MUTEX_xxx if error.
 *	If any tasks are waiting for mutex, the first one in the
 *	list is queued at QIO_ASTLVL.
 */
extern int qio_freemutex(QioMutex *mutex, QioIOQ *ioq);

/************************************************************
 * qio_install_dvc - Add a new device to the device table
 * 
 * At entry:
 *	dvc - pointer to new device 
 *
 * At exit:
 *	device number installed or -1 if failed to install.
 */
int qio_install_dvc(const QioDevice *dvc);

/************************************************************
 * qio_deinstall_dvc - Remove a device from the device table
 * 
 * At entry:
 *	dvc - pointer to device 
 *
 * At exit:
 *	returns 0 if success, 1 if device not in table
 */
int qio_deinstall_dvc(const QioDevice *dvc);

/************************************************************
 * qio_freefile - Put an unused QioFile back into the system's pool
 * 
 * At entry:
 *	file - pointer to QioFile which to free
 *
 * At exit:
 *	returns 0 if success or 1 if failure.
 */
int qio_freefile(QioFile *file);

/***********************************************************************
 * qio_init - Initialize the QIO data structs. To be called once during
 * boot sequence.
 * 
 * At entry:
 *	No requirements.
 *
 * At exit:
 *	Returns 0.
 */
int qio_init(void);

/************************************************************
 * qio_lookupdvc - Get a pointer to device
 * 
 * At entry:
 *	name - pointer to null terminated string with device name
 *
 * At exit:
 *	Returns pointer to QioDevice if one is found or 0 if not.
 */

const QioDevice *qio_lookupdvc(const char *name);

/************************************************************
 * qio_fd2file - convert fd to pointer to ptr to QioFile
 * 
 * At entry:
 *	fd - file descriptor
 *
 * At exit:
 *	returns pointer to QioFile or 0 if error
 */
QioFile *qio_fd2file( int fd );

/************************************************************
 * qio_complete - call user's completion routine. This routine
 * is used by device drivers and such at the conclusion of their
 * operations.
 * 
 * At entry:
 *	ioq - pointer to QioIOQ struct
 *
 * NOTE: the following members in the ioq must have already been
 *	set before calling this function:
 *	ioq->complete - if completion required else 0
 *
 * At exit:
 *	returns nothing. May have queued the user's completion
 *	routine (if provided) on the QIO_ASTLVL queue.
 */
void qio_complete(QioIOQ *ioq);

#if HOST_BOARD
/************************************************************
qio_sbrk - doles out sections from a predefined array (qio_memory). This
function is the underlying support function required and used by _malloc_r()
and company (the re-enterant equivalent functions to malloc(), etc). Although
qio_sbrk could be used directly by any local function, it it reserved for use
by the libc functions. The re-enterant functions actually call _sbrk_r() which
notices that the 'ptr' argument == qio_reent and, in turn, calls this
function. _sbrk_r() is a function found in phx_stubs.c.
 *
 * At entry:
 *	ptr - pointer to struct _reent
 *	amt - amount to increase(decrease) allocated area
 * At exit:
 *	'amt' bytes removed from allocation pool
 *	returns pointer to newly allocated area or
 *	0 if there is not enough memory to satisfy the
 *	request.
 */
void *qio_sbrk (struct _reent *ptr, size_t amt);

/************************************************************
qio_get_pool_size - returns amount of free space in the qio_free_pool
 *
 * At entry:
 *	ptr - pointer to struct _reent
 * At exit:
 *	'amt' bytes remaining in free pool
 */
int qio_get_pool_size (struct _reent *ptr);

/************************************************************
qio_reset_free_pool - reset the free pool size to its max.
 *
 * At entry:
 *	ptr - pointer to struct _reent
 * At exit:
 *	'amt' bytes remaining in free pool
 */
int qio_reset_free_pool (struct _reent *ptr);
#endif		/* HOST_BOARD */

/*
 * The following functions can be used by game code and/or device drivers.
 */

/************************************************************
 * qio_getioq - Get a QioIOQ from the system I/O queue pool
 * 
 * At entry:
 *	no requirements
 *
 * At exit:
 *	returns pointer to queue or 0 if none available.
 */
QioIOQ *qio_getioq(void);

/************************************************************
 * qio_freeioq - Free a QioIOQ as obtained from a previous
 * call to qio_getioq().
 * 
 * At entry:
 *	que - pointer to queue element to put back in pool.
 *
 * At exit:
 *	0 if success or 1 if queue didn't belong to pool.
 */
int qio_freeioq(QioIOQ *que);

/************************************************************
 * qio_open - Open a device or file
 * 
 * At entry:
 *	ioq - a pointer to a QioIOQ struct
 *	name - pointer to a null terminated string with dvc/path/name.
 *	mode - the logical 'or' of one or more of the O_xxx flags found
 *		in fcntl.h
 *
 * NOTE: the following members in the ioq must have already been
 *	set before calling this function:
 *	ioq->complete - if completion required else 0
 *	ioq->timeout - if a timeout (in microseconds) is desired else 0
 *	(other members are don't cares).
 *
 * At exit:
 *	Returns one of QIO_xxx if there is an error or 0 for success.
 *	The completion routine specified in the que struct is called
 *	when the open completes. The iostatus member of ioq will
 *	contain status about the success or failure of the open and the
 *	file member of ioq will contain a file descriptor (a number
 *	from 0 to QIO_MAX_FILES-1) if the open completes successfully
 *	(or a -1 if not).
 */
int qio_open(QioIOQ *ioq, const char *name, int mode);

/************************************************************
 * qio_openspc - Open a device or file
 * 
 * At entry:
 *	que - a pointer to a QioIOQ struct
 *	spc - pointer to a QioOpenSpc struct which may be defined
 *		differently depending on the device which is being opened.
 *		The first two members of the struct _MUST_ be path and
 *		mode respectively.
 *
 * NOTE: the following members in the ioq must have already been
 *	set before calling this function:
 *	ioq->complete - if completion required else 0
 *	ioq->timeout - if a timeout (in microseconds) is desired else 0
 *	(other members are don't cares).
 *
 * At exit:
 *	Returns one of QIO_xxx if there is an error or 0 for success.
 *	The completion routine specified in the que struct is called
 *	when the open completes. The iostatus member of ioq will
 *	contain status about the success or failure of the open and the
 *	file member of ioq will contain a file descriptor (a number
 *	from 0 to QIO_MAX_FILES-1) if the open completes successfully
 *	(or a -1 if not).
 */
int qio_openspc(QioIOQ *ioq, QioOpenSpc *spc);

/************************************************************
 * qio_close - close a file
 * 
 * At entry:
 *	ioq - pointer to QioIOQ struct
 *
 * NOTE: the following members in the ioq must have already been
 *	set before calling this function:
 *	ioq->file - must be a valid file descriptor
 *	ioq->complete - if completion required else 0
 *	ioq->timeout - if a timeout (in microseconds) is desired else 0
 *	(other members are don't cares).
 *
 * At exit:
 *	0 if function successfully queued and completion routine
 *	will be called, if one is provided, when close completes.
 *	non-zero if unable to queue the close and completion routne
 *	will _not_ be called in that case.
 */
int qio_close(QioIOQ *ioq);

/************************************************************
 * qio_ioctl - issue an ioctl to specified file
 * 
 * At entry:
 *	ioq - pointer to QioIOQ struct
 *	cmd - command argument
 *	arg - argument defined by command and device
 *
 * At exit:
 *	0 if function successfully queued and completion routine
 *	will be called, if one is provided, when close completes.
 *	non-zero if unable to queue the close and completion routne
 *	will _not_ be called in that case.
 */
extern int qio_ioctl(QioIOQ *ioq, unsigned int cmd, void *arg);
extern int ioctl(int fd, unsigned int cmd, unsigned long arg);

/************************************************************
 * qio_delete - delete a file
 * 
 * At entry:
 *	ioq - pointer to QioIOQ struct
 *	name - name of file to delete
 *
 * NOTE: the following members in the ioq must have already been
 *	set before calling this function:
 *	ioq->complete - if completion required else 0
 *	ioq->timeout - if a timeout (in microseconds) is desired else 0
 *	(other members are don't cares).
 *
 * At exit:
 *	0 if function successfully queued and completion routine
 *	will be called, if one is provided, when close completes.
 *	non-zero if unable to queue the close and completion routne
 *	will _not_ be called in that case.
 */
int qio_delete(QioIOQ *ioq, const char *name);

/************************************************************
 * qio_rename - rename a file
 * 
 * At entry:
 *	ioq - pointer to QioIOQ struct
 *	old - pointer to old filename
 *	new - pointer to new filename
 *
 * NOTE: the following members in the ioq must have already been
 *	set before calling this function:
 *	ioq->complete - if completion required else 0
 *	ioq->timeout - if a timeout (in microseconds) is desired else 0
 *	(other members are don't cares).
 *
 * At exit:
 *	0 if function successfully queued and completion routine
 *	will be called, if one is provided, when close completes.
 *	non-zero if unable to queue the close and completion routne
 *	will _not_ be called in that case. The ioq->file field will
 *	be set to -1.
 */
int qio_rename(QioIOQ *ioq, const char *old, const char *new);

/************************************************************
 * qio_isatty - is device a tty
 * 
 * At entry:
 *	ioq - pointer to QioIOQ struct
 *
 * NOTE: the following members in the ioq must have already been
 *	set before calling this function:
 *	ioq->file - must be a valid file descriptor
 *	ioq->complete - if completion required else 0
 *	ioq->timeout - if a timeout (in microseconds) is desired else 0
 *	(other members are don't cares).
 *
 * At exit:
 *	0 if function successfully queued and completion routine
 *	will be called, if one is provided, when close completes.
 *	non-zero if unable to queue the close and completion routne
 *	will _not_ be called in that case. If device is a tty,
 *	iostatus will be IO_SUCC otherwise IO_NOTSUPP.
 */
int qio_isatty(QioIOQ *ioq);

/************************************************************
 * qio_fstat - stat a file or device
 * 
 * At entry:
 *	ioq - pointer to QioIOQ struct (ioq->file must be open)
 *	stat - pointer to struct stat into which the stats are to
 *		be placed.
 * 
 * NOTE: the following members in the ioq must have already been
 *	set before calling this function:
 *	ioq->file - must be a valid file descriptor
 *	ioq->complete - if completion required else 0
 *	ioq->timeout - if a timeout (in microseconds) is desired else 0
 *	(other members are don't cares).
 *
 * At exit:
 *	0 if function successfully queued and completion routine
 *	will be called, if one is provided, when stat completes.
 *	non-zero if unable to queue the stat and completion routne
 *	will _not_ be called in that case. The values set to the
 *	members, and which members, in the struct stat are device
 *	dependent.
 */
int qio_fstat(QioIOQ *ioq, struct stat *stat);

/************************************************************
 * qio_read - read bytes from file
 * 
 * At entry:
 *	ioq - pointer to QioIOQ struct
 *	buf - pointer to buffer into which to read
 *	len - number of bytes to read
 * 
 * NOTE: the following members in the ioq must have already been
 *	set before calling this function:
 *	ioq->file - must be a valid file descriptor
 *	ioq->complete - if completion required else 0
 *	ioq->timeout - if a timeout (in microseconds) is desired else 0
 *	(other members are don't cares).
 *
 * At exit:
 *	returns 0 if success or non-zero if error
 */
int qio_read(QioIOQ *ioq, void *buf, int len);

/****************************************************************
 * qio_readwpos - read bytes from file after positioning to 'where'
 * 
 * At entry:
 *	ioq - pointer to QioIOQ struct
 *	where - position of place in file to begin reading
 *	buf - pointer to buffer into which to read
 *	len - number of bytes to read
 * 
 * This function is equivalent to a
 *	qio_lseek(ioq, where, SEEK_SET) followed by a
 *	qio_read(ioq, buf, len);
 *
 * NOTE: the following members in the ioq must have already been
 *	set before calling this function:
 *	ioq->file - must be a valid file descriptor
 *	ioq->complete - if completion required else 0
 *	ioq->timeout - if a timeout (in microseconds) is desired else 0
 *	(other members are don't cares).
 *
 * At exit:
 *	returns 0 if success or non-zero if error
 */
int qio_readwpos(QioIOQ *ioq, size_t where, void *buf, int len);

/************************************************************
 * qio_write - write bytes to file
 * 
 * At entry:
 *	ioq - pointer to QioIOQ struct
 *	buf - pointer to buffer which to write
 *	len - number of bytes to write
 * 
 * NOTE: the following members in the ioq must have already been
 *	set before calling this function:
 *	ioq->file - must be a valid file descriptor
 *	ioq->complete - if completion required else 0
 *	ioq->timeout - if a timeout (in microseconds) is desired else 0
 *	(other members are don't cares).
 *
 * At exit:
 *	returns 0 if success or non-zero if error
 */
int qio_write(QioIOQ *ioq, const void *buf, int len);

/****************************************************************
 * qio_writewpos - write bytes to file after position to 'where'
 * 
 * At entry:
 *	ioq - pointer to QioIOQ struct
 *	where - position in file/device to start writing
 *	buf - pointer to buffer which to write
 *	len - number of bytes to write
 * 
 * This function is equivalent to a
 *	qio_lseek(ioq, where, SEEK_SET) followed by a
 *	qio_write(ioq, buf, len);
 *
 * NOTE: the following members in the ioq must have already been
 *	set before calling this function:
 *	ioq->file - must be a valid file descriptor
 *	ioq->complete - if completion required else 0
 *	ioq->user (and/or ioq->user2) - if ioq->complete also set
 *	ioq->timeout - if a timeout (in microseconds) is desired else 0
 *	(other members are don't cares).
 *
 * At exit:
 *	returns 0 if success or non-zero if error
 */
int qio_writewpos(QioIOQ *ioq, size_t where, const void *buf, int len);

/************************************************************
 * qio_lseek - seek a file to a specific position
 * 
 * At entry:
 *	ioq - pointer to QioIOQ struct
 *	where - new position
 *	whence - one of SEEK_SET, SEEK_END or SEEK_CUR (as defined
 *		in <unistd.h>
 * 
 * NOTE: the following members in the ioq must have already been
 *	set before calling this function:
 *	ioq->file - must be a valid file descriptor
 *	ioq->complete - if completion required else 0
 *	ioq->timeout - if a timeout (in microseconds) is desired else 0
 *	(other members are don't cares).
 *
 * At exit:
 *	0 if successfully queued, although it normally does not
 *	require that any queuing take place. iostatus contains
 *	error code if any and iocount contains the new position.
 */
extern int qio_lseek(QioIOQ *ioq, size_t where, int whence );

/************************************************************
 * qio_mkdir - make a directory 
 * 
 * At entry:
 *	que - a pointer to a QioIOQ struct
 *	arg - pointer to null terminated string with dvc/path/dirname
 *	mode - not used at this time.
 *
 * At exit:
 *	0 if function successfully queued and completion routine
 *	will be called, if one is provided, when open completes.
 *	non-zero if unable to queue the open and completion routne
 *	will _not_ be called in that case.
 *	The file member of ioq will be set to -1 in any case.
 */
extern int qio_mkdir(QioIOQ *ioq, const char *name, int mode);

/************************************************************
 * qio_opendir - Open a directory 
 * 
 * At entry:
 *	ioq - a pointer to a QioIOQ struct
 *	dirp - pointer to pointer to type DIR (as defined in fsys.h for fsys directories)
 *	path - pointer to a null terminated string with dvc/path/name.
 *
 * At exit:
 *	0 if function successfully queued and completion routine
 *	will be called, if one is provided, when open completes.
 *	non-zero if unable to queue the open and completion routne
 *	will _not_ be called in that case.
 *	A pointer to a kernel provided struct (DIR in the case of fsys files)
 *	will have been placed into the location pointed to by the dirp parameter
 *	or 0 if an error prevented the open.
 *	ioq->private holds information required by subsequent 'qio_xxxdir' qio
 *	functions. Do not modify the contents of ioq->private until after a
 *	qio_closedir() has been executed using the ioq.
 */
extern int qio_opendir(QioIOQ *ioq, void **dirp, const char *path);

/************************************************************
 * qio_readdir - read a directory entry.
 * 
 * At entry:
 *	ioq - a pointer to a QioIOQ struct
 *	dirp - pointer to pointer to type DIR (as defined in fsys.h for fsys directories)
 *	direct - pointer to a type 'struct direct' into which the results will be placed.
 *
 * NOTE: the following members in the ioq must have already been
 *	set before calling this function:
 *	ioq->complete - if completion required else 0
 *	ioq->timeout - if a timeout (in microseconds) is desired else 0
 *	ioq->private is required to have a value as set by a previous call to qio_opendir().
 *	(other members are don't cares).
 *
 * At exit:
 *	0 if function successfully queued and completion routine
 *	will be called, if one is provided, when open completes.
 *	non-zero if unable to queue the open and completion routne
 *	will _not_ be called in that case.
 */
extern int qio_readdir(QioIOQ *ioq, void *dirp, void *direct);

extern int qio_rewdir(QioIOQ *ioq, void *dirp);
extern int qio_closedir(QioIOQ *ioq, void *dirp);
extern int qio_seekdir(QioIOQ *ioq, void *dirp, long loc);
extern int qio_telldir(QioIOQ *ioq, void *dirp);

/************************************************************
 * qio_fsync - sync a filesystem
 * 
 * At entry:
 *	ioq - pointer to QioIOQ struct
 *	name - pointer to null terminated string with name of
 *		filesystem.
 * 
 * NOTE: the following members in the ioq must have already been
 *	set before calling this function:
 *	ioq->complete - if completion required else 0
 *	ioq->timeout - if a timeout (in microseconds) is desired else 0
 *	(other members are don't cares).
 *
 * At exit:
 *	0 if function successfully queued and completion routine
 *	will be called, if one is provided, when fsync completes.
 *	non-zero if unable to queue the fsync and completion routne
 *	will _not_ be called in that case.
 */
extern int qio_fsync(QioIOQ *ioq, const char *name);

/************************************************************
 * qio_errmsg - decode status code into an ASCII error message
 *
 * At entry:
 *	sts - status code to decode (one of the values listed below)
 *	ans - ptr to string into which message will be placed
 *	size - size of ans 
 *
 * At exit:
 *	returns length of message in ans which will have had the
 *	text of the decoded message copied into it or a
 *	"unknown error xxxxxxxx" string if sts isn't decodable.
 */

extern int qio_errmsg(int sts, char *ans, int size);

/************************************************************
 * qio_cancel - cancel a pending qio
 *
 * At entry:
 *	ioq - pointer to QioIOQ struct
 *
 * At exit:
 *	0 if function successfully queued and completion routine
 *	will be called, if one is provided, when cancel completes.
 *	Non-zero if unable to queue the cancel; completion routine
 *	will _not_ be called in that case.
 */

extern int qio_cancel(QioIOQ *ioq);

/************************************************************
 * qio_cleanup - remove QioIOQ structures associated with "file"
 *		 from mutex waiting list specified by "pqm", and
 *		 complete them with "status".
 * 
 * At entry:
 *	pqm    - pointer to QioMutex structure
 *	file   - file id to match during search
 *	status - completion iostatus
 *
 * At exit:
 *	Non-zero if QioIOQ structure on mutex.current is associated
 *	with "file" -- pointer to QioIOQ but mutex is not released.
 *	Zero if QioIOQ structure on mutex.current is not associated
 *	with "file"
 *
 *	Used in qio_eth.c by eth_cancel_ast.
 */

extern QioIOQ *qio_cleanup( QioMutex *pqm, int file, size_t status );

/************************************************************
 * unlink - delete a file
 * 
 * At entry:
 *	str - pointer to null terminated ASCII filename
 *
 * At exit:
 *	returns 0 on success or negative 'n' if error. 'n'
 *	is one of the QIO_ERR's described below.
 */

extern int unlink(const char *ptr);

/*
 * The following are the definitions of the various errors and their
 * descriptions. This qio.h file includes itself if it is #included
 * with QIO_ERR_MSG undef'd which defines the error enums, required
 * by all users of the qio subsystem. The qio.h file is #included a
 * second time (technically, a third time) in qio_errs.c with
 * QIO_ERR_MSG defined appropriately beforehand which gets the ASCII
 * error message text deposited into a message table.
 */

/*
If you need to add new messages, invent a new facility name (if you want or
need one) and insert it at the end of FACILITY list below (just before the
#include <qio.h>). Next add the 0'th error code for your facility by adding at
the end of the QIO_ERR_MSG() list, the following:

QIO_ERR_MSG( your_name_SUCC, =FACILITY_your_name<<FACILITY_SHIFT, 
    	     "successful I/O" ),

where 'your_name' is the name you invented for your facility. Next, add your
errors in any order immediately following that entry.
*/

#ifndef QIO_ERR_MSG
#define QIO_ERR_CODE(x) ( (x) & ((1<<SEVERITY_SHIFT)-1) )
#define QIO_ERR_SEV(x)	( ((x) >> SEVERITY_SHIFT) & 3 )
#define QIO_ERR_FAC(x)	( ((x) >> FACILITY_SHIFT) )

enum error_codes {
    FACILITY_SHIFT=10,		/* number of bits to shift right to get facility code */
    SEVERITY_SHIFT=8,		/* number of bits to shift right to get severity code */
    SEVERITY_ERROR=0,		/* error is the normal case */
    SEVERITY_INFO=1<<SEVERITY_SHIFT,	/* code is informational */
    SEVERITY_FATAL=2<<SEVERITY_SHIFT,	/* code is fatal */
    SEVERITY_WARN=3<<SEVERITY_SHIFT,	/* code is a warning */
#define QIO_FACILITY_MSG(name, text) FACILITY_##name
#include <qio.h>
#undef QIO_FACILITY_MSG
#define QIO_ERR_MSG(name, init, text) name init
#include <qio.h>
#undef QIO_ERR_MSG
    MAX_ERROR_MSGS		/* placeholder for end of list */
};

#endif				/* QIO_ERR_MSG */
#endif				/* _QIO_H_ */

#ifdef QIO_FACILITY_MSG
QIO_FACILITY_MSG( FSYSIO=1, "FSYSIO" ),	/* filesystem I/O	*/
QIO_FACILITY_MSG( MOUNT, "MOUNT" ), 	/* volume MOUNT errors */
QIO_FACILITY_MSG( INITFS, "INITFS" ), 	/* volume INIT errors */
QIO_FACILITY_MSG( CREATE, "CREATE" ),	/* file CREATE errors */
QIO_FACILITY_MSG( DELETE, "DELETE" ), 	/* file DELETE errors */
QIO_FACILITY_MSG( LOOKUP, "LOOKUP" ), 	/* file LOOKUP errors */
QIO_FACILITY_MSG( OPEN, "OPEN" ),	/* file OPEN errors */
QIO_FACILITY_MSG( EXTEND, "EXTEND" ),	/* file EXTEND errors */
QIO_FACILITY_MSG( SYNC, "SYNC" ),	/* volume SYNC errors */
QIO_FACILITY_MSG( FREE, "FREE" ), 	/* volume FREE errors */
QIO_FACILITY_MSG( MUTEX, "MUTEX" ),	/* MUTEX handling errors */
QIO_FACILITY_MSG( QIO, "QIO" ),		/* QIO errors */
QIO_FACILITY_MSG( HDIO, "HDIO" ),	/* Hard Drive I/O errors */
QIO_FACILITY_MSG( DMA, "DMA" ),		/* DMA Driver errors */
QIO_FACILITY_MSG( ETH, "ETH" ),		/* Ethernet Driver errors */
QIO_FACILITY_MSG( SOCK, "SOCKET" ),	/* BSD socket functions */

#endif				/* QIO_FACILITY_MSG */

#ifdef QIO_ERR_MSG
QIO_ERR_MSG( FSYS_IO_SUCC, =FACILITY_FSYSIO<<FACILITY_SHIFT, "successful fsys I/O" ),
QIO_ERR_MSG( FSYS_IO_RDERR,,		"file read error" ),
QIO_ERR_MSG( FSYS_IO_WRTERR,,		"file write error" ),
QIO_ERR_MSG( FSYS_IO_FATAL,,		"file read/write fatal internal error" ),
QIO_ERR_MSG( FSYS_IO_NOSUPP,,		"function not supported by fsys" ),
QIO_ERR_MSG( FSYS_IO_NOTOPEN,,		"file not open" ),
QIO_ERR_MSG( FSYS_IO_NOT512,,		"length not a multiple of sector size (512)" ),
QIO_ERR_MSG( FSYS_IO_NOQIO,,		"ran out of FsysQio's" ),
QIO_ERR_MSG( FSYS_IO_NODIRS,,		"ran of of DIR structs" ),
QIO_ERR_MSG( FSYS_MOUNT_SUCC, =FACILITY_MOUNT<<FACILITY_SHIFT,	"successful mount" ),
QIO_ERR_MSG( FSYS_MOUNT_FATAL,,		"fatal internal error in mount code" ),
QIO_ERR_MSG( FSYS_MOUNT_BADLVL,,	"executing at wrong level" ),
QIO_ERR_MSG( FSYS_MOUNT_NOMEM,,		"not enough memory for mount" ),
QIO_ERR_MSG( FSYS_MOUNT_NOSUPP,,	"feature not supported yet" ),
QIO_ERR_MSG( FSYS_MOUNT_NOHBLK,,	"could not find any valid home blocks" ),
QIO_ERR_MSG( FSYS_MOUNT_NOINDX,,	"could not read any index file headers" ),
QIO_ERR_MSG( FSYS_MOUNT_NOINDXF,,	"could not read any index files" ),
QIO_ERR_MSG( FSYS_MOUNT_NOFREE,,	"could not read any freemap file headers" ),
QIO_ERR_MSG( FSYS_MOUNT_NOFREEF,,	"could not read any freemap files" ),
QIO_ERR_MSG( FSYS_MOUNT_NOROOT,,	"could not read any root file headers" ),
QIO_ERR_MSG( FSYS_MOUNT_NOROOTF,,	"could not read any root files" ),
QIO_ERR_MSG( FSYS_MOUNT_BUSY,,		"volume is busy mounting" ),
QIO_ERR_MSG( FSYS_MOUNT_FHRDERR,,	"file header read error" ),
QIO_ERR_MSG( FSYS_MOUNT_RDERR,,		"(probably a) directory read error" ),
QIO_ERR_MSG( FSYS_MOUNT_DIRCORR,,	"directory corrupt" ),
QIO_ERR_MSG( FSYS_MOUNT_MOUNTED,,	"volume is already mounted" ),
QIO_ERR_MSG( FSYS_MOUNT_NSV,,		"no such volume name" ),
QIO_ERR_MSG( FSYS_MOUNT_NOPHYS,,	"couldn't stat the physical device" ),
QIO_ERR_MSG( FSYS_MOUNT_NOIOQ,,		"ran out of IOQ's" ),
QIO_ERR_MSG( FSYS_INITFS_SUCC, =FACILITY_INITFS<<FACILITY_SHIFT, "completed successfully" ),
QIO_ERR_MSG( FSYS_INITFS_INVARG,,	"invalid argument to initfs" ),
QIO_ERR_MSG( FSYS_INITFS_FATAL,,	"fatal internal error in initfs code" ),
QIO_ERR_MSG( FSYS_INITFS_BADLVL,,	"executing at wrong level" ),
QIO_ERR_MSG( FSYS_INITFS_NOMEM,,	"ran out of memory" ),
QIO_ERR_MSG( FSYS_INITFS_NOSUPP,,	"feature not supported" ),
QIO_ERR_MSG( FSYS_INITFS_BADHB,,	"error writing the home blocks" ),
QIO_ERR_MSG( FSYS_INITFS_BADINDX,,	"error writing the index header" ),
QIO_ERR_MSG( FSYS_INITFS_BADINDXF,,	"error writing the index file" ),
QIO_ERR_MSG( FSYS_INITFS_BADFREE,,	"error writing the freemap header" ),
QIO_ERR_MSG( FSYS_INITFS_BADFREEF,, 	"error writing the freemap file" ),
QIO_ERR_MSG( FSYS_INITFS_BADROOT,,	"error writing the root header" ),
QIO_ERR_MSG( FSYS_INITFS_BADROOTF,,	"error writing the root file" ),
QIO_ERR_MSG( FSYS_INITFS_BUSY,,		"initfs is already busy" ),
QIO_ERR_MSG( FSYS_INITFS_NODRV,,	"no drive" ),
QIO_ERR_MSG( FSYS_INITFS_NOINFO,,	"error getting drive info" ),
QIO_ERR_MSG( FSYS_INITFS_NOFREE,,	"ran out of free blocks" ),
QIO_ERR_MSG( FSYS_INITFS_NOIV,,		"No FsysInitVol parameter supplied" ),
QIO_ERR_MSG( FSYS_INITFS_CLUSTERNOT1,,	"Cluster size not equal to 1" ),
QIO_ERR_MSG( FSYS_INITFS_TOOMANYCLUST,,	"Allocated file sizes greater than drive capacity" ),
QIO_ERR_MSG( FSYS_CREATE_SUCC, =FACILITY_CREATE<<FACILITY_SHIFT,	"file creation completed successfully" ),
QIO_ERR_MSG( FSYS_CREATE_INVARG,,	"invalid argument to file creation call" ),
QIO_ERR_MSG( FSYS_CREATE_FATAL,,	"fatal internal error in creation handler" ),
QIO_ERR_MSG( FSYS_CREATE_BADLVL,,	"executing at wrong level" ),
QIO_ERR_MSG( FSYS_CREATE_NOMEM,,	"create ran out of memory" ),
QIO_ERR_MSG( FSYS_CREATE_NOSUPP,,	"cannot create on read only filesystem" ),
QIO_ERR_MSG( FSYS_CREATE_NOPATH,,	"no such path" ),
QIO_ERR_MSG( FSYS_CREATE_NAMEINUSE,,	"name already in use by a directory" ),
QIO_ERR_MSG( FSYS_CREATE_NOFH,,		"filesystem full, no more file headers" ),
QIO_ERR_MSG( FSYS_CREATE_FULL,,		"filesystem full, no room for file" ),
QIO_ERR_MSG( FSYS_CREATE_FATALNOPRIV,,	"private field has been zeroed" ),
QIO_ERR_MSG( FSYS_DELETE_SUCC, =FACILITY_DELETE<<FACILITY_SHIFT,	"file deletion completed successfully" ),
QIO_ERR_MSG( FSYS_DELETE_INVARG,,	"invalid argument to file deletion call" ),
QIO_ERR_MSG( FSYS_DELETE_NOMEM,,	"delete ran out of memory" ),
QIO_ERR_MSG( FSYS_DELETE_FNF,,		"no such file" ),
QIO_ERR_MSG( FSYS_DELETE_DIR,,		"cannot delete a directory" ),
QIO_ERR_MSG( FSYS_LOOKUP_SUCC, =FACILITY_LOOKUP<<FACILITY_SHIFT,	"lookup worked" ),
QIO_ERR_MSG( FSYS_LOOKUP_INVARG,,	"invalid argument to lookup" ),
QIO_ERR_MSG( FSYS_LOOKUP_FATAL,,	"fatal internal error in lookup" ),
QIO_ERR_MSG( FSYS_LOOKUP_NOPATH,,	"no such directory" ),
QIO_ERR_MSG( FSYS_LOOKUP_FNF,,		"no such file" ),
QIO_ERR_MSG( FSYS_LOOKUP_TOODEEP,,	"too many directories" ),
QIO_ERR_MSG( FSYS_LOOKUP_FSNF,,		"no such file system to stat" ),
QIO_ERR_MSG( FSYS_LOOKUP_NOTDIR,,	"not a directory" ),
QIO_ERR_MSG( FSYS_OPEN_SUCC, =FACILITY_OPEN<<FACILITY_SHIFT,	"file open completed successfully" ),
QIO_ERR_MSG( FSYS_OPEN_INVARG,,		"invalid argument to file open call" ),
QIO_ERR_MSG( FSYS_OPEN_FATAL,,		"fatal internal error in open handler" ),
QIO_ERR_MSG( FSYS_OPEN_BADLVL,,		"executing at wrong level" ),
QIO_ERR_MSG( FSYS_OPEN_NOMEM,,		"open ran out of memory" ),
QIO_ERR_MSG( FSYS_OPEN_NOFID,,		"No such file (FID out of range)" ),
QIO_ERR_MSG( FSYS_OPEN_NOGEN,,		"No such file (generation number mismatch)" ),
QIO_ERR_MSG( FSYS_OPEN_NOTMNT,,		"No such file (volume not mounted)" ),
QIO_ERR_MSG( FSYS_OPEN_NOTOPEN,,	"Fatal internal error. fsys_open_q found file closed." ),
QIO_ERR_MSG( FSYS_EXTEND_SUCC, =FACILITY_EXTEND<<FACILITY_SHIFT,	"extend completed successfully" ),
QIO_ERR_MSG( FSYS_EXTEND_INVARG,,	"invalid argument to extend function" ),
QIO_ERR_MSG( FSYS_EXTEND_FATAL,,	"fatal internal error in extend function" ),
QIO_ERR_MSG( FSYS_EXTEND_2MNYRP,,	"too many retrieval pointers" ),
QIO_ERR_MSG( FSYS_EXTEND_NOMEM,,	"extend function ran out of memory" ),
QIO_ERR_MSG( FSYS_EXTEND_FULL,,		"filesystem full" ),
QIO_ERR_MSG( FSYS_SYNC_SUCC, =FACILITY_SYNC<<FACILITY_SHIFT,	"sync completed successfully" ),
QIO_ERR_MSG( FSYS_SYNC_INVARG,,		"invalid argument to sync" ),
QIO_ERR_MSG( FSYS_SYNC_FATAL,,		"fatal internal error in sync function" ),
QIO_ERR_MSG( FSYS_SYNC_NOMEM,,		"sync function ran out of memory" ),
QIO_ERR_MSG( FSYS_SYNC_NOMNT,,		"file system not mounted" ),
QIO_ERR_MSG( FSYS_FREE_SUCC, =FACILITY_FREE<<FACILITY_SHIFT,	"freelist bit update completed successfully" ),
QIO_ERR_MSG( FSYS_FREE_INVARG,,		"invalid argument to freelist function" ),
QIO_ERR_MSG( FSYS_FREE_FATAL,,		"fatal internal error in freelist function" ),
QIO_ERR_MSG( FSYS_FREE_NOMEM,,		"freelist function ran out of memory" ),
QIO_ERR_MSG( QIO_MUTEX_SUCC, =FACILITY_MUTEX<<FACILITY_SHIFT,	"mutex function completed successfully" ),
QIO_ERR_MSG( QIO_MUTEX_INVARG,,		"invalid argument to mutex call" ),
QIO_ERR_MSG( QIO_MUTEX_FATAL,,		"fatal internal error in mutex handler" ),
QIO_ERR_MSG( QIO_MUTEX_BADLVL,,		"executing at wrong level" ),
QIO_ERR_MSG( QIO_MUTEX_NESTED,,		"mutex is already claimed by 'task'" ),
QIO_ERR_MSG( QIO_MUTEX_NOTBUSY,,	"mutex is not busy" ),
QIO_ERR_MSG( QIO_MUTEX_NOMUTEX,,	"mutex pool is empty" ),
QIO_ERR_MSG( QIO_MUTEX_QUEUED,,		"function to claim mutex has been queued" ),
QIO_ERR_MSG( QIO_MUTEX_NOTOWN,,		"mutex 'free'd by other than 'task' that claimed it" ),
QIO_ERR_MSG( QIO_MUTEX_NONE,,		"no mutex to free" ),
QIO_ERR_MSG( QIO_SUCC, =FACILITY_QIO<<FACILITY_SHIFT,	"function completed successfully" ),
QIO_ERR_MSG( QIO_INVARG,,		"invalid argument to QIO call" ),
QIO_ERR_MSG( QIO_FATAL,,		"fatal internal error in QIO handler" ),
QIO_ERR_MSG( QIO_BADLVL,,		"executing at wrong AST level" ),
QIO_ERR_MSG( QIO_NOTSUPP,,		"function not supported on device" ),
QIO_ERR_MSG( QIO_NOIOQ,,		"ran out of IOQ's" ),
QIO_ERR_MSG( QIO_NOMEM,,		"ran out of memory" ),
QIO_ERR_MSG( QIO_IOQBUSY,,		"QioIOQ already on a queue" ),
QIO_ERR_MSG( QIO_NSD,,			"no such device" ),
QIO_ERR_MSG( QIO_NOFILE,,		"no more file descriptors" ),
QIO_ERR_MSG( QIO_STAT_FNF,,		"no such file to stat" ),
QIO_ERR_MSG( QIO_NOTOPEN,,		"file not open" ),
QIO_ERR_MSG( QIO_EOF,,			"EOF on file" ),
QIO_ERR_MSG( QIO_NOT_ORD,,		"file not open for read" ),
QIO_ERR_MSG( QIO_NOT_OWR,,		"file not open for write" ),
QIO_ERR_MSG( QIO_NOTSAMEDVC,,		"cannot rename to different device" ),
QIO_ERR_MSG( QIO_ALREADY_INST,,		"device already installed" ),
QIO_ERR_MSG( QIO_TOO_MANY_DVCS,,	"no room for more devices" ),
QIO_ERR_MSG( HDIO_SUCC, =FACILITY_HDIO<<FACILITY_SHIFT, "function completed successfully" ),
QIO_ERR_MSG( HDIO_INVARG,,		"invalid argument to QIO call" ),
QIO_ERR_MSG( HDIO_FATAL,,		"fatal internal error in QIO handler" ),
QIO_ERR_MSG( HDIO_BADLVL,,		"executing at wrong AST level" ),
QIO_ERR_MSG( HDIO_NOTSUPP,,		"function not supported" ),
QIO_ERR_MSG( HDIO_NOHDIO,,		"ran out of HdIO's" ),
QIO_ERR_MSG( HDIO_RDERR,,		"read error on hard disk" ),
QIO_ERR_MSG( HDIO_WRERR,,		"write error on hard disk" ),
QIO_ERR_MSG( HDIO_SEEKERR,,		"seek error on hard disk" ),
QIO_ERR_MSG( HDIO_TIMEOUT,,		"device timeout on hard disk" ),
QIO_ERR_MSG( HDIO_NOMEM,,               "IDE driver ran out of memory" ),
QIO_ERR_MSG( HDIO_WRONLY,,              "file opened \"write only\"" ),
QIO_ERR_MSG( HDIO_RDONLY,,              "file opened \"read only\"" ),
QIO_ERR_MSG( HDIO_UNCORR,,              "Uncorrectable read error" ),
QIO_ERR_MSG( HDIO_IDNF,,                "Sector ID not found" ),
QIO_ERR_MSG( HDIO_ABRT,,		"device error" ),
QIO_ERR_MSG( HDIO_TK0NF,,		"Track 0 not found" ),
QIO_ERR_MSG( HDIO_AMNF,,		"Address mark not found" ),
QIO_ERR_MSG( DMA_SUCC, =FACILITY_DMA<<FACILITY_SHIFT, "function completed successfully" ),
QIO_ERR_MSG( DMA_NOTSUPP,,		"function not supported" ),
QIO_ERR_MSG( DMA_INVARG,,		"invalid argument" ),
QIO_ERR_MSG( DMA_TIMEOUT,,		"timeout on device" ),
QIO_ERR_MSG( DMA_CANCEL,,		"cancel on device" ),
QIO_ERR_MSG( DMA_INCONSISTENT,,		"inconsistent state" ),
QIO_ERR_MSG( ETH_SUCC, =FACILITY_ETH<<FACILITY_SHIFT, "function completed successfully" ),
QIO_ERR_MSG( ETH_NOTSUPP,,		"function not supported on device" ),
QIO_ERR_MSG( ETH_INVARG,,		"invalid argument" ),
QIO_ERR_MSG( ETH_TIMEOUT,,		"timeout on device" ),
QIO_ERR_MSG( ETH_CANCEL,,		"cancel on device" ),
QIO_ERR_MSG( ETH_NOIOQ,,		"ran out of IOQ's" ),
QIO_ERR_MSG( SOCK_SUCC, =FACILITY_SOCK<<FACILITY_SHIFT, "function completed successfully" ),
QIO_ERR_MSG( SOCK_INVARG,,		"invalid argument to socket call" ),
QIO_ERR_MSG( SOCK_FATAL,,		"fatal internal error in socket handler" ),
QIO_ERR_MSG( SOCK_BADLVL,,		"cannot run at interrupt, action or AST level" ),
QIO_ERR_MSG( SOCK_NOTSUPP,,		"function not supported in sockets" ),
QIO_ERR_MSG( SOCK_NOSOCKETS,,		"ran out of network sockets" ),
QIO_ERR_MSG( SOCK_NOTSOCK,,		"fd is not a socket" ),
QIO_ERR_MSG( SOCK_NOTAVAIL,,		"address is not available" ),
QIO_ERR_MSG( SOCK_AFNOSUPP,,		"address family not supported") ,
QIO_ERR_MSG( SOCK_ISCONN,,		"socket already connected" ),
QIO_ERR_MSG( SOCK_TIMEDOUT,,		"connection timed out" ),
QIO_ERR_MSG( SOCK_REFUSED,,		"connection refused" ),
QIO_ERR_MSG( SOCK_NOTREACH,,		"network not reachable" ),
QIO_ERR_MSG( SOCK_INUSE,,		"address already in use" ),
QIO_ERR_MSG( SOCK_INPROGRESS,,		"connection is proceeding" ),
QIO_ERR_MSG( SOCK_NONAMESRVR,,		"cannot contact nameserver" ),
QIO_ERR_MSG( SOCK_TOOMANYMEMBERS,,	"IGMP group member table full" ),
QIO_ERR_MSG( SOCK_RDERR,,		"read error on socket" ),
QIO_ERR_MSG( SOCK_WRERR,,		"write error on socket" ),
QIO_ERR_MSG( SOCK_NOTCONN,,		"socket not connected" ),
QIO_ERR_MSG( SOCK_TOOMUCH,,		"UDP datagram too big" ),
QIO_ERR_MSG( SOCK_BINDERR,,		"Unable to bind to addr/port" ),
QIO_ERR_MSG( SOCK_NOTSTREAM,,		"Not a TCP socket" ),
QIO_ERR_MSG( SOCK_NOTBOUND,,		"No BIND done on this socket" ),
QIO_ERR_MSG( SOCK_ERRACCEPT,,		"Error doing accept" ),
QIO_ERR_MSG( SOCK_NOPEER,,		"Error getting peer" ),
#endif		/* QIO_ERR_MSG */
