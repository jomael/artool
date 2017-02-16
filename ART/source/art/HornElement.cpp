/**************************************************************************
*                                                                         *
*                   Acoustic Research Tool (ART)                          *
*                                                                         *
*   A Simulation Framework with Modelling Library for Acoustic Systems    *
*                                                                         *
*         Project of the Workgroup 2 of the Technical Committee           *
*      Musical Acoustics of the European Acoustics Association EAA)       *
*                                                                         *
*   http://www.eaa-fenestra.org/technical-committees/ma/workgroups/wg2    *
*                                                                         *
*  Copyright (C) 2011 by the authors and their organisations:             *
*    Alistair Braden            1)                                        *
*    Wilfried Kausel            2)         kausel(at)mdw.ac.at            *
*    Delphine Cadefaux          2)                                        *
*    Vasileios Chatziioannou    2)                                        *
*    Sadjad Siddiq              2)                                        *
*                                                                         *
*    1) School of Physics and Astronomy, Univ. of Edinburgh, GB           *
        (http://www.ph.ed.ac.uk/)                                         *
*    2) Inst. of Music Acoustics, Univ. of Music, Vienna, AT              *
        (http://iwk.mdw.ac.at)                                            *
*                                                                         *
*  This program is free software: you can redistribute it and/or modify   *
*  it under the terms of the GNU General Public License as published by   *
*  the Free Software Foundation, either version 3 of the License, or      *
*  any later version.                                                     *
*                                                                         *
*  This program is distributed in the hope that it will be useful,        *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
*  GNU General Public License for more details.                           *
*                                                                         *
*  You should have received a copy of the GNU General Public License      *
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
*                                                                         *
***************************************************************************/

/* This is a class that contains data describing a horn element and encapsulates any methods needed by the GA.
 * HornElement is an abstract base class. Each type of element is then a class inheriting from it and including all necessary code.
 */

#ifndef HORNELEMENT_CPP
#define HORNELEMENT_CPP
#include "HornElement.h"
#include "WindInstrument.h"
#include <float.h>
#include <math.h>

using namespace ART;
//max+min macro def. from VC98/include/windef.h
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif
bool bendwarning = false;
bool bendwarningrad = false;

// ############## TLA CALCULATIONS (i.e. plane-wave assumption) ######################
// series of functions to implement the impedance theory for simple geometries
// Methods themselves require little commenting; see the references for details of the theory.

//Maths sub-functions for TLA Input Impedance calculations

float DT(const float T){ return (T - 26.85);}
double T_Kelvin(const float T) {return(T + 273.15);}


double f(const float T) {return (1.00062+(3.14e-8)*P+(5.6e-7)*T*T);}
double psv(const float T) {double t = T_Kelvin(T); return (exp((1.2811805e-5)*t*t - (1.950987e-2)*t + 34.04926034 - (6.3536311e3)/t));}

double xw(const float T,const float h){return (h*f(T)*psv(T)/P);}

//C2: Speed of sound with Temperature, Pressure, CO2 and water vapor molar fractions, and Humidity dependencies: 
//More Accurate than C. C2 is used in the following code
double C2(const float T, const float h, const float xc)   {
  double  xw_h=xw(T,h);
  return(331.5024 + 0.603055*T - 0.000528*T*T 
	 + (51.471935 + 0.1495874*T - 0.000782*T*T)*xw_h
	 + (-1.82e-7 + (3.73e-8)*T + (-2.93e-10)*T*T)*P
	 + (-85.20931 - 0.228525*T + (5.91e-5)*T*T)*xc
	 -2.835149*xw_h*xw_h + (-2.15e-13)*P*P + 29.179762*xc*xc
	 + 0.000486*xw_h*xc*P)*100;
  // Chadefaux, p. 14 refers to: Cramer, JASA 93
}
//C: Speed of sound with Temperature dependency
double C(const float T)   { return(3.4723e4 * (1+0.00166*DT(T))); } 	//Speed of sound in air in cm/s

//RHO2: take into account more dependency than RHO but there's no big difference between both (RHO2 is used in the code)
double RHO2(const float T, const float h)   {double t=T_Kelvin(T); return((1/(287.06*t))*(P - 230.617*h*exp((17.5043*T)/(241.2+T))))/1000;}
double RHO(const float T) { return(1.1769e-3 * (1-0.00335*DT(T)));}	//Density of ambient air in g/cm^3

float ETA(const float T)  { return(1.846e-4 * (1+0.0025*DT(T))); }		//Shear viscosity in g/(cm s)


double r_v(const double w, const double Sm, const float T, const float L, const float h)  {return (sqrt(RHO2(T,h)*w*Sm/ETA(T)/L/M_PI));}
double r_zero(const double Si, const float T, const float h, const float xc) { return (RHO2(T,h)*C2(T,h,xc)/Si); }
dcomp Z_c(const double w, const double Si, const double Sm, const float T, const float L, const float h, const float xc) {double rvinv=1/r_v(w, Sm, T, L,h); return dcomp(r_zero(Si, T,h,xc)*(1+0.369*rvinv), -r_zero(Si, T,h,xc)*rvinv*(0.369+rvinv*(1.149+0.303*rvinv)));}
dcomp Gamma(const double w, const double Sm, const float T, const float L, const float h, const float xc) {double rvinv=1/r_v(w, Sm, T, L,h); return (dcomp(w/C2(T,h,xc)*rvinv*(1.045+rvinv*(1.080+0.750*rvinv)), w/C2(T,h,xc)*(1+1.045*rvinv)));}


void HornElement::SetImpedanceFile(const char filename[], int RadHole, int Contrib, vector <Matrix> &Mv){
	fstream file(filename);
	if (!file.is_open()) {REPORT_ERROR("ERROR: failed to open the input file");}
	int const arraySize=500;
	string line, tempValues[arraySize];
	bool nearEnd=false; // noErrors=true;
	int Length = 0;
	size_t c = 0;
	Matrix Z;
	if (nModes_==0){Z = Matrix(1,1);}
	else Z = Matrix(nModes_,nModes_);

	while (!file.eof()) { 

		getline(file, line, '\n');
		Length=(int)line.size();
		if (Length>0) if (line[Length-1]=='\r') line[Length-1]='\0'; //Correcting for Windows \r\n endlines.
		if (file.eof()) nearEnd=true; //checks to see if this is the last line in the file
		if (line[0]!=';') { //checks for comments in the file
			int counter=0;
			for(int i=0, itemp=0; i<=Length; i++) {
				if((line[i]==',' || line[i]=='\n' || line[i]=='\0' || line[i]=='\r' || line[i]==' ' || line[i]=='	'||(nearEnd && i==Length)) && i!=itemp) {
					tempValues[counter]=line.substr(itemp, (i-itemp));
					counter++;
					itemp=i+1;
				}
			}
			if(counter!=0){
				if(counter<1+2*Contrib){REPORT_ERROR("ERROR: Problem with columns's number");}
				if(!RadHole){//We are reading an input impedance file
					Freq.push_back((double)atof(tempValues[0].c_str())); //Filling the frequencies axis
					//Filling the input impedance matrix:
					if(Contrib==1){ //Just the PW contribution
						Z.Null();
						Z(0,0) = ((double)atof(tempValues[1].c_str())*exp(J*(double)atof(tempValues[2].c_str())));
						MZ.push_back(Z);
					}
					else if (Contrib<=nModes_){ //All the contributions: Fill the matrix's diagonale
						int i=1;
						int compt=0;
						Z.Null();
						while(i<counter){
							Z(compt,compt) = (double)atof(tempValues[i].c_str())*exp(J*(double)atof(tempValues[i+1].c_str()));
							i+=2;
							compt++;
						}
						MZ.push_back(Z);
					}	
					else{//Take into account the interaction between modes too: Fill all the coefficients
						int k = 1;
						Z.Null();
						for(int j=0;j<nModes_;j++){
							for(int i=0;i<nModes_;i++){Z(j,i) = (double)atof(tempValues[k].c_str())*exp(J*(double)atof(tempValues[k+1].c_str())); k+=2;}
						}
						Mv.push_back(Z);
					}
				}if(RadHole){//We are reading a radiation impedance file
					if(Freq.size()==c) {Freq.push_back((double)atof(tempValues[0].c_str()));}//Filling the frequencies axis
					//Filling the input impedance matrix:
					if(Contrib==1){
						Z.Null();
						Z(0,0) = ((double)atof(tempValues[1].c_str())*exp(J*(double)atof(tempValues[2].c_str())));
						Mv.push_back(Z);
					}
					else if (Contrib<=nModes_){
						int i=1;
						int compt=0;
						Z.Null();
						while(i<Contrib){
							Z(compt,compt) = (double)atof(tempValues[i].c_str())*exp(J*(double)atof(tempValues[i+1].c_str()));
							i+=2;
							compt++;
						}
						Mv.push_back(Z);
					}	
					else{
						int k = 1;
						Z.Null();
						for(int j=0;j<nModes_;j++){
							for(int i=0;i<nModes_;i++){Z(j,i) = (double)atof(tempValues[k].c_str())*exp(J*(double)atof(tempValues[k+1].c_str())); k+=2;}
						}
						Mv.push_back(Z);
					}

				}
			}
		}

		c++;
	}

	file.close();
}


