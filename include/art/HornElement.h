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

// also includes definitions of BoreJumpSection, CylinderSection, ConeSection, BesselSection, CylinderBendSection, ConeBendSection, TerminationElement, Branch

#ifndef HORNELEMENT_H
#define HORNELEMENT_H

#include <iostream>
#include <fstream>
//#include <stdlib.h>
#ifdef __GNUG__
	#include <cstring>
#else
	#include <string>
#endif
#include <complex>
#include <list>
#include <vector>
#include "ARTlink.h" //used to link hornelements to ARTelements
#include "SimpleMatrix.h"
#include "matrix.h"
//#include <ga/garandom.h> //used for Bessel-functions
#include "LeGquadrature.h"


#ifndef _NO_NAMESPACE 
//using namespace std;
using namespace math;
#define STD std
#else
#define STD
#endif

//#define TRACE
//#define TRACE1
//#define TRACE2

#ifdef TRACE
#	ifdef TRACE1
#		define DBG1
#	else
#		define DBG1 if (false) 
#	endif
#	ifdef TRACE2
#		define DBG2
#	else
#		define DBG2 if (false) 
#	endif
#else
#	ifdef _USRDLL
#		if !defined(NDEBUG)
#			define DBG1
#			define DBG2
#		else
#			define DBG1 if (false) 
#			define DBG2 if (false) 
#		endif /* _DEBUG */
//#		define std::cout if (false) std::cout 
#	else
#		define DBG1 if (false) 
#		define DBG2 if (false) 
#	endif
#endif	

/*
//What other std::cout could be used??? Well, I will leave this ugly definition for now...
#ifdef _CONSOLE
#		define std::cout std::cout 
#endif
//...and add my own BUT in any case: we'll undefine it at the end of this file!
#ifdef ARTinterfaceTest
#		define std::cout std::cout 
#endif
*/

//using namespace std;
typedef std::complex<double> dcomp;
typedef matrix<dcomp> Matrix;

//WK removed const TEMP..ETA - made them variables tempC and lossF and global functions C(T), RHO(T) and ETA(T)
//const float TEMP = 20;//13.85; //Celcius
//const float DT = TEMP - 26.85;
//const double C = 3.4723e4 * (1+0.00166*DT); 	//Speed of sound in air
//const double RHO = 1.1769e-3 * (1-0.00335*DT); //Density of ambient air
//const float ETA = 1.846e-4 * (1+0.0025*DT); //Shear viscosity

const dcomp J = dcomp(0,1);
const dcomp cinfinity = dcomp(1.0E24,0);  
const double ROOT2 = 1.41421356237;
const double ROOT2INV = 0.707106781187;
#ifndef M_PI_2
const double M_PI_2 = M_PI / 2;
#endif
#ifndef M_PI_4
const double M_PI_4 = M_PI / 4;
#endif
const double eps = 1.0E-6;

//WK removed LOSSY - made it global variable LOSSY_
//const bool LOSSY=1; 
 //States whether wall losses are included in impedance calculations

// Zeroes of Bessel function J1
const double GAMMA_I[32] = {0.0, 3.8317059702, 7.0155866698, 10.1734681351, 13.3236919363, 16.4706300509, 19.61686, 22.76008, 25.90367, 29.04683, 32.18968, 35.33231, 38.47477, 41.61709, 44.75932, 47.90147, 51.0435352, 54.1855536, 57.3275254, 60.4694578, 63.6113567, 66.7532267, 69.8950718, 73.0368952, 76.1786996, 79.3204872, 82.4622599, 85.6040194, 88.7457671, 91.8875043, 95.0292318, 98.1709507};
//square of above
const double GAMMA_I2[32] = {0.0, 14.68192489, 49.21864336, 103.5001023, 177.5209817, 271.2806644, 384.787456, 518.017600, 671.017216, 843.728209, 1036.196100, 1248.350224, 1480.307636, 1731.98253072, 2003.39663714, 2294.55, 2605.442486, 2936.074219, 3286.445168, 3656.555327, 4046.404701, 4455.993275, 4885.321062, 5334.388060, 5803.194273, 6291.739690, 6800.024308, 7328.048137, 7875.811178, 8443.313446, 9030.554896, 9637.535561};

//Gauss-Legendre coefficients for a..b = 0..10 with 20 points.
const double GAUSSX[20] = {0.034357, 0.18014, 0.438828, 0.804415, 1.26834, 1.81973, 2.44566, 3.13147, 3.86107, 4.61737, 5.38263, 6.13893, 6.86853, 7.55434, 8.18027, 8.73166, 9.19558, 9.56117, 9.81986, 9.96564};
const double GAUSSW[20] = {0.08807, 0.203007, 0.31336, 0.416384, 0.509651, 0.590973, 0.658443, 0.710481, 0.745865, 0.763767, 0.763767, 0.745865, 0.710481, 0.658443, 0.590973, 0.509651, 0.416384, 0.31336, 0.203007, 0.08807};
const int GAUSSN = 20;
//cosh of Gauss x[i]
const double COSHGAUSSX[20] = {1.00059, 1.01627, 1.09784, 1.34137, 1.91812, 3.16613, 5.81242, 11.4756, 23.7686, 50.6187, 108.799, 231.78, 480.768, 954.505, 1784.91, 3098, 4926.74, 7101.23, 9197.74, 10641.2};

const int mbend_[32]={0, 1, 2, 0, 3, 4, 1, 5, 2, 0, 6, 3, 1, 7, 4, 8, 2, 0, 5, 9, 3, 1, 6, 10, 4, 11, 7, 2, 0, 12, 5, 8}; // Maps from alpha to m (see FP02 - this is the order of the modes sorted by cutoff freq gamma_m_n)
const int nbend_[32]={0, 0, 0, 1, 0, 0, 1, 0, 1, 2, 0, 1, 2, 0, 1, 0, 2, 3, 1, 0, 2, 3, 1, 0, 2, 0, 1, 3, 4, 0, 2, 1}; // Maps from beta to n

//Indices used to identify each type of derived class.
const int FILE_RADIATION = -2;
const int UNKNOWN_RADIATION = -1;
const int REFLECTING_RADIATION = 0;
const int ZORUMSKI_RADIATION = 1;
const int LEVINE_RADIATION = 2;
const int CUSTOM_RADIATION = 3;

const int BOREJUMPSECTION_TYPE=0;
const int CYLINDERSECTION_TYPE=1;
const int CONESECTION_TYPE=2;
const int BESSELSECTION_TYPE=3;
const int LISTSECTION_TYPE=4;
const int CYLINDERBENDSECTION_TYPE=5;
const int CONEBENDSECTION_TYPE=6;
const int TERMINATIONELEMENT_TYPE=7;
const int BRANCH_TYPE=8;
const int INPUT_TYPE=9;
const int nQuadraturePoints=50;

const string BOREJUMPSECTION_TYPE_STR="0";
const string CYLINDERSECTION_TYPE_STR="1";
const string CONESECTION_TYPE_STR="2";
const string BESSELSECTION_TYPE_STR="3";
const string LISTSECTION_TYPE_STR="4";
const string CYLINDERBENDSECTION_TYPE_STR="5";
const string CONEBENDSECTION_TYPE_STR="6";
const string TERMINATIONELEMENT_TYPE_STR="7";
const string BRANCH_TYPE_STR="8";
const string INPUT_TYPE_STR="9";

class HornElement {
public:

	HornElement(const float tempC=21.0, const float lossF=1.0, const float humidity=80, 
		const float xc=382, const string name=" ", int type=-1, Matrix MA=NULL, Matrix MB=NULL, 
		double rr=0.0, double length=0.0, const bool canSplit=false, const bool canModify=false): type_(type), MA_(MA), MB_(MB), rr_(rr), tempC_(tempC), lossF_(lossF), 
		humidity_(humidity), xc_(xc), name_(name), length_(length), canSplit_(canSplit), canModify_(canModify) {}
	virtual ~HornElement() {}

