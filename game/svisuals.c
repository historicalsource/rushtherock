/*
 *	visuals.c	-- Functions for visual overlay effects.
 *
 *	Copyright 1996 Time Warner Interactive.
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: gray $						$Date: 1997/09/19 01:59:21 $
 *	$Revision: 3.82 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: visuals.c,v $
 * Revision 3.82  1997/09/19  01:59:21  gray
 * Changed shadow back scale fudge to 1.1.
 *
 * Revision 3.81  1997/09/18  01:20:38  gray
 * Fix verticle shadow bug, reorder cars, cheat shadow up for v3.
 *
 * Revision 3.80  1997/09/13  04:55:17  gray
 * fixed gObjList initialization for varying number of cars.
 *
 * Revision 3.79  1997/09/13  01:49:54  grjost
 * Fixed for 12 cars.
 *
 * Revision 3.78  1997/09/02  05:53:08  gray
 * Force proper reflections in car select.
 *
 * Revision 3.77  1997/07/23  01:36:26  gray
 * Moved reflection debug from switch 10 to switch 9.
 *
 * Revision 3.76  1997/05/29  21:08:03  gray
 * Clean up mph stuff.
 *
 * Revision 3.75  1996/12/14  10:36:10  geraci
 * prevents animation functions from running in track and car select
 *
 * Revision 3.74  1996/12/11  11:34:50  gray
 * Complete rework of skids. Now handled seperatly from Environs.
 *
 * Revision 3.73  1996/12/03  06:16:51  petrick
 * Totally clears Visuals entries when they're allocated.
 *
 * Revision 3.72  1996/12/02  03:33:22  geraci
 * fixed skid bug
 *
 * Revision 3.71  1996/11/27  17:51:03  gray
 * No body reflections in car select.
 *
 * Revision 3.70  1996/11/27  17:08:08  geraci
 * each car has its own in tunnel flag
 *
 * Revision 3.69  1996/11/27  16:43:20  gray
 * Reflection tweaks.
 *
 * Revision 3.68  1996/11/27  09:25:24  petrick
 * Changed types of error messages to production types.
 *
 * Revision 3.67  1996/11/27  08:13:46  petrick
 * Turned off spark debug code.
 *
 * Revision 3.66  1996/11/27  05:02:45  petrick
 * Added err msg to GrabEnvEntry.
 *
 * Revision 3.65  1996/11/27  01:54:03  petrick
 * Took out a bunch of debug, made objnum == 0 work, added spark debug.
 *
 * Revision 3.64  1996/11/26  16:48:05  geraci
 * debug version
 *
 * Revision 3.63  1996/11/25  23:02:18  petrick
 * Minor cleanup.
 *
 * Revision 3.62  1996/11/25  16:24:12  gray
 * Turn car body reflections off for tunnel2.
 *
 * Revision 3.61  1996/11/25  15:53:34  geraci
 * tunnel 2 supported
 *
 * Revision 3.60  1996/11/25  14:36:17  gray
 * New grey tunnel reflection.
 *
 * Revision 3.59  1996/11/25  14:02:28  petrick
 * Turned off skid poly shortening code to stop objects from vanishing.
 *
 * Revision 3.58  1996/11/24  10:48:38  petrick
 * Fixes the random object loss bug.
 *
 * Revision 3.57  1996/11/24  09:43:20  gray
 * switch 10 turns reflections off, body reflection on window (debug).
 *
 * Revision 3.56  1996/11/24  06:18:35  petrick
 * Removed flag woman, added flags.  Also added some debug code to check car model numbers.
 *
 * Revision 3.55  1996/11/23  03:00:34  geraci
 * kill tail end of skid in air
 *
 * Revision 3.54  1996/11/22  12:44:57  petrick
 * Added some comments to DoSkids.
 *
 * Revision 3.53  1996/11/22  05:09:41  geraci
 * hide shadow of resurrecting car
 *
 * Revision 3.52  1996/11/21  18:05:05  gray
 * Made hood non-reflection. Liquid metal on debug switch 10.
 *
 * Revision 3.51  1996/11/21  03:15:40  geraci
 * tweaked view 2 rumble
 *
 * Revision 3.50  1996/11/20  12:28:34  gray
 * Scale shadow for each body type. Limit visual tire compression to 0.5 ft.
 *
 * Revision 3.49  1996/11/19  21:48:01  gray
 * Make LOWCARS the only game in town.
 *
 * Revision 3.48  1996/11/19  17:47:43  petrick
 * Deactivated sparks during demo game.
 *
 * Revision 3.47  1996/11/19  12:59:24  gray
 * New reflect stuff.
 *
 * Revision 3.46  1996/11/15  16:21:04  petrick
 * Makes sure no fog in select, smoke moves different, Frame reflect flag set explicitly.
 *
 * Revision 3.45  1996/11/14  14:40:23  gray
 * Made reflection default OFF.
 *
 * Revision 3.44  1996/11/10  14:08:26  petrick
 * Fog now updated in game task.
 *
 * Revision 3.43  1996/11/09  11:13:50  gray
 * Fixed frame-to-hood reflect crash. Turn reflect off with switch 10 up.
 *
 * Revision 3.42  1996/11/09  08:52:01  petrick
 * Added flag girl objs.
 *
 * Revision 3.41  1996/11/08  11:08:10  petrick
 * Turn on/off car body flag for hood parts.
 *
 * Revision 3.40  1996/11/07  08:18:24  gray
 * Car shading and reflection stuff.
 *
 * Revision 3.39  1996/11/07  00:40:16  petrick
 * Hood no longer appears in car select.
 *
 * Revision 3.38  1996/11/05  01:32:01  petrick
 * Fixed non-hidden hood when switching from view 2 to view 1.
 *
 * Revision 3.37  1996/11/03  05:57:44  geraci
 * split up debug switches and regular game switches
 *
 * Revision 3.36  1996/11/02  10:57:35  petrick
 * Crowds are supposed to have 4 frames each.
 *
 * Revision 3.35  1996/11/02  10:49:14  petrick
 * Crowds version 2.
 *
 * Revision 3.34  1996/11/02  00:13:57  geraci
 * defines for human and drone types
 *
 * Revision 3.33  1996/11/01  23:27:46  petrick
 * Added crowds.
 *
 * Revision 3.32  1996/10/25  05:46:41  geraci
 * make car data all caps
 *
 * Revision 3.31  1996/10/25  01:29:32  petrick
 * Added XYZ arrows back in.
 *
 * Revision 3.30  1996/10/24  09:27:21  petrick
 * Added multiple tree types.
 *
 * Revision 3.29  1996/10/23  00:24:25  petrick
 * Made track_no a global.
 *
 * Revision 3.28  1996/10/22  10:35:40  petrick
 * Knockdown fence added to track 1.
 *
 * Revision 3.27  1996/10/16  09:40:43  petrick
 * Window texture changes if in tunnel.
 *
 * Revision 3.26  1996/10/12  11:25:34  petrick
 * Got rid of object-based markers.
 *
 * Revision 3.25  1996/10/11  10:10:56  petrick
 * Added marker and no damage flags, supressed shadow in views 1 & 2.
 *
 * Revision 3.24  1996/10/10  05:12:22  petrick
 * Made irqtime a true global in globals_h.
 *
 * Revision 3.23  1996/10/09  05:48:17  petrick
 * Cleanup.
 *
 * Revision 3.22  1996/10/05  10:25:14  petrick
 * Made Camaro shadow longer.
 *
 * Revision 3.21  1996/10/04  10:42:27  petrick
 * Tied smoke to frame rate, made shadow fade with height from ground.
 *
 * Revision 3.20  1996/10/03  09:11:05  petrick
 * Made shadow a better size, turned off smoke if view < 3.
 *
 * Revision 3.19  1996/10/02  21:21:50  petrick
 * Added more frames to pole knockdown targets.
 *
 * Revision 3.18  1996/10/02  03:37:18  petrick
 * Shrunk shadow, cleaned up, splash work.
 *
 * Revision 3.17  1996/09/28  01:18:06  gray
 * Removed unused extern func references.
 *
 * Revision 3.16  1996/09/23  09:58:05  petrick
 * Made sure sparks are hid when not on.
 *
 * Revision 3.15  1996/09/23  08:57:24  petrick
 * Put in limits on spark retexturing.
 *
 * Revision 3.14  1996/09/21  04:54:20  petrick
 * Added splash.
 *
 * Revision 3.13  1996/09/18  10:18:14  geraci
 * added code for view2 car shake
 *
 * Revision 3.12  1996/09/17  21:36:38  petrick
 * 2nd half of hood now flickers as it should when aborting in view 2.
 *
 * Revision 3.11  1996/09/17  15:07:23  petrick
 * Got rid of some stuff from the hud and cleaned up more.
 *
 * Revision 3.10  1996/09/17  08:52:51  geraci
 * skids and smoke now can be tied to volume level
 *
 * Revision 3.9  1996/09/17  07:29:00  petrick
 * Temporary check-in.
 *
 * Revision 3.8  1996/09/17  05:09:01  petrick
 * Skids fade by volume.
 *
 * Revision 3.7  1996/09/16  06:12:28  gray
 * New CAMERAFACE flag for smoke objects.
 *
 * Revision 3.6  1996/09/16  03:31:42  petrick
 * Moved smoke up to ground height.
 *
 * Revision 3.5  1996/09/14  08:41:32  petrick
 * Cleanup.
 *
 * Revision 3.4  1996/09/14  01:05:10  petrick
 * Checked in a moment too soon...
 *
 * Revision 3.3  1996/09/14  00:41:48  petrick
 * Skid extending works right.  Xlu changes for resurrecting cars.
 *
 * Revision 3.2  1996/09/12  10:52:05  petrick
 * New skid marks.
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 *	======================================================================
 */

#include <string.h>
#include "modeldat.h"
#include "globals.h"
#include "dis.h"
#include "mb.h"
#include "debug.h"
#include "os_proto.h"
#include "road.h"
#include "checkpoint.h"
#include "visuals.h"
#include "carsnd.h"

#include "Pro/vecmath.pro"
#include "Pro/unitvecs.pro"
#include "Pro/mdrive.pro"

/*===============================  DEFINES  =================================*/

#define DEBUG_SPARKS    0				/*  1== Turn on spark debug code. */

#define MAX_VISUALS		100				/*	Maximum concurrent environmental visuals. */
#define HULK_FRAME 		7				/*	Frame # in blast when car turns to hulk. */
#define BLAST_FRAMES	16				/*	Number of frames in blast sequence. */
#define BLAST_HOLD		40				/*	msecs to hold each frame of blast animation. */
#define SMOKE_FRAMES	16				/*	Number of frames in smoke sequence. */
#define SMOKE_OBJS		3*SMOKE_FRAMES	/*	Number of smokes to keep predefined/tire. */
#define SKID_OBJS		100				/*	Number of skids to keep predefined. */
#define SKID_DEVIATION 	1.0			/*	Max deviation from straight skid allowed. */

#define VIS_BIT			0x10			/*	Bit used in data field to indicate visibility. */
#define XLU_BIT			0x20			/*	Bit used in data field to indicate tranlucency. */
#define HULK_BIT		0x40			/*	Bit used in data field to indicate car is hulk. */
#define HOOD_BIT		0x80			/*	Bit used in data field to indicate car view 2. */
#define SHOW_ONLY		SHOW_ALL

typedef enum DynTextures 
{	
	TEX_SPARK1,TEX_SPARK2,TEX_SPARK3,TEX_SPARK4,TEX_SPARK5,TEX_SPARK6,
	TEX_SHADOW,
	TEX_SKID,
	TEX_FBALL00,TEX_FBALL01,TEX_FBALL02,TEX_FBALL03,TEX_FBALL04,TEX_FBALL05,
	TEX_FBALL06,TEX_FBALL07,TEX_FBALL08,TEX_FBALL09,TEX_FBALL10,TEX_FBALL11,
	TEX_FBALL12,TEX_FBALL13,TEX_FBALL14,TEX_FBALL15,TEX_FBALL16,TEX_FBALL17,
	TEX_FBALL18,TEX_FBALL19,TEX_FBALL20,TEX_FBALL21,TEX_FBALL22,TEX_FBALL23,
	TEX_BCLOUD,TEX_WCLOUD,TEX_BTUNNEL,TEX_WTUNNEL,TEX_BTUNNEL2,TEX_WTUNNEL2,
	TEX_WCLOUDA,
	NUM_DYN_TEXS 
} DynTexture;

