#include "ConeBendSection.h"

//###############CONE BEND############
using namespace ART;
//bool ConeBendSection::dimBendMatrices_=true;
double ConeBendSection::Zc_;
Matrix ConeBendSection::A_ = Matrix(nModes_, nModes_);
Matrix ConeBendSection::B_ = Matrix(nModes_, nModes_);
Matrix ConeBendSection::C_ = Matrix(nModes_, nModes_);
Matrix ConeBendSection::K_ = Matrix(nModes_, nModes_);
Matrix ConeBendSection::Fbend_ = Matrix(nModes_, nModes_);
Matrix ConeBendSection::G_ = Matrix(nModes_, nModes_);
Matrix ConeBendSection::Z_ = Matrix(nModes_, nModes_);
Matrix ConeBendSection::Zprime_ = Matrix(nModes_, nModes_);
Matrix ConeBendSection::Zstore_ = Matrix(nModes_, nModes_);
Matrix ConeBendSection::Ztotal_ = Matrix(nModes_, nModes_);

//Sadjad: See comments in function CylinderBendSection::setnModes for explanations
void ConeBendSection::setnModes(const int nModes)
{
	if (nModes_!=nModes) 
	{
		nModes_=nModes; 
		if (nModes>0) initMatrices();
	};
	A_.Null(nModes_, nModes_);
	B_.Null(nModes_, nModes_);
	C_.Null(nModes_, nModes_);
	K_.Null(nModes_, nModes_);
	Fbend_.Null(nModes_, nModes_);
	G_.Null(nModes_, nModes_);
	Z_.Null(nModes_, nModes_);
	Zprime_.Null(nModes_, nModes_);
	Zstore_.Null(nModes_, nModes_);
	Ztotal_.Null(nModes_, nModes_);
}