	//Sadjad: setnModes used to be static, but when we change the number of modes, different elements need to
	//do different preparations. The bent elements (CylinderBendSection & ConeBendSection) override this method!
	virtual void setnModes(const int nModes) 		// nModes_: take so many modes into account 
		{if (nModes_!=nModes) {nModes_=nModes; if (nModes>0) initMatrices();};}

// global class properties, same for all object instances
// these need to be declared only once; they are called from set_nModes from ART.cpp
//====================================================================================
	static int getnModes() {return nModes_;} // nowhere used yet

	static void setlossy(const bool lossy) {LOSSY_=lossy;}	// don't ignore viscous losses
	static bool getlossy() {return LOSSY_;} // nowhere used yet

	static void setfmin(const double fmin) {F_MIN_=fmin;}	// fmin
	static double getfmin() {return F_MIN_;} // nowhere used yet

	static void setfmax(const double fmax) {F_MAX_=fmax;}	// fmax
	static double getfmax() {return F_MAX_;} // nowhere used yet

	static void setdf(const double df) {D_F_=df;}	// df
	static double getdf() {return D_F_;} // nowhere used yet

	static void setsimplified(const bool simplified) 	// select simplified, less accurate calculations
		{SIMPLIFIED_=simplified;}
	static bool getsimplified() {return SIMPLIFIED_;} // nowhere used yet

	static void setboreresolution(const double boreresolution) // affect discretisation in cone and bessel sections
		{BORERESOLUTION_=boreresolution;}
	static double getboreresolution() {return BORERESOLUTION_;} // nowhere used yet

	// use directly the text file's radiation impedance
	static void SetImpedanceFile(const char filemame[], int RadHole,int Contrib, vector<Matrix> &Z);



// object properties called when appending a HornElement
//========================================================
	virtual void FirstElement(const int val) {FirstElement_=val;}	//useful in the bore list case for the transmission matrix's calculation (to know when we stop)
	virtual int FirstElement() const {return FirstElement_;}

	void settempC(float tempC) {tempC_ = tempC;}		// tempC_ local temperature in degC, defaults to 21
	float gettempC() const {return tempC_;}
//	virtual void tempC(const float tempC) {tempC_=tempC;}	// details left up to descendents
//	virtual float tempC() const {return tempC_;}

	void setlossF(float lossF) {lossF_ = lossF;}		// lossF_ local loss factor, defaults to 1
	float getlossF() const {return lossF_;}
//	virtual void lossF(const float lossF) {lossF_=lossF;}	// details left up to descendents
//	virtual float lossF() const {return lossF_;}

	void sethumidity(float humidity) {humidity_ = humidity;} //humidity factor, defaults to 80%		
	float gethumidity() const {return humidity_;}
//	virtual void humidity(const float humidity) {humidity_=humidity;}
//	virtual float humidity() const {return humidity_;}	

	void setxc(float xc) {xc_ = xc;}		//CO2 molar fraction, defaults to 382.0 ppm	
	float getxc() const {return xc_;}
//	virtual void xc(const float xc) {xc_=xc;}
//	virtual float xc() const {return xc_;}

	int type() const {return type_;}			// type_ readonly, defined by constructor
	void setName(string name) {name_ = name;}		// name_ identifier of instance
	string getName() const {return name_;}


// generic properties needed by optimiser
//=========================================
//	bool canModify() const {return canModify_;}		// allow modification by optimiser
//	bool canSplit() const {return canSplit_;}		// allow splitting by GA


// START of generic object properties, needed by several HornElements
// only helper functions here; implementations are left to descendents
//======================================================================

	virtual double length() const {return length_;}		// length_ total length of element along axis
	virtual void length(const double length) 		// details left up to descendents
		{length_=length;}

	virtual double rIn() const=0;				// descendents have to return leftside radius
	virtual double rOut() const=0;				// descendents have to return rightside radius
	virtual double gradientIn() const=0;			// descendents have to return leftside gradient
	virtual double gradientOut() const=0;			// descendents have to return rightside gradient
	virtual double findRadius (const double x) const=0; 	// descendents have to return radius at given distance from input - NOTE will extrapolate if given a value outside the bounds of the element
	virtual string getSaveString() const {return "";} 	// descendents have to return string describing this element (to be saved in an elemental file)
	virtual dcomp zt(const double w) const {return 0;} // descendents have to return radiation impedance (complex value, TLA case)
//	virtual Matrix Mzt(const int Ind) const {Matrix T; return T;} // was needed when radiaton was stocked during the constructions of TerminationElement
	virtual string Name() const {return "0";} // descendents have to return current element's name
	virtual int NbHole() const {return 1;} // descendents have to return number of holes at the current branch section
	virtual int RadiationType() const {return 1;} // descendents have to return the radiation's type at the current hole
	virtual string filename() const {return " ";}  


// generic methods needed by optimiser
//=======================================
	virtual int mutate (const float pmut, const float mu, const double previousRout)=0;
	virtual bool split(list<HornElement*> &l, list<HornElement*>::iterator &iter) {return false;}
	virtual void elementCrossover (const HornElement& m, HornElement* s, HornElement* b, const float pcross)=0;
	virtual void elementInitialise(const double previousRout) 
		{std::cout << "baseinit\n";}				// adapt Rin for previous Rout
	virtual HornElement* clone()=0;
	virtual void print() const {std::cout << "Base Horn Element\n";}


// generic object member functions for Mapes-Riordian Tla (spherical waves)
//=============================================================================

	// returns p,u propagation matrix of 0-modes-Tla model at given frequency, to be implemented by descendents
	virtual int TlaAccumulate (const double w, int IndFreq, SimpleMatrix *a, const int Ind, vector<int>&mem, dcomp *zt,list<Matrix> *ImpedanceList,vector<int> &IndHole) 
		{/*(*a) = (SimpleMatrix (dcomp(1.0,0.0), dcomp(0.0,0.0), dcomp(0.0,0.0), dcomp(1.0,0.0)));*/ return 0;} 	

	// returns radiation impedance of 0-modes-Tla model at given frequency, to be implemented by descendents
	// will be used to be propagated from the mouth of the instrument back to the mouthpiece
	virtual dcomp radiationImpedance(const double w) const {return dcomp(1.0, 0);}
	// alternate radiation impedance of 0-modes-Tla model at given frequency from Zorumski (ignoring higher modes)
	virtual dcomp ZorumskiPlaneRadiation(const double w) const {return dcomp(1.0, 0);} 

	// returns radiation impedance complex at given frequency for the current termination element: tla case
	virtual dcomp PlaneWaveRadiationImpedance_TermElt (const double w) const {return dcomp(1.0, 0);}


// generic object member functions (multi modal waves)
//=======================================================

	// propagate impedance matrix at given frequency from right to left end, to be implemented by descendents
	virtual void multimodeInputImpedance (const double w, const bool bends, Matrix *Zout) {return;}

	// returns p,u propagation matrix of nModes multi modal model at given frequency, to be implemented by descendents
	virtual int MultimodeAccumulate (const double w,int IndFreq,const bool bends,Matrix *a, const int Ind, vector<int>&mem, Matrix *zt,list<Matrix> *ImpedanceList,vector<int> &IndHole) {(*a) = Matrix (nModes_,nModes_); (*a).Unit(); return 0;} 

	// returns radiation impedance matrix at given frequency, to be implemented by descendents
	virtual void multimodeRadiationImpedance (const double w, const bool bends, const int IndFreq, Matrix* result) const {std::cout << "MMRI generic\n"; return;}

