/*#include <math.h>*/
#include "stdtypes.h"
#include <ieeefp.h>
#define IEEE_BIAS __IEEE_FLT_EXPBIAS
#define BITSTYPE __ieee_float_shape_type

#if 0
extern F32 ldexpf(F32 val, int exp);

/* sincos.c */
extern F32 fsin(F32 a);
extern F32 fcos(F32 a);

/* asincos.c */
extern F32 fasin(F32 a);
extern F32 facos(F32 a);

/* sqrt.c */
extern F32 ffrexp(F32 val,int *exp);
extern F32 fldexp(F32 rval, int exp);
extern F32 fsqrt(F32 xa);
#endif
