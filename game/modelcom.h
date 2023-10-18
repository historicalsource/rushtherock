  
#define CTLSCALE (1.0/0x1000)	/* 1.0 for control inputs */
#define INVCTLSCALE 0x1000	/* 1.0 for control inputs */
#define S4SCALE (1600.0/40.0)	/* 1600 in system four equals 30 feet */
#define INVS4SCALE (40.0/1600.0)
#define MPHSCALE 4
#define FUELSCALE 4
#define VELSCALE 16		/* scale for x,y,and z velocities */
#define TICKTIME .004		/* time/tick on 68000 */
#define INVVELFACTOR (40.0 / (1600.0 * TICKTIME * VELSCALE))

#define GRAVCON (32.2)

/* These are for communication between the simulator and tomcat */

#include "modeldat.h"

