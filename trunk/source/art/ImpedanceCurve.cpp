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

/* Implements a data structure containing a list of 
**  points which describe an impedance curve.
** Based on Horn v2.01. Although the two classes have different purposes and
**  physical interpretations, they require very similar structure and member functions.
*/

#ifndef IMPEDANCECURVE_CPP
#define IMPEDANCECURVE_CPP

#include "ImpedanceCurve.h"
#include <iomanip>

void Horn::copyArrays (double x[], double r[], int size) {
	for (int i=0; i<size; i++) {
		xArray_.push_back(x[i]);
		rArray_.push_back(r[i]);
		fArray_.push_back(0); 		// WK
	}

	//cout << "copying array to std::vector: \n";
	//for (int i=0; i<size; i++) cout << i << ": " << xArray__.at(i) << " from " << x[i] << endl;
}

// WK
void Horn::copyArrays (double x[], double r[], double f[], int size) {
	for (int i=0; i<size; i++) {
		xArray_.push_back(x[i]);
		rArray_.push_back(r[i]);
		fArray_.push_back(f[i]);
	}
}

void Horn::copyLists (list<double> x, list<double> r) {
	xArray_ = x;
	rArray_ = r;
}

// WK
void Horn::copyLists (list<double> x, list<double> r, list<double> f) {
	xArray_ = x;
	rArray_ = r;
	fArray_ = f;
}

Horn::Horn() {
	xArray_ = list<double>();
	rArray_ = list<double>();
	fArray_ = list<double>();		// WK
	xIter_ = xArray_.begin();
	rIter_ = rArray_.begin();
	fIter_ = fArray_.begin();		// WK
}


Horn::Horn (double x[], double r[], int size) {
	xArray_ = list<double>();
	rArray_ = list<double>();
	fArray_ = list<double>();		// WK
	xIter_ = xArray_.begin();
	rIter_ = rArray_.begin();
	fIter_ = fArray_.begin();		// WK
	copyArrays (x, r, size);
}

// WK
Horn::Horn (double x[], double r[], double f[], int size) {
	xArray_ = list<double>();
	rArray_ = list<double>();
	fArray_ = list<double>();
	xIter_ = xArray_.begin();
	rIter_ = rArray_.begin();
	fIter_ = fArray_.begin();
	copyArrays (x, r, f, size);
}

Horn::Horn (double x, double r) {
	xArray_ = list<double>();
	rArray_ = list<double>();
	fArray_ = list<double>(); 		// WK
	xIter_ = xArray_.begin();
	rIter_ = rArray_.begin();
	fIter_ = fArray_.begin();		// WK
	xArray_.push_back(x);
	rArray_.push_back(r);
	fArray_.push_back(0);			// WK
}

// WK
Horn::Horn (double x, double r, double f) {
	xArray_ = list<double>();
	rArray_ = list<double>();
	fArray_ = list<double>();
	xIter_ = xArray_.begin();
	rIter_ = rArray_.begin();
	fIter_ = fArray_.begin();
	xArray_.push_back(x);
	rArray_.push_back(r);
	fArray_.push_back(f);
}

Horn::Horn (Horn& a) { 
	xArray_ = a.getX();
	rArray_ = a.getR();
	fArray_ = a.getF();
	xIter_ = xArray_.begin();
	rIter_ = rArray_.begin();
	fIter_ = fArray_.begin();
}

Horn::~Horn() {
	xArray_.clear();
	rArray_.clear();
	fArray_.clear();
}

void Horn::operator = (Horn& a) { //Copies a horn but DOES NOT copy iterators.
	xArray_=a.getX();
	rArray_=a.getR();
	fArray_=a.getF();

	xIter_ = xArray_.begin();
	rIter_ = rArray_.begin();
	fIter_ = fArray_.begin();
}

void Horn::clear() {
	//clears the lists.
	
	xArray_.clear();
	rArray_.clear();
	fArray_.clear();
}

list<double> Horn::getX()  const { return xArray_; }

double Horn::getX (int i) { //random access - deprecated

	list<double>::iterator tempIter = xArray_.begin();
	for (int j=0; j<i; j++) tempIter++; 

	return (*tempIter);
}

list<double> Horn::getR() const { return rArray_; }

double Horn::getR (int i)  { //random access - deprecated

	list<double>::iterator tempIter = rArray_.begin();
	for (int j=0; j<i; j++) tempIter++;
	
	return *tempIter;
}

// WK
list<double> Horn::getF() const { return fArray_; }

double Horn::getF (int i)  { //random access - deprecated

	list<double>::iterator tempIter = fArray_.begin();
	for (int j=0; j<i; j++) tempIter++;
	
	return *tempIter;
}