	// returns radiation impedance matrix at given frequency, perfect reflection condition, to be implemented by descendents
	virtual void multimodeRadiationImpedanceClosedEnd (const double w, const bool bends, Matrix* result) const {std::cout << "MMRI generic\n"; return;}

	// returns radiation impedance matrix at given frequency, infinite baffle condition, to be implemented by descendents
	virtual void multimodeRadiationImpedanceOpenEnd (const double w, const bool bends, Matrix* result) const {std::cout << "MMRI generic\n"; return;}

	 // returns radiation impedance matrix at given frequency, user specified condition, to be implemented by descendents
	virtual void multimodeRadiationImpedanceTerminated (const double w, const bool bends, Matrix* result, const dcomp z_trm) const {std::cout << "MMRI generic\n"; return;}
	
	// returns radiation impedance matrix at given frequency for the current termination element: mm case
	// currently needed only by termination element, so only defined there
	// virtual Matrix multimodeRadiationImpedance_TermElt (const double w, const bool bends, dcomp z_tr) const {static Matrix T; return T;}

	//calculation of levine's radiation impedance
	virtual void radiationImpedanceLevine (const double w, double *InputRadius, dcomp *z_t) const {std::cout << "MMRI generic\n"; return;};

	
// member functions used for branching
//=====================================

	// copy the list in argument to the branch element's list
	virtual void BranchImpedanceList(list <Matrix> *ImpListe) {std::cout << "MMRI generic\n"; return;};

	// copy the useful parameters to calculate the impedance correction
	virtual void PutHoleParam(const double length_Hole, const double radius_Hole) {std::cout << "MMRI generic\n"; return;};
	virtual void PutRoutParam(const double rOut) {std::cout << "MMRI generic\n"; return;};

	// impedances correction
	virtual void ImpLengthCorrection() {std::cout << "MMRI generic\n"; return;};
	virtual void ImpEltsAcoustic() {std::cout << "MMRI generic\n"; return;};

	// impedance at the end of the branch section (when we are looking for the input impedance)
	virtual Matrix GetParallelImpedance(const double w, const int IndFreq) {static Matrix T; return T;}

	// impedance at the begining of the branch section (when we are looking for the radiation impedance)
	virtual Matrix GetImpedance(const double w, const int IndFreq) {static Matrix T; return T;}
//END of generic object properties

// Frequency vector
	static vector<double> Freq;

	#ifdef TRACE
	// redirecting std::cout<<, DBG1 std::cout<< and DBG2 std::cout<< messages to log file, especiall in user DLL
	//	static ofstream clogf;
	//	static void open_trace(string fname) { if (!clogf.is_open()) clogf.open(fname,ios_base::app);}
	//	static void begin_trace() 	     { if ( clogf.is_open()) std::cout.reopen(clogf);}
	//	static void close_trace()            { if ( clogf.is_open()) clogf.close();}
	#endif

// return pointer to properties, needed for ARTinterface
//=====================================
	virtual ARTvariant* getPropertyPointer(const char* name)
	{
		ARTvariant* var = new ARTvariant();

		if (0 == strcmp(name,"length")) {var->typ = C_ART_ndbl;	var->val->nd = &(length_);	return var; }
		if (0 == strcmp(name,"xc"))		{var->typ = C_ART_nflo;	var->val->nf = &(xc_);	return var; }
		if (0 == strcmp(name,"temp"))	{var->typ = C_ART_nflo;	var->val->nf = &(tempC_);		return var; }
		if (0 == strcmp(name,"loss"))	{var->typ = C_ART_nflo;	var->val->nf = &(lossF_);		return var; }
		if (0 == strcmp(name,"humidity")) {var->typ = C_ART_nflo;	var->val->nf = &(humidity_);	return var; }

		//otherwise return the struct with type undefined
		var->typ = C_ART_undef;
		return var;
	}

protected:

// global class properties, same for all object instances
	static int nModes_;	
	static bool LOSSY_;			//Lossy switch
	static bool SIMPLIFIED_;	//Simplified switch

	static double BORERESOLUTION_; //Slicing accuracy
	static double F_MAX_; //The highest frequency
	static double F_MIN_; //The smallest frequency
	static double D_F_; //Step between 2 frequencies during the execution
	static vector<Matrix> MZ; //Radiation impedance matrices vector for each frequency 

// generic object properties
	int type_;			// Geometrical section's type
	string name_;		// Identifier
	float tempC_; 		// Celsius
	float humidity_; 	// Relative Humidity 
	float xc_; 			// Fraction Molaire CO2
	float lossF_; 		// Loss factor (1 = standard modell)
	int FirstElement_;	
	double length_;		// Overall length


// generic properties needed by optimiser
	bool canModify_;
	bool canSplit_;

// local cache to be used by element instances
// currently used by BoreJump and discretising Cone, Bessel and ConeBend sections)
	Matrix MA_;  // Matrix cache of individual element for frequency invariant scattering matrix
	Matrix MB_;  // Matrix cache of individual element for frequency invariant scattering matrix
	double rr_; // corresponding radius ratio


// specific object properties, used by several objects
//=======================================================

// helping descendents to implement their Tla (plane wave) model. Static for performance reasons.

	// returns p,u propagation matrix of 0-modes-Tla model at given frequency, cylindrical (plane wave) case
	static void cylTlaAccumulate (const double w, const double length, const double radius, const float T, const float F, const float H, const float XC, SimpleMatrix *a);
	// returns associated complex radiation impedance at given frequency, cylindrical (plane wave) case
	static dcomp radiationImpedanceLevineCyl (const double w, const double radius, const float T, const float H, const float XC);
	// alternate complex radiation impedance at given frequency, from Zorumski (simply ignoring higher modes)
	static dcomp cylZorumskiPlaneRadiation(const double w, const double radius, const float T, const float H, const float XC);

// helping descendents to implement their Mapes-Riordian Tla (spherical waves) model. Static for performance reasons.

	// returns p,u propagation matrix of 0-modes-Tla model at given frequency, conical (sperical wave case)
	static void coneTlaAccumulate (const double w, const double Xin, const double Xout, const double Rin, const double Rout, const float T, const float F, const float H, const float XC, SimpleMatrix *a);
	// returns associated complex radiation impedance at given frequency, cylindrical (plane wave) case
	static dcomp radiationImpedanceLevineCone (const double w, const double Xin, const double Xout, const double Rin, const double Rout, const float T, const float H, const float XC);

// helping descendents to implement their multi modal model. Static for performance reasons.

	// return lossy complex modal wave numbers
	static void kn (const double w, const double radius, const bool lossy, const bool bends, const float T, const float L, const float H, const float XC, dcomp* kn);
	
	// for radiation impedance calculation
	static double Dn(const double tau, const int i, const double kR);
	static double Dmn(const double tau, const int m, const int n, const double kR);

	// propagate impedance matrix at given frequency from right to left end, bore jump case
	static void jumpMultimodeInputImpedance (const double w, double *rr, Matrix *A, Matrix *B, Matrix *Zout, const double rIn, const double rOut, const bool bends, const float T);
	// propagate impedance matrix at given frequency from right to left end, cylindrical case
	static void cylMultimodeInputImpedance (const double w, Matrix *Zout, const double length, const double radius, const bool lossy, const bool simplified, const bool bends, const float T, const float L, const float H, const float XC);
	// propagate impedance matrix at given frequency from right to left end, cylindrical bend case
	static void cylBendMultimodeInputImpedance (const double w, Matrix *Zout, const double length, const double radius, const double bendRadius, const bool lossy, const float T, const float L, const float Humidity, const float XC);
	// returns p,u propagation matrix of nModes multi modal model at given frequency, cylindrical case
	static int cylMultimodeAccumulate (const double w, const double length, const double radius, const bool lossy,const bool bends, const float T, const float F,const float H, const float XC, Matrix *a);
	// returns p,u propagation matrix of nModes multi modal model at given frequency, borejump case
	static void jumpMultimodeTransmMatrix (double zeta,const bool bends,Matrix *a);

