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

// All this is defined at global scope; this is essentially an extension of main.cpp (separated for clarity)

// This is a front-end for the Rosenbrock optimiser in rosenbrock.h rosenbrock.cpp.
// It can load in .rb parameter files, which define an optimisation problem. It does the necesary book-keeping,
// and runs the optimiser, with given objective function. The two structs are used to simplify data-passing, 
// and also include tuning-parameters for the objective fn.

#ifndef RB_CPP
#define RB_CPP

#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#if defined(_MSC_VER) && (_MSC_VER <= 1200)
	#include "memory.h"
#else
	#include <memory>
#endif
#include <iomanip>
#include <string>

using namespace std;

#include "WindInstrument.h"
#include "rosenbrock.h"
using namespace ART;
#ifndef OBJ_PARAMS
#define OBJ_PARAMS

	struct objectiveParameters {
		// encapsulates parameters needed within scope of objective function (and must therefore be passed through Rosenbrock function).

		double nu_phi, nu_z, nu, mu_phi, mu_z, maxfreq;
		int nPeaks, nPoints, nModes;
		double *peakFreq, *peakHeight;
		bool scoreIC, scoreFreq, scoreHeight;
 	};
#endif

struct runRBParameters { //no need to make this a class, struct has enough encapsulation
	// encapsulates parameters needed within scope of runRB function
	
	double *x,*bl,*bu,eps;
	objectiveParameters objParams;
	string targetfilename;
	WindInstrument* instrument;
	bool hasTarget;
	int nparams, nconsts, nElements, maxIter;
};

bool runRB(char filename[]);
bool objective(int nparam,double *x,double *fj, const double *bl, const double *bu, void *extraparams);
bool loadRB (char filename[], runRBParameters* output);
bool readLine (ifstream *file, string* tempValues, int* lineCounter, int* wordCounter, int* length, bool* nearEnd);
void exploreSpace(const int n, const int xVar, const int yVar, double *x, const double *bl, const double *bu,
				const int nEvals, bool J(int,double *,double *,const double *, const double*,void *), void *extraparams, char filename[]);

bool runRB(char filename[]) {
	WindInstrument ins = WindInstrument ();

	runRBParameters input;
	input.instrument = &ins;
	if (!(loadRB(filename, &input))) return false;
	
	if(input.hasTarget) 
		if(!(ins.loadTargetImpedanceCurve(input.targetfilename.c_str()))) {
			cout << "\nERROR : ic file load failed. Exiting\n";
			return false;
		}
	ins.setVerboseMode(false);
	ins.userData((void*)&(input.objParams));

//	exploreSpace (input.nparams, 0, 5, input.x, input.bl, input.bu, input.maxIter, objective, (void*)&ins, "explore.m"); return true;
// This line (and the function it calls) was used for a single experiment for a thesis result, is not expected to be used again!

	cout << endl << "Running Rosenbrock iterator\n";
	cout << "(Scores of each instrument: ";
	if (input.objParams.scoreIC) cout << "whole-curve ";
	if (input.objParams.scoreFreq) cout << "Peak freq. ";
	if (input.objParams.scoreHeight) cout << "Peak Height ";	
	cout << "= TOTAL)\n";

	//x is the set of design variables, bl and bu are the lower and upper bounds.
	
	rosenbrock(input.nparams, input.nconsts, input.x, input.bl, input.bu, input.maxIter, input.eps, objective, (void *)&ins); //This calls the Rosenbrock iterator

	//x is now the optimised solution. Preparing the saved output:

	ins.rosen(input.x);	// converts the design vbls into an instrument
	ins.setVerboseMode(true);
	ins.printDescription();
	ins.impedanceCurve.clear();
	ins.impedanceCurve = ImpedanceCurve();
	const int minFreq = ((input.objParams.nPoints==498)?8:10); // Special case for reconstructing from BIAS data (which starts at 8Hz, not 10)
	ins.roughPlot(input.objParams.nModes, minFreq, input.objParams.maxfreq, input.objParams.nPoints,1);
	ins.impedanceCurve.save("result_C.ic");
	ins.refinePeaks(0.00001);
//	ins.impedanceCurve.findPeaks(false);
	ins.impedanceCurve.printPeaks();
	ins.save("result.ins");
	ins.savePointwise("result.int");
	ins.saveImpedancePlot("result_C");
	ins.saveInstrumentPlot("result_H");
	ins.impedanceCurve.harmonicityPlot(-100.0, 100.0, 100, 2, 16, "result_harm.m");
	
	free(input.x);
	free(input.bl);
	free(input.bu);
	delete[] input.objParams.peakFreq;
	delete[] input.objParams.peakHeight;
	return true;
}

