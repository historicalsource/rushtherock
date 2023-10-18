/*
 *	stree.c	-- Surface tree functions.
 *
 *	Copyright 1996 Time Warner Interactive.
 *	Unauthorized reproduction, adaptation, distribution, performance or 
 *	display of this computer program or the associated audiovisual work
 *	is strictly prohibited.
 *
 *	======================================================================
 *	$Author: gray $						$Date: 1996/11/20 12:17:47 $
 *	$Revision: 3.12 $						$Locker:  $
 *	======================================================================
 *	Change Log:
 *
 *	$Log: stree.c,v $
 * Revision 3.12  1996/11/20  12:17:47  gray
 * Removed old shadow stuff.
 *
 * Revision 3.11  1996/11/09  11:15:51  gray
 * Module description was bogus.
 *
 * Revision 3.10  1996/10/31  06:55:24  geraci
 * car data is in upper case now
 *
 * Revision 3.9  1996/10/30  10:40:57  gray
 * Look at p1 for surface loft. Slightly less bumpy.
 *
 * Revision 3.8  1996/10/30  07:13:36  gray
 * Fixed road x interpolation value.
 *
 * Revision 3.7  1996/10/30  00:24:21  gray
 * New suspension tires and roads.
 *
 * Revision 3.6  1996/10/04  10:28:39  gray
 * New collision surface force.
 *
 * Revision 3.5  1996/10/03  21:28:44  gray
 * Temp non-roadrunner walls.
 *
 * Revision 3.4  1996/09/25  01:25:59  gray
 * romaddr not used now.
 *
 * Revision 3.3  1996/09/25  00:22:18  gray
 * Removed old ZOID download stuff.
 *
 * Revision 3.2  1996/09/17  00:58:40  gray
 * New roaddat structure. Passing roadcode,sviscode,roadboost,sound_flags, and shortcut_flags.
 *
 * Revision 3.1  1996/09/09  02:24:52  gray
 * JAMMA version
 *
 * Revision 2.19  1996/09/06  13:13:31  gray
 * new bodysurf force. drones dont orbit now.
 *
 * Revision 2.18  1996/09/06  01:30:48  gray
 * Maybe fixed obit drones for JAMMA.
 *
 * Revision 2.17  1996/09/05  01:45:35  gray
 * Passing airdist (for shadow) across link.
 *
 * Revision 2.16  1996/09/04  20:37:22  gray
 * Adding dead reckon data to game_car. Killing mainout.
 *
 * Revision 2.15  1996/09/01  09:18:02  gray
 * Model types cleanup. First pass - removed fptyp.
 *
 * Revision 2.14  1996/08/29  00:12:05  petrick
 * Removed excess transpose in shadow calculation.
 *
 * Revision 2.13  1996/08/21  06:02:19  gray
 * Tweaked wall collisions.
 *
 * Revision 2.12  1996/08/18  19:36:33  gray
 * Make PHOENIX_PORT only, removed flag.
 *
 * Revision 2.11  1996/08/17  18:14:58  gray
 * Increased top slide friction, added wall friction.
 *
 * Revision 2.10  1996/08/07  20:35:57  geraci
 * a totally game version of the shadow
 *
 * Revision 2.9  1996/07/23  20:50:25  gray
 * Moved bodyforce zeroing to road().
 *
 * Revision 2.8  1996/07/08  23:10:14  geraci
 * backward compatible for zoid
 *
 * Revision 2.7  1996/07/07  09:14:46  gray
 * Load roaddat into RAM not RAMROM.
 *
 * Revision 2.6  1996/06/29  00:05:25  geraci
 * fixed new bug due to bad path data
 *
 * Revision 2.5  1996/06/20  22:57:24  gray
 * minit_stree back to init_stree.
 *
 * Revision 2.4  1996/06/20  22:39:00  gray
 * Made hard drive loading the default.
 *
 * Revision 2.3  1996/05/25  02:15:21  gray
 * Hard Drive stuff - just load single track road data.
 *
 * Revision 2.2  1996/05/08  01:26:16  gray
 * Made car shadows as close to road as possible with no z conflict.
 *
 * Revision 2.1  1996/05/04  02:56:18  petrick
 * Focus 2 release
 *
 * Revision 1.41  1996/05/03  21:51:38  gray
 * Make shadows sort properly when car is upside down.
 *
 * Revision 1.40  1996/05/01  22:20:34  gray
 * Make shadow placement fit new shadow objs. Check for div by 0 in AlignShadow.
 *
 * Revision 1.39  1996/05/01  19:13:57  gray
 * Added surface finding code for shadow placement.
 *
 * Revision 1.38  1996/04/25  09:28:54  gray
 * Orbit mode _seems_ to be fixed. Removed force limiting stuff.
 *
 * Revision 1.37  1996/04/23  22:57:01  gray
 * Started to get wall collisions hard again.
 *
 * Revision 1.36  1996/04/01  13:55:52  geraci
 * tweaked maximum surface collision force
 *
 * Revision 1.35  1996/03/30  02:05:17  geraci
 * helped 'evil' collision somewhat
 *
 * Revision 1.34  1996/03/12  05:28:26  gray
 * Cleanup of collision stuff.
 *
 * Revision 1.33  1996/03/10  01:02:37  gray
 * FP underflow trap in GUTS. Removed all LimitFP stuff.
 *
 * Revision 1.32  1996/03/09  09:54:22  petrick
 * Alan's stop-crash fix.
 *
 * Revision 1.31  1996/03/05  03:41:35  gray
 *  Remove UVDEBUG flags.
 *
 * Revision 1.30  1996/03/01  18:25:01  gray
 * Removed DOFPSURF conditional, fixed mnames (some).
 *
 * Revision 1.29  1996/02/29  01:09:10  geraci
 * dskcomm now resides in mdrive
 *
 * Revision 1.28  1996/02/27  23:33:11  petrick
 * Added header correctly.
 *
 * Revision 1.27  1996/02/27  21:49:09  petrick
 * Added header to file.
 *
 *
 *	11/14/94 asg	Created
 *
 *	======================================================================
 */