	//wasilakis: to be corrected
	static void cylBendMultimodeAccumulate (const double w, const double length, const double radius, const double bendRadius, const bool lossy, const float T, const float Humidity, const float XC, Matrix *a);

	// returns radiation impedance matrix at given frequency, infinite baffle condition, multi modal
	static void radiationImpedanceZorumski(const double w, Matrix* result, const double radius, const bool bends, const float T, const float H, const float XC);
	// returns radiation impedance matrix at given frequency, infinite cylindrical pipe, multi modal
	static void radiationImpedanceInfiniteCylPipe(const double w, Matrix* result, const double radius, const float T, const float H, const float XC);
	// returns radiation impedance matrix at given frequency, as specified by user, multi modal
	static void radiationImpedanceTerminated(const double w, Matrix* result, const double radius, const bool bends, const float T, const float H, const float XC, const dcomp z_trm);



//	static void bendRadiationImpedanceInfiniteCylPipe0(const double w, Matrix* result, const double radius, const float T, const float H, const float XC);

//WK removed static member functions - introduced global functions instead...
	//Maths sub-functions for TLA Input Impedance calculations
//	static double r_zero(const double Si) { return (RHO*C/Si); }
//	static dcomp Z_c(const double w, const double Si, const double Sm)  {	double rvinv=1/r_v(w, Sm); return dcomp(r_zero(Si)*(1+0.369*rvinv), -r_zero(Si)*rvinv*(0.369+rvinv*(1.149+0.303*rvinv)));}
//	static double r_v(const double w, const double Sm)  {return (sqrt(RHO*w*Sm/ETA/M_PI));}
//	static dcomp Gamma(const double w, const double Sm) {double rvinv=1/r_v(w, Sm); return (dcomp(w/C*rvinv*(1.045+rvinv*(1.080+0.750*rvinv)), w/C*(1+1.045*rvinv)));}
	
// global static class storage, same for all object instances
// as there is only ever one of each matrix in existence at a time, thus it saves them being declared/deleted many times in the main loop
	static Matrix I_; //Identity matrix
	static Matrix D_, Zc_, ZcInv_, F_;
	static dcomp* ki_;
	static double *D1_, *D2_;
	static bool haveInitMatrices;
	static void initMatrices() {
		if (!haveInitMatrices) {
DBG1 std::cout << "First matrix initialisation..." << "\n";
			I_=Matrix(nModes_, nModes_); D_=Matrix(nModes_, nModes_); Zc_=Matrix(nModes_, nModes_); ZcInv_=Matrix(nModes_, nModes_); F_=Matrix(nModes_, nModes_);
			ki_=new dcomp[nModes_]; D1_=new double[nModes_]; D2_=new double[nModes_];
			for(int i=0; i<nModes_; i++) I_(i,i)=dcomp(1,0);
			F_(0,0)=1.0; //Always true
			haveInitMatrices = true;
DBG1 std::cout << "First matrix set to " << nModes_ << "\n";
		} else {
DBG1 std::cout << "Matrix resize to " << nModes_ << "\n";
			I_.Null(nModes_, nModes_); D_.Null(nModes_, nModes_); Zc_.Null(nModes_, nModes_); ZcInv_.Null(nModes_, nModes_); F_.Null(nModes_, nModes_);
			delete ki_; delete D1_; delete D2_;
			ki_=new dcomp[nModes_]; D1_=new double[nModes_]; D2_=new double[nModes_];
			for(int i=0; i<nModes_; i++) I_(i,i)=dcomp(1,0);
			F_(0,0)=1.0; //Always true
DBG1 std::cout << "Matrix resize done" << "\n";
		}
	}
	static int mIndex (const int alpha, const bool bends) {
		if (bends) return mbend_[alpha];
		else return 0;
	}
	static int nIndex (const int beta, const bool bends) {
		if (bends) return nbend_[beta]; 
		else return beta;
	}
};

class BoreJumpSection : public HornElement {
public:
	BoreJumpSection(const double rIn=0.0, const double rOut=0.0, const float tempC=21.0, 
		const float lossF=1.0, const float humidity=80, const float xc=382, const string name = " ", 
		Matrix MA=NULL, Matrix MB=NULL, double rr=0.0, double length=0.0, bool canModify=false, 
		bool canSplit=false, double jumpSize=0.0) : rIn_(rIn), rOut_(rOut), jumpSize_(rOut-rIn), 
		HornElement(tempC,lossF,humidity/100,xc/1e06,name,BOREJUMPSECTION_TYPE,MA,MB,rr,length, canSplit, 
		canModify)  {}
	~BoreJumpSection();
	BoreJumpSection(HornElement *c);

	virtual ARTvariant* getPropertyPointer(const char* name)
	{
		ARTvariant* var = new ARTvariant();

		if (0 == strcmp(name,"length")) {var->typ = C_ART_ndbl;	var->val->nd = &(length_);	return var; }
		if (0 == strcmp(name,"xc"))		{var->typ = C_ART_nflo;	var->val->nf = &(xc_);	return var; }
		if (0 == strcmp(name,"temp"))	{var->typ = C_ART_nflo;	var->val->nf = &(tempC_);		return var; }
		if (0 == strcmp(name,"loss"))	{var->typ = C_ART_nflo;	var->val->nf = &(lossF_);		return var; }
		if (0 == strcmp(name,"humidity")) {var->typ = C_ART_nflo;	var->val->nf = &(humidity_);	return var; }
		if (0 == strcmp(name,"r1"))		{var->typ = C_ART_ndbl;	var->val->nd = &(rIn_);	return var; }
		if (0 == strcmp(name,"r2"))		{var->typ = C_ART_ndbl;	var->val->nd = &(rOut_);	return var; }

		//otherwise return the struct with type undefined
		var->typ = C_ART_undef;
		return var;
	}

// object properties
	double rIn() const {return rIn_;}
	void rIn(const double rIn) {rIn_=rIn;}
	double rOut() const {return rOut_;}
	void rOut(const double rOut) {rOut_=rOut;}
	double gradientIn() const {return -999.9;}
	double gradientOut() const {return -999.9;}
	double findRadius (const double x) const {if (x<0) return rIn_; else return rOut_;}
	string getSaveString() const;

	
// methods needed by optimiser
	int mutate (const float pmut, const float mu, const double previousRout) {return 0;}
	bool split(list<HornElement*> &l, list<HornElement*>::iterator &iter) {return false;}
	void elementCrossover (const HornElement& m, HornElement* s, HornElement* b, const float pcross);
	void elementInitialise(const double previousRout);
	HornElement* clone() {return new BoreJumpSection(this);}
	void print() const;

// object member functions for Mapes-Riordian Tla (spherical waves)
	// not needed - BoreJump has no effect in Tla!

// object member functions (multi modal waves)
	void multimodeInputImpedance (const double w, const bool bends, Matrix *Zout) ;
	void multimodeRadiationImpedance(const double w, const bool bends, const int IndFreq, Matrix* result) const;
	int MultimodeAccumulate (const double w, int IndFreq, const bool bends, Matrix *a, const int Ind, vector<int>&mem, Matrix *zt,list<Matrix> *ImpedanceList,vector<int> &IndHole);

protected:
	double rIn_, rOut_, jumpSize_; //tempC_, lossF_, humidity_, xc_;
	//const string name_;
	//Allows the GA to define this by jumpSize and leave rIn to be given by previous rOut.

};