//WK got properties tempC and lossF
void HornElement::cylTlaAccumulate (const double w, const double length, const double radius, const float T, const float F, const float H, const float XC, SimpleMatrix *a) {
	//Standard TLA for a cylinder [Braden, section 2.2]

	if (LOSSY_) {
		const double Sm = M_PI * radius * radius;		
		const dcomp gl = Gamma(w, Sm, T, F,H,XC)*length;
		const dcomp sgl = std::sinh(gl); 
		const dcomp cgl = std::cosh(gl); 
		const dcomp one = dcomp(1.0,0.0); 
		const dcomp z_c = Z_c(w, Sm, Sm, T, F,H,XC);
		(*a) = SimpleMatrix (cgl, z_c*sgl, one/z_c*sgl, cgl); //lossy	
	}
	else {
		const double kl = w/C2(T,H,XC)  * length;  //lossless model
		const double S = M_PI*radius*radius;
		const double zc=RHO2(T,H)*C2(T,H,XC)/S;

		(*a).setValues(cos(kl), zc*J*sin(kl), 1/zc*J*sin(kl), cos(kl));
	}
}

void HornElement::coneTlaAccumulate(const double w, const double Xin, const double Xout, const double Rin, const double Rout, const float T, const float F, const float H, const float XC, SimpleMatrix *a) {
	//Standard TLA (Mapes-Riordan) [Braden, section 2.3]

	double x0, x1, Si;
	double L = sqrt( (Xin-Xout)*(Xin-Xout) + (Rin-Rout)*(Rin-Rout) );
	dcomp ratio;
	const double Sm = M_PI*(Rin+Rout)*(Rin+Rout)/4; 
	dcomp z_c;
	const dcomp one = dcomp(1.0,0.0);
	
	if (Rin!=Rout) { //Conical cases
		double x_centre = (Xin*Rout-Xout*Rin)/(Rout-Rin);
		
		if (Rin<Rout) { //expanding cone
			x0 = sqrt((Xin-x_centre)*(Xin-x_centre) + Rin*Rin);	
			x1 = x0 + L;
			Si = 2*M_PI*x0*(x0+x_centre-Xin);
		} else { //tapering cone
			x1 = -sqrt((Xout-x_centre)*(Xout-x_centre) + Rout*Rout); 
			x0 = (x1 - L);
			Si = 2*M_PI*(-x0)*(Xin - (x_centre+x0));		
		}
		
		//x0 = Xin-x_centre;  //plane-wave assumption (used for thesis experiments, not worth making into a separate function)
		//x1 = Xout-x_centre;
		//L=x1-x0;
		//Si = M_PI * Rin * Rin;

		ratio = x1/x0; 

		if (LOSSY_) { //lossy case
			const dcomp gamma = Gamma(w, Sm, T, F,H, XC); 
			const dcomp gl = gamma*L; 
			const dcomp sgl = std::sinh(gl); 
			const dcomp cgl = std::cosh(gl); 
			z_c = Z_c(w, Si, Sm, T, F,H,XC);
	
			(*a) = SimpleMatrix (ratio*(cgl-sgl/gamma/x1), 
			one/ratio*z_c*sgl,
			(one/z_c)*((ratio-one/gamma/gamma/x0/x0)*sgl + gl*cgl/gamma/gamma/x0/x0),
			one/ratio*(cgl+sgl/gamma/x0));
		} else { //lossless case
			const double k = w/C2(T,H,XC);
			const dcomp ck = cos(k*L);
			const dcomp sk = sin(k*L);
			const double S = M_PI * Rin * Rin;			  
			const dcomp r0 = RHO2(T,H)*C2(T,H, XC)/S;	

			(*a).setValues(ratio*ck-sk/k/x0, 
			J/ratio*r0*sk,
			(J/r0)*((ratio+one/k/k/x0/x0)*sk - L/k*ck/x0/x0),
			one/ratio*(one/k/x0*sk + ck));
		}
	} else { //cylinder 
		cylTlaAccumulate(w, (Xout-Xin), Rin, T, F,H,XC,a);	
	}
}

// ################## RADIATION IMPEDANCE ###########################

double HornElement::Dn(const double tau, const int i, const double kR)  {
	// For radiation impedance (Zorumski), see Kemp.
	return ( -ROOT2 * tau * j1(tau * kR) ) / ( (GAMMA_I[i]*GAMMA_I[i]/kR/kR) - tau*tau );
}

double HornElement::Dmn(const double tau, const int m, const int n, const double kR)  {
	// For radiation impedance (Zorumski) with nodal diameter modes
	const double gmn=BESSELDERIVZERO[m][n];
	const double num = tau * jn(m, gmn) * jn(m-1, tau * kR) - 1/kR*gmn * jn(m-1, gmn) * jn(m, tau * kR);
	const double denom = gmn*gmn/(kR*kR) - tau*tau;

	return ( ROOT2 * num/denom);
}

dcomp HornElement::radiationImpedanceLevineCyl(const double w, const double radius, const float T, const float H, const float XC)  {
	//This is Levine & Schwinger's model
	const double S = M_PI * radius * radius;
	dcomp z_t = dcomp(w * w * radius * radius /4/C2(T, H,XC)/C2(T, H,XC), 0.61 * w * radius /C2(T, H,XC));
	z_t = z_t * r_zero(S, T,H,XC);
	return z_t;
}

dcomp HornElement::radiationImpedanceLevineCone(const double w, const double Xin, const double Xout, const double Rin, const double Rout, const float T, const float H, const float XC)  {
	//This is Levine & Schwinger's model

	dcomp z_t = dcomp(RHO2(T,H)*w*w/M_PI/C2(T, H,XC)/4, 0.6133*RHO2(T,H)*w/M_PI/Rout); 
	
	return z_t;
}


void HornElement::radiationImpedanceZorumski(const double w, Matrix* result, const double radius, const bool bends, const float T, const float H, const float XC)  {
// Radiation impedance (Zorumski), see Kemp. Using Simpson's rule to evaluate the 1st integral, and Gaussian Quadrature the second.

	const double kR = w/C2(T, H,XC)*radius;
	const double Zc = RHO2(T,H)*C2(T, H,XC)/(M_PI*radius*radius);
	double realpart, imagpart;
	if (!bends) {  // straight multimodal case
		for (int i=0; i<nModes_; i++) {
			D1_[i]=Dn(ROOT2INV, i, kR);
  			D2_[i]=Dn(1.0, i, kR);
		}
	
		for (int n=0; n<nModes_; n++)
		 for (int m=n; m<nModes_; m++) { //note that matrix is symmetric

			realpart = Zc/3.0 * M_PI_4 * ( 4.0*ROOT2INV*D1_[n]*D1_[m] + D2_[n]*D2_[m] );  //Simpson's rule
		  	imagpart = 0.0;
			for (int i=0; i<GAUSSN; i++) imagpart+=GAUSSW[i] * Zc * COSHGAUSSX[i] * Dn(COSHGAUSSX[i], n, kR) * Dn(COSHGAUSSX[i], m, kR); //Guassian quadrature, using stored values


			//wasilakis: set radiation to zero for numerical comparisons
			//realpart = 0;
			//imagpart = 0;

			(*result)(n,m) = dcomp(realpart, imagpart);
			if (m!=n) (*result)(m,n) = dcomp(realpart, imagpart);
					} 
	} else {  //bent multimodal case
		
		for (int alpha=0; alpha<nModes_; alpha++) {
			const int m=mIndex(alpha, bends);	const int n=nIndex(alpha, bends); 
			const double gmn = BESSELDERIVZERO[m][n];
			const double jmn = jn(m, gmn);
			double A_a;
			if (m==0)  A_a = 1.0 / jmn;
			else A_a = 1.0 / sqrt(0.5 * (1-m*m/gmn/gmn) *jmn*jmn);
	
			for (int beta=alpha; beta<nModes_; beta++) { //matrix is symmetric
				const int mu=mIndex(beta, bends); const int nu=nIndex(beta, bends);
				const double gmunu = BESSELDERIVZERO[mu][nu];
				const double jmunu = jn(mu, gmunu);
				double A_b;
				
				if (mu==0)  A_b = 1.0 / jmunu;
				else A_b = 1.0 / sqrt(0.5 * (1-mu*mu/gmunu/gmunu) *jmunu*jmunu)     *     0.5;

				if (m==mu)  {
					realpart = Zc/3.0 * M_PI_4 * A_a*A_b* ( 4.0*ROOT2INV*Dmn(ROOT2INV, m, n, kR) * Dmn(ROOT2INV, m, nu, kR) + Dmn(1.0, m, n, kR)*Dmn(1.0, m, nu, kR) );  //Simpson's rule
					imagpart = 0.0;
					for (int i=0; i<GAUSSN; i++) imagpart+=GAUSSW[i] * Zc * COSHGAUSSX[i] * A_a*A_b * Dmn(COSHGAUSSX[i], m, n, kR) * Dmn(COSHGAUSSX[i], m, nu, kR); //Guassian quadrature, using stored values
				} else {
					realpart=0.0;
					imagpart=0.0;
				}

				//wasilakis: set radiation to zero for numerical comparisons
			//realpart = 0;
			//imagpart = 0;

				(*result)(alpha,beta) = dcomp(realpart, imagpart);
				if (alpha!=beta) (*result)(beta, alpha) = dcomp(realpart, imagpart);

				}
		}	
	}
}