#define DEBUGORBIT 1

#include "drivsym.h"
#include "globals.h"
#include "mdrive.h"
#include "modelcom.h"
#include "road.h"
#include "stree.h"
#include "config.h"			/*for printing*/

#include "Pro/communic.pro"
#include "Pro/d3math.pro"
#include "Pro/math.pro"
#include "Pro/road.pro"
#include "Pro/stree.pro"
#include "Pro/unitvecs.pro"
#include "Pro/vecmath.pro"

/*===============================  GLOBALS  =================================*/

STREETYP	*stree;
SURFTYP 	*surf;
SVERTTYP 	*svert;
PATHTYP 	*path;
U16 		*slist;
U16 		*vlist;
U16 		path_start, path_end, path_loop;

U32 *gRoadDat;


/*===============================  EXTERNS  =================================*/

extern S16 	trackno;


/*==============================  PROTOTYPES  ===============================*/

static void mats2fcopy(S16 *sp,F32 *fp);
static void veci2fcopy(S32 *lp,F32 *fp);

STHEADERTYP *gRoadHead;

/* ----------------------------------------------------------------------------- */
void init_stree(void)
{
	STHEADERTYP *hp;

	hp = (STHEADERTYP *)gRoadDat;

	gRoadHead = hp;

	/* setup stree pointer */    
	stree = (STREETYP *)(hp + 1);

	/* setup surf pointer */	
	surf = (SURFTYP *)(stree + hp->tnum);

	/* setup svert pointer */	
	svert = (SVERTTYP *)(surf + hp->snum);

	/* setup path pointer */	
	path = (PATHTYP *)(svert + hp->vnum);

	/* setup slist pointer */	
	slist = (U16 *)(path + hp->pnum);

	/* setup vlist pointer */	
	vlist = (U16 *)(slist + hp->slnum);

	/* setup path start, end, loop point */
	path_start = (U16)(hp->pathstart);
	path_end = (U16)(hp->pathend);
	path_loop = (U16)(hp->pathloop);

}

U16 xxxNumSurfs;
S32 xxxSurfDist;

#define MINSURFDIST (120)
#define DEFAULTDIST (-10000)

void tiresurf(MODELDAT *m, F32 ipos[3], F32 opos[3], S32 *roadcode, 
			F32 uvs[3][3], int whl)
{
	U16 i,j;
	S16 quad;
	S32 sli,tpos[3],dist,maxdist;
	S32 xxpos[3],topos[3];
	U16 *slp,snum;
	STREETYP *ltp;
	SURFTYP *sp,*savesp=0;

	/*temp - till updownleaf uses floats */
	for(i=0;i<3;i++)
		tpos[i] = ipos[i] * S4SCALE;
	/*temp - till updownleaf uses floats */

	/* traverse tree from prev leaf */
	ltp = updownleaf(m->lasttp[whl], tpos, &quad);
	m->lasttp[whl] = ltp;

	sli = -((S32)ltp->child[quad]); /* neg = slist index */
	if(sli)	/* > 0 surfs, check list for intersection */
		{
		slp = &slist[sli];
		snum = *slp++;

		maxdist = MAXSURFPOS;
		for(i=0;i<snum;i++)
			{
			sp = &surf[*slp++];
			if(mInSurf(tpos, topos, sp, &dist)) /* dist pos if tire below surface!!*/
				{
				if((dist < MAXSURFHOP) && (dist > maxdist))
					{
					maxdist = dist;
					savesp = sp;
					for(j=0;j<3;j++) xxpos[j] = topos[j];
					}
				}
			}

if(m == &model[gThisNode])
{
 xxxNumSurfs = snum;
 xxxSurfDist = maxdist;
}
		/**/
		if((maxdist != MAXSURFPOS)&&((savesp->flags & SURF_MASK) != SURF_COLLIDE))
/*		if(maxdist != MAXSURFPOS)*/
			{
			if(savesp->flags & INTERP_BIT)
				{
				fPathInterp(savesp, ipos, opos, uvs);
				}
#if 0
			else if((savesp->flags & SURF_MASK) == SURF_COLLIDE)
				{
/*				ivecsub(xxpos, ipos, xxpos);*/
/*				irwtobod(xxpos, opos, &m->UV);*/
				/* collision test */
				if(maxdist > 0)
				{
					if((whl == 0) || (whl == 2))
					m->BODYFORCE[whl][1] = maxdist * -1000.0;
					else
					m->BODYFORCE[whl][1] = maxdist * 1000.0;
					opos[ZCOMP] = 0;
				}
/*				opos[ZCOMP] = maxdist + WH_Z_OFFSET;*/
				opos[ZCOMP] = -2.5;
				/* collision test */
				}
#endif
 			else
				{
				fSurfDist(savesp,ipos,&opos[0],uvs);
				}

			*roadcode = savesp->flags & SURF_MASK;
			m->sviscode[whl] = (savesp->flags & SVIS_MASK) >> SVIS_SHIFT;
			m->roadboost[whl] = (savesp->flags & SURF_BOOST_MASK) >> SURF_BOOST_SHIFT;
			m->sound_flags[whl] = savesp->snd_flags;
			m->shortcut_flags[whl] = savesp->cut_flags;
			return; /* for now ret on 1st int */
			}
		}

	/* If here, no surface found. Use ground default */
	*roadcode = DIRT;
	m->sviscode[whl] = DIRT;
	m->roadboost[whl] = 0;
	m->sound_flags[whl] = 0;
	m->shortcut_flags[whl] = 0;
	for(i=0;i<3;i++)
		opos[i] = ipos[i];
	opos[2] -= 200.0; /* default plane -200 ft */

	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			uvs[i][j] = (i == j) ? 1.0 : 0.0; /* set to unit */

}