class CylinderSection : public HornElement {
public:
	CylinderSection(const bool canModify=false, const bool canSplit=false, const double length=100.0, 
		const double radius=1.0, const float tempC=21.0, const float lossF=1.0, const float humidity=80, 
		const float xc=382, const string name = " ", Matrix MA=NULL, Matrix MB=NULL, double rr=0.0) : 
		radius_(radius), HornElement(tempC,lossF,humidity/100,xc/1e06,name,CYLINDERSECTION_TYPE,MA,MB,
		rr,length, canSplit, canModify)  {}
	CylinderSection(const bool canModify, const bool canSplit, const double minLength, const double maxLength, 
		const double minradius, const double maxradius); //max & min values
	CylinderSection(HornElement* c);
	~CylinderSection();

	virtual ARTvariant* getPropertyPointer(const char* name)
	{
		ARTvariant* var = new ARTvariant();

		if (0 == strcmp(name,"length")) {var->typ = C_ART_ndbl;	var->val->nd = &(length_);	return var; }
		if (0 == strcmp(name,"xc"))		{var->typ = C_ART_nflo;	var->val->nf = &(xc_);	return var; }
		if (0 == strcmp(name,"temp"))	{var->typ = C_ART_nflo;	var->val->nf = &(tempC_);		return var; }
		if (0 == strcmp(name,"loss"))	{var->typ = C_ART_nflo;	var->val->nf = &(lossF_);		return var; }
		if (0 == strcmp(name,"humidity")) {var->typ = C_ART_nflo;	var->val->nf = &(humidity_);	return var; }
		if (0 == strcmp(name,"r"))		{var->typ = C_ART_ndbl;	var->val->nd = &(radius_);	return var; }

		//otherwise return the struct with type undefined
		var->typ = C_ART_undef;
		return var;
	}

// object properties
	void radius(const double radius) {radius_=radius;}
	double rIn() const {return radius_;}
	double rOut() const {return radius_;}
	double gradientIn() const {return 0.0;}
	double gradientOut() const {return 0.0;}
	double findRadius (const double x) const {return radius_;}
	string getSaveString() const;

// methods needed by optimiser
	int mutate (const float pmut, const float mu, const double previousRout);
	bool split(list<HornElement*> &l, list<HornElement*>::iterator &iter);
	void elementCrossover (const HornElement& m, HornElement* s, HornElement* b, const float pcross);
	void elementInitialise(const double previousRout);
	HornElement* clone() {return new CylinderSection(this);}
	void print() const ;

// object member functions for Mapes-Riordian Tla (spherical waves)
	int TlaAccumulate (const double w, int IndFreq, SimpleMatrix *a, const int Ind, vector<int>&mem, dcomp *zt,list<Matrix> *ImpedanceList,vector<int> &IndHole);
	dcomp radiationImpedance (const double w) const;
	  dcomp ZorumskiPlaneRadiation(const double w) const;

// object member functions (multi modal waves)
	void multimodeInputImpedance (const double w, const bool bends, Matrix *Zout) ;
	void multimodeRadiationImpedance(const double w, const bool bends, const int IndFreq, Matrix* result) const;
	void multimodeRadiationImpedanceClosedEnd(const double w, const bool bends, Matrix* result) const;
	void multimodeRadiationImpedanceOpenEnd(const double w, const bool bends, Matrix* result) const;
	void multimodeRadiationImpedanceTerminated(const double w, const bool bends, Matrix* result, const dcomp z_trm) const;
	int MultimodeAccumulate (const double w, int IndFreq, const bool bends, Matrix *a, const int Ind, vector<int>&mem, Matrix *zt,list<Matrix> *ImpedanceList,vector<int> &IndHole);

	// complex wavenumber for higher modes (Bruneau et al)
	// Returns an array, size nModes, containing kn for n=1..nModes	
//	dcomp Zorumskikn (const double w, const double radius) const; // currently not used

protected:
	double radius_;
	double minLength_, maxLength_, minRadius_, maxRadius_;
};

class ConeSection : public HornElement {
public:

	ConeSection(const double length, const double rDiff);
	ConeSection(const bool canModify=false, const bool canSplit=false, const double length=100.0, 
		const double rIn=1.0, const double rOut=2.0, const float tempC=21.0, const float lossF=1.0, 
		const float humidity=80, const float xc=382, const string name = " ", Matrix MA=NULL, 
		Matrix MB=NULL, double rr=0.0, const double rDiff=-0.0) : rIn_(rIn), rOut_(rOut), rDiff_(rDiff), 
		HornElement(tempC,lossF,humidity/100,xc/1e06,name,CONESECTION_TYPE,MA,MB,rr,length, 
		canSplit, canModify)  {}
	ConeSection(const bool canModify, const bool canSplit, const double minLength, const double maxLength, 
		const double minRin, const double maxRin, const double minRout, const double maxRout); //max&min values
	ConeSection(HornElement *c);
	~ConeSection();

	virtual ARTvariant* getPropertyPointer(const char* name)
	{
		ARTvariant* var = new ARTvariant();

		if (0 == strcmp(name,"length")) {var->typ = C_ART_ndbl;	var->val->nd = &(length_);	return var; }
		if (0 == strcmp(name,"xc"))		{var->typ = C_ART_nflo;	var->val->nf = &(xc_);	return var; }
		if (0 == strcmp(name,"temp"))	{var->typ = C_ART_nflo;	var->val->nf = &(tempC_);		return var; }
		if (0 == strcmp(name,"loss"))	{var->typ = C_ART_nflo;	var->val->nf = &(lossF_);		return var; }
		if (0 == strcmp(name,"humidity")) {var->typ = C_ART_nflo;	var->val->nf = &(humidity_);	return var; }
		if (0 == strcmp(name,"r1"))		{var->typ = C_ART_ndbl;	var->val->nd = &(rIn_);	return var; }
		if (0 == strcmp(name,"r2"))		{var->typ = C_ART_ndbl;	var->val->nd = &(rOut_);	return var; }

		//otherwise return the struct with type undefined
		var->typ = C_ART_undef;
		return var;
	}

// object properties
	double rIn() const {return rIn_;}
	void rIn(const double rIn) {rIn_=rIn;}
	double rOut() const {return rOut_;}
	void rOut(const double rOut) {rOut_=rOut;}
	double gradientIn() const {return (rOut_ - rIn_) / length_;}
	double gradientOut() const {return (rOut_ - rIn_) / length_;}
	double findRadius (const double x) const;
	double findPosition (const double r) const;
	string getSaveString() const;

// methods needed by optimiser
	int mutate (const float pmut, const float mu, const double previousRout);
	bool split(list<HornElement*> &l, list<HornElement*>::iterator &iter);
	void elementCrossover (const HornElement& m, HornElement* s, HornElement* b, const float pcross);
	void elementInitialise(const double previousRout);
	HornElement* clone() {return new ConeSection(this);}
	void print() const ;

// object member functions for Mapes-Riordian Tla (spherical waves)
	int TlaAccumulate (const double w,int IndFreq, SimpleMatrix *a, const int Ind, vector<int>&mem, dcomp *zt,list<Matrix> *ImpedanceList,vector<int> &IndHole);
	dcomp radiationImpedance (const double w) const;
	dcomp ZorumskiPlaneRadiation(const double w) const; 

// object member functions (multi modal waves)
	void multimodeInputImpedance (const double w, const bool bends, Matrix *Zout) ;
	void multimodeInputImpedance (const double w, const double Xin, const double Xout, const double Rin, const double Rout, const bool bends, Matrix *Zout) ;
	void multimodeRadiationImpedance(const double w, const bool bends, const int IndFreq, Matrix* result) const;
	void multimodeRadiationImpedanceClosedEnd(const double w, const bool bends, Matrix* result) const;
	void multimodeRadiationImpedanceOpenEnd(const double w, const bool bends, Matrix* result) const;
	void multimodeRadiationImpedanceTerminated(const double w, const bool bends, Matrix* result, const dcomp z_trm) const;
	int MultimodeAccumulate (const double w, int IndFreq, const bool bends, Matrix *a, const int Ind, vector<int>&mem, Matrix *zt,list<Matrix> *ImpedanceList,vector<int> &IndHole);

protected:
	double rIn_, rOut_, rDiff_;
	double minLength_, maxLength_, minRin_, maxRin_, minRout_, maxRout_;
};