static const char * const TexNames[NUM_DYN_TEXS] = 
{ 
	"SPARK01A", "SPARK02A", "SPARK03A", "SPARK04A", "SPARK05A", "SPARK06A", 
	"SHADBLUR",
	"SKID04",	/* skid texture */
	"FBALL00",  "FBALL01",	"FBALL02",	"FBALL03",	"FBALL04",	"FBALL05",
	"FBALL06",	"FBALL08",	"FBALL09",	"FBALL10",	"FBALL12",	"FBALL14",	
	"FBALL15",	"FBALL16",	"FBALL18",	"FBALL20",	"FBALL21",	"FBALL22",
	"FBALL24",	"FBALL25",	"FBALL26",	"FBALL27",	"FBALL28",	"FBALL30",
	"REF_BCLOUD","REF_WCLOUD","REF_BTUNNEL","REF_WTUNNEL","REF_BTUNNEL2","REF_WTUNNEL2",
	"REF_WCLOUDA"
};


/*==============================  PROTOTYPES  ===============================*/

static void HandleASpark( Visual *v, BOOL on, F32 x, F32 y, F32 z, S32 name, S16 op);
static S32  PlaceAnObj(S32 objnum, F32 v[3]);
static S32  PlaceChildObj(S32 objnum,F32 dx,F32 dy,F32 dz,F32 *orient,S32 parent);
static void StartSmoke( S16 slot, U32 tire, BOOL fast );
static void StartMarker( S16 slot );
static void StartBrakeLights(S16 slot);
static void StartTire( S16 slot, U32 tire );
static void StartShadow( S16 slot );
static void StartQuad( S16 slot, S16 corner );
static void StartWindow( S16 slot, S16 corner );
static void PositionObject( S32 objnum, F32 x, F32 y, F32 z, F32 u, F32 v, F32 w );
static void RemoveVisuals( S16 slot );
static BOOL CheckVisible( Visual *v, BOOL on );
static BOOL CheckXlu( Visual *v, BOOL lucent, S32 xlu);
static S32 	InitChildObjFlags(S32 objnum, S32 parent, U32 flags);
static void DoSkid(S16 slot, U32 tire, BOOL on);
static void AnimateSkids(void);
static Skid *GetSkid(void);
static void AddSkid(Skid *s);
static void ReleaseSkid(Skid *s);
static void StartSkid(NewSkid *ns, S16 slot, U32 tire);
static void ContinueSkid(NewSkid *ns, S16 slot, U32 tire);
static void StopSkid(NewSkid *ns);
static void UpdateSkid(NewSkid *ns, S16 slot, U32 tire);

static VisFunc	AnimateBlast, AnimateBrakeLights, AnimateLSpark, AnimateRSpark,
				AnimateBSpark, AnimateSmoke, AnimateMarker,
				AnimateTire, AnimateShadow, AnimateFrame, AnimateQuad, AnimateWindow; 

extern void SetTargets(void);


/*==============================  TYPEDEFS  =================================*/

typedef struct CarParts
{
	S32	frame;
	S32	hood;
	S32	brakes;
	S32	blast;
	S32	window[4];
	S32 tires[4];
	S32 quad[5];
	S32	smoke[SMOKE_OBJS];
	S8	nextSmoke;
	S32	lSpark, rSpark, bSpark;
} CarParts;

/*===============================  GLOBALS  =================================*/

static NewSkid 	gNewSkid[MAX_LINKS][4];			/* Holds info about skids being created. */
static Skid	gSkid[SKID_OBJS];			/* Skid pool. */
static Skid	*gSkidFree;				/* Points to list of available skids. */
static Skid	*gSkidList;				/* Points to list of previously created skids. */
static Visual	gEnvirons[MAX_VISUALS];			/*	Environmental visual effects list. */
static Visual	*gEnvList,*gFreeVis;			/*	List of used & free fx slots. */
static S32 		gTexList[NUM_DYN_TEXS];			/*	List of dynamic texture maps. */
static CarParts	gCarParts[NCARS];				/*	List of pre-created objects on the car. */
static BOOL		gGoodFrameRate;					/*	Set true if instantaneous frame rate is OK. */

S16				gCarModelNums[NCARS];		/*	Model number of each car.  */
F32				spring_save;					/*  view2 tweak value */

const U8		gDamageShift[5] = { 28, 30, 24, 26, 22};
const U32		gDamageMask[5]  = { App_M_RF_DAMAGE, App_M_LF_DAMAGE, 
									App_M_RR_DAMAGE, App_M_LR_DAMAGE, App_M_TOP_DAMAGE};


/*===============================  EXTERNS  =================================*/

extern S8			view;				/*	Current camera view		camera.c */
extern const F32 	unituvs[3][3];
extern const F32 	zerovector[3];
extern snd_state 	scrch_state[MAX_LINKS][4];
extern S16			skid_intensity[MAX_LINKS][4][2];
extern S16			smoke_intensity[MAX_LINKS][4][2];
extern F32 			gGameExecTime;		/* Seconds per frame. */

/* ----------------------------------------------------------------------------- */
void InitDynamicObjs(BOOL for_game)
{	
	S32			i,j,k, model;
	char		partName[20];
	static const char * const ObjNames[NUM_DYN_OBJS] = 
	{
		"FRAME1", "WHEEL0", "WBLUR0", "BRAKES1",
		"D0_WD1", "D0_WF1", "D0_WP1", "D0_WB1",
		"H0_L",   "H0_R",   "H1_L",   "H1_R",    "H2_L",   "H2_R",
		"D0_FR1", "D1_FR1", "D2_FR1", "D0_FL1",  "D1_FL1", "D2_FL1",
		"D0_RR1", "D1_RR1", "D2_RR1", "D0_RL1",  "D1_RL1", "D2_RL1", 
		"D0_TOP1","D1_TOP1","D2_TOP1",

		"FRAME1", "WHEEL0", "WBLUR0", "BRAKES1",
		"D0_WD1", "D0_WF1", "D0_WP1", "D0_WB1",
		"H0_L",   "H0_R",   "H1_L",   "H1_R",    "H2_L",   "H2_R",
		"D0_FR1", "D1_FR1", "D2_FR1", "D0_FL1",  "D1_FL1", "D2_FL1",
		"D0_RR1", "D1_RR1", "D2_RR1", "D0_RL1",  "D1_RL1", "D2_RL1", 
		"D0_TOP1","D1_TOP1","D2_TOP1",

		"FRAME1", "WHEEL0", "WBLUR0", "BRAKES1",
		"D0_WD1", "D0_WF1", "D0_WP1", "D0_WB1",
		"H0_L",   "H0_R",   "H1_L",   "H1_R",    "H2_L",   "H2_R",
		"D0_FR1", "D1_FR1", "D2_FR1", "D0_FL1",  "D1_FL1", "D2_FL1",
		"D0_RR1", "D1_RR1", "D2_RR1", "D0_RL1",  "D1_RL1", "D2_RL1", 
		"D0_TOP1","D1_TOP1","D2_TOP1",

		"FRAME1", "WHEEL0", "WBLUR0", "BRAKES1",
		"D0_WD1", "D0_WF1", "D0_WP1", "D0_WB1",
		"H0_L",   "H0_R",   "H1_L",   "H1_R",    "H2_L",   "H2_R",
		"D0_FR1", "D1_FR1", "D2_FR1", "D0_FL1",  "D1_FL1", "D2_FL1",
		"D0_RR1", "D1_RR1", "D2_RR1", "D0_RL1",  "D1_RL1", "D2_RL1", 
		"D0_TOP1","D1_TOP1","D2_TOP1",

		"FRAME1", "WHEEL0", "WBLUR0", "BRAKES1",
		"D0_WD1", "D0_WF1", "D0_WP1", "D0_WB1",
		"H0_L",   "H0_R",   "H1_L",   "H1_R",    "H2_L",   "H2_R",
		"D0_FR1", "D1_FR1", "D2_FR1", "D0_FL1",  "D1_FL1", "D2_FL1",
		"D0_RR1", "D1_RR1", "D2_RR1", "D0_RL1",  "D1_RL1", "D2_RL1", 
		"D0_TOP1","D1_TOP1","D2_TOP1",

		"FRAME1", "WHEEL0", "WBLUR0", "BRAKES1",
		"D0_WD1", "D0_WF1", "D0_WP1", "D0_WB1",
		"H0_L",   "H0_R",   "H1_L",   "H1_R",    "H2_L",   "H2_R",
		"D0_FR1", "D1_FR1", "D2_FR1", "D0_FL1",  "D1_FL1", "D2_FL1",
		"D0_RR1", "D1_RR1", "D2_RR1", "D0_RL1",  "D1_RL1", "D2_RL1", 
		"D0_TOP1","D1_TOP1","D2_TOP1",

		"FRAME1", "WHEEL0", "WBLUR0", "BRAKES1",
		"D0_WD1", "D0_WF1", "D0_WP1", "D0_WB1",
		"H0_L",   "H0_R",   "H1_L",   "H1_R",    "H2_L",   "H2_R",
		"D0_FR1", "D1_FR1", "D2_FR1", "D0_FL1",  "D1_FL1", "D2_FL1",
		"D0_RR1", "D1_RR1", "D2_RR1", "D0_RL1",  "D1_RL1", "D2_RL1", 
		"D0_TOP1","D1_TOP1","D2_TOP1",

		"FRAME1", "WHEEL0", "WBLUR0", "BRAKES1",
		"D0_WD1", "D0_WF1", "D0_WP1", "D0_WB1",
		"H0_L",   "H0_R",   "H1_L",   "H1_R",    "H2_L",   "H2_R",
		"D0_FR1", "D1_FR1", "D2_FR1", "D0_FL1",  "D1_FL1", "D2_FL1",
		"D0_RR1", "D1_RR1", "D2_RR1", "D0_RL1",  "D1_RL1", "D2_RL1", 
		"D0_TOP1","D1_TOP1","D2_TOP1",

		"FRAME1", "WHEEL0", "WBLUR0", "BRAKES1",
		"D0_WD1", "D0_WF1", "D0_WP1", "D0_WB1",
		"H0_L",   "H0_R",   "H1_L",   "H1_R",    "H2_L",   "H2_R",
		"D0_FR1", "D1_FR1", "D2_FR1", "D0_FL1",  "D1_FL1", "D2_FL1",
		"D0_RR1", "D1_RR1", "D2_RR1", "D0_RL1",  "D1_RL1", "D2_RL1", 
		"D0_TOP1","D1_TOP1","D2_TOP1",

		"FRAME1", "WHEEL0", "WBLUR0", "BRAKES1",
		"D0_WD1", "D0_WF1", "D0_WP1", "D0_WB1",
		"H0_L",   "H0_R",   "H1_L",   "H1_R",    "H2_L",   "H2_R",
		"D0_FR1", "D1_FR1", "D2_FR1", "D0_FL1",  "D1_FL1", "D2_FL1",
		"D0_RR1", "D1_RR1", "D2_RR1", "D0_RL1",  "D1_RL1", "D2_RL1", 
		"D0_TOP1","D1_TOP1","D2_TOP1",

		"FRAME1", "WHEEL0", "WBLUR0", "BRAKES1",
		"D0_WD1", "D0_WF1", "D0_WP1", "D0_WB1",
		"H0_L",   "H0_R",   "H1_L",   "H1_R",    "H2_L",   "H2_R",
		"D0_FR1", "D1_FR1", "D2_FR1", "D0_FL1",  "D1_FL1", "D2_FL1",
		"D0_RR1", "D1_RR1", "D2_RR1", "D0_RL1",  "D1_RL1", "D2_RL1", 
		"D0_TOP1","D1_TOP1","D2_TOP1",

		"FRAME1", "WHEEL0", "WBLUR0", "BRAKES1",
		"D0_WD1", "D0_WF1", "D0_WP1", "D0_WB1",
		"H0_L",   "H0_R",   "H1_L",   "H1_R",    "H2_L",   "H2_R",
		"D0_FR1", "D1_FR1", "D2_FR1", "D0_FL1",  "D1_FL1", "D2_FL1",
		"D0_RR1", "D1_RR1", "D2_RR1", "D0_RL1",  "D1_RL1", "D2_RL1", 
		"D0_TOP1","D1_TOP1","D2_TOP1",

		"HULKL1",
		"FINISH2O1",  "CHECKPOINTL1",
		"AAANULLOBJ",  "POST2REDO1", "POST2GREENO1","POST2BLUEO1", "MGUVARROWS", 
		"CARBLAST1",  "CARBLAST2",  "CARBLAST3",  "CARBLAST4",  "CARBLAST5",
		"CARBLAST6",  "CARBLAST7",  "CARBLAST8",  "CARBLAST9",  "CARBLAST10",
		"CARBLAST11", "CARBLAST12", "CARBLAST13", "CARBLAST14", "CARBLAST15",
		"SPARKO3",
		"SMOKE1", "SMOKE2", "SMOKE3", "SMOKE4",
		"SMOKE5", "SMOKE6", "SMOKE7", "SMOKE8",
		"SMOKE9", "SMOKE10","SMOKE11","SMOKE12",
		"SMOKE13","SMOKE14","SMOKE15","SMOKE16",
		"SMOKE_FAST1", "SMOKE_FAST2", "SMOKE_FAST3", "SMOKE_FAST4",	/* Fast smoke */
		"SMOKE_FAST5", "SMOKE_FAST6", "SMOKE_FAST7", "SMOKE_FAST8",
		"SMOKE_FAST9", "SMOKE_FAST10","SMOKE_FAST11","SMOKE_FAST12",
		"SMOKE_FAST13","SMOKE_FAST14","SMOKE_FAST15","SMOKE_FAST16",
		"SMOKE_DARK1", "SMOKE_DARK2", "SMOKE_DARK3", "SMOKE_DARK4",
		"SMOKE_DARK5", "SMOKE_DARK6", "SMOKE_DARK7", "SMOKE_DARK8",
		"SMOKE_DARK9", "SMOKE_DARK10","SMOKE_DARK11","SMOKE_DARK12",
		"SMOKE_DARK13","SMOKE_DARK14","SMOKE_DARK15","SMOKE_DARK16",
		"DUST1", "DUST2", "DUST3", "DUST4",
		"DUST5", "DUST6", "DUST7", "DUST8",
		"DUST9", "DUST10","DUST11","DUST12",
		"DUST13","DUST14","DUST15","DUST16",
		"SPLASH1", "SPLASH2", "SPLASH3", "SPLASH4",
		"SPLASH5", "SPLASH6", "SPLASH7", "SPLASH8",
		"SPLASH9", "SPLASH10","SPLASH11","SPLASH12",
		"SPLASH13","SPLASH14","SPLASH15","SPLASH16",
	};

	/*	These are track specific, and so might not all be there. */
	static const char * const TrkNames[NUM_TRK_OBJS] = 
	{	
		"CONE1L1", "CONE2L1",
		"METERL1", "METERL2", "METERL3", "METERL4", "METERL5",
		"TMUNCHL1","TMUNCHL2","TMUNCHL3","TMUNCHL4","TMUNCHL5","TMUNCHL6",
		"TREEHIT1L1","TREEHIT1L2","TREEHIT1L3",
		"TREEHIT2L1","TREEHIT2L2","TREEHIT2L3",
		"TREEHIT3L1","TREEHIT3L2","TREEHIT3L3",
		"TREEHIT4L1","TREEHIT4L2","TREEHIT4L3",
		"WINDOWAL1","WINDOWAL2","WINDOWAL3","WINDOWAL4","WINDOWAL5","WINDOWAL6","WINDOWAL7",
		"WINDOWBL1","WINDOWBL2","WINDOWBL3","WINDOWBL4","WINDOWBL5","WINDOWBL6","WINDOWBL7",
		"PMUNCH_01L1", "PMUNCH_01L2",
		"PMUNCH_02L1", "PMUNCH_02L2",
		"PMUNCH_03L1", "PMUNCH_03L2",
		"FENCEL1","FENCEL2","FENCEL3","FENCEL4", "FENCEL5", "FENCEL6",
		"FENCEL7","FENCEL8","FENCEL9","FENCEL10","FENCEL11","FENCEL12",
		"CROWD0L1","CROWD0L2","CROWD0L3","CROWD0L4",
		"CROWD1L1","CROWD1L2","CROWD1L3","CROWD1L4",
		"CROWD2L1","CROWD2L2","CROWD2L3","CROWD2L4",
		"FLAG1AL9","FLAG1AL8","FLAG1AL7","FLAG1AL6","FLAG1AL5",
		"FLAG1AL4","FLAG1AL3","FLAG1AL2","FLAG1AL1","FLAG1AL0", 
		"FLAG2L2", "FLAG2L1", "FLAG2L0", "FLAG2L9", "FLAG2L8", 
		"FLAG2L7", "FLAG2L6", "FLAG2L5", "FLAG2L4", "FLAG2L3",  
		"FLAG3L6", "FLAG3L5", "FLAG3L4", "FLAG3L3", "FLAG3L2", 
		"FLAG3L1", "FLAG3L0", "FLAG3L9", "FLAG3L8", "FLAG3L7", 
	};

	/* just setup car stuff for select. */
	if(!for_game)
		{
		for (k=i=0; i<NCARS; ++i)
			for (j=0; j < CAR_PARTS; ++j)
				{
				sprintf( partName, "TEMP%s", ObjNames[k] );
				gObjList[k++] = MBOX_FindObject_Sub( partName, gCarModelNums[i],
								gCarModelNums[i], MBOX_FATAL );
				}

		for (i=OBJ_HULK; i<MAX_TRK_OBJS; ++i)
			gObjList[i] = 0;
		return;
		}

	/*	Handle car parts specially. */
	for (k=i=0; i<MAX_LINKS; ++i)
		for (j=0; j < CAR_PARTS; ++j)
			{
			sprintf( partName, "TEMP%s", ObjNames[k] );
			gObjList[k++] = MBOX_FindObject_Sub( partName, gCarModelNums[i],
							gCarModelNums[i], MBOX_FATAL );
			}

	for (i=OBJ_HULK; i<NUM_DYN_OBJS; ++i)
		gObjList[i] = MBOX_FindObject( ObjNames[i] );
	for (; i<MAX_TRK_OBJS; ++i)
		gObjList[i] = MBOX_FindObject( TrkNames[i-NUM_DYN_OBJS] );

	SetTargets();
}	/*	InitDynamicObjs */


