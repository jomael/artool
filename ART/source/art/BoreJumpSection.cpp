//***************JUMP DISCONTINUITY******************
#include "BoreJumpSection.h"
using namespace ART;
BoreJumpSection::~BoreJumpSection() { }

BoreJumpSection::BoreJumpSection(HornElement *c) { //copy constructor
	if ((*c).type()==BOREJUMPSECTION_TYPE) {
		BoreJumpSection* a = static_cast<BoreJumpSection*>(c);

		type_ = BOREJUMPSECTION_TYPE;
		MA_ = NULL; MB_ = NULL; rr_ = 0.0; 
		rIn_  = (*a).rIn_;
		rOut_ = (*a).rOut_;
		tempC_= (*a).tempC_;
		lossF_= (*a).lossF_;
		jumpSize_ = (*a).jumpSize_;
		length_ = (*a).length_;
		canModify_ = (*a).canModify_;
		canSplit_ = (*a).canSplit_;
		name_ = (*a).name_;
		canModify_ = false;
		canSplit_ = false;
	} else std::cout << "ERROR: Trying copy constructor with incompatible elements (JUMP)\n";
}

void BoreJumpSection::elementInitialise(const double previousRout) { 
	rIn_ = previousRout;
	rOut_ = rIn_ + jumpSize_;
}

void BoreJumpSection::elementCrossover (const HornElement& m, HornElement* s, HornElement* b, const float pcross) {
	// Crossover function for GA (deprecated)
	//(*this) is dad, takes mum and makes crossed-over elements in sis and bro if they exist.
	// Goes through each physical parameter for this element type; children either get mum's or dad's
	//version of that parameter depending on pcross-loaded coin. If canModify_ is false, this simply copies dad into bro and mum into sis.

	const BoreJumpSection& mum = (const BoreJumpSection &)m;
	BoreJumpSection* sis = (BoreJumpSection *)s;
	BoreJumpSection* bro = (BoreJumpSection *)b;

	if (s) {
		(*sis).rIn_ = mum.rIn_;
		(*sis).rOut_ = mum.rOut_;
	}

	if (b) {
		(*bro).rIn_ = rIn_;
		(*bro).rOut_ = rOut_;
	}
}


void BoreJumpSection::print() const {
	if (name_!="") std::cout << "JUMP: '" << name_.c_str() << "', radius=" << rIn_ << ".." << rOut_ << " TEMP: " << tempC_;
	else std::cout << "JUMP: radius=" << rIn_ << ".." << rOut_ << " TEMP: " << tempC_;
}

string BoreJumpSection::getSaveString() const {
	char buffer[32];
	string output = BOREJUMPSECTION_TYPE_STR + ", ";
	
	#if defined(_MSC_VER)
	_gcvt(rIn_, 10, buffer);
	output = output + buffer + ", ";
	_gcvt(rOut_, 10, buffer);
	output = output + buffer + ",";

	#elif defined(__GNUG__)
	//g++ doesn't have _gcvt built-in  
	sprintf(buffer,"%f",rIn_);	
	output = output + buffer + ", ";
	sprintf(buffer,"%f",rOut_);	
	output = output + buffer + ", ";
	#endif

	output = output + name_;
	return output;	
}

void BoreJumpSection::multimodeInputImpedance (const double w, const bool bends, Matrix *Zout) {
DBG2 std::cout << endl << "*** BoreJumpSection::multimodeInputImpedance ***" << endl;
	
	jumpMultimodeInputImpedance (w, &rr_, &MA_, &MB_, Zout, rIn_, rOut_, bends, tempC_);
}

void BoreJumpSection::multimodeRadiationImpedance(const double w, const bool bends, const int IndFreq, Matrix* result) const {
	radiationImpedanceZorumski(w, result, rOut_, bends, tempC_, humidity_, xc_);	
}

int BoreJumpSection::MultimodeAccumulate (const double w,int IndFreq,const bool bends,Matrix *a, const int Ind, vector<int>&mem, Matrix *zt,list<Matrix> *ImpedanceList,vector<int> &IndHole) {
	double zeta;
	double rad = 0;
	if(mem[3]==0) {rad = rIn_; rIn_ = rOut_; rOut_ = rad;}
	if (rIn_<rOut_) zeta = rIn_/rOut_; else zeta = rOut_/rIn_;
	jumpMultimodeTransmMatrix(zeta,bends, a);
	return 0;
}