dcomp HornElement::cylZorumskiPlaneRadiation(const double w, const double radius, const float T, const float H, const float XC)  {
	//##################KLUDGE! TEMPORARY FUNCTION TO EXPT WITH TLA USING ZORUMSKI#################
	// Radiation impedance (Zorumski), see Kemp. Using Simpson's rule to evaluate the 1st integral, and Gaussian Quadrature the second.

	const double kR = w/C2(T, H,XC)*radius;
	const double Zc = RHO2(T,H)*C2(T, H,XC)/(M_PI*radius*radius);
	double realpart, imagpart;
	double D1=Dn(ROOT2INV, 0, kR);
	double D2=Dn(1, 0, kR);
	
	realpart = Zc/3 * M_PI_4 * ( 4*ROOT2INV*D1*D1 + D2*D2 );  //Simpson's rule
  	imagpart = 0.0;
	for (int i=0; i<GAUSSN; i++) imagpart+=GAUSSW[i] * Zc * COSHGAUSSX[i] * Dn(COSHGAUSSX[i], 0, kR) * Dn(COSHGAUSSX[i], 0, kR); //Guassian quadrature, using stored values
	return dcomp(realpart, imagpart);
}


void HornElement::radiationImpedanceInfiniteCylPipe(const double w, Matrix* result, const double radius, const float T, const float H, const float XC)  {
	//Infinite cylindrical pipe termination (multimodal)
	
	const double k =w/C2(T, H,XC);
	double alpha2;
	dcomp kn;
 
	for (int n=0; n<nModes_; n++)  {
		alpha2 = GAMMA_I2[n]/radius/radius; // WK: Correction alpha_n = Gamma_n / R!
		if (k*k - alpha2>=0.0) 	kn = sqrt(k*k - alpha2);
		else 	kn = dcomp(0.0, -sqrt(alpha2-k*k));
		(*result)(n,n) = RHO2(T,H)*C2(T, H,XC)/M_PI/radius/radius *  k/kn;
		// Zc scaled with k/kn for modes > 0: {Zc, Zc*k/k1, Zc*k/k2...}  
	} 
}

void HornElement::radiationImpedanceTerminated(const double w, Matrix* result, const double radius, const bool bends, const float T, const float H, const float XC, const dcomp z_trm)  {
	//Acoustically hard reflecting termination (multimodal)

	const double k =w/C2(T, H,XC);
	double alpha2;
	dcomp kn;
	for (int n=0; n<nModes_; n++)  {
		alpha2 = GAMMA_I2[n]/radius/radius; // WK: Correction alpha_n = Gamma_n / R!
		if (k*k - alpha2>=0.0) 	kn = sqrt(k*k - alpha2);
		else 	kn = dcomp(0.0, -sqrt(alpha2-k*k));
		(*result)(n,n) = z_trm *  k/kn;
		// z_trm scaled with k/kn for modes > 0
	} 
}

// ##################### MULTIMODAL #####################

void HornElement::kn (const double w, const double radius, const bool lossy, const bool bends, const float T, const float L, const float H, const float XC,dcomp* kn)  {
	//complex wavenumber for higher modes (Bruneau et al)
	//Returns an array, size nModes, containing kn for n=1..nModes
	// WK: corrected cutoff threshold
	// WK: corrected signs of real and imag part in lossy kn

	const double k = w/C2(T, H,XC);

	const double kk = k*k;
	const double kkRR = kk * radius * radius;
	const dcomp lf = dcomp(1/L/L/L,0);					// WK: compensating loss frequency characteristics
	double cof2; //cut-off frequency
	double root_f = (abs(L-1) < eps) ? sqrt(w/2/M_PI) : pow(w/2/M_PI,static_cast<double>(L/2)); 	// WK: loss with more than root_f!

	for (int i=0; i<nModes_; i++) {
		//const double gammai2 = GAMMA_I2[i];				//square of gamma_i
		const double gammai2 = GAMMA_I[i]*GAMMA_I[i];
		
		if (!bends) cof2 = gammai2/radius/radius;
		else {cof2 = BESSELDERIVZERO[mIndex(i, bends)][nIndex(i, bends)]/radius; cof2=cof2*cof2;}

		if (lossy) {
			const dcomp eps_v = dcomp(2.03e-5 * root_f, 2.03e-5 * root_f);	//[Kemp, section 2.4.1]
			const dcomp eps_t = dcomp(0.95e-5 * root_f, 0.95e-5 * root_f);
			dcomp eps_i = lf*((1-gammai2/kkRR)*eps_v + eps_t); // WK extra loss dcomp(L,0)!		// boundary specific admittance
			kn[i] = sqrt(kk - cof2 + 2*k/radius*dcomp(eps_i.imag(), -eps_i.real())); 	//lossy kn, any sign!
			if (kn[i].real() < 0) kn[i] = -kn[i];		// WK: corrected certain cases - enforce positive real part
			if (kn[i].imag() > 0) kn[i] = conj(kn[i]);	// WK: corrected certain cases - enforce negative imag to avoid exponentially increased amplitude
		} else {
			if (kk > cof2) kn[i] = sqrt(kk - cof2);		//lossless kn (propagating)
			else kn[i] = dcomp(0,-sqrt(cof2 - kk));		//lossless kn (evanescent)
		}
	}
}

void HornElement::jumpMultimodeInputImpedance (const double w, double *rr, Matrix *A, Matrix *B, Matrix *Zout, const double rIn, const double rOut, const bool bends, const float T)  {
	// Evaluates input impedance of a jump discontinuity

//	std::cout << "HornElement::jumpMultimodeInputImpedance("<<w<<","<<*rr<<","<<*A<<","<<*B<<","<<*Zout<<","<<rIn<<","<<rOut<<","<<bends<<")\n";
	if (nModes_==1) {return;} 

	// ############ THIS IS NOT FREQUENCY DEPENDENT!!! (rewrite desirable, costing run-time speed)
	// WK: Verify Matrix A and its RadiusRatio rr
DBG2 std::cout << "	--- static jumpMultimodeInputImpedance: ";

	if ((abs(rOut/rIn - (*rr)) > eps) || (A == NULL) || (A->RowNo() != (size_t) nModes_)) {

		double zeta;
		if (rIn<rOut) zeta = rIn/rOut; else zeta = rOut/rIn;

		if (!bends) { // straight case	[Kemp, section 2.5.1] (Multimodal equations at discontinuity / circular cross-section)
			for (int i=0; i<nModes_; i++)
			 for (int j=1; j<nModes_; j++) { //column 1 = (1 0 0 0 0...) for all arguments
				F_(i,j) = (2*zeta*GAMMA_I[j]*j1(zeta*GAMMA_I[j])) / ( (zeta*zeta*GAMMA_I2[j] - GAMMA_I2[i])*j0(GAMMA_I[j]));
			
			}
		} else { // bent case	[Braden, section 3.4.1] (Bore discontinuitues)
			for (int alpha=0; alpha<nModes_; alpha++) {
				const int m=mIndex(alpha, bends);	const int n=nIndex(alpha, bends); 
				const double gmn = BESSELDERIVZERO[m][n];
				const double jmn = jn(m, gmn);
				double A_a;
				if (m==0)  A_a = 1.0 / jmn;
				else A_a = 1.0 / sqrt(0.5 * (1-m*m/gmn/gmn) *jmn*jmn);
	
				for (int beta=1; beta<nModes_; beta++) { //column 1 = (1 0 0 0 0...) for all arguments
					const int mu=mIndex(beta, bends); const int nu=nIndex(beta, bends);
				
					if (m==mu) { 
						const double gmunu = BESSELDERIVZERO[mu][nu];
						const double jmunu = jn(mu, gmunu);
						double A_b;
					
						if (mu==0)  A_b = 1.0 / jmunu;
						else A_b = 1.0 / sqrt(0.5 * (1-mu*mu/gmunu/gmunu) *jmunu*jmunu);
					
						F_(alpha, beta) = A_a*A_b * ((m==0?2:1)) * (gmunu*zeta*jmn* jn(m-1,gmunu*zeta)  -  gmn*jn(m-1,gmn)*jn(m,gmunu*zeta) )/(gmn*gmn - zeta*zeta*gmunu*gmunu);
			
					} else F_(alpha,beta)=0.0;		
				}
			}
		}


	// WK cache scattering matrices!
	if (A == NULL) A = new Matrix(nModes_, nModes_) ;
	else if (A->RowNo() != (size_t) nModes_) A->Null(nModes_, nModes_) ;
	if (B == NULL) B = new Matrix(nModes_, nModes_) ;
	else if (B->RowNo() != (size_t) nModes_) B->Null(nModes_, nModes_) ;


	if (rIn<rOut) 	{ (*A) = F_; 	(*B) = (~F_);    }
	else 		{ (*A) = (!F_); (*B) = (!(~F_)); }

	(*rr) = rOut/rIn;
DBG1 std::cout << " Caching scattering matrices for rOut=" << rOut << ", rIn=" << rIn << ", RR=" << (*rr) << endl;
	}

	(*Zout) = (*A) * (*Zout) * (*B);

}