/* ----------------------------------------------------------------------------- *
 *	Initializes Visual effects for the environment.
 *	for_game	= true if this is during game play and not a select screen.
 * ----------------------------------------------------------------------------- */
void InitVisuals(BOOL for_game)
{
	S32 		i, j;
	CarParts	*car;

	/*	Prelocate dynamic objects. */
	InitDynamicObjs(for_game);
	
	/*	Initialize the visual structs. */
	gFreeVis = &gEnvirons[0];
	for (i=0; i<MAX_VISUALS-1; ++i)
		{
		gEnvirons[i].next = &gEnvirons[i+1]; 
		gEnvirons[i].objnum = -1;
		} 
		
	gEnvirons[i].next = nil;
	gEnvirons[i].objnum = -1;
	gEnvList = nil;
	InitSkids();

	memset( (void *)gCarParts, 0, sizeof(gCarParts));
	memset( (void *)gTexList,  0, sizeof(gTexList));

	/*	Prelocate dynamic tex maps. */
	for (i=0; i<NUM_DYN_TEXS; ++i)
		MBOX_FindTexture( TexNames[i], &gTexList[i] );

	spring_save = 0;

}	/*	end InitVisuals */


/* ----------------------------------------------------------------------------- */
void SetCarReflection( S16 slot, S32 wintex, S32 bodtex, F32 speed )
{
	CarParts        *car = &gCarParts[slot];
	RomTex		*win_rtex,*bod_rtex;
	S8			in_tunnel = game_car[slot].in_tunnel;
	S16			j;
	U32			flags;

	win_rtex = ZOID_GetRomTex(gTexList[wintex]);
	bod_rtex = ZOID_GetRomTex(gTexList[bodtex]);
	if(dlevels & SW_DEBUG9)
		win_rtex = bod_rtex;
	
	ZOID_SetReflect(car->hood, bod_rtex, speed);
	ZOID_SetReflect(car->frame, bod_rtex, speed);	/* for when it becomes left hood */
	for (j=0; j < 4; ++j)
		{
		ZOID_SetReflect(car->quad[j], bod_rtex, speed);
		flags 		= ZOID_ObjectFlags(car->quad[j]);
		if(in_tunnel == 2)
			ZOID_SetObjectFlags(car->quad[j], flags & ~ZATR_CAR_BODY);
		else
			ZOID_SetObjectFlags(car->quad[j], flags | ZATR_CAR_BODY);
		ZOID_SetReflect(car->window[j], win_rtex, speed);
		}
	ZOID_SetReflect(car->quad[j], bod_rtex, speed);
	flags 		= ZOID_ObjectFlags(car->quad[j]);
	if(in_tunnel == 2)
		ZOID_SetObjectFlags(car->quad[j], flags & ~ZATR_CAR_BODY);
	else
		ZOID_SetObjectFlags(car->quad[j], flags | ZATR_CAR_BODY);


}	/* end SetCarReflection */


/* ----------------------------------------------------------------------------- */
void CreateCar( S16 slot, BOOL for_game )
{
	CarParts	*car;
	S16			j,offset;
	extern S32	gPadObj[];
	F32 		x,y,z;
	U32			refl_flag;

	if (!in(0,slot,NCARS-1))
		return;

	/* force proper reflections for car select. */
	if(!for_game)
		{
		in_tunnel = 2;
		game_car[slot].in_tunnel = 2;
	}

	/* turn car body reflection off if SW_DEBUG9 up. */
	refl_flag = ((dlevels & SW_DEBUG9) || !for_game) ? 0 : ZATR_CAR_BODY;

	car				= &gCarParts[slot];
	offset			= slot * CAR_PARTS;
	car->frame		= InitChildObj(gObjList[OBJ_CAR1BODY+offset], -1);
	car->hood		= InitChildObj(gObjList[OBJ_CAR1H0R+offset], car->frame);
	for (j=0; j < 4; ++j)
		{
		car->window[j]  = InitChildObjFlags(gObjList[OBJ_CAR1WIN0+offset+j], car->frame, ZATR_CAR_WINDOW); 
		car->quad[j]	= InitChildObjFlags(gObjList[OBJ_CAR1Q0D0+offset+j*MAX_DAMAGE], car->frame,ZATR_SORTALPHA|refl_flag); 
		car->tires[j]	= InitChildObjFlags(gObjList[OBJ_CAR1WHEEL+offset], car->frame,ZATR_SORTALPHA);
		}
	car->quad[j]		= InitChildObjFlags(gObjList[OBJ_CAR1Q4D0+offset], car->frame,ZATR_SORTALPHA|refl_flag); 
	car->brakes 	= InitChildObjFlags(gObjList[OBJ_CAR1BRAKE+offset], car->frame,ZATR_SORTALPHA);
	ZOID_SetSortOffset(car->brakes, -1.0);	/* make brakes draw after car */

	SetCarReflection(slot, TEX_WTUNNEL2, TEX_BTUNNEL2, 2.0);

	if (for_game)
		{
		car->lSpark		= InitChildObjFlags(gObjList[OBJ_SPARKS], car->frame,ZATR_SORTALPHA);
		car->rSpark		= InitChildObjFlags(gObjList[OBJ_SPARKS], car->frame,ZATR_SORTALPHA);
		car->bSpark		= InitChildObjFlags(gObjList[OBJ_SPARKS], car->frame,ZATR_SORTALPHA);
		}
}	/* end CreateCar */


