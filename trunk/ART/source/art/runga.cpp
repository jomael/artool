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


// All this is defined at global scope; this is essentially an extension of main.cpp

// ***************
// * DEPRECATED! *
// ***************
// this code is currently not maintained. Early results suggested that the GA was not worth pursuing. Code retained in case I ever wanted to make further investigations
// Will need to be tidied up before further use. Execute at own risk...


#ifndef GA_CPP
#define GA_CPP

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

//#include <ga/ga.h>
//#include <ga/GAListGenome.h> 
#include <ga/GAGenome.h>
#include <ga/GASelector.h>
#include <ga/GASimpleGA.h>


#ifndef OBJ_PARAMS
#define OBJ_PARAMS
	struct objectiveParameters {
		// encapsulates parameters needed within scope of objective function (and must therefore be passed through rosenbrock function).
		double nu_phi, nu_z, nu, mu_phi, mu_z, maxfreq;
		int nPeaks, nPoints, nModes;
		double *peakFreq, *peakHeight;
		bool scoreIC, scoreFreq, scoreHeight;
	};
#endif


#include "WindInstrument.h"
using namespace ART;

void runGA (char filename[]);
float objective(GAGenome &);
void splitPopulation (const GAPopulation &pop);

void runGA (char filename[]) {
	WindInstrument ins = WindInstrument ();
	objectiveParameters params;
	char* parfilename;
	char* targetfilename;
	bool hasTarget=false;
	int popSize, nGen;
	double pMut, pCross;


	cout << "*Loading parameter file: " << filename << endl; //This whole section ought to be in its own function, but MSVCPP didn't like my pointer-to-a-pointer-to-an-array (params).
	//Loads .gap file and puts values into params.

	ifstream file(filename);
	char buffer[300], command[300];
	int line=0, ptr; //Counts the number of lines read from file. ptr keeps track of where in each line the program is reading

	if (!file.is_open()) {cout << "ERROR: File failed to open\n"; return;}
	
	while (!file.eof()) {
		line++;
		file.getline(buffer, 300);
		if (buffer[0]!=';') { //ignoring comments in the file
			command[0]='\0';
			ptr=0;
			for (int i=0; i<200; i++) { //finds command
				if (buffer[i]!=' ' && buffer[i]!='\n' && buffer[i]!='\0') {
					command[i]=buffer[i];
				} else {
					command[i]='\0';
					ptr=i+1;
					break;
				}
			}
			//Chooses which command to call, then reads the relevant parameters and calls it
			// Note that switch statements don't work on char arrays
			if (ins.stringEquality(command, "popsize\0")) popSize=ins.parseInt(buffer, &ptr);
			else if (ins.stringEquality(command, "ngen\0")) nGen=ins.parseInt(buffer, &ptr); 
			else if (ins.stringEquality(command, "pmut\0")) pMut=ins.parseDouble(buffer, &ptr); 
			else if (ins.stringEquality(command, "pcross\0")) pCross=ins.parseDouble(buffer, &ptr);
			else if (ins.stringEquality(command, "nu_phi\0")) params.nu_phi=ins.parseDouble(buffer, &ptr); 
			else if (ins.stringEquality(command, "nu_z\0")) params.nu_z=ins.parseDouble(buffer, &ptr); 
			else if (ins.stringEquality(command, "mu_phi\0")) params.mu_phi=ins.parseDouble(buffer, &ptr); 
			else if (ins.stringEquality(command, "mu_z\0")) params.mu_z=ins.parseDouble(buffer, &ptr); 
//			else if (ins.stringEquality(command, "thresh\0")) temparray[8]=ins.parseInt(buffer, &ptr); 
//			else if (ins.stringEquality(command, "mu\0")) temparray[9]=ins.parseDouble(buffer, &ptr); 
			else if (ins.stringEquality(command, "peaks\0")) params.nPeaks=ins.parseInt(buffer, &ptr);
			else if (ins.stringEquality(command, "maxfreq\0")) params.maxfreq=ins.parseInt(buffer, &ptr);
			else if (ins.stringEquality(command, "setOutputMode\0") || ins.stringEquality(command, "sOM\0")) ins.setOutputMode(ins.parseBool(buffer, &ptr));	
//			else if (ins.stringEquality(command, "dim\0")) params = new double[12+2*(int)temparray[10]];
			else if (ins.stringEquality(command, "peakfreq\0")) {
				params.peakFreq = new double[params.nPeaks];
				for (int i=0; i<params.nPeaks; i++) params.peakFreq[i]=ins.parseDouble(buffer, &ptr);
			}
			else if (ins.stringEquality(command, "peakheight\0")) {
				params.peakHeight = new double[params.nPeaks];
				for (int i=0; i<params.nPeaks; i++) params.peakHeight[i]=ins.parseDouble(buffer, &ptr);
			}
			else if (ins.stringEquality(command, "par\0")) parfilename=ins.parseString(buffer, &ptr);
			else if (ins.stringEquality(command, "target\0")) {targetfilename=ins.parseString(buffer, &ptr); hasTarget=true;}
			else {cout << "ERROR: command not recognised on line " << line << ", aborting\n"; break;}
	
		}
	} 

	cout << endl;
	 
//### GA ###

	WindInstrument genome(parfilename, true);
	if (hasTarget) genome.loadTargetImpedanceCurve(targetfilename);	
	genome.userData((void*)&params);
	genome.evaluator(objective);
	genome.setVerboseMode(false);
	GASimpleGA ga(genome);
	
	GATournamentSelector selector;
	ga.selector(selector);
	ga.populationSize(popSize);
	ga.nGenerations(nGen);
	ga.pMutation(pMut);
	ga.pCrossover(pCross);
	//ga.elitist(gaTrue);
	ga.initialize();
	//cout << ga.population();
	
	int i;
	for(i=1; i<=nGen; i++) 
	{
		cout << "gen " << i << endl;
		++ga;
	//cout << "---\n";
	//cout << ga.population();
	
		if (i==1) {splitPopulation(ga.population());splitPopulation(ga.population());}
		//if (i==20) splitPopulation(ga.population()); //if (hasTarget) genome.loadTargetImpedanceCurve(targetfilename);
//	if (i==3) splitPopulation(ga.population());
	}
	
	//cout << ga.population();
	//ga.evolve();

	//outputting results
	//cout << "The GA found:\n" << ga.statistics().bestIndividual() << "\n";
	cout << endl << "Producing output for best individual. Score: " << ga.statistics().maxEver() << endl;

	
	WindInstrument* best = (WindInstrument*)(&ga.statistics().bestIndividual());
	best->setVerboseMode(false);
	best->roughPlot(0, 10, params.maxfreq, 250,1);
	best->refinePeaks(0.1);
	best->saveInstrumentPlot("gahorn");
	best->saveImpedancePlot("gaic");
	best->impedanceCurve.saveLists("galists.mws", false);
	best->save("ga.ins");
	best->printDescription();

	return;
} 

void splitPopulation (const GAPopulation &pop) {
	//Takes entire population and calls each members' split function.
	const int size = pop.size();
	WindInstrument* member;

	for (int i=0; i<size; i++) {
		member = (WindInstrument*) &(pop.individual(i));
		(*member).ListSplit(false);
	}
	(*member).ListSplit(true); //modifies defaultList
}

float objective(GAGenome& g) {
	//Calculates the fitness of given genome
	WindInstrument &genome = (WindInstrument&)g;
	objectiveParameters *params = (objectiveParameters *)g.userData();

	float score = 0.0;
	genome.impedanceCurve.clear();
	genome.impedanceCurve = ImpedanceCurve();
	
	if (genome.getHasTarget()) {
		genome.roughPlot(0, 10, params->maxfreq, 250,1);
		score = genome.impedanceCurve.scoreCurve(genome.getTarget(), params->nu_z, params->mu_z); 
	} else {genome.roughPlot(0, 10, params->maxfreq, 70,1);}

	genome.refinePeaks(1);
	score = genome.impedanceCurve.scorePeaks(params, true, false);
		
	if (score<0) genome.print();

	cout << score << endl;	
	return score;
}

#endif
