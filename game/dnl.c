/*
 *  dnl.c   -- harddrive loading routines
 *
 *  Copyright 1996 Time Warner Interactive.
 *  Unauthorized reproduction, adaptation, distribution, performance or
 *  display of this computer program or the associated audiovisual work
 *  is strictly prohibited.
 *
 *  ======================================================================
 *  $Author: hightower $                     $Date: 1997/10/11 21:06:47 $
 *  $Revision: 3.62 $                       $Locker:  $
 *  ======================================================================
 *  Change Log:
 *
 *  $Log: dnl.c,v $
 * Revision 3.62  1997/10/11  21:06:47  hightower
 * Made drone cars the same
 *
 * Revision 3.61  1997/09/25  03:03:50  gray
 * Select mirrored track paths if mirror_flag.
 *
 * Revision 3.60  1997/09/16  22:08:13  gray
 * LoadCars no longer accesses DRONES directory.
 *
 * Revision 3.59  1997/09/15  22:27:35  gray
 * Removed trackset hack - trackno now 0 to NTRACKS.
 *
 * Revision 3.58  1997/09/13  04:57:23  gray
 * car setup for NCARS (not 12).
 *
 * Revision 3.57  1997/09/13  01:47:32  grjost
 * Fixed for 12 cars.
 *
 * Revision 3.56  1997/07/01  18:58:32  gray
 * Made alternate tracks (additional 4 tracks) view 1 selectable.
 *
 * Revision 3.55  1997/06/28  01:40:05  gray
 * Extended to 4 tracks.
 *
 * Revision 3.54  1997/03/31  03:10:37  gray
 * Nucleus gone - got rid of include and nucleus types.
 *
 * Revision 3.53  1997/03/28  19:28:45  gray
 * Removed unused nucleus calls.
 *
 * Revision 3.52  1997/03/13  04:01:14  geraci
 * removed some text to screen calls during loads
 *
 * Revision 3.51  1997/02/26  03:28:51  gray
 * Added 3Dfx/glide reset every 10 minutes (in attract mode only).
 *
 * Revision 3.50  1997/02/15  01:44:52  geraci
 * join now works during hard drive load
 *
 * Revision 3.49  1996/12/14  09:12:32  gray
 * RUSHWHISPER for race transition screen.
 *
 * Revision 3.48  1996/12/13  13:48:23  gray
 * Get transition screen working without texture corruption.
 *
 * Revision 3.47  1996/12/13  13:36:37  geraci
 * uses inited model data for loading cars
 *
 * Revision 3.46  1996/12/13  10:44:34  gray
 * Random car for drones only.
 *
 * Revision 3.45  1996/12/12  10:38:50  geraci
 * if markers are enabled drones are multi-colored
 *
 * Revision 3.44  1996/12/12  09:59:15  gray
 * Make track textures always load at same place.
 *
 * Revision 3.43  1996/12/06  15:48:28  gray
 * Keep screen from going black during load.
 *
 * Revision 3.42  1996/12/05  11:13:35  gray
 * Fixed texture corruption by disabling the insanely stupid stuff in TransitionTask.
 *
 * Revision 3.41  1996/12/04  11:26:57  geraci
 * randomly chose drone colors
 *
 * Revision 3.40  1996/11/27  09:18:21  petrick
 * Set gLink[n].car for drones.
 *
 * Revision 3.39  1996/11/27  08:11:48  geraci
 * I can write to the hard drive again
 *
 * Revision 3.38  1996/11/27  01:58:27  petrick
 * Enabled more tex memory savings.
 *
 * Revision 3.37  1996/11/24  10:54:22  petrick
 * Got rid of setmintexbase.
 *
 * Revision 3.36  1996/11/24  05:49:54  petrick
 * Set up to get even more space out of hud.
 *
 * Revision 3.35  1996/11/21  13:58:13  petrick
 * Made locked models work again.
 *
 * Revision 3.34  1996/11/19  21:43:46  gray
 * Load reflection textures for select.
 *
 * Revision 3.33  1996/11/19  17:49:44  petrick
 * Dynamic texture loading, centralized loading.
 *
 * Revision 3.32  1996/11/19  12:58:49  gray
 * New reflect stuff.
 *
 * Revision 3.31  1996/11/17  14:19:12  petrick
 * Changes to make sure the attract graphics are loaded when needed.
 *
 * Revision 3.30  1996/11/16  10:10:18  gray
 * Free up some old maxpath arrays.
 *
 * Revision 3.29  1996/11/15  04:23:55  petrick
 * Changes to support new disk driver.
 *
 * Revision 3.28  1996/11/14  01:30:13  petrick
 * Alan's new background loader.
 *
 * Revision 3.27  1996/11/13  11:54:53  petrick
 * Made sure all load model calls can abort OK.
 *
 * Revision 3.26  1996/11/11  14:25:59  petrick
 * Made LoadModel smarter about allowing aborts.
 *
 * Revision 3.25  1996/11/09  13:11:00  petrick
 * Start of abortable downloading code.
 *
 * Revision 3.24  1996/11/07  08:18:24  gray
 * Car shading and reflection stuff.
 *
 * Revision 3.23  1996/11/06  02:48:19  petrick
 * Gave more stack space to background loader task.
 *
 * Revision 3.22  1996/11/05  12:28:03  petrick
 * Commented out Forrest's hack to turn on interrupts in disk load task.
 *
 * Revision 3.21  1996/11/03  05:56:44  geraci
 * split up debug switches and regular game switches
 *
 * Revision 3.20  1996/10/30  05:46:43  petrick
 * Got rid of entityList.
 *
 * Revision 3.19  1996/10/29  08:35:35  petrick
 * Hopefully helped mole mode.
 *
 * Revision 3.18  1996/10/29  08:32:04  geraci
 * allocated more memory for maxpath data
 *
 * Revision 3.17  1996/10/25  09:56:17  geraci
 * new size for maxpath data structure
 *
 * Revision 3.16  1996/10/23  00:24:25  petrick
 * Made track_no a global.
 *
 * Revision 3.15  1996/10/18  10:59:52  petrick
 * Made LoadModel into a background task.
 *
 * Revision 3.14  1996/10/15  02:05:43  geraci
 * sound call during hard drive access waits till queue is 25% full
 *
 * Revision 3.13  1996/10/10  05:26:48  petrick
 * Changes to give immediate feedback during disk loads.
 *
 * Revision 3.12  1996/10/08  01:49:31  petrick
 * *** empty log message ***
 *
 * Revision 3.11  1996/10/07  16:13:06  geraci
 * made pointer to track name more pointer specific
 *
 * Revision 3.10  1996/10/05  09:26:01  geraci
 * sound call when gas pressed during disk idle time
 *
 * Revision 3.9  1996/10/02  21:19:15  petrick
 * Made ide callback do something.
 *
 * Revision 3.8  1996/09/27  02:14:05  petrick
 * Changes for static model.
 *
 * Revision 3.7  1996/09/26  09:57:16  petrick
 * Changes for new high score screens.
 *
 * Revision 3.6  1996/09/26  00:07:27  petrick
 * Hopefully fot rid of CAR256 bug.
 *
 * Revision 3.5  1996/09/25  02:03:20  petrick
 * Got rid of include file.
 *
 * Revision 3.4  1996/09/25  01:25:59  gray
 * romaddr not used now.
 *
 * Revision 3.3  1996/09/25  00:22:18  gray
 * Removed old ZOID download stuff.
 *
 * Revision 3.2  1996/09/19  15:03:41  petrick
 * Added offline msg.
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 *  ======================================================================
 */