/* ----------------------------------------------------------------------------- */
void SetupCar( S16 slot, S16 type, BOOL for_game )
{
	S16 i, body, offset;
	U32	damage, appear;
	S32	state;

	if (!in(0,slot,NCARS-1) || (!game_car[slot].data_valid && for_game))
		return;

	/*	Pick car body based on type and camera view (fed as part of type param). */
	offset 	= CAR_PARTS * slot;
	appear	= (for_game) ? game_car[slot].appearance : 0;
	if (!damage_flag)
		appear &= ~App_M_ANY_DAMAGE;
	ZOID_SetObjectDef( gCarParts[slot].frame, PickCar(slot, type));
	game_car[slot].objnum = gCarParts[slot].frame;
	if (for_game)
		ZOID_SetObjectDef( gCarParts[slot].blast, gObjList[OBJ_BLAST01]);

	/*	Customize car parts based on type. */
	ZOID_SetObjectDef( gCarParts[slot].brakes, gObjList[OBJ_CAR1BRAKE+offset]);
	ZOID_SetObjectDef( gCarParts[slot].hood,   gObjList[OBJ_CAR1H0R+offset]);
	for (i=0; i<4; ++i)
		{
		ZOID_SetObjectDef( gCarParts[slot].tires[i], gObjList[OBJ_CAR1WHEEL+offset]);
		damage = OBJ_CAR1Q0D0 + ((appear & gDamageMask[i]) >> gDamageShift[i]);
		ZOID_SetObjectDef( gCarParts[slot].quad[i], gObjList[damage+offset+i*MAX_DAMAGE]);
		ZOID_SetObjectDef( gCarParts[slot].window[i], gObjList[OBJ_CAR1WIN0+offset+i]);
		}
	damage = OBJ_CAR1Q4D0 + ((appear & gDamageMask[4]) >> gDamageShift[4]);
	ZOID_SetObjectDef( gCarParts[slot].quad[4], gObjList[damage+offset]);

	MBOX_ShowObject( gCarParts[slot].frame,  SHOW_EACHCHILD);
	MBOX_HideObject( gCarParts[slot].brakes, HIDE_ONLY);
	MBOX_HideObject( gCarParts[slot].hood,   HIDE_ONLY);
	if (!for_game)
		return;

	MBOX_HideObject( gCarParts[slot].blast,  HIDE_ONLY);
	MBOX_HideObject( gCarParts[slot].lSpark, HIDE_ONLY);
	MBOX_HideObject( gCarParts[slot].rSpark, HIDE_ONLY);
	MBOX_HideObject( gCarParts[slot].bSpark, HIDE_ONLY);

	/*	Turn off certain parts if this car is a hulk. */
	if (type == HULK_TYPE || (view < 3 && slot == this_node))
		{
		MBOX_HideObject( gCarParts[slot].brakes, HIDE_ONLY);
		for (i=0; i<4; ++i)
			{
			MBOX_HideObject( gCarParts[slot].tires[i], HIDE_ONLY);
			MBOX_HideObject( gCarParts[slot].quad[i], HIDE_ONLY);
			MBOX_HideObject( gCarParts[slot].window[i], HIDE_ONLY);
			}
		MBOX_HideObject( gCarParts[slot].quad[i], HIDE_ONLY);
		if (type == HULK_TYPE)
			{
			MBOX_HideObject( gCarParts[slot].hood, HIDE_ONLY);
			state = prc_set_ipl (INTS_OFF);
			model[slot].appearance    |= App_M_HULK;
			game_car[slot].appearance |= App_M_HULK;
			prc_set_ipl(state);
			}
		}
	if (type != HULK_TYPE && (game_car[slot].appearance & App_M_HULK))
		{
		state = prc_set_ipl (INTS_OFF);
		model[slot].appearance    &= ~(App_M_HULK | App_M_ANY_DAMAGE);
		game_car[slot].appearance &= ~(App_M_HULK | App_M_ANY_DAMAGE);
		prc_set_ipl(state);
		}
}	/*	end SetupCar */

    
/* ----------------------------------------------------------------------------- */
void StartCar( S16 slot )
{
	S16	j;

	StartFrame(slot);
	StartSparks(slot);
	StartBrakeLights(slot);

	for (j=0; j < 4; ++j)
		{
		StartTire(slot,j);
		StartQuad(slot,j);
		StartWindow(slot,j);
		}
	StartQuad(slot,4);
}	/* end StartCar */


/* ----------------------------------------------------------------------------- */
S32 PickCar( S16 slot, S16 type )
{
	S32 index, damage;

	if (type == HULK_TYPE)
		return gObjList[OBJ_HULK];

	/*	Hood view? */
	if ((slot == this_node) && (view < 3))
		{
		damage	= 0;
		if (damage_flag)
			damage	= (game_car[slot].appearance & gDamageMask[1]) >> gDamageShift[1];
		index   = OBJ_CAR1H0L + slot*CAR_PARTS + damage*2;
		return gObjList[index];
		}
 
	return gObjList[OBJ_CAR1BODY + CAR_PARTS*slot];
}	/* end PickCar */


/* ----------------------------------------------------------------------------- */
void RemoveCar( S16 slot )
{	
	if (game_car[slot].objnum >= 0)
		{
		/*	Kill off any visuals if they exist. */
		MBOX_HideObject( gCarParts[slot].frame, HIDE_EACHCHILD);
		RemoveVisuals(slot);
		}
}	/*	end RemoveCar */
   

/* ----------------------------------------------------------------------------- *
 *	Initiates environmental effects for the current world.
 * ----------------------------------------------------------------------------- */
void StartEnvironment( void )
{
	S16 i,j,k, offset;
	CarParts *car;

	/*	Init all non-translucent stuff first. */
	for (i=0; i<MAX_LINKS; ++i)
		if (model[i].in_game)
			{
			CreateCar(i, true);
			StartCar(i);
			}

	/*	Init all translucent stuff last to avoid sorting problems. */
	for (i=0; i<MAX_LINKS; ++i)
		if (model[i].in_game)
			{
			car = &gCarParts[i];
			StartShadow(i);

			for (k=0; k < SMOKE_OBJS; ++k)
				{
				car->smoke[k] = InitChildObjFlags(gObjList[OBJ_SMOKE01], -1,ZATR_SORTALPHA|ZATR_CAMERAFACE);
				MBOX_HideObject( car->smoke[k], HIDE_ONLY );
				}
			car->nextSmoke = 0;
			car->blast	= InitChildObjFlags(gObjList[OBJ_BLAST01], car->frame,ZATR_SORTALPHA);
			MBOX_HideObject( car->blast, HIDE_ONLY );
			game_car[i].collidable = 1;
			}

}	/*	end StartEnvironment */


/* ----------------------------------------------------------------------------- *
 *	Allocates an entry from the environment list.
 * ----------------------------------------------------------------------------- */
Visual *GrabEnvEntry(void)
{
	Visual 	*v;
	S32		state;

	/*	Skip if no visual entry are available. */
	state = prc_set_ipl (INTS_OFF);
	if (!gFreeVis)
		{
		prc_set_ipl (state);
		return nil;
		}

	/*	Grab an entry.  Fill it in. */
	v 			= gFreeVis;
	gFreeVis 	= gFreeVis->next;
	prc_set_ipl (state);

	v->next 	= nil;
	v->index 	= -1;
	v->objnum 	= -1;
	v->slot		= 0;
	v->data		= 0;
	v->timeStamp= 0;
	v->func		= nil;
	return v;
}	/* end GrabEnvEntry */


/* ----------------------------------------------------------------------------- *
 *	Allocates an entry from the environment list.
 * ----------------------------------------------------------------------------- */
void AddToEnvList(Visual *v)
{
	S32		state;

	/*	Skip if no visual entry are available. */
	state 		= prc_set_ipl (INTS_OFF);
	v->next		= gEnvList;
	gEnvList	= v;
	prc_set_ipl (state);
}	/* end AddToEnvList */


/* ----------------------------------------------------------------------------- *
 *	Updates Visual effects for a given car.
 * ----------------------------------------------------------------------------- */
void UpdateEnvirons(void)
{
	Visual	*v, *next;
	S32		state;

	state	= prc_set_ipl (INTS_OFF);
	v 	 	= gEnvList;
	next 	= (gEnvList) ? gEnvList->next : nil;
	prc_set_ipl (state);

	while (v)
		{
		if (v->func)
			(*v->func)(v,1);
		if ((v = next) != nil)
			next = v->next;
		}

	AnimateSkids();

	/* Check for fog table updates. */
	if (gstate == TRKSEL || gstate == CARSEL)
		ConstantFog( 0, 0 );
	else
		HandleFog();
}	/*	end UpdateEnvirons */


/* ----------------------------------------------------------------------------- *
 *	Frees up an environmental entry.
 * ----------------------------------------------------------------------------- */
void ReleaseEnviron(Visual *v, BOOL unlink)
{
	Visual	*p;
	S32		state;
		
	if (v->objnum >= 0)
		MBOX_HideObject( v->objnum, HIDE_ONLY);
	v->func 	= (void *)v->index = 0;
	v->objnum   = -1;

	/*	Remove from the used environment list. */
	state = prc_set_ipl (INTS_OFF);
	if (unlink)
		{
		if (v == gEnvList)
			gEnvList = gEnvList->next;
		else
			{
			/*	Find the previous link. */
			for (p = gEnvList; p && p->next != v; p = p->next)
				;

			/*	Remove from the list. */	
			if (p && p->next == v)
				p->next = v->next;
			}
		}

	/* Put back on the free list. */
	v->next = gFreeVis;
	gFreeVis = v;
	prc_set_ipl (state);
}	/* end ReleaseEnviron */
			

/* ----------------------------------------------------------------------------- *
 *	Removes Visual effects for a the environment.
 * ----------------------------------------------------------------------------- */
void RemoveEnvirons(void)
{
	while (gEnvList)
		ReleaseEnviron(gEnvList, true);

	InitSkids();
}	/* end RemoveEnvirons */

U32 dbgGCapp[MAX_LINKS];

/* ----------------------------------------------------------------------------- *
 *	Updates Visual effects for a given car.
 * ----------------------------------------------------------------------------- */
void UpdateVisuals( S16 slot )
{
	extern VU32	frames;
	CAR_DATA  	*car	= &game_car[slot];
	Visual 		*v 		= &car->visuals[0];
	S16			i, t;

	gGoodFrameRate = gGameExecTime < (1.0/20.0);	/* 20 fps */

	/*	Debug stuff to turn off the car body via switch. */
	if (dedges & SW_DEBUG6)
		MBOX_HideObject( car->objnum, HIDE_ONLY);
	else if (trailing_dedges & SW_DEBUG6)
		{
		MBOX_ShowObject( car->objnum, SHOW_EACHCHILD);	
		MBOX_HideObject( gCarParts[slot].blast, HIDE_ONLY);
		MBOX_HideObject( gCarParts[slot].brakes, HIDE_ONLY);
		}

#if 0
 if(slot == gThisNode)
	{
 	CMSG( 5+slot,2,((model[slot].we_control) 		 ? RED_PALB : 
 					 ((slot==gThisNode)       		 ? GRN_PALB : 
 					  ((model[slot].drone_type == HUMAN) ? CYN_PALB : YEL_PALB))),
		  "F:%8.1lf %8.1lf %8.1lf %8.1lf",
		  model[slot].BODYFORCE[0][0],model[slot].BODYFORCE[1][0],
		  model[slot].BODYFORCE[2][0],model[slot].BODYFORCE[3][0] );
 	CMSG( 6+slot,2,GRN_PALB,
		  "F:%8.1lf %8.1lf %8.1lf %8.1lf",
		  model[slot].BODYFORCE[0][1],model[slot].BODYFORCE[1][1],
		  model[slot].BODYFORCE[2][1],model[slot].BODYFORCE[3][1] );
 	CMSG( 7+slot,2,GRN_PALB,
		  "F:%8.1lf %8.1lf %8.1lf %8.1lf",
		  model[slot].BODYFORCE[0][2],model[slot].BODYFORCE[1][2],
		  model[slot].BODYFORCE[2][2],model[slot].BODYFORCE[3][2] );
	}
#endif
	for (i=0; i<NUM_VISUALS; ++i,++v)
		if (v->func)
			(*v->func)(v,1);

dbgGCapp[slot] = car->appearance;

	/*	Check if car is smoking. */
	if ((car->appearance & App_M_SMOKE) && gGoodFrameRate)
		StartSmoke( slot, 4, false );

	if (gGoodFrameRate)
		{
		t = frames % 4;
		if ((car->appearance & App_M_RF_SMOKE) || model[slot].sviscode[0] == WATER)
			StartSmoke( slot, 0, (t != 2));
		if ((car->appearance & App_M_LF_SMOKE) || model[slot].sviscode[1] == WATER)
			StartSmoke( slot, 1, (t != 0));
		if ((car->appearance & App_M_RR_SMOKE) || model[slot].sviscode[2] == WATER)
			StartSmoke( slot, 2, (t != 3));
		if ((car->appearance & App_M_LR_SMOKE) || model[slot].sviscode[3] == WATER)
			StartSmoke( slot, 3, (t != 1));
		}

	DoSkid( slot, 0, (car->appearance & App_M_RF_SKID) != 0 );
	DoSkid( slot, 1, (car->appearance & App_M_LF_SKID) != 0 );
	DoSkid( slot, 2, (car->appearance & App_M_RR_SKID) != 0 );
	DoSkid( slot, 3, (car->appearance & App_M_LR_SKID) != 0 );
}	/*	end UpdateVisuals */


