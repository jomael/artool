#include "Branch.h"

//###############BRANCH ELEMENT############
using namespace ART;
Branch::Branch(HornElement *c) { //copy constructor
	if ((*c).type()==BRANCH_TYPE) {
		Branch* a = static_cast<Branch*>(c);

		type_ = BRANCH_TYPE;
		MA_ = NULL; MB_ = NULL; rr_ = 0.0; 
		//Sadjad: The copy constructor seemed incomplete. I have added these:
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
		NbHole_ = (*a).NbHole_;
		rIn_ = (*a).rIn_;
		rOut_ = (*a).rOut_;
		SimpleModel_ = (*a).SimpleModel_;

	} else std::cout << "ERROR: Trying copy constructor with incompatible elements (CYLR)" << endl;
}

Branch::~Branch() {}


int Branch::mutate(const float pmut, const float mu, const double previousRout) {
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
	else if (GAFlipCoin(pmut)) { //RadiationRadius_
			counter++;
			radius_ += GARandomFloat (-radius_*mu, radius_*mu);
			if (radius_>radius_) radius_=maxRadius_;
			if (radius_<radius_) radius_=minRadius_;
		}

	return counter;
}

void Branch::elementCrossover (const HornElement& m, HornElement* s, HornElement* b, const float pcross) {
	//(*this) is dad, takes mum and makes crossed-over elements in sis and bro if they exist.
	// Goes through each physical parameter for this element type; children either get mum's or dad's
	//version of that parameter depending on pcross-loaded coin. If canModify_ is false, this simply copies dad into bro and mum into sis.*/

	const Branch& mum = (const Branch &)m;
	Branch* sis = (Branch *)s;
	Branch* bro = (Branch *)b;

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

void Branch::PutHoleParam(const double length_Hole, const double radius_Hole){
	length_ = 2*radius_Hole;
	length_Hole_ = length_Hole;
	radius_Hole_ = radius_Hole;
	if(!SimpleModel_){//In the more complete model, we calculate the impedance corrections
		ImpLengthCorrection();
		ImpEltsAcoustic();
	}
}

void Branch::PutRoutParam(const double rOut){rOut_ = rOut;}
void Branch::BranchImpedanceList(list <Matrix> *ImpListe) 
{
	BranchImpedanceList_ = ImpListe;
/*	//Sadjad:
	std::cout << "\n(LISTE) ";
	list <Matrix>::iterator it;
	for ( it=BranchImpedanceList_->begin() ; it != BranchImpedanceList_->end(); it++ )
		std::cout << "\n\n " << *it;
	std::cout << "\n\n(ENDE) ";
*/
}

void Branch::ImpLengthCorrection() {

	double Lb, w, La_open, La_close;
	double delta = radius_Hole_/rIn_;
	double tm = (radius_Hole_*delta)*(1+0.207*delta*delta*delta)/8;
	//double Epsilon = tm/length_Hole_;
	double S = M_PI * rIn_ * rIn_; 
	//double SPost = M_PI * rOut_ * rOut_;
	double Shole = M_PI * radius_Hole_ * radius_Hole_;
	//double VHole = Shole*(length_Hole_);
	double lb = (radius_Hole_)*(8/(3*M_PI) - 0.193*delta - 1.09*delta*delta + 1.27*delta*delta*delta - 0.71*delta*delta*delta*delta);
	double la_open = radius_Hole_*delta*delta/(1.78*tanh(1.84*(tm+length_Hole_)/radius_Hole_) - 0.940 + 0.540*delta + 0.285*delta*delta);
	double la_close = radius_Hole_*delta*delta/(1.78*(1/tanh(1.84*(tm+length_Hole_))/radius_Hole_) - 0.940 + 0.540*delta + 0.285*delta*delta);

	//Length corrections
	La_open = -RHO2(tempC_,humidity_)*la_open/S; //main tube with opened hole
	La_close = -RHO2(tempC_,humidity_)*la_close/S; //main tube with closed hole
	Lb = RHO2(tempC_,humidity_)*lb/Shole; //hole

	if(Freq.size()==0){
		for(double f=F_MIN_; f<=F_MAX_; f+=D_F_){
			w = 2.0*M_PI*f;
			Za_open.push_back(dcomp(J*w*La_open)); //impedance correction: main tube, opened hole
			Za_close.push_back(dcomp(J*w*La_close)); //closed hole
			Zb_.push_back(dcomp(J*w*Lb)); //impedance correction: hole
		}
	}
	else{
		for(size_t k =0; k< Freq.size(); k++){
			double f = Freq[k];
			w = 2.0*M_PI*f;
			Za_open.push_back(dcomp(J*w*La_open)); //impedance correction: main tube, opened hole
			Za_close.push_back(dcomp(J*w*La_close)); //closed hole
			Zb_.push_back(dcomp(J*w*Lb)); //impedance correction: hole
		}
	}			
}

void Branch::ImpEltsAcoustic() {
	//corrections for the change of sections S_hole->S_main_tube
	double Shole = M_PI * radius_Hole_ * radius_Hole_ ;	
	double delta = radius_Hole_/rIn_;
	Tm_ = (radius_Hole_*delta)*(1+0.207*delta*delta*delta)/8;
	Cm_ = (Tm_*Shole)/(RHO2(tempC_,humidity_)*C2(tempC_,humidity_,xc_)*C2(tempC_,humidity_,xc_));
	Lm_ = (Tm_*RHO2(tempC_,humidity_))/Shole;
}

Matrix Branch::GetParallelImpedance(const double w, const int IndFreq) {
// returns Matrix Ztm, of size nModes*nModes so that the multimodal case is also covered,
// but since only plane-wave propagation is modelled at the branch, only Ztm(0,0) is non-zero.
	Matrix Zt = Matrix(1,1);
	Matrix Ztm = Matrix(nModes_,nModes_);
	Matrix Yt = Matrix(1,1);

	list<Matrix>::iterator iter;
	iter = (BranchImpedanceList_)->begin();

	if(SimpleModel_){ //parallel calculation
		iter++;
		for(size_t i=0;i<(BranchImpedanceList_)->size()-1;i+=2){
			Yt = Yt + (!(*iter));
			iter++;
			iter++;
		}
		Zt = !Yt;
		//return Zt;
		Ztm(0,0) = Zt(0,0);
		return(Ztm);
	}

	else{//+ take into account the corrections impedance
		//Z1 = hole(s), Z2 = main tube

		int Ind = 0;
		if(IndFreq==-1){const double f = w/(2.0*M_PI);Ind = floor(f-F_MIN_+0.5)/D_F_;}
		else Ind = IndFreq;

		Matrix Z1 = Matrix(1,1);
		Matrix Z2 = Matrix(1,1);
		Matrix Lm = Matrix(1,1);
		Matrix Cm = Matrix(1,1);
		Matrix Za_op = Matrix(1,1);
		Matrix Za_cl = Matrix(1,1);
		Matrix Ones = Matrix(1,1);
		Ones(0,0) = dcomp(1);
		Lm(0,0) = J*w*Lm_;
		Cm(0,0) = J*w*Cm_;
		Za_op(0,0) = Za_open[Ind]/dcomp(2);
		Za_cl(0,0) = Za_close[Ind]/dcomp(2);

		int NbHole = abs((*iter)(0,0)); 
		iter++;


		for(int i=0;i<NbHole;i++){
			Z1 = (((*iter)+Lm) / (Ones+Cm*(*iter)+Lm*Cm));
			iter ++;
			if((*iter)(0,0)!=dcomp(10)) Z1(0,0) += Zb_[Ind];
			Yt += !Z1;
			iter++;
		}
		if((*iter)(0,0)!=dcomp(10)) Z2 = *iter + Za_op;
		else Z2 = *iter + Za_cl;
		Yt += !Z2;
		Zt = !Yt;
		if((*iter)(0,0)!=dcomp(10))Zt += Za_op;
		else Z2 += Za_cl;
		//return Zt;

		Ztm(0,0) = Zt(0,0);
		return(Ztm);

	}

	return 0;
}

Matrix Branch::GetImpedance(const double w, const int IndFreq) {
//The same idea but we are going through the instrument in the other direction!
	Matrix Zt = Matrix(1,1);
	Matrix Yt = Matrix(1,1);
	list<Matrix>::iterator iter;
	iter = (BranchImpedanceList_)->begin();

	if(SimpleModel_){
		iter++;
		for(size_t i=0;i<(BranchImpedanceList_)->size()-1;i+=2){
			if(i==(BranchImpedanceList_)->size()-2){Yt = Yt + (!(*iter));}
			else {Yt = Yt - (!(*iter));}		
			iter++;
			iter++;
		}
		Zt = !Yt;
		return Zt;
	}

	else{
		int Ind = 0;
		if(IndFreq==-1){const double f = w/(2.0*M_PI);Ind = floor(f-F_MIN_+0.5)/D_F_;}
		else Ind = IndFreq;
		Matrix Z1 = Matrix(1,1);
		Matrix Z2 = Matrix(1,1);
		Matrix Lm = Matrix(1,1);
		Matrix Cm = Matrix(1,1);
		Matrix Za_op = Matrix(1,1);
		Matrix Za_cl = Matrix(1,1);
		Matrix Ones = Matrix(1,1);
		Ones(0,0) = dcomp(1);
		Lm(0,0) = J*w*Lm_;
		Cm(0,0) = J*w*Cm_;
		Za_op(0,0) = Za_open[Ind]/dcomp(2);
		Za_cl(0,0) = Za_close[Ind]/dcomp(2);	
		int NbHole = abs((*iter)(0,0)); 
		iter++;
		
		for(int i=0;i<NbHole;i++){
			Z1 = (((*iter)+Lm) / (Ones+Cm*(*iter)+Lm*Cm)); 
			iter ++;
			if((*iter)(0,0)!=dcomp(10)){
				Z1(0,0) += Zb_[Ind];
			}
			Yt -= !Z1;
			iter++;
		}
		if((*iter)(0,0)!=dcomp(10)){Z2 = *iter - Za_op;}
		else{Z2 = *iter - Za_cl;}
		Yt += !Z2;
		Zt = !Yt;
		if((*iter)(0,0)!=dcomp(10)){Zt -= Za_op;}
		else{Z2 -= Za_cl;}
		return Zt;
	}

	return 0;
}
