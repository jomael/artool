#include "CylinderSection.h"
//#######CYLINDER##########
using namespace ART;
CylinderSection::CylinderSection(const bool canModify, const bool canSplit, const double minLength, const double maxLength, 
		const double minradius, const double maxradius) {

	if (minLength>maxLength) {std::cout << "ERROR in cylinder constructor: mismatch of length boundaries\n"; return;}
	if (minradius>maxradius) {std::cout << "ERROR in cylinder constructor: mismatch of radius boundaries\n"; return;}

	type_ = CYLINDERSECTION_TYPE;
	MA_ = NULL; MB_ = NULL; rr_ = 0.0; 
	radius_ = 0;
	length_ = 0;
	tempC_=21.0;
	lossF_=1.0;
	minLength_ = minLength;
	maxLength_ = maxLength;
	minRadius_ = minradius;
	maxRadius_ = maxradius;
	canModify_ = canModify;
	canSplit_ = canSplit;
	name_ = "";
}

CylinderSection::CylinderSection(HornElement *c) { //copy constructor
	if ((*c).type()==CYLINDERSECTION_TYPE) {
		CylinderSection* a = static_cast<CylinderSection*>(c);

		type_ = CYLINDERSECTION_TYPE;
		MA_ = NULL; MB_ = NULL; rr_ = 0.0; 
		radius_ = (*a).radius_;
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

CylinderSection::~CylinderSection() {}

void CylinderSection::print() const {
	if (name_ != "") std::cout << "CYLR: '" << name_.c_str() << "', length=" << length_ << ", radius=" << radius_ << " TEMP: " << tempC_ << " LOSS: " << lossF_;
	else std::cout << "CYLR: length=" << length_ << ", radius=" << radius_ << " TEMP: " << tempC_ << " LOSS: " << lossF_;
}

void CylinderSection::elementInitialise(const double previousRout) { //randomly sets physical parameters within their limits.
	length_ = GARandomFloat(minLength_, maxLength_);
	if(minRadius_>0) radius_ = GARandomFloat(minRadius_, maxRadius_); //-ve minRadius values denote that the radius of this element is dependent on the previous element
	else radius_ = previousRout;
}


int CylinderSection::mutate(const float pmut, const float mu, const double previousRout) {
	//mutates parameter number a (length_=0, radius_=1) by a factor of up to +-mu.
	if (!canModify_) return 0;
	int counter=0;

	if (GAFlipCoin(pmut)) { //length_
		counter++;
		length_ += GARandomFloat (-length_*mu/5, length_*mu/5);
		if (length_>maxLength_) length_=maxLength_;
		if (length_<minLength_) length_=minLength_;
	}
	
	if (minRadius_<0) radius_ = previousRout; // minRadius<0 => radius depends on previous element.
	else if (GAFlipCoin(pmut)) { //radius_
			counter++;
			radius_ += GARandomFloat (-radius_*mu, radius_*mu);
			if (radius_>maxRadius_) radius_=maxRadius_;
			if (radius_<minRadius_) radius_=minRadius_;
		}

	return counter;
}

bool CylinderSection::split(list<HornElement*> &l, list<HornElement*>::iterator &iter)  {
	// Takes list and iterator that contain and point to this element, then splits this into 2 identical (half-size) elements and modifies the list accordingly.
	// Makes no physical change to the instrument.

	if (!canSplit_) {return false;}
	
	length_ = length_/2;
	minLength_ = minLength_/2;
	maxLength_ = maxLength_/2;
	CylinderSection* output = new CylinderSection(*this);
	l.insert(iter, output);
	return true;
}

void CylinderSection::elementCrossover (const HornElement& m, HornElement* s, HornElement* b, const float pcross) {
	//(*this) is dad, takes mum and makes crossed-over elements in sis and bro if they exist.
	// Goes through each physical parameter for this element type; children either get mum's or dad's
	//version of that parameter depending on pcross-loaded coin. If canModify_ is false, this simply copies dad into bro and mum into sis.*/

	const CylinderSection& mum = (const CylinderSection &)m;
	CylinderSection* sis = (CylinderSection *)s;
	CylinderSection* bro = (CylinderSection *)b;

	if (s) {
		if(GAFlipCoin(pcross) && canModify_) {
			(*sis).length_ = length_;
		} else {
			(*sis).length_ = mum.length_;
		}
		if(GAFlipCoin(pcross) && canModify_) {
			(*sis).radius_ = radius_;
		} else {
			(*sis).radius_ = mum.radius_;
		}
	}

	if (b) {
		if(GAFlipCoin(1-pcross) && canModify_) {
			(*bro).length_ = mum.length_;
		} else {
			(*bro).length_ = length_;
		}
		if(GAFlipCoin(1-pcross)  && canModify_) {
			(*bro).radius_ = mum.radius_;
		} else {
			(*bro).radius_ = radius_;
		}
	}
}

int CylinderSection::TlaAccumulate (const double w, int IndFreq, SimpleMatrix *a, const int Ind, vector<int>&mem, dcomp *zt,list<Matrix> *ImpedanceList,vector<int> &IndHole) {
	cylTlaAccumulate(w, length_, radius_, tempC_, lossF_, humidity_, xc_, a);
	return 0;
}

dcomp CylinderSection::radiationImpedance(const double w) const {
	//Called if this is the last element in the list and returns relevant radiation impedance
	return radiationImpedanceLevineCyl (w, radius_, tempC_, humidity_, xc_);
}

int CylinderSection::MultimodeAccumulate (const double w,int IndFreq,const bool bends,Matrix *a, const int Ind, vector<int>&mem, Matrix *zt,list<Matrix> *ImpedanceList,vector<int> &IndHole) {
	//length, radius, lossy, bends, temp, lossF
	cylMultimodeAccumulate(w, length_, radius_, LOSSY_,bends ,tempC_, lossF_, humidity_, xc_, a);
	return 0;
}

//##### HIGHER MODES #####

dcomp CylinderSection::ZorumskiPlaneRadiation(const double w) const {
	return cylZorumskiPlaneRadiation(w, radius_, tempC_, humidity_, xc_);
}

/*
dcomp CylinderSection::Zorumskikn (const double w, const double radius) const {
	//complex wavenumber for higher modes (Bruneau et al)
	//Returns an array, size nModes, containing kn for n=1..nModes
//	const double k = w/C(tempC_);
	const double k = w/C2(tempC_,humidity_,xc_);
	dcomp result;
		 
	if (LOSSY_) {
		const double root_f = sqrt(w/2/M_PI);
		const dcomp eps_v = dcomp(2.03e-5 * root_f, 2.03e-5 * root_f);
		const dcomp eps_t = dcomp(0.95e-5 * root_f, 0.95e-5 * root_f);
		const dcomp eps_i = (eps_v + eps_t); // WK no extra loss dcomp(lossF_,0) here! 
	
		result = sqrt(k*k + 2*k/radius*dcomp(eps_i.imag(), -eps_i.real())); //lossy
	} else result=k;	//lossless
	return result;
}
*/

void CylinderSection::multimodeRadiationImpedance(const double w, const bool bends, const int IndFreq, Matrix* result) const {
	radiationImpedanceZorumski(w, result, radius_, bends, tempC_, humidity_, xc_);	
	//std::cout << "CylinderSection::multimodeRadiationImpedance\n";
	//std::cout << "result = " << *result;
}

void CylinderSection::multimodeRadiationImpedanceClosedEnd(const double w, const bool bends, Matrix* result) const {
	radiationImpedanceTerminated(w, result, radius_, bends, tempC_, humidity_, xc_, cinfinity);	
}

void CylinderSection::multimodeRadiationImpedanceOpenEnd(const double w, const bool bends, Matrix* result) const {
	radiationImpedanceZorumski(w, result, radius_, bends, tempC_, humidity_, xc_);	
}

void CylinderSection::multimodeRadiationImpedanceTerminated(const double w, const bool bends, Matrix* result, const dcomp z_trm) const {
	radiationImpedanceTerminated(w, result, radius_, bends, tempC_, humidity_, xc_, z_trm);	
}

void CylinderSection::multimodeInputImpedance (const double w, const bool bends, Matrix *Zout) {
DBG2 std::cout << endl << "*** CylinderSection::multimodeInputImpedance ***" << endl;
	//std::cout << "cylMultimodeInputImpedance(" <<w<< "," << *Zout<< "," << length_<< "," << radius_<< "," << LOSSY_<< "," << SIMPLIFIED_<< "," << bends<< "," << tempC_<< "," << lossF_<< "," << humidity_<< "," << xc_<< ")\n";
	cylMultimodeInputImpedance (w, Zout, length_, radius_, LOSSY_, SIMPLIFIED_, bends, tempC_, lossF_, humidity_, xc_);
	//std::cout << "CylinderSection::multimodeInputImpedance\n";
	//std::cout << "result = " << *Zout;
}

//### END HIGHER MODES ###

string CylinderSection::getSaveString() const {
	char buffer[32];
	string output = CYLINDERSECTION_TYPE_STR + ", ";
	
	#if defined(_MSC_VER)
	_gcvt(length_, 10, buffer);
	output = output + buffer + ", ";
	_gcvt(radius_, 10, buffer);
	output = output + buffer + ",";

	#elif defined(__GNUG__)
	//g++ doesn't have _gcvt built-in  
	sprintf(buffer,"%f",length_);	
	output = output + buffer + ", ";
	sprintf(buffer,"%f",radius_);	
	output = output + buffer + ", ";
	#endif

	output = output + name_;
	return output;	
}
