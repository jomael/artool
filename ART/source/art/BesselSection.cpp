#include "BesselSection.h"

//###############BESSEL#########

using namespace ART;
BesselSection::BesselSection(const bool canModify, const bool canSplit, const double minLength, const double maxLength, const double minRin, const double maxRin, 
		const double minRout, const double maxRout, const double minFlare, const double maxFlare) {
	
	if (minLength>maxLength) {std::cout << endl << "ERROR in Bessel constructor: mismatch of length boundaries: " << minLength << ">" << maxLength << endl; return;}
	if (minRin>maxRin) {std::cout << endl << "ERROR in Bessel constructor: mismatch of rIn boundaries: " << minRin << ">" << maxRin << endl; return;}
	if (minRout>maxRout) {std::cout << endl << "ERROR in Bessel constructor: mismatch of rOut boundaries" << minRout << ">" << maxRout << endl; return;}
	if (minFlare>maxFlare) {std::cout << endl << "ERROR in Bessel constructor: mismatch of flare boundaries" << minFlare << ">" << maxFlare << endl; return;}

	type_ = BESSELSECTION_TYPE;
	MA_ = NULL; MB_ = NULL; rr_ = 0.0; 
	length_ = 0;
	rIn_ = 0;
	rOut_ = 0;
	tempC_=21.0;
	lossF_=1.0;
	
	minLength_ = minLength;
	maxLength_ = maxLength;
	minRin_ = minRin;
	maxRin_ = maxRin;
	minRout_ = minRout;
	maxRout_ = maxRout;
	minFlare_ = minFlare;
	maxFlare_ = maxFlare;

	canModify_ = canModify;
	canSplit_ = canSplit;
	name_ = "";
}

BesselSection::BesselSection(HornElement *c) {

	if ((*c).type()==BESSELSECTION_TYPE) {
		BesselSection* a = static_cast<BesselSection*>(c);
		
		type_ = BESSELSECTION_TYPE;
		MA_ = NULL; MB_ = NULL; rr_ = 0.0; 
		length_ = (*a).length_;
		rIn_ = (*a).rIn_;
		rOut_ = (*a).rOut_;
		flare_ = (*a).flare_;
		tempC_ = (*a).tempC_;
		lossF_ = (*a).lossF_;
		
		minLength_ = (*a).minLength_;
		maxLength_ = (*a).maxLength_;
		minRin_ = (*a).minRin_;
		maxRin_ = (*a).maxRin_;
		minRout_ = (*a).minRout_;
		maxRout_ = (*a).maxRout_;
		minFlare_ = (*a).minFlare_;
		maxFlare_ = (*a).maxFlare_;
	
		canModify_ = (*a).canModify_;
		canSplit_ = (*a).canSplit_;
	} else std::cout << "ERROR: Trying copy constructor with incompatible elements (BESS)\n";
}

BesselSection::~BesselSection() {}

void BesselSection::print() const {
	if (name_!="") std::cout << "BESS: '" << name_.c_str() << "', length=" << length_ << ", radius=" << rIn_ << ".." << rOut_ << ", flare=" << flare_ << " TEMP: " << tempC_;
	else std::cout << "BESS: length=" << length_ << ", radius=" << rIn_ << ".." << rOut_ << ", flare=" << flare_ << " TEMP: " << tempC_;
}


void BesselSection::elementInitialise(const double previousRout) {
	//Randomly sets physical parameters within their boundaries. 
	
	length_=GARandomFloat(minLength_, maxLength_);
	if (minRin_>0) rIn_=GARandomFloat(minRin_, maxRin_); //-ve rIn values denote that the radius of this element is dependent on the previous element
	else rIn_ = previousRout;
	rOut_=GARandomFloat(minRout_, maxRout_);
	flare_=GARandomFloat(minFlare_, maxFlare_);
}