class BesselSection : public HornElement {
public:

	BesselSection(const bool canModify=false, const bool canSplit=false, const double length=100.0, 
		const double rIn=1.0, const double rOut=2.0, const double flare=2, const float tempC=21.0, const float lossF=1.0, 
		const float humidity=80, const float xc=382, const string name = " ", Matrix MA=NULL, 
		Matrix MB=NULL, double rr=0.0) : rIn_(rIn), rOut_(rOut), flare_(flare), 
		HornElement(tempC,lossF,humidity/100,xc/1e06,name,BESSELSECTION_TYPE,MA,MB,rr,length, 
		canSplit, canModify)  {}
	
	BesselSection(const bool canModify, const bool canSplit, const double minLength, const double maxLength, const double minRin, const double maxRin, 
		const double minRout, const double maxRout, const double minFlare, const double maxFlare); //max&min values
	BesselSection(HornElement *c);
	~BesselSection();

	virtual ARTvariant* getPropertyPointer(const char* name)
	{
		ARTvariant* var = new ARTvariant();

		if (0 == strcmp(name,"length")) {var->typ = C_ART_ndbl;	var->val->nd = &(length_);	return var; }
		if (0 == strcmp(name,"xc"))		{var->typ = C_ART_nflo;	var->val->nf = &(xc_);	return var; }
		if (0 == strcmp(name,"temp"))	{var->typ = C_ART_nflo;	var->val->nf = &(tempC_);		return var; }
		if (0 == strcmp(name,"loss"))	{var->typ = C_ART_nflo;	var->val->nf = &(lossF_);		return var; }
		if (0 == strcmp(name,"humidity")) {var->typ = C_ART_nflo;	var->val->nf = &(humidity_);	return var; }
		if (0 == strcmp(name,"r1"))		{var->typ = C_ART_ndbl;	var->val->nd = &(rIn_);	return var; }
		if (0 == strcmp(name,"r2"))		{var->typ = C_ART_ndbl;	var->val->nd = &(rOut_);	return var; }
		if (0 == strcmp(name,"flare"))		{var->typ = C_ART_ndbl;	var->val->nd = &(flare_);	return var; }

		//otherwise return the struct with type undefined
		std::cout << "undef!\n";
		var->typ = C_ART_undef;
		return var;
	}

// object properties
	double rIn() const {return rIn_;}
	void rIn(const double rIn) {rIn_=rIn;}
	double rOut() const {return rOut_;}
	void rOut(const double rOut) {rOut_=rOut;}
	double flare() const {return flare_;}
	void flare(const double flare) {flare_=flare;}
	double gradientIn() const;
	double gradientOut() const;
	double findRadius(const double x) const;
	double findPosition (const double r) const;
	double findExtremeRadius (const double x, const bool isMax) const;
	string getSaveString() const;
	bool nudgeFlare (const double target) {	const double epsilon = -1/flare_; const double b = pow( length_/(pow(rIn_, epsilon)-pow(rOut_, epsilon)) , flare_);	const double x0 = pow(rOut_/b, epsilon);
		std::cout << gradientIn() << ", " << flare_; flare_ = target /b / pow(x0+length_, -1-flare_); std::cout << "& " << gradientIn() << "*  " << target << ", " << flare_ << "\n";
		if (fabs(gradientIn()-target)>0.01) nudgeFlare(target);
			return (flare_>minFlare_ && flare_<maxFlare_);}

// methods needed by optimiser
	int mutate (const float pmut, const float mu, const double previousRout);
	void elementCrossover (const HornElement& m, HornElement* s, HornElement* b, const float pcross);
	bool split(list<HornElement*> &l, list<HornElement*>::iterator &iter);
	void elementInitialise(const double previousRout);
	HornElement* clone() {return new BesselSection(this);}
	void print() const ;

// object member functions for Mapes-Riordian Tla (spherical waves)
	int TlaAccumulate (const double w,int IndFreq, SimpleMatrix *a, const int Ind, vector<int>&mem, dcomp *zt,list<Matrix> *ImpedanceList,vector<int> &IndHole);
	dcomp radiationImpedance (const double w) const;
	dcomp ZorumskiPlaneRadiation(const double w) const; 

// object member functions (multi modal waves)
	void multimodeInputImpedance (const double w, const bool bends, Matrix *Zout) ;
	void multimodeRadiationImpedance(const double w, const bool bends, const int IndFreq, Matrix* result) const;
	void multimodeRadiationImpedanceClosedEnd(const double w, const bool bends, Matrix* result) const;
	void multimodeRadiationImpedanceOpenEnd(const double w, const bool bends, Matrix* result) const;
	void multimodeRadiationImpedanceTerminated(const double w, const bool bends, Matrix* result, const dcomp z_trm) const;
	int MultimodeAccumulate (const double w,int IndFreq, const bool bends, Matrix *a, const int Ind, vector<int>&mem, Matrix *zt,list<Matrix> *ImpedanceList,vector<int> &IndHole);

protected:
	double rIn_, rOut_, flare_;
	double minLength_, maxLength_, minRin_, maxRin_, minRout_, maxRout_, maxFlare_, minFlare_;
};

class CylinderBendSection : public HornElement {
public:

	CylinderBendSection(const bool canModify=false, const bool canSplit=false, const double length=100.0, 
		const double radius=1.0, const double bendRadius=50.0, const float tempC=21.0, const float lossF=1.0, 
		const float humidity=80, const float xc=382, const string name = " ", Matrix MA=NULL, 
		Matrix MB=NULL, double rr=0.0, bool hasBendMatrices = false, 
		bool dimBendMatrices = false) : radius_(radius), bendRadius_(bendRadius),
		hasBendMatrices_(hasBendMatrices), dimBendMatrices_(dimBendMatrices),
		HornElement(tempC,lossF,humidity/100,xc/1e06,name,CYLINDERBENDSECTION_TYPE,MA,MB,rr,length, 
		canSplit, canModify)  {}
	CylinderBendSection(const bool canModify, const bool canSplit, const double minLength, const double maxLength, 
		const double minradius, const double maxradius); //DEPRECATED!!!!
	CylinderBendSection(HornElement* c);
	~CylinderBendSection();

