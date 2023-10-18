/******************************************************************************
 * Collision routines to detect the intersection of 'balloons.'
 * The balloons are described by a line segment with a radius - 
 * a cylinder with rounded ends. The name comes from the long balloons
 * they use at the fair to make balloon animals. Our purpose is to
 * make balloon people for quick and accurate collisions.
 #-------------------------------[1994, BR]------------------------------------
 * BalloonCollide	Returns true if the skins of the balloons intersect
 * BalloonInit		Converts a line start, end, radius into faster internal for
 * BalloonMove		Transforms balloon info based on current segment transform
 *****************************************************************************/
#include "stdtypes.h"
#include "fmath.h"
#include "balloon.h"

/* Returns true if the skins of the balloons intersect */
F32 BCDVec[3];
F32	BalloonCollide(Balloon *b1,Balloon *b2)
{
	F32 pos_diff[3],v_cross[3],near1[3],near2[3];
	F32	v_cross_mag,scale_line1,scale_line2,dist,t;

	SubVector(b2->CurrPos,b1->CurrPos,pos_diff);
	t = b1->Length + b2->Length;
	if (VectorLength2(pos_diff) > t*t) return(0);

	CrossVector(b1->CurrVec,b2->CurrVec,v_cross);
	v_cross_mag = VectorLength2(v_cross);
	if (v_cross_mag)
	{
		scale_line1 = Det3Vectors(pos_diff,b2->CurrVec,v_cross)/v_cross_mag;
		if (scale_line1 + b2->Radius < -b1->Length
			|| scale_line1 - b2->Radius > b1->Length)
		{
			return(0);
		}
		scale_line2 = Det3Vectors(pos_diff,b1->CurrVec,v_cross)/v_cross_mag;
		if (scale_line2 + b1->Radius < -b2->Length
			|| scale_line2 - b1->Radius > b2->Length)
		{
			return(0);
		}
	}
	else /* zero cross product means lines are parallel */
	{
		scale_line1 = scale_line2 = 0;
	}
	ScaleVector(b1->CurrVec,scale_line1,near1);
	ScaleVector(b2->CurrVec,scale_line2,near2);
	AddVector(b1->CurrPos,near1,near1);
	AddVector(b2->CurrPos,near2,near2);
	SubVector(near2,near1,BCDVec);
	dist = VectorLength2(BCDVec);
	t = b1->Radius + b2->Radius;
	t *= t;
	if (dist >= t)
	{
		return(0);
	}
	else
		return(t - dist);
}

/*
 * Converts a line start, end, radius into faster internal format
 * The internal format consists of:
 * BasePos	- Center of the line
 * UnitVec	- Unit direction vector
 * Length	- |Line| / 2 + Radius
 * Radius	- Radius of the skin of the balloon
 */
void BalloonInit(Balloon *b,const F32 startp[3],const F32 endp[3],F32 radius)
{
	F32		tp[3];
	F32		length;
	int		i;

	SubVector(endp,startp,tp);
	length = fsqrt(VectorLength2(tp));
	if (length < .000001)
	{
		length = 0.0;
		for(i=0;i<3;i++)
		{
			b->UnitVec[i] = (i==2) ? 1.0 : 0.0;
			b->BasePos[i] = startp[i];
		}
	}
	else
	{
		for(i=0;i<3;i++)
		{
			b->UnitVec[i] = tp[i] / length;
			b->BasePos[i] = startp[i] + tp[i] / 2;
		}
	}
	b->Length		= length / 2 + radius;
	b->Radius		= radius;
	CopyVector(b->BasePos,b->CurrPos);
	CopyVector(b->UnitVec,b->CurrVec);
}

/*
 * Transforms balloon info based on current segment transform
 * Call this before you call BalloonCollide
 */
void BalloonMove(Balloon *b,F32 m[4][3])
{
	MulVecMat(b->BasePos,b->CurrPos,m);
	WorldVector(b->UnitVec,b->CurrVec,m);
}