double Horn::getXcurrent() const {return *(xIter_);}

double Horn::getRcurrent() const {return *(rIter_);} // WK: corrected iterator

double Horn::getFcurrent() const {return *(fIter_);} // WK

int Horn::getSize() const { return xArray_.size(); }

void Horn::insert (const double x, const double r) {
	xIter_ = xArray_.insert(xIter_, x);
	rIter_ = rArray_.insert(rIter_, r);
	fIter_ = fArray_.insert(fIter_, 0);			// WK
}

// WK
void Horn::insert (const double x, const double r, const double f) {
	xIter_ = xArray_.insert(xIter_, x);
	rIter_ = rArray_.insert(rIter_, r);
	fIter_ = fArray_.insert(fIter_, f);
}

// WK
void Horn::insert (const double x, const dcomp r) {
        insert (x, abs(r), arg(r));
}

void Horn::append (const double x, const double r) {
	//Appends given point onto the end of the list.
		
	xArray_.push_back( x );
	rArray_.push_back( r );
	fArray_.push_back( 0 );					// WK
	if (xArray_.size()==1) setItersToBegin(); // If arrays were empty, sets pointer to begin (would be NULL else)
	//cout << "append :" << xArray_.size() << endl;
}

// WK
void Horn::append (const double x, const double r, const double f) {
	//Appends given point onto the end of the list.
		
	xArray_.push_back( x );
	rArray_.push_back( r );
	fArray_.push_back( f );
	if (xArray_.size()==1) setItersToBegin(); // If arrays were empty, sets pointer to begin (would be NULL else)
	//cout << "append :" << xArray_.size() << endl;
}

// WK
void Horn::append (const double x, const dcomp r) {
        append (x, abs(r), arg(r));
}
		
void Horn::print (const bool carriageReturn) const {
	// Returns a text representation to the screen.
	// If argument is true, a new line is used for each entry.
	// Output is formatted to be cut'n'pasted into Maple as a list

	list<double>::const_iterator tempxIter_ = xArray_.begin();
	list<double>::const_iterator temprIter_ = rArray_.begin();
	list<double>::const_iterator tempfIter_ = fArray_.begin();	// WK
	int size = this->getSize();
	cout << "size: " << size << endl;
	cout << "[";
	
//	cout << setiosflags (ios::fixed | ios::showpoint) << setprecision(6);

	while (true) { 
		cout << "[" << *tempxIter_ << ", " << *temprIter_ << ", " << *tempfIter_ << "]";
		tempxIter_++;
		temprIter_++;
		tempfIter_++;		// WK
		if (tempxIter_ == xArray_.end()) break; else cout << ", ";
		if (carriageReturn) cout << "\n";
	}
	cout << "]\n";
}

void Horn::setItersToBegin() {
	xIter_ = xArray_.begin();
	rIter_ = rArray_.begin();
	fIter_ = fArray_.begin(); 	// WK
}

void Horn::setItersToEnd() { 
	//Sets iterators to point to the last value (NOT to the actual end of the list)
	
	xIter_ = xArray_.end();
	rIter_ = rArray_.end();
	fIter_ = fArray_.end();		// WK
	xIter_--; rIter_--; fIter_--;	// WK
}

bool Horn::load (const char filename[]) {
	//Loads instrument from given filename, and sets this horn to equal it.
	//Instrument files use .int extension, and are arranged thus:
	/*  name
	 *  horn.getX(0), horn.getR(0)
	 *  etc.
	 */
	//Any line starting with a semi-colon ';' will be ignored as a comment.
	//The last horn r-value must match cyl[1] (the radius) unless cyl[1]=0.
	//#######Note that only float precision is used here######

	cout << "Loading ic file: " << filename << " ";

	ifstream file(filename);
	char buffer[100];

	if (!file.is_open()) {
		cout << "ERROR: File failed to open\n";
		return false;
	}

	//file.getline(name, 32);

	char xBuffer[100], rBuffer[100], fBuffer[100];
	int pointer=0;
	
	// WK
	while (!file.eof()) { //Reading horn from file
		xBuffer[0]='\0';
		rBuffer[0]='\0';
		fBuffer[0]='\0';		// WK
		file.getline(buffer, 100);
		if (buffer[0]!=';') { //ignoring comments
			pointer=100; //default case, will be reassigned in for loop
			//declare for-loop-var as local var to resolve VC compatibility issues...
			int i;
			for ( i=0; i<100; i++) {
				if (buffer[i]!=',') {xBuffer[i]=buffer[i];}
				else {xBuffer[i]='\0'; pointer=i; break;}
			}
						
			for ( i=pointer+2; i<100; i++) {							
				if (buffer[i]!=',') {rBuffer[i-pointer-2]=buffer[i];}
				else {rBuffer[i]='\0'; pointer=i; break;}
			}
						
			for ( i=pointer+2; i<100; i++) {
				if (buffer[i]!='\0') fBuffer[i-pointer-2]=buffer[i];
				else {fBuffer[i-pointer-2]='\0'; break;}
			}
			if (fBuffer[0]!='\0') 	append ((double)atof(xBuffer), (double)atof(rBuffer), (double)atof(fBuffer));
			else  			append ((double)atof(xBuffer), (double)atof(rBuffer));
		}
	}
	file.close();
	cout << " : done\n";
	return true;
}