void HornElement::jumpMultimodeTransmMatrix (double zeta, const bool bends, Matrix *a) {

	Matrix F1 = Matrix(nModes_,nModes_);
	Matrix F2 = Matrix(nModes_,nModes_);

	if (!bends) { // straight case   [Kemp, section 2.5.1] (Multimodal equations at discontinuity / circular cross-section)

		F1(0,0) = 1;
		for (int i=0; i<nModes_; i++){
		 for (int j=1; j<nModes_; j++) { //column 1 = (1 0 0 0 0...) for all arguments
			F1(i,j) = (2*zeta*GAMMA_I[j]*j1(zeta*GAMMA_I[j])) / ( (zeta*zeta*GAMMA_I2[j] - GAMMA_I2[i])*j0(GAMMA_I[j]));
		 }
		}

	}
	else { // bent case    [Braden, section 3.4.1] (Bore discontinuitues)

		//WASILAKIS:
		F1(0,0) = 1;	//see Kemp, p50

			for (int alpha=0; alpha<nModes_; alpha++) {
				const int m=mIndex(alpha, bends);	const int n=nIndex(alpha, bends); 
				const double gmn = BESSELDERIVZERO[m][n];
				const double jmn = jn(m, gmn);
				double A_a;
				if (m==0)  A_a = 1.0 / jmn;
				else A_a = 1.0 / sqrt(0.5 * (1-m*m/gmn/gmn) *jmn*jmn);
	
				for (int beta=1; beta<nModes_; beta++) { //column 1 = (1 0 0 0 0...) for all arguments
					const int mu=mIndex(beta, bends); const int nu=nIndex(beta, bends);
				
					if (m==mu) { 
						const double gmunu = BESSELDERIVZERO[mu][nu];
						const double jmunu = jn(mu, gmunu);
						double A_b;
					
						if (mu==0)  A_b = 1.0 / jmunu;
						else A_b = 1.0 / sqrt(0.5 * (1-mu*mu/gmunu/gmunu) *jmunu*jmunu);
					
						F1(alpha, beta) = A_a*A_b * ((m==0?2:1)) * (gmunu*zeta*jmn* jn(m-1,gmunu*zeta)  -  gmn*jn(m-1,gmn)*jn(m,gmunu*zeta) )/(gmn*gmn - zeta*zeta*gmunu*gmunu);
			
					} else F1(alpha,beta)=0.0;		
				}
			}
		}

	F2 = (!(~F1));
	for (int k=0;k<2*nModes_;k++) {
		for (int l=0;l<2*nModes_;l++){
			if(k<nModes_ && l<nModes_) {(*a)(k,l) = F1(k,l);}	
			if(k>=nModes_ && l>=nModes_) {(*a)(k,l) = F2(k-nModes_,l-nModes_);}
		}
	}

}