extern const F32 g_Kcs;
extern const F32 g_Krs;
extern const F32 g_Kcd;
extern const F32 g_Krd;

F32 xxxColDist;
F32 xxxColForce;


S32 bodysurf(MODELDAT *m, F32 ipos[3], F32 ivel[3],F32 uvs[3][3], int whl)
{
	U16 i,j;
	S16 quad;
	S32 sli,tpos[3],dist,maxdist;
	S32 topos[3];
	U16 *slp,snum;
	STREETYP *ltp;
	SURFTYP *sp,*savesp=0;
	F32 v1,v2,force,xyvel,tvel[3],opos[3], maxforce;
	
	/*temp - till updownleaf uses floats */
	for(i=0;i<3;i++)
		tpos[i] = ipos[i] * S4SCALE;
	/*temp - till updownleaf uses floats */

	/* traverse tree from wheel prev leaf */
	ltp = updownleaf(m->lasttp[whl], tpos, &quad);

	sli = -((S32)ltp->child[quad]); /* neg = slist index */
	if(sli)	/* > 0 surfs, check list for intersection */
		{
		slp = &slist[sli];
		snum = *slp++;

		maxdist = MAXSURFPOS;
		for(i=0;i<snum;i++)
			{
			sp = &surf[*slp++];
			if(mInSurf(tpos, topos, sp, &dist)) /* dist pos if tire below surface!!*/
				{

				/* look 1 foot out from surf (40) */
				if((dist < MAXSURFHOP) && (dist > maxdist))
					{
					maxdist = dist;
					savesp = sp;
					}
				}
			}


		/**/
		if((maxdist != MAXSURFPOS)&&(maxdist > -10))
		{
		    /* Get velocity in surf frame of ref */
		    fSurfDist(savesp,ipos,&opos[0],uvs);
		    if(opos[2] > -0.25)	/*more than 3in from surf*/
		    {
			frwtobod(ivel,tvel,uvs[0]);

			switch(savesp->flags & SURF_MASK)
			{
			    case SURF_COLLIDE:
			    	xyvel = xxsqrt(tvel[0]*tvel[0] + tvel[1]*tvel[1]);
				if(xyvel < 0.0001) xyvel = 0.0001;
			    	force = (opos[2] + 0.25) * m->weight * 1.5;/*was 0.6*/
			    	m->BODYFORCE[whl][0] = -force * (tvel[0]/xyvel);
			    	m->BODYFORCE[whl][1] = -force * (tvel[1]/xyvel);
			    	force = COLL_force(m,opos,tvel);
if((m == &model[gThisNode]) && ((whl == 0)||(whl == 1)))
 {
 xxxColDist = opos[2];
 xxxColForce = force;
 }

			        break;
			    default:	/* DIRT or PAVE */
			    	xyvel = xxsqrt(tvel[0]*tvel[0] + tvel[1]*tvel[1]);
				if(xyvel < 0.0001) xyvel = 0.0001;
			    	force = (opos[2] + 0.25) * m->weight * 1.5;/*was 0.6*/
			    	m->BODYFORCE[whl][0] = -force * (tvel[0]/xyvel);
			    	m->BODYFORCE[whl][1] = -force * (tvel[1]/xyvel);
				if(opos[2] < 0.0)
					force = m->weight * -0.25;
				else
					{
					v1 = tvel[2];
					v2 = (opos[2] * -4.0); /* 4fps @ 1foot */
					force = (v2 - v1) * m->mass * 0.25 * m->idt;
					/*force = opos[2] * -100000;*/
					}
			}

#if DEBUGORBIT
			maxforce = 1000000.0;
			if(force > maxforce) force = maxforce;
			else if(force < -maxforce) force = -maxforce;
#endif

			m->BODYFORCE[whl][2] = force;
			fbodtorw(m->BODYFORCE[whl],opos,uvs[0]);
			frwtobod(opos,m->BODYFORCE[whl],m->UV.fpuvs[0]);
			return(1);
			}
		    }
		}
	return(0);
}

/*const S32 g_collalg = 0;*/
const S32 g_collalg = 4;

F32 COLL_force(MODELDAT *m,F32 r[3],F32 v[3])
{
	F32 force;

	switch(g_collalg)
		{
		case 0:	force = COLL_test0(m,r,v);	break;
		case 1:	force = COLL_test1(m,r,v);	break;
		case 2:	force = COLL_test2(m,r,v);	break;
		case 3:	force = COLL_test3(m,r,v);	break;
		case 4:	force = COLL_test4(m,r,v);	break;
		default:force = COLL_test0(m,r,v);	break;
		}
	return(force);
}

F32 COLL_test4(MODELDAT *m,F32 r[3],F32 v[3])
{
	F32 v1,Kd,Ks,force;
	F32 din,vin;

	din = r[2];
	vin = v[2];

	if(din > 0.25)
		/* if more than 0.25 feet into surf - force velocity to 1fps out. */
		force = (vin + 1.0) * m->mass * m->idt * -0.5;
	else
		/* medium scrape force = 4000 ft-lb. */
		/* spring force s.t. medium scrap dist = 0.0. (16000 * 0.25) */
		force = ((din + 0.25) * -16000.0) + (vin * -2000.0);

	if(force > 0.0) force = 0.0;    /*only force AWAY*/
	return(force);
    }

F32 COLL_test3(MODELDAT *m,F32 r[3],F32 v[3])
{
	F32 v1,Kd,Ks,force;
    
 	v1 = v[2];

	if(r[2] < 0.0)	/* outside 0.25 ft*/
		force = (0.25 + r[2]) * -80000.0;
	else
		force = (v1 + 5.0) * m->mass * m->idt * -0.5;

	if(force > 0.0) force = -10.0;    /*only force AWAY*/
	return(force);
    }

