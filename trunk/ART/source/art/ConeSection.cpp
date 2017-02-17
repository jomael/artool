#include "ConeSection.h"

//###############CONE############
using namespace ART;
ConeSection::ConeSection(const double length, const double rDiff) {
	type_ = CONESECTION_TYPE;
	MA_ = NULL; MB_ = NULL; rr_ = 0.0; 
	length_ = length;
	rIn_ = 0.0; //Will be modified into feasible values when initialised.
	rOut_ = 0.0;
	rDiff_ = rDiff;
	tempC_=DFLT_TEMP_C;
	lossF_=DFLT_TEMP_C;
	humidity_ = DFLT_HUMIDITY/100; 		
	xc_ = DFLT_CO2_PPM/10e7;
	minLength_ = length;
	maxLength_ = length;
	minRin_ = -1.0;
	maxRin_ = -1.0;
	minRout_ = -1.0;
	maxRout_ = -1.0;

	canModify_ = false;
	canSplit_ = false;
	
}


ConeSection::ConeSection(const bool canModify, const bool canSplit, const double minLength, const double maxLength, 
		const double minRin, const double maxRin, const double minRout, const double maxRout) {
	//Sets up the limits for the physical paramaters. Most often used to define the default.

	if (minLength>maxLength) {std::cout << "ERROR in cone constructor: mismatch of length boundaries\n"; return;}
	if (minRin>maxRin) {std::cout << "ERROR in cone constructor: mismatch of rIn boundaries\n"; return;}
	if (minRout>maxRout) {std::cout << "ERROR in cone constructor: mismatch of rOut boundaries\n"; return;}

	type_ = CONESECTION_TYPE;
	MA_ = NULL; MB_ = NULL; rr_ = 0.0; 
	length_ = 0;
	rIn_ = 0;
	rOut_ = 0;
	rDiff_ = -0.0;
	tempC_=21.0;
	lossF_=1.0;
	
	minLength_ = minLength;
	maxLength_ = maxLength;
	minRin_ = minRin;
	maxRin_ = maxRin;
	minRout_ = minRout;
	maxRout_ = maxRout;

	canModify_ = canModify;
	canSplit_ = canSplit;
	
}

ConeSection::ConeSection(HornElement *c) { //copy constructor

	if ((*c).type()==CONESECTION_TYPE) {
		ConeSection* a = static_cast<ConeSection*>(c);
		type_ = CONESECTION_TYPE;
		MA_ = NULL; MB_ = NULL; rr_ = 0.0; 
		length_ = (*a).length_;
		rIn_ = (*a).rIn_;
		rOut_ = (*a).rOut_;
		rDiff_ = (*a).rDiff_;
		tempC_ = (*a).tempC_;
		lossF_ = (*a).lossF_;

		minLength_ = (*a).minLength_;
		maxLength_ = (*a).maxLength_;
		minRin_ = (*a).minRin_;
		maxRin_ = (*a).maxRin_;
		minRout_ = (*a).minRout_;
		maxRout_ = (*a).maxRout_;
	
		canModify_ = (*a).canModify_;
		canSplit_ = (*a).canSplit_;
		name_ = (*a).name_;
	} else std::cout << "ERROR: Trying copy constructor with incompatible elements (CONE). c.type:"<<(*c).type()<<" \n";
}

ConeSection::~ConeSection() {}

void ConeSection::print() const {
	if (name_!="") std::cout << "CONE: '" << name_.c_str() << "', length=" << length_ << ", radius=" << rIn_ << ".." << rOut_ << ", gradient=" << (rOut_-rIn_)/length_ << " TEMP: " << tempC_ << " LOSS: " << lossF_;
	else std::cout << "CONE: length=" << length_ << ", radius=" << rIn_ << ".." << rOut_ << ", gradient=" << (rOut_-rIn_)/length_ << " TEMP: " << tempC_ << " LOSS: " << lossF_;
}

void ConeSection::elementInitialise(const double previousRout) {
	length_=GARandomFloat(minLength_, maxLength_);
	if (minRin_>0) rIn_=GARandomFloat(minRin_, maxRin_); //-ve rIn values denote that the radius of this element is dependent on the previous element
	else rIn_ = previousRout;
	if (rDiff_ == 0.0)	// i.e. Cone not defined in terms of rDiff
		rOut_ = GARandomFloat(minRout_, maxRout_);
	else rOut_ = rIn_ + rDiff_;
}