#include "mb.h"
#include "blit.h"
#include "mem.h"
#include "globals.h"
#include "game.h"
#include "os_proto.h"
#include "intvecs.h"
#include "world.h"
#include "modeldat.h"
#include "drivsym.h"
#include "checkpoint.h"
#include "menus.h"
#include "debug.h"
#include "attract.h"
#include "maxpath.h"
#include "dis.h"
#include "sounds.h"
#include "targets.h"
#include "gamenet.h"

#include "Pro/dnl.pro"
#include "Pro/fswheel.pro"
#include "Pro/globals.pro"
#include "Pro/init.pro"
#include "Pro/initiali.pro"
#include "Pro/money.pro"
#include "Pro/moobs.pro"
#include "Pro/pots.pro"
#include "Pro/sounds.pro"
#include "Pro/select.pro"
#include "Pro/strings.pro"
#include "Pro/timer.pro"
#include "Pro/timint.pro"
#include "Pro/world.pro"

/*for mole stuff*/
#if !INCLUDE_QIO
#include "hdrive.h"
#include "Pro/mole.pro"
#endif
#include "controls.h"
#include "error.h"

/* reset 3Dfx/glide in attract every 10 minutes*/
#define SST_RESET_SECONDS (10 * 60)

#define EVEN_MORE_SPACE 1       /*  Set to 1 to get even more space out of the hud.
                                 *  Must also take the textures in Movie7 out of HUD and STATIC. */