void HornElement::cylMultimodeInputImpedance (const double w, Matrix *Zout, const double length, const double radius, const bool lossy, const bool simplified, const bool bends, const float T, const float L, const float H, const float XC)  {
	// Input impedance of a cylinder
	kn(w, radius, lossy, bends, T, L,H,XC, ki_); 
	const double krcs = w*RHO2(T,H)/M_PI/radius/radius;

	if (simplified) {
DBG2 std::cout << " --- static cylSimplifiedInputImpedance: radius=" << radius << ", length=" << length << ", temp=" << T << ", loss=" << L << ", frq=" << w/2/M_PI << ", modes=" << nModes_<< endl << "Zout(before) = " << (*Zout) << endl;

		Zout->Diagonal();
		for (int i=0; i<nModes_; i++) {
			dcomp DD = (abs(ki_[i].imag() * length) < 690) ? sin(ki_[i]*length) / cos(ki_[i]*length): -J; //tan doesn't seem to work
			dcomp ZO = ((*Zout)(i,i) + J * DD * (krcs/ki_[i])) / (J * DD * (ki_[i]/krcs) * (*Zout)(i,i) + dcomp(1,0));
			(*Zout)(i,i) = ZO;
		}
	}
	else {
DBG2 std::cout << " --- static cylMultimodeInputImpedance: radius=" << radius << ", length=" << length << ", temp=" << T << ", loss=" << L << ", frq=" << w/2/M_PI << ", modes=" << nModes_ <<endl << "Zout(before) = " << (*Zout) << endl;

		for (int i=0; i<nModes_; i++) {
			if (abs(ki_[i].imag() * length) < 690) D_(i,i) =  sin(ki_[i]*length) / cos(ki_[i]*length); //tan doesn't seem to work
			else D_(i,i) = -J;		//i.e. if ki_[i]>0, this is +J else -J
			Zc_(i,i) = krcs/ki_[i];
			ZcInv_(i,i) = ki_[i]/krcs; //Zc is diagonal, so each term of the inverse is the reciprocal of the corresponding term in Zc
		}
		if (nModes_ == 1) (*Zout)(0,0) = ((*Zout)(0,0) + J * D_(0,0) * Zc_(0,0)) / (J * D_(0,0) * ZcInv_(0,0) * (*Zout)(0,0) + I_(0,0));
		else if (nModes_ == 2) {
			dcomp Z00 = (Zc_(0,0)*(-((*Zout)(0,1)*(*Zout)(1,0)*D_(1,1)) + 
		        	((*Zout)(0,0) + J*D_(0,0)*Zc_(0,0))*((*Zout)(1,1)*D_(1,1) - J*Zc_(1,1))))/
				(-J*(*Zout)(0,1)*(*Zout)(1,0)*D_(0,0)*D_(1,1) + (J*(*Zout)(0,0)*D_(0,0) + Zc_(0,0))*((*Zout)(1,1)*D_(1,1) - J*Zc_(1,1)));
		     	
			
			dcomp Z01 = ((*Zout)(0,1)*(dcomp(1,0) + D_(0,0)*D_(0,0))*Zc_(0,0)*Zc_(1,1))/
				((*Zout)(0,1)*(*Zout)(1,0)*D_(0,0)*D_(1,1) - ((*Zout)(0,0)*D_(0,0) - J*Zc_(0,0))*((*Zout)(1,1)*D_(1,1) - J*Zc_(1,1)));
		    	
			dcomp Z10 = ((*Zout)(1,0)*(dcomp(1,0) + D_(1,1)*D_(1,1))*Zc_(0,0)*Zc_(1,1))/
				((*Zout)(0,1)*(*Zout)(1,0)*D_(0,0)*D_(1,1) - ((*Zout)(0,0)*D_(0,0) - J*Zc_(0,0))*((*Zout)(1,1)*D_(1,1) - J*Zc_(1,1)));
		     
			
			dcomp Z11 = (Zc_(1,1)*(-J*(*Zout)(0,1)*(*Zout)(1,0)*D_(0,0) + (J*(*Zout)(0,0)*D_(0,0) + Zc_(0,0))*((*Zout)(1,1) + J*D_(1,1)*Zc_(1,1))))/
				((*Zout)(0,1)*(*Zout)(1,0)*D_(0,0)*D_(1,1) - ((*Zout)(0,0)*D_(0,0) - J*Zc_(0,0))*((*Zout)(1,1)*D_(1,1) - J*Zc_(1,1)));
			(*Zout)(0,0) = Z00;
			(*Zout)(0,1) = Z01; 
			(*Zout)(1,0) = Z10;
			(*Zout)(1,1) = Z11;
		}
		else if (nModes_ == 3) {
			dcomp Z00 = (Zc_(0,0)*(Zc_(1,1)*(Zc_(2,2)*(*Zout)(0,0) + J*D_(2,2)*(-((*Zout)(0,2)*(*Zout)(2,0)) + (*Zout)(0,0)*(*Zout)(2,2))) + 
				J*D_(0,0)*Zc_(0,0)*(Zc_(1,1)*(Zc_(2,2) + J*D_(2,2)*(*Zout)(2,2)) + D_(1,1)*(J*Zc_(2,2)*(*Zout)(1,1) + D_(2,2)*(*Zout)(1,2)*(*Zout)(2,1) - D_(2,2)*(*Zout)(1,1)*(*Zout)(2,2))) + 
				D_(1,1)*(-J*Zc_(2,2)*((*Zout)(0,1)*(*Zout)(1,0) - (*Zout)(0,0)*(*Zout)(1,1)) + 
				D_(2,2)*((*Zout)(0,2)*(*Zout)(1,1)*(*Zout)(2,0) - (*Zout)(0,1)*(*Zout)(1,2)*(*Zout)(2,0) - (*Zout)(0,2)*(*Zout)(1,0)*(*Zout)(2,1) + (*Zout)(0,0)*(*Zout)(1,2)*(*Zout)(2,1) + (*Zout)(0,1)*(*Zout)(1,0)*(*Zout)(2,2) - 
				(*Zout)(0,0)*(*Zout)(1,1)*(*Zout)(2,2)))))/
				(Zc_(0,0)*(Zc_(1,1)*(Zc_(2,2) + J*D_(2,2)*(*Zout)(2,2)) + D_(1,1)*(J*Zc_(2,2)*(*Zout)(1,1) + D_(2,2)*(*Zout)(1,2)*(*Zout)(2,1) - D_(2,2)*(*Zout)(1,1)*(*Zout)(2,2))) + 
				J*D_(0,0)*(Zc_(1,1)*(Zc_(2,2)*(*Zout)(0,0) + J*D_(2,2)*(-((*Zout)(0,2)*(*Zout)(2,0)) + (*Zout)(0,0)*(*Zout)(2,2))) + 
				D_(1,1)*(-J*Zc_(2,2)*((*Zout)(0,1)*(*Zout)(1,0) - (*Zout)(0,0)*(*Zout)(1,1)) + 
				D_(2,2)*((*Zout)(0,2)*(*Zout)(1,1)*(*Zout)(2,0) - (*Zout)(0,1)*(*Zout)(1,2)*(*Zout)(2,0) - (*Zout)(0,2)*(*Zout)(1,0)*(*Zout)(2,1) + (*Zout)(0,0)*(*Zout)(1,2)*(*Zout)(2,1) + (*Zout)(0,1)*(*Zout)(1,0)*(*Zout)(2,2) - 
				(*Zout)(0,0)*(*Zout)(1,1)*(*Zout)(2,2)))));
			dcomp Z01 = ((dcomp(1,0) + D_(0,0)*D_(0,0))*Zc_(0,0)*Zc_(1,1)*(-J*Zc_(2,2)*(*Zout)(0,1) - D_(2,2)*(*Zout)(0,2)*(*Zout)(2,1) + D_(2,2)*(*Zout)(0,1)*(*Zout)(2,2)))/
				(Zc_(0,0)*(-J*Zc_(1,1)*Zc_(2,2) + D_(1,1)*Zc_(2,2)*(*Zout)(1,1) - J*D_(1,1)*D_(2,2)*(*Zout)(1,2)*(*Zout)(2,1) + D_(2,2)*(Zc_(1,1) + J*D_(1,1)*(*Zout)(1,1))*(*Zout)(2,2)) + 
				D_(0,0)*(Zc_(1,1)*(Zc_(2,2)*(*Zout)(0,0) + J*D_(2,2)*(-((*Zout)(0,2)*(*Zout)(2,0)) + (*Zout)(0,0)*(*Zout)(2,2))) + 
				D_(1,1)*(-J*Zc_(2,2)*((*Zout)(0,1)*(*Zout)(1,0) - (*Zout)(0,0)*(*Zout)(1,1)) + 
				D_(2,2)*((*Zout)(0,2)*(*Zout)(1,1)*(*Zout)(2,0) - (*Zout)(0,1)*(*Zout)(1,2)*(*Zout)(2,0) - (*Zout)(0,2)*(*Zout)(1,0)*(*Zout)(2,1) + (*Zout)(0,0)*(*Zout)(1,2)*(*Zout)(2,1) + (*Zout)(0,1)*(*Zout)(1,0)*(*Zout)(2,2) - 
				(*Zout)(0,0)*(*Zout)(1,1)*(*Zout)(2,2)))));
			dcomp Z02 = ((dcomp(1,0) + D_(0,0)*D_(0,0))*Zc_(0,0)*Zc_(2,2)*(-J*Zc_(1,1)*(*Zout)(0,2) + D_(1,1)*(*Zout)(0,2)*(*Zout)(1,1) - D_(1,1)*(*Zout)(0,1)*(*Zout)(1,2)))/
				(Zc_(0,0)*(-J*Zc_(1,1)*Zc_(2,2) + D_(1,1)*Zc_(2,2)*(*Zout)(1,1) - J*D_(1,1)*D_(2,2)*(*Zout)(1,2)*(*Zout)(2,1) + D_(2,2)*(Zc_(1,1) + J*D_(1,1)*(*Zout)(1,1))*(*Zout)(2,2)) + 
				D_(0,0)*(Zc_(1,1)*(Zc_(2,2)*(*Zout)(0,0) + J*D_(2,2)*(-((*Zout)(0,2)*(*Zout)(2,0)) + (*Zout)(0,0)*(*Zout)(2,2))) + 
				D_(1,1)*(-J*Zc_(2,2)*((*Zout)(0,1)*(*Zout)(1,0) - (*Zout)(0,0)*(*Zout)(1,1)) + 
				D_(2,2)*((*Zout)(0,2)*(*Zout)(1,1)*(*Zout)(2,0) - (*Zout)(0,1)*(*Zout)(1,2)*(*Zout)(2,0) - (*Zout)(0,2)*(*Zout)(1,0)*(*Zout)(2,1) + (*Zout)(0,0)*(*Zout)(1,2)*(*Zout)(2,1) + (*Zout)(0,1)*(*Zout)(1,0)*(*Zout)(2,2) - 
				(*Zout)(0,0)*(*Zout)(1,1)*(*Zout)(2,2)))));
			dcomp Z10 = ((dcomp(1,0) + D_(1,1)*D_(1,1))*Zc_(0,0)*Zc_(1,1)*(-J*Zc_(2,2)*(*Zout)(1,0) - D_(2,2)*(*Zout)(1,2)*(*Zout)(2,0) + D_(2,2)*(*Zout)(1,0)*(*Zout)(2,2)))/
				(Zc_(0,0)*(-J*Zc_(1,1)*Zc_(2,2) + D_(1,1)*Zc_(2,2)*(*Zout)(1,1) - J*D_(1,1)*D_(2,2)*(*Zout)(1,2)*(*Zout)(2,1) + D_(2,2)*(Zc_(1,1) + J*D_(1,1)*(*Zout)(1,1))*(*Zout)(2,2)) + 
				D_(0,0)*(Zc_(1,1)*(Zc_(2,2)*(*Zout)(0,0) + J*D_(2,2)*(-((*Zout)(0,2)*(*Zout)(2,0)) + (*Zout)(0,0)*(*Zout)(2,2))) + 
				D_(1,1)*(-J*Zc_(2,2)*((*Zout)(0,1)*(*Zout)(1,0) - (*Zout)(0,0)*(*Zout)(1,1)) + 
				D_(2,2)*((*Zout)(0,2)*(*Zout)(1,1)*(*Zout)(2,0) - (*Zout)(0,1)*(*Zout)(1,2)*(*Zout)(2,0) - (*Zout)(0,2)*(*Zout)(1,0)*(*Zout)(2,1) + (*Zout)(0,0)*(*Zout)(1,2)*(*Zout)(2,1) + (*Zout)(0,1)*(*Zout)(1,0)*(*Zout)(2,2) - 
				(*Zout)(0,0)*(*Zout)(1,1)*(*Zout)(2,2)))));
			dcomp Z11 = (Zc_(1,1)*(J*D_(1,1)*Zc_(1,1)*(Zc_(0,0)*(Zc_(2,2) + J*D_(2,2)*(*Zout)(2,2)) + D_(0,0)*(J*Zc_(2,2)*(*Zout)(0,0) + D_(2,2)*(*Zout)(0,2)*(*Zout)(2,0) - D_(2,2)*(*Zout)(0,0)*(*Zout)(2,2))) + 
				Zc_(0,0)*(Zc_(2,2)*(*Zout)(1,1) + J*D_(2,2)*(-((*Zout)(1,2)*(*Zout)(2,1)) + (*Zout)(1,1)*(*Zout)(2,2))) + 
				D_(0,0)*(-J*Zc_(2,2)*((*Zout)(0,1)*(*Zout)(1,0) - (*Zout)(0,0)*(*Zout)(1,1)) + 
				D_(2,2)*((*Zout)(0,2)*(*Zout)(1,1)*(*Zout)(2,0) - (*Zout)(0,1)*(*Zout)(1,2)*(*Zout)(2,0) - (*Zout)(0,2)*(*Zout)(1,0)*(*Zout)(2,1) + (*Zout)(0,0)*(*Zout)(1,2)*(*Zout)(2,1) + (*Zout)(0,1)*(*Zout)(1,0)*(*Zout)(2,2) - 
				(*Zout)(0,0)*(*Zout)(1,1)*(*Zout)(2,2)))))/
				(Zc_(0,0)*(Zc_(1,1)*(Zc_(2,2) + J*D_(2,2)*(*Zout)(2,2)) + D_(1,1)*(J*Zc_(2,2)*(*Zout)(1,1) + D_(2,2)*(*Zout)(1,2)*(*Zout)(2,1) - D_(2,2)*(*Zout)(1,1)*(*Zout)(2,2))) + 
				J*D_(0,0)*(Zc_(1,1)*(Zc_(2,2)*(*Zout)(0,0) + J*D_(2,2)*(-((*Zout)(0,2)*(*Zout)(2,0)) + (*Zout)(0,0)*(*Zout)(2,2))) + 
				D_(1,1)*(-J*Zc_(2,2)*((*Zout)(0,1)*(*Zout)(1,0) - (*Zout)(0,0)*(*Zout)(1,1)) + 
				D_(2,2)*((*Zout)(0,2)*(*Zout)(1,1)*(*Zout)(2,0) - (*Zout)(0,1)*(*Zout)(1,2)*(*Zout)(2,0) - (*Zout)(0,2)*(*Zout)(1,0)*(*Zout)(2,1) + (*Zout)(0,0)*(*Zout)(1,2)*(*Zout)(2,1) + (*Zout)(0,1)*(*Zout)(1,0)*(*Zout)(2,2) - 
				(*Zout)(0,0)*(*Zout)(1,1)*(*Zout)(2,2)))));
			dcomp Z12 = ((dcomp(1,0) + D_(1,1)*D_(1,1))*Zc_(1,1)*Zc_(2,2)*(-(D_(0,0)*(*Zout)(0,2)*(*Zout)(1,0)) - J*Zc_(0,0)*(*Zout)(1,2) + D_(0,0)*(*Zout)(0,0)*(*Zout)(1,2)))/
				(Zc_(0,0)*(-J*Zc_(1,1)*Zc_(2,2) + D_(1,1)*Zc_(2,2)*(*Zout)(1,1) - J*D_(1,1)*D_(2,2)*(*Zout)(1,2)*(*Zout)(2,1) + D_(2,2)*(Zc_(1,1) + J*D_(1,1)*(*Zout)(1,1))*(*Zout)(2,2)) + 
				D_(0,0)*(Zc_(1,1)*(Zc_(2,2)*(*Zout)(0,0) + J*D_(2,2)*(-((*Zout)(0,2)*(*Zout)(2,0)) + (*Zout)(0,0)*(*Zout)(2,2))) + 
				D_(1,1)*(-J*Zc_(2,2)*((*Zout)(0,1)*(*Zout)(1,0) - (*Zout)(0,0)*(*Zout)(1,1)) + 
				D_(2,2)*((*Zout)(0,2)*(*Zout)(1,1)*(*Zout)(2,0) - (*Zout)(0,1)*(*Zout)(1,2)*(*Zout)(2,0) - (*Zout)(0,2)*(*Zout)(1,0)*(*Zout)(2,1) + (*Zout)(0,0)*(*Zout)(1,2)*(*Zout)(2,1) + (*Zout)(0,1)*(*Zout)(1,0)*(*Zout)(2,2) - 
				(*Zout)(0,0)*(*Zout)(1,1)*(*Zout)(2,2)))));
			dcomp Z20 = ((dcomp(1,0) + D_(2,2)*D_(2,2))*Zc_(0,0)*Zc_(2,2)*(-J*Zc_(1,1)*(*Zout)(2,0) + D_(1,1)*(*Zout)(1,1)*(*Zout)(2,0) - D_(1,1)*(*Zout)(1,0)*(*Zout)(2,1)))/
				(Zc_(0,0)*(-J*Zc_(1,1)*Zc_(2,2) + D_(1,1)*Zc_(2,2)*(*Zout)(1,1) - J*D_(1,1)*D_(2,2)*(*Zout)(1,2)*(*Zout)(2,1) + D_(2,2)*(Zc_(1,1) + J*D_(1,1)*(*Zout)(1,1))*(*Zout)(2,2)) + 
				D_(0,0)*(Zc_(1,1)*(Zc_(2,2)*(*Zout)(0,0) + J*D_(2,2)*(-((*Zout)(0,2)*(*Zout)(2,0)) + (*Zout)(0,0)*(*Zout)(2,2))) + 
				D_(1,1)*(-J*Zc_(2,2)*((*Zout)(0,1)*(*Zout)(1,0) - (*Zout)(0,0)*(*Zout)(1,1)) + 
				D_(2,2)*((*Zout)(0,2)*(*Zout)(1,1)*(*Zout)(2,0) - (*Zout)(0,1)*(*Zout)(1,2)*(*Zout)(2,0) - (*Zout)(0,2)*(*Zout)(1,0)*(*Zout)(2,1) + (*Zout)(0,0)*(*Zout)(1,2)*(*Zout)(2,1) + (*Zout)(0,1)*(*Zout)(1,0)*(*Zout)(2,2) - 
				(*Zout)(0,0)*(*Zout)(1,1)*(*Zout)(2,2)))));
			dcomp Z21 = ((dcomp(1,0) + D_(2,2)*D_(2,2))*Zc_(1,1)*Zc_(2,2)*(-(D_(0,0)*(*Zout)(0,1)*(*Zout)(2,0)) - J*Zc_(0,0)*(*Zout)(2,1) + D_(0,0)*(*Zout)(0,0)*(*Zout)(2,1)))/
				(Zc_(0,0)*(-J*Zc_(1,1)*Zc_(2,2) + D_(1,1)*Zc_(2,2)*(*Zout)(1,1) - J*D_(1,1)*D_(2,2)*(*Zout)(1,2)*(*Zout)(2,1) + D_(2,2)*(Zc_(1,1) + J*D_(1,1)*(*Zout)(1,1))*(*Zout)(2,2)) + 
				D_(0,0)*(Zc_(1,1)*(Zc_(2,2)*(*Zout)(0,0) + J*D_(2,2)*(-((*Zout)(0,2)*(*Zout)(2,0)) + (*Zout)(0,0)*(*Zout)(2,2))) + 
				D_(1,1)*(-J*Zc_(2,2)*((*Zout)(0,1)*(*Zout)(1,0) - (*Zout)(0,0)*(*Zout)(1,1)) + 
				D_(2,2)*((*Zout)(0,2)*(*Zout)(1,1)*(*Zout)(2,0) - (*Zout)(0,1)*(*Zout)(1,2)*(*Zout)(2,0) - (*Zout)(0,2)*(*Zout)(1,0)*(*Zout)(2,1) + (*Zout)(0,0)*(*Zout)(1,2)*(*Zout)(2,1) + (*Zout)(0,1)*(*Zout)(1,0)*(*Zout)(2,2) - 
				(*Zout)(0,0)*(*Zout)(1,1)*(*Zout)(2,2)))));
			dcomp Z22 = (Zc_(2,2)*(J*D_(2,2)*Zc_(2,2)*(Zc_(0,0)*(Zc_(1,1) + J*D_(1,1)*(*Zout)(1,1)) + D_(0,0)*(J*Zc_(1,1)*(*Zout)(0,0) + D_(1,1)*(*Zout)(0,1)*(*Zout)(1,0) - D_(1,1)*(*Zout)(0,0)*(*Zout)(1,1))) + 
				Zc_(0,0)*(Zc_(1,1)*(*Zout)(2,2) + J*D_(1,1)*(-((*Zout)(1,2)*(*Zout)(2,1)) + (*Zout)(1,1)*(*Zout)(2,2))) + 
				D_(0,0)*(-J*Zc_(1,1)*((*Zout)(0,2)*(*Zout)(2,0) - (*Zout)(0,0)*(*Zout)(2,2)) + 
				D_(1,1)*((*Zout)(0,2)*(*Zout)(1,1)*(*Zout)(2,0) - (*Zout)(0,1)*(*Zout)(1,2)*(*Zout)(2,0) - (*Zout)(0,2)*(*Zout)(1,0)*(*Zout)(2,1) + (*Zout)(0,0)*(*Zout)(1,2)*(*Zout)(2,1) + (*Zout)(0,1)*(*Zout)(1,0)*(*Zout)(2,2) - 
				(*Zout)(0,0)*(*Zout)(1,1)*(*Zout)(2,2)))))/
				(Zc_(0,0)*(Zc_(1,1)*(Zc_(2,2) + J*D_(2,2)*(*Zout)(2,2)) + D_(1,1)*(J*Zc_(2,2)*(*Zout)(1,1) + D_(2,2)*(*Zout)(1,2)*(*Zout)(2,1) - D_(2,2)*(*Zout)(1,1)*(*Zout)(2,2))) + 
				J*D_(0,0)*(Zc_(1,1)*(Zc_(2,2)*(*Zout)(0,0) + J*D_(2,2)*(-((*Zout)(0,2)*(*Zout)(2,0)) + (*Zout)(0,0)*(*Zout)(2,2))) + 
				D_(1,1)*(-J*Zc_(2,2)*((*Zout)(0,1)*(*Zout)(1,0) - (*Zout)(0,0)*(*Zout)(1,1)) + 
				D_(2,2)*((*Zout)(0,2)*(*Zout)(1,1)*(*Zout)(2,0) - (*Zout)(0,1)*(*Zout)(1,2)*(*Zout)(2,0) - (*Zout)(0,2)*(*Zout)(1,0)*(*Zout)(2,1) + (*Zout)(0,0)*(*Zout)(1,2)*(*Zout)(2,1) + (*Zout)(0,1)*(*Zout)(1,0)*(*Zout)(2,2) - 
				(*Zout)(0,0)*(*Zout)(1,1)*(*Zout)(2,2)))));

			(*Zout)(0,0) = Z00;
			(*Zout)(0,1) = Z01; 
			(*Zout)(0,2) = Z02; 
			(*Zout)(1,0) = Z10;
			(*Zout)(1,1) = Z11;
			(*Zout)(1,2) = Z12;
			(*Zout)(2,0) = Z20;
			(*Zout)(2,1) = Z21;
			(*Zout)(2,2) = Z22;
		}
		else (*Zout) = ((*Zout) + J * D_ * Zc_) * !(J * D_ * ZcInv_ * (*Zout) + I_); // Kemp (2.100), p. 55


	}

DBG2 std::cout << "Zout(after) = " << (*Zout) << endl;
}