/* ----------------------------------------------------------------------------- *
 *	Removes Visual effects for a given car.
 * ----------------------------------------------------------------------------- */
static void RemoveVisuals( S16 slot )
{
	Visual *v = &game_car[slot].visuals[0];
	S16		i;

	for (i=0; i<NUM_VISUALS; ++i,++v)
		if (v->func)
			(*v->func)(v,0);
		else if (v->objnum >= 0)
			{
			MBOX_HideObject( v->objnum, HIDE_ONLY);
			v->objnum = -1;
			}
}	/* end RemoveVisuals */


/* ----------------------------------------------------------------------------- *
 *	Initiates a Car quadrant/top animation.
 * ----------------------------------------------------------------------------- */
static void StartQuad( S16 slot, S16 quad )
{
	Visual *v 	= &game_car[slot].visuals[VIS_QUAD0+quad];

	v->func		= AnimateQuad;
	v->slot		= slot;
	v->data		= quad;
	v->objnum	= gCarParts[slot].quad[quad];

	MBOX_ShowObject( v->objnum, SHOW_ONLY);

}	/*	end StartQuad */


/* ----------------------------------------------------------------------------- *
 *	Makes frame animate.
 * ----------------------------------------------------------------------------- */
static void AnimateQuad(Visual *v, S16 op)
{
	BOOL		hulk, lucent;
	U32			flags, xlu;
	S16			damage, index, hid;
	S16			quad	= v->data & 0xf;
	S16 		slot	= v->slot;
	CAR_DATA	*car	= &game_car[slot];

	if (gstate == TRKSEL || gstate == CARSEL)
		return;

	damage 	= 0;
	if (damage_flag)
		damage	= (car->appearance & gDamageMask[quad]) >> gDamageShift[quad];
	hulk	= (car->appearance & App_M_HULK) != 0;
	lucent	= (car->appearance & App_M_TRANSLUCENT) != 0;

#if 0
	if (!CheckVisible(v, false))
		return;
#endif

	/*	Check if quad needs to be disabled or enabled. */
	if (op == 0)
		{
		if (v->objnum >= 0)
			MBOX_HideObject( v->objnum, HIDE_ONLY);
		v->func = nil; 
		v->objnum = -1;
		return;
		}

	/*	Change object based on damage. */
	if (((v->data>>8) & 0xF) != damage)
		{
		index = OBJ_CAR1Q0D0 + v->slot*CAR_PARTS + quad*MAX_DAMAGE + damage;
		ZOID_SetObjectDef( v->objnum, gObjList[index]);
		v->data = (damage << 8) | (v->data & 0xFF);
		}

	/*	Hide quads if a hulk or if part isn't visible in this camera view. */
	if (!CheckVisible(v, !hulk && (slot != this_node || view >= 3)))
		return;

	xlu = (slot == this_node || model[slot].resurrect.moving_state <= -1) ? 0x50 : 0;
	CheckXlu( v, lucent, xlu);
}	/*	end AnimateQuad */


/* ----------------------------------------------------------------------------- *
 *	Initiates a Car window animation.
 * ----------------------------------------------------------------------------- */
static void StartWindow( S16 slot, S16 quad )
{
	Visual *v 	= &game_car[slot].visuals[VIS_WINDOW0+quad];

	v->func		= AnimateWindow;
	v->slot		= slot;
	v->data		= quad;
	v->objnum	= gCarParts[slot].window[quad];

	MBOX_ShowObject(v->objnum, SHOW_ONLY);

}	/*	end StartWindow */


/* ----------------------------------------------------------------------------- *
 *	Makes windows animate.
 * ----------------------------------------------------------------------------- */
static void AnimateWindow(Visual *v, S16 op)
{
	BOOL		hulk, lucent;
	U32			xlu, lod;
	S32			idx;
	S16			damage, index, hid, offset;
	S16 		slot	= v->slot;
	CAR_DATA	*car	= &game_car[slot];
	S8			in_tunnel = car->in_tunnel;

	hulk	= (car->appearance & App_M_HULK) != 0;
	lucent	= (car->appearance & App_M_TRANSLUCENT) != 0;

	/*	Check if window needs to be disabled or enabled. */
	if (op == 0)
		{
		if (v->objnum >= 0)
			MBOX_HideObject( v->objnum, HIDE_ONLY);
		v->func = nil;
		v->objnum = -1;
		return;
		}

	/*	Hide window if a hulk or if part isn't visible in this camera view. */
	if (!CheckVisible(v, !hulk && (slot != this_node || view >= 3)))
		return;

	/*	Check if entering or leaving tunnel. */
	lod		= MBOX_ObjectLOD(v->objnum);
	offset	= OBJ_CAR1WIN0 + slot*CAR_PARTS + (v->data & 0xf);
	if (in_tunnel && (!(v->data & 0x10) || (lod<<8 != (v->data & 0xf00)) || (in_tunnel<<12 != (v->data & 0x3000))))
		{
		v->data &= ~0x3f10;
		v->data	|= 0x10 | lod << 8 | in_tunnel << 12;
		if(in_tunnel == 1)
			SetCarReflection(slot, TEX_WTUNNEL2, TEX_BTUNNEL2, 1.8);
		else
			SetCarReflection(slot, TEX_WTUNNEL, TEX_BTUNNEL, 1.8);
		}
	else if (!in_tunnel && ((v->data & 0x10) || (lod<<8 != (v->data & 0xf00))))
		{
		v->data &= ~0x3f10;
		v->data	|= lod << 8;
		if(trackno == 3)	/* special reflection for alcatraz. */
			SetCarReflection(slot, TEX_WCLOUDA, TEX_BCLOUD, 0.6);
		else
			SetCarReflection(slot, TEX_WCLOUD, TEX_BCLOUD, 0.6);
		}
	xlu = (slot == this_node || model[slot].resurrect.moving_state <= -1) ? 0x60 : 0;
	CheckXlu( v, lucent, xlu);
}	/*	end AnimateWindow */


/* ----------------------------------------------------------------------------- *
 *	Initiates a Car frame animation.
 * ----------------------------------------------------------------------------- */
void StartFrame( S16 slot )
{
	Visual *v 	= &game_car[slot].visuals[VIS_FRAME];

	v->func		= AnimateFrame;
	v->slot		= slot;
	v->data		= 0;
	v->objnum 	= gCarParts[slot].frame;

	MBOX_ShowObject( v->objnum, SHOW_ONLY);

}	/*	end StartFrame */


/* ----------------------------------------------------------------------------- *
 *	Makes frame animate.
 * ----------------------------------------------------------------------------- */
static void AnimateFrame(Visual *v, S16 op)
{
	BOOL		hulk, lucent, show;
	S16 		damage, index, 
		        slot	= v->slot;
	CAR_DATA	*car	= &game_car[slot];
	S32 		def 	= ObjInstanceList[v->objnum].index;
	U32			flags, hobj, xlu;

	if (gstate == TRKSEL || gstate == CARSEL)
		return;

	/*	Check if frame needs to be disabled. */
	if (op == 0)
		{
		if (v->objnum >= 0)
			MBOX_HideObject(v->objnum, HIDE_ONLY);
		v->func = nil;
		v->objnum = -1;
		return;
		}
#if 0
	if (!CheckVisible(v, false))
		return;
#endif

	hobj 	= gCarParts[slot].hood;
	flags 	= ZOID_ObjectFlags( hobj );
	if (!CheckVisible(v, (view != 1) || (slot != this_node)))
		{
		if (!(flags & ZATR_OFF))
			MBOX_HideObject( hobj, HIDE_ONLY );
		return;
		}

	hulk	= (car->appearance & App_M_HULK) != 0;
	lucent	= (car->appearance & App_M_TRANSLUCENT) != 0;
	if (hulk)
		{
		if (def != PickCar(slot, HULK_TYPE))
			ZOID_SetObjectDef( v->objnum, PickCar(slot, HULK_TYPE));
		if (!(flags & ZATR_OFF))
			MBOX_HideObject( hobj, HIDE_ONLY );
		return;
		}

	if (def != PickCar(slot, car->body_type))
		ZOID_SetObjectDef( v->objnum, PickCar(slot, car->body_type));

	/*	Handle second half of hood. */
	damage = 0;
	if (damage_flag)
		damage	= (car->appearance & gDamageMask[0]) >> gDamageShift[0];
	index   = OBJ_CAR1H0R + slot*CAR_PARTS + damage*2;
	def 	= ObjInstanceList[hobj].index;
	show	= (view == 2 && slot == this_node);
	if (def != gObjList[index])
		ZOID_SetObjectDef( hobj, gObjList[index]);
	ShowHood( slot, show );

	xlu = (slot == this_node || model[slot].resurrect.moving_state <= -1 ) ? 0x60 : 0;
	CheckXlu( v, lucent, xlu);
	if (!lucent)
		xlu = 0xFF;
	if (ObjInstanceList[hobj].xlucency != xlu)
		ZOID_SetObjectXlu(hobj, xlu);

}	/*	end AnimateFrame */


/* ----------------------------------------------------------------------------- *
 *	Shows or hides hood based on 'show'.
 * ----------------------------------------------------------------------------- */
void ShowHood(S16 slot, BOOL show)
{
	U32	hobj 	= gCarParts[slot].hood;
	U32	flags 	= ZOID_ObjectFlags( hobj );
	
	if ( !show && !(flags & ZATR_OFF))
		MBOX_HideObject( hobj, HIDE_ONLY ); 
	else if (show && (flags & ZATR_OFF))
		MBOX_ShowObject( hobj, SHOW_ONLY );

}	/*	end ShowHood */ 


/* ----------------------------------------------------------------------------- *
 *	Initiates a blast sequence.
 * ----------------------------------------------------------------------------- */
void StartBlast( S16 slot )
{
	U32 flags;
	Visual *v 	= &game_car[slot].visuals[VIS_BLAST];

	v->index 	= 1;
	v->objnum 	= gCarParts[slot].blast;
	v->func		= AnimateBlast;
	v->timeStamp= IRQTIME;
	v->slot		= slot;
	flags 		= ZOID_ObjectFlags(v->objnum);
	ZOID_SetObjectFlags(v->objnum, (flags & ~ZATR_NOZCOMP)|ZATR_SORTALPHA);

}	/*	end StartBlast */


/* ----------------------------------------------------------------------------- *
 *	Makes blast sequence animate.
 * ----------------------------------------------------------------------------- */
static void AnimateBlast(Visual *v, S16 op)
{
	S16 	slot, index, bIndex;
	char	str[20];
	U32 	flags;

	/*	Handle cleanup call. */
	if (op == 0)
		{
		/*	Remove the old blast. */
		if (v->objnum >= 0)
			MBOX_HideObject( v->objnum, HIDE_ONLY);
		v->func = (void *)v->index = 0;
		v->objnum = -1;
		return;
		}

	/*	Check if it's time to advance a frame. */
	slot	= v->slot;
	if (IRQTIME - v->timeStamp > BLAST_HOLD)
		{
		v->timeStamp = IRQTIME;

		/*	If it's time, replace the car with a hulk. */
		if (++v->index == HULK_FRAME)
			SetupCar( slot, HULK_TYPE, true );
		MBOX_ShowObject( v->objnum, SHOW_ONLY );

		/*	If it's the end of the sequence, clean up. */
		if (v->index == BLAST_FRAMES)
			{
			MBOX_HideObject( v->objnum, HIDE_ONLY);
			v->index = 0;
			v->func = 0;
			}

		/*	It's not the end, so advance a frame. */
		else 
			ZOID_SetObjectDef( v->objnum, gObjList[OBJ_BLAST01 + v->index - 1] );
		}					

}	/*	end AnimateBlast */


/* ----------------------------------------------------------------------------- *
 *TEMP - new mathbox needs transpose
 * ----------------------------------------------------------------------------- */
void Transpose3x3(F32 mat[3][3])
{
	F32 t;
	
	t 			= mat[0][1];
	mat[0][1]	= mat[1][0];
	mat[1][0]	= t;

	t 			= mat[0][2];
	mat[0][2]	= mat[2][0];
	mat[2][0]	= t;

	t 			= mat[2][1];
	mat[2][1]	= mat[1][2];
	mat[1][2]	= t;
}


/* ----------------------------------------------------------------------------- *
 *	Initiates a smoke/grass sequence.
 * ----------------------------------------------------------------------------- */
