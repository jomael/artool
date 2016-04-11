/**************************************************************************
*                                                                         *
*                   Acoustic Research Tool (ART)                          *
*                                                                         *
*   A Simulation Framework with Modelling Library for Acoustic Systems    *
*                                                                         *
*         Project of the Workgroup 2 of the Technical Committee           *
*      Musical Acoustics of the European Acoustics Association EAA)       *
*                                                                         *
*   http://www.eaa-fenestra.org/technical-committees/ma/workgroups/wg2    *
*                                                                         *
*  Copyright (C) 2011 by the authors and their organisations:             *
*    Alistair Braden            1)                                        *
*    Wilfried Kausel            2)         kausel(at)mdw.ac.at            *
*    Delphine Cadefaux          2)                                        *
*    Vasileios Chatziioannou    2)                                        *
*    Sadjad Siddiq              2)                                        *
*                                                                         *
*    1) School of Physics and Astronomy, Univ. of Edinburgh, GB           *
        (http://www.ph.ed.ac.uk/)                                         *
*    2) Inst. of Music Acoustics, Univ. of Music, Vienna, AT              *
        (http://iwk.mdw.ac.at)                                            *
*                                                                         *
*  This program is free software: you can redistribute it and/or modify   *
*  it under the terms of the GNU General Public License as published by   *
*  the Free Software Foundation, either version 3 of the License, or      *
*  any later version.                                                     *
*                                                                         *
*  This program is distributed in the hope that it will be useful,        *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
*  GNU General Public License for more details.                           *
*                                                                         *
*  You should have received a copy of the GNU General Public License      *
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
*                                                                         *
***************************************************************************/

#ifndef LEGQUADRATURE_CPP
#define LEGQUADRATURE_CPP

//
// the Legendre-Gauss quadrature formula 
// for the integral of a function f(x) over a finite interval [a, b]
//
// copyright : Hidenori Ogata, 8 June 2004, ver.1
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
// In no event shall the author (H. Ogata) be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to) procurement of substitute goods or
// services; loss of use, data, or profits, or business interruption)
// however caused and on any theory of liability, whether in contract,
// strict liability, or tort (including negligence or otherwise) arising
// in any way out of the use of this software, even if advised of the
// possibility of such damage.
//
//
// (usage)
// 1. Make an object, say "leg", of the class "LeG_quadrature".
//    If you want to compute an integral by n-point formula, 
//    write "LeG_quadrature leg(n)" with an integer argument "n".
//
// 2. "leg.integral(func, xmin, xmax)" gives the integral. 
//    The arguments are as follows.
//      func  double (*func)(double):  integrand function(x)
//      xmin  double:  lower bound of the integral
//      xmax  double:  upper bound of the integral
//
#include "LeGquadrature.h"
//
//   Legendre polynomial of order n
//   (input)
//   n: order n of the Legendre polynomial Pn(x)
//   x: the variable x
//
double LeG_quadrature::Pn_Legendre(int n, double x) {
  double p0, p1, p2, ri, pp;
  int r;
  
  if (n==0) pp = 1.0;
  if (n==1) pp = x;
  if (n>=2)
    {
      p0 = 1.0;
      p1 = x;
      for (r=2; r<=n; ++r)
        {
	  ri = 1.0 / (double)r;
	  p2 = (2.0-ri)*x*p1 - (1.0-ri)*p0;
	  p0 = p1;
	  p1 = p2;
        }
      pp = p2;
    }
  return pp;
}
//
//   derivative of the Legendre polynomial of order n: Pn'(x)
//   (input)
//   n: order n of the Legendre polynomial
//   x: variable x
//
double LeG_quadrature::dPn_Legendre(int n, double x) {
  double dpn, pn, pn_1;
  
  if (n==0) dpn = 0.0;
  if (n>=1)
    {
      pn = Pn_Legendre(n, x);
      pn_1 = Pn_Legendre(n-1, x);
      dpn = n * (pn_1 - x * pn) / (1.0 - x*x);
    }
  return dpn;
}
// ------------------------------------------------------------------
//
// class for the (n-point) Legendre-Gauss quadrature
//

//
//   constructor of the class "LeG_quadrature".  
//   When this generator is called, the abscissae (wk) 
//   and the weights (wk) of the quadrature are generated.  
//   (input)
//   n0: the number of function evaluations
//
LeG_quadrature::LeG_quadrature(int n0) {
  n = n0;
  int n1 = n + 1;
  xk = (double *)malloc(n1*sizeof(double));
  wk = (double *)malloc(n1*sizeof(double));
  get_abscissa_weight();
}