extern int RdFile(const char * path_str, const char * filename, U32 max_len, U32 * dest);
extern int read_file_section (const char *path, const char *fname, U32 start, U32 max_len, U32 * dest);
extern int HDStart(void);
extern void WrFile(const char * path_str, const char * filename, U32 len, U32 * src);
extern void grGlideShutdown(void);
extern AttractMode                    attractFunc;

extern U32      *gRoadDat;
extern MPHEADER *gMaxPath, *gmpath_save, *gmpath_save2;
extern U8       gStartDuringLoad;
extern S16      mBlitIndex;

static U32      gLoadStartTime;
static BOOL     gLoadFinished;

int             (*gTransitionFunc)(void *);
static int      gLastProgress;

ModelType       gLastModelLoaded;


/* ----------------------------------------------------------------------------- */
void loadInit(void)
{
    U32 tlevels,tdlevels,molecode;

    tlevels = get_levels();
    tdlevels = get_dlevels();
#if !INCLUDE_QIO
    /* make darn sure you want mole loop... SW8 up plus all 3 view buttons. */
    molecode = SW_VIEW1 | SW_VIEW2 | SW_VIEW3;
    if(((tlevels & molecode) == molecode) && (tdlevels & SW_DEBUG8))
        {
        ChangeToMole();
        while(1);
        }
#endif
    /* Load static  graphics from HD. */
    Load(0);

    /*!!! Need to make so init code does not need road loaded!!!*/
    /*load roaddat.rom from HDD to ramrom*/
    gRoadDat = AllocFile("TRACKS/TRACK1", "roaddat.rom");
}   /* end loadInit */

/* ----------------------------------------------------------------------------- */
static S32 TransitionTask(void)
{
    /*  Handle pdus and switch inputs. */
    ProcessPDUs(false);
    get_switches (Do_it);

    /*  If there is a transition animation, play it. */
    if (gTransitionFunc)
        {
        gLastProgress = gTransitionFunc(nil);
        ZOID_EndFrame();
        /* wait till the frame swap is finished. */
        /* otherwise - texture corruption. */
        prc_delay(0);
        }

    return 0;
}   /*  end TransitionTask */

BOOL        join_during_load;

/* ----------------------------------------------------------------------------- */
static S32 AbortTask(void)
{
    TransitionTask();

    join_during_load |= (levels & SW_JOIN) != 0;

    /*  Check for abort conditions, like a game start. */
    if (!gStartDuringLoad && chk_start())
        {
        gStartDuringLoad = 1;
        MSG( "ABORT" );
        return 1;
        }

    /* 0 = normal, 1 = join now pressed */
    return (join_during_load);

}   /*  end AbortTask */

/* ----------------------------------------------------------------------------- *
 *  Centralizes loading of Initial modesl for various modes.
 * ----------------------------------------------------------------------------- */