int ConeSection::mutate(const float pmut, const float mu, const double previousRout) {
	if (!canModify_) return 0;
	int counter=0;
	
	if (GAFlipCoin(pmut)) { //length_
		counter++;
		length_ += GARandomFloat (-length_*mu/5, length_*mu/5);
		if (length_>maxLength_) length_=maxLength_;
		if (length_<minLength_) length_=minLength_;
	} 
	if (minRin_<0) rIn_ = previousRout; // minRadius<0 => radius depends on previous element.
	else {
		if (GAFlipCoin(pmut)) { //rIn_
			counter++;
			rIn_ += GARandomFloat (-rIn_*mu, rIn_*mu);
			if (rIn_>maxRin_) rIn_=maxRin_;
			if (rIn_<minRin_) rIn_=minRin_;
		} 
	}
	if (GAFlipCoin(pmut)) { //rOut_
		counter++;
		rOut_ += GARandomFloat (-rOut_*mu, rOut_*mu);
		if (rOut_>maxRout_) rOut_=maxRout_;
		if (rOut_<minRout_) rOut_=minRout_;
	}
	return counter;
}

bool ConeSection::split(list<HornElement*> &l, list<HornElement*>::iterator &iter)  {
	// Takes list and iterator that contain and point to this element, then splits this into 2 (half-size) elements and modifies the list accordingly with new element before this.
	// Makes no physical change to the instrument.
	// NOTE: does not modify radius boundaries for either element.

	if (!canSplit_) {return false;}
	
	length_ = length_/2;
	minLength_ = minLength_/2;
	maxLength_ = maxLength_/2;
	double halfRadius = (rOut_ + rIn_)/2;
	ConeSection* output = new ConeSection(*this);
	rIn_ = halfRadius;
	(*output).rOut_ = halfRadius;

	l.insert(iter, output);
	return true;
}

void ConeSection::elementCrossover (const HornElement& m, HornElement* s, HornElement* b, const float pcross) {
	//this is dad, takes mum and makes crossed-over elements in sis and bro if they exist.
	// Goes through each physical parameter for this element type; children either get mum's or dad's
	//version of that parameter depending on pcross-loaded coin. If canModify_ is false, this simply copies dad into bro and mum into sis.

	const ConeSection& mum = (const ConeSection &)m;
	ConeSection* sis = (ConeSection *)s;
	ConeSection* bro = (ConeSection *)b;

	if (s) {
		if(GAFlipCoin(pcross) && canModify_) {
			(*sis).length_ = length_;
		} else {
			(*sis).length_ = mum.length_;
		}
		if(GAFlipCoin(pcross) && canModify_) {
			(*sis).rIn_ = rIn_;
		} else {
			(*sis).rIn_ = mum.rIn_;
		}
		if(GAFlipCoin(pcross) && canModify_) {
			(*sis).rOut_ = rOut_;
		} else {
			(*sis).rOut_ = mum.rOut_;
		}
	}

	if (b) {
		if(GAFlipCoin(1-pcross) && canModify_) {
			(*bro).length_ = mum.length_;
		} else {
			(*bro).length_ = length_;
		}
		if(GAFlipCoin(1-pcross) && canModify_) {
			(*bro).rIn_ = mum.rIn_;
		} else {
			(*bro).rIn_ = rIn_;
		}
		if(GAFlipCoin(1-pcross) && canModify_) {
			(*bro).rOut_ = mum.rOut_;
		} else {
			(*bro).rOut_ = rOut_;
		}
	}
}

double ConeSection::findRadius (const double x) const {
	const double gradient=(rOut_ - rIn_)/length_;
	return (gradient*x + rIn_);
}

double ConeSection::findPosition (const double r) const {
	const double delta=(rOut_ - rIn_);
	return ( (delta == 0) ? 0 : length_ * (r-rIn_)/delta);
}

int ConeSection::TlaAccumulate (const double w, int IndFreq, SimpleMatrix *a, const int Ind, vector<int>&mem, dcomp *zt,list<Matrix> *ImpedanceList,vector<int> &IndHole) {
	coneTlaAccumulate(w, 0, length_, rIn_, rOut_, tempC_, lossF_, humidity_, xc_, a);
	return 0;
}

