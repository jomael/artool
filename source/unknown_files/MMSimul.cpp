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
/*
#include <iostream>
#include "MMSimul.h"
#include "WindInstrument.h"

#define NOERROR 0
#define ERROR -1


//**************************************************************************************************************


int __stdcall load_net(int N, int* m, pChar* p0, double* p1, double* p2, double* p3, double* p4, double* p5, double* p6) 
{
  if (ins == NULL) ins = new WindInstrument();
  if (ins->getSize() > 0) ins->cleanElements();
  if (ins->impedanceCurve.getSize() > 0) ins->impedanceCurve.clear();

//ofstream file("load_net.log");
  
  for (int i=0; i<N; i++) {
	if        (m[i]==BOREJUMPSECTION_TYPE)    {
//file << "appending JUMP " << p1[i] << " " << p2[i] << " " << p3[i] << " " << p4[i] << endl;
		ins->append(new BoreJumpSection      (p1[i], p2[i], p3[i], p4[i], p0[i]), true); 
	} else if (m[i]==CYLINDERSECTION_TYPE)    {
//file << "appending CYL  " << p1[i] << " " << p2[i] << " " << p3[i] << " " << p4[i] << endl;
		ins->append(new CylinderSection      (false, false, p1[i], p2[i], p3[i], p4[i], p0[i]), true);
	} else if (m[i]==CONESECTION_TYPE)        {
//file << "appending CONE " << p1[i] << " " << p2[i] << " " << p3[i] << " " << p4[i] << " " << p5[i] << endl;
		ins->append(new ConeSection          (false, false, p1[i], p2[i], p3[i], p4[i], p5[i], p0[i]), true); 
	} else if (m[i]==BESSELSECTION_TYPE)      {
		ins->append(new BesselSection        (false, false, p1[i], p2[i], p3[i], p4[i], p5[i], p6[i], p0[i]), true); 
	} else if (m[i]==LISTSECTION_TYPE)        {
		WindInstrument* tempIns = new WindInstrument();
		if( tempIns->load(p0[i]) == false ) return ERROR;
		ins->append (tempIns, true);
	} else if (m[i]==CYLINDERBENDSECTION_TYPE){
		ins->append(new CylinderBendSection  (false, false, p1[i], p2[i], p3[i], p4[i], p5[i], p0[i]), true); 
	} else if (m[i]==CONEBENDSECTION_TYPE)    {
		ins->append(new ConeBendSection      (false, false, p1[i], p2[i], p3[i], p4[i], p5[i], p6[i], p0[i]), true); 
	};
  }
//file.close();
  return NOERROR;
}

//**************************************************************************************************************


int __stdcall unload_net(int dummy)
{
  if (ins == NULL) ins = new WindInstrument();
  else ins->cleanElements();
  return NOERROR;
} 

//**************************************************************************************************************

int __stdcall set_nModes(int nModes, int lossy, int simplified, double boreResolution)
{
  if (ins == NULL) ins = new WindInstrument();
  ins->setnModes(nModes);
  ins->setlossy(lossy==1);
  ins->setsimplified(simplified==1);
  ins->setboreresolution(boreResolution);
  return NOERROR;
}

//**************************************************************************************************************

int __stdcall calc_imp(double f, double* mag, double* pha, double* tmag, double* tpha, int term, double gain)
{
  if (ins == NULL) ins = new WindInstrument();
  if (ins->getSize() > 0) {

	dcomp res;

	if (ins->impedanceCurve.getSize() > 0) {
		res = ins->impedanceCurve.getImpedance(f) / dcomp(gain * 1.0E-5,0);
	}
	else 	res = ins->getInputImpedance(f,term,tmag,tpha,gain*1.0E-5);

  	(*mag) = abs(res);
  	(*pha) = arg(res);
  	return NOERROR;
  }
  else return ERROR;
}

//**************************************************************************************************************

int __stdcall preview_imp(double fMin, double fMax, double fStep, int term)
{
  if (ins == NULL) ins = new WindInstrument();
  if (ins->getSize() > 0) {
	double tmag = 0; double tpha = 0; bool aborted = false;
	for (double f=fMin + fStep*(fMin==0.0); f<=fMax && !aborted; f+=fStep) {
	  	dcomp imp = ins->getInputImpedance(f,term,&tmag,&tpha,1.0);
		ins->impedanceCurve.append(f, imp );
	if (Status_Indication != NULL) aborted = (*Status_Indication)("Preview...", floor(100*(f-fMin)/(fMax-fMin)));
	}
	ins->impedanceCurve.save("preview.imp");
	return ins->impedanceCurve.getSize();
  }
  else return ERROR;
}

//**************************************************************************************************************

int __stdcall refine_imp(double eps, int term)
{
  if (ins == NULL) ins = new WindInstrument();
  if (ins->impedanceCurve.getSize() > 0) {
	ins->refinePeaks(eps, term);
	ins->impedanceCurve.save("refine.imp");
	return ins->impedanceCurve.getSize();
  }
  else return ERROR;
}

//**************************************************************************************************************

int __stdcall get_number_of_elements(int dummy)
{
  if (ins == NULL) ins = new WindInstrument();
  return ins->getSize();
}

//**************************************************************************************************************

int __stdcall goto_first_section(int dummy)
{
  if (ins == NULL) ins = new WindInstrument();
  if (ins->getSize() > 0) { ins->setIterToBegin(); return NOERROR; }
  else return ERROR;
}

//**************************************************************************************************************

int __stdcall goto_last_section(int dummy)
{
  if (ins == NULL) ins = new WindInstrument();
  if (ins->getSize() > 0) { ins->setIterToEnd();  return NOERROR; }
  else return ERROR;
}

//**************************************************************************************************************

int __stdcall section_plus_plus(int dummy)
{
  if (ins == NULL) ins = new WindInstrument();
  if (ins->getSize() > 0) { ins->iter++;  return NOERROR; }
  else return ERROR;
}

//**************************************************************************************************************

int __stdcall section_minus_minus(int dummy)
{
  if (ins == NULL) ins = new WindInstrument();
  if (ins->getSize() > 0) { ins->iter--;  return NOERROR; }
  else return ERROR;
}

//**************************************************************************************************************

int __stdcall return_section(int* m, pChar* p0, double* p1, double* p2, double* p3, double* p4, double* p5, double* p6)
{
  if (ins == NULL) ins = new WindInstrument();
  if (ins->getSize() > 0) {
    (*m) = ((HornElement*)(*ins->iter))->type();

    if        ((*m)==BOREJUMPSECTION_TYPE)   {
      (*p0) = ((BoreJumpSection*)(*ins->iter))->getName().c_str();
      (*p1) = ((BoreJumpSection*)(*ins->iter))->rIn();
      (*p2) = ((BoreJumpSection*)(*ins->iter))->rOut();
      (*p3) = ((BoreJumpSection*)(*ins->iter))->tempC();
      (*p4) = ((BoreJumpSection*)(*ins->iter))->lossF();
      (*p5) = 1.0;
      (*p6) = 1.0;
    } else if ((*m)==CYLINDERSECTION_TYPE)    {
      (*p0) = ((CylinderSection*)(*ins->iter))->getName().c_str();
      (*p1) = ((CylinderSection*)(*ins->iter))->length();
      (*p2) = ((CylinderSection*)(*ins->iter))->rIn();
      (*p3) = ((CylinderSection*)(*ins->iter))->tempC();
      (*p4) = ((CylinderSection*)(*ins->iter))->lossF();
      (*p5) = 1.0;
      (*p6) = 1.0;
    } else if ((*m)==CONESECTION_TYPE)        {
      (*p0) = ((ConeSection*)(*ins->iter))->getName().c_str();
      (*p1) = ((ConeSection*)(*ins->iter))->length();
      (*p2) = ((ConeSection*)(*ins->iter))->rIn();
      (*p3) = ((ConeSection*)(*ins->iter))->rOut();
      (*p4) = ((ConeSection*)(*ins->iter))->tempC();
      (*p5) = ((ConeSection*)(*ins->iter))->lossF();
      (*p6) = 1.0;
    } else if ((*m)==BESSELSECTION_TYPE)      {
      (*p0) = ((BesselSection*)(*ins->iter))->getName().c_str();
      (*p1) = ((BesselSection*)(*ins->iter))->length();
      (*p2) = ((BesselSection*)(*ins->iter))->rIn();
      (*p3) = ((BesselSection*)(*ins->iter))->rOut();
      (*p4) = ((BesselSection*)(*ins->iter))->flare();
      (*p5) = ((BesselSection*)(*ins->iter))->tempC();
      (*p6) = ((BesselSection*)(*ins->iter))->lossF();
    } else if ((*m)==LISTSECTION_TYPE)        {
      (*p0) = ((HornElement*)(*ins->iter))->getName().c_str();
      (*p1) = ((HornElement*)(*ins->iter))->length();
      (*p2) = 1.0;
      (*p3) = 1.0;
      (*p4) = 1.0;
      (*p5) = 1.0;
      (*p6) = 1.0;
    } else if ((*m)==CYLINDERBENDSECTION_TYPE){
      (*p0) = ((CylinderBendSection*)(*ins->iter))->getName().c_str();
      (*p1) = ((CylinderBendSection*)(*ins->iter))->length();
      (*p2) = ((CylinderBendSection*)(*ins->iter))->rIn();
      (*p3) = ((CylinderBendSection*)(*ins->iter))->bendRadius();
      (*p4) = ((CylinderBendSection*)(*ins->iter))->tempC();
      (*p5) = ((CylinderBendSection*)(*ins->iter))->lossF();
      (*p6) = 1.0;
    } else if ((*m)==CONEBENDSECTION_TYPE)    {
      (*p0) = ((ConeBendSection*)(*ins->iter))->getName().c_str();
      (*p1) = ((ConeBendSection*)(*ins->iter))->length();
      (*p2) = ((ConeBendSection*)(*ins->iter))->rIn();
      (*p3) = ((ConeBendSection*)(*ins->iter))->rOut();
      (*p4) = ((ConeBendSection*)(*ins->iter))->bendRadius();
      (*p5) = ((ConeBendSection*)(*ins->iter))->tempC();
      (*p6) = ((ConeBendSection*)(*ins->iter))->lossF();
    }
    return NOERROR;
  }
  else return ERROR;
}

//**************************************************************************************************************

int __stdcall modify_section(int m, pChar p0, double p1, double p2, double p3, double p4, double p5, double p6)
{
  if (ins == NULL) ins = new WindInstrument();
  if (ins->getSize() > 0) {
    if (m == ((HornElement*)(*ins->iter))->type()) {

      if        (m==BOREJUMPSECTION_TYPE)   {
        ((BoreJumpSection*)(*ins->iter))->setName(string(p0));
        ((BoreJumpSection*)(*ins->iter))->rIn(p1);
        ((BoreJumpSection*)(*ins->iter))->rOut(p2);
        ((BoreJumpSection*)(*ins->iter))->tempC(p3);
        ((BoreJumpSection*)(*ins->iter))->lossF(p4);
      } else if (m==CYLINDERSECTION_TYPE)    {
        ((CylinderSection*)(*ins->iter))->setName(string(p0));
        ((CylinderSection*)(*ins->iter))->length(p1);
        ((CylinderSection*)(*ins->iter))->radius(p2);
        ((CylinderSection*)(*ins->iter))->tempC(p3);
        ((CylinderSection*)(*ins->iter))->lossF(p4);
      } else if (m==CONESECTION_TYPE)        {
        ((ConeSection*)(*ins->iter))->setName(string(p0));
        ((ConeSection*)(*ins->iter))->length(p1);
        ((ConeSection*)(*ins->iter))->rIn(p2);
        ((ConeSection*)(*ins->iter))->rOut(p3);
        ((ConeSection*)(*ins->iter))->tempC(p4);
        ((ConeSection*)(*ins->iter))->lossF(p5);
      } else if (m==BESSELSECTION_TYPE)      {
        ((BesselSection*)(*ins->iter))->setName(string(p0));
        ((BesselSection*)(*ins->iter))->length(p1);
        ((BesselSection*)(*ins->iter))->rIn(p2);
        ((BesselSection*)(*ins->iter))->rOut(p3);
        ((BesselSection*)(*ins->iter))->flare(p4);
        ((BesselSection*)(*ins->iter))->tempC(p5);
        ((BesselSection*)(*ins->iter))->lossF(p6);
      } else if (m==LISTSECTION_TYPE)        {
        ((HornElement*)(*ins->iter))->setName(string(p0));
        ((HornElement*)(*ins->iter))->length(p1);
      } else if (m==CYLINDERBENDSECTION_TYPE){
        ((CylinderBendSection*)(*ins->iter))->setName(string(p0));
        ((CylinderBendSection*)(*ins->iter))->length(p1);
        ((CylinderBendSection*)(*ins->iter))->radius(p2);
        ((CylinderBendSection*)(*ins->iter))->bendRadius(p3);
        ((CylinderBendSection*)(*ins->iter))->tempC(p4);
        ((CylinderBendSection*)(*ins->iter))->lossF();
      } else if (m==CONEBENDSECTION_TYPE)    {
        ((ConeBendSection*)(*ins->iter))->setName(string(p0));
        ((ConeBendSection*)(*ins->iter))->length(p1);
        ((ConeBendSection*)(*ins->iter))->rIn(p2);
        ((ConeBendSection*)(*ins->iter))->rOut(p3);
        ((ConeBendSection*)(*ins->iter))->bendRadius(p4);
        ((ConeBendSection*)(*ins->iter))->tempC(p5);
        ((ConeBendSection*)(*ins->iter))->lossF(p6);
      }
    }
    else {
	if        (m==BOREJUMPSECTION_TYPE)    {
		ins->insert(new BoreJumpSection      (p1, p2, p3, p4, p0)); 
	} else if (m==CYLINDERSECTION_TYPE)    {
		ins->insert(new CylinderSection      (false, false, p1, p2, p3, p4, p0));
	} else if (m==CONESECTION_TYPE)        {
		ins->insert(new ConeSection          (false, false, p1, p2, p3, p4, p5, p0)); 
	} else if (m==BESSELSECTION_TYPE)      {
		ins->insert(new BesselSection        (false, false, p1, p2, p3, p4, p5, p6, p0)); 
	} else if (m==LISTSECTION_TYPE)        {
		WindInstrument* tempIns = new WindInstrument();
		if( tempIns->load(p0) == false ) return ERROR;
		ins->insert(tempIns);
	} else if (m==CYLINDERBENDSECTION_TYPE){
		ins->insert(new CylinderBendSection  (false, false, p1, p2, p3, p4, p5, p0)); 
	} else if (m==CONEBENDSECTION_TYPE)    {
		ins->insert(new ConeBendSection      (false, false, p1, p2, p3, p4, p5, p6, p0)); 
	};
	ins->iter++;
	ins->erase ();
    }

    double tempC = (*ins->iter)->gettempC();
    double lossF = (*ins->iter)->getlossF();
    double rOldOut = (*ins->iter)->rOut();
    *ins->iter++;
    double rNewIn = (*ins->iter)->rIn();
    if (abs(rOldOut-rNewIn) > eps)  ins->insert(new BoreJumpSection(rOldOut, rNewIn, tempC, lossF));

    return NOERROR;
  }
  else return ERROR;
}

//**************************************************************************************************************

int __stdcall insert_section(int m, pChar p0, double p1, double p2, double p3, double p4, double p5, double p6)
{
  if (ins == NULL) ins = new WindInstrument();
  
  if        (m==BOREJUMPSECTION_TYPE)    {
	ins->insert(new BoreJumpSection      (p1, p2, p3, p4, p0)); 
  } else if (m==CYLINDERSECTION_TYPE)    {
	ins->insert(new CylinderSection      (false, false, p1, p2, p3, p4, p0));
  } else if (m==CONESECTION_TYPE)        {
	ins->insert(new ConeSection          (false, false, p1, p2, p3, p4, p5, p0)); 
  } else if (m==BESSELSECTION_TYPE)      {
	ins->insert(new BesselSection        (false, false, p1, p2, p3, p4, p5, p6, p0)); 
  } else if (m==LISTSECTION_TYPE)        {
	WindInstrument* tempIns = new WindInstrument();
	if( tempIns->load(p0) == false ) return ERROR;
	ins->insert (tempIns);
  } else if (m==CYLINDERBENDSECTION_TYPE){
	ins->insert(new CylinderBendSection  (false, false, p1, p2, p3, p4, p5, p0)); 
  } else if (m==CONEBENDSECTION_TYPE)    {
	ins->insert(new ConeBendSection      (false, false, p1, p2, p3, p4, p5, p6, p0)); 
  };

  double tempC = (*ins->iter)->gettempC();
  double lossF = (*ins->iter)->getlossF();
  double rOldOut = (*ins->iter)->rOut();
  *ins->iter++;
  double rNewIn = (*ins->iter)->rIn();
  if (abs(rOldOut-rNewIn) > eps)  ins->insert(new BoreJumpSection(rOldOut, rNewIn, tempC, lossF));

  return NOERROR;
}

//**************************************************************************************************************

int __stdcall delete_section(int dummy)
{
  if (ins == NULL) ins = new WindInstrument();
  if (ins->getSize() > 0) {
    ins->erase ();
    return NOERROR;
  }
  else return ERROR;
}

//**************************************************************************************************************

int __stdcall begin_trace(char* filename)
{
  if (ins == NULL) ins = new WindInstrument();

#ifdef TRACE
  logfil = freopen( filename, "a+", stdout );
#endif
  return NOERROR;
}

//**************************************************************************************************************

int __stdcall end_trace(int dummy)
{
  if (ins == NULL) ins = new WindInstrument();

#ifdef TRACE
  fclose(logfil);
#endif
  return NOERROR;
}

//**************************************************************************************************************

int __stdcall run_script(char* filename)
{
  if (ins == NULL) ins = new WindInstrument();
  ins->runScript(filename);
  return NOERROR;
}

//**************************************************************************************************************

int __stdcall store_callback(TStatus_Indication func)
{
  Status_Indication = func;
  return NOERROR;
}

//**************************************************************************************************************

#ifdef _CONSOLE

#include "io.h"
 
int main(int argc, char **argv) {
        if (argc < 2) {
	   cout << endl;
	   cout << "   *****************\n";
	   cout << "   * MMSimul v1.00 *\n";
	   cout << "   *****************\n";
	   cout << endl;
	   cout << "Wilfried Kausel 16/09/08  :  kausel@mdw.ac.at\n\n";

	   cout << "**Impedance calculation script**\n";
	   cout << endl;
	   cout << "Usage: \n";
	   cout << " MMSimul <termcode> <elemcode> <p1> .. <p6> <nModes> <lossy> <simplified> <resol> <debug>\n";
	   cout << " f.e. MMSimul 0 1 100 2 21 1 0 0 0 1 0 10 0 for closed cylinder l=100cm d=2cm 21°C plane wave viscous nosimplif 10Hz nodeb\n";
	   cout << " f.e. MMSimul 2 2 100 2 5 21 1 0 5 0 1 5 1 for open cone       l=100cm d=2..5cm 21°C 5_modes lossless simplified 5Hz debug\n";
           return NOERROR;
	}

        int               tcod = (argc>1) ? atoi(argv[1])  : 2;
	int     m[100];  m[0]  = (argc>2) ? atoi(argv[2])  : 1;
	double p1[100];  p1[0] = ((argc>3) && (m[0]!=4)) ? atof(argv[3]) : 1000;
	double p2[100];  p2[0] = (argc>4) ? atof(argv[4]) : 20;
	double p3[100];  p3[0] = (argc>5) ? atof(argv[5]) : 50;
	double p4[100];  p4[0] = (argc>6) ? atof(argv[6]) : 1;
	double p5[100];  p5[0] = (argc>7) ? atof(argv[7]) : 1;
	double p6[100];  p6[0] = (argc>8) ? atof(argv[8]) : 1;
        int               nmod = (argc>9) ? atoi(argv[9]) : 0;
        bool              loss = (argc>10) ? (1==atoi(argv[10])) : true;
        bool              smpl = (argc>11) ? (1==atoi(argv[11])) : true;
        int               reso = (argc>12) ? atoi(argv[12]) : 10;
        bool              deb  = (argc>13) ? (1==atoi(argv[13])) : false;

        pChar p0[100]; 
	char buf[1024];
	int i = 0;
        if (m[0]==4) for (char* p=argv[3];(*p)!='\0';p++) {buf[i]=(*p);i++;}
        buf[i] = '\0'; p0[0] = &buf[0];

	load_net(1, m, p0, p1, p2, p3, p4, p5, p6);

	set_nModes(nmod, loss, smpl, 0.5);

        if (deb) {
	   #define FILE_EXISTS(path) (0==access(path,0))
	   cout << "**Debug output**\n";
	   if (m[0]!=4) cout << "element = " << m[0] << " p1=" << p1[0] << " p2=" << p2[0] << " p3=" << p3[0] << " p4=" << p4[0] << " p5=" << p5[0] << " p6=" << p6[0] << endl;
	   else cout << "element = " << m[0] << " fn=" << p0 << endl;
	   cout << "termination=" << tcod << endl;
	   cout << "modes=" << nmod << endl;
	   cout << "lossy=" << loss << endl;
	   cout << "simpl=" << smpl << endl;
	   ins->print();
	   ins->roughPlot(nmod, 10, 1800, floor((1800-10) / reso));
           ins->refinePeaks(0.00001, 2);
	   ins->impedanceCurve.printPeaks();
	   ins->printDescription();
           if (FILE_EXISTS("debug.scr")) {
		cout << "**Executing script..." << endl;
		ins->runScript("debug.scr");
	   }
           return NOERROR;
	}


	double r,ph, tr, tph;

	for (double f = 5; f < 8000; f+=reso) {
	  if (0 ==calc_imp(f, &r, &ph, &tr, &tph, tcod, 1)) cout << f << " " << r << " " << ph << endl;
          else cout << "*** ERROR *** calculating freq = " << f << endl;
	}

	return NOERROR;
}
#endif
*/