	virtual ARTvariant* getPropertyPointer(const char* name)
	{
		ARTvariant* var = new ARTvariant();

		if (0 == strcmp(name,"length")) {var->typ = C_ART_ndbl;	var->val->nd = &(length_);	return var; }
		if (0 == strcmp(name,"xc"))		{var->typ = C_ART_nflo;	var->val->nf = &(xc_);	return var; }
		if (0 == strcmp(name,"temp"))	{var->typ = C_ART_nflo;	var->val->nf = &(tempC_);		return var; }
		if (0 == strcmp(name,"loss"))	{var->typ = C_ART_nflo;	var->val->nf = &(lossF_);		return var; }
		if (0 == strcmp(name,"humidity")) {var->typ = C_ART_nflo;	var->val->nf = &(humidity_);	return var; }
		if (0 == strcmp(name,"r"))		{var->len = 1; var->typ = C_ART_ndbl;	var->val->nd = &(radius_);	return var; }
		if (0 == strcmp(name,"bendradius"))	{var->len = 1; var->typ = C_ART_ndbl;	var->val->nd = &(bendRadius_);		return var; }

		//otherwise return the struct with type undefined
		var->typ = C_ART_undef;
		return var;
	}

// object properties
	virtual void setnModes(const int nModes);
	double length () const {return length_;}
	void length(const double length) {length_=length; hasBendMatrices_=false;}
	double bendRadius () const {return bendRadius_;}
	void bendRadius(const double bendRadius) {bendRadius_=bendRadius; hasBendMatrices_=false;}
	double rIn() const {return radius_;}
	double rOut() const {return radius_;}
	void radius(const double radius) {radius_=radius; hasBendMatrices_=false;}
	double gradientIn() const {return 0.0;}
	double gradientOut() const {return 0.0;}
	double findRadius (const double x) const {return radius_;}
	string getSaveString() const;

// methods needed by optimiser
	int mutate (const float pmut, const float mu, const double previousRout); //DEPRECATED
	void elementCrossover (const HornElement& m, HornElement* s, HornElement* b, const float pcross); //DEPRECATED
	bool split(list<HornElement*> &l, list<HornElement*>::iterator &iter); //DEPRECATED
	void elementInitialise(const double previousRout); //DEPRECATED
	HornElement* clone() {return new CylinderBendSection(this);}
	void print() const ;

// object member functions for Mapes-Riordian Tla (spherical waves)
	int TlaAccumulate (const double w, int IndFreq, SimpleMatrix *a, const int Ind, vector<int>&mem, dcomp *zt,list<Matrix> *ImpedanceList,vector<int> &IndHole);
	dcomp radiationImpedance (const double w) const;
	dcomp ZorumskiPlaneRadiation(const double w) const;

// object member functions (multi modal waves)
	void multimodeInputImpedance (const double w, const bool bends, Matrix *Zout) ;
	void multimodeRadiationImpedance(const double w, const bool bends, const int IndFreq, Matrix* result) const;
	void multimodeRadiationImpedanceClosedEnd(const double w, const bool bends, Matrix* result) const;
	void multimodeRadiationImpedanceOpenEnd(const double w, const bool bends, Matrix* result) const;
	void multimodeRadiationImpedanceTerminated(const double w, const bool bends, Matrix* result, const dcomp z_trm) const;
	int MultimodeAccumulate (const double w, int IndFreq, const bool bends, Matrix *a, const int Ind, vector<int>&mem, Matrix *zt,list<Matrix> *ImpedanceList,vector<int> &IndHole);
	void bendRadiationImpedanceInfiniteCylPipe(const double w, Matrix* result, const double radius) const;
	void initBendMatrices();

protected:
	double radius_, bendRadius_;
	double minLength_, maxLength_, minRadius_, maxRadius_;
	Matrix A_,B_,C_; 		//frequency-independent matrices
	LeG_quadrature leg; // function for Gauss-Legendre quadrature

	bool hasBendMatrices_;
	bool dimBendMatrices_;
	static Matrix K_, BCKB_, X_, N_, Dbend_, H_, E_, nu_, HXN_, F_;	//frequency-dependent, but only need one at a time so static
	void dimensionBendMatrices() {
		dimBendMatrices_=true;
		K_ = Matrix(nModes_, nModes_);
		BCKB_ = Matrix(nModes_, nModes_);
		H_ = Matrix(nModes_, nModes_);
		Dbend_ = Matrix(nModes_, nModes_);
		X_ = Matrix(nModes_, nModes_);
		N_ = Matrix(nModes_, nModes_);
		nu_ = Matrix(nModes_, 1);
		HXN_ = Matrix(nModes_, nModes_);
		F_ = Matrix(nModes_, nModes_);
	}
};


class ConeBendSection : public HornElement {
public:

	ConeBendSection(const bool canModify=false, const bool canSplit=false, const double length=100.0, 
		const double rIn=1.0, const double rOut = 2.0, const double bendRadius=50.0, const float tempC=21.0, const float lossF=1.0, 
		const float humidity=80, const float xc=382, const string name = " ", Matrix MA=NULL, 
		Matrix MB=NULL, double rr=0.0, bool dimBendMatrices = false) : rIn_(rIn), rOut_(rOut), 
		bendRadius_(bendRadius),dimBendMatrices_(dimBendMatrices),
		HornElement(tempC,lossF,humidity/100,xc/1e06,name,CONEBENDSECTION_TYPE,MA,MB,rr,length, 
		canSplit, canModify)  {}
	ConeBendSection(const bool canModify, const bool canSplit, const double minLength, const double maxLength, 
		const double minRin, const double maxRin, const double minRout, const double maxRout); //DEPRECIATED
	ConeBendSection(HornElement *c);
	~ConeBendSection();

	virtual ARTvariant* getPropertyPointer(const char* name)
	{
		ARTvariant* var = new ARTvariant();

		if (0 == strcmp(name,"length")) {var->typ = C_ART_ndbl;	var->val->nd = &(length_);	return var; }
		if (0 == strcmp(name,"xc"))		{var->typ = C_ART_nflo;	var->val->nf = &(xc_);	return var; }
		if (0 == strcmp(name,"temp"))	{var->typ = C_ART_nflo;	var->val->nf = &(tempC_);		return var; }
		if (0 == strcmp(name,"loss"))	{var->typ = C_ART_nflo;	var->val->nf = &(lossF_);		return var; }
		if (0 == strcmp(name,"humidity")) {var->typ = C_ART_nflo;	var->val->nf = &(humidity_);	return var; }
		if (0 == strcmp(name,"r1"))		{var->len = 1; var->typ = C_ART_ndbl;	var->val->nd = &(rIn_);	return var; }
		if (0 == strcmp(name,"r2"))		{var->len = 1; var->typ = C_ART_ndbl;	var->val->nd = &(rOut_);	return var; }
		if (0 == strcmp(name,"bendradius")) {var->len = 1; var->typ = C_ART_ndbl;	var->val->nd = &(bendRadius_);	return var; }

		//otherwise return the struct with type undefined
		var->typ = C_ART_undef;
		return var;
	}

// object properties
	virtual void setnModes(const int nModes);
	double rIn() const {return rIn_;}
	double rOut() const {return rOut_;}
	double bendRadius () const {return bendRadius_;}
	void rIn(const double rIn) {rIn_=rIn;}
	void rOut(const double rOut) {rOut_=rOut;}
	void bendRadius(const double bendRadius) {bendRadius_ = bendRadius;}
	double gradientIn() const {return (rOut_ - rIn_) / length_;}
	double gradientOut() const {return (rOut_ - rIn_) / length_;}
	double findRadius (const double x) const;
	string getSaveString() const;

// methods needed by optimiser
	int mutate (const float pmut, const float mu, const double previousRout);
	void elementCrossover (const HornElement& m, HornElement* s, HornElement* b, const float pcross);
	bool split(list<HornElement*> &l, list<HornElement*>::iterator &iter);
	void elementInitialise(const double previousRout);
	HornElement* clone() {return new ConeBendSection(this);}
	void print() const ;

// object member functions for Mapes-Riordian Tla (spherical waves)
	int TlaAccumulate (const double w, int IndFreq, SimpleMatrix *a, const int Ind, vector<int>&mem, dcomp *zt,list<Matrix> *ImpedanceList,vector<int> &IndHole);
	dcomp radiationImpedance (const double w) const;
	dcomp ZorumskiPlaneRadiation(const double w) const; 

// object member functions (multi modal waves)
	void multimodeInputImpedance (const double w, const bool bends, Matrix *Zout) ;
	//wasilakis
	void multimodeInputImpedance (const double w, const double Xin, const double Xout, const double Rin, const double Rout, const bool bends, Matrix *Zout) ;
	void multimodeRadiationImpedance(const double w, const bool bends, const int IndFreq, Matrix* result) const;
	void multimodeRadiationImpedanceClosedEnd(const double w, const bool bends, Matrix* result) const;
	void multimodeRadiationImpedanceOpenEnd(const double w, const bool bends, Matrix* result) const;
	void multimodeRadiationImpedanceTerminated(const double w, const bool bends, Matrix* result, const dcomp z_trm) const;
	void bendRadiationImpedanceInfiniteCylPipe(const double w, Matrix* result, const double radius) const;


///////////////////////////////////
//WASILAKIS
	int MultimodeAccumulate (const double w, int IndFreq, const bool bends, Matrix *a, const int Ind, vector<int>&mem, Matrix *zt,list<Matrix> *ImpedanceList,vector<int> &IndHole);