F32 COLL_test0(MODELDAT *m,F32 r[3],F32 v[3])
{
    F32 v1,Kd,Ks,force;
    
#if 1
    v1 = v[2];
    if(v1 > 0.0)	/* compression */
    	force = ((r[2] + 0.25) * g_Kcs) + (v1 * v1 * (g_Kcd * 1.5));
    else		/* rebound */
    	force = ((r[2] + 0.25) * g_Krs) - (v1 * v1 * g_Krd);
    if(force > 0.0) force = -10.0;    /*only force AWAY*/
	else if(force < -100000.0) force = -100000.0;
#else
    v1 = v[2];
    if(v1 > 0.0)	/* compression */
    	force = ((r[2] + 0.25) * g_Kcs) + (v1 * v1 * (g_Kcd * 1.5));
    else		/* rebound */
    	force = ((r[2] + 0.25) * g_Krs) - (v1 * v1 * g_Krd);
    if(force > 0.0) force = 0.0;    /*only force AWAY*/
#endif
    return(force);
    }

/* spring outside, velocity fix on inside */
F32 COLL_test1(MODELDAT *m,F32 r[3],F32 v[3])
{
    F32 v1,v2,force;


    if(r[2] < 0.0)
    	force = r[2] * m->weight * 40.0;
    else
    {
	v1 = v[2];
	v2 = (r[2] * -8.0) - 4.0; /* 5fps @ 1foot */
	force = (v2 - v1) * m->mass * 0.25 * m->idt;
	}
    return(force);
    }

/* simple spring */
F32 COLL_test2(MODELDAT *m,F32 r[3],F32 v[3])
{
    F32 force;
    
    if(r[2] < -0.25)
    	force = 0.0;
    else
    	force = (r[2] + 0.25) * m->weight * -40.0;
    return(force);
    }

static void mats2fcopy(S16 *sp,F32 *fp)
{
    S16 i;
    
    for(i=0;i<9;i++)
    	*fp++ = (F32)*sp++ * 1.0/0x4000;
}

static void veci2fcopy(S32 *lp,F32 *fp)
{
    S16 i;
    
    for(i=0;i<3;i++)
    	*fp++ = (F32)*lp++ * INVS4SCALE;
}


void fSurfDist(SURFTYP *sp, F32 ipos[3], F32 opos[3], F32 uv[3][3])
{
	S16 i;
	U16 *vlp,vnum,vi;
	SVERTTYP *vp;
	float fpos[3];
    
	vlp = &vlist[sp->vli];
	vnum = *vlp++;
	vi = *vlp++; /* read first vert index */
	vp = &svert[vi];
	for(i=0;i<3;i++)
		fpos[i] = ipos[i] - (vp->pos[i] * INVS4SCALE);
	mats2fcopy(sp->uvs[0],&uv[0][0]);
	frwtobod(fpos,opos,uv[0]);
}



S16 mInSurf(S32 ipos[3], S32 opos[3], SURFTYP *sp, S32 *dist)
{
	S32 i,spos[3],tpos[3];
	U16 *vlp,vnum,vi;
	SVERTTYP *vp;
	float nz,pos[3],vax,vay,vbx,vby;


	vlp = &vlist[sp->vli];
	vnum = *vlp++;

	vi = *vlp++; /* read first vert index */
	vp = &svert[vi];
	/* tpos = testpos - surfpos */
	ivecsub(ipos, vp->pos, tpos);
	/* get testpos into surface coordinate system */
	irwtobod(tpos, spos, (struct uvect *)(sp->uvs));
	pos[0] = (float)spos[0];
	pos[1] = (float)spos[1];
	pos[2] = (float)spos[2];

	/* 1st vertex = [0,0,0] in surface frame of ref after xform. */
	/* Special case last-to-1st and 1st-to-2nd edge (simpler). */
	/* This routine assumes minimum of 3 verts. */
	vi = vlp[vnum-2]; /* read last vert index */
	vp = &svert[vi];
	vax = (float)vp->pos[XCOMP];
	vay = (float)vp->pos[YCOMP];
	nz = ((vax - pos[XCOMP]) * vay) - ((vay - pos[YCOMP]) * vax);
	if(nz < 0.0) return(0);	/* pos outside */

	vi = *vlp++; /* read 2nd vert index */
	vp = &svert[vi];
	vax = (float)vp->pos[XCOMP];
	vay = (float)vp->pos[YCOMP];
	nz = (pos[XCOMP] * vay) - (pos[YCOMP] * vax);
	if(nz < 0.0) return(0);	/* pos outside */

	for(i=2;i<vnum;i++)
		{
		vi = *vlp++;
		vp = &svert[vi];
		vbx = (float)vp->pos[XCOMP];
		vby = (float)vp->pos[YCOMP];
		nz = ((pos[XCOMP]-vax)*(vby-vay)) - ((pos[YCOMP]-vay)*(vbx-vax));
		if(nz < 0.0) return(0);	/* pos outside */
		vax = vbx;
		vay = vby;
		}
/*	if(spos[ZCOMP] > 80)*/	/* must be higher level surface */
/*		return(0);*/
	*dist = spos[ZCOMP];
	ibodtorw(spos, opos, (struct uvect *)(sp->uvs));
	ivecsub(opos, ipos, opos);
	return(1);
}

/* all iteration vars */
short stempa[3],stempb[3],stemp[3];
S32 temp[3],gpos[3];
/* all iteration vars */

#define CUBIC_SURF 1

#if CUBIC_SURF
F32 p0v[3],p0uv[3][3];
F32 p3v[3],p3uv[3][3];
#endif
F32 p1v[3],p1uv[3][3];
F32 p2v[3],p2uv[3][3];
F32 terpv[3],terpuv[3][3];
F32 fgpos[3],ftemp[3],ftempa[3],ftempb[3];