ConeBendSection::ConeBendSection(const bool canModify, const bool canSplit, const double minLength, const double maxLength, 
		const double minRin, const double maxRin, const double minRout, const double maxRout) {
	//Sets up the limits for the physical paramaters. Most often used to define the default. DEPRECIATED

	if (minLength>maxLength) {std::cout << "ERROR in cone constructor: mismatch of length boundaries\n"; return;}
	if (minRin>maxRin) {std::cout << "ERROR in cone constructor: mismatch of rIn boundaries\n"; return;}
	if (minRout>maxRout) {std::cout << "ERROR in cone constructor: mismatch of rOut boundaries\n"; return;}

	type_ = CONEBENDSECTION_TYPE;
	MA_ = NULL; MB_ = NULL; rr_ = 0.0; 
	length_ = 0;
	rIn_ = 0;
	rOut_ = 0;
	bendRadius_ = 0.0;
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

ConeBendSection::ConeBendSection(HornElement *c) { //copy constructor

	if ((*c).type()==CONEBENDSECTION_TYPE) {
		ConeBendSection* a = static_cast<ConeBendSection*>(c);
		
		type_ = CONEBENDSECTION_TYPE;
		MA_ = NULL; MB_ = NULL; rr_ = 0.0; 
		length_ = (*a).length_;
		rIn_ = (*a).rIn_;
		rOut_ = (*a).rOut_;
		bendRadius_ = (*a).bendRadius_;
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
		if ((*a).dimBendMatrices_) dimensionBendMatrices();
	} else std::cout << "ERROR: Trying copy constructor with incompatible elements (OBND)\n";
}

ConeBendSection::~ConeBendSection() {}

void ConeBendSection::print() const {
	if (name_!="") std::cout << "OBND: '" << name_.c_str() << "', length=" << length_ << ", radius=" << rIn_ << ".." << rOut_ << ", bend radius=" << bendRadius_ << " TEMP: " << tempC_ << " LOSS: " << lossF_;
	else std::cout << "OBND: length=" << length_ << ", radius=" << rIn_ << ".." << rOut_ << ", bend radius=" << bendRadius_ << " TEMP: " << tempC_ << " LOSS: " << lossF_;
}

void ConeBendSection::elementInitialise(const double previousRout) {  //DEPRECATED
	length_=GARandomFloat(minLength_, maxLength_);
	if (minRin_>0) rIn_=GARandomFloat(minRin_, maxRin_); //-ve rIn values denote that the radius of this element is dependent on the previous element
	else rIn_ = previousRout;
	rOut_ = GARandomFloat(minRout_, maxRout_);
}

int ConeBendSection::mutate(const float pmut, const float mu, const double previousRout) { //DEPRECATED
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

bool ConeBendSection::split(list<HornElement*> &l, list<HornElement*>::iterator &iter)  { //DEPRECATED
	// Takes list and iterator that contain and point to this element, then splits this into 2 (half-size) elements and modifies the list accordingly with new element before this.
	// Makes no physical change to the instrument.
	// NOTE: does not modify radius boundaries for either element.

	if (!canSplit_) {return false;}
	
	length_ = length_/2;
	minLength_ = minLength_/2;
	maxLength_ = maxLength_/2;
	double halfRadius = (rOut_ + rIn_)/2;
	ConeBendSection* output = new ConeBendSection(*this);
	rIn_ = halfRadius;
	(*output).rOut_ = halfRadius;

	l.insert(iter, output);
	return true;
}

void ConeBendSection::elementCrossover (const HornElement& m, HornElement* s, HornElement* b, const float pcross) { //DEPRECATED
	//this is dad, takes mum and makes crossed-over elements in sis and bro if they exist.
	// Goes through each physical parameter for this element type; children either get mum's or dad's
	//version of that parameter depending on pcross-loaded coin. If canModify_ is false, this simply copies dad into bro and mum into sis.

	const ConeBendSection& mum = (const ConeBendSection &)m;
	ConeBendSection* sis = (ConeBendSection *)s;
	ConeBendSection* bro = (ConeBendSection *)b;

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

double ConeBendSection::findRadius (const double x) const {
	const double gradient=(rOut_ - rIn_)/length_;
	return (gradient*x + rIn_);
}

int ConeBendSection::TlaAccumulate (const double w,int IndFreq, SimpleMatrix *a, const int Ind, vector<int>&mem, dcomp *zt,list<Matrix> *ImpedanceList,vector<int> &IndHole) {
	coneTlaAccumulate(w, 0, length_, rIn_, rOut_, tempC_, lossF_,humidity_, xc_, a);
	return 0;
}


////////////////////////////////////////
//WASILAKIS
int ConeBendSection::MultimodeAccumulate (const double w,int IndFreq, const bool bends, Matrix *a, const int Ind, vector<int>&mem, Matrix *zt,list<Matrix> *ImpedanceList,vector<int> &IndHole) {

	const double logfactor = log(1+BORERESOLUTION_/100);					// bore resolution
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
	r = rnext;
	rnext = r * factor;
	lstep = pos - findPosition((rnext+r)/2);
	pos = pos - lstep;
	cylMultimodeAccumulate(w,pos, rIn_,LOSSY_,bends,tempC_,lossF_, humidity_, xc_, &Mcyl);
	(*a) = Mcyl*(*a);

	return 0;
}


double ConeBendSection::findPosition (const double r) const {
	const double delta=(rOut_ - rIn_);
	return ( (delta == 0) ? 0 : length_ * (r-rIn_)/delta);
}
///////////////////////////////////////







dcomp ConeBendSection::ZorumskiPlaneRadiation(const double w) const {
	return cylZorumskiPlaneRadiation(w, rOut_, tempC_, humidity_, xc_);
}

dcomp ConeBendSection::radiationImpedance(const double w) const {
	//Called if this is the last element in the list and returns relevant radiation impedance
	//This is Levine & Schwinger's model

	return radiationImpedanceLevineCone (w, 0, length_, rIn_, rOut_, tempC_, humidity_, xc_);
}

void ConeBendSection::dimensionBendMatrices() {
	// These are declared static
	dimBendMatrices_=true;
	A_ = Matrix(nModes_, 1);
	B_ = Matrix(nModes_, nModes_);
	C_ = Matrix(nModes_, nModes_);
	K_ = Matrix(nModes_, nModes_);
	Fbend_ = Matrix(nModes_, nModes_);
	G_ = Matrix(nModes_, nModes_);
	Z_ = Matrix(nModes_, nModes_);
	Zprime_ = Matrix(nModes_, nModes_);
	Zstore_ = Matrix(nModes_, nModes_);
	Ztotal_ = Matrix(nModes_, nModes_);
}


//wasilakis
/*
void ConeBendSection::thetaDependence (const double theta, const double k) {
	// Gives values to the theta-dependent, frequency-independent matrices
	// ################Note, this will be called repeatedly for same theta, different w values - figure out a way to solve this! ##################

	const double hIn = rIn_*2.0;
	const double hOut = rOut_*2.0; //####TIDY UP
	const double thetaf=length_/bendRadius_;
	const double hDiff = -(hIn-hOut);
	const double h = hDiff*theta/thetaf + hOut; //theta-dependent bore diameter
	const double R=h/2.0;
	const int sigma = 1; //denotes whether symmetric or antisymmetric modes are used.

	for (int alpha=0; alpha<nModes_; alpha++) {
		const int m=mIndex(alpha, true);
		const int n=nIndex(alpha, true);
		const double j = jn(m, BESSELDERIVZERO[m][n]);
		if (m==0)  A_(alpha,0) = 1.0 / sqrt(M_PI * R*R * j*j);
		else A_(alpha,0) = 1.0 / sqrt((M_PI * R*R*0.5) * (1-m*m/BESSELDERIVZERO[m][n]/BESSELDERIVZERO[m][n]) *j*j);
	}
		
	for ( alpha=0; alpha<nModes_; alpha++) 
	 for (int beta=0; beta<nModes_; beta++) {
		const int m=mIndex(alpha, true); const int n=nIndex(alpha, true); const int mu=mIndex(beta, true); const int nu=nIndex(beta, true);
		const double s = ((sigma==1)?1.0:-1.0); // = delta_sigma_1 - delta_sigma_0
		
			//eqns C3-C5 of FP02 (Wave Motion 36 p157-168)
		//const double I_m_mu = M_PI * ((m==mu)?1.0:0.0) * (1+ s*((m==0)?1.0:0.0)   );
		const double J_m_mu = M_PI_2 * ((abs(m-mu)==1)?1.0:0.0) * (1+ s * (((m==0)?1.0:0.0)+((mu==0)?1.0:0.0)));
		const double H_m_mu = M_PI_2 * ( ((mu-m==1)?1.0:0.0) * (1-s*((m==0)?1.0:0.0)) - ((m-mu==1)?1.0:0.0)*(1+s*((mu==0)?1.0:0.0)));
		  
		// These are the versions from FP04, JASA 116 p1921-1931, divided by kappa to transform from s -> theta
		B_(alpha, beta) = ((alpha==beta)?1.0:0.0)*bendRadius_ -  A_(alpha, 0)*A_(beta, 0)*J_m_mu * leg.Bintegral(0.0, R, m, n, mu, nu);
		C_(alpha, beta) = A_(alpha, 0)*A_(beta, 0)*BESSELDERIVZERO[m][n]/R*J_m_mu * leg.Cintegral_1(0.0, R, m, n, mu, nu)
				- A_(alpha, 0)*A_(beta, 0)*((double)m)*(J_m_mu - H_m_mu)*leg.Cintegral_2(0.0, R, m, n, mu, nu);
	}

	for ( alpha=0; alpha<nModes_; alpha++) K_(alpha, alpha) = k*k - BESSELDERIVZERO[mIndex(alpha, true)][nIndex(alpha, true)] * BESSELDERIVZERO[mIndex(alpha, true)][nIndex(alpha, true)]/R/R;
	Zc_ = RHO(tempC_)*C(tempC_)/M_PI/R/R;
}




void ConeBendSection::Zprime (const double theta, const double k) {
	thetaDependence(theta, k);

	Zstore_ = -J*k*Zc_*B_ - 1.0/J/k/Zc_*Z_*(C_ + K_*B_)*Z_;// + Z_*G_ + (~G_)*Z_;  //(Fbend_-G_)*Z_;	
}
*/




void ConeBendSection::multimodeInputImpedance (const double w, const double Xin, const double Xout, const double Rin, const double Rout, const bool bends, Matrix *Zout) {
	if (!bends && bendwarning == false) {std::cout << "WARNING: bend calculation without nodal-diameter modes\n"; bendwarning = true;}

/*
// WK: needs optimisation (constant R2/R1 -> constant jumpmatrix can be saved with section)
	if (!bends) {std::cout << "WARNING: bend calculation without nodal-diameter modes\n";}
	
	const int p=20;
	int nPoints=p*(int)length_;	if (nPoints==0) nPoints=p;
//int nPoints=5;
	const double lstep = length_/(nPoints);
	const double rstep = (rOut_-rIn_)/(nPoints) ;

	double r=rOut_;
	cylBendMultimodeInputImpedance(w, Zout, lstep/2, r, bendRadius_, LOSSY_, tempC_, humidity_, xc_);

	for (int i=1; i<nPoints; i++) {
		jumpMultimodeInputImpedance(w, &rr_, &A_, &B_, Zout, r-rstep, r, true, tempC_);
		r-=rstep;
		cylBendMultimodeInputImpedance(w, Zout, lstep, r, bendRadius_, LOSSY_, tempC_, humidity_, xc_);
	}
	jumpMultimodeInputImpedance(w, &rr_, &A_, &B_, Zout, rIn_, r, true, tempC_); 
	cylBendMultimodeInputImpedance(w, Zout, lstep/2, rIn_, bendRadius_, LOSSY_, tempC_, humidity_, xc_);
*/


	//wasilakis
	
	const double logfactor = log(1+BORERESOLUTION_/100);					// bore resolution
	const double ratio = Rin/(Rout+eps);
	const double logratio = log(ratio);										// at most 50 slices per cm
	const int nSlices = max(min(50*Xout,floor(abs(logratio)/logfactor)),2.0);	// at least two slices with step between
    const double factor = pow(ratio, 1.0/nSlices);							// no steps at both ends!

	double r=Rout;															// start slicing from end
	double rnext=r * factor;												// next radius
	double pos=findPosition((rnext+r)/2);									// slices have different thicknesses
	double lstep=Xout-pos;													// staircase around straight line!

	cylBendMultimodeInputImpedance(w, Zout, lstep, r, bendRadius_, LOSSY_, tempC_, lossF_, humidity_, xc_);

	for (int i=1; i<nSlices; i++) {
		jumpMultimodeInputImpedance(w, &rr_, &MA_, &MB_, Zout, rnext, r, bends, tempC_);
		r = rnext;
		rnext=r * factor;
		lstep=pos-findPosition((rnext+r)/2);				// find again average!
		pos=pos-lstep;
		cylBendMultimodeInputImpedance(w, Zout, lstep, r, bendRadius_, LOSSY_, tempC_, lossF_, humidity_, xc_);
	}
	jumpMultimodeInputImpedance(w, &rr_, &MA_, &MB_, Zout, rnext, r, bends, tempC_);

	cylBendMultimodeInputImpedance(w, Zout, lstep, r, bendRadius_, LOSSY_, tempC_, lossF_, humidity_, xc_);




	/*
	// Uses Runge-Kutta algorithm to calculate input impedance from the Riccati equation.
	
	// Never quite got this to give sensible answers; was not a necessary part of PhD work anyway, so abandoned
	
//	if (w/2/M_PI > 11.0) return;
	//	std::cout << w/2/M_PI << endl;
	if (!dimBendMatrices_) 	dimensionBendMatrices();	
	const double k=w/C;
	const int nSteps=2000;
	const double thetaf=-length_/bendRadius_;
	const double step=-thetaf/nSteps;
	const dcomp cstep = dcomp(step, 0.0); //saves conversion from double to dcomp for Matrix *= operator
	//	thetaDependence(0.0, k);
	Z_ = (*Zout);	

	for (double theta=0.0; theta>=thetaf; theta-=step) {
		Zprime(theta, k); //puts result in Zstore_ - this is k1
		Ztotal_ = cstep/6.0 * Zstore_; // keeping a running total 
//		std::cout << Z_ << Zstore_ << Ztotal_ << endl ;
		
		Z_ += cstep/2.0 * Zstore_;
		Zprime(theta+step/2.0, k); //this is k2
		Ztotal_ += cstep/3.0 * Zstore_;
//		std::cout << Z_ << Zstore_ << Ztotal_<< endl ;
		
		Z_ = (*Zout) + cstep/2.0 * Zstore_;
		Zprime(theta+step/2.0, k); //this is k3
		Ztotal_ += cstep/3.0 * Zstore_;
//		std::cout << Z_ << Zstore_ << Ztotal_<< endl ;
		
		Z_ = (*Zout) + cstep * Zstore_;
		Zprime(theta+step, k); //this is k4
		Ztotal_ += cstep/6.0 * Zstore_;
//		std::cout << Z_ << Zstore_ << Ztotal_<< endl ;
    
    		Z_ = (*Zout) + Ztotal_;
		(*Zout) = Z_; //stores Z_n into Zout ready for the next iteration
	//	std::cout << Z_;
	}
//	std::cout << w/2/M_PI << "," << Z_;
	//for (int alpha=0; alpha<nModes_; alpha++) for (int beta=0; beta<nModes_; beta++) (*Zout)(alpha,beta)=conj(Z_(alpha,beta));	
	
	*/
}

void ConeBendSection::multimodeInputImpedance (const double w, const bool bends, Matrix *Zout) {
	multimodeInputImpedance (w, 0, length_, rIn_, rOut_, bends, Zout);	
}

void ConeBendSection::multimodeRadiationImpedance(const double w, const bool bends, const int IndFreq, Matrix* result) const {
	if (!bends && bendwarningrad == false) {std::cout << "WARNING: bend radiation calculation without nodal-diameter modes\n"; bendwarningrad = true;}
	//wasilakis
	//bendRadiationImpedanceInfiniteCylPipe(w, result, rOut_*2);
	radiationImpedanceZorumski(w, result, rOut_, bends, tempC_, humidity_, xc_);
}

void ConeBendSection::multimodeRadiationImpedanceClosedEnd(const double w, const bool bends, Matrix* result) const {
	if (!bends && bendwarningrad == false) {std::cout << "WARNING: bend radiation calculation without nodal-diameter modes\n"; bendwarningrad = true;}

	radiationImpedanceTerminated(w, result, rOut_, bends, tempC_, humidity_, xc_, cinfinity);	
}

void ConeBendSection::multimodeRadiationImpedanceOpenEnd(const double w, const bool bends, Matrix* result) const {
	if (!bends && bendwarningrad == false) {std::cout << "WARNING: bend radiation calculation without nodal-diameter modes\n"; bendwarningrad = true;}

	radiationImpedanceZorumski(w, result, rOut_, bends, tempC_, humidity_, xc_);	
}

void ConeBendSection::multimodeRadiationImpedanceTerminated(const double w, const bool bends, Matrix* result, const dcomp z_trm) const {
	if (!bends && bendwarningrad == false) {std::cout << "WARNING: bend radiation calculation without nodal-diameter modes\n"; bendwarningrad = true;}
	radiationImpedanceTerminated(w, result, rOut_, bends, tempC_, humidity_, xc_, z_trm);	
}


void ConeBendSection::bendRadiationImpedanceInfiniteCylPipe(const double w, Matrix* result, const double radius) const {
	//Infinite cylindrical pipe termination (multimodal)
	const double k =w/C2(tempC_,humidity_,xc_);  
	double kt2;
	dcomp factor;
 
	for (int alpha=0; alpha<nModes_; alpha++)  {
		const int m=mIndex(alpha, true); const int n=nIndex(alpha, true);

		kt2 = BESSELDERIVZERO[m][n]*BESSELDERIVZERO[m][n]/radius/radius;
		if ((k*k - kt2)>=0.0) 	factor = sqrt(k*k - kt2);
		else 	factor = dcomp(0.0, -sqrt(kt2-k*k));
		(*result)(alpha,alpha) = RHO2(tempC_,humidity_)*C2(tempC_,humidity_,xc_)/M_PI/radius/radius *  k/factor;
	} 
}

string ConeBendSection::getSaveString() const {
	char buffer[32];
	string output = CONEBENDSECTION_TYPE_STR + ", ";

	#if defined(_MSC_VER)
	_gcvt(length_, 10, buffer);
	output = output + buffer + ", ";
	_gcvt(rIn_, 10, buffer);
	output = output + buffer + ",";
	_gcvt(rOut_, 10, buffer);
	output = output + buffer + ",";
	_gcvt(bendRadius_, 10, buffer);
	output = output + buffer + ",";

	#elif defined(__GNUG__)
	//g++ doesn't have _gcvt built-in  
	sprintf(buffer,"%f",length_);	
	output = output + buffer + ", ";
	sprintf(buffer,"%f",rIn_);	
	output = output + buffer + ", ";
	sprintf(buffer,"%f",rOut_);	
	output = output + buffer + ", ";
	sprintf(buffer,"%f",bendRadius_);	
	output = output + buffer + ", ";
	#endif

	output = output + name_;
	return output;	
}