bool objective(int nparam,double *x,double *fj, const double *bl, const double *bu, void *extraparams) {
	//Calculates the fitness of given instrument - returns false if any design variable is out-of-bounds or otherwise unsuitable.

	WindInstrument* genome = (WindInstrument*)extraparams;
	objectiveParameters* params = (objectiveParameters *)((*genome).userData());
	float const maxScore=10.0*(params->scoreIC + params->scoreFreq + params->scoreHeight);
	
	for (int i=0; i<nparam; i++) { //Checking for out-of-bounds
		if (x[i]>bu[i]) {
			*fj = 0.0;
			cout << "x[" << i << "] is >OB   =  " << 0.0 << "%";
			return false;
		}
		
		if (x[i]<bl[i]) {
			*fj = 0.0;
			cout << "x[" << i << "] is <OB   =  " << 0.0 << "%";
			return false;
		}
	}
	
	float score = 0.0;
	(*genome).rosen(x);
	(*genome).impedanceCurve.clear();
	(*genome).impedanceCurve = ImpedanceCurve();
	
	const int minFreq = ((params->nPoints==498)?8:10); // Special case for reconstructing from BIAS data

	if (params->scoreIC && (*genome).getHasTarget()) {
		(*genome).roughPlot(params->nModes, minFreq, params->maxfreq, params->nPoints,1);
		score = (*genome).impedanceCurve.scoreCurve ( (*genome).getTarget(), params->nu, params->mu_z );		
	} else {(*genome).roughPlot(params->nModes, minFreq, params->maxfreq, params->nPoints,1);}

	if (params->scoreFreq || params->scoreHeight)	{
		(*genome).refinePeaks(0.00001);
		score += (*genome).impedanceCurve.scorePeaks (params, params->scoreFreq, params->scoreHeight);
		//(*genome).impedanceCurve.printPeaks();
	}			

	*fj = score/maxScore*100;
	cout << " =  " <<  *fj << "%";
	return true;
}

bool readLine (ifstream *file, string* tempValues, int* lineCounter, int* wordCounter, int* length, bool* nearEnd) {
	//reads a line from given .rb file. Returns false if line is a comment, true otherwise
	string line;
	getline(*file, line, '\n');
	(*lineCounter)++;
	(*length)=(int)line.size();

	if ((*length)>0) if (line[(*length)-1]=='\r') line[(*length)-1]='\0'; 	//Correcting for Windows \r\n endlines.
	if ((*file).eof()) (*nearEnd)=true;		 //checks to see if this is the last line in the file
	if (line[0]!=';') { 		//checks for comments in the file		
		*wordCounter=0;
		for(int i=0, itemp=0; i<=(*length); i++) { //get command
			if((line[i]==' ' || line[i]==',' || line[i]=='\n' || line[i]=='\0' || line[i]=='\r' || ((*nearEnd) && i==(*length))) && i!=itemp) {
				tempValues[*wordCounter]=line.substr(itemp, (i-itemp));
				(*wordCounter)++; 	//counts number of distinct values read from this line
				itemp=i+1;
			}
		}
	} else {tempValues[0]="Comment"; return false;}
	return true;
}

