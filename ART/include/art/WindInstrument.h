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


#ifndef WINDINSTRUMENT_H
#define WINDINSTRUMENT_H

#include <stdlib.h>
#include <list>
#include <string>
#include <fstream>
#include "HornElement.h"
#include "ImpedanceCurve.h"

#include "BoreJumpSection.h"
#include "CylinderSection.h"
#include "ConeSection.h"
#include "BesselSection.h"
#include "Branch.h"
#include "CylinderBendSection.h"
#include "ConeBendSection.h"
#include "TerminationElement.h"

#include <string>
using namespace std;

#include "matrix.h"

#ifndef _NO_NAMESPACE 
using namespace std;
using namespace math;
#define STD std
#else
#define STD
#endif


#  if defined(_MSC_VER)
#     if _MSC_VER > 1000
#        include <complex>
         typedef complex<double> type;
#     endif
# include <complex>
typedef std::complex<double> type;
#  endif

#if !defined(__GNUG__)
_CRTIMP char * __cdecl itoa( int value, char* result, int base );
#endif
namespace ART{
  class WindInstrument : public GAGenome, public ART::HornElement {
private:
	list<HornElement*> elementList_;
//	static bool verboseMode_; 	//Whether text is output to screen as functions are called - defaults to true.
	bool outputMode_; 	//Whether output is saved in format for true:Maple, false:Matlab - defaults to true in MSVCPP6 and false in GCC
	bool supressedLists_; 	//Whether, when print() is called, the entire list is printed or not (useful when *this is not the primary list)
	
public:
	WindInstrument();
	//WindInstrument (const WindInstrument& orig) { copy(orig); }
	WindInstrument (const char filename[], const bool setDefault);
	WindInstrument(const WindInstrument& a);
	WindInstrument(HornElement *c);
	WindInstrument (const bool supressedLists, const char filename[]);

	~WindInstrument();

	list<HornElement*>::iterator iter;
	ImpedanceCurve impedanceCurve;
	int *paramRefs_; 

	static bool hasBends_;		// if true nodal-diameter modes are included
	static bool fixedTerm_;		// true if termination is specifed by user
	static bool hasBranch_;		// true if there are branches in the instrument (then fixedTerm is ignored)
	
	//Inherited Member Functions

	//From GAGenome
	GADefineIdentity("WindInstrument", 223);
	static float Compare(const GAGenome&, const GAGenome&) {std::cout << "C\n"; return 1.0;}
	static float Evaluate(GAGenome&) {std::cout << "E\n"; return 1.0;}

	WindInstrument& operator=(const GAGenome& orig) {if(&orig != this) copy(orig);	return *this; }
	virtual GAGenome* clone(CloneMethod) const {return new WindInstrument(*this);}  	//Note there is another clone method below, inherited from HornElement
	virtual void copy(const GAGenome& orig) {GAGenome::copy(orig); WindInstrument &child=(WindInstrument &)(orig); copyLists(&(child.elementList_));}


	//From HornElement

// needed by optimiser
	void elementInitialise(const double previousRout);
	int mutate (const float pmut, const float mu, const double previousRout);
	HornElement* clone() {return new WindInstrument (*this);}
	void elementCrossover (const HornElement& m, HornElement* s, HornElement* b, const float pcross) {} 

// object properties
	double rIn() const {return (*elementList_.begin())->rIn();}
	double rOut() const {return (*(--elementList_.end()))->rOut();}
	double gradientIn() const {return (*elementList_.begin())->gradientIn();}
	double gradientOut() const {return (*(--elementList_.end()))->gradientOut();}
//	Matrix Mzt(const int Ind) const {return (*(--elementList_.end()))->Mzt(Ind);}  // was needed when radiation was calculated during construction of TermElt
//	dcomp Zt(const int IndFreq) const {return (*(--elementList_.end()))->Zt(IndFreq);}
//	double abs_Zt(const int IndFreq) const {return (*(--elementList_.end()))->abs_Zt(IndFreq);}
//	double pha_Zt(const int IndFreq) const {return (*(--elementList_.end()))->pha_Zt(IndFreq);}
	string Name() const {return (*(--elementList_.end()))->Name();}
	double findRadius (const double x) const;
	string getSaveString() const {return "";}
	void print() const;


