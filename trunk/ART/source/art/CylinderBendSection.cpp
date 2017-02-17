#include "CylinderBendSection.h"
//#######CYLINDERBEND##########
using namespace ART;
Matrix CylinderBendSection::K_ = Matrix(nModes_, nModes_);
Matrix CylinderBendSection::BCKB_ = Matrix(nModes_, nModes_);
Matrix CylinderBendSection::H_ = Matrix(nModes_, nModes_);
Matrix CylinderBendSection::Dbend_ = Matrix(nModes_, nModes_);
Matrix CylinderBendSection::X_ = Matrix(nModes_, nModes_);
Matrix CylinderBendSection::N_ = Matrix(nModes_, nModes_);
Matrix CylinderBendSection::E_ = Matrix(nModes_, nModes_);
Matrix CylinderBendSection::nu_ = Matrix(nModes_, 1);
Matrix CylinderBendSection::HXN_ = Matrix(nModes_, nModes_);
Matrix CylinderBendSection::F_ = Matrix(nModes_, nModes_);

//Sadjad: override to resize matrices according to number of modes
void CylinderBendSection::setnModes(const int nModes)
{
	if (nModes_!=nModes) 
	{
		nModes_=nModes; 
		if (nModes>0) initMatrices();
	};
	//The following added by Sadjad (July 2011):
	//The global static variable nModes_ used to initialise these matrices in the beginning is not the same
	//as nModes_ belonging to this object - so if nModes == nModes_ does not mean anything for these matrices.
	//-> just resize them (Find a better way if you want...)
	K_.Null(nModes_, nModes_);
	BCKB_.Null(nModes_, nModes_);
	H_.Null(nModes_, nModes_);
	Dbend_.Null(nModes_, nModes_);
	X_.Null(nModes_, nModes_);
	N_.Null(nModes_, nModes_);
	E_.Null(nModes_, nModes_);
	nu_.Null(nModes_, 1);
	HXN_.Null(nModes_, nModes_);
	F_.Null(nModes_, nModes_);

	//resize bend matrices and recalculate their initial values according to new number of modes
	//initBendMatrices(); //Sadjad: Do we have to call this function BEFORE calculating the impedance?? It seems no...
	hasBendMatrices_ = false;
}

CylinderBendSection::CylinderBendSection(HornElement *c) { //copy constructor
	if ((*c).type()==CYLINDERBENDSECTION_TYPE) {
		CylinderBendSection* a = static_cast<CylinderBendSection*>(c);

		type_ = CYLINDERBENDSECTION_TYPE;
		MA_ = NULL; MB_ = NULL; rr_ = 0.0; 
		radius_ = (*a).radius_;
		bendRadius_ = (*a).bendRadius_;
		tempC_ = (*a).tempC_;
		lossF_ = (*a).lossF_;
		length_ = (*a).length_;
		minLength_ = (*a).minLength_;
		maxLength_ = (*a).maxLength_;
		minRadius_ = (*a).minRadius_;
		maxRadius_ = (*a).maxRadius_;
		canModify_ = (*a).canModify_;
		canSplit_ = (*a).canSplit_;
		name_ = (*a).name_;
		hasBendMatrices_ = (*a).hasBendMatrices_;
		if (hasBendMatrices_) {
			A_ = (*a).A_;
			B_ = (*a).B_;  
			C_ = (*a).C_;
		}
		dimBendMatrices_ = (*a).dimBendMatrices_;
	} else std::cout << "ERROR: Trying copy constructor with incompatible elements (CBND)" << endl;
}

CylinderBendSection::~CylinderBendSection() {}

void CylinderBendSection::print() const {
	if (name_ != "") std::cout << "CBND: '" << name_.c_str() << "', "; 
	else std::cout << "CBND: ";
	std::cout << "length=" << length_ << ", radius=" << radius_ << ", bend radius=" << bendRadius_ << " TEMP: " << tempC_ << " LOSS: " << lossF_;
}


int CylinderBendSection::TlaAccumulate (const double w,int IndFreq, SimpleMatrix *a, const int Ind, vector<int>&mem, dcomp *zt,list<Matrix> *ImpedanceList,vector<int> &IndHole) {
	cylTlaAccumulate(w, length_, radius_, tempC_, lossF_, humidity_, xc_, a);
	return 0;
}

dcomp CylinderBendSection::radiationImpedance(const double w) const {
	//Called if this is the last element in the list and returns relevant radiation impedance
	//This is Levine & Schwinger's model

	return radiationImpedanceLevineCyl (w, radius_, tempC_, humidity_, xc_);
}

