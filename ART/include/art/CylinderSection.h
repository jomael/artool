#ifndef CYLINDER_SECTION_H
#define CYLINDER_SECTION_H

#include "HornElement.h"
namespace ART{
class CylinderSection : public HornElement {
public:
	CylinderSection(const bool canModify=false, const bool canSplit=false, const double length=100.0, 
		const double radius=1.0, const float tempC=DFLT_TEMP_C, const float lossF=DFLT_LOSS_F, const float humidity=DFLT_HUMIDITY, 
		const float xc=DFLT_CO2_PPM, const string name = " ", Matrix MA=NULL, Matrix MB=NULL, double rr=0.0) : 
		HornElement(tempC,lossF,humidity/100,xc/1e06,name,CYLINDERSECTION_TYPE,MA,MB,
		rr,length, canSplit, canModify), radius_(radius)  {}
	CylinderSection(const bool canModify, const bool canSplit, const double minLength, const double maxLength, 
		const double minradius, const double maxradius); //max & min values
	CylinderSection(HornElement* c);
	~CylinderSection();

	virtual ART::ARTvariant* getPropertyPointer(const char* name)
	{
		ART::ARTvariant* var = new ART::ARTvariant();

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
}
#endif //CYLINDER_SECTION_H
