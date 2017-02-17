#include "TerminationElement.h"
using namespace ART;
//###############TERMINATION ELEMENT############
TerminationElement::TerminationElement(HornElement *c) { //copy constructor
	if ((*c).type()==TERMINATIONELEMENT_TYPE) {
		TerminationElement* a = static_cast<TerminationElement*>(c);
		type_ = TERMINATIONELEMENT_TYPE;
		MA_ = NULL; MB_ = NULL; rr_ = 0.0; 
		RadiationRadius_ = (*a).RadiationRadius_;
		length_ = (*a).length_;
		tempC_  = (*a).tempC_;
		lossF_  = (*a).lossF_;
		minLength_ = (*a).minLength_;
		maxLength_ = (*a).maxLength_;
		minRadius_ = (*a).minRadius_;
		maxRadius_ = (*a).maxRadius_;
		canModify_ = (*a).canModify_;
		canSplit_ = (*a).canSplit_;
		name_ = (*a).name_;
	} else std::cout << "ERROR: Trying copy constructor with incompatible elements (CYLR)" << endl;
}

TerminationElement::~TerminationElement() {}

int TerminationElement::mutate(const float pmut, const float mu, const double previousRout) {
	//mutates parameter number a (length_=0, radius_=1) by a factor of up to +-mu.
	if (!canModify_) return 0;
	int counter=0;

	if (GAFlipCoin(pmut)) { //length_
		counter++;
		length_ += GARandomFloat (-length_*mu/5, length_*mu/5);
		if (length_>maxLength_) length_=maxLength_;
		if (length_<minLength_) length_=minLength_;
	}
	
	if (minRadius_<0) RadiationRadius_ = previousRout; // minRadius<0 => radius depends on previous element.
	else if (GAFlipCoin(pmut)) { //RadiationRadius_
			counter++;
			RadiationRadius_ += GARandomFloat (-RadiationRadius_*mu, RadiationRadius_*mu);
			if (RadiationRadius_>maxRadius_) RadiationRadius_=maxRadius_;
			if (RadiationRadius_<minRadius_) RadiationRadius_=minRadius_;
		}

	return counter;
}

void TerminationElement::elementCrossover (const HornElement& m, HornElement* s, HornElement* b, const float pcross) {
	//(*this) is dad, takes mum and makes crossed-over elements in sis and bro if they exist.
	// Goes through each physical parameter for this element type; children either get mum's or dad's
	//version of that parameter depending on pcross-loaded coin. If canModify_ is false, this simply copies dad into bro and mum into sis.*/

	const TerminationElement& mum = (const TerminationElement &)m;
	TerminationElement* sis = (TerminationElement *)s;
	TerminationElement* bro = (TerminationElement *)b;

	if (s) {
		if(GAFlipCoin(pcross) && canModify_) {
			(*sis).length_ = length_;
		} else {
			(*sis).length_ = mum.length_;
		}
		if(GAFlipCoin(pcross) && canModify_) {
			(*sis).RadiationRadius_ = RadiationRadius_;
		} else {
			(*sis).RadiationRadius_ = mum.RadiationRadius_;
		}
	}

	if (b) {
		if(GAFlipCoin(1-pcross) && canModify_) {
			(*bro).length_ = mum.length_;
		} else {
			(*bro).length_ = length_;
		}
		if(GAFlipCoin(1-pcross)  && canModify_) {
			(*bro).RadiationRadius_ = mum.RadiationRadius_;
		} else {
			(*bro).RadiationRadius_ = RadiationRadius_;
		}
	}
}


dcomp TerminationElement::radiationImpedanceLevine(const double w) const {
	dcomp z_t;
	//The radiation impedance depends on the previous geometry
	if(PrevType_ == CYLINDERSECTION_TYPE || PrevType_ == CYLINDERBENDSECTION_TYPE){
		return radiationImpedanceLevineCyl (w, RadiationRadius_, tempC_, humidity_, xc_);
	}
	else {
		double xIn;
		if(PrevType_ == BESSELSECTION_TYPE) xIn=length_*449/450;
		else xIn = 0;
		return radiationImpedanceLevineCone (w, xIn, length_, PrevInputRadius_, RadiationRadius_, tempC_, humidity_, xc_);
	}
}

