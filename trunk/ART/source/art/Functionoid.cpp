
#ifndef ARTFUNCTIONOID_CPP
#define ARTFUNCTIONOID_CPP

#include "Functionoid.h"
#include "DataContainer.h"

#include "ARTlink.h"

using namespace ART;
void Functionoid::SetDependencies()
{
	//add all registered input parameters as dependency to output
	vector<DataContainer*>::iterator it;
	for ( it=in_.begin() ; it < in_.end(); it++ )
		out_->AddDependency(*it);
}

Functionoid::~Functionoid() {}


bool ARTfrqGridFunc::IsSameFunctionoid(Functionoid* f)
{
	ARTfrqGridFunc* mf = dynamic_cast<ARTfrqGridFunc*>(f);
	if (!mf) return false;
	if (mf->lowfrq_ != lowfrq_) return false;
	if (mf->highfrq_ != highfrq_) return false;
	if (mf->frqStep_ != frqStep_) return false;
	return true;
}

void ARTfrqGridFunc::ApplyFunction()
{
//	if (DataContainer* cd = dynamic_cast<DataContainer*>(out_))
//		cout << "\n" << out_->GetVarName();// << ":\n";

	double fmax = highfrq_->GetFloat();
	double fmin = lowfrq_->GetFloat();
	double fstep = frqStep_->GetFloat();
	//calculate length of frq array
	int flen = (fmax - (fmin + fstep*(fmin==0.0))) / fstep + 1;
/*
	cout << "fmin= " << fmin << " ";
	cout << "fmax= " << fmax << " ";
	cout << "fstep= " << fstep << " ";
	cout << "flen= " << flen << "\n";
*/
	out_->SetType(C_ART_ndbl);
	out_->SetArrayLength(flen);
	
	double f;
	int j;
	for (f=fmin + fstep*(fmin==0.0), j = 0; j<flen; f+=fstep, j++) 
	{

    
		DataContainer::progressIndicator.Continue(out_->GetComplexity(),out_->GetVarName());
		out_->val->nd[j] = f;  

	}
}

int ARTfrqGridFunc::GetIterationNumber()
{
	double fmax = highfrq_->GetFloat();
	double fmin = lowfrq_->GetFloat();
	double fstep = frqStep_->GetFloat();
	//cout << " frq" << (fmax - (fmin + fstep*(fmin==0.0))) / fstep << "\n";
	//calculate length of frq array
	return (fmax - (fmin + fstep*(fmin==0.0))) / fstep + 1;
}

//**********************************************************************************************************

//functionoid as wrapper for wfrq-Grid
/*void ARTwfrqGridFunc::SetDependencies()
{
	out_->AddDependency(frequencies_);
}*/

bool ARTwfrqGridFunc::IsSameFunctionoid(Functionoid* f)
{
	ARTwfrqGridFunc* mf = dynamic_cast<ARTwfrqGridFunc*>(f);
	if (!mf) return false;
	if (mf->frequencies_ != frequencies_) return false;
	return true;
}

void ARTwfrqGridFunc::ApplyFunction()
{
	out_->SetType(C_ART_ndbl);
	out_->SetArrayLength(frequencies_->len);

	//convert Hz in angular frequency (Kreisfrequenz)
	for (int j=0; j<frequencies_->len; j++) 
	{
		DataContainer::progressIndicator.Continue(out_->GetComplexity(),out_->GetVarName());
		out_->val->nd[j] = 2.0*M_PI*frequencies_->val->nd[j];
	}
}

int ARTwfrqGridFunc::GetIterationNumber()
{
	return frequencies_->GetIterationNumber();
}

#endif

