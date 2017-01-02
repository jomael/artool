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
*  Copyright (C) 2013 by the authors and their organisations:             *
*    Alistair Braden            1)                                        *
*    Wilfried Kausel            2)         kausel(at)mdw.ac.at            *
*    Delphine Cadefaux          2)                                        *
*    Vasileios Chatziioannou    2)                                        *
*    Sadjad Siddiq              2)                                        *
*    Clemens Geyer              2)                                        *
*                                                                         *
*    1) School of Physics and Astronomy, Univ. of Edinburgh, GB           *
*       (http://www.ph.ed.ac.uk/)                                         *
*    2) Inst. of Music Acoustics, Univ. of Music, Vienna, AT              *
*       (http://iwk.mdw.ac.at)                                            *
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

#ifndef IMPEDANCECURVE_H
#define IMPEDANCECURVE_H

#include "Horn.h"

using std::list;
using namespace std;

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

const double LOG2=log10(2.0);


class ImpedanceCurve : public Horn {
public:
	ImpedanceCurve();
//	~ImpedanceCurve();

	void setVerboseMode (const bool isVerbose) {verboseMode_ = isVerbose;}
	bool getVerboseMode() const {return verboseMode_;}
	void setOutputMode (const bool outputMode) {outputMode_ = outputMode;}
	bool getOutputMode() const {return outputMode_;}
	std::list<double> getY() const {return getR();}	//Defined solely to make user-handling of the... 
	double getY (int i) {return getR(i);} // class more intuitive
	ImpedanceCurve& operator = (const ImpedanceCurve& a);
	void saveImpedancePlot (const char filename[]);
	void saveLists (const char filename[], const bool fromSaveAsMaple);
	void peakListAppend (const double x, const double y);	
	void peakListAppend (const double x, const double y, const double e);	
	void peakListModify (const double x, const double y);
	void peakListModify (const double x, const double y, const double e);
	void namePeaks();
	double getPeakList(const int x, const int y) const { return peakList_[x][y]; }
	double getPeakNames(const int x, const int y) const { return peakNames_[x][y]; }
	double getCentList(const int x, const int y) const { return centList_[x][y]; }
	int getPeakListCounter() const { return peakListCounter_; }
	void printPeaks() const;
	void printPeaks(double* insParams) const;
	void printPeakData() const;
	float scoreCurve (ImpedanceCurve *target, const float cic, const float mu_z);
	float scorePeaks (const objectiveParameters* params, const bool scoreFreq, const bool scoreHeight);
	float scoreChallengeEFP(); 
	float scoreChallenge(); 
	void importBIAS (const char filename[]);
	void convertToStandard();
	void findPeaks(const bool ignoreNoise);
	double efp(const double F, const double fn, const int n) const;
	double inverseEFP(const double efp, const double F , const int i) const; 	
	void harmonicityPlot(const double efpMin, const double efpMax, const int nEvals, const int peakMin, const int peakMax, char filename[], const double F);
	void harmonicityPlot(const double efpMin, const double efpMax, const int nEvals, const int peakMin, const int peakMax, char filename[]);
	double getMagnitude(const double f);
	dcomp  getImpedance(const double f);
	

private:

	void saveImpedancePlotMaple (const char filename[]);
	void saveImpedancePlotMatlab (const char filename[]);
	bool verboseMode_;
	bool outputMode_;
	double peakList_[30][3]; //Only the first 30 peaks are stored; this is as many as is musically useful
	double peakNames_[30][2]; //[note number (C=1, C#/Db=2 etc.), octave]
	double centList_[30][2]; //[freq. of perfectly-tuned equal-temprament note, cents sharp] - used to make a plot
	int peakListCounter_;
};

#endif