double BesselSection::gradientIn() const {
	//Calculates the gradient at the mouth and throat of the section.
	const double epsilon = -1/flare_;
	const double b = pow( length_/(pow(rIn_, epsilon)-pow(rOut_, epsilon)) , flare_);
	const double x0 = pow(rOut_/b, epsilon);
	
	return flare_ * b * pow(x0 + length_, -flare_-1);
}

double BesselSection::gradientOut() const {
	//Calculates the gradient at the mouth and throat of the section.
	const double epsilon = -1/flare_;
	const double b = pow( length_/(pow(rIn_, epsilon)-pow(rOut_, epsilon)) , flare_);
	const double x0 = pow(rOut_/b, epsilon);
	
	return flare_ * b * pow(x0, -flare_-1);
}

int BesselSection::mutate(const float pmut, const float mu, const double previousRout) {
	if (!canModify_) return 0;

	int counter=0;
	
	if (GAFlipCoin(pmut)) { //length_
		counter++;
		length_ += GARandomFloat (-length_*mu, length_*mu);
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
	if (GAFlipCoin(pmut)) { //flare_
		counter++;
		flare_ += GARandomFloat (-flare_*mu, flare_*mu);
		if (flare_>maxFlare_) flare_=maxFlare_;
		if (flare_<minFlare_) flare_=minFlare_;
	}
	return counter;
}

bool BesselSection::split(list<HornElement*> &l, list<HornElement*>::iterator &iter)  {
	// Takes list and iterator that contain and point to this element, then splits this into 2 identical (half-size) elements and modifies the list accordingly.
	// Makes no physical change to the instrument. New element is added *before* existing one.

	if (!canSplit_) {return false;}
	
	double halfLength;
	
	if (length_>0.0) halfLength = length_/2;
	else {halfLength = (maxLength_ + minLength_)/2; std::cout << "***";}
	
	const double halfRadius = findRadius(halfLength); 
	length_ = halfLength;
	BesselSection* output = new BesselSection(*this);

	minLength_ = minLength_/2;
	maxLength_ = maxLength_/2;
	rIn_ = halfRadius;
	minRin_ = -1.0; //ensures that rIn is bound to previous element => no jump at join.
	
	(*output).rOut_ = halfRadius;
	(*output).minRout_ = 0.0;
	(*output).minLength_ = length_;
	(*output).maxLength_ = length_; //All but the mouth of a split Bessel have fixed length. WLOG.

	l.insert(iter, output);
	return true;
}


void BesselSection::elementCrossover (const HornElement& m, HornElement* s, HornElement* b, const float pcross) {
	//this is dad, takes mum and makes crossed-over elements in sis and bro if they exist.
	//Goes through each physical parameter for this element type; children either get mum's or dad's
	//version of that parameter depending on pcross-loaded coin. If canModify_ is false, this simply copies dad into bro and mum into sis.

	const BesselSection& mum = (const BesselSection &)m;
	BesselSection* sis = (BesselSection *)s;
	BesselSection* bro = (BesselSection *)b;

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
		if(GAFlipCoin(pcross) && canModify_) {
			(*sis).flare_ = flare_;
		} else {
			(*sis).flare_ = mum.flare_;
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
		if(GAFlipCoin(1-pcross) && canModify_) {
			(*bro).flare_ = mum.flare_;
		} else {
			(*bro).flare_ = flare_;
		}
	}
}

double BesselSection::findRadius(const double x) const {
	//Calculates radius at given distance from throat.
	const double epsilon = -1/flare_;
	const double b = pow( length_/(pow(rIn_, epsilon)-pow(rOut_, epsilon)) , flare_);
	const double x0 = pow(rOut_/b, epsilon);
	
	const double xt = length_ - x;
	const double r = b*pow((x0+xt),-flare_);

	return r;
}

double BesselSection::findPosition (const double r) const {
	//Calculates distance from throat of given radius.
	const double epsilon = -1/flare_;
	const double b = pow( length_/(pow(rIn_, epsilon)-pow(rOut_, epsilon)) , flare_);
	const double x0 = pow(rOut_/b, epsilon);

	// r == b*pow((x0+length_-x),-flare_)
	// log(r/b) == -flare * log (x0+length_-x)
	// pow(b/r,1/flare) == x0+length_-x
	// x == x0+length_-pow(b/r,1/flare)

	//wasilakis
	//return ((r == 0) ? 0 : pow(b/r,1/flare_)-x0);
	return ((r == 0) ? 0 : length_ + x0 - pow(b/r,1/flare_));
}


double BesselSection::findExtremeRadius (const double x, const bool isMax) const {
	//Calculates radius at given distance from throat, using extreme values for flare and rOut but keeping rIn constant
	double epsilon;
	double b;
	double x0;
	double xt, r;


	if (isMax) {
		epsilon = -1/maxFlare_;
		b = pow( maxLength_/(pow(maxRin_, epsilon)-pow(maxRout_, epsilon)) , maxFlare_);
		x0 = pow(maxRout_/b, epsilon);
		xt = maxLength_ - x;
		r = b*pow((x0+xt),-maxFlare_);
	} else {
		epsilon = -1/minFlare_;
		b = pow( minLength_/(pow(minRin_, epsilon)-pow(minRout_, epsilon)) , minFlare_);
		x0 = pow(minRout_/b, epsilon);
		xt = minLength_ - x;
		r = b*pow((x0+xt),-minFlare_);
	}

	return r;
}


int BesselSection::TlaAccumulate (const double w,int IndFreq, SimpleMatrix *a, const int Ind, vector<int>&mem, dcomp *zt,list<Matrix> *ImpedanceList,vector<int> &IndHole) {
	(*a).setValues(dcomp(1.0,0.0), dcomp(0.0,0.0), dcomp(0.0,0.0), dcomp(1.0,0.0));
	static SimpleMatrix temp;

	//const int imax=50;
	//const double step=length_/imax;

	const double step=length_/(int)length_;
	double xIn=0;
	double tempRin = findRadius(0.0); 
	double tempRout;
	double xOut=step;
	const int imax=(int)(length_);
	
	//for (double xOut=step; xOut<=(length_); xOut+=step) {
	for (int i=0; i<imax; i++) {
		tempRout=findRadius(xOut);
		coneTlaAccumulate(w, xIn, xOut, tempRin, tempRout, tempC_, lossF_, humidity_, xc_, &temp);
		(*a) = (*a) * temp;
		xIn=xOut;
		tempRin=tempRout;
		xOut+=step;
	}
	return 0;
}

dcomp BesselSection::ZorumskiPlaneRadiation(const double w) const {
	return cylZorumskiPlaneRadiation(w, rOut_, tempC_, humidity_, xc_);
}


//wasilakis
// corrected findPosition for bessel horn
// left commented out older (much slower) implementation using findRadius for comparison

/*
void BesselSection::multimodeInputImpedance (const double w, const bool bends, Matrix *Zout)  {

	const int nJumps = (int)length_*10;

	const double lstep = length_/nJumps;

	double l=length_ - lstep;

	double rNew=rOut_;

	double rOld=0.0;



	cylMultimodeInputImpedance(w, Zout, lstep/2, rNew, LOSSY_, SIMPLIFIED_, bends, tempC_, lossF_, humidity_, xc_);

	

	for (int i=1; i<nJumps; i++) {

		rOld=rNew;

		rNew=findRadius(l);

		jumpMultimodeInputImpedance(w, &rr_, &MA_, &MB_, Zout, rNew, rOld, bends ,tempC_);

		cylMultimodeInputImpedance(w, Zout, lstep, rNew, LOSSY_, SIMPLIFIED_, bends, tempC_, lossF_, humidity_, xc_);

		l-=lstep;

	}

	jumpMultimodeInputImpedance(w, &rr_, &MA_, &MB_, Zout, rIn_, rNew, bends, tempC_);

	cylMultimodeInputImpedance(w, Zout, lstep/2, rIn_, LOSSY_, SIMPLIFIED_, bends, tempC_, lossF_, humidity_, xc_);

}
*/


void BesselSection::multimodeInputImpedance (const double w, const bool bends, Matrix *Zout)  {
// WK: optimisation (constant R2/R1 -> constant jumpmatrix can be saved with section)
	const double logfactor = log(1+BORERESOLUTION_/100);						// bore resolution
	const double ratio = rIn_/rOut_;
	const double logratio = log(ratio);											// at most 50 slices per cm
	const int nSlices = max(min(50*length_,floor(abs(logratio)/logfactor)),2.0);	// at least two slices with step between
	const double factor = pow(ratio, 1.0/nSlices);								// no steps at both ends!


DBG1 std::cout << endl << "*** BesselSection::multimodeInputImpedance *** for " << w/2/M_PI << " Hz" << endl;
DBG1 std::cout << "nSlices:	" << nSlices << "	rin:	" << rIn_ << "	rout:	" << rOut_ << "	len:	" << length_ << endl;
	double r=rOut_;																// start slicing from end
	double rnext=r * factor;													// next radius
	double pos=findPosition((rnext+r)/2);										// slices have different thicknesses
	double lstep=length_-pos;													// staircase around straight line!


DBG1 std::cout << "first radius:	" << r << "	@ pos:	" << length_ << endl;
DBG1 std::cout << "next radius:	" << rnext << "	@ pos:	" << pos << "	dist:	" << lstep << endl;
DBG1 std::cout << "cylMultimodeInputImpedance len=" << lstep << "	r=" << r << endl;
	cylMultimodeInputImpedance(w, Zout, lstep, r, LOSSY_, SIMPLIFIED_, bends, tempC_, lossF_, humidity_, xc_);

	for (int i=1; i<nSlices; i++) {
DBG1 std::cout << "jumpMultimodeInputImpedance	rin=" << rnext << "	rout=" << r << "	ratio=" << r/rnext << endl;
		jumpMultimodeInputImpedance(w, &rr_, &MA_, &MB_, Zout, rnext, r, bends, tempC_);
		r = rnext;
		rnext=r * factor;
		lstep=pos-findPosition((rnext+r)/2);									// find again average!
		pos=pos-lstep;
DBG1 std::cout << "next radius:	" << rnext << "	@ pos:	" << pos << "	dist:	" << lstep << endl;
DBG1 std::cout << "cylMultimodeInputImpedance len=" << lstep << "	r=" << r << endl;
		cylMultimodeInputImpedance(w, Zout, lstep, r, LOSSY_, SIMPLIFIED_, bends, tempC_, lossF_, humidity_, xc_);
	}
DBG1 std::cout << "jumpMultimodeInputImpedance	rin=" << rnext << "	rout=" << r << "	ratio=" << r/rnext << endl;
	jumpMultimodeInputImpedance(w, &rr_, &MA_, &MB_, Zout, rnext, r, bends, tempC_);
DBG1 std::cout << "last radius:	" << rnext << "	@ pos:	" << 0 << "	dist:	" << pos << endl;
DBG1 std::cout << "cylMultimodeInputImpedance len=" << pos << "	r=" << rIn_ << endl;
	cylMultimodeInputImpedance(w, Zout, pos, rIn_, LOSSY_, SIMPLIFIED_, bends, tempC_, lossF_, humidity_, xc_);
}


dcomp BesselSection::radiationImpedance(const double w) const {
	//Called if this is the last element in the list and returns relevant radiation impedance
	double xIn=length_*449/450;
	return radiationImpedanceLevineCone(w, xIn, length_, findRadius(xIn), findRadius(length_), tempC_, humidity_, xc_); 
}


void BesselSection::multimodeRadiationImpedance(const double w, const bool bends, const int IndFreq,Matrix* result) const {
	radiationImpedanceZorumski(w, result, rOut_, bends, tempC_, humidity_, xc_);
}


void BesselSection::multimodeRadiationImpedanceClosedEnd(const double w, const bool bends, Matrix* result) const {
	radiationImpedanceTerminated(w, result, rOut_, bends, tempC_, humidity_, xc_, cinfinity);
}

void BesselSection::multimodeRadiationImpedanceOpenEnd(const double w, const bool bends, Matrix* result) const {
	radiationImpedanceZorumski(w, result, rOut_, bends, tempC_, humidity_, xc_);
}

void BesselSection::multimodeRadiationImpedanceTerminated(const double w, const bool bends, Matrix* result, const dcomp z_trm) const {
	radiationImpedanceTerminated(w, result, rOut_, bends, tempC_, humidity_, xc_, z_trm);
}

int BesselSection::MultimodeAccumulate (const double w,int IndFreq,const bool bends,Matrix *a, const int Ind, vector<int>&mem, Matrix *zt,list<Matrix> *ImpedanceList, vector<int> &IndHole) {

	const double logfactor = log(1+BORERESOLUTION_/100);						// bore resolution
	const double ratio = rIn_/rOut_;
	const double logratio = log(ratio);											// at most 50 slices per cm
	const int nSlices = max(min(50*length_,floor(abs(logratio)/logfactor)),2.0);	// at least two slices with step between
	const double factor = pow(ratio, 1.0/nSlices);								// no steps at both ends!

	double r=rOut_;																// start slicing from end
	double rnext=r * factor;													// next radius
	double pos=findPosition((rnext+r)/2);										// slices have different thicknesses
	double lstep=length_-pos;

	double zeta;

	
/*
	Matrix Mjump = Matrix(2*nModes_,2*nModes_);
	for(int p=0;p<nModes_;p++){
		for(int q=p;q<2*nModes_;q++){
			if(p!=q && p!=(q-nModes_)){Mjump(p,q)=0; Mjump(p+nModes_,q)=0;}
		}
	}


	Matrix Mcyl = Matrix(2*nModes_,2*nModes_);

	if (r<rnext) {zeta = r/rnext;} 
	else {zeta = rnext/r;} 
	jumpMultimodeTransmMatrix(zeta,bends,&Mjump);

	cylMultimodeAccumulate(w,lstep,r,LOSSY_,bends,tempC_,lossF_, humidity_, xc_, a);
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
	cylMultimodeAccumulate(w,pos, rIn_,LOSSY_,bends,tempC_,lossF_, humidity_, xc_, &Mcyl);
	(*a) = Mcyl*(*a);
*/

	//wasilakis, replacing the above commented out section
	
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
	cylMultimodeAccumulate(w,pos, rIn_,LOSSY_,bends,tempC_,lossF_, humidity_, xc_, &Mcyl);
	(*a) = Mcyl*(*a);


	return 0;
}

string BesselSection::getSaveString() const {
	char buffer[32];
	string output = BESSELSECTION_TYPE_STR + ", ";

	#if defined(_MSC_VER)
	_gcvt(length_, 10, buffer);
	output = output + buffer + ", ";
	_gcvt(rIn_, 10, buffer);
	output = output + buffer + ",";
	_gcvt(rOut_, 10, buffer);
	output = output + buffer + ",";
	_gcvt(flare_, 10, buffer);
	output = output + buffer + ",";

	#elif defined(__GNUG__)
	//g++ doesn't have _gcvt built-in  
	sprintf(buffer,"%f",length_);	
	output = output + buffer + ", ";
	sprintf(buffer,"%f",rIn_);	
	output = output + buffer + ", ";
	sprintf(buffer,"%f",rOut_);	
	output = output + buffer + ", ";
	sprintf(buffer,"%f",flare_);	
	output = output + buffer + ", ";
	#endif

	output = output + name_;
	return output;	
}