//##### HIGHER MODES #####

void CylinderBendSection::initBendMatrices() {
	// Defines frequency-independent matrices A,B,C - need only be calculated once for each bend shape

	hasBendMatrices_=true;
	A_ = Matrix(nModes_,1);
	B_ = Matrix(nModes_,nModes_);  	
	C_ = Matrix(nModes_,nModes_);
	const int sigma = 1; //denotes whether symmetric or antisymmetric modes are used.
	
	leg = LeG_quadrature(nQuadraturePoints);
	//double (CylinderBendSection::*fp)(double)=&CylinderBendSection::Bintegral;

    //declare for-loop-var as local var to resolve VC compatibility issues...
    int alpha;
	
	for (alpha=0; alpha<nModes_; alpha++) {
		const int m=mIndex(alpha, true);
		const int n=nIndex(alpha, true);
		const double j = jn(m, BESSELDERIVZERO[m][n]);
		if (m==0)  A_(alpha,0) = 1.0 / sqrt(M_PI * radius_*radius_ * j*j);
		else A_(alpha,0) = 1.0 / sqrt((M_PI * radius_*radius_*0.5) * (1-m*m/BESSELDERIVZERO[m][n]/BESSELDERIVZERO[m][n]) *j*j);
	}
		
	for (alpha=0; alpha<nModes_; alpha++) 
	 for (int beta=0; beta<nModes_; beta++) {
		const int m=mIndex(alpha, true); const int n=nIndex(alpha, true); const int mu=mIndex(beta, true); const int nu=nIndex(beta, true);
		const double s = ((sigma==1)?1.0:-1.0); // = delta_sigma_1 - delta_sigma_0
		
			//eqns C3-C5 of FP02 (Wave Motion 36 p157-168)
	//	const double I_m_mu = M_PI * ((m==mu)?1.0:0.0) * (1+ s*((m==0)?1.0:0.0)   );
		const double J_m_mu = M_PI_2 * ((abs(m-mu)==1)?1.0:0.0) * (1+ s * (((m==0)?1.0:0.0)+((mu==0)?1.0:0.0)));
		const double H_m_mu = M_PI_2 * ( ((mu-m==1)?1.0:0.0) * (1-s*((m==0)?1.0:0.0)) - ((m-mu==1)?1.0:0.0)*(1+s*((mu==0)?1.0:0.0)));
		  
		// These are the versions from FP04, JASA 116 p1921-1931, divided by kappa to transform from s -> theta
		B_(alpha, beta) = ((alpha==beta)?1.0:0.0) -  A_(alpha, 0)*A_(beta, 0)*J_m_mu * leg.Bintegral(0.0, radius_, m, n, mu, nu)/bendRadius_;

		C_(alpha, beta) = A_(alpha, 0)*A_(beta, 0)*BESSELDERIVZERO[m][n]/radius_*J_m_mu * leg.Cintegral_1(0.0, radius_, m, n, mu, nu)/bendRadius_
				- A_(alpha, 0)*A_(beta, 0)*((double)m)*(J_m_mu - H_m_mu)*leg.Cintegral_2(0.0, radius_, m, n, mu, nu)/bendRadius_;
		 
	}

	if (!dimBendMatrices_) 	dimensionBendMatrices();	
}

void CylinderBendSection::multimodeRadiationImpedance(const double w, const bool bends, const int IndFreq, Matrix* result) const {
	/*std::cout << "CylinderBendSection::multimodeRadiationImpedance:\n";
	std::cout << " w=" << w << "\n";
	std::cout << " bends=" << bends << "\n";
	std::cout << " result=" << *result << "\n";*/
	if (!bends && bendwarningrad == false) {std::cout << "WARNING: bend radiation calculation without nodal-diameter modes\n"; bendwarningrad = true;}

	radiationImpedanceZorumski(w, result, radius_, bends, tempC_, humidity_, xc_);
}

void CylinderBendSection::multimodeRadiationImpedanceClosedEnd(const double w, const bool bends, Matrix* result) const {
	if (!bends && bendwarningrad == false) {std::cout << "WARNING: bend radiation calculation without nodal-diameter modes\n"; bendwarningrad = true;}

	radiationImpedanceTerminated(w, result, radius_, bends, tempC_, humidity_, xc_, cinfinity);	
}