bool loadRB (char filename[], runRBParameters* output) {
	// Loads given .rb file, read all its parameters and dumps them into output
	// Rewritten 05-10-20 to use strings, is more elegant and robust
	// Makes a series of checks on the suitability of the data provided by the user
	// Called only by RunRB, returns true iff no errors were detected in the file.
	// See the spec of .rb files for more information.

	// Default values (i.e. remain unless changed by user)
	(*output).nconsts=0;
	output->hasTarget=false;
	output->objParams.nu_phi = 20.0;
	output->objParams.nu_z = 0.3;
	output->objParams.nu = 1000.0;
	output->objParams.mu_phi = 20.0;
	output->objParams.mu_z = 10.0;
	output->objParams.maxfreq = 2000.0;
	output->objParams.scoreIC = false;
	output->objParams.scoreFreq = false;
	output->objParams.scoreHeight = false;

	// Error-catching values (i.e. throw error unless changed by user)
	output->eps = -1.0;
	output->maxIter = -1;
	output->objParams.nPeaks = -1;
	output->objParams.nPoints = -1;
	int scoring = -1;
	output->objParams.nModes = -1;
	output->objParams.peakFreq = NULL;
	output->objParams.peakHeight = NULL;

	ifstream file(filename);
	cout << "Loading Rosenbrock setup file: '" << filename << "' ";	//Gets params, target data etc. from a .rb file
	if (!file.is_open()) {
		cout << "ERROR: File '" << filename << "' failed to open\n";
		return false;	
	}

	const int arraySize=50;
	string line, command;
	string tempValues[arraySize];
	int lineCounter=0, wordCounter, length;
	bool nearEnd=false, noErrors=true;
	int i;
	

	while (!file.eof()) { 
		readLine(&file, tempValues, &lineCounter, &wordCounter, &length, &nearEnd);
		if(wordCounter>0) command = tempValues[0]; 
		else command = "Empty Line";
		bool throwLineError=false;

		// This section was *less* readable as a switch statement...
		// Compares the loaded command against the valid options and acts accordingly

		if (command=="target") {
			if (wordCounter==2) { output->targetfilename=tempValues[1]; output->hasTarget=true; }
			else throwLineError=true;
		}
		else if (command=="eps") {
			if (wordCounter==2) output->eps=atof(tempValues[1].c_str()); 
			else throwLineError=true;
			if (output->eps<=0.0) {cout << "\nERROR on line " << lineCounter << " of '" << filename << "' : eps must be positive double"; noErrors=false;}
		}
		else if (command=="maxiter") {
			if (wordCounter==2) output->maxIter=atoi(tempValues[1].c_str()); 
			else throwLineError=true;
			if (output->maxIter<=0) {cout << "\nERROR on line " << lineCounter << " of '" << filename << "' : maxIter must be positive integer"; noErrors=false;}
		}
		else if (command=="nu_phi" || command=="cfreq") {
			if (wordCounter==2) output->objParams.nu_phi=atof(tempValues[1].c_str()); 
			else throwLineError=true;
			if (output->objParams.nu_phi<=0.0) {cout << "\nERROR on line " << lineCounter << " of '" << filename << "' : nu_phi must be positive double"; noErrors=false;}
		}
		else if (command=="nu_z" || command=="cheight") {
			if (wordCounter==2) output->objParams.nu_z=atof(tempValues[1].c_str()); 
			else throwLineError=true;
			if (output->objParams.nu_z<=0.0) {cout << "\nERROR on line " << lineCounter << " of '" << filename << "' : nu_z must be positive double"; noErrors=false;}
		}
		else if (command=="nu" || command=="cic") {
			if (wordCounter==2) output->objParams.nu=atof(tempValues[1].c_str()); 
			else throwLineError=true;
			if (output->objParams.nu<=0.0) {cout << "\nERROR on line " << lineCounter << " of '" << filename << "' : nu must be positive double"; noErrors=false;}
		}
		else if (command=="mu_phi" || command=="betafreq") {
			if (wordCounter==2) output->objParams.mu_phi=atof(tempValues[1].c_str()); 
			else throwLineError=true;
			if (output->objParams.mu_phi<=0.0) {cout << "\nERROR on line " << lineCounter << " of '" << filename << "' : mu_phi must be positive double"; noErrors=false;}
		}
		else if (command=="mu_z" || command=="betaheight") {
			if (wordCounter==2) output->objParams.mu_z=atof(tempValues[1].c_str()); 
			else throwLineError=true;
			if (output->objParams.mu_z<=0.0) {cout << "\nERROR on line " << lineCounter << " of '" << filename << "' : mu_z must be positive double"; noErrors=false;}
		}
		else if (command=="maxfreq") {
			if (wordCounter==2) output->objParams.maxfreq=atof(tempValues[1].c_str()); 
			else throwLineError=true;
			if (output->objParams.maxfreq<=0.0) {cout << "\nERROR on line " << lineCounter << " of '" << filename << "' : maxfreq must be positive double"; noErrors=false;}
		}
		else if (command=="npeaks") {
			if (wordCounter==2) output->objParams.nPeaks=atoi(tempValues[1].c_str()); 
			else throwLineError=true;
			if (output->objParams.nPeaks<=0) {cout << "\nERROR on line " << lineCounter << " of '" << filename << "' : npeaks must be positive integer"; noErrors=false;}
		}
		else if (command=="npoints") {
			if (wordCounter==2) output->objParams.nPoints=atoi(tempValues[1].c_str()); 
			else throwLineError=true;
			if (output->objParams.nPoints<=0) {cout << "\nERROR on line " << lineCounter << " of '" << filename << "' : npoints must be positive integer"; noErrors=false;}
		}
		else if (command=="scoring") {
			if (wordCounter==2) scoring=atoi(tempValues[1].c_str()); 
			else throwLineError=true;
			if (scoring<1 || scoring>7) {cout << "\nERROR on line " << lineCounter << " of '" << filename << "' : scoring must be integer in range [1,7]"; noErrors=false;}
			output->objParams.scoreIC = scoring&1; 
			output->objParams.scoreFreq = (scoring&2)/2==1; 
			output->objParams.scoreHeight = (scoring&4)/4==1;
		}
		else if (command=="nmodes") {
			if (wordCounter==2) output->objParams.nModes=atoi(tempValues[1].c_str()); 
			else throwLineError=true;
			if (output->objParams.nModes<0) {cout << "\nERROR on line " << lineCounter << " of '" << filename << "' : nModes must be positive integer"; noErrors=false;}
		}
		else if (command=="peakfreq") {
			if (output->objParams.nPeaks!=-1) { //checks nPeaks is already defined
				if (wordCounter>=1+output->objParams.nPeaks) {
					output->objParams.peakFreq = new double[output->objParams.nPeaks];
					for (int i=0; i<output->objParams.nPeaks; i++) output->objParams.peakFreq[i]=atof(tempValues[1+i].c_str()); 
				}
				else throwLineError=true;
			} else {cout << "\nERROR on line " << lineCounter << " of '" << filename << "' : peakFreq defined before nPeaks defined"; noErrors=false;}
		}
		else if (command=="peakheight") {
			if (output->objParams.nPeaks!=-1) { //checks nPeaks is already defined
				if (wordCounter>=1+output->objParams.nPeaks) {
					output->objParams.peakHeight = new double[output->objParams.nPeaks];
					for (int i=0; i<output->objParams.nPeaks; i++) output->objParams.peakHeight[i]=atof(tempValues[1+i].c_str()); 
				}
				else throwLineError=true;
			} else {cout << "\nERROR on line " << lineCounter << " of '" << filename << "' : peakHeight defined before nPeaks defined"; noErrors=false;}
		}
		else if (command=="nconsts") {
			if (wordCounter==2) output->nconsts=atoi(tempValues[1].c_str()); 
			else throwLineError=true;
			if (output->nconsts<0) {cout << "\nERROR on line " << lineCounter << " of '" << filename << "' : nConsts must be positive integer"; noErrors=false;}		
		}
		else if (command=="nparams") {
			if (wordCounter==2) {
				output->nparams = atoi(tempValues[1].c_str());
				if (output->nparams>0) {
					output->x=(double*)calloc(output->nparams + output->nconsts,sizeof(double));
					output->bl=(double *)calloc(output->nparams,sizeof(double));
					output->bu=(double *)calloc(output->nparams,sizeof(double));
					for (i=0; i < output->nparams; i++) {
						while(readLine(&file, tempValues, &lineCounter, &wordCounter, &length, &nearEnd)==false) {} //loop allows comments in the list of parameters
						if (tempValues[0]=="#") {
							output->bl[i] = atof(tempValues[1].c_str());
							output->bu[i] = atof(tempValues[2].c_str());
							output->x[i] = atof(tempValues[3].c_str());
							if (fabs(output->x[i]+1.0)<0.01) output->x[i]=output->bl[i];
							else if (fabs(output->x[i]+2.0)<0.01) output->x[i]=(output->bl[i]+output->bu[i])/2;
							else if (output->x[i]>output->bu[i] || output->x[i]<output->bl[i]) {cout << "\nERROR on line " << lineCounter << " of '" << filename << "' : specified initial value outside boundaries"; noErrors=false;}		
						} else {cout << "\nERROR on line " << lineCounter << " of '" << filename << "' : parameter line following 'nparams' call not prefixed with #"; noErrors=false;}		
					}
					for (i=0; i < output->nconsts; i++) {
						while(readLine(&file, tempValues, &lineCounter, &wordCounter, &length, &nearEnd)==false) {}
						if (tempValues[0]=="#") output->x[i+output->nparams] = atof(tempValues[1].c_str());
						else {cout << "\nERROR on line " << lineCounter << " of '" << filename << "' : parameter line following 'nparams' (expected to be a const) call not prefixed with #"; noErrors=false;}		
					}
				} else {cout << "\nERROR on line " << lineCounter << " of '" << filename << "' : nParams must be positive integer"; noErrors=false;}			
			} else throwLineError=true;		
		}
		else if (command=="nelements") {
			if (wordCounter==2) {
				output->nElements = atoi(tempValues[1].c_str()); 
				if (output->nElements>0) {
					output->instrument->paramRefs_ = new int[5*output->nElements];
					for (i=0; i<output->nElements; i++) {
						while(readLine(&file, tempValues, &lineCounter, &wordCounter, &length, &nearEnd)==false) {} //loop allows comments in the list of parameters
						if (tempValues[0]=="#") {
							int type=atoi(tempValues[1].c_str());
							if (type==LISTSECTION_TYPE) {
								WindInstrument* tempIns = new WindInstrument();
								tempIns->setSupressedLists(true);
								if( (*tempIns).load(tempValues[2].c_str()) == false ) noErrors=false;
								output->instrument->append (tempIns, false);
								output->instrument->paramRefs_[5*i] = LISTSECTION_TYPE;
							}
							else {
									//Putting parameters into the instrument (needed in Rosenbrock objective function)
								for (int j=0; j<5; j++) output->instrument->paramRefs_[j+5*i]=atoi(tempValues[j+1].c_str());
								if (type==BOREJUMPSECTION_TYPE) output->instrument->append( new BoreJumpSection(), false);	
								if (type==CYLINDERSECTION_TYPE) output->instrument->append( new CylinderSection(true, true), false);	
								if (type==CONESECTION_TYPE) output->instrument->append( new ConeSection(true, true), false);	
								if (type==BESSELSECTION_TYPE) output->instrument->append( new BesselSection(true, true), false);	
								if (type==CYLINDERBENDSECTION_TYPE) output->instrument->append( new CylinderBendSection(true, true), false);									
								if (type==CONEBENDSECTION_TYPE) output->instrument->append( new ConeBendSection(true, true), false);	
								if (type==TERMINATIONELEMENT_TYPE) output->instrument->append( new TerminationElement(true, true), false);
								if (type==BRANCH_TYPE) output->instrument->append( new Branch(true, true), false);	
							}
						}
						else {cout << "\nERROR on line " << lineCounter << " of '" << filename << "' : parameter line following 'nelements' call not prefixed with #"; noErrors=false;}		
					}
				} else {cout << "\nERROR on line " << lineCounter << " of '" << filename << "' : nElements must be positive integer"; noErrors=false;}			
			} else throwLineError=true;
		} 
		else if (command=="#") {cout << "\nERROR on line " << lineCounter << " of '" << filename << " : # command found outside nparams/nelements list"; noErrors=false;}		
		else if (command!="Empty Line" && command!="Comment") {cout << "\nERROR on line " << lineCounter << " of '" << filename << "' : command (" << command << ") not recognised"; noErrors=false;}		
		if (throwLineError) {cout << "\nERROR on line " << lineCounter << " of '" << filename << "' : incorrect number of parameters (" << wordCounter << ")"; noErrors=false;}		
	}	

	// Checking for error-catching defaults (i.e. user has not specified a value)
	if (output->eps == -1.0) {cout << "\nERROR in '" << filename << "' : value of eps not specified"; noErrors=false;}			
	if (output->maxIter == -1) {cout << "\nERROR in '" << filename << "' : value of maxIter not specified"; noErrors=false;}			
	if (output->objParams.nPeaks == -1) {cout << "\nERROR in '" << filename << "' : value of nPeaks not specified"; noErrors=false;}			
	if (output->objParams.nPoints == -1) {cout << "\nERROR in '" << filename << "' : value of nPoints not specified"; noErrors=false;}			
	if (scoring == -1) {cout << "\nERROR in '" << filename << "' : value of scoring not specified"; noErrors=false;}			
	else {
		if (output->objParams.scoreFreq && output->objParams.peakFreq == NULL) {cout << "\nERROR in '" << filename << "' : freq scoring set but no target peakFreq given"; noErrors=false;}			
		if (output->objParams.scoreHeight && output->objParams.peakHeight == NULL) {cout << "\nERROR in '" << filename << "' : height scoring set but no target peakHeight given"; noErrors=false;}			
	}
	if (output->objParams.nModes == -1) {cout << "\nERROR in '" << filename << "' : value of nModes not specified"; noErrors=false;}			
	if (output->objParams.scoreIC && output->hasTarget==false) {cout << "\nERROR in '" << filename << "' : ic scoring set but no target .ic file given"; noErrors=false;}			

	file.close();
	if (noErrors) cout << " : done\n"; else cout << "\n : errors found, load failed\n"; 
	return noErrors;
}