BOOL Load(S16 what)
{
    static  U32 last_sst_reset;

    switch (what)
        {
        case 0: /*  Initial loading.    */
            /* Load static  graphics from HD. */
            MBOX_ResetModels(0,0);
#if TRACE_MODELS
            MSG("--- MBOX_ResetModels()");
            MSG("M%2ld MBOX_LoadModel(STATIC)", (long)model_count ); ZOID_EndFrame();
            MBOX_LoadModel("STATIC");
#if EVEN_MORE_SPACE
            MSG("M%2ld MBOX_LoadMovie(MOVIES/MOVIE7)", (long)model_count); ZOID_EndFrame();
            MBOX_LoadMovie("MOVIES/MOVIE7",(256L*64L*2L),1);
#endif
            MSG("M%2ld MBOX_LoadMovie(MOVIES/MOVIE8)", (long)model_count); ZOID_EndFrame();
            MBOX_LoadMovie("MOVIES/MOVIE8",(256L*64L*2L),1);
            MSG("+++ MBOX_LockModels()"); ZOID_EndFrame();
            MBOX_LockModels();
            MSG("M%2ld MBOX_LoadModel(ATTRACT)", (long)model_count ); ZOID_EndFrame();
            MBOX_LoadModel("ATTRACT");
#else
            MBOX_LoadModel("STATIC");
#if EVEN_MORE_SPACE
            MBOX_LoadMovie("MOVIES/MOVIE7",(256L*64L*2L),1);
#endif
            MBOX_LoadMovie("MOVIES/MOVIE8",(256L*64L*2L),1);
            MBOX_LockModels();
            MBOX_LoadModel("ATTRACT");
#endif
            mBlitIndex      = -1;
            gLastModelLoaded=MT_NONE;
            break;

        case 1: /*  Transition from attract to select. */
            MBOX_ResetUnlockedModels(0,0);
#if TRACE_MODELS
            MSG("--- MBOX_ResetUnlockedModels(0)"); ZOID_EndFrame();
#endif
            MBOX_LoadModel("LANG/LANG7" );
            mBlitIndex      = -1;
            gLastModelLoaded=MT_NONE;
            break;

        case 3: /*  During select initialization. (this plus case 1 gets loaded).*/
            LoadCars(true, false);
            LoadModel("SELECT", false, false);
            MBOX_SetMinTexBase(0x400000);
            LoadModel("REFLECT", false, false);
            gStartDuringLoad= 0;
            gTransitionFunc = nil;
            gLastModelLoaded=MT_NONE;
            ShowTransit(false, 1);
            break;

        case 2:     /*  Transition from select to start of game. */
            SetupTargets(trackno);
#if TRACE_MODELS
            MSG("M%2ld MBOX_LoadModel(LANG/LANG7)", (long)model_count );
#endif
            MBOX_LoadModel("LANG/LANG7" );/* ZOID_EndFrame();*/
            gLastModelLoaded=MT_NONE;
            break;

        case 4:     /*  Start of a demo game. */
            /* Reset 3Dfx/glide occasionally. */
            if( (IRQTIME - last_sst_reset) > (SST_RESET_SECONDS * ONE_SEC) &&
                (attractFunc == ATR_DEMO) )
                {
                grGlideShutdown();
                sst_reset();
                MBOX_ResetModels(0,0);
                MBOX_LoadModel("STATIC");
                MBOX_LoadMovie("MOVIES/MOVIE7",(256L*64L*2L),1);
                MBOX_LoadMovie("MOVIES/MOVIE8",(256L*64L*2L),1);
                MBOX_LockModels();
                last_sst_reset = IRQTIME;
                }
            else
                MBOX_ResetUnlockedModels(0,0);

#if TRACE_MODELS
            MSG("--- MBOX_ResetUnlockedModels(0)"); ZOID_EndFrame();
#endif
            SetupTargets(trackno);
            mBlitIndex      = -1;
            gLastModelLoaded=MT_NONE;
            break;

        case 5:     /*  Start of a real game. */
            mBlitIndex      = -1;
            gLastModelLoaded=MT_NONE;
            break;

        case 6:     /*  Playing an attract movie. */
            gLastModelLoaded=MT_NONE;
            if (!LoadModel("ATTRACT", true, false))
                return false;
            gLastModelLoaded=MT_ATTRACT;
            break;
        }

    return true;
}   /* end Load */