dcomp ConeSection::ZorumskiPlaneRadiation(const double w) const {
	return cylZorumskiPlaneRadiation(w, rOut_, tempC_, humidity_, xc_);
}

dcomp ConeSection::radiationImpedance(const double w) const {
	//Called if this is the last element in the list and returns relevant radiation impedance
	//This is Levine & Schwinger's model

	return radiationImpedanceLevineCone (w, 0, length_, rIn_, rOut_, tempC_, humidity_, xc_); //Used for spherical-wave assumption
}



void ConeSection::multimodeInputImpedance (const double w, const double Xin, const double Xout, const double Rin, const double Rout, const bool bends, Matrix *Zout) {
// WK: optimisation (constant R2/R1 -> constant jumpmatrix can be saved with section)

	const double logfactor = log(1+BORERESOLUTION_/100);					// bore resolution
	const double ratio = Rin/(Rout+eps);
	const double logratio = log(ratio);										// at most 50 slices per cm
	const int nSlices = max(min(50*Xout,floor(abs(logratio)/logfactor)),2.0);	// at least two slices with step between
    const double factor = pow(ratio, 1.0/nSlices);							// no steps at both ends!


DBG1 std::cout << endl << "*** ConeSection::multimodeInputImpedance *** for " << w/2/M_PI << " Hz" << endl;
DBG1 std::cout << "nSlices:	" << nSlices << "	rin:	" << Rin << "	rout:	" << Rout << "	len:	" << Xout-Xin << endl;
	double r=Rout;															// start slicing from end
	double rnext=r * factor;												// next radius
	double pos=findPosition((rnext+r)/2);									// slices have different thicknesses
	double lstep=Xout-pos;													// staircase around straight line!

DBG1 std::cout << "first radius:	" << r << "	@ pos:	" << Xout << endl;
DBG1 std::cout << "next radius:	" << rnext << "	@ pos:	" << pos << "	dist:	" << lstep << endl;
DBG1 std::cout << "cylMultimodeInputImpedance len=" << lstep << "	r=" << r << endl;
	cylMultimodeInputImpedance(w, Zout, lstep, r, LOSSY_, SIMPLIFIED_, bends, tempC_, lossF_, humidity_, xc_);

	for (int i=1; i<nSlices; i++) {
DBG1 std::cout << "jumpMultimodeInputImpedance	rin=" << rnext << "	rout=" << r << "	ratio=" << r/rnext << endl;
		jumpMultimodeInputImpedance(w, &rr_, &MA_, &MB_, Zout, rnext, r, bends, tempC_);
		r = rnext;
		rnext=r * factor;
		lstep=pos-findPosition((rnext+r)/2);				// find again average!
		pos=pos-lstep;
DBG1 std::cout << "next radius:	" << rnext << "	@ pos:	" << pos << "	dist:	" << lstep << endl;
DBG1 std::cout << "cylMultimodeInputImpedance len=" << lstep << "	r=" << r << endl;
		cylMultimodeInputImpedance(w, Zout, lstep, r, LOSSY_, SIMPLIFIED_, bends, tempC_, lossF_, humidity_, xc_);
	}
DBG1 std::cout << "jumpMultimodeInputImpedance	rin=" << rnext << "	rout=" << r << "	ratio=" << r/rnext << endl;
	jumpMultimodeInputImpedance(w, &rr_, &MA_, &MB_, Zout, rnext, r, bends, tempC_);
DBG1 std::cout << "last radius:	" << rnext << "	@ pos:	" << Xin << "	dist:	" << pos << endl;
DBG1 std::cout << "cylMultimodeInputImpedance len=" << pos << "	r=" << Rin << endl;

	cylMultimodeInputImpedance(w, Zout, pos, Rin, LOSSY_, SIMPLIFIED_, bends, tempC_, lossF_, humidity_, xc_);
}

void ConeSection::multimodeInputImpedance (const double w, const bool bends, Matrix *Zout) {
	multimodeInputImpedance (w, 0, length_, rIn_, rOut_, bends, Zout);	
}