void Horn::save (const char filename[]) {
	//Saves instrument to given file
	
	cout << "Saving impedance curve as file '" << filename << "'";
	setItersToBegin();
	
	ofstream file(filename);
	file << setiosflags (ios::fixed | ios::showpoint) << setprecision(6);

	//file << name << endl;
	for (int i=0; i<getSize()-1; i++) {
		file << *(xIter_) << ", " << *(rIter_) << ", " << *(fIter_) << endl; 	// WK
		xIter_++; rIter_++; fIter_++;								// WK
	}
	file << *(xIter_) << ", " << *(rIter_)<< ", " << *(fIter_);

	file.close();
	cout << " : done\n";
}


// ################## IMPEDANCE CURVE ###################


ImpedanceCurve::ImpedanceCurve() {
	peakListCounter_=0;
	xArray_ = list<double>();
	rArray_ = list<double>();
	fArray_ = list<double>();	// WK
	xIter_ = xArray_.begin();
	rIter_ = rArray_.begin();
	fIter_ = fArray_.begin();	// WK
	verboseMode_=true;
#if defined(__GNUG__)   //sets default output to Matlab on GCC, but Maple on MSVCPP
	outputMode_=false;
#else
	outputMode_=true;
#endif
}

ImpedanceCurve& ImpedanceCurve::operator = (const ImpedanceCurve& a) { 
	if (this == &a) return *this;
	
	copyLists( a.getX(), a.getR(), a.getF() );	// WK
	for (int i=0; i<30; i++) {
		peakList_[i][0] = a.getPeakList(i, 0);
		peakList_[i][1] = a.getPeakList(i, 1);
		peakNames_[i][0] = a.getPeakNames(i, 0);
		peakNames_[i][1] = a.getPeakNames(i, 1);
		centList_[i][0] = a.getCentList(i, 0);
		centList_[i][1] = a.getCentList(i, 1);
	}
	peakListCounter_ = a.getPeakListCounter();
	return *this;
}

void ImpedanceCurve::saveImpedancePlot(const char filename[]) {
	if(outputMode_) saveImpedancePlotMaple (filename);
	else saveImpedancePlotMatlab (filename); 
}

void ImpedanceCurve::saveImpedancePlotMaple(const char filename[]) {
	//Saves the curve in a format which Maple can then plot graphically
	//if pointPlot, then the points are superimposed on the curve
		
	setItersToBegin();
	char filenameWithExtension[17];
	strcpy(filenameWithExtension, filename);
	strcat(filenameWithExtension, ".mws");
	if(verboseMode_) cout << "Saving impedance curve as Maple plot '" << filenameWithExtension << "'"  << flush;
	ofstream file(filenameWithExtension);
	
	file << "plotList:=[";
	for (int i=0; i<getSize()-1; i++) {
		file << "[" << *(xIter_) << ", " << *(rIter_) << "], ";
		xIter_++;
		rIter_++;
	}
	file << "[" << *(xIter_) << ", " << *(rIter_) << "]]:";
	file << "plot(plotlist, style=line, color=red);\n";
	file.close();
	if(verboseMode_) cout << " : done" << endl;
}

void ImpedanceCurve::saveImpedancePlotMatlab(const char filename[]) {
	//Saves the curve in a format which Matlab can plot graphically
	
	setItersToBegin();
	char filenameWithExtension[17];
	strcpy(filenameWithExtension, filename);
	strcat(filenameWithExtension, ".m");
	if(verboseMode_) cout << "Saving impedance curve as Matlab plot: '" <<	filenameWithExtension << "'"  << flush;
	ofstream file(filenameWithExtension);
	file << setprecision(12);
	
	file << "function [icX, icY] = " << filename << "(makePlot);\n";

	file << "icX=[";

	//declare for-loop-var as local var to resolve VC compatibility issues...
	int i;

	for ( i=0; i<getSize()-1; i++) {
		file << *(xIter_) << ", ";
		xIter_++;
	}
	file << *(xIter_) << "];\n";
	
	file << "icY=[";
	for ( i=0; i<getSize()-1; i++) {
		file << *(rIter_) << ", ";
		rIter_++;
	}
	file << *(rIter_) << "];\n";
	file << "if(makePlot) plot(icX, icY); end;";
	
	file.close();
	if(verboseMode_) cout << " : done" << endl;
}


