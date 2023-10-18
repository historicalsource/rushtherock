
/* This file contains functions which simulate the drivetrain of
the vehicle*/

#include "drivsym.h"

#include "modeldat.h"

#include "road.h"
#include "Pro/drivetra.pro"


/* These are for the automatic transmission */

/* This is the loss factor for automatic transmission not in top gear */
#define AUTOLOSS .925


#define NEUTRALGEAR 0
#define REVERSEGEAR -1

/*This array contains the transmission gear ratios for each gear*/

void drivetrain(MODELDAT *m)
{
	float rearload;
	
	if(m->autotrans)
		autoshift(m);
	engine(m);
	transmission(m);

	m->dwangvel = (m->tires[2].angvel+m->tires[3].angvel)*.5;
	
	whatslips(m);
	
	rearload = (m->TIREFORCE[2][ZCOMP] + m->TIREFORCE[3][ZCOMP]);

	/* Magic rear differential with load sensitive torque split */
	if((!m->magicdif) || (rearload > -500)){
		m->torque[2] += m->dwtorque * .5;
		m->torque[3] += m->dwtorque * .5;
	}
	else{
		if(m->TIREFORCE[2][ZCOMP] >= 0){
			m->torque[2] = 0;
			m->torque[3] = m->dwtorque;
		}
		else if(m->TIREFORCE[3][ZCOMP] >= 0){
			m->torque[2] = m->dwtorque;
			m->torque[3] = 0;
		}
		else{
			m->torque[2] = (m->dwtorque * m->TIREFORCE[2][ZCOMP])/rearload;
			m->torque[3] = (m->dwtorque * m->TIREFORCE[3][ZCOMP])/rearload;
		}
	}
		
	m->tires[2].invmi = m->efdwinvmi*2;	/* This isn't correct!!! */
	m->tires[3].invmi = m->efdwinvmi*2;
}


void whatslips(MODELDAT *m)
{
	F32 roadangvel,roadmaxt,slip,I1,I2;
	F32 totratsq,centrifvel;
    	F32 curclmaxt;

	totratsq=m->totalratio*m->totalratio;

	if(m->gear == NEUTRALGEAR){
		m->clutchtorque=0;
		m->dwtorque=0;
		m->clutchangvel=m->engangvel;
		m->engangvel += m->engtorque*m->enginvmi*m->dt;
		m->efdwinvmi = m->dwinvmi;
		return;
	}

	m->clutchangvel = m->dwangvel*m->totalratio;

	if(m->autotrans){
		if(m->engangvel < (m->clutchangvel - (100*rpmtordps))){
			curclmaxt = m->clutchmaxt;
		}
		else{
			centrifvel = m->engangvel - (m->clutchangvel * .25);
			if(centrifvel < ((1500 * rpmtordps)))
				curclmaxt = 0;
			else{
				if(centrifvel > (3000 * rpmtordps))
					curclmaxt = m->clutchmaxt;
				else
					curclmaxt=(m->clutchmaxt*
					    (1/(1500 * rpmtordps))) *
					    (centrifvel - (1500 * rpmtordps));
			}
		}
	}
	else{
		if(m->clutch < 0)
			curclmaxt=m->clutchmaxt;
		else{
			if(m->clutch > .8){      /* friction point at .8 */
				curclmaxt = 0;
			}
			else{
				curclmaxt = (1.0/.8)*(.8-m->clutch)*m->clutchmaxt;
			}
		}
	}
	
	if(curclmaxt < 0) curclmaxt = 0;

	if(m->engangvel > m->clutchangvel){
		m->clutchtorque=curclmaxt;
		m->engangvel += (m->engtorque - m->clutchtorque)*m->enginvmi*m->dt;
		if(m->engangvel < m->clutchangvel){
			if(m->engtorque < -curclmaxt){
				m->clutchtorque = -curclmaxt;
/* We just did this, why now?*/	m->engangvel += (m->engtorque - m->clutchtorque) *
					m->enginvmi*m->dt;
				m->efdwinvmi = m->dwinvmi;
			}
			else{
				m->engangvel = m->clutchangvel;
					m->clutchtorque=m->engtorque;
				m->efdwinvmi = 1/(1/m->dwinvmi + 
					totratsq/m->enginvmi);
			}
		}
		else{
			m->efdwinvmi = 1/(1/m->dwinvmi + 
/* Clutch slipping, so why engine? */	totratsq/m->enginvmi);
		}
	}
	else{
		m->clutchtorque= -curclmaxt;
		m->engangvel += (m->engtorque - m->clutchtorque)*m->enginvmi*m->dt;
		if(m->engangvel > m->clutchangvel){
			if(m->engtorque > curclmaxt){
				m->clutchtorque = curclmaxt;
				m->engangvel += (m->engtorque - m->clutchtorque) *
					m->enginvmi*m->dt;
				m->efdwinvmi = m->dwinvmi;
			}
			else{
				m->engangvel = m->clutchangvel;
				m->clutchtorque=m->engtorque;
				m->efdwinvmi = 1/(1/m->dwinvmi + 
					totratsq/m->enginvmi);
				
			}
		}
		else{
			m->efdwinvmi = 1/(1/m->dwinvmi + 
					totratsq/m->enginvmi);
		}
	}
	m->dwtorque = m->clutchtorque * m->totalratio;
}