static void StartSmoke( S16 slot, U32 tire, BOOL fast )
{
	Visual		*v;
	CAR_DATA	*car;
	F32			pos[3];
	U32			flags, intensity;
	S16			surface = model[slot].sviscode[tire];
	S16			mph;
	U8			mult;

/*	if (dlevels & SW_DEBUG5) return; */

	/*	Skip if no visual entry are available. */
	car			= &game_car[slot];
	mph 		= (S16)car->mph;

	if (surface == AIR || (mph < 10 && tire != 4) || (slot == this_node && view < 3) )
		return;

	/*	Grab an entry.  Fill it in. */
	if (!(v = GrabEnvEntry()))
		return;

	v->index 	= 0;
	v->func		= AnimateSmoke;
	v->slot		= slot;
	v->timeStamp= IRQTIME;
	v->objnum	= gCarParts[slot].smoke[gCarParts[slot].nextSmoke];

	if (++gCarParts[slot].nextSmoke >= SMOKE_OBJS)
		gCarParts[slot].nextSmoke = 0;
	if (tire == 4)
		ZOID_SetObjectDef( v->objnum, gObjList[v->data = OBJ_BSMOKE01]);
	else if (surface == DIRT)
		ZOID_SetObjectDef( v->objnum, gObjList[v->data = OBJ_DUST01]);
	else if (surface == WATER)
		ZOID_SetObjectDef( v->objnum, gObjList[v->data = OBJ_SPLASH01]);
	else if (mph >= 60)
		ZOID_SetObjectDef( v->objnum, gObjList[v->data = OBJ_FSMOKE01]);
	else
		ZOID_SetObjectDef( v->objnum, gObjList[v->data = OBJ_SMOKE01]);

	mult = 0;
	if (tire == 4)
		if ((mult = 1+((IRQTIME - game_car[slot].crashtime) >> 10)) > 5)
			mult = 5;

	intensity = max(smoke_intensity[slot][tire][0], smoke_intensity[slot][tire][1]) & 0xff;
	v->data |= (fast << 16) | (tire << 17) | (mult << 20) | (intensity << 24);

	/*use dead reckoned data*/
	if (tire == 4)	/* Explosion smoke, center on car center. */
		veccopy(car->dr_pos, pos);
	else			/*	Tire smoke, center on specific tire. */
		{
		veccopy(car->dr_tirepos[tire], pos);
		pos[1]	+= model[slot].reckon.suscomp[tire];
		}

	MBOX_ShowObject( v->objnum, SHOW_ONLY );
	ZOID_UpdateObject(v->objnum,pos,unituvs);

	/*	Put it on the env list. */
	AddToEnvList(v);

}	/*	end StartSmoke */


/* ----------------------------------------------------------------------------- *
 *	Makes Smoke sequence animate.
 * ----------------------------------------------------------------------------- */
static void AnimateSmoke(Visual *v, S16 op)
{
	S16			base, fast, i, tire, offset, intensity;
	const S8	smokeMap[32] = {  0, 1, 2, 3, 4, 5, 6,23, 7, 8, 9,23,10,23,11,12,
								 13,23,14,23,15,16,17,23,18,19,20,21,22,23,23,23 };
	U32			flags, xlu; 
	F32			t;
	CAR_DATA	*gc = &game_car[v->slot];

	/*	Handle cleanup call. */
	if (op == 0)
		goto remove;

	fast		= (v->data >> 16) & 0x1;
	tire		= (v->data >> 17) & 0x7;
	offset		= (v->data >> 20) & 0xf;
	intensity	= (v->data >> 24) & 0xff;
	base		= (v->data & 0xffff);

	t = (F32)(IRQTIME - v->timeStamp) / (8.0 * ONE_SEC);
	v->timeStamp = IRQTIME;
	++v->index;	

	/*	If it's the end of the sequence, clean up. */
	if (tire == 4)
		{
		if (v->index >= SMOKE_FRAMES * 1.5)
			goto remove;

		ObjInstanceList[v->objnum].mat[3][0] += ZoidCamInfo[0].mat[2][0]*0.5 + Random(1.0);
		ObjInstanceList[v->objnum].mat[3][1] += ZoidCamInfo[0].mat[1][0]*0.5 + Random(1.0);
		ObjInstanceList[v->objnum].mat[3][2] += ZoidCamInfo[0].mat[0][0]*0.5;

		if ((offset *= v->index) >= 32)
			offset = 31;
		if (offset < 0)
			offset = 0;
		if (v->index >= SMOKE_FRAMES)
			base += SMOKE_FRAMES-1;
		else
			base += v->index;
		ZOID_ChangeTexture(gObjList[base], 0,-1,gTexList[TEX_FBALL00+smokeMap[offset]]);
		CheckXlu( v, false, 0xff);
		flags = ZOID_ObjectFlags( v->objnum );
		ZOID_SetObjectFlags(v->objnum, flags | ZATR_SORTALPHA|ZATR_CAMERAFACE);
		}
	else if (v->index >= SMOKE_FRAMES || (fast && v->index > 5))
		goto remove;
	else
		{
		base += v->index;
		/*	Fade smoke. */
		xlu = (t >= 1.0) ? 0 : ((intensity) * (1.0 - t) * .66);
		CheckXlu( v, true, xlu);
		}

	t *= 4.0;
	ObjInstanceList[v->objnum].mat[3][0] += gc->dr_vel[0] * t;
	ObjInstanceList[v->objnum].mat[3][1] += gc->dr_vel[1] * t;
	ObjInstanceList[v->objnum].mat[3][2] += gc->dr_vel[2] * t;

	/*	Replace the old smoke object. */
	ZOID_SetObjectDef( v->objnum, gObjList[base] );
	return;

remove:
	ReleaseEnviron(v,true);

}	/*	end AnimateSmoke */

S32 dbgSkidCnt[MAX_LINKS];

/* ----------------------------------------------------------------------------- *
 *	Handles initial creation of skid mark.
 * ----------------------------------------------------------------------------- */
static void DoSkid( S16 slot, U32 tire, BOOL on )
{
	BOOL	laston;
	MODELDAT	*m	 	= &model[slot];
	NewSkid *ns = &gNewSkid[slot][tire];

	on = on && (m->sviscode[tire] == PAVEMENT) && (m->resurrect.moving_state == -1);
	laston = (ns->skid != nil);

if(!laston && on)
 dbgSkidCnt[slot]++;

	if(!laston && on)
		StartSkid(ns, slot, tire);
	else if(laston && on)
		ContinueSkid(ns, slot, tire);
	else if(laston && !on)
		StopSkid(ns);
}

/* ----------------------------------------------------------------------------- *
 *	Handles fading of skids after creation.
 * ----------------------------------------------------------------------------- */
static void AnimateSkids(void)
{
	Skid *s, *nexts;
	F32 t, fadetime;
	U32 xlu;
	S32 numfree;

	/* Modify skid fade rate base on number of free skids. */
	for(numfree=0,s=gSkidFree; s; numfree++,s=s->next);
	fadetime = (2.0 * ONE_SEC) + (2.0 * ONE_SEC * (F32)numfree) / (F32)SKID_OBJS;

	for(s=gSkidList; s; )
		{
		/* fade rate = full to off in 4 seconds. */
/*		t = (F32)(IRQTIME - s->lastTime) / (4.0 * ONE_SEC);*/
		t = (F32)(IRQTIME - s->lastTime) / fadetime;
		xlu = (0xff - s->xlu) + (t * 255.0);
		if (xlu <= 255)
			{
			/*	Fade skid. */
			ZOID_UpdatePoly(s->objnum,0,-2,nil,-1,xlu);
			s = s->next;
			}
		else
			{
			nexts = s->next;
			ReleaseSkid(s);
			s = nexts;
			}
		}
}

static void StartSkid(NewSkid *ns, S16 slot, U32 tire)
{
	Skid	*s;

	s = GetSkid();
	if(s == nil)
		return;

	/* Initial NewSkid data */
	ns->skid = s;
	ns->lensq = 0.0;	/* dir[3] valid if lensq > 0.0 */

	/* Get current skid end. */
	UpdateSkid(ns, slot, tire);

	/* Copy poly end verts to poly start verts. */
	veccopy(ns->end, ns->start);
	veccopy(ns->vert[3], ns->vert[0]);
	veccopy(ns->vert[2], ns->vert[1]);
	ZOID_UpdatePoly(s->objnum, 0, -2, ns->vert, -1, 0xff - s->xlu);
}

static void ContinueSkid(NewSkid *ns, S16 slot, U32 tire)
{
	CAR_DATA	*car 	= &game_car[slot];
	Skid	*s = ns->skid;
	F32	vec[3], dirpos[3], lensq, len, ds, invlen;
	S32	i;

	/* Update skid to current tire pos. */
	UpdateSkid(ns, slot, tire);

	/* Calulate new skid length (square). */
	vecsub(car->dr_tirepos[tire], ns->start, vec);
	for(lensq=0.0,i=0;i<3;i++)
		lensq += vec[i] * vec[i];

	/* Check if a new skid is needed. */
	if(ns->lensq > 0.0)	/* Skid has non-zero length. */
		{
		len = magnitude(vec);
		scalmul(ns->dir, len, dirpos);
		vecsub(ns->end, dirpos, vec);	/* dist from dir line */
		for(ds=0.0,i=0;i<3;i++)
			ds += (vec[i] * vec[i]);
		/* If deviation too big or skid getting shorter, start a new skid. */
		if((ds > SKID_DEVIATION) || (lensq < ns->lensq))
			{
			StopSkid(ns);			/* Add current skid to skid list. */
			StartSkid(ns, slot, tire);	/* Start a new skid at the end. */
			}
		}
	else if(lensq > 0.1)	/* First non-zero length, set up deviation stuff. */
		{
		invlen = 1.0/sqrt(lensq);
		for(i=0;i<3;i++)
			ns->dir[i] = vec[i] * invlen;
		ns->lensq = lensq;
		}
}

static void StopSkid(NewSkid *ns)
{
	Skid	*s = ns->skid;
	S32 i;

	/* Get skid pos. */
	for(i=0;i<3;i++)
		s->pos[i] = (ns->start[i] + ns->end[i]) * 0.5;

	AddSkid(ns->skid);	/* Put newly created skid on skid list. */
	ns->skid = nil;
}

static void UpdateSkid(NewSkid *ns, S16 slot, U32 tire)
{
	CAR_DATA	*car 	= &game_car[slot];
	MODELDAT	*m	 	= &model[slot];
	Skid	*s = ns->skid;
	S16	i, other, cur_volume;
	F32	orth[3], dir[3], wid;

	other = tire + ((tire & 1) ? -1 : 1);
	vecsub( car->dr_tirepos[tire], car->dr_tirepos[other], orth );
	orth[1] += m->reckon.suscomp[tire] - m->reckon.suscomp[other];
	direction(orth, dir);
	for (i=0; i<3; ++i)
		{
		wid			= dir[i] * 0.75;
		ns->end[i]	= car->dr_tirepos[tire][i];
		ns->vert[2][i]	= ns->end[i] - wid;
		ns->vert[3][i]	= ns->end[i] + wid;
		}
	/* fudge above road surface. */
	ns->vert[2][1] += 0.8;
	ns->vert[3][1] += 0.8;

	if((slot == this_node) || (m->we_control))
		cur_volume = max(skid_intensity[slot][tire][0], skid_intensity[slot][tire][1]);
	else
		cur_volume = 0xff;	/* set to darkest skid for remote cars. */
/*	s->xlu = 0xff;*/
	s->xlu = ((cur_volume * cur_volume) >> 8) & 0xff;
	s->lastTime = IRQTIME;

	/* Update mathbox polygon. */
	ZOID_UpdatePoly(s->objnum, 0, -2, ns->vert, -1, 0xff - s->xlu);
}

void InitSkids(void)
{
	S32 i,j;
	Skid *s;

	/* Initialize NewSkid structures. */
	memset( (void *)gNewSkid,    0, sizeof(gNewSkid));

	/* Init skid free list. */
	for(s=gSkid,i=0;i<SKID_OBJS;i++,s++)
		{
		s->prev = s-1;
		s->next = s+1;
		s->lastTime = 0;
		s->xlu = 0;
		}
	gSkid[0].prev = nil;
	gSkid[SKID_OBJS-1].next = nil;
	gSkidFree = &gSkid[0];

	/* Init anim skids. */
	gSkidList = nil;
}