void ImpedanceCurve::saveLists (const char filename[], const bool fromSaveAsMaple) {
	//Saves the lists describing the peak to the file
	
	ofstream file;
	if(fromSaveAsMaple) file.open(filename, ios::app); else {file.open(filename, ios::trunc); namePeaks();}

	if(verboseMode_) cout << "Saving peak data: " << filename  << flush;
	
	//declare for-loop-var as local var to resolve VC compatibility issues...
	int i;

	file << "peakList_:=[";
	for ( i=0; i<peakListCounter_-1; i++) { //Defines peakList_ in Maple
		file << "[" << peakList_[i][0] << ", " << peakList_[i][1] << "], ";
	}
	file << "[" << peakList_[peakListCounter_-1][0] << ", " << peakList_[peakListCounter_-1][1] << "]]:\n";

	file << "centList_:=["; 
	for ( i=0; i<peakListCounter_-1; i++) { //Defines centList_ in Maple
		file << "[" << centList_[i][0] << ", " << centList_[i][1] << "], ";
	}
	file << "[" << centList_[peakListCounter_-1][0] << ", " << centList_[peakListCounter_-1][1] << "]]:\n";


	char noteNames[13][3]={"C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B", "C"};

	file << "peakNames_:=[";
	for ( i=0; i<peakListCounter_-1; i++) { //Defines peakNames_ in Maple
		file << "['" << noteNames[(int)peakNames_[i][0]] << "', "<< peakNames_[i][1] << "], ";
	}
	file << "['" << noteNames[(int)peakNames_[peakListCounter_-1][0]] << "', "<< peakNames_[peakListCounter_-1][1] << "]]: ";
	
	//file << "for i from 1 to " << peakListCounter_ << " do print(i, peakList_[i][1], peakNames_[i], centList_[i][2]); od;\n";
		//This gets Maple to produce an output describing each peak, it's (uncorrected) frequency, the closest note and cents sharp
	
	if (fromSaveAsMaple) file << "plot(centList_);\n";
	file.close();
	if(verboseMode_) cout << " : done" << endl;
}

void ImpedanceCurve::peakListAppend(const double x, const double y) {peakListAppend(x, y, 0.0);}

void ImpedanceCurve::peakListAppend(const double x, const double y, const double e) {
	//Appends given value (freq, imp, freq error) to peakList_
	if (peakListCounter_<30) {
		peakList_[peakListCounter_][0]=x;
		peakList_[peakListCounter_][1]=y;
		peakList_[peakListCounter_][2]=e;
		peakListCounter_++;
	}
}

void ImpedanceCurve::peakListModify(const double x, const double y)	{peakListModify(x, y, 0);}

void ImpedanceCurve::peakListModify(const double x, const double y, const double e) { //resets value currently pointed to in peak list to given values
	peakListCounter_--;
	peakList_[peakListCounter_][0]=x;
	peakList_[peakListCounter_][1]=y;
	peakList_[peakListCounter_][2]=e;
	peakListCounter_++;
}

void ImpedanceCurve::namePeaks() {
	//Examines peakList_ and sets peakNames_ to describe which notes the peaks correspond to
	// and how in tune they are.
	
	if(verboseMode_) cout << "Naming peaks"  << flush;

	double xi = pow(2.0, 0.08333333);
	double noteList[13];
	double temp;
	double nt=1/log(2.0); //**
	int n, j; //Denotes which octave the peak is in.
	noteList[0]=27.5*pow(xi, 3); //Defines C1

	//declare for-loop-var as local var to resolve VC compatibility issues...
	int i;

	for ( i=1; i<=12; i++) { //Builds notelist as a list of frequencies of notes in octave 1
		noteList[i]=noteList[i-1]*xi;
	}

	for ( i=0; i<peakListCounter_; i++) {
		temp=peakList_[i][0];
		
		for (n=1; n<=8; n++) {
			if (temp>noteList[11]) temp=temp/2; else break; 
		} //temp is now the peak freq. transposed into octave 1
		for (j=0; j<12; j++) { 
			if (temp>noteList[j] && temp<noteList[j+1]) {
				if (temp-noteList[j]>noteList[j+1]-temp) j++; //finds which note temp is closest to
				break;
			}
		}
		
		peakNames_[i][0]=j;
		peakNames_[i][1]=n;
		if (j==12) {n--; temp*=2;}
		centList_[i][0]=noteList[j]*pow(2.0, n-1.0);
		centList_[i][1]=1200*nt*log(temp/noteList[j]);
	} 
	if(verboseMode_) cout << " : done" << endl;
}

