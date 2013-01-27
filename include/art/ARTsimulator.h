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

/*
 * ARTSimulator.h
 *
 *  Created on: Jul 11, 2012
 *      Author: cbg
 */

#ifndef ARTSIMULATOR_H_
#define ARTSIMULATOR_H_

//#include <list>
#include <map>
#include "mpParser.h"
#include "Interface.h"

using namespace mup;
//using std::list;

// forward declaration
class ARTItimeModule;

/**
 * Simulators belong to a simulation domain (frequency, time) and simulate waves of a
 * certain type (plain, spherical, multimodal).
 */
class ARTsimulator : public ARTobject {
protected:
	ARTproperty domain_; //frequency or (some day) time
	ParserX* parser_;
public:

	ARTsimulator(const string name, const string domain="invalidDomain",
				 const string sds="", const string lds="", const string htm="");

	virtual ARTproperty* GetDomain() {return &domain_;}
	virtual ParserX* GetParser() {return parser_;}

	virtual ARTdataProp* FindDataPropInSimulator(string exp);
	virtual ~ARTsimulator();
	//these lists point into list of AcousticResearchTool Object
	ARTlistProp* userElements;
	ARTlistProp* circuits;
};

class ARTfreqSimulator : public ARTsimulator
{
protected:
	ARTproperty wavetype_; //plain, spherical, multimodal
	ARTdataContainer* frqGrid;
	ARTdataContainer* wfrqGrid;
	ARTdataContainer* modes;
public:

	ARTfreqSimulator(const string name, const string domain="FrequencyDomain", const string wavetype="MultiModal",
					 const string sds="", const string lds="", const string htm="");

	//void SetMultimodeParameters(ARTdataProp* fmin, ARTdataProp* fmax, ARTdataProp* fstep, ARTdataProp* modes);

	virtual ARTdataContainer* GetFrequencyGrid() {return frqGrid;}
	virtual ARTdataContainer* GetAngularFrequencyGrid() {return wfrqGrid;}
	virtual ARTdataContainer* GetNumberOfModes() {return modes;}

	virtual ARTproperty* GetWavetype() {return &wavetype_;}

	virtual ~ARTfreqSimulator() {}
};


class ARTtimeSimulator : public ARTsimulator
{
public:

	ARTtimeSimulator(const string name, const string domain="TimeDomain",
					 const string sds="", const string lds="", const string htm="");

	virtual void AddTimeModule(ARTItimeModule* timeModule);
	virtual void AddSimulationParameter(const string& name, const string& expr);
	virtual void AddSimulationParameter(const string& name, const std::complex<double>& val);
	virtual void AddSimulationParameter(const string& name, double val);

	virtual void SimulateTimeStep(int idx);

	virtual void SetSimulationParameter(const string& name, const string& expr);
	virtual void SetSimulationParameter(const string& name, const std::complex<double>& val);
	virtual void SetSimulationParameter(const string& name, double val);

	virtual ARTdataProp* FindDataPropInSimulator(string exp);
	virtual ARTItimeModule* FindTimeModuleInSimulator(string exp);

	virtual ~ARTtimeSimulator();
protected:
	struct simulParameterType
	{
		ARTdataContainer* _val;
		ParserX* _parser;
	};
	typedef std::map<const string, simulParameterType*> simulParameterMap;
	typedef simulParameterMap::iterator simulParameterMapIterator;

	//simulParameterMap _simulParams;

	virtual void initStandardSimulParams();

	virtual void clean();
	virtual void addParamsToModule(ARTItimeModule* timeModule);
	//virtual void addParamToCurrentModules(const string& name, simulParameterType* newParam);
	virtual void addParamToCurrentModules(ARTdataProp* newParam);

};

#endif /* ARTSIMULATOR_H_ */
