#include "drivsym.h"
#include "modelcom.h"
#include "road.h"
#include "stree.h"
#include "targets.h"

#include "Pro/stree.pro"
#include "Pro/unitvecs.pro"
#include "Pro/math.pro"
#include "Pro/communic.pro"
#include "Pro/vecmath.pro"
#include "Pro/drivsym.pro"
#include "Pro/collision.pro"

#define degtosdeg (0x4000/90)


void init_road(MODELDAT *m)
{
	F32 temp[3];
	S32 i;

    	/* Create unit vects */
	fmatcopy(&m->initin.uvs[0][0],&m->UV.fpuvs[0][0]);
	makesuvs(&m->UV);	/* Make short uvs */

    	/* Get offset relative to start uvs */
	bodtorw(m->initin.offset,temp,&m->UV);

    	/* RWR = pos + relative offset */
	vecadd(m->initin.pos,temp,m->RWR);
	epveccopy(m->RWR,m->EPRWR);	/* Initialize extended precision radius */

	veccopy(m->RWR,m->reckon.base_RWR);

	/* Find	tire positions in real world */
	for(i=0;i<4;++i){
		veccopy(m->TIRER[i],temp);
		bodtorw(temp,m->TIRERWR[i],&m->UV);
		vecadd(m->TIRERWR[i],m->RWR,m->TIRERWR[i]);
		m->suscomp[i] = 0.0;
		m->tpcomp[i] = 0.0;
		m->roadcode[i] = m->sviscode[i] = AIR;
		m->roadboost[i] = 0;
		m->sound_flags[i] = m->shortcut_flags[i] = 0;
		veccopy(m->BODYR[i],temp);
		bodtorw(temp,m->BODYRWR[i],&m->UV);
		vecadd(m->BODYRWR[i],m->RWR,m->BODYRWR[i]);
	}
}


F32 BODYRWV[3];
F32 tuvs[3][3];

S32 xSaveRoadInfo;
extern MODELDAT model[];
extern U8 gThisNode;

void road(MODELDAT *m)
{	
	F32 igpos[3],temp[3],last_airdist;
	int i,j;

	/*zero surface forces*/
	for(i=0;i<3;i++)
		{
		m->CENTERFORCE[i] = 0.0;
		for(j=0;j<4;j++)
			m->BODYFORCE[j][i] = 0.0;
		}

	for(i=0;i<4;++i)
		{
		if((m == &model[gThisNode]) && (i == 0))
			xSaveRoadInfo = 1;
		else
			xSaveRoadInfo = 0;

		/* find tire interaction with surfaces. */
		tiresurf(m,&m->TIRERWR[i][0],igpos,&m->roadcode[i],m->roaduvs[i].fpuvs, i);
		makesuvs(&m->roaduvs[i]);

		/* find car body interaction with surfaces. */
		/* BODYV = V + (W x BODYR) */
		crossprod(m->W,m->BODYR[i],temp);
		vecadd(temp,m->V,temp);
		bodtorw(temp,BODYRWV,&m->UV);
		bodysurf(m,m->BODYRWR[i],BODYRWV,tuvs,i);

		/* update suspension compression */
		/* make tire test point from last position. */
		last_airdist = m->airdist[i];
		m->suscomp[i] = m->tpcomp[i] + igpos[ZCOMP];
		m->airdist[i] = -m->suscomp[i];
		if(m->suscomp[i] > 3.0)
			m->suscomp[i] = 3.0;
		else if(m->suscomp[i] < -3.0)
			m->suscomp[i] = -3.0;
		m->tpcomp[i] = (m->suscomp[i] < 0) ? 0.0 : m->suscomp[i];
		if(m->suscomp[i] < 0.0)
			m->roadcode[i] = m->sviscode[i] = AIR;
		m->airvel[i] = (m->airdist[i] - last_airdist) * m->idt;
		if(m->airvel[i] < -40.0)
			m->airvel[i] = -40.0;
		else if(m->airvel[i] > 40.0)
			m->airvel[i] = 40.0;
		}
	collision(m);
}

/* This function interpolates unit vectors; c1 out of s1 uv1 and 
c2 out of s2 uv2 */

#if 0
void uvinterp(short uv1[9], int c1, int s1, short uv2[9], short res[9])
#else
void uvinterp(short *uv1, int c1, int s1, short *uv2, short *res)
#endif
{
	register int i;
	register short *p1,*p2,*rp;
	float fc1,fc2,fis1;
	
	fc1 = c1;
	fc2 = s1-c1;
	fis1 = 1.0/(float)s1;
	
	for(i=0,p1=uv1,p2=uv2,rp=res;i < 9; ++i)
		*rp++ = ((*p1++ * fc1) + (*p2++ * fc2))*fis1;
}

void vecinterp(int v1[3], int c1, int s1, int v2[3], int res[3])
{
	register int i,*p1,*p2,*rp;
	float fc1,fc2,fis1;
	
	fc1 = c1;
	fc2 = s1-c1;
	fis1 = 1.0/(float)s1;
	for(i=0,p1=v1,p2=v2,rp=res;i < 3; ++i)
		*rp++ = ((*p1++ * fc1) + (*p2++ * fc2))*fis1;
}

	