void xfPathInterp(SURFTYP *sp, F32 ipos[3], F32 opos[3], F32 uvs[3][3])
{
	S32 i;
	F32 dist,del1,del2,interp;
#if CUBIC_SURF
	PATHTYP *p0p,*p1p,*p2p,*p3p;
	F32 m0,m1;
#else
	PATHTYP *p1p,*p2p;
#endif
	U16 *vlp,vnum,pi;

	/* get road uv index */
	vlp = &vlist[sp->vli];
	vnum = *vlp++;
	vlp += vnum; /* skip past vert indexes */
	pi = *vlp;
#if CUBIC_SURF
	p0p = (pi > 0) ? &path[pi - 1] : &path[pi];
#endif
	p1p = &path[pi];
	p2p = &path[pi + 1];
#if CUBIC_SURF
	p3p = &path[pi + 2];
#endif

	/* create leading and trailing surfs (pos+uvs) */
#if CUBIC_SURF
	veci2fcopy(p0p->pos,p0v);
	mats2fcopy(p0p->uvs[0],p0uv[0]);
#endif
	veci2fcopy(p1p->pos,p1v);
	mats2fcopy(p1p->uvs[0],p1uv[0]);
	veci2fcopy(p2p->pos,p2v);
	mats2fcopy(p2p->uvs[0],p2uv[0]);
#if CUBIC_SURF
	veci2fcopy(p3p->pos,p3v);
	mats2fcopy(p3p->uvs[0],p3uv[0]);
#endif

	/* create temp interp surf and get relative offset */
	dist = AlignSurf(p1v,p1uv,p2v,p2uv,terpv,terpuv);
	surfpos(terpv,terpuv[0],ipos,fgpos);

	/* find angle del for leading edge */
	for(i=0;i<3;i++)
		ftemp[i] = p1uv[i][1];
	frwtobod(ftemp,ftempa,terpuv[0]);

	if(ftempa[1] == 0.0)
		del1 = 0.0;
	else
		del1 = (ftempa[0]*fgpos[1])/ftempa[1];

	/* find angle del for trailing edge */
	for(i=0;i<3;i++)
		ftemp[i] = p2uv[i][1];
	frwtobod(ftemp,ftempb,terpuv[0]);

	if(ftempb[1] == 0.0)
		del2 = 0.0;
	else
		del2 = (ftempb[0]*fgpos[1])/ftempb[1];

	/* modify size and comp for edge angels */
	interp = (fgpos[XCOMP] - del1)/(dist + del2 - del1);

	fvecinterp(p1v,p2v,interp,terpv);
	fuvinterp(p1uv[0],p2uv[0],interp,uvs[0]);
/*#if CUBIC_SURF*/
#if 0
	{
	F32 offset,i2,imag;

#if 0
#if 0
	for(i=0;i<3;i++)
		ftemp[i] = p0uv[i][0];
	frwtobod(ftemp,ftempb,terpuv[0]);
	m0 = (ftempb[2]/ftempb[0]) * 0.5;

	for(i=0;i<3;i++)
		ftemp[i] = p2uv[i][0];
	frwtobod(ftemp,ftempb,terpuv[0]);
	m1 = (ftempb[2]/ftempb[0]) * 0.5;
#else
	for(i=0;i<3;i++)
		ftemp[i] = p1v[i] - p0v[i];
	frwtobod(ftemp,ftempb,terpuv[0]);
	m0 = (ftempb[0] == 0.0) ? 0.0 : (ftempb[2]/ftempb[0]) * 0.5;

	for(i=0;i<3;i++)
		ftemp[i] = p3v[i] - p2v[i];
	frwtobod(ftemp,ftempb,terpuv[0]);
	m1 = (ftempb[0] == 0.0) ? 0.0 : (ftempb[2]/ftempb[0]) * 0.5;
#endif
#endif
	dist = getSlopes(pi,terpuv,fgpos[YCOMP],&m0,&m1);

	i2 = interp * interp;
/*	offset = (interp * i2 * m1) - (i2 * m1);*/
	offset = (interp * i2 * (m0 + m1)) - (i2 * (m0 + m0 + m1)) + (interp * m0);
	offset *= (dist * 1.0);
/*	offset = (interp < 0.5) ? interp * dist : (1.0 - interp) * dist;*/
	for(i=0;i<3;i++)
		terpv[i] += (offset * uvs[i][2]);
	}
#endif
	surfpos(terpv,uvs[0],ipos,opos);
}

extern S32 xSaveRoadInfo;
F32 xDel1,xDel2,xInterp;
F32 xUv[3][3];
F32 xFRpos[3];

