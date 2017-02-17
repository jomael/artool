#ifndef CONE_BEND_SECTION_H
#define  CONE_BEND_SECTION_H

#include "HornElement.h"
namespace ART{
class ConeBendSection : public HornElement {
public:

	ConeBendSection(const bool canModify=false, const bool canSplit=false, const double length=100.0, 
		const double rIn=1.0, const double rOut = 2.0, const double bendRadius=50.0, const float tempC=DFLT_TEMP_C, const float lossF=DFLT_LOSS_F, 
		const float humidity=DFLT_HUMIDITY, const float xc=DFLT_CO2_PPM, const string name = " ", Matrix MA=NULL, 
		Matrix MB=NULL, double rr=0.0, bool dimBendMatrices = false) :
		HornElement(tempC,lossF,humidity/100,xc/1e06,name,CONEBENDSECTION_TYPE,MA,MB,rr,length, 
		canSplit, canModify), rIn_(rIn), rOut_(rOut),
		bendRadius_(bendRadius), dimBendMatrices_(dimBendMatrices) {}
	ConeBendSection(const bool canModify, const bool canSplit, const double minLength, const double maxLength, 
		const double minRin, const double maxRin, const double minRout, const double maxRout); //DEPRECIATED
	ConeBendSection(HornElement *c);
	~ConeBendSection();

	virtual ART::ARTvariant* getPropertyPointer(const char* name)
	{
		ART::ARTvariant* var = new ART::ARTvariant();

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
}
#endif //CONE_BEND_SECTION_H
