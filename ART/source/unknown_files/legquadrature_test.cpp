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

// test program for "LeGquadrature.cpp"
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

#include "LeGquadrature.h"
//--------------------------------------------------------------------------
//
// samples of the integrand function
//
double func1(double x)
{
  return sin(x);
}
//
double func2(double x)
{
  return 1.0 / (1.0 + x*x);
}
//--------------------------------------------------------------------------
//
// main program
//
int main(int argc, char* argv[])
{
  double itgl, itgl_e;
  double error;
  //
  printf("Legendre-Gauss quadrature formula\n\n");
  //
  // example 1
  //
  printf("I1: integral of sin(x) over [0,pi/2]\n");
  //
  int n = 7;
  LeG_quadrature leg(n);  
  itgl_e = 1.0;
  printf("I1(exact)=%22.15e\n", itgl_e);
  itgl = leg.integral(func1, 0.0, 0.5*M_PI);
  error = (itgl - itgl_e) / fabs(itgl_e);
  printf("n = %2d, I1 = %22.15e, relative error = %8.1e\n", n, itgl, error);
  printf("\n");
  //
  // example 2
  //
  printf("I2: integral of 1/(1+x^2) over [-1,+1]\n");
  itgl_e = 0.5 * M_PI;  
  printf("I2(exact)=%22.15e\n", itgl_e);
  for (n = 7; n <= 21; n+=7)
    {
      if (n>7) leg = LeG_quadrature(n);
      itgl = leg.integral(func2, -1.0, 1.0);
      error = (itgl - itgl_e) / fabs(itgl_e);
      printf("n = %2d, I2 = %22.15e, relative error = %8.1e\n", n, itgl, error);
    }
  //
  return 0;
}