void autoshift(MODELDAT *m)
{
	F32 modupshiftangvel,moddownshiftangvel,fact;
	
	/* Modify shift points for throttle position */
	fact = (3.0 + m->throttle)*.25;
	modupshiftangvel = m->upshiftangvel * fact;
	moddownshiftangvel = m->downshiftangvel * fact;
	
	if ((m->commandgear == NEUTRALGEAR) || 
		(m->commandgear == REVERSEGEAR))
		{
		m->gear = m->commandgear;
		}
	else
		{
		if((m->gear == NEUTRALGEAR) ||
           (m->gear == REVERSEGEAR))	/* Shift out of neutral or reverse */
			find_best_gear(m,modupshiftangvel,moddownshiftangvel);
		if(m->engangvel > modupshiftangvel)
			upshift(m);
		if(m->engangvel < moddownshiftangvel)
			downshift(m);
		}
}

void transmission(MODELDAT *m)
{
	m->transratio=m->transarray[m->gear+1];
	m->totalratio=m->transratio*m->dwratio;
}

void find_best_gear(MODELDAT *m,F32 usang,F32 dsang)
{
	F32 angvel,tstratio;
	for(m->gear = 1;m->gear < m->topgear;++m->gear){
		tstratio=m->dwratio*m->transarray[m->gear+1];
		angvel = m->dwangvel * tstratio;
		if(angvel < usang)
			break;
	}
}

void upshift(MODELDAT *m)
{
	if(m->gear < m->topgear && (m->thetime > m->shifttime)){
		m->shifttime = m->thetime + 1;
		m->gear++;
	}
}

void downshift(MODELDAT *m)
{
	if(m->gear > MINGEAR && (m->thetime > m->shifttime)){
		m->shifttime = m->thetime + 1;
		m->gear--;
	}
}

void engine(MODELDAT *m)
{
	/*dont do dirt slowdown if under 40 fps (~28 mph)*/
	if((m->roadcode[2] == DIRT) && (m->roadcode[3]== DIRT) && (m->magvel > 40.0))
		m->engtorque= m->dirttorquescale * enginetorque(m,(short)(m->engangvel*rdpstorpm),
		(short)(m->throttle*128),m->ignition,m->startermotor,m->dirttorquecurve);
	else{
		if(m->gear ==1){
			m->engtorque= m->fgtorquescale * enginetorque(m,(short)(m->engangvel*rdpstorpm),(short)(m->throttle*128),m->ignition,m->startermotor,m->torquecurve);
		}
		else{
			if(m->gear == 2){
				m->engtorque= m->sgtorquescale * enginetorque(m,(short)(m->engangvel*rdpstorpm),(short)(m->throttle*128),m->ignition,m->startermotor,m->torquecurve);
			}
			else{
				m->engtorque= m->torquescale * enginetorque(m,(short)(m->engangvel*rdpstorpm),(short)(m->throttle*128),m->ignition,m->startermotor,m->torquecurve);
/*			m->engtorque *= m->catchup;*/ /*catchup done with time fudge now*/
			}
		}
	}
	/* Reduce transmission torque if in auto and not top gear (car
	has locking torque converter which locks in top gear */
	if(m->autotrans && (m->gear != m->topgear))
		m->engtorque *= AUTOLOSS;
}

short enginetorque(MODELDAT *m, short rpm, short throttle, short ignition, 
					short start, const short *torquecurve)
{
	short rindex,tindex,rrem,trem;
	short left,right;
	short *zzp,*fnp;
    	short *low_ptr,*hi_ptr;

	zzp = (short *)(torquecurve);
	fnp = (short *)(torquecurve)+11;

	if(rpm < 2000){
		m->amperes = ((10.0/2000) * rpm) - 10;
		m->oilpressure = (40.0/2000) * rpm;
	}
	else{
		m->oilpressure = 40;
		m->amperes = 10;
	}

	if(start){
		m->amperes = -50;
	}
	
	if(rpm < 500){
		if(rpm >= 0){
			if(start)
				return(*zzp);
			else
				return((short)((-*zzp * rpm)/500));
		}
		m->oilpressure = 0;
		if(rpm >= -1000){
			if(start)
				return(*zzp+(short)((*fnp*rpm)/1000));
			else
				return((short)((*fnp*rpm)/1000));
		}
		return(70);
	}
	
	if(m->thetime < 60){
		m->enginetemp = 70 + ((130.0/60) * m->thetime);
	}
	else{
		m->enginetemp = 200;
	}
	
	if(!ignition){
		return(-*zzp);
	}

	if((rpm < 1000) && start)
		return(*zzp);

	rindex=rpm/(short)m->rpmperent;
	rrem=rpm % (short)m->rpmperent;
	if(rindex<0){
		rindex=0;
		rrem=0;
	}
	if(rindex>=11){
		rindex=10;
		rrem=m->rpmperent-1;
	}
	tindex=throttle/(short)14;
	trem=throttle % (short)14;
	if(tindex>=9){
		tindex=8;
		trem=13;
	}
	low_ptr = (short *)torquecurve + (tindex*12) + rindex;
	hi_ptr = (short *)torquecurve + (tindex*12) + rindex + 1;
	left=interp((*low_ptr),(*hi_ptr), rrem, m->rpmperent-1);
	low_ptr = (short *)torquecurve + ((tindex+1)*12) + rindex;
	hi_ptr = (short *)torquecurve + ((tindex+1)*12) + rindex + 1;
	right=interp((*low_ptr),(*hi_ptr),rrem,m->rpmperent-1);
	right=interp(left,right,trem,14);
	return(right);
}

short interp(int a, int b, int rem, int total)
{
	return(a + (((b-a)*rem)/total));

}