//
// compute the abscissae and the weights 
// of the Legendre-Gauss quadrature
//
void LeG_quadrature::get_abscissa_weight() {
  int jmax=50;
  double xk0, xk1, pn, dpn;
  double x, ww, pn_1;
  double err_tol = 1.0e-15, err_func;
  int l_conv;
  //
  // compute the abscissae by Newton's method
  //
  
  //declare for-loop-var as local var to resolve VC compatibility issues...
  int k;
  
  for (k=1; k<=n; ++k)
    {
      l_conv = 0;
      //
      //  initial values for Newton's method
      //
      xk0 = cos(M_PI*(k-0.25)/(n+0.5));
      for (int j=1; j<=jmax; ++j)
        {
	  pn = Pn_Legendre(n, xk0);
	  dpn = dPn_Legendre(n, xk0);
	  xk1 = xk0 - pn / dpn;
	  pn = Pn_Legendre(n, xk1);
	  dpn = dPn_Legendre(n, xk1);
	  err_func = err_tol * fabs(dpn);
	  if (fabs(pn) < err_func)
            {
	      l_conv = 1;
	      break;
            }
	  xk0 = xk1;
        }
      xk[k] = xk1;
      if (l_conv == 0)
        {
	  printf("k=%3d: not convergent\n", k);
        }
    }
  //
  // compute the weights
  //
  for (k=1; k<=n; ++k)
    {
      x = xk[k];
      pn_1 = Pn_Legendre(n-1, x);
      ww = 2 * (1.0 - x*x) / (n*n*pn_1*pn_1);
      wk[k] = ww;
    }
}
//
//   compute the integral over a finite interval [xmin, xmax]
//   by the (n-point) Legendre-Gauss quadrature formula
//   (input)
//   func: integrand function, which is double-precision valued
//          with one variable in double precision
//   xmin: lower bound of integration
//   xmax: upper bound of integration
//                 of the Legendre-Gauss quadrature formula
//   (output)
//   computed value of the integral
//
double LeG_quadrature::integral(double (*func)(double), double xmin, double xmax) {
  double c1, c2, s;
  c1 = 0.5 * (xmax + xmin);
  c2 = 0.5 * (xmax - xmin);
  s = 0.0;
  for (int k=1; k<=n; ++k)
    s += c2 * wk[k] * func(c1 + c2 * xk[k]);
  return s;
}

double LeG_quadrature::Bintegral(double xmin, double xmax, int m1, int n1, int mu, int nu) {
// Integrand of definite integral from eqn C1, Felix & Pagneux, Wave Motion 36 (2002) p.157-168	
	double c1, c2, s, r;
	c1 = 0.5 * (xmax + xmin);
	c2 = 0.5 * (xmax - xmin);
	s = 0.0;
	for (int k=1; k<=n; ++k) {
		r=c1 + c2 * xk[k];
		s += (c2 * wk[k])  *  r*r*jn(m1,BESSELDERIVZERO[m1][n1]*r/xmax)*jn(mu,BESSELDERIVZERO[mu][nu]*r/xmax);
	}
  
	//cout << endl << BESSELDERIVZERO[m1][n1] << ", " << BESSELDERIVZERO[mu][nu] << endl;
		
	return s;
}

double LeG_quadrature::Cintegral_1(double xmin, double xmax, int m1, int n1, int mu, int nu) {
// Integrand of first definite integral from eqn C2, Felix & Pagneux, Wave Motion 36 (2002) p.157-168	
	double c1, c2, s, r;
	c1 = 0.5 * (xmax + xmin);
	c2 = 0.5 * (xmax - xmin);
	s = 0.0;
	for (int k=1; k<=n; ++k) {
		r=c1 + c2 * xk[k];
		s += (c2 * wk[k])  *  r*jn(m1+1,BESSELDERIVZERO[m1][n1]*r/xmax)*jn(mu,BESSELDERIVZERO[mu][nu]*r/xmax);
	}
  	
	return s;
}

double LeG_quadrature::Cintegral_2(double xmin, double xmax, int m1, int n1, int mu, int nu) {
// Integrand of second definite integral from eqn C2, Felix & Pagneux, Wave Motion 36 (2002) p.157-168	
	double c1, c2, s, r;
	c1 = 0.5 * (xmax + xmin);
	c2 = 0.5 * (xmax - xmin);
	s = 0.0;
	for (int k=1; k<=n; ++k) {
		r=c1 + c2 * xk[k];
		s += (c2 * wk[k])  *  jn(m1, BESSELDERIVZERO[m1][n1]*r/xmax)*jn(mu,BESSELDERIVZERO[mu][nu]*r/xmax);
	}
  	
	return s;
}


#endif
