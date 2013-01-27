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
*  Copyright (C) 2013 by the authors and their organisations:             *
*    Alistair Braden            1)                                        *
*    Wilfried Kausel            2)         kausel(at)mdw.ac.at            *
*    Delphine Cadefaux          2)                                        *
*    Vasileios Chatziioannou    2)                                        *
*    Sadjad Siddiq              2)                                        *
*    Clemens Geyer              2)                                        *
*                                                                         *
*    1) School of Physics and Astronomy, Univ. of Edinburgh, GB           *
*       (http://www.ph.ed.ac.uk/)                                         *
*    2) Inst. of Music Acoustics, Univ. of Music, Vienna, AT              *
*       (http://iwk.mdw.ac.at)                                            *
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

// this file:
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

#ifndef LEGQUADRATURE_H
#define LEGQUADRATURE_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

//Zeroes of Jn' - i.e. first derivative of Bessel fn of first kind, integer order n.
//This is NOT the best way to represent this, as many of these values will never be used. Rewrite.
//Have included -1.0 for some of the higher values (not in book "Handbook of Mathematical Functions") as they are useless.

const double BESSELDERIVZERO[13][8] = {{0.0, 3.8317059702, 7.0155866698, 10.1734681351, 13.3236919363, 16.4706300509, 19.6158585105, 22.7600843606}, {1.84118, 5.33144, 8.53632, 11.70600, 14.86359, 18.01553, 21.16437, 24.31133},
 {3.05424, 6.70613, 9.96947, 13.17037, 16.34752, 19.51291, 22.67158, 25.82604}, {4.20119, 8.01524, 11.34592, 14.58585, 17.78875, 20.97248, 24.14490, 27.31006}, {5.31775, 9.28240, 12.68191, 15.96411, 19.19603, 22.40103, 25.58976, 28.76784}, {6.41562, 10.51986, 13.98719, 17.31284, 20.57551, 23.80358, 27.01031, 30.20285},
 {7.50127, 11.73494, 15.26818, 18.63744, 21.93172, 25.18393, 28.40978,  31.61788}, {8.57784, 12.93239, 16.52937, 19.94185, 23.26805, 26.54503, 29.79075, 33.01518}, {9.64742, 14.11552, 17.77401, 21.22906, 24.58720, -1.0, -1.0, -1.0}, {10.71443, 15.28674, 19.00459, 22.50140, 25.89128, -1.0, -1.0, -1.0}, {11.77088, 16.44785, 20.22303, 23.76072, 27.18202, -1.0, -1.0, -1.0}, {12.82649, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0}, {13.87884, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0}  };

#ifndef M_PI
const double M_PI = 3.141592653589;
#endif /* M_PI */

class LeG_quadrature {
	int n;      // number of function evaluation
	double *xk; // abscissae of the quadrature
	double *wk; // weights of the quadrature
public:
	LeG_quadrature(){ n = 0; }
	LeG_quadrature (int);
	double integral (double (*func)(double), double, double);
	double Bintegral (double xmin, double xmax, int m1, int n1, int mu, int nu);
	double Cintegral_1 (double xmin, double xmax, int m1, int n1, int mu, int nu);
	double Cintegral_2 (double xmin, double xmax, int m1, int n1, int mu, int nu);

private:
	void get_abscissa_weight();
	double Pn_Legendre(int n, double x);
	double dPn_Legendre(int n, double x);
};


#endif