float ImpedanceCurve::scoreCurve (ImpedanceCurve *target, const float cic, const float betaheight) {
	//Scores this ic by comparing its impedance values to the ic given - must have target having exactly the same nPoints
	
	setItersToBegin();
	(*target).setItersToBegin();

	float ydiff, sigma;
	float score=0.0;
	float tempscore;

	const int icSize = getSize();
	const int targetSize = (*target).getSize();

	if (icSize != targetSize) {cout << "ERROR: IC size mismatch: calculated size=" << icSize << ", target size=" << targetSize << endl; return 0.0; }
	for (int i=0; i<icSize-1; i++) {

		ydiff=fabs( (*rIter_) - *((*target).rIter_));
		sigma = cic;
		tempscore=0;
		if (ydiff<=sigma) tempscore += 1-(ydiff*ydiff)/sigma/sigma; //Least-squares approach
	
		score+=tempscore;
		rIter_++;
		(*target).rIter_++;
	}
	score = score/(icSize-1)*10; //score from 0 to 10.
	cout << score << " ";
	return score;
}

float ImpedanceCurve::scorePeaks (const objectiveParameters* params, const bool scoreFreq, const bool scoreHeight) {
	//returns score for given parameter set 
	//Score is calculated by comparing the peaks of this impedance curve to the values given.
	
	if (!scoreFreq && !scoreHeight) return 0.0;
	if (getPeakListCounter()<params->nPeaks) {cout << getPeakListCounter() << " = too few peaks"; return 0;}
		
	float freqScore=0.0, heightScore=0.0;
	float xdiff, ydiff;
	float alpha, a, b, gamma, c, d, xi, sigma; 	//Used in calculation of fitness

	//###Calculating score###
		
	for (int i=0; i<params->nPeaks; i++) {
		xi = params->nu_phi;		// * target[i]; 
		sigma = fabs(params->nu_z * params->peakHeight[i]);
	
		if (scoreFreq) {
			xdiff = getPeakList(i,0) - params->peakFreq[i];
			if (fabs(xdiff)<=xi) { 
				alpha = params->mu_phi/xi/xi;
				a = -1/(exp(-alpha*xi*xi)-1);
				b = 1-a;
				freqScore += (a*exp(-alpha*xdiff*xdiff)+b); //Gaussian
				// freqScore += 1-(xdiff*xdiff)/xi/xi; //Least-squares

			}
		}

		if (scoreHeight) {
			ydiff = getPeakList(i,1)-params->peakHeight[i];
			if (fabs(ydiff)<=sigma) {	//Gaussian approach
				gamma = params->mu_z/sigma/sigma;
				c = -1/(exp(-gamma*sigma*sigma)-1); //not the same c as cfreg, cheight!
				d =	1-c;
				heightScore += (c*exp(-gamma*ydiff*ydiff)+d); //Gaussian
				// heightScore += 1- ydiff*ydiff/sigma/sigma; //Least-squares
			}
		}
	}
	freqScore = freqScore/params->nPeaks*10;
	heightScore = heightScore/params->nPeaks*10;
	if(scoreFreq) cout << freqScore << " ";
	if(scoreHeight) cout << heightScore << " ";
	return (freqScore + heightScore);
}

float ImpedanceCurve::scoreChallengeEFP () { 
	//An EFP-based peak scoring routine customised for French Trumpet Challenge. 
	
	float score=0, tempscore=0, efpcalc;
	const double f0=getPeakList(3,0)/4;
	const float EFPlimit = 50;
	// LINEAR SCORING 
	for (int i=1; i<8; i++) {
		efpcalc = efp(f0, getPeakList(i,0), i+1);
		if (!(efpcalc>EFPlimit || efpcalc<-EFPlimit)) {
			if (efpcalc<0) tempscore = 1+efpcalc/EFPlimit; else tempscore = 1-efpcalc/EFPlimit;
			score +=tempscore;
		}
	}
	/*GAUSSIAN SCORING
	const float betafreq=100;
	float alpha,a,b;
	for (int i=1; i<8; i++) {
		efpcalc = efp(f0, getpeakList(i,0), i+1);
		if (fabs(efpcalc)<=EFPlimit) { 
			alpha = betafreq/EFPlimit/EFPlimit;
			a = -1/(exp(-alpha*EFPlimit*EFPlimit)-1);
			b = 1-a;
			score += (a*exp(-alpha*efpcalc*efpcalc)+b);
		}
	}*/
	return score/7*10;
}

