#ifndef BESSEL_SECTION_H
#define BESSEL_SECTION_H

#include "HornElement.h"
namespace ART{
class BesselSection : public HornElement {
public:

	BesselSection(const bool canModify=false, const bool canSplit=false, const double length=100.0, 
		const double rIn=1.0, const double rOut=2.0, const double flare=2,
		      const float tempC=DFLT_TEMP_C, const float lossF=DFLT_LOSS_F, 
		      const float humidity=DFLT_HUMIDITY, const float xc=DFLT_CO2_PPM, const string name = " ", Matrix MA=NULL, 
		Matrix MB=NULL, double rr=0.0) :
		HornElement(tempC,lossF,humidity/100,xc/1e06,name,BESSELSECTION_TYPE,MA,MB,rr,length, 
		canSplit, canModify), rIn_(rIn), rOut_(rOut), flare_(flare) {}
	
	BesselSection(const bool canModify, const bool canSplit, const double minLength, const double maxLength, const double minRin, const double maxRin, 
		const double minRout, const double maxRout, const double minFlare, const double maxFlare); //max&min values
	BesselSection(HornElement *c);
	~BesselSection();

	virtual ART::ARTvariant* getPropertyPointer(const char* name)
	{
		ART::ARTvariant* var = new ART::ARTvariant();

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
}
#endif //BESSEL_SECTION_H