	dcomp zt(const double w) const {return (*(--elementList_.end()))->zt(w);}
	int RadiationType() const {return (*(--elementList_.end()))->RadiationType();}
    string filename() const {return (*(--elementList_.end()))->filename();} 
	int type() const {return 4;}

// methods for Input Impedance calculation
	int TlaAccumulate (const double w, int IndFreq, SimpleMatrix *a, const int Ind, vector<int>&mem, dcomp *zt,list<Matrix> *ImpedanceList,vector<int> &IndHole);
	int MultimodeAccumulate (const double w,int IndFreq, const bool bends,Matrix *a, const int Ind, vector<int>&mem, Matrix *zt,list<Matrix> *ImpedanceList,vector<int> &IndHole);
	void multimodeInputImpedance (const double w, const bool bends, Matrix *Zout);

// methods for Radiation Impedance calculation
	dcomp radiationImpedance(const double w) const {return (*(--elementList_.end()))->radiationImpedance(w);}
	dcomp ZorumskiPlaneRadiation(const double w) const {return (*(--elementList_.end()))->ZorumskiPlaneRadiation(w);}
	void multimodeRadiationImpedance(const double w, const bool bends, const int IndFreq, Matrix* result) const {(*(--elementList_.end()))->multimodeRadiationImpedance(w, bends, IndFreq, result);}
	void multimodeRadiationImpedanceClosedEnd(const double w, const bool bends, Matrix* result) const {(*(--elementList_.end()))->multimodeRadiationImpedanceClosedEnd(w, bends, result);}
	void multimodeRadiationImpedanceOpenEnd(const double w, const bool bends, Matrix* result) const {(*(--elementList_.end()))->multimodeRadiationImpedanceOpenEnd(w, bends, result);}
	void multimodeRadiationImpedanceTerminated(const double w, const bool bends, Matrix* result, const dcomp z_trm) const {(*(--elementList_.end()))->multimodeRadiationImpedanceTerminated(w, bends, result, z_trm);}
	dcomp PlaneWaveRadiationImpedance_TermElt(const double w) const {return (*(--elementList_.end()))->PlaneWaveRadiationImpedance_TermElt(w);}
	void radiationImpedanceLevine(const double w, double *InputRadius, dcomp *z_t) const {(*(--elementList_.end()))->radiationImpedanceLevine(w, InputRadius, z_t);}
	Matrix GetTerminationImpedance(const double w,int IndFreq, Matrix *zt, int test, int NextPos, vector<int>&mem,HornElement * iter,list<Matrix> *ImpedanceList);
	dcomp GetTerminationImpedance(const double w,int IndFreq, dcomp *zt, int test, int NextPos, vector<int>&mem,HornElement * iter,list<Matrix> *ImpedanceList);


	
	
//tests
	//Matrix multimodeRadiationImpedance_TermElt(const double w, const bool bends, dcomp z_tr) const {return (*(--elementList_.end()))->multimodeRadiationImpedance_TermElt(w, bends, z_tr);}
    //Matrix multimodeRadiationImpedance_TermElt(const double w, const bool bends) const {return (*(--elementList_.end()))->multimodeRadiationImpedance_TermElt(w, bends);}

// methods for branching
	void BranchImpedanceList(list<Matrix> *ImpListe){(*(--elementList_.end()))->BranchImpedanceList(ImpListe);}
	void PutRoutParam(const double rOut)  {(*(--elementList_.end()))->PutRoutParam(rOut);}
	void PutHoleParam(const double length_Hole, const double radius_Hole) {(*(--elementList_.end()))->PutHoleParam(length_Hole, radius_Hole);}
	Matrix GetParallelImpedance(const double w, const int IndFreq) {return (*(--elementList_.end()))->GetParallelImpedance(w,-1);}
	Matrix GetImpedance(const double w, const int IndFreq) {return (*(--elementList_.end()))->GetParallelImpedance(w,-1);}
	void ImpLengthCorrection(){(*(--elementList_.end()))->ImpLengthCorrection();}
	void ImpEltsAcoustic(){(*(--elementList_.end()))->ImpEltsAcoustic();}


// functions used to call the above methods. Called from ART.cpp within calc_imp, calc_rad, calc_Param or roughPlot/refinePeaks
//==============================================================================================================================
	vector <dcomp> getInputImpedance (const double f, const int t, int IndFreq,const int Contrib, const double gain, const int val,list<Matrix> *ImpedanceList,const int var,vector<int> &IndHole, const int accumulate);
	dcomp getInputImpedance (const double f, const int t, const bool accum);
	vector <dcomp> getParam(const double Freq, const double IndFreq, const int Contrib, const int Ind,list<Matrix> *ImpedanceList, const int var, vector<int> &IndHole);
	vector <dcomp> getRadiationImpedance(const double Freq, const double IndFreq, const int Contrib, const int Ind,list<Matrix> *ImpedanceList, const int var, vector<int> &IndHole);
	double inputImpedanceMagnitude (const double f, const int term, const bool accumulate);


//Member functions
	void cleanElements();
	void copyLists(list<HornElement*> *orig);
	void printList(bool indent);
	int getSize () {return elementList_.size();}
	void setToDefault();
	list<HornElement*>::iterator begin() {return elementList_.begin();}
//	list<HornElement*>::iterator end() {return elementList_.end();}
	int write(std::ostream & os) const;
	double length() const {return length_;}
	void setLength (double length) {length_ = length;}
	static int ListCrossover(const GAGenome&m, const GAGenome&d, GAGenome*s, GAGenome*b);
	static int ListMutator(GAGenome & c, float pmut);
	static void ListInitializer(GAGenome &c);
	void ListSplit(bool modifyDefault);
	