/* ----------------------------------------------------------------------------- */
BOOL LoadModel(char *name, BOOL reset, BOOL canAbort)
{
    S32 ret;

    if (reset)
        {
        MBOX_ResetUnlockedModels(0,0);
#if TRACE_MODELS
        MSG("--- MBOX_ResetUnlockedModels(0)"); ZOID_EndFrame();
#endif
        }

#if TRACE_MODELS
    MSG("M%2ld LoadModel(%s)", (long)model_count, name ); ZOID_EndFrame();
#endif
    if (canAbort)
        ret = MBOX_LoadModelCheck( name, AbortTask );
    else
        ret = MBOX_LoadModelCheck( name, TransitionTask );
    return (ret >= 0);

}   /* end LoadModel */


/* ----------------------------------------------------------------------------- *
 *  Loads car models from hard disk.  If 'all' is set, all models with the local
 *  stack's color are loaded; if false, only active car models are loaded and
 *  drones stuck in other slots.
 * ----------------------------------------------------------------------------- */
BOOL LoadCars(BOOL all, BOOL canAbort)
{
    S32 i;
    char carName[40];
    extern S16 gCarModelNums[NCARS];    /*  Model number of each car.  */

    /*  Load one of each car type. Only happens during car select. */
    if (all)
    {
        for (i = 0; i < NCARS; ++i)
        {
            /*  Save model number of each car for later car part retrieval. */
            gCarModelNums[i] = model_count;
            sprintf (carName, "CARS/CAR%ld/COL%ld", i + 1L, gThisNode + 1L);
            if (!LoadModel (carName, false, false))
                return false;
        }
        for (; i < MAX_LINKS; ++i)
            gCarModelNums[i] = model_count - 1;
    }

    /* Load cars for each active slot. */
    else
    {
        for (i = 0; i < MAX_LINKS; i++)
    {
        if( model[i].drone_type == HUMAN )
        {
        Randomize( gn_state[i].d.seed );
        break;
        }
    }
        for (i = 0; i < MAX_LINKS; ++i)
        {
            /*  Save model number of each car for later car part retrieval. */
            gCarModelNums[i] = model_count;

            if (model[i].drone_type != HUMAN)
                gLink[i].car = Random (NCARS);
            /* Check car num even for HUMAN. */
            if (gLink[i].car >= NCARS)
                gLink[i].car = NCARS - 1;
            sprintf (carName, "CARS/CAR%ld/COL%ld", gLink[i].car + 1L, i + 1L);
            if (!LoadModel (carName, false, canAbort))
                return false;
        }
    }
    return true;
}                               /* end LoadCars */