void fPathInterp(SURFTYP *sp, F32 ipos[3], F32 opos[3], F32 uvs[3][3])
{
	S32 i,j;
	U16 *vlp,vnum;
	PATHTYP *p0p,*p1p;
	F32 xi,yi;
	F32 vx0[3],vx1[3],vx[3],vy[3],vz[3];
	F32 pos0[3],pos1[3],pos[3];
	S32 p0,p1;
	F32 m0,m1,length,i2,offset;

	/* get road uv index */
	vlp = &vlist[sp->vli];
	vnum = *vlp++;
	vlp += vnum; /* skip past vert indexes */
	p0 = *vlp;
	p1 = ((p0+1) < gRoadHead->pnum) ? p0+1 : gRoadHead->pathloop;
	p0p = &path[p0];
	p1p = &path[p1];

	/* determine x and y interpolation values*/
	surfpos(p0p->fpos,p0p->fuvs[0],ipos,fgpos);
	length = p0p->w + (fgpos[1] * (p0p->m1 - p0p->m0));
	xi = (fgpos[0] - (fgpos[1] * p0p->m0))/length;
	if (fgpos[1] > 0.0)
		yi = 0.5 + ((fgpos[1]/(p0p->wr + (fgpos[0] * p0p->mr))) * 0.5);
	else
		yi = 0.5 + ((fgpos[1]/(p0p->wl + (fgpos[0] * p0p->ml))) * -0.5);

	/*interpolate x vector*/
	fvecinterp(p0p->vl,p0p->vr,yi,vx0);
	fvecinterp(p1p->vl,p1p->vr,yi,vx1);
	fvecinterp(vx0,vx1,xi,vx);

	/*interpolate y vector*/
	fvecinterp(p0p->ve,p1p->ve,xi,vy);

	/* generate z vector*/
	crossprod(vx, vy, vz);

	/* make uvs from direction vectors */
	for(i=0;i<3;i++)
		{
		uvs[i][0] = vx[i];
		uvs[i][1] = vy[i];
		uvs[i][2] = vz[i];
		}	

	/*interpolate road surface*/
	for(i=0;i<3;i++) pos0[i] = p0p->fpos[i] + ((yi - 0.5) * p0p->fwidth * p0p->ve[i]);
	for(i=0;i<3;i++) pos1[i] = p1p->fpos[i] + ((yi - 0.5) * p1p->fwidth * p1p->ve[i]);
	fvecinterp(pos0,pos1,xi,pos);

	/*loft surface*/
	m0 = p0p->m0l + ((p0p->m0r - p0p->m0l) * yi);
/*	m1 = p0p->m1l + ((p0p->m1r - p0p->m1l) * yi);*/
/*	m1 = p1p->m0l + ((p1p->m0r - p1p->m0l) * yi);*/
	m1 = -(p1p->m0l + ((p1p->m0r - p1p->m0l) * yi));
#if 1

	i2 = xi * xi;
	offset = (xi * i2 * (m0 + m1)) - (i2 * (m0 + m0 + m1)) + (xi * m0);
/*	offset = (xi * i2 * (m0 - m1)) - (i2 * (m0 + m0 - m1)) + (xi * m0);*/
	offset *= (length * 1.0);
	for(i=0;i<3;i++)
		pos[i] += (offset * uvs[i][2]);
#endif

	/*get position relative to interpolated surface*/
	surfpos(pos,uvs[0],ipos,opos);


if(xSaveRoadInfo)
 {
 xDel1 = m0;
 xDel2 = m1;
 xInterp = offset;
 for(i=0;i<3;i++)
  for(j=0;j<3;j++)
   xUv[i][j] = uvs[i][j];
 for(i=0;i<3;i++)
  xFRpos[i] = ipos[i];
 }


}

F32 getSlopes(F32 surfuv[3][3],F32 v0[3],F32 v1[3],F32 v2[3],F32 v3[3],
	F32 uv0[3][3],F32 uv1[3][3],F32 uv2[3][3],F32 uv3[3][3],F32 ypos,F32 width,F32 *ma, F32 *mb)
{
	S32 i;
	F32 temp[3],xtan[3],m;


	for(i=0;i<3;i++) temp[i] = uv1[i][0];
	frwtobod(temp,xtan,surfuv[0]);
	m = (xxsqrt(xtan[1]*xtan[1] + xtan[2]*xtan[2]))/xtan[0];
	*ma = (xtan[2] < 0.0) ? -m : m;

	for(i=0;i<3;i++) temp[i] = uv2[i][0];
	frwtobod(temp,xtan,surfuv[0]);
	m = (xxsqrt(xtan[1]*xtan[1] + xtan[2]*xtan[2]))/xtan[0];
/*	*mb = (xtan[2] < 0.0) ? m : -m;*/
	*mb = 0.0;
}

#if 0
/*F32 getSlopes(U16 pi,F32 surfuv[3][3],F32 ypos,F32 *ma, F32 *mb)*/
F32 getSlopes(F32 surfuv[3][3],F32 v0[3],F32 v1[3],F32 v2[3],F32 v3[3],
	F32 uv0[3][3],F32 uv1[3][3],F32 uv2[3][3],F32 uv3[3][3],F32 ypos,F32 width,F32 *ma, F32 *mb)
{
	S32 i;
	F32 len,interp,temp[3];
	F32 p0[3],p1[3],p2[3],p3[3];
	F32 ml0,ml1,ml2,mr0,mr1,mr2;
	F32 mla,mlb,mra,mrb;

	/*calculate left slopes*/
	len = width * -0.5;
	for(i=0;i<3;i++) temp[i] = (uv0[i][1] * len) + v0[i];
	vecsub(temp,v1,temp);
	frwtobod(temp,p0,surfuv[0]);
	for(i=0;i<3;i++) temp[i] = (uv1[i][1] * len) + v1[i];
	vecsub(temp,v1,temp);
	frwtobod(temp,p1,surfuv[0]);
	for(i=0;i<3;i++) temp[i] = (uv2[i][1] * len) + v2[i];
	vecsub(temp,v1,temp);
	frwtobod(temp,p2,surfuv[0]);
	for(i=0;i<3;i++) temp[i] = (uv3[i][1] * len) + v3[i];
	vecsub(temp,v1,temp);
	frwtobod(temp,p3,surfuv[0]);
	ml0 = (p1[0] == p0[0]) ? 0.0 : (p1[2] - p0[2])/(p1[0] - p0[0]);
	ml1 = (p2[0] == p1[0]) ? 0.0 : (p2[2] - p1[2])/(p2[0] - p1[0]);
	ml2 = (p3[0] == p2[0]) ? 0.0 : (p3[2] - p2[2])/(p3[0] - p2[0]);
	mla = (ml0 - ml1) * 0.5;
	mlb = (ml1 - ml2) * 0.5;

	/*calculate right slopes*/
	len = width * 0.5;
	for(i=0;i<3;i++) temp[i] = (uv0[i][1] * len) + v0[i];
	vecsub(temp,v1,temp);
	frwtobod(temp,p0,surfuv[0]);
	for(i=0;i<3;i++) temp[i] = (uv1[i][1] * len) + v1[i];
	vecsub(temp,v1,temp);
	frwtobod(temp,p1,surfuv[0]);
	for(i=0;i<3;i++) temp[i] = (uv2[i][1] * len) + v2[i];
	vecsub(temp,v1,temp);
	frwtobod(temp,p2,surfuv[0]);
	for(i=0;i<3;i++) temp[i] = (uv3[i][1] * len) + v3[i];
	vecsub(temp,v1,temp);
	frwtobod(temp,p3,surfuv[0]);
	mr0 = (p1[0] == p0[0]) ? 0.0 : (p1[2] - p0[2])/(p1[0] - p0[0]);
	mr1 = (p2[0] == p1[0]) ? 0.0 : (p2[2] - p1[2])/(p2[0] - p1[0]);
	mr2 = (p3[0] == p2[0]) ? 0.0 : (p3[2] - p2[2])/(p3[0] - p2[0]);
	mra = (mr0 - mr1) * 0.5;
	mrb = (mr1 - mr2) * 0.5;

	interp = ypos/width + 0.5;
	*ma = mla + ((mra - mla)*interp);
	*mb = mlb + ((mrb - mlb)*interp);
}
#endif