void HornElement::cylBendMultimodeInputImpedance (const double w, Matrix *Zout, const double length, const double radius, const double bendRadius, const bool lossy, const float T, const float L, const float Humidity, const float XC)  {
	//Uses same code as that in cylBend itself, but is less efficient as requires repeated re-definition of matrices
	
	Matrix A, B, CC, K, BCKB, X, N, Dbend, H, E, nu, HXN;
	
	A = Matrix(nModes_,1);
	B = Matrix(nModes_,nModes_);
	CC = Matrix(nModes_,nModes_);
	K = Matrix(nModes_, nModes_);
	BCKB = Matrix(nModes_, nModes_);
	H = Matrix(nModes_, nModes_);
	Dbend = Matrix(nModes_, nModes_);
	X = Matrix(nModes_, nModes_);
	N = Matrix(nModes_, nModes_);
	nu = Matrix(nModes_, 1);
	HXN = Matrix(nModes_, nModes_);

	const int sigma = 1; //denotes whether symmetric or antisymmetric modes are used.
	
	LeG_quadrature leg = LeG_quadrature(nQuadraturePoints);

	//declare for-loop-var as local var to resolve VC compatibility issues...
	int alpha;

	for (alpha=0; alpha<nModes_; alpha++) {
		const int m=mIndex(alpha, true);
		const int n=nIndex(alpha, true);
		const double j = jn(m, BESSELDERIVZERO[m][n]);
		if (m==0)  A(alpha,0) = 1.0 / sqrt(M_PI * radius*radius * j*j);
		else A(alpha,0) = 1.0 / sqrt((M_PI * radius*radius*0.5) * (1-m*m/BESSELDERIVZERO[m][n]/BESSELDERIVZERO[m][n]) *j*j);
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
		B(alpha, beta) = ((alpha==beta)?1.0:0.0) -  A(alpha, 0)*A(beta, 0)*J_m_mu * leg.Bintegral(0.0, radius, m, n, mu, nu)/bendRadius;
		CC(alpha, beta) = A(alpha, 0)*A(beta, 0)*BESSELDERIVZERO[m][n]/radius*J_m_mu * leg.Cintegral_1(0.0, radius, m, n, mu, nu)/bendRadius
				- A(alpha, 0)*A(beta, 0)*((double)m)*(J_m_mu - H_m_mu)*leg.Cintegral_2(0.0, radius, m, n, mu, nu)/bendRadius;
	}

	const double sf = -length;
	//const double thetaf = -length/bendRadius;
	const double Zc = RHO2(T,Humidity)*C2(T, Humidity,XC)/M_PI/radius/radius;
	const double k=w/C2(T, Humidity,XC);

	//wasilakis
	if(!LOSSY_)
	for (int alpha=0; alpha<nModes_; alpha++) K(alpha, alpha) = k*k - BESSELDERIVZERO[mIndex(alpha, true)][nIndex(alpha, true)] * BESSELDERIVZERO[mIndex(alpha, true)][nIndex(alpha, true)]/radius/radius;
	else{
	
	//wasilakis: lossy kn (can be written as separate function returnig kn2 = kn^2)
	dcomp kn2;
	//const double k = w/C2(T, Humidity,XC);
	const double kk = k*k;
	const double kkRR = kk * radius * radius;
	const dcomp lf = dcomp(1/L/L/L,0);					// WK: compensating loss frequency characteristics
	double cof2; //cut-off frequency
	double root_f = (abs(L-1) < eps) ? sqrt(w/2/M_PI) : pow(w/2/M_PI,static_cast<double>(L/2)); 	// WK: loss with more than root_f!

	for (int i=0; i<nModes_; i++) {
		const double gammai2 = GAMMA_I2[i];				//square of gamma_i
		cof2 = BESSELDERIVZERO[mIndex(i, true)][nIndex(i, true)]/radius; cof2=cof2*cof2;

		
			const dcomp eps_v = dcomp(2.03e-5 * root_f, 2.03e-5 * root_f);	//[Kemp, section 2.4.1]
			const dcomp eps_t = dcomp(0.95e-5 * root_f, 0.95e-5 * root_f);
			dcomp eps_i = lf*((1-gammai2/kkRR)*eps_v + eps_t); // WK extra loss dcomp(L,0)!		// boundary specific admittance
			kn2 = kk - cof2 + 2*k/radius*dcomp(eps_i.imag(), -eps_i.real()); 	//lossy kn, any sign!
			//if (kn[i].real() < 0) kn[i] = -kn[i];		// WK: corrected certain cases - enforce positive real part
			//if (kn[i].imag() > 0) kn[i] = conj(kn[i]);	// WK: corrected certain cases - enforce negative imag to avoid exponentially increased amplitude
		
	K(i, i) = kn2;
	}
	}


	BCKB = B*(CC + K*B);
	H = 1.0/J/k/Zc * (CC+K*B);
	BCKB.eig(X, nu);

	for (int n=0; n<nModes_; n++)  {
		if (fabs(nu(n,0).imag())<1e-10) nu(n,0) = dcomp(nu(n,0).real(),  0.0); // counters a numerical glitch in eig that gives small complex components to real numbers
		nu(n,0) = sqrt(nu(n,0));
		Dbend(n,n) = exp(-J*nu(n,0)*sf);  //This is D from the derivation
		N(n,n) = 1.0/J/nu(n,0);
	}

	HXN = H*X*N;
	E = Dbend * !((*Zout)*HXN + X) * (X - (*Zout)*HXN) * Dbend;
	(*Zout) = (X - X*E) * !(HXN + HXN*E);	// Braden (3.64), p. 34

#ifdef TRACE
	if (!Zout->IsDiagonal()) std::cout << endl << "!!! Warning - Zout(cylBend) @" << w << " not diagonal. Maximal error = " << Zout->DiagonalError() << endl;
#endif	 
}