	double findPosition (const double r) const;
///////////////////////////////////


protected:
	double rIn_, rOut_, bendRadius_;
	double minLength_, maxLength_, minRin_, maxRin_, minRout_, maxRout_;
	bool dimBendMatrices_;
	static Matrix A_, B_, C_, K_, Fbend_, G_, Z_, Zprime_, Zstore_, Ztotal_; //frequency-dependent, but only need one at a time so static
	void dimensionBendMatrices();
	static double Zc_;


	//void thetaDependence (const double theta, const double k); //currenlty not used
	//void Zprime (const double theta, const double k);

	
	LeG_quadrature leg; // function for Gauss-Legendre quadrature

};


//Element which contains the radiation impedance
class TerminationElement : public HornElement {
public:
	TerminationElement(const bool canModify=false, const bool canSplit=false, int test=0,int Contrib=1, const double RadiationRadius=1.0, const double PrevInputRadius  = 1.0, const int PrevType=1, 
		const int radiation_type = 0,const char filename[100] = " ",const float tempC=21.0, const float lossF=1.0, 
		const float humidity=80, const float xc=382, const string name = " ", Matrix MA=NULL, 
		Matrix MB=NULL, double rr=0.0) : RadiationRadius_(RadiationRadius), Test_(test), Contrib_(Contrib),
		PrevInputRadius_(PrevInputRadius), PrevType_(PrevType), RADIATION_TYPE_(radiation_type),
		FILENAME_(filename), NAME_(name),
		HornElement(tempC,lossF,humidity/100,xc/1e06,name,TERMINATIONELEMENT_TYPE,MA,MB,rr,0, 
		canSplit, canModify) {} //{if(radiation_type<10) {GetRadiationImpedance(test, Contrib);}}
	TerminationElement(HornElement* c);
	~TerminationElement();

// object properties
	void radius(const double radius) {RadiationRadius_=radius;}
	double rIn() const {return RadiationRadius_;}
	double rOut() const {return RadiationRadius_;}
	double gradientIn() const {return 0.0;}
	double gradientOut() const {return 0.0;}
	double findRadius (const double x) const {return RadiationRadius_;}
	string Name() const {return NAME_;}

	dcomp zt(const double w) const {return zt_[(w-(2*M_PI*F_MIN_))/(2*M_PI*D_F_)];}
	//Matrix Mzt(const int Ind) const {return Mzt_[Ind];}
	int RadiationType() const {return RADIATION_TYPE_;}
	string filename() const {return FILENAME_;} 

// object member functions for Mapes-Riordian Tla (spherical waves)
	Matrix multimodeRadiationImpedance_TermElt(const double w, const bool bends) const;
	dcomp PlaneWaveRadiationImpedance_TermElt(const double w) const;
	dcomp radiationImpedanceLevine(const double w) const;


// object member functions (multi modal waves)
	int mutate (const float pmut, const float mu, const double previousRout);
	void elementCrossover (const HornElement& m, HornElement* s, HornElement* b, const float pcross);
	HornElement* clone() {return new TerminationElement(this);}


// object member functions for Radiation Impedance calculation
	void multimodeRadiationImpedance(const double w, const bool bends, const int IndFreq, Matrix* result) const;
	void multimodeRadiationImpedanceClosedEnd(const double w, const bool bends, Matrix* result) const;
	void multimodeRadiationImpedanceOpenEnd(const double w, const bool bends, Matrix* result) const;


	int RADIATION_TYPE_;
	string FILENAME_;
	string NAME_;
	vector<dcomp> zt_;
	//vector <Matrix> Mzt_;


protected:
	void GetRadiationImpedance(int test, int contrib);
	double RadiationRadius_,PrevInputRadius_;
	int PrevType_, Test_, Contrib_;
	double minLength_, maxLength_, minRadius_, maxRadius_;
};

//Connexion between a hole and the main section
class Branch : public HornElement{
public:
	Branch(const bool canModify=false, const bool canSplit=false, 
		const int NbHole = 1, const double rIn = 1.0, const double rOut = 1.0, const int SimpleModel = 1, 
		const float tempC=21.0, const float lossF=1.0, const float humidity=80, const float xc=382, 
		const string name = " ", Matrix MA=NULL, Matrix MB=NULL, double rr=0.0) : 
		NbHole_(NbHole), rIn_(rIn), rOut_(rOut), SimpleModel_(SimpleModel),
		HornElement(tempC,lossF,humidity/100,xc/1e06,name,BRANCH_TYPE,MA,MB,rr,0, 
		canSplit, canModify)  {}
	Branch(HornElement* c);
	~Branch();

// object properties
	void radius(const double radius) {radius_ = radius_;}
	double rIn() const {return rIn_;}
	double rOut() const {return rOut_;}
	double gradientIn() const {return 0.0;}
	double gradientOut() const {return 0.0;}
	double findRadius (const double x) const {return radius_;}
	int NbHole() const {return NbHole_;}


	// object member functions (multi modal waves)
	int mutate (const float pmut, const float mu, const double previousRout);
	void elementCrossover (const HornElement& m, HornElement* s, HornElement* b, const float pcross);
	HornElement* clone() {return new Branch(this);} //Sadjad: was "return new TerminationElement(this);", I believe that was a mistake; I change it to "...Branch(..."
	list <Matrix> *BranchImpedanceList_;
	void BranchImpedanceList(list<Matrix> *ImpListe);
	Matrix GetParallelImpedance(const double w, const int IndFreq);
	Matrix GetImpedance(const double w, const int IndFreq);
	void ImpLengthCorrection();
	void ImpEltsAcoustic();
	void PutHoleParam(const double length_Hole, const double radius_Hole);
	void PutRoutParam(const double rOut);

protected:
	double radius_, minLength_, maxLength_, minRadius_, maxRadius_, 
		length_Hole_, radius_Hole_, rIn_, rOut_;
	int SimpleModel_, NbHole_;
	dcomp Tm_, Cm_, Lm_;
	vector<dcomp> Za_open;
	vector<dcomp> Za_close;
	vector<dcomp> Zb_;
};

#ifdef _MSC_VER
#undef std::cout
#endif

#endif


/* ################# PROCEDURE FOR ADDING NEW HORN ELEMENTS #####################

1. Add declarations to HornElement.h - best to copy CylinderSection and modify (delete) where appropriate
2. Add definitions to HornElement.cpp 
3. Define the new _TYPE constant and add to the list at the top of this file
4. Modify the following functions: 
	WindInstrument::rosen
	WindInstrument::copyLists
	WindInstrument::loadElemental     (all in WindInstrument.cpp)
	loadRB     (in runRB.cpp)
5. If the geometry is bent, also modify WindInstrument::append	
*/