#if 0
F32 getSlopes(U16 pi,F32 surfuv[3][3],F32 ypos,F32 *ma, F32 *mb)
{
	S32 i;
	S32 p0,p1,p2,p3,pnum;
	PATHTYP *p0p,*p1p,*p2p,*p3p;
	F32 width,len,interp,temp[3],pos[3];
	F32 v0[3],v1[3],v2[3],v3[3];
	F32 m0,m1,m2,mla,mlb,mra,mrb;
	F32 distl,distr;

	pnum = gRoadHead->pnum;
	p1 = pi;
	p0 = (p1 == 0) ? pnum - 2 : p1 - 1;
	p2 = ((p1+1) < pnum) ? p1+1 : gRoadHead->pathloop;
	p3 = ((p1+2) < pnum) ? p1+2 : (p1+2) - pnum + gRoadHead->pathloop;

	p0p = &path[p0];
	p1p = &path[p1];
	p2p = &path[p2];
	p3p = &path[p3];

	width = (F32)p1p->width * 0.025;	/* 1/40 */
	for(i=0;i<3;i++)
		pos[i] = (F32)p1p->pos[i] * 0.025;

	/*calculate left slopes*/
	len = width * -0.5;
	for(i=0;i<3;i++) temp[i] = (F32)p0p->uvs[i][1] * len * 1.0/0x4000 + (F32)p0p->pos[i] * 0.025;
	vecsub(temp,pos,temp);
	frwtobod(temp,v0,surfuv[0]);
	for(i=0;i<3;i++) temp[i] = (F32)p1p->uvs[i][1] * len * 1.0/0x4000 + (F32)p1p->pos[i] * 0.025;
	vecsub(temp,pos,temp);
	frwtobod(temp,v1,surfuv[0]);
	for(i=0;i<3;i++) temp[i] = (F32)p2p->uvs[i][1] * len * 1.0/0x4000 + (F32)p2p->pos[i] * 0.025;
	vecsub(temp,pos,temp);
	frwtobod(temp,v2,surfuv[0]);
	for(i=0;i<3;i++) temp[i] = (F32)p3p->uvs[i][1] * len * 1.0/0x4000 + (F32)p3p->pos[i] * 0.025;
	vecsub(temp,pos,temp);
	frwtobod(temp,v3,surfuv[0]);
	m0 = (v1[0] == v0[0]) ? 0.0 : (v1[2] - v0[2])/(v1[0] - v0[0]);
	m1 = (v2[0] == v1[0]) ? 0.0 : (v2[2] - v1[2])/(v2[0] - v1[0]);
	m2 = (v3[0] == v2[0]) ? 0.0 : (v3[2] - v2[2])/(v3[0] - v2[0]);
	mla = (m0 + m1) * 0.5;
	mlb = (m1 + m2) * 0.5;
	distl = v2[0] - v1[0];

	/*calculate right slopes*/
	len = width * 0.5;
	for(i=0;i<3;i++) temp[i] = (F32)p0p->uvs[i][1] * len * 1.0/0x4000 + (F32)p0p->pos[i] * 0.025;
	vecsub(temp,pos,temp);
	frwtobod(temp,v0,surfuv[0]);
	for(i=0;i<3;i++) temp[i] = (F32)p1p->uvs[i][1] * len * 1.0/0x4000 + (F32)p1p->pos[i] * 0.025;
	vecsub(temp,pos,temp);
	frwtobod(temp,v1,surfuv[0]);
	for(i=0;i<3;i++) temp[i] = (F32)p2p->uvs[i][1] * len * 1.0/0x4000 + (F32)p2p->pos[i] * 0.025;
	vecsub(temp,pos,temp);
	frwtobod(temp,v2,surfuv[0]);
	for(i=0;i<3;i++) temp[i] = (F32)p3p->uvs[i][1] * len * 1.0/0x4000 + (F32)p3p->pos[i] * 0.025;
	vecsub(temp,pos,temp);
	frwtobod(temp,v3,surfuv[0]);
	m0 = (v1[0] == v0[0]) ? 0.0 : (v1[2] - v0[2])/(v1[0] - v0[0]);
	m1 = (v2[0] == v1[0]) ? 0.0 : (v2[2] - v1[2])/(v2[0] - v1[0]);
	m2 = (v3[0] == v2[0]) ? 0.0 : (v3[2] - v2[2])/(v3[0] - v2[0]);
	mra = (m0 + m1) * 0.5;
	mrb = (m1 + m2) * 0.5;
	distr = v2[0] - v1[0];

	interp = ypos/width + 0.5;
	*ma = mla + ((mra - mla)*interp);
	*mb = mlb + ((mrb - mlb)*interp);
	return(distl + ((distr - distl)*interp));
}
#endif

void frwtobod(F32 *vprime, F32 *v, F32 *uvp)
{
	F32 r;
	int i;
	
	i=3;
	do{
		r = *vprime++ * *uvp;
		uvp += 3;
		r += *vprime++ * *uvp;
		uvp += 3;
		*v++ = r + *vprime++ * *uvp;
		uvp -= 5;
		vprime -= 3;
	}while(--i);
}

