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

//max+min macro def. from VC98/include/windef.h
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

// ############## TLA CALCULATIONS (i.e. plane-wave assumption) ######################
// series of functions to implement the impedance theory for simple geometries
// Methods themselves require little commenting; see the references for details of the theory.

	//Maths sub-functions for TLA Input Impedance calculations

#define DT (T - 26.85)
#define t (T + 273.15)
#define P (103325)
#define xw (h*f(T)*psv(T)/P)

double f(const float T) {return (1.00062+(3.14e-8)*P+(5.6e-7)*T*T);}
double psv(const float T) {return (exp((1.2811805e-5)*t*t - (1.950987e-2)*t + 34.04926034 - (6.3536311e3)/t));}

bool bendwarning = false;
bool bendwarningrad = false;


//C2: Speed of sound with Temperature, Pressure, CO2 and water vapor molar fractions, and Humidity dependencies: 
//More Accurate than C. C2 is used in the following code
double C2(const float T, const float h, const float xc)   { 
		return(331.5024 + 0.603055*T - 0.000528*T*T 
			+ (51.471935 + 0.1495874*T - 0.000782*T*T)*xw
			+ (-1.82e-7 + (3.73e-8)*T + (-2.93e-10)*T*T)*P
			+ (-85.20931 - 0.228525*T + (5.91e-5)*T*T)*xc
			-2.835149*xw*xw + (-2.15e-13)*P*P + 29.179762*xc*xc
			+ 0.000486*xw*xc*P)*100;
			// Chadefaux, p. 14 refers to: Cramer, JASA 93
}
//C: Speed of sound with Temperature dependency
double C(const float T)   { return(3.4723e4 * (1+0.00166*DT)); } 	//Speed of sound in air in cm/s

//RHO2: take into account more dependency than RHO but there's no big difference between both (RHO2 is used in the code)
double RHO2(const float T, const float h)   { return((1/(287.06*t))*(P - 230.617*h*exp((17.5043*T)/(241.2+T))))/1000;}
double RHO(const float T) { return(1.1769e-3 * (1-0.00335*DT));}	//Density of ambient air in g/cm^3

float ETA(const float T)  { return(1.846e-4 * (1+0.0025*DT)); }		//Shear viscosity in g/(cm s)


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

	const double x_centre = (Xin*Rout-Xout*Rin)/(Rout-Rin);
	double x0, x1, Si;
	const double L = sqrt((Xin-Xout)*(Xin-Xout) + (Rin-Rout)*(Rin-Rout));

	x0 = sqrt((Xin-x_centre)*(Xin-x_centre) + Rin*Rin);	
	x1 = x0 + L;
	Si = 2*M_PI*x1*(x1+x_centre-Xout);
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

//***************JUMP DISCONTINUITY******************

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


//#######CYLINDER##########

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



//###############CONE############

ConeSection::ConeSection(const double length, const double rDiff) {
	type_ = CONESECTION_TYPE;
	MA_ = NULL; MB_ = NULL; rr_ = 0.0; 
	length_ = length;
	rIn_ = 0.0; //Will be modified into feasible values when initialised.
	rOut_ = 0.0;
	rDiff_ = rDiff;
	tempC_=21.0;
	lossF_=1.0;
		humidity_ = 0.8; 		
	xc_ = 0.000382;
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


//###############BESSEL#########


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

//#######CYLINDERBEND##########

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


//###############CONE BEND############

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

//###############BRANCH ELEMENT############

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

#endif