int HornElement::cylMultimodeAccumulate (const double w, const double length, const double radius, const bool lossy,const bool bends, const float T, const float F, const float H, const float XC, Matrix *a) {
	kn(w, radius, lossy, bends, T, F,H,XC, ki_);
	Matrix D3 = Matrix(nModes_,nModes_);
	Matrix Zc = Matrix(nModes_,nModes_);
	Matrix Zc_Inv = Matrix(nModes_,nModes_);
	const double k = w/C2(T, H,XC);
	const double S = M_PI * radius * radius;
	const double zc = RHO2(T,H)*C2(T, H,XC)/S;
	dcomp kL;

	for (int i=0;i<2*nModes_;i++) {
		if(i<nModes_){
			kL = ki_[i]*length;
			Zc(i,i) = (k*zc)/(ki_[i]);
			Zc_Inv(i,i) = (ki_[i])/(k*zc);
			if(abs(kL)<690) D3(i,i) = J*sin(kL)/cos(kL);
			else D3(i,i) = -J;			
		}
		
		for (int j=0;j<2*nModes_;j++){
			if(i<nModes_ && j<nModes_) {if(i==j) (*a)(i,j) = 1;}
			if(i>=nModes_ && j>=nModes_) {if(i==j) (*a)(i,j) = 1; }	
			if(i<nModes_ && j>=nModes_) (*a)(i,j) = (D3*Zc)(i,j-nModes_);
			if(i>=nModes_ && j<nModes_) (*a)(i,j) = (D3*Zc_Inv)(i-nModes_,j);
		}
	}		//[Kemp, section 2.6.2]

	return 0;
}