//Multimodal resolution way
Matrix TerminationElement::multimodeRadiationImpedance_TermElt(const double w, const bool bends) const {
	//static 
	Matrix z_t = Matrix(nModes_,nModes_);
	//For each type of radiation:
	if(RADIATION_TYPE_==REFLECTING_RADIATION){radiationImpedanceTerminated(w, &z_t, RadiationRadius_, bends, tempC_,humidity_, xc_, cinfinity);}
	else if(RADIATION_TYPE_==ZORUMSKI_RADIATION){
		//if(PrevType_ == CONEBENDSECTION_TYPE) bendRadiationImpedanceInfiniteCylPipe0(w, &z_t, RadiationRadius_*2,tempC_, humidity_, xc_);
		//else {radiationImpedanceZorumski(w, &z_t, RadiationRadius_, bends, tempC_, humidity_, xc_);}
		radiationImpedanceZorumski(w, &z_t, RadiationRadius_, bends, tempC_, humidity_, xc_);
	}
	else{REPORT_ERROR("ERROR: termination mode isn't 0 or 1!");}
	return z_t;	
}


//wasilakis
void TerminationElement::multimodeRadiationImpedance(const double w, const bool bends, const int IndFreq, Matrix* result) const {
	//radiationImpedanceZorumski(w, result, RadiationRadius_, bends, tempC_, humidity_, xc_);	
	//*result = Mzt(IndFreq);
	*result = multimodeRadiationImpedance_TermElt(w, bends);
	//std::cout << "TerminationElement::multimodeRadiationImpedance("<<w<<","<<bends<<")\n";
	//std::cout << "result = " << *result;
}

void TerminationElement::multimodeRadiationImpedanceClosedEnd(const double w, const bool bends, Matrix* result) const {
	radiationImpedanceTerminated(w, result, RadiationRadius_, bends, tempC_, humidity_, xc_, cinfinity);	
}

void TerminationElement::multimodeRadiationImpedanceOpenEnd(const double w, const bool bends, Matrix* result) const {
	radiationImpedanceZorumski(w, result, RadiationRadius_, bends, tempC_, humidity_, xc_);	
}



//TLA resolution way
dcomp TerminationElement::PlaneWaveRadiationImpedance_TermElt(const double w) const {
	//For each type of radiation:	
	if(RADIATION_TYPE_==REFLECTING_RADIATION){return dcomp(-1);}
	else if(RADIATION_TYPE_==ZORUMSKI_RADIATION){return cylZorumskiPlaneRadiation(w, RadiationRadius_, tempC_, humidity_, xc_);}
	else if(RADIATION_TYPE_==LEVINE_RADIATION){return radiationImpedanceLevine(w);}
	else{REPORT_ERROR("ERROR: termination mode isn't 0, 1 or 2 !");}
}

/*
void TerminationElement::GetRadiationImpedance(int test, int Contrib){
	bool bends = false;
	Matrix Ones = Matrix(nModes_,nModes_);
	Ones.Unit();
	//The radiation impedance is specified in a file
	if(RADIATION_TYPE_==CUSTOM_RADIATION) SetImpedanceFile(FILENAME_.c_str(),1,Contrib,Mzt_); 
	//We are using theoretical models for the calculation of the radiation impedance	
	else{
		if (PrevType_ ==CYLINDERBENDSECTION_TYPE || PrevType_==CONEBENDSECTION_TYPE){bends = true;}
		if(nModes_==0){ //tla
			if(RADIATION_TYPE_==UNKNOWN_RADIATION)  for(double f=0; f<Freq.size(); f++){zt_.push_back(dcomp(1));} //Nothing is specified -> We are looking for the radiation impedance
			else if(Freq.size()!=0) for(double f=0; f<Freq.size(); f++){zt_.push_back(PlaneWaveRadiationImpedance_TermElt(2*M_PI*Freq[f]));} //The frequency axis isn't specified
			else for(double f=F_MIN_; f<=F_MAX_; f+=D_F_){zt_.push_back(PlaneWaveRadiationImpedance_TermElt(2*M_PI*f));} //The frequency axis is specified in the commande line
		}
		else{ //multimode: 
			if(RADIATION_TYPE_==UNKNOWN_RADIATION)  for(double f=0; f<Freq.size(); f++){Mzt_.push_back(Ones);}
			else if(Freq.size()!=0) for(double f=0; f<Freq.size(); f++){Mzt_.push_back(multimodeRadiationImpedance_TermElt(2*M_PI*Freq[f], bends));}
			else for(double f=F_MIN_; f<=F_MAX_; f+=D_F_){Mzt_.push_back(multimodeRadiationImpedance_TermElt(2*M_PI*f, bends));}
		}
	}

}
*/