void fbodtorw(F32 *v, F32 *vprime, F32 *uvp)
{
	register F32 r;
	register int i;
	
	i=3;
	do{
		r = *v++ * *uvp++;
		r += *v++ * *uvp++;
		*vprime++ = r + *v++ * *uvp++;
		v -= 3;
	}while(--i);
}


void surfpos(F32 *pos, F32 *uvs, F32 *pt, F32 *rpt)
{
	F32 temp[3];

	vecsub(pt,pos,temp);
	frwtobod(temp,rpt,uvs);
}

void fuvinterp(F32 *uv1, F32 *uv2, F32 t, F32 *uvr)
{
	F32 tp;
	S16 i;

	tp = 1.0 - t;
	for(i=0;i<9;i++) {
		*uvr++ = (*uv1++ * tp) + (*uv2++ * t);
        }
}

void fvecinterp(F32 *v1, F32 *v2, F32 t, F32 *vr)
{
	F32 tp;
	S16 i;

	tp = 1.0 - t;
	for(i=0;i<3;i++)
		*vr++ = (*v1++ * tp) + (*v2++ * t);
}



F32 AlignSurf(F32 v1[3], F32 uv1[3][3], F32 v2[3], F32 uv2[3][3], F32 *v, F32 uv[3][3])
{
	short i;
	F32 vx[3],vy[3],vz[3];
	F32 tv[3],dist,inv;

	veccopy(v1,v);
	veccopy(v2,tv);

	for(i=0;i<3;i++)
		{
		vx[i] = tv[i] - v[i];
		vy[i] = uv1[i][1];
		}

    	/* normalize vx and get dist */
	dist = xxsqrt((vx[0]*vx[0]) + (vx[1]*vx[1]) + (vx[2]*vx[2]));
	inv = 1.0/dist;
	for(i=0;i<3;i++)
		vx[i] *= inv;

	crossprod(vx, vy, vz);
	direction(vz, vz);	/*normalize (vx and vy mat not be orthogonal)*/
	crossprod(vz, vx, vy);

	for(i=0;i<3;i++)
		{
		uv[i][0] = vx[i];
		uv[i][1] = vy[i];
		uv[i][2] = vz[i];
		}
	return(dist);
}

#if 0
/*F32 AlignSurf(PATHTYP *p1p, PATHTYP *p2p, F32 *v, F32 uv[3][3])*/
F32 AlignSurf(F32 v1[3], F32 uv1[3][3], F32 v2[3], F32 uv2[3][3], F32 *v, F32 uv[3][3])
{
	short i;
	F32 vx[3],vy[3],vz[3];
	F32 tv[3],dist,inv;

	veccopy(v1,v);
	veccopy(v2,tv);

	for(i=0;i<3;i++)
		{
		vx[i] = tv[i] - v[i];
		vz[i] = uv1[i][2];
		}

    	/* normalize vx and get dist */
	dist = xxsqrt((vx[0]*vx[0]) + (vx[1]*vx[1]) + (vx[2]*vx[2]));
	inv = 1.0/dist;
	for(i=0;i<3;i++)
		vx[i] *= inv;

	crossprod(vz, vx, vy);

	for(i=0;i<3;i++)
		{
		uv[i][0] = vx[i];
		uv[i][1] = vy[i];
		uv[i][2] = vz[i];
		}
	return(dist);
}
#endif

#if 0
F32 oldAlignSurf(PATHTYP *p1p, PATHTYP *p2p, F32 *v, F32 uv[3][3])
{
	short i;
	F32 vx[3],vy[3],vz[3];
	F32 tv[3],dist,inv;

	veci2fcopy(p1p->pos,v);
	veci2fcopy(p2p->pos,tv);


	inv = 1.0/0x4000;
	for(i=0;i<3;i++)
		{
		vx[i] = tv[i] - v[i];
		vz[i] = (F32)p1p->uvs[i][2] * inv;
		}

    	/* normalize vx and get dist */
	dist = xxsqrt((vx[0]*vx[0]) + (vx[1]*vx[1]) + (vx[2]*vx[2]));
	inv = 1.0/dist;
	for(i=0;i<3;i++)
		vx[i] *= inv;


	crossprod(vz, vx, vy);

	for(i=0;i<3;i++)
		{
		uv[i][0] = vx[i];
		uv[i][1] = vy[i];
		uv[i][2] = vz[i];
		}
	return(dist);
}
#endif

/* Searches up and down tree for leaf node. */
/* If pos is outside bounds, downleaf is called */
/* to find closest node/quad. */
STREETYP *updownleaf(STREETYP *tp, S32 pos[3], S16 *quad)
{
	S16 out;

	out = ((pos[HPOS] > tp->maxh) || (pos[HPOS] < tp->minh) ||
	       (pos[VPOS] > tp->maxv) || (pos[VPOS] < tp->minv));

	if(out && (tp->parent != -1))
		return(updownleaf(&stree[tp->parent], pos, quad));
	else
		return(downleaf(tp, pos, quad));

}

/* down search only */
/*STREETYP *downleaf(STREETYP *tp, S32 pos[3], S16 *quad)*/
STREETYP *downleaf(STREETYP *tp, S32 pos[3], S16 *quad)
{
	S32 midh,midv;
	S32 child;
	S16 q;


	/* get quadrant bounds */
	midh = (tp->minh + tp->maxh) >> 1;
	midv = (tp->minv + tp->maxv) >> 1;

	/* determine quadrant */
	q = (pos[HPOS] < midh) ? 0 : 1;
	if(pos[VPOS] < midv)
		q += 2;
	child = tp->child[q];
	if(child > 0)	/* not a leaf, keep looking */
		return(downleaf(&stree[child], pos, quad));
	else		/* found leaf, all done */
		{
		*quad = q;
		return(tp);
		}
}

