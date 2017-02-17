#ifndef BORE_JUMP_SECTION_H
#define  BORE_JUMP_SECTION_H
#include "HornElement.h"
namespace ART{
class BoreJumpSection : public HornElement {
public:
	BoreJumpSection(const double rIn=0.0, const double rOut=0.0, const float tempC=21.0, 
		const float lossF=DFLT_LOSS_F, const float humidity=DFLT_HUMIDITY, const float xc=DFLT_CO2_PPM, const string name = " ", 
		Matrix MA=NULL, Matrix MB=NULL, double rr=0.0, double length=0.0, bool canModify=false, 
		bool canSplit=false, double jumpSize=0.0) :
		HornElement(tempC, lossF, humidity/100, xc/1e06, name, BOREJUMPSECTION_TYPE, MA, MB, rr, length, canSplit, canModify),
		rIn_(rIn), rOut_(rOut), jumpSize_(rOut-rIn) {}
	~BoreJumpSection();
	BoreJumpSection(HornElement *c);

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
}
#endif // BORE_JUMP_SECTION_H
