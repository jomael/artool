#ifndef TERMINATION_ELEMENT_H
#define TERMINATION_ELEMENT_H
#include "HornElement.h"
namespace ART{
//Element which contains the radiation impedance
class TerminationElement : public HornElement {
public:
	TerminationElement(const bool canModify=false, const bool canSplit=false, int test=0,int Contrib=1, const double RadiationRadius=1.0, const double PrevInputRadius  = 1.0, const int PrevType=1, 
		const int radiation_type = 0,const char filename[100] = " ",const float tempC=DFLT_TEMP_C, const float lossF=DFLT_LOSS_F, 
		const float humidity=DFLT_HUMIDITY, const float xc=DFLT_CO2_PPM, const string name = " ", Matrix MA=NULL, 
		Matrix MB=NULL, double rr=0.0) :
		HornElement(tempC,lossF,humidity/100,xc/1e06,name,TERMINATIONELEMENT_TYPE,MA,MB,rr,0, 
		canSplit, canModify), RADIATION_TYPE_(radiation_type), FILENAME_(filename), NAME_(name),
		RadiationRadius_(RadiationRadius), PrevInputRadius_(PrevInputRadius),
		PrevType_(PrevType), Test_(test), Contrib_(Contrib) {} //{if(radiation_type<10) {GetRadiationImpedance(test, Contrib);}}
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
}
#endif //TERMINATION_ELEMENT_H
