
#ifndef _TIREDES_H
#define _TIREDES_H

/* Tire parameters for model */
typedef struct tiredes {
	F32 tradius;		/* radius of tire */
	F32 springK;		/* static lateral stiffness */
	F32 rubdamp;		/* lateral damping */
	F32 PaveCstiff;	/* cornering stiffness lb/radian on pvt */
	F32 PaveCfmax;	/* maximum sliding friction coefficient, pvt*/
	F32 Cstiff;		/* cornering stiffness lb/radian */
	F32 Cfmax;		/* maximum sliding friction coefficient */
	F32 invmi;		/* 1/moment of inertia of wheel */
	F32 Zforce;		/* loading for cornering stiffness */
	F32 Afmax;		/* alpha where maximum occurs */
	F32 k1;		/* Cf = k1a + k2 abs(a)*a + k3 a^3 */
	F32 k2;
	F32 k3;
	F32 l2;
	F32 l3;
	F32 m1,m2,m3,m4;	/* coefficients for aligning torque equation */
	F32 patchy;
	F32 angvel;		/* rotational speed of tire */
	F32 sliptorque;	/* torque from tire slipping */
	F32 sideforce;	/* for tire squeal sound */
	F32 traction;
	char  slipflag;		/* flag that tire is slipping */
} tiredes;


#endif
/* end #ifndef _TIREDES_H */