void CylinderBendSection::multimodeRadiationImpedanceOpenEnd(const double w, const bool bends, Matrix* result) const {
	if (!bends && bendwarningrad == false) {std::cout << "WARNING: bend radiation calculation without nodal-diameter modes\n"; bendwarningrad = true;}

	radiationImpedanceZorumski(w, result, radius_, bends, tempC_, humidity_, xc_);
}

void CylinderBendSection::multimodeRadiationImpedanceTerminated(const double w, const bool bends, Matrix* result, const dcomp z_trm) const {
	if (!bends && bendwarningrad == false) {std::cout << "WARNING: bend radiation calculation without nodal-diameter modes\n"; bendwarningrad = true;}

	radiationImpedanceTerminated(w, result, radius_, bends, tempC_, humidity_, xc_, z_trm);	
}

int CylinderBendSection::MultimodeAccumulate (const double w, int IndFreq, const bool bends, Matrix *a, const int Ind, vector<int>&mem, Matrix *zt,list<Matrix> *ImpedanceList,vector<int> &IndHole) {

cylBendMultimodeAccumulate (w, length_, radius_, bendRadius_, lossF_, tempC_, humidity_, xc_, a);
	
	return 0;
}

dcomp CylinderBendSection::ZorumskiPlaneRadiation(const double w) const {

	return cylZorumskiPlaneRadiation(w, radius_, tempC_, humidity_, xc_);
}

void CylinderBendSection::multimodeInputImpedance (const double w, const bool bends, Matrix *Zout) {
/*
	std::cout << "CylinderBendSection::multimodeInputImpedance:\n";
	std::cout << " w=" << w << "\n";
	std::cout << " bends=" << bends << "\n";
	std::cout << " zout=" << *Zout << "\n";
*/
	if (!bends && bendwarning == false) {std::cout << "WARNING: bend calculation without nodal-diameter modes\n"; bendwarning = true;}
	
	if (!hasBendMatrices_) initBendMatrices();
//	const double thetaf = -length_/bendRadius_;
//	const double sf=-length_;
	const double sf= -length_;
	const double Zc = RHO2(tempC_,humidity_)*C2(tempC_,humidity_,xc_)/M_PI/radius_/radius_;
	const double k=w/C2(tempC_,humidity_,xc_);


	//wasilakis
	if(!LOSSY_)
	for (int alpha=0; alpha<nModes_; alpha++) K_(alpha, alpha) = k*k - BESSELDERIVZERO[mIndex(alpha, bends)][nIndex(alpha, bends)] * BESSELDERIVZERO[mIndex(alpha, bends)][nIndex(alpha, bends)]/radius_/radius_;
	else{
	
	//wasilakis: lossy kn (can be written as separate function returnig kn2 = kn^2)
	dcomp kn2;
	const double k = w/C2(tempC_, humidity_,xc_);
	const double kk = k*k;
	const double kkRR = kk * radius_ * radius_;
	const dcomp lf = dcomp(1/lossF_/lossF_/lossF_,0);					// WK: compensating loss frequency characteristics
	double cof2; //cut-off frequency
	double root_f = (abs(lossF_-1) < eps) ? sqrt(w/2/M_PI) : pow(w/2/M_PI,static_cast<double>(lossF_/2)); 	// WK: loss with more than root_f!

	for (int i=0; i<nModes_; i++) {
		const double gammai2 = GAMMA_I2[i];				//square of gamma_i
		cof2 = BESSELDERIVZERO[mIndex(i, bends)][nIndex(i, bends)]/radius_; cof2=cof2*cof2;

		
			const dcomp eps_v = dcomp(2.03e-5 * root_f, 2.03e-5 * root_f);	//[Kemp, section 2.4.1]
			const dcomp eps_t = dcomp(0.95e-5 * root_f, 0.95e-5 * root_f);
			dcomp eps_i = lf*((1-gammai2/kkRR)*eps_v + eps_t); // WK extra loss dcomp(L,0)!		// boundary specific admittance
			kn2 = kk - cof2 + 2*k/radius_*dcomp(eps_i.imag(), -eps_i.real()); 	//lossy kn, any sign!
			//if (kn[i].real() < 0) kn[i] = -kn[i];		// WK: corrected certain cases - enforce positive real part
			//if (kn[i].imag() > 0) kn[i] = conj(kn[i]);	// WK: corrected certain cases - enforce negative imag to avoid exponentially increased amplitude
		
	K_(i, i) = kn2;
	}
	}


	BCKB_ = B_*(C_ + K_*B_);

	H_ = 1.0/J/k/Zc * (C_+K_*B_);
	BCKB_.eig(X_, nu_);

	for (int n=0; n<nModes_; n++)  {
		if (fabs(nu_(n,0).imag())<1e-10) nu_(n,0) = dcomp(nu_(n,0).real(),  0.0); // counters a numerical glitch in eig that gives small complex components to real numbers
		nu_(n,0) = sqrt(nu_(n,0));
		Dbend_(n,n) = exp(-J*nu_(n,0)*sf);  //This is D from the derivation
		N_(n,n) = 1.0/J/nu_(n,0);
	}

	HXN_ = H_*X_*N_;
	E_ = Dbend_ * !((*Zout)*HXN_ + X_) * (X_ - (*Zout)*HXN_) * Dbend_;

	(*Zout) = (X_ - X_*E_) * !(HXN_ + HXN_*E_);	//##cf thesis




// The following are alternative (equivalent) formulations.

//	E_ = Dbend_ * !((*Zout)*HXN_ + X_) * ((*Zout)*HXN_ - X_) * Dbend_;
//	(*Zout) = (X_ + X_*E_) * !(HXN_ - HXN_*E_);

//	E_ = Dbend_ * !(X_ - (*Zout)*HXN_) * (X_ + (*Zout)*HXN_) * Dbend_;
//	(*Zout) = (X_*E_ - X_) * !(HXN_ + HXN_*E_);

//	E_ = Dbend_ * !((*Zout)*HXN_ - X_) * (X_ + (*Zout)*HXN_) * Dbend_;
//	(*Zout) = (X_ + X_*E_) * !(-HXN_ + HXN_*E_);

}

