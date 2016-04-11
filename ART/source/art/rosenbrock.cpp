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

// Implementation of Rosenbrock search algorithm

// Set up as a maximisation problem (scores in range 0.0 -> 100.0)
// Algorithm is extended to work in a unit-sized normalised space, irrespective of the design variables. See thesis for mathematical details.

#ifndef ROSENBROCK_CPP
#define ROSENBROCK_CPP

#include "rosenbrock.h"

// As defined in Rosenbrock paper
const double ALPHA=3.0;
const double BETA=0.5;
const double DEFAULT_STEP_SIZE = 0.1;

bool multiAnd(bool *a, int n) {
	//Logical AND of n bools
	bool result=true;
	for (int i=0; i<n; i++) {
		result = result && a[i];
	}
	return result;
}

void rosenbrock(const int n, const int nconsts, double *xBest, const double *bl, const double *bu,
				const int maxEvals, const double eps, 
				bool J(int,double *,double *,const double *, const double*,void *), void *extraparams) {

	double* xNormBest = new double[n];
	double* xNormTest = new double[n];
	double* xTest = new double[n+nconsts];
	double* xStored = new double[n];
	double JBest=0.0, JTest=0.0;
	double* stepSize = new double[n]; // e in paper
	double* sizeOfBounds = new double[n];
	double* t = new double[n]; //defined in Palmer paper
	double div=0.0; //as in Palmer
	DMatrix D = DMatrix(n,n); // direction vectors 
	DMatrix Q = DMatrix(n,n); //Vectors joining initial and final points of each step 
	bool* success = new bool[n]; //Record whether a successful move has been made, in each direction
	bool* failure = new bool[n];
	double* lambda = new double[n]; // sum of moves in each direction
	int nEvals = 1, nStages=1;
	
	//declare for-loop-var as local var to resolve VC compatibility issues...
	int i;
	
	for ( i=0; i<n; i++) {
		stepSize[i] = DEFAULT_STEP_SIZE;
	
		//Normalisation
		//Using xBest|xTest|xStored to be coordinates in unnormalised space, xNormBest|xNormTest as coord in normalised space
		sizeOfBounds[i]=bu[i]-bl[i];
		xNormBest[i] = (xBest[i]-bl[i]) / sizeOfBounds[i];
	
		xStored[i] = xBest[i];
		lambda[i] = 0.0;
		success[i] = false; failure[i]= false; 
		for (int j=0; j<n; j++) D(i,j)=(double)(i==j); //Initialise to identity matrix
	}

	for ( i=n; i<n+nconsts; i++) xTest[i]=xBest[i]; //copying const dimensions accross
	(*J)(n, xBest, &JBest, bl, bu, extraparams); 
	cout << endl;

	while (true) {
		for (int i=0; i<n; i++) { //permute through each dimension
			for (int j=0; j<n; j++) {
				xNormTest[j] = xNormBest[j] + stepSize[i]*D(j,i); 
				//de-Normalisation
				xTest[j] = xNormTest[j]*sizeOfBounds[j] + bl[j];
			}
			if( (*J)(n, xTest, &JTest, bl, bu, extraparams) ) nEvals++; 

			if(JTest >= JBest) { //success
				cout << " +\n";
				success[i]=true;
				lambda[i]+=stepSize[i];
				stepSize[i]*=ALPHA;
				JBest = JTest;
				for (int j=0; j<n; j++) {xBest[j] = xTest[j]; xNormBest[j] = xNormTest[j];}
			} else { //failure
				cout << " -\n";
				failure[i]=true;
				stepSize[i]*=-BETA;
			}		
		}
		cout << endl;

		if (nEvals>maxEvals) break; //ends main while loop
		if (JBest>=100.0)  break; //perfect result found

		if (multiAnd(success, n) && multiAnd(failure, n)) {
			// termination condition - if largest step size < eps, end program
			double maxStepSize = 0.0;
			double absStepSize;
			double totalStepSize=0.0;
			
			//declare for-loop-var as local var to resolve VC compatibility issues...
			int i;

			for ( i=0; i<n; i++) {
				absStepSize = abs(xBest[i]-xStored[i]);
				if (absStepSize>maxStepSize) maxStepSize=absStepSize;
				xStored[i]=xBest[i]; 
				totalStepSize += abs(stepSize[i]); //used later
			} 
			cout << "End of stage " << nStages  << " after " << nEvals << " evals, largest change " << maxStepSize << endl;
			if (maxStepSize<eps) break; //breaks main while loop  
		
			// Change coordinate directions

			//Setup matrix Q from total step lengths
			nStages++;
		
			for ( i=0; i<n; i++) {
			//cout << lambda[i] << ", " << stepSize[i] << endl;
				for (int j=0; j<n; j++) {
					Q(j,i)=0.0;
					for (int k=i; k<n; k++) {
						Q(j,i) += lambda[k]*D(j,k);
					}	
				}
			}

			//Palmer change of direction procedure.
			t[n-1] = lambda[n-1] * lambda[n-1];
			//declare for-loop-var as local var to resolve VC compatibility issues...
			int k;
			for ( k=n-2; k>=0; k--) t[k]=t[k+1] + lambda[k]*lambda[k];
			for ( k=n-1; k>0; k--) {
				div = sqrt(t[k-1]*t[k]);
				if (div!=0.0) {
					for (int i=0; i<n; i++)	D(i,k) = (lambda[k-1] * Q(i,k) - D(i,k-1) * t[k])/div;
				}
			}
			div = sqrt(t[0]);

			for ( i=0; i<n; i++) D(i,0) = Q(i,0)/div;
	
			totalStepSize/=n;
			for ( i=0; i<n; i++) { //resetting arrays before next stage
				stepSize[i] = totalStepSize;
				success[i] = false; 
				failure[i]= false; 
				lambda[i]=0.0;
			}	
			cout << "**********************\n\n";
		}
	}
	cout << "nEvals : " << nEvals << ", nStages : " << nStages << endl;

	delete[] xNormBest;
	delete[] xNormTest;
	delete[] xTest;
	delete[] xStored;
	delete[] stepSize;
	delete[] success;
	delete[] failure;
	delete[] lambda;
	return;
}

#endif
