#ifndef BRANCH_H
#define BRANCH_H
#include "HornElement.h"

//Connexion between a hole and the main section
namespace ART{
class Branch : public HornElement{
public:
	Branch(const bool canModify=false, const bool canSplit=false, 
		const int NbHole = 1, const double rIn = 1.0, const double rOut = 1.0, const int SimpleModel = 1, 
		const float tempC=DFLT_TEMP_C, const float lossF=DFLT_LOSS_F,
	       const float humidity=DFLT_HUMIDITY, const float xc=DFLT_CO2_PPM, 
		const string name = " ", Matrix MA=NULL, Matrix MB=NULL, double rr=0.0) : 


  HornElement(tempC,lossF,humidity/100,xc/1e06,name,BRANCH_TYPE,MA,MB,rr,0, 
		canSplit, canModify), rIn_(rIn), rOut_(rOut), SimpleModel_(SimpleModel),
		NbHole_(NbHole) {}
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
}
#endif //BRANCH_H
