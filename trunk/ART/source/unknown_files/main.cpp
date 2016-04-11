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

#include <iostream>
#include "WindInstrument.h"
#include "runGA.cpp"
#include "runRB.cpp"
#include <time.h>

#if defined(__GNUG__) //MSVCPP6 on Windows and GCC3 on linux give different values when clock() is called
const int TIME_FACTOR=1000000;
#elif defined(_MSC_VER)
const int TIME_FACTOR=1000;
#endif


void printUsage();

int main(int argc, char **argv) {
	cout << endl;
	cout << "   **************\n";
	cout << "   * BIES v1.00 *\n";
	cout << "   **************\n";
	cout << endl;
	cout << "Alistair Braden 04/10/06  :  alistair.braden@ed.ac.uk\n\n";

	WindInstrument ins = WindInstrument();

	cout << setiosflags (ios::fixed | ios::showpoint) << setprecision(4);	
	long last_time = clock();

	// Program takes command-line parameters; see printUsage() for details. 

	if (argc==1) {
		printUsage();
		return 0;
	}

	if(ins.stringEquality(argv[1], "-help") || ins.stringEquality(argv[1], "--help")) {
		printUsage();
		return 0;
	}

	if (ins.stringEquality(argv[1], "-sct")) { 	
		//Allows .sct files to be run (included for backward-compatibility)
		
		cout << "Script mode\n";
		ins.runScript(argv[2]); 
		last_time=clock();
		cout << "This program ran for " << (double)last_time/TIME_FACTOR << " s\n";
		return 0;
	}
	
	if (ins.stringEquality(argv[1], "-ga")) { 	
		//Runs a .gap file, activating the Genetic Algorithm optimiser
		
		cout << "GA mode\n";
		runGA(argv[2]); 
		last_time=clock();
		cout << "This program ran for " << (double)last_time/TIME_FACTOR << " s\n";
		return 0;
	} 
	
	if (ins.stringEquality(argv[1], "-rb")) { 	
		//Runs a .rb file, activating the Rosenbrock optimiser
		
		cout << "Rosenbrock mode\n";
		runRB(argv[2]); 
		last_time=clock();
		cout << "This program ran for " << (double)last_time/TIME_FACTOR << " s\n";
		return 0;
	}

	if (ins.stringEquality(argv[1], "-import")) {	
		//Imports ASCII files dumped from the BIAS software (i.e. experimental data).
		
		cout << "**Import script (complete output)**\n";
		char temp[17];
		strcpy(temp, argv[3]);
		ins.impedanceCurve.importBIAS(argv[2]);
		ins.saveImpedancePlot(strcat(argv[3], "_M"));
		ins.impedanceCurve.save(strcat(temp, "_M.ic"));
		ins.impedanceCurve.printPeaks();
		last_time=clock();
		cout << "This program ran for " << (double)last_time/TIME_FACTOR << " s\n";
		return 0;
	}

	if (ins.stringEquality(argv[1], "-importstd")) {	
		//Imports ASCII files dumped from the BIAS software (i.e. experimental data).
		
		cout << "**Import script (standard output)**\n";
		char temp[17];
		strcpy(temp, argv[3]);
		ins.impedanceCurve.importBIAS(argv[2]);
		ins.impedanceCurve.convertToStandard();
		ins.saveImpedancePlot(strcat(argv[3], "_sM"));
		ins.impedanceCurve.save(strcat(temp, "_sM.ic"));
		ins.impedanceCurve.printPeaks();
		last_time=clock();
		cout << "This program ran for " << (double)last_time/TIME_FACTOR << " s\n";
		return 0;
	}

	if (ins.stringEquality(argv[1], "-peaks")) {	
		cout << "**Impedance-curve peak-finding script**\n";
		char temp[17];
		strcpy(temp, argv[2]);
		ins.impedanceCurve.load(strcat(argv[2], ".ic"));
		ins.impedanceCurve.findPeaks(true);
		ins.impedanceCurve.printPeaks();
		last_time=clock();
		cout << "This program ran for " << (double)last_time/TIME_FACTOR << " s\n";
		return 0;		
	}


	if (ins.stringEquality(argv[1], "-ic")) {
		// Loads .ic impedance curve and saves as maple/matlab plot
		
		cout << "**Export script**\n";
		char temp[17];
		strcpy(temp, argv[2]);
		ins.impedanceCurve.load(strcat(argv[2], ".ic"));
		ins.impedanceCurve.saveImpedancePlot(temp);
		last_time=clock();
		cout << "This program ran for " << (double)last_time/TIME_FACTOR << " s\n";
		return 0;	
	}
	
	if (ins.stringEquality(argv[1], "-harmonicity")) {
		// Loads .ic impedance curve and saves as maple/matlab plot
		
		cout << "**Harmonicity plot script**\n";
		if (argc==4) {
			char temp[32]; //used to construct filenames of saved files
			strcpy(temp, argv[2]);
			int nModes = atoi(argv[3]);
			char modes[3];
			itoa(nModes, modes, 10);

			if(ins.load(strcat(temp, ".ins"))==true) {
				ins.roughPlot(nModes, 10, 2000, 500);
				ins.refinePeaks(0.00001);
				strcpy(temp, argv[2]);
				ins.impedanceCurve.harmonicityPlot(-100.0, 100.0, 100, 2, 16, strcat(temp, "_harm.m"));
			} else cout << "Aborting script, printing current instrument\n";
		} else cout << "ERROR at command-line : bad arguments: requires -harmonicity <filename> <nModes>\n";
		last_time=clock();
		cout << "This program ran for " << (double)last_time/TIME_FACTOR << " s\n";
		return 0;
	}

	if (ins.stringEquality(argv[1], "-ic2harm")) {
		// Loads .ic impedance curve and saves a harmonicity plot
		
		cout << "**ic -> harmonicity script**\n";
		if (argc==4) {		
			char temp[17];
			strcpy(temp, argv[2]);
			ins.impedanceCurve.load(strcat(temp, ".ic"));
			bool isExpt = (bool)atoi(argv[3]);
			if (isExpt) ins.impedanceCurve.peakListAppend(1.0,1.0);
			ins.impedanceCurve.findPeaks(isExpt);
			ins.impedanceCurve.printPeaks();
			strcpy(temp, argv[2]);
			ins.impedanceCurve.harmonicityPlot(-100.0, 100.0, 100, 2, 16, strcat(temp, "_harm.m"));
		} else if (argc==5) {
			char temp[17];
			strcpy(temp, argv[2]);
			ins.impedanceCurve.load(strcat(temp, ".ic"));
			bool isExpt = (bool)atoi(argv[3]);
			if (isExpt) ins.impedanceCurve.peakListAppend(1.0,1.0);
			ins.impedanceCurve.findPeaks(isExpt);
			ins.impedanceCurve.printPeaks();
			strcpy(temp, argv[2]);
			ins.impedanceCurve.harmonicityPlot(-100.0, 100.0, 100, 2, 16, strcat(temp, "_harm.m"), atof(argv[4]));
		} else cout << "ERROR at command-line : bad arguments: requires -ic2harm <filename> <isExpt>\n";
		last_time=clock();
		cout << "This program ran for " << (double)last_time/TIME_FACTOR << " s\n";
		return 0;	
	}


	if (ins.stringEquality(argv[1], "-ins2int")) {	
		//Converts file from .ins (instrument) format to .int
		
		cout << "**ins->int convert script**\n";
		ins.load(argv[2]);
		ins.savePointwise(argv[3]);
		last_time=clock();
		cout << "This program ran for " << (double)last_time/TIME_FACTOR << " s\n";
		return 0;
	}

	if (ins.stringEquality(argv[1], "-int2ins")) {
		//Converts from .int saved (instrument) format to .ins
		
		cout << "**int->ins convert script**\n";
		ins.loadPointwise(argv[2]);
		ins.save(argv[3]);
		last_time=clock();
		cout << "This program ran for " << (double)last_time/TIME_FACTOR << " s\n";
		return 0;
	}

	if (ins.stringEquality(argv[1], "-compare")) {
		//Takes two instrument files, compares their bore profile and outputs the difference as another plot
	
		cout << "**Built-in compare script**\n";
		ins.load(argv[2]);
		ins.compareBore(argv[3], argv[4]);
		last_time=clock();
		cout << "This program ran for " << (double)last_time/TIME_FACTOR << " s\n";
		return 0;
	}

	if (ins.stringEquality(argv[1], "-customimpedance")) {
		//Calculates input impedance plot of given instrument			
		
		cout << "**Impedance calculation script**\n";
		if (argc==7) {
			char temp[32]; //used to construct filenames of saved files
			strcpy(temp, argv[2]);
			int nModes = atoi(argv[3]);
			int fStart = atoi(argv[4]);
			int fStop  = atoi(argv[5]);
			int nPoints = atoi(argv[6]);

			if(ins.load(strcat(temp, ".ins"))==true) {
				ins.roughPlot(nModes, fStart, fStop, nPoints);
				strcpy(temp, argv[2]);
				strcat(temp, ".ic");
				ins.impedanceCurve.save(temp); strcpy(temp, argv[2]);
			} else cout << "Aborting script, calculating instrument\n";
			//ins.printDescription();
		} else cout << "ERROR at command-line : bad arguments: requires -customimpedance <filename> <nModes> <frqStart> <frqEnd> <nPoints>\n";
		last_time=clock();
		cout << "This program ran for " << (double)last_time/TIME_FACTOR << " s\n";
		return 0;
	}

	if (ins.stringEquality(argv[1], "-impedance")) {
		//Calculates input impedance plot of given instrument			
		
		cout << "**Impedance calculation script**\n";
		if (argc==4) {
			char temp[32]; //used to construct filenames of saved files
			strcpy(temp, argv[2]);
			int nModes = atoi(argv[3]);
			char modes[3];
			itoa(nModes, modes, 10);

			if(ins.load(strcat(temp, ".ins"))==true) {
				strcpy(temp, argv[2]);
				ins.roughPlot(nModes, 10, 2000, 500);
				strcat(temp, "_C"); strcat(temp, modes); strcat(temp, ".ic");
				ins.impedanceCurve.save(temp); strcpy(temp, argv[2]);
				ins.refinePeaks(0.00001);

				//ins.impedanceCurve.findPeaks(false);
				strcat(temp, "_C"); strcat(temp, modes); 
				ins.saveImpedancePlot(temp); 
				strcpy(temp, argv[2]);
				ins.saveInstrumentPlot(strcat(temp, "_H")); 
				ins.impedanceCurve.printPeaks();
			} else cout << "Aborting script, printing current instrument\n";
			//ins.printDescription();
		} else cout << "ERROR at command-line : bad arguments: requires -impedance <filename> <nModes>\n";
		last_time=clock();
		cout << "This program ran for " << (double)last_time/TIME_FACTOR << " s\n";
		return 0;
	}

	if (ins.stringEquality(argv[1], "-presentation")) {
		//Calculates input impedance plot of given instrument on a very fine grid for presentation output
		
		cout << "**Impedance calculation script (presentation)**\n";
		if (argc==4) {
			char temp[32];
			strcpy(temp, argv[2]);
			int nModes = atoi(argv[3]);
			char modes[3];
			itoa(nModes, modes, 10);

			if(ins.load(strcat(temp, ".ins"))==true) {
				strcpy(temp, argv[2]);
				ins.roughPlot(nModes, 10, 1200, 2000);
				strcat(temp, "_pC"); strcat(temp, modes); strcat(temp, ".ic");
				ins.impedanceCurve.save(temp); strcpy(temp, argv[2]);
				ins.refinePeaks(0.00001);
				
				//ins.impedanceCurve.findPeaks(false);
				strcat(temp, "_pC"); strcat(temp, modes); 
				ins.saveImpedancePlot(temp); 
				strcpy(temp, argv[2]);
				ins.saveInstrumentPlot(strcat(temp, "_H")); 
				ins.impedanceCurve.printPeaks();
			} else cout << "Aborting script, printing current instrument\n";
			ins.printDescription();
		} else cout << "ERROR at command-line : bad arguments: requires -impedance <filename> <nModes>\n";
		last_time=clock();
		cout << "This program ran for " << (double)last_time/TIME_FACTOR << " s\n";
		return 0;
	}

	if (ins.stringEquality(argv[1], "-showimp")) {
		//Calculates input impedance plot of given instrument without saving output
		
		cout << "**Impedance calculation script (no save)**\n";
		if (argv[2]) {
			char temp[17];
			strcpy(temp, argv[2]);
			const int nModes = atoi(argv[3]);

			if(ins.load(strcat(temp, ".ins"))==true){
				strcpy(temp, argv[2]);
				ins.roughPlot(nModes, 10, 2000, 500);
				ins.refinePeaks(0.00001);
				ins.impedanceCurve.printPeaks();
			} else cout << "Aborting script, printing current instrument\n";
			ins.printDescription();
		} else cout << "ERROR at command-line : bad arguments: requires -showimp <filename> <nModes>\n";
		last_time=clock();
		cout << "This program ran for " << (double)last_time/TIME_FACTOR << " s\n";
		return 0;
	}

	if (ins.stringEquality(argv[1], "-benchmark")) {
		// Repeatedly calculates impedance of benchmark.ins and outputs computation time
		
		cout << "**Benchmark script**\n";
		if (argv[2] && argv[3]) {
			const int nTests = atoi(argv[2]);
			const int nModes = atoi(argv[3]);
			cout << "Evaluating impedance of benchmark instrument " << nTests << " times, nModes = " << nModes << "\n";
		
			ins.setOutputMode(true);
			ins.setVerboseMode(false);
			if(ins.load("benchmark.ins")==true) {  
				//ins.load("MODES/88H-braden-lp.ins");
				last_time=clock();
				for (int i=0; i<nTests; ) {
					cout << ++i << endl;
					ins.impedanceCurve.clear();
					ins.impedanceCurve = ImpedanceCurve();
					ins.roughPlot(nModes, 10, 2000, 250);
				}
				last_time=clock();
				cout << "This program ran for total " << (double)last_time/TIME_FACTOR << " s\n";
				cout << "Mean time per impedance calculation: " << (double)last_time/TIME_FACTOR/nTests << " s\n";
				ins.impedanceCurve.save("benchmark.ic");
				ins.saveImpedancePlot("benchmark_C");
			} else {cout << "Aborting script, printing current instrument\n"; ins.printDescription();}
		} else cout << "ERROR at command-line : bad arguments: requires -benchmark <nTests> <nModes>\n";
		last_time=clock();
		cout << "This program ran for " << (double)last_time/TIME_FACTOR << " s\n";
		return 0;
	}

	if (ins.stringEquality(argv[1], "-print")) {
		//Loads instrument and prints it to screen.
	
		cout << "**Print script**\n";	
		if(argc==3) {
			char temp[32];
			strcpy(temp, argv[2]);
			ins.load(strcat(temp, ".ins"));
			ins.printDescription();
		} else cout << "ERROR at command-line : bad arguments: requires -print <filename>\n";
		last_time=clock();
		cout << "This program ran for " << (double)last_time/TIME_FACTOR << " s\n";
		return 0;
	}

	printUsage(); //called only if none of the above options are called
	return 0;	
}