	bool check();
	void rosen (double *params); 

	list<HornElement*> getList() const {return elementList_;}
	HornElement* getElement (int i);
	void append (HornElement* a, bool addJumps);
	void erase ();
	void erase (int i);
	void chgtempC (const float tempC);
	void chglossF (const float lossF);
	void insert (HornElement* a);
	void setIterToBegin();
	void setIterToEnd();
	void setVerboseMode (const bool isVerbose) {verboseMode_=isVerbose; impedanceCurve.setVerboseMode(isVerbose);}
	void setSupressedLists (const bool isSupressed) {supressedLists_=isSupressed;}
	void setOutputMode (const bool isMaple) {outputMode_=isMaple; impedanceCurve.setOutputMode(isMaple);}
	bool getOutputMode() const {return outputMode_;}
	int getSize() const {return elementList_.size();}
	ImpedanceCurve getImpedanceCurve () { return impedanceCurve; }
	void printImpedanceCurve(const bool carriageReturn) const {impedanceCurve.print(carriageReturn);}
	void printDescription() const;
	void printList(const bool indent) const; 
	void compareBore (const char instrumentFile[], const char outputFile[]) const;
	void roughPlot(const int nModes, const double fmin, const double fmax, const int nPoints, const bool accumulate);

	void roughPlot(const int nModes, const double fmin, const double fmax, const int nPoints, const bool accumulate, const int termination);
	void refinePeaks(const double epsilon, const int t, const bool accumulate);
	void refinePeaks(const double epsilon);
	void recursiveQuadraticFindPeak (const double xLeft, const double rLeft, const double xCentre, const double rCentre, const double xRight, const double rRight, const double epsilon, const int counter, const int t, const bool accum);
	void recursiveCombinedFindPeak (const double xLeft, const double rLeft, const double xCentre, const double rCentre, const double xRight, const double rRight, const double epsilon, const int counter, const int t, const bool accum);
	void toLowerCase (char a[]) const;
	bool stringEquality (char a[], char b[]) const;
	int parseInt (const char input[], int *ptr) const;
	char* parseString (const char input[], int *ptr) const;
	double parseDouble (const char input[], int *ptr) const;
	//double* parseDoubleArray (const char input[], int *ptr, const int size) const;
	dcomp parseDcomp (const char input[], int *ptr) const;
	bool parseBool (const char input[], int *ptr) const;
	void runScript (const char filename[]);
	void saveImpedancePlot (const char filename[]);
	void saveInstrumentPlot (const char filename[]);
	void loadPointwise (const char filename[]);
	void savePointwise (const char filename[]);
	int savePointwise (std::ofstream *file, double *x, const bool asMaple);
	bool loadElemental (const char filename[]);
	void saveElemental (const char filename[]);
	bool load (const char filename[]);
	void save (const char filename[]);

	int term;

private:
	void saveInstrumentPlotMaple (const char filename[]);
	void saveInstrumentPlotMatlab (const char filename[]);

	static list<HornElement*> defaultList_;
	list<HornElement*>::iterator defaultIter;
	static ImpedanceCurve targetImpedanceCurve;
	static bool hasTarget;
	static bool verboseMode_;


public:
	static bool loadTargetImpedanceCurve (const char filename[]) {
		bool result = targetImpedanceCurve.load(filename);
		hasTarget = result;
		return result;
	}

	static bool getHasTarget() {return hasTarget;}
	static ImpedanceCurve* getTarget() {return &targetImpedanceCurve;}
	static void loadBase (const char filename[]) ;
};
}

#endif