float ImpedanceCurve::scoreChallenge () { 
	//Simple harmonic multiple scoring routine for above challenge
	
	float score=0, tempscore=0;
	
	double 
	f2=getPeakList(1,0),
	f3=getPeakList(2,0),
	f4=getPeakList(3,0),
	f5=getPeakList(4,0),
	f6=getPeakList(5,0),
	f8=getPeakList(7,0);

	tempscore = f3/f2;
	cout << tempscore;
	tempscore = fabs(1.5-tempscore);
	score += tempscore;
	cout << ", " << tempscore << ", " << score << endl;

	tempscore = f4/f2;
	cout << tempscore;
	tempscore = fabs(2-tempscore);
	score += tempscore;
	cout << ", " << tempscore << ", " << score << endl;

	tempscore = f6/f4;
	cout << tempscore;
	tempscore = fabs(1.5-tempscore);
	score += tempscore;
	cout << ", " << tempscore << ", " << score << endl;

	tempscore = f8/f4;
	cout << tempscore;
	tempscore = fabs(2-tempscore);
	score += tempscore;
	cout << ", " << tempscore << ", " << score << endl;

	tempscore = f5/f4;
	cout << tempscore;
	tempscore = fabs(1.25-tempscore);
	score += tempscore;
	cout << ", " << tempscore << ", " << score << endl;

	return 10-score;
}

void ImpedanceCurve::importBIAS (const char filename[]) {
// Takes a BIAS ASCII file and loads it as an ImpedanceCurve.

	if(verboseMode_) cout << "Importing BIAS ASCII: '" << filename << "'"  << flush;

	ifstream file(filename);
	char buffer[32];
	double freq = 0.0;
	const float step = 0.5;

	if (!file.is_open()) {
		cout << "ERROR: File failed to open\n";
		return;
	}

	while (file >> buffer) {
		append (freq, ((double)atof(buffer))*1000);	
		freq+=step;
	}

	file.close();
	if(verboseMode_) cout << " : done" << endl;
	findPeaks(true);
	//convertToStandard();
}

void ImpedanceCurve::convertToStandard() {
//Converts this ic into (8,2000) Hz and 498 points. Assumes it already has points exactly at multiples of 8Hz
	
	const int nPoints = 498;
	const float freqStep = 1992/nPoints;
	
	if(verboseMode_) cout << "Converting to standard form, " << nPoints << " points, mesh size=" << freqStep << flush;

	setItersToBegin();
	double divisor;

	do {
		divisor = (*xIter_)/freqStep; 
		if (divisor != (double)((int)divisor) || (*xIter_)<7.99 || (*xIter_)>1999.99) 
		{ 
			xIter_ = xArray_.erase(xIter_); 
			rIter_ = rArray_.erase(rIter_);
			fIter_ = fArray_.erase(fIter_);
		}
		else {xIter_++; rIter_++; fIter_++;}
				
	} while (xIter_!=xArray_.end());

	if(verboseMode_) cout << " : done" << endl;
}

void ImpedanceCurve::findPeaks(const bool ignoreNoise) {
	//Finds peaks. If ignoreNoise, any data <50Hz are ignored (useful for imported ASCIIs). 
	//Uses internal iterators.
	
	if(verboseMode_) cout << "Finding peaks:" << flush;

	bool gradient[3]; //true means +ve gradient, false means -ve gradient)
	double rCurrent = 0, rPrev, rPrevPrev, xCurrent = 0, xPrev, xPrevPrev, xPrevPeak = 0, rPrevPeak = 0;
	const double threshold = 10; //max distance between 'truncated' double-peaks
	int peakCounter=1;
	
	setItersToBegin();
	xPrev = *(xIter_);
	rPrev = *(rIter_);
	rIter_++; xIter_++;
	rCurrent = *(rIter_);
	xCurrent = *(xIter_);
        gradient[1] = false;
	gradient[2] = rPrev < rCurrent; 

	do {
		xPrevPrev = xPrev;
		rPrevPrev = rPrev;
		rPrev = rCurrent;
		xPrev = xCurrent;
		rIter_++; xIter_++;
		rCurrent = *(rIter_);
		xCurrent = *(xIter_);
		gradient[0] = gradient[1];
		gradient[1] = gradient[2];
		gradient[2] = rPrev < rCurrent;
			
		if (gradient[0] && gradient[1] && !gradient[2]) {
			if (!(ignoreNoise && xPrev<40.0)) {
				if(xPrev - xPrevPeak > threshold) {
					peakListAppend(xPrev, rPrev);
					xPrevPeak = xPrev;
					rPrevPeak = rPrev;
					peakCounter++;
				}
				else {
					bool a=(rPrev>rPrevPeak);
					peakListModify((xPrev+xPrevPeak)/2, a*rPrev	+ !a*rPrevPeak, (xPrev-xPrevPeak)/2);			
				}
			}
		}
	} while (!itersNearEnd()); 

	if(verboseMode_) cout << " : done" << endl;
}


