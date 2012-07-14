/*
 * ARTSimulator.h
 *
 *  Created on: Jul 11, 2012
 *      Author: cbg
 */

#ifndef ARTSIMULATOR_H_
#define ARTSIMULATOR_H_

#include <list>
#include "mpParser.h"
#include "Interface.h"

using namespace mup;
using std::list;

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
protected:

public:

	ARTtimeSimulator(const string name, const string domain="TimeDomain",
					 const string sds="", const string lds="", const string htm="");

	virtual void SetModulesToCurrentTimeIndex(int idx);

	virtual ~ARTtimeSimulator() {}
};

#endif /* ARTSIMULATOR_H_ */