/* Get a new skid from free list or farthest skid from camera in gSkidList. */
static Skid *GetSkid(void)
{
	S32 i;
	Skid *s, *fars;
	F32 d, dsq, fardsq;
	F32 initvert[4][3] = {{ 1, 1, 0}, {-1, 1, 0}, {-1,-1, 0}, { 1,-1, 0}}; 

	/* All skids in initial creation. */
	/* (This can only happen if SKID_OBJS < MAX_LINKS*4) */
	if(!gSkidFree && !gSkidList)
		return nil;

	/* No free skids, release one from skid list (furthest from camera). */
	if(!gSkidFree)
		{
		/* Find skid furthest from camera. */
		fardsq = -1.0;
		fars = gSkidList;
		for(s=gSkidList;s->next;s=s->next)
			{
			for(dsq=0.0,i=0;i<3;i++)
				{
				d = s->pos[i] - gCamPos[i];
				dsq += (d * d);
				}
			if(dsq > fardsq)
				{
				fardsq = dsq;
				fars = s;
				}
			}
		s = fars;

		/* Remove from skid list and put on free list. */
		ReleaseSkid(s);
		}

	/* Remove from skid free list. */
	s = gSkidFree;
	gSkidFree = s->next;
	if(gSkidFree)
		gSkidFree->prev = nil;

	/* Break links. */
	s->prev = nil;
	s->next = nil;

	/* Get a mathbox poly for skid. */
	s->objnum = ZOID_NewPoly(4, gTexList[TEX_SKID], initvert, 0, 0xff);

	return s;
}

/* Add unlinked skid to gSkidList. */
static void AddSkid(Skid *s)
{
	Skid *fs;

	fs = gSkidList;
	s->next = fs;
	s->prev = nil;

	gSkidList = s;
	if(fs)
		fs->prev = s;
}

/* Release poly, unlink from gSkidList, add to gFreeList. */
static void ReleaseSkid(Skid *s)
{
	/* Remove skid polygon. */
	ZOID_RemovePoly(s->objnum);

	/* Remove from skid list. */
	if(s->prev)
		s->prev->next = s->next;
	else
		gSkidList = s->next;
	if(s->next)
		s->next->prev = s->prev;

	/* Add to free skid list. */
	s->next = gSkidFree;
	s->prev = nil;
	if(gSkidFree)
		gSkidFree->prev = s;
	gSkidFree = s;
}

/* ----------------------------------------------------------------------------- *
 *	Initiates a wheel.
 * ----------------------------------------------------------------------------- */
static void StartTire( S16 slot, U32 tire )
{
	CAR_DATA	*car = &game_car[slot];
	Visual		*v	 = &car->visuals[VIS_TIRE1+tire];

	v->func		= AnimateTire;
	v->slot		= slot;
	v->data		= tire;
	v->index	= 0;
	v->timeStamp= IRQTIME;
	v->objnum 	= gCarParts[slot].tires[tire];

}	/*	end StartTire */


/* ----------------------------------------------------------------------------- *
 *	Handles wheels.
 * ----------------------------------------------------------------------------- */
static void AnimateTire(Visual *v, S16 op)
{
	F32			pos[3],mat[3][3], rpy[3], spring, spring2, t, vel, lastpos;
	CAR_DATA  	*car;
	MODELDAT	*m;
	S16			tire, i,j, slot, tire_offset, xlu;
	BOOL		lucent;

	/*	Handle cleanup call. */
	if (op == 0)
		{
		MBOX_HideObject( v->objnum, HIDE_ONLY);
		v->func = (void *)v->index = v->data = 0;
		v->objnum = -1;
		return;
		}

	slot	= v->slot;
	car 	= &game_car[slot];
	tire	= v->data & 0xF;
	m		= &model[slot];
	spring 	= m->suscomp[tire];
	lucent	= (car->appearance & App_M_TRANSLUCENT) != 0;

	/*	Hide the tires if blast is visible. */
	if ( !CheckVisible(v, !(car->appearance & App_M_HULK) && (slot != this_node || view != 1) ))
		return;

	/* Add in some random bounce if on the dirt. */
	if (model[slot].sviscode[tire] == DIRT)
		{
		t = car->mph / 20.0;
		if (t > 0.6)
			t = 0.6;
		spring += Random(t);
		}

	if (spring < 0.0)
		spring = 0.0;
	else if (spring > 0.5)
		spring = 0.5;

	if (slot == this_node)
		{
		t = car->mph / 100.0;
		if (t > 1.5)
			t = 1.5;

		if (model[slot].sviscode[tire] == DIRT)
			spring2 = (Random(2.0)-1) * t;
		else if (model[slot].sviscode[tire] == AIR)
			spring2 = spring_save*.25*-.5;
		else
			spring2 = (Random(2.0)-1) * t * .1;

		spring_save = spring2 + spring_save * .75;
		if (spring_save < -4*.2)
			spring_save = -4*.2;
		}

	pos[0]	=  car->TIRER[tire][1];
	pos[1]	= -car->TIRER[tire][2] + m->tires[tire].tradius + spring;
	pos[2]	=  car->TIRER[tire][0];

	vel = car->tireW[tire];
	if (vel < 0.015 && vel > -0.015)
		vel = 0.0;
	lastpos = *((F32 *)&v->index);

	/* 2 == pitch, 1 == yaw, 0 == roll */
	if (tire & 1)
		{
		rpy[2] = 0.0;
		rpy[1] = (tire < 2) ? -m->steerangle : 0.0;
		rpy[0] = lastpos += vel * (IRQTIME - v->timeStamp)/1000.0;
		}
	else
		{
		rpy[2] = 3.1415926;
		rpy[1] = (tire < 2) ? m->steerangle : 0.0;
		rpy[0] = lastpos -= vel * (IRQTIME - v->timeStamp)/1000.0;
		}

	*((F32 *)&v->index) = lastpos;
	tire_offset = (vel < 64.0) ? OBJ_CAR1WHEEL : OBJ_CAR1BWHEEL;
	ZOID_SetObjectDef( v->objnum, gObjList[slot*CAR_PARTS + tire_offset] );
	v->timeStamp = IRQTIME;
	CreatePYRMatrix(mat, rpy);
	Transpose3x3(mat);

	xlu = (slot == this_node || m->resurrect.moving_state <= -1 ) ? 0x60 : 0;
	CheckXlu( v, lucent, xlu);
	ZOID_UpdateObject(v->objnum,pos,mat);

}	/*	end AnimateTire */


/* ----------------------------------------------------------------------------- *
 *	Initiates a Shadow.
 * ----------------------------------------------------------------------------- */
static void StartShadow( S16 slot )
{
	CAR_DATA	*car = &game_car[slot];
	Visual		*v	 = &car->visuals[VIS_SHADOW];
	S32			i,j;
	F32			pos[3],mat[3][3];
	const F32 	xyz[4][3] = {{ 1, 1, 0}, {-1, 1, 0}, {-1,-1, 0}, { 1,-1, 0}}; 

	v->func		= AnimateShadow;
	v->slot		= slot;
	v->timeStamp= IRQTIME;

	/*	Initial placement. */
	v->objnum = ZOID_NewPoly(4, gTexList[TEX_SHADOW], xyz, 0, 0x20);

}	/*	end StartShadow */

const F32 shadow_scale[NCARS][3] = {
/*       */		/*      SIDE    FRONT   BACK    */
/*BMW    */		{	0.17,	0.34,	0.35	},
/*INTEGRA*/		{	0.19,	0.34,	0.40	},
/*VETTE  */		{	0.19,	0.34,	0.40	},
/*BUGATTI*/		{	0.18,	0.41,	0.35	},
/*TAXI   */		{	0.19,	0.34,	0.40	},
/*VIPER  */		{	0.19,	0.33,	0.32	},
/*BUG    */		{	0.18,	0.32,	0.33	},
/*TRUCK  */		{	0.19,	0.34,	0.40	},
/*BUS    */		{	0.15,	0.35,	0.34	},
/*CAMERO */		{	0.17,	0.40,	0.43	},
/*SUPRA  */		{	0.16,	0.35,	0.40	},
/*CONCEPT*/		{	0.19,	0.34,	0.40	},
};

#define V3_SHADOW_OFFSET (0.7)
#define STD_SHADOW_OFFSET (0.5)

/* ----------------------------------------------------------------------------- *
 *	Handles shadows.
 * ----------------------------------------------------------------------------- */
static void AnimateShadow(Visual *v, S16 op)
{
	F32			pos[3],mat[3][3], xyz[4][3],dist[4], d1,d2;
	S16			i,whl;
	S32			xlu;
	CAR_DATA  	*car = &game_car[v->slot];
	MODELDAT	*m   = &model[v->slot];
	F32			wscale,fscale,bscale,len,offset;

	/*	Handle cleanup call. */
	if (op == 0)
		{
		if (v->objnum >= 0)
			ZOID_RemovePoly( v->objnum );
		v->func = (void *)v->index = 0;
		v->objnum = -1;
		return;
		}

	/* hide shadow if bad data or resurrecting. */
	/* check shadow diagonal vertical distance */
	d1 = m->reckon.airdist[0] - m->reckon.airdist[3];
	d2 = m->reckon.airdist[1] - m->reckon.airdist[2];
	if (abs(d1) > 8 || abs(d2) > 8 || (m->resurrect.moving_state > -1) ||
		(v->slot == this_node && view < 3) || m->hide_car)
		{
		ZOID_HidePoly(v->objnum);
		return;
		}

	for(d1=i=0;i<4;i++)
		d1+= (dist[i] = m->reckon.airdist[i]);
	d1 /= 4.0;

#if 1
	for(i=0;i<4;i++)
		{
		whl = (i < 2) ? i : 5 - i;	/* 0,1,3,2 */
		xyz[i][0] = car->dr_tirepos[whl][0];
		xyz[i][1] = car->dr_tirepos[whl][1];
		xyz[i][2] = car->dr_tirepos[whl][2];
		offset = (v->slot == this_node) ? V3_SHADOW_OFFSET - dist[whl] : STD_SHADOW_OFFSET - dist[whl];
		if(m->suscomp[whl] > 0.0)
			{
			/* offset in car up dir. */
			xyz[i][0] += (offset * car->dr_uvs[0][1]);
			xyz[i][1] += (offset * car->dr_uvs[1][1]);
			xyz[i][2] += (offset * car->dr_uvs[2][1]);
			}
		else	/* offset in world up dir. */
			xyz[i][1] +=  offset;
		}
#else
	for(i=0;i<4;i++)
		{
		whl = (i < 2) ? i : 5 - i;	/* 0,1,3,2 */
		xyz[i][0] = car->dr_tirepos[whl][0];
		xyz[i][1] = car->dr_tirepos[whl][1] - dist[whl] + 0.5;
		xyz[i][2] = car->dr_tirepos[whl][2];
		}
#endif

	/* scale shadow width */
	wscale = shadow_scale[m->body_type][0];
	for(i=0;i<3;i++)
		{
		len = xyz[0][i] - xyz[1][i];
		xyz[0][i] += (len * wscale);
		xyz[1][i] -= (len * wscale);

		len = xyz[3][i] - xyz[2][i];
		xyz[3][i] += (len * wscale);
		xyz[2][i] -= (len * wscale);
		}

	/* scale shadow front and back length */
	fscale = shadow_scale[m->body_type][1];
	bscale = shadow_scale[m->body_type][2];
	if(v->slot == this_node)
		bscale *= 1.1;
	for(i=0;i<3;i++)
		{
		len = xyz[0][i] - xyz[3][i];
		xyz[0][i] += (len * fscale);
		xyz[3][i] -= (len * bscale);

		len = xyz[1][i] - xyz[2][i];
		xyz[1][i] += (len * fscale);
		xyz[2][i] -= (len * bscale);
		}

/*	CheckVisible(v, view != 1 || (v->slot != this_node)); */
#if 0
	if (v->slot == this_node)
		for(i=0;i<4;i++)
			SMSG( 7+i, 2, "shad xyz[%ld] %8.3f %8.3f %8.3f", (long)i, xyz[i][0] , xyz[i][1] , xyz[i][2] );
#endif
	ZOID_ShowPoly(v->objnum);
	xlu = rng( d1*8, 20, 255 );
	ZOID_UpdatePoly(v->objnum, 0, -2, xyz, -1, xlu);
}	/*	end AnimateShadow */


/* ----------------------------------------------------------------------------- *
 *	Enables brake lights for a car.
 * ----------------------------------------------------------------------------- */