void ImpedanceCurve::printPeaks() const {
	//Prints data to screen detailing peak locations
	
	cout << "--------------------------------------------\n";
	cout << "Peak list: (frequency(Hz), impedance (Ohm))\n";
	cout << "--------------------------------------------\n";
	for (int i=0; i<peakListCounter_; i++) {
		cout << peakList_[i][0] << ", " << peakList_[i][1]; //freq. and impedance
	//	cout << ", " << peakList_[i][2]; //'freq. error' (is 0 unless curve had truncated peaks, where it is the span of the two peaks
	//	cout << ", " << efp(peakList_[3][0]/4, peakList_[i][0], i+1); //EFP of peak location rel. to peak 4
		cout << endl;
	}
	cout << "--------------------------------------------\n";
	printPeakData();
	cout << "--------------------------------------------\n";
}

void ImpedanceCurve::printPeaks(double* insParams) const {
	//Prints data to screen detailing peak locations and compares them to optimisation target data given (called after Rosenbrock) (Deprecated)
	cout << "\nPEAK LIST: freq., impedance, result EFP (target rel. EFP)\n";
	double efpValue;

	for (int i=0; i<peakListCounter_; i++) {
		cout << i+1 << ": ";
		cout << peakList_[i][0] << ", " << peakList_[i][1]; //freq. and impedance
				
		efpValue = efp(peakList_[3][0]/4, peakList_[i][0], i+1);

		cout << ", " << efpValue; //EFP of peak location rel. to peak 4
		cout << " (" << (efpValue - efp(peakList_[3][0]/4, insParams[6+i], i+1)) << ")"; //EFP of target peak location rel. to result peak 4
		cout << endl;
	}
}

void ImpedanceCurve::printPeakData() const {
	//Prints a simple list of peak frequencies in a format to be cut'n'pasted into a .rb or .gap file 
	const int limit = peakListCounter_-1;
	cout << "peakfreq ";

	//declare for-loop-var as local var to resolve VC compatibility issues...
	int i;

	for ( i=0; i<peakListCounter_; i++) {
		cout << peakList_[i][0];
		if (i!=limit) cout << ",";
	}
	cout << endl;

	cout << "peakheight ";

	for ( i=0; i<peakListCounter_; i++) {
		cout << peakList_[i][1];
		if (i!=limit) cout << ",";
	}
	cout << endl;
}

double ImpedanceCurve::efp(const double F, const double fn, const int n) const {
	//Calculates efp of given tone (fn) related to fundamental (F) and mode number (n)
	return ( 1200/LOG2 * log10(fn/n/F) );
}

double ImpedanceCurve::inverseEFP(const double efp, const double F, const int i) const {
	//Calculates the frequency which is efp cents away from the i^th harmonic of frequency F.
	return ( i*F*pow(10, LOG2*efp/1200) );
}

double ImpedanceCurve::getMagnitude(const double f) {
	//Returns impedance magnitude at given frequency. Interpolates linearly between known values.

	double rCurrent=0, rPrev, xCurrent=0, xPrev;
	setItersToBegin();
	xPrev = *(xIter_);
	rPrev = *(rIter_);
	if (f<=xPrev) return rPrev/xPrev*f;
	
	do {
		rPrev = rCurrent;
		xPrev = xCurrent;
		rIter_++; fIter_++; xIter_++;		// WK
		rCurrent = *(rIter_);
		xCurrent = *(xIter_);
		if(f>xPrev && f<=xCurrent) return ((rCurrent-rPrev)/(xCurrent-xPrev)*(f-xPrev) + rPrev);
	} while (!itersNearEnd()); 
	return rCurrent;
}