void printUsage() {
	cout << "Usage:\n";
//	cout << "-sct <filename.sct> : runs script file (deprecated)\n";
	cout << "-rb <filename.rb> : runs Rosenbrock optimiser from given .rb parameter file\n";
//	cout << "-ga <filename.gap> : runs Genetic Algorithm from given .gap script file (deprecated)\n";
	cout << "-ic <filename.ic> : Loads .ic impedance curve and saves as maple/matlab plot\n";
	cout << "-peaks <filename.ic> : Loads .ic impedance curve and prints peak data to screen\n";
	cout << "-import <biasfile.txt> <output> : imports BIAS ASCII file and saves output_M.m  and output_M.ic\n";
	cout << "-importstd <biasfile.txt> <output> : imports BIAS ASCII file and saves output_sM.m  and output_sM.ic in STANDARD FORM\n";
	cout << "-ins2int <input.ins> <output.int> : converts elemental .ins file into pointwise .int file\n";
	cout << "-int2ins <input.int> <output.ins> : converts pointwise .int file into elemental .ins file\n";
//	cout << "-compare <a.ins> <b.ins> <output.mws> : compares bores of a and b and saves to output.mws (deprecated)\n";
	cout << "-customimpedance <filename> <nModes> <frqStart> <frqEnd> <nPoints>: Loads filename.ins, calculates n-modal impedance curve and saves several filename.<ext> files\n";
	cout << "-impedance <filename> <nModes> : Loads filename.ins, calculates n-modal impedance curve and saves several filename.<ext> files\n";
	cout << "-showimp <filename> <nModes> : Loads filename.ins, and calculates n-modal impedance curve (no save)\n";
	cout << "-benchmark <nTests> <nModes> : Repeated, timed calculation of n-modal impedance of prescribed benchmark instrument\n";
	cout << "-print <filename> : Loads .filename.ins file and prints to screen\n";
	cout << "-harmonicity <filename> <nModes> <nPeaks> :  Loads filename.ins, evaluates impedance with nModes and save an nPeaks plot to harmonicity.m\n";
	cout << "\nSee readme files for more details\n";
}
