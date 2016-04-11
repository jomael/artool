/* cdiv.f -- translated by f2c (version 20060506).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#include "f2c.h"

/* Subroutine */ int cdiv_(doublereal *ar, doublereal *ai, doublereal *br, 
	doublereal *bi, doublereal *cr, doublereal *ci)
{
    /* System generated locals */
    doublereal d__1, d__2;

    /* Local variables */
    static doublereal s, ais, bis, ars, brs;


/*     complex division, (cr,ci) = (ar,ai)/(br,bi) */

    s = abs(*br) + abs(*bi);
    ars = *ar / s;
    ais = *ai / s;
    brs = *br / s;
    bis = *bi / s;
/* Computing 2nd power */
    d__1 = brs;
/* Computing 2nd power */
    d__2 = bis;
    s = d__1 * d__1 + d__2 * d__2;
    *cr = (ars * brs + ais * bis) / s;
    *ci = (ais * brs - ars * bis) / s;
    return 0;
} /* cdiv_ */