void exploreSpace(const int n, const int xVar, const int yVar, double *x, const double *bl, const double *bu,
				const int nEvals, bool J(int,double *,double *,const double *, const double*,void *), void *extraparams, char filename[]) {
// Iterates through values of two variables of index xVar and yVar, evaluating the objective function at each and saving as a matlab 3d plot	
// Deprecated - this is not currently called anywhere, was written to generate a thesis result.
			
	double Jvalue=0.0;
	double* xMesh = new double[nEvals];
	double* yMesh = new double[nEvals];
	int i;
	ofstream file(filename);
	file << "function [X,Z] = explore();\n";
	file << "Z=[";
	
	double xValue=bl[xVar]; 
	double yValue=bl[yVar]; 

	for (i=0; i<nEvals; i++) {
		xMesh[i]=xValue;
		yValue = bl[yVar];
	
		for (int j=0; j<nEvals; j++) {
			if (i==0) yMesh[j]=yValue;
			x[xVar] = xValue;
			x[yVar] = yValue;
			(*J)(n, x, &Jvalue, bl, bu, extraparams);
			cout << endl;
			file << Jvalue << " , ";
			yValue+=(bu[yVar]-bl[yVar]-0.00001)/(nEvals-1);
		}
		file << " ; ";
		xValue+=(bu[xVar]-bl[xVar]-0.00001)/(nEvals-1);
	}

	file << "];\n";
	
	file << "X=[";
	for (i=0; i<nEvals; i++) file << xMesh[i] << " , " << yMesh[i] << " ; ";
	file << "];\n";
	
	file << "surface(X(:,2), X(:,1), Z);";
	file.close();				
}



#endif
