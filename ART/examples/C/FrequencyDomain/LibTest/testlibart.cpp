#include <iostream>
#include <iomanip>
#include <cmath>

#include "ART.h" 

using namespace std;


int main(void) {

	P_ART_Object pSim;

	P_ART_Element myCyl;
	P_ART_Element myCon;
	P_ART_Element myBes;

	P_ART_Simulator mySim;

	P_ART_Circuit myIns;

	P_ART_Variant myImpCurve;

	int i;

	T_ART_Tripl tri;

	double mag;

	char input;

	pSim = ARTRootObject();

	mySim = ARTCreateSimulator("MySimulator", "FrequencyDomain", "MultiModal");
	
	// Create elements
	myCyl = ARTCreateElement(mySim, "Cyl", "Cylinder");
	myCon = ARTCreateElement(mySim, "Con", "Cone");
	myBes = ARTCreateElement(mySim, "Bes", "Besselhorn");

	// Set parameters
	ARTSetParameter(mySim, "Cyl.r = 1; Cyl.length = 100;");

	ARTSetParameter(mySim, "Con.length = 50;");
	ARTSetParameter(mySim, "Con.r1 = Cyl.r; Con.r2 = 1.5 * Con.r1;");

	ARTSetParameter(mySim, "Bes.length = 20; Bes.r1 = Con.r2;");
	ARTSetParameter(mySim, "Bes.r2 = 12.5;");
	ARTSetParameter(mySim, "Bes.flare = 0.4;");

	// Create circuit and add elements to circuit
	myIns = ARTCreateCircuit(mySim, "MyInstrument");

	ARTAppendReference(myIns, myCyl);
	ARTAppendReference(myIns, myCon);
	ARTAppendReference(myIns, myBes);

	// Set simulation properties
	ARTSetFrequencyRange(mySim, 10, 1800, 10);
	ARTSetNModes(mySim, 1); 

	cout << "Calculating..." << flush;
	myImpCurve = ARTGetValue(ARTInputImpedance(myIns));
	cout << "Done." << endl << endl;
	cout << "Frequency     Impedance" << endl;
  	
	for(i = 0; i < ARTGetLength(myImpCurve); i++) {
		// get data structure
		tri = ARTGetTriple(myImpCurve, i);
		// compute magnitude
		mag = sqrt(tri.re*tri.re + tri.im*tri.im);
		cout.width(5);
		cout << tri.f << " Hz   " << setiosflags(ios::fixed) << setprecision(2);
		cout.width(12);
		cout << mag << " Ohm" << endl << setiosflags(ios::fixed) << setprecision(0);
	}

	// wait for unser input;
	cout << "Please press ENTER to continue..." << endl;
	cin.getline(&input, 1);

	ARTDestroyCircuit(mySim, myIns);

	ARTDestroyElement(mySim, myCyl);
	ARTDestroyElement(mySim, myCon);
	ARTDestroyElement(mySim, myBes);

	ARTDestroySimulator(mySim);

	ARTRootDestroy();

	return 0;
}