/* ----------------------------------------------------------------------------- */
BOOL loadTrack(S32 track,BOOL useHud, BOOL canAbort)
{
    const char  *dir;
    extern char DBG_fltinfo[];
    BOOL    ret;


    dir = get_path (track);

    /* Load static  graphics from HD. */
    if (demo_game)
        Load(4);
    else
        Load(5);

    /*  Load track object and texture data.*/
    gRoadDat        = AllocFile(dir, "roaddat.rom");
    gMaxPath        = AllocMem(MAXMPATH*sizeof(MPATH)*8);
    gmpath_save     = AllocMem(MAXMPATH*sizeof(MPATH));
    gmpath_save2    = AllocMem(MAXMPATH*sizeof(MPATH));
    RdFile(dir, "dpath.rom", MAXMPATH*sizeof(MPATH)*8, (U32 *)gMaxPath);
/*  gMaxPath = AllocFile(dir, "dpath.rom");*/

    if(!demo_game)
        SOUND(S_RUSHWHISPER);

    if (useHud)
        {
        if ((ret = LoadModel("HUD",false, canAbort)) != 0)
            gLastModelLoaded = MT_HUD;
        }
    else if ((ret = LoadModel("HISCORE",false, canAbort)) != 0)
        gLastModelLoaded = MT_HISCORE;

#if 1
/* Get track textures to load at same place */
/* independent of previously loaded textures. */
    if (!ret || !LoadCars(false,canAbort))
        return false;
    MBOX_SetMinTexBase(0x290000);
    if(!LoadModel( (char *)dir,false, canAbort))
        return false;
    MBOX_SetMinTexBase(0x400000);
    if (!LoadModel("REFLECT", false, canAbort))
        return false;
#else
    if (!ret || !LoadCars(false,canAbort) || !LoadModel( (char *)dir,false, canAbort))
        return false;
    MBOX_SetMinTexBase(0x400000);
    if (!LoadModel("REFLECT", false, canAbort))
        return false;
#endif

    /* Load world objects into display list.*/
    RdFile(dir, "worlds.rom", WORLD_BSIZE, (U32 *)WORLD_DATA);
    LoadWorld();

    /*load fltinfo.rom for debug*/
    RdFile(dir, "fltinfo.rom",200,(U32 *)DBG_fltinfo);
    DBG_fltinfo[64] = 0;    /*null terminate string to screen size just in case */

    /*  reInitialize the visual effects. */
    InitVisuals(true);
    init_cp_data();

    /* disable the friggin thing for attract. */
    gTransitionFunc = nil;

    return true;
}   /* end loadTrack */


/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Write a file to the DRONETEST directory                              */

void write_file_to_hd (char *filename, U32 len, U32 *data)
{
    char   buf[60];

    sprintf (buf, "Writing %s to hard drive", filename);
    txt_str (10, 10, buf, RED_PAL);
    WrFile ("/DRONETEST", filename, len, data);
    txt_str (10, 10, "                                    ", RED_PAL);
}

/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*
*       Read a file from the DRONETEST directory                             */

void read_file_from_hd (char *filename, U32 len, U32 *data)
{
    char   buf[60];

#if 0
    sprintf (buf, "Reading %s from hard drive", filename);
    txt_str (10, 10, buf, RED_PAL);
#endif
    read_file_section ("/DRONETEST", filename, 0, len, data);
#if 0
    txt_str (10, 10, "                                    ", RED_PAL);
#endif
}

char trkpath[80];

char *get_path (S32 track)
{
    extern S32 mirror_flag;

    /* set gMirrorMode to match track loaded. */
    gMirrorMode = ((gstate != ATTRACT) && mirror_flag) ? 1 : 0;

    if(gMirrorMode)
        {
        if(dlevels & SW_DEBUG11)    /* development track */
            sprintf(trkpath,"TRACKS/DEV/MIRROR%ld",track+1);
        else                /* saved track */
            sprintf(trkpath,"TRACKS/MIRROR%ld",track+1);
        }
    else
        {
        if(dlevels & SW_DEBUG11)    /* development track */
            sprintf(trkpath,"TRACKS/DEV/TRACK%ld",track+1);
        else                /* saved track */
            sprintf(trkpath,"TRACKS/TRACK%ld",track+1);
        }

    return(trkpath);
}

/* ----------------------------------------------------------------------------- *
 *  Checks if the given model type is loaded and if not loads it.
 * ----------------------------------------------------------------------------- */
BOOL CheckModelType(S16 type)
{
    BOOL        ret = true;
    ModelType   t = (ModelType)type;

    if (gLastModelLoaded == t)
        return true;

    switch (t)
        {
        case MT_ATTRACT:    ret = LoadModel("ATTRACT", true, false);    break;
        case MT_HISCORE:    ret = LoadModel("HISCORE", false, true);    break;
        case MT_HUD:        ret = LoadModel("HUD",     false, true);    break;
        case MT_STATIC:     return true;
        default:
            break;
        }
    if (ret)
        gLastModelLoaded = t;

    return (ret != 0);
}   /*  end CheckModelType */