static void StartBrakeLights(S16 slot)
{
	Visual *v 	= &game_car[slot].visuals[VIS_BRAKES];

	v->func		= AnimateBrakeLights;
	v->slot		= slot;
	v->data		= 0;
	v->objnum	= gCarParts[slot].brakes;
}	/*	end StartBrakeLights */


/* ----------------------------------------------------------------------------- *
 *	Makes brake lights animate.
 * ----------------------------------------------------------------------------- */
static void AnimateBrakeLights(Visual *v, S16 op)
{
	BOOL		braking, hid;
	S16 		slot	= v->slot;
	CAR_DATA	*car	= &game_car[slot];

	braking = ((car->appearance & App_M_BRAKE_LIGHTS) && 
			   !coast_flag &&
			   !(car->appearance & (App_M_LR_DAMAGE | App_M_RR_DAMAGE | App_M_HULK)) &&
			   (slot != this_node || view >= 3));
	/*	Check if brakes need to be destroyed. */
	if (op == 0)
		{
		if (v->objnum >= 0)
			MBOX_HideObject( v->objnum, HIDE_ONLY);
		v->objnum 	= -1;
		return;
		}

	if (v->objnum < 0)
        v->objnum = gCarParts[slot].brakes;

	/*	Check if brakes need to be displayed. */
	CheckVisible( v, braking );

}	/*	end AnimateBrakeLights */


/* ----------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------- *
 *	Enables sparks for a car.
 * ----------------------------------------------------------------------------- */
void StartSparks(S16 slot)
{
	Visual *v;

#if !DEBUG_SPARKS
	/*	Sparks not loaded during demo. */
	if (demo_game)
		return;
#endif
	v 			= &game_car[slot].visuals[VIS_LSPARK];
	v->func		= AnimateLSpark;
	v->slot		= slot;
	v->objnum	= gCarParts[slot].lSpark;
	v 			= &game_car[slot].visuals[VIS_RSPARK];
	v->func		= AnimateRSpark;
	v->slot		= slot;
	v->objnum	= gCarParts[slot].rSpark;
	v 			= &game_car[slot].visuals[VIS_BSPARK];
	v->func		= AnimateBSpark;
	v->slot		= slot;
	v->objnum	= gCarParts[slot].bSpark;

}	/*	end StartSparks */


/* ----------------------------------------------------------------------------- *
 *	Makes Left Spark animate.
 * ----------------------------------------------------------------------------- */
static void AnimateLSpark(Visual *v, S16 op)
{
	BOOL	spark;
	F32		x,y,z;
#if DEBUG_SPARKS
	static S32 startTime, maxTime;

	if (game_car[v->slot].appearance & App_M_LSPARK)
		{
		if (!startTime)
			startTime = IRQTIME;
		if (IRQTIME - startTime > maxTime)
			{
			SMSG( 4, 41, "l spark on for %4ld msec", IRQTIME-startTime);
			maxTime = IRQTIME - startTime;
			}
		}
	else
		startTime = 0;
#endif

	spark 	= game_car[v->slot].mph > 5.0 && (game_car[v->slot].appearance & App_M_LSPARK) != 0;	
	x		=  game_car[v->slot].TIRER[3][1] - 0.6;
	y		= -game_car[v->slot].TIRER[3][2] + model[v->slot].tires[3].tradius*2;
	z		=  game_car[v->slot].TIRER[3][0];
	HandleASpark( v, spark, x,y,z, OBJ_SPARKS, op );
}	/* end AnimateLSpark */


/* ----------------------------------------------------------------------------- *
 *	Makes Right Spark animate.
 * ----------------------------------------------------------------------------- */
static void AnimateRSpark(Visual *v, S16 op)
{
	BOOL	spark;
	F32		x,y,z;
#if DEBUG_SPARKS
	static S32 startTime, maxTime;

	if (game_car[v->slot].appearance & App_M_RSPARK)
		{
		if (!startTime)
			startTime = IRQTIME;
		if (IRQTIME - startTime > maxTime)
			{
			SMSG( 5, 41, "r spark on for %4ld msec", IRQTIME-startTime);
			maxTime = IRQTIME - startTime;
			}
		}
	else
		startTime = 0;
#endif

	spark 	= game_car[v->slot].mph > 5.0 && (game_car[v->slot].appearance & App_M_RSPARK) != 0;	
	x		=  game_car[v->slot].TIRER[2][1] + 0.6;
	y		= -game_car[v->slot].TIRER[2][2] + model[v->slot].tires[2].tradius*2;
	z		=  game_car[v->slot].TIRER[2][0];

	HandleASpark( v, spark, x,y,z, OBJ_SPARKS, op );
}	/* end AnimateRSpark */


/* ----------------------------------------------------------------------------- *
 *	Makes Bottom Spark animate.
 * ----------------------------------------------------------------------------- */
static void AnimateBSpark(Visual *v, S16 op)
{
	BOOL	spark, top;
	F32		x,y,z;
#if DEBUG_SPARKS
	static S32 startTime,maxTime;

	if (game_car[v->slot].appearance & App_M_BSPARK)
		{
		if (!startTime)
			startTime = IRQTIME;
		if (IRQTIME - startTime > maxTime)
			{
			SMSG( 6, 41, "b spark on for %4ld msec", IRQTIME-startTime);
			maxTime = IRQTIME - startTime;
			}
		}
	else
		startTime = 0;
#endif

	top		= model[v->slot].topscrape;
	spark 	= game_car[v->slot].mph > 3.0 && (top || (game_car[v->slot].appearance & App_M_BSPARK) != 0);

	x		=  0.0;
	y		= -game_car[v->slot].TIRER[3][2] + ((top) ? 2.0 : model[v->slot].tires[2].tradius);
	z		=  game_car[v->slot].TIRER[3][0]-2.0;
	HandleASpark( v, spark, x,y,z, OBJ_SPARKS, op );
}	/* end AnimateBSpark */


/* ----------------------------------------------------------------------------- *
 *	Makes any Spark animate.
 * ----------------------------------------------------------------------------- */
static void HandleASpark(Visual *v,BOOL on,F32 x,F32 y,F32 z,S32 spark,S16 op)
{
	CAR_DATA	*car = &game_car[v->slot];

	/* Handle kill call. */
	if (op == 0)
		goto remove;

	if (v->index < 0)
		if (on)
			{
			v->index = 0;
			PositionObject( v->objnum, x,y,z, car->V[1],-car->V[2],car->V[0] );
			}
		else
			goto remove;

	/*	Check if sparks need to be created. */
	else if (on)
		{
		PositionObject( v->objnum, x,y,z, car->V[1],-car->V[2],car->V[0] );

		/*	Spark object exists, animate it. */
		if (++v->index >= 3)
			v->index = Random(4.0) + 2;
		}
	
	/*	Check if they need to be destroyed. */
	else if (v->index > 3)
		v->index = 3;
	else 
		--v->index;

	if (v->index > 5)
		v->index = 5;

	/*	Install a new spark texture. */
	if (CheckVisible(v,v->index >= 0))
		{
		ZOID_ChangeTexture(gObjList[spark], 0,-1,gTexList[TEX_SPARK1+v->index]);
		return;
		}

remove:
	MBOX_HideObject( v->objnum, HIDE_ONLY);
	v->index = -1;

}	/*	end HandleASpark */


/* ----------------------------------------------------------------------------- */
static S32 PlaceAnObj(S32 objnum, F32 v[3])
{
    S32 		i;
	MATRIX		tmat;
	MATV		*matv;

	/*	Build UV matrix from input direction vector. */
	/*	Use direction vector for the z vector of the UV array. */
	matv = &tmat.matv;

	if (!(v[0] || v[1] || v[2]))
		v[2] = 1;
	CopyVector (v, matv->zuv);
	direction (matv->zuv, matv->zuv);
   
	/*	Create a sort-of perpendicular temporary vector to fix the up direction.*/
	CopyVector (rvector,matv->xuv);

	/*	Cross the direction and temp vectors to get a true perpendicular y component,
		then use the y and the direction to create a true x component. */
	CrossVector (matv->zuv,matv->xuv,matv->yuv);
	CrossVector (matv->yuv,matv->zuv,matv->xuv);

	i = MBOX_NewObject(objnum, zerovector, tmat.mat3.uvs, -1, 0);
	if (i > 2045)
		EMSG( "Warning: running out of objects" );
    return i;
}	/* end PlaceAnObj */


/* ----------------------------------------------------------------------------- */
/* create uv object */
S32 InitChildObj(S32 objnum, S32 parent)
{
	S32	i;

	i = MBOX_NewObject(objnum,zerovector,unituvs, parent, 0);
	if (i > 2045)
		EMSG( "Warning: running out of objects" );
	
    return i;
}	/* end InitChildObj */


/* ----------------------------------------------------------------------------- */
/* create uv object */
static S32 InitChildObjFlags(S32 objnum, S32 parent, U32 flags)
{
	S32	i;

	i = MBOX_NewObject(objnum,zerovector,unituvs, parent,  flags);
	if (i > 2045)
		EMSG( "Warning: running out of objects" );
	
    return i;
}	/* end InitChildObjFlags */


/* ----------------------------------------------------------------------------- */
/* create uv object */
static S32 PlaceChildObj(S32 objnum,F32 dx,F32 dy,F32 dz,F32 *orient,S32 parent)
{
    S32 	i;
	F32		pos[3],rvec[3];
	MATRIX	tmat;

	pos[0] 	= dx;
	pos[1]	= dy;
	pos[2]	= dz;
	if (orient && (orient[0] || orient[1] || orient[2]))
		{
		rvec[0] = orient[0]; 
		rvec[1] = orient[1]; 
		rvec[2] = orient[2];
		PointInDir( orient, &tmat );
		i = MBOX_NewObject(objnum,pos,tmat.mat3.uvs, parent, 0);
		}
	else
		i = MBOX_NewObject(objnum,pos,unituvs, parent, 0);

	if (i > 2045)
		EMSG( "Warning: running out of objects" );
    return i;
}	/* end PlaceChildObj */


/* ----------------------------------------------------------------------------- */
static void PositionObject( S32 objnum, F32 x, F32 y, F32 z, F32 u, F32 v, F32 w )
{
	F32		pos[3];
	MATRIX	tmat;

	pos[0] 	= u;
	pos[1]	= v;
	pos[2]	= w;
	PointInDir( pos, &tmat );
	pos[0]	= x;
	pos[1] 	= y;
	pos[2] 	= z;
	ZOID_UpdateObject(objnum,pos,tmat.mat3.uvs);
}	/* end PositionObject */


/* ----------------------------------------------------------------------------- */
void PointInDir(F32 lookdir[3], MATRIX *mat) 
{
	F32 length,temp;

	CopyVector (lookdir, mat->matv.zuv);
	direction (mat->matv.zuv, mat->matv.zuv);
   
	mat->matv.xuv[0] = lookdir[2];
	mat->matv.xuv[1] = 0.0;
	mat->matv.xuv[2] = -lookdir[0];

	length = magnitude (mat->matv.xuv);

	if (length <= 0.1)
		CopyVector (rvector,mat->matv.xuv);

	direction (mat->matv.xuv, mat->matv.xuv);

	CrossVector (mat->matv.zuv,mat->matv.xuv,mat->matv.yuv);
	CrossVector (mat->matv.yuv,mat->matv.zuv,mat->matv.xuv);

}	/* end PointInDir */


/* -----------------------------------------------------------------------------
 *	Turns an item on or off based on the 'on' test and if item is already on or not.
 *	Returns true if item is visible.
 * ----------------------------------------------------------------------------- */
static BOOL CheckVisible( Visual *v, BOOL on )
{
	U32	hid = ZOID_ObjectFlags(v->objnum) & ZATR_OFF;

	if (on && hid)
		MBOX_ShowObject( v->objnum, SHOW_ONLY);
	else if (!on && !hid)
		MBOX_HideObject( v->objnum, HIDE_ONLY);
	return on;
}	/* end CheckVisible */


/* -----------------------------------------------------------------------------
 *	Handles translucency changes for an object.
 *	Returns true if item is translucent.
 * ----------------------------------------------------------------------------- */
static BOOL CheckXlu( Visual *v, BOOL lucent, S32 xlu)
{
	if (!lucent)
		xlu = 0xFF;

	if (ObjInstanceList[v->objnum].xlucency != xlu)
		ZOID_SetObjectXlu(v->objnum, xlu);
	return lucent;
}	/* end CheckXlu */