// WK
dcomp ImpedanceCurve::getImpedance(const double f) {
	//Returns complex impedance at given frequency. Interpolates linearly between known values.

	double rCurrent, rPrev, fCurrent, fPrev, xCurrent, xPrev;
	xIter_--;					// need xPrev again from previous call
	if (f<*(xIter_)) setItersToBegin();		// WK: optimised if retrieved in sequence
	else {rIter_--; fIter_--;}			// WK: don't rewind if position matches
	xCurrent = *(xIter_);
	rCurrent = *(rIter_);
	fCurrent = *(fIter_);
	if (f<=xCurrent) {
		double m = rCurrent/xCurrent*f;
		double a = fCurrent/xCurrent*f;
		return dcomp(m*cos(a), m*sin(a));
	}
	
	do {
		rPrev = rCurrent;
		fPrev = fCurrent;
		xPrev = xCurrent;
		rIter_++; fIter_++; xIter_++;
		rCurrent = *(rIter_);
		fCurrent = *(fIter_);
		xCurrent = *(xIter_);
		if(f>xPrev && f<=xCurrent) {
			double m = (rCurrent-rPrev)/(xCurrent-xPrev)*(f-xPrev) + rPrev;
			double a = (fCurrent-fPrev)/(xCurrent-xPrev)*(f-xPrev) + fPrev;
			return dcomp(m*cos(a),m*sin(a));
		}
	} while (!itersNearEnd()); 
	return dcomp(rCurrent*cos(fCurrent),rCurrent*sin(fCurrent));
}

void ImpedanceCurve::harmonicityPlot(const double efpMin, const double efpMax, const int nEvals, const int peakMin, const int peakMax, char filename[]) {
	const double F = getPeakList(3,0)/4.0;
	harmonicityPlot(efpMin, efpMax, nEvals, peakMin, peakMax, filename, F);
}
	

void ImpedanceCurve::harmonicityPlot(const double efpMin, const double efpMax, const int nEvals, const int peakMin, const int peakMax, char filename[], const double F) {
	// Computes and outputs a harmonicity plot, with overlaid EFP plot, as a MATLAB file.	
	// Requires peakList to have correct data
		
	//findPeaks(false);
	if(verboseMode_) cout << "Compiling Harmonicity Plot : "  << filename << flush;
			
	double Jvalue=0.0;
	double* xMesh = new double[nEvals];
	double* yMesh = new double[nEvals];
	double Jmax=0.0;
	double Jmin=HUGE_VAL;
	ofstream file(filename);
	file << "function [X,Y,Z] = harmonicity();\n";
	file << "Z=[";
	
	double xValue=efpMin; 
	int yValue=peakMin; 
	const int nySteps = peakMax-peakMin+1;

	//declare for-loop-var as local var to resolve VC compatibility issues...
	int i;

	for ( i=0; i<nEvals; i++) {
		xMesh[i]=xValue;
		yValue = peakMin;
		for (int j=0; j<nySteps; j++) {
			if (i==0) yMesh[j]=yValue;
			double f=inverseEFP(xValue, F, yValue);
			Jvalue = getMagnitude(f);
			if (Jvalue>Jmax) Jmax=Jvalue;
			else if (Jvalue<Jmin) Jmin=Jvalue;
			//cout << xValue << ", " << yValue << " : " << f << ", " << Jvalue << endl;
			file << Jvalue << " , ";
			yValue+=1;
		}
		file << " ; ";
		xValue+=(efpMax-efpMin-0.00001)/(nEvals-1);
	}

	cout << " : Magnitude range = [" << Jmin << " " << Jmax << "]," << " F = " << F;

	file << "];\n";
	file << "Z=Z.';\n";
	
	file << "X=[";
	for ( i=0; i<nEvals; i++) file << xMesh[i] << " , " ;
	file << "];\n";

	file << "Y=[";
	for ( i=0; i<nySteps; i++) file << yMesh[i] << " , " ;
	file << "];\n";
	
//	file << "surface(X, Y, Z);";
	file << "hold on;";
	file << "for i=1:1:length(Y)\n";
	file << "surface(X,[Y(i),Y(i)+1],[Z(i,1:length(X));Z(i,1:length(X))])\n";
	file << "end;\n";
	file << "shading interp;\n";
	file << "colormap gray(256);\n";
	file << "set(gca,'CLim',[" << Jmin << " " << Jmax << "]);\n";
	file << "set(gca,'XLim',[-100 100]);\n";
	file << "colorbar;\n";
	
	file << "EFP=[";
	for ( i=0; i<nySteps; i++) file << efp(F, getPeakList(peakMin-1+i,0), peakMin+i) << " , " ;
	file << "];\n";
	
	file << "elev=ones(1," << nySteps << ")*" << Jmax*1.1 << ";\n";
	file << "plot3(EFP,Y+0.5,elev,':x','LineWidth',2,'Marker','o','MarkerSize',8,'MarkerEdgeColor',[0 0 0],'MarkerFaceColor',[1 1 1],'Color','black');\n"; 
	file << "xlabel('EFP/cents'); ylabel('Peak number'); zlabel('Impedance magnitude');\n";
	file << "ylim([" << peakMin << ", " << peakMax+1 << "]);\n";
	
	file.close();	
	if(verboseMode_) cout << " : done" << endl;			
}

#endif

