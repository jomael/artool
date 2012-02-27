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

// Useful where code will certainly require 2x2 complex<double> matrix, as is faster than a generalised templated matrix.

#ifndef SIMPLEMATRIX_CPP
#define SIMPLEMATRIX_CPP

#include "SimpleMatrix.h"
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <complex>
#include <string.h>
using namespace std;

typedef std::complex<double> dcomp;

SimpleMatrix::SimpleMatrix () {
	array[0][0]=dcomp(0,0);
	array[0][1]=dcomp(0,0);
	array[1][0]=dcomp(0,0);
	array[1][1]=dcomp(0,0);
}

SimpleMatrix::SimpleMatrix (const dcomp a, const dcomp b, const dcomp c, const dcomp d) {
	array[0][0]=a;
	array[0][1]=b;
	array[1][0]=c;
	array[1][1]=d;
}

SimpleMatrix SimpleMatrix::operator* (const SimpleMatrix& a) {
	//post-multiplies matrix by a
	
	return (SimpleMatrix(a.getArray(0,0)*getArray(0,0) + getArray(0,1)*a.getArray(1,0),
		getArray(0,0)*a.getArray(0,1)+getArray(0,1)*a.getArray(1,1),
		getArray(1,0)*a.getArray(0,0)+getArray(1,1)*a.getArray(1,0),
		getArray(1,0)*a.getArray(0,1)+getArray(1,1)*a.getArray(1,1)));
}	

//SimpleMatrix::~SimpleMatrix() {}

SimpleMatrix& SimpleMatrix::operator = (const SimpleMatrix &a) {
  if (this == &a) return *this;
	array[0][0]=a.getArray(0,0);
	array[0][1]=a.getArray(0,1);
	array[1][0]=a.getArray(1,0);
	array[1][1]=a.getArray(1,1);
	return *this;
}

void SimpleMatrix::printAbs() const {
	//Prints the absolute value of each matrix element to the screen.
	
	cout << "[[" << std::abs(array[0][0]) << ", " << std::abs(getArray(0,1)) << "]\n[" << std::abs(getArray(1,0)) << ", " << std::abs(getArray(1,1)) << "]]\n";
}

void SimpleMatrix::printMaple() const {
	//Prints matrix to screen in a format readable by Maple
	
	cout << "matrix(2, 2, [" << array[0][0].real() << " + I* " << array[0][0].imag();
	cout << ", " << array[1][0].real() << " + I* " << array[1][0].imag();
	cout << ", " << array[0][1].real() << " + I* " << array[0][1].imag();
	cout << ", " << array[1][1].real() << " + I* " << array[1][1].imag() << "]);";
}

void SimpleMatrix::setValues(const dcomp a, const dcomp b, const dcomp c, const dcomp d) {
	array[0][0]=a;
	array[0][1]=b;
	array[1][0]=c;
	array[1][1]=d;
}

#endif