//wasilakis
void HornElement::cylBendMultimodeAccumulate (const double w, const double length, const double radius, const double bendRadius, const bool lossy, const float T, const float Humidity, const float XC, Matrix *a)  {

	Matrix A, B, CC, K, BCKB, X, N, Dbend, H, E, nu, HXN;
	
	A = Matrix(nModes_,1);
	B = Matrix(nModes_,nModes_);
	CC = Matrix(nModes_,nModes_);
	K = Matrix(nModes_, nModes_);
	BCKB = Matrix(nModes_, nModes_);
	H = Matrix(nModes_, nModes_);
	Dbend = Matrix(nModes_, nModes_);
	X = Matrix(nModes_, nModes_);
	N = Matrix(nModes_, nModes_);
	nu = Matrix(nModes_, 1);
	HXN = Matrix(nModes_, nModes_);

	const int sigma = 0; //denotes whether symmetric or antisymmetric modes are used.
	
	LeG_quadrature leg = LeG_quadrature(nQuadraturePoints);

    //declare for-loop-var as local var to resolve VC compatibility issues...
    int alpha;
	for (alpha=0; alpha<nModes_; alpha++) {
		const int m=mIndex(alpha, true);
		const int n=nIndex(alpha, true);
		const double j = jn(m, BESSELDERIVZERO[m][n]);
		if (m==0)  A(alpha,0) = 1.0 / sqrt(M_PI * radius*radius * j*j);
		else A(alpha,0) = 1.0 / sqrt((M_PI * radius*radius*0.5) * (1-m*m/BESSELDERIVZERO[m][n]/BESSELDERIVZERO[m][n]) *j*j);
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
		B(alpha, beta) = ((alpha==beta)?1.0:0.0)*bendRadius -  A(alpha, 0)*A(beta, 0)*J_m_mu * leg.Bintegral(0.0, radius, m, n, mu, nu);
		CC(alpha, beta) = A(alpha, 0)*A(beta, 0)*BESSELDERIVZERO[m][n]/radius*J_m_mu * leg.Cintegral_1(0.0, radius, m, n, mu, nu)
				- A(alpha, 0)*A(beta, 0)*((double)m)*(J_m_mu - H_m_mu)*leg.Cintegral_2(0.0, radius, m, n, mu, nu);
	}

	const double thetaf = -length/bendRadius;
	const double Zc = RHO2(T,Humidity)*C2(T, Humidity,XC)/M_PI/radius/radius;
	const double k=w/C2(T, Humidity,XC);

	for (alpha=0; alpha<nModes_; alpha++) K(alpha, alpha) = k*k - BESSELDERIVZERO[mIndex(alpha, true)][nIndex(alpha,true)] * BESSELDERIVZERO[mIndex(alpha, true)][nIndex(alpha, true)]/radius/radius;

	BCKB = B*(CC + K*B);
	BCKB.eig(X, nu);

	for (int n=0; n<nModes_; n++)  {
		if (fabs(nu(n,0).imag())<1e-10) nu(n,0) = dcomp(nu(n,0).real(),  0.0); // counters a numerical glitch in eig that gives small complex components to real numbers
		nu(n,0) = sqrt(nu(n,0));
		Dbend(n,n) = exp(-J*nu(n,0)*thetaf);  //This is D from the derivation
		N(n,n) = 1.0/J/nu(n,0);
	}
	
	H = 1.0/J/k/Zc * (CC+K*B);
	HXN = H*X*N;
	
	for (int i=0;i<2*nModes_;i++) {
		for (int j=0;j<2*nModes_;j++){
			if(i<nModes_ && j<nModes_) {if(i==j) (*a)(i,j) = X(i,j)*HXN(i,j)-X(i,j)*Dbend(i,j)*X(i,j)*Dbend(i,j);}
			if(i>=nModes_ && j>=nModes_) {if(i==j) (*a)(i-nModes_,j-nModes_) = HXN(i-nModes_,j-nModes_)*X(i-nModes_,j-nModes_) - HXN(i-nModes_,j-nModes_)*Dbend(i-nModes_,j-nModes_)*X(i-nModes_,j-nModes_); }	
			if(i<nModes_ && j>=nModes_) (*a)(i,j) = HXN(i,j-nModes_)*HXN(i,j-nModes_)-HXN(i,j-nModes_)*Dbend(i,j-nModes_)*X(i,j-nModes_)*Dbend(i,j-nModes_);
			if(i>=nModes_ && j<nModes_) (*a)(i,j) = X(i-nModes_,j)*X(i-nModes_,j) + X(i-nModes_,j)*Dbend(i-nModes_,j)*X(i-nModes_,j);
		}
	}	
}

/*
void HornElement::bendRadiationImpedanceInfiniteCylPipe0(const double w, Matrix* result, const double radius, const float T, const float H, const float XC) {
	//Infinite cylindrical pipe termination (multimodal)

	const double k =w/C2(T, H,XC);	
	double kt2;
	dcomp factor;
 
	for (int alpha=0; alpha<nModes_; alpha++)  {
		const int m=mIndex(alpha, true); const int n=nIndex(alpha, true);

		kt2 = BESSELDERIVZERO[m][n]*BESSELDERIVZERO[m][n]/radius/radius;
		if ((k*k - kt2)>=0.0) 	factor = sqrt(k*k - kt2);
		else 	factor = dcomp(0.0, -sqrt(kt2-k*k));
		(*result)(alpha,alpha) = RHO2(T,H)*C2(T, H,XC)/M_PI/radius/radius *  k/factor;
	} 
}
*/


bool HornElement::LOSSY_=true;
bool HornElement::SIMPLIFIED_=false;
double HornElement::BORERESOLUTION_=0.2;
int HornElement::nModes_=0;
double HornElement::F_MIN_=0;
double HornElement::F_MAX_=1800;
double HornElement::D_F_=10;
Matrix HornElement::I_; 
Matrix HornElement::D_; 
Matrix HornElement::Zc_;
Matrix HornElement::ZcInv_; 
Matrix HornElement::F_; 
dcomp* HornElement::ki_;
vector<double> HornElement::Freq;
vector<Matrix> HornElement::MZ;
double* HornElement::D1_; 
double* HornElement::D2_; 
bool HornElement::haveInitMatrices=false;

#ifdef TRACE
//ofstream HornElement::clogf;
#endif

#endif

