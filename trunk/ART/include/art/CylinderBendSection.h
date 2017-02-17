#ifndef CYLINDER_BEND_SECTION_H
#define CYLINDER_BEND_SECTION_H

#include "HornElement.h"
namespace ART{
class CylinderBendSection : public HornElement {
public:

	CylinderBendSection(const bool canModify=false, const bool canSplit=false, const double length=100.0, 
		const double radius=1.0, const double bendRadius=50.0, const float tempC=DFLT_TEMP_C, const float lossF=DFLT_LOSS_F, 
		const float humidity=DFLT_HUMIDITY, const float xc=DFLT_CO2_PPM, const string name = " ", Matrix MA=NULL, 
		Matrix MB=NULL, double rr=0.0, bool hasBendMatrices = false, 
		bool dimBendMatrices = false) :
		HornElement(tempC,lossF,humidity/100,xc/1e06,name,CYLINDERBENDSECTION_TYPE,MA,MB,rr,length, 
		canSplit, canModify), radius_(radius), bendRadius_(bendRadius),
		hasBendMatrices_(hasBendMatrices), dimBendMatrices_(dimBendMatrices) {}
	CylinderBendSection(const bool canModify, const bool canSplit, const double minLength, const double maxLength, 
		const double minradius, const double maxradius); //DEPRECATED!!!!
	CylinderBendSection(HornElement* c);
	~CylinderBendSection();

	virtual ART::ARTvariant* getPropertyPointer(const char* name)
	{
		ART::ARTvariant* var = new ART::ARTvariant();

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
}
#endif //CYLINDER_BEND_SECTION_H