void CylinderBendSection::bendRadiationImpedanceInfiniteCylPipe(const double w, Matrix* result, const double radius) const {
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

//### END HIGHER MODES ###

string CylinderBendSection::getSaveString() const {
	char buffer[32];
	string output = CYLINDERBENDSECTION_TYPE_STR + ", ";
	
	#if defined(_MSC_VER)
	_gcvt(length_, 10, buffer);
	output = output + buffer + ", ";
	_gcvt(radius_, 10, buffer);
	output = output + buffer + ", ";
	_gcvt(bendRadius_, 10, buffer);
	output = output + buffer + ",";

	#elif defined(__GNUG__)
	//g++ doesn't have _gcvt built-in  
	sprintf(buffer,"%f",length_);	
	output = output + buffer + ", ";
	sprintf(buffer,"%f",radius_);	
	output = output + buffer + ", ";
	sprintf(buffer,"%f",bendRadius_);	
	output = output + buffer + ", ";
	#endif

	output = output + name_;
	return output;	
}


//## Deprecated GA functions - copied from cylinder and not properly written

void CylinderBendSection::elementInitialise(const double previousRout) { //randomly sets physical parameters within their limits.
	length_ = GARandomFloat(minLength_, maxLength_);
	if(minRadius_>0) radius_ = GARandomFloat(minRadius_, maxRadius_); //-ve minRadius values denote that the radius of this element is dependent on the previous element
	else radius_ = previousRout;
}


int CylinderBendSection::mutate(const float pmut, const float mu, const double previousRout) {
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

bool CylinderBendSection::split(list<HornElement*> &l, list<HornElement*>::iterator &iter)  {
	// Takes list and iterator that contain and point to this element, then splits this into 2 identical (half-size) elements and modifies the list accordingly.
	// Makes no physical change to the instrument.

	if (!canSplit_) {return false;}
	
	length_ = length_/2;
	minLength_ = minLength_/2;
	maxLength_ = maxLength_/2;
	CylinderBendSection* output = new CylinderBendSection(*this);
	l.insert(iter, output);
	return true;
}

void CylinderBendSection::elementCrossover (const HornElement& m, HornElement* s, HornElement* b, const float pcross) {
	//(*this) is dad, takes mum and makes crossed-over elements in sis and bro if they exist.
	// Goes through each physical parameter for this element type; children either get mum's or dad's
	//version of that parameter depending on pcross-loaded coin. If canModify_ is false, this simply copies dad into bro and mum into sis.*/

	const CylinderBendSection& mum = (const CylinderBendSection &)m;
	CylinderBendSection* sis = (CylinderBendSection *)s;
	CylinderBendSection* bro = (CylinderBendSection *)b;

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