void ConeSection::multimodeRadiationImpedance(const double w, const bool bends, const int IndFreq, Matrix* result) const {
	radiationImpedanceZorumski(w, result, rOut_, bends, tempC_, humidity_, xc_);
}

void ConeSection::multimodeRadiationImpedanceClosedEnd(const double w, const bool bends, Matrix* result) const {
	radiationImpedanceTerminated(w, result, rOut_, bends, tempC_,humidity_, xc_, cinfinity);	
}

void ConeSection::multimodeRadiationImpedanceOpenEnd(const double w, const bool bends, Matrix* result) const {
	radiationImpedanceZorumski(w, result, rOut_, bends, tempC_, humidity_, xc_);	
}

void ConeSection::multimodeRadiationImpedanceTerminated(const double w, const bool bends, Matrix* result, const dcomp z_trm) const {
	radiationImpedanceTerminated(w, result, rOut_, bends, tempC_, humidity_, xc_, z_trm);	
}


int ConeSection::MultimodeAccumulate (const double w,int IndFreq, const bool bends, Matrix *a, const int Ind, vector<int>&mem, Matrix *zt,list<Matrix> *ImpedanceList,vector<int> &IndHole) {

	const double logfactor = log(1+BORERESOLUTION_/100);					// bore resolution
	//const double ratio = rIn_/(rOut_+eps);
	//wasilakis
	const double ratio = rIn_/(rOut_+eps);
	const double logratio = log(ratio);										// at most 50 slices per cm
	const int nSlices = max(min(50*length_,floor(abs(logratio)/logfactor)),2.0);	// at least two slices with step between
	const double factor = pow(ratio, 1.0/nSlices);							// no steps at both ends!
	double zeta;
	double r = rOut_;															// start slicing from Xin
	double rnext = r * factor;					
	double pos = findPosition((rnext+r)/2);
	double lstep = length_ - pos;

	Matrix Mjump = Matrix(2*nModes_,2*nModes_);
	Matrix Mcyl = Matrix(2*nModes_,2*nModes_);

	if (r<rnext) {zeta = r/rnext;} 
	else {zeta = rnext/r;} 
	jumpMultimodeTransmMatrix(zeta,bends,&Mjump);
	for(int p=0;p<nModes_;p++){
		for(int q=p;q<2*nModes_;q++){
			if(p!=q && p!=(q-nModes_)){Mjump(p,q)=0; Mjump(p+nModes_,q)=0;}
		}
	}

	cylMultimodeAccumulate(w,lstep,r,LOSSY_,bends,tempC_,lossF_, humidity_, xc_,a);
	Mcyl.Unit();
	for (int i=1; i<nSlices; i++) {
		(*a) = Mjump*(*a);
		r = rnext;
		rnext = r * factor;
		lstep = pos - findPosition((rnext+r)/2);
		pos = pos - lstep;
		cylMultimodeAccumulate(w,lstep, r,LOSSY_,bends,tempC_,lossF_, humidity_, xc_, &Mcyl);
		(*a) = Mcyl*(*a);
		Mcyl.Unit();
	}
	(*a) = Mjump*(*a);
	//wasilakis: the following iteration is not needed!! Last position is already found
	//r = rnext;
	//rnext = r * factor;
	//lstep = pos - findPosition((rnext+r)/2);
	//pos = pos - lstep;
	cylMultimodeAccumulate(w,pos, rIn_,LOSSY_,bends,tempC_,lossF_, humidity_, xc_, &Mcyl);
	(*a) = Mcyl*(*a);

	return 0;
}


string ConeSection::getSaveString() const {
	char buffer[32];
	string output = CONESECTION_TYPE_STR + ", ";

	#if defined(_MSC_VER)
	_gcvt(length_, 10, buffer);
	output = output + buffer + ", ";
	_gcvt(rIn_, 10, buffer);
	output = output + buffer + ",";
	_gcvt(rOut_, 10, buffer);
	output = output + buffer + ",";

	#elif defined(__GNUG__)
	//g++ doesn't have _gcvt built-in  
	sprintf(buffer,"%f",length_);	
	output = output + buffer + ", ";
	sprintf(buffer,"%f",rIn_);	
	output = output + buffer + ", ";
	sprintf(buffer,"%f",rOut_);	
	output = output + buffer + ", ";
	#endif

	output = output + name_;
	return output;	
}
