/*
 * ARTSimulator.cpp
 *
 *  Created on: Jul 11, 2012
 *      Author: cbg
 */


#include "ARTsimulator.h"
#include "ARTtimeModule.h"

using namespace mup;

//**************************************************************************************************************
// ARTsimulator

ARTsimulator::ARTsimulator(const string name, const string domain,
		const string sds, const string lds, const string htm) :
		ARTobject(name,sds,lds,htm), domain_(domain), parser_(new ParserX(mup::pckCOMPLEX_NO_STRING))
{
}


/// Finds the data property represented by the string exp; This can be a data property of an element or model in the simulator (then the string is something like "Cyl.length") or a data property of the simulator itself.
ARTdataProp* ARTsimulator::FindDataPropInSimulator(string exp)
{
	ARTdataProp* prop;
	//try to find a property of the simulator with name *exp*
	prop = dynamic_cast<ARTdataProp*>( FindProperty( strcrop( exp ) ));
	//if not found, try to find a property *exp* in the element list
	if (!prop)
	{
		vector<string> nameparts = strsplit(exp,'.');
		ARTelement* element = dynamic_cast<ARTelement*>(userElements->FindObject( strcrop(nameparts[0]).c_str() ));
		if (element == NULL) throw ARTerror("FindDataPropInSimulator", "An element of the specified name '%s1' does not exist and '%s1' is not recognized as a data property of the simulator.", strcrop(nameparts[0]).c_str() );
		prop = dynamic_cast<ARTdataProp*>(element->model->FindProperty( strcrop(nameparts[1]).c_str() ));
		if (prop == NULL) throw ARTerror("FindDataPropInSimulator", "The element '%s1' does not have the specified data property '%s2'.",  strcrop(nameparts[0]).c_str() ,  strcrop(nameparts[1]).c_str() );
	}
	return prop;
}

ARTsimulator::~ARTsimulator()
{
	// IMPORTANT: do NOT free the memory of the parser here!
	//delete (parser_);
}


//**************************************************************************************************************
// ARTfreqSimulator

ARTfreqSimulator::ARTfreqSimulator(const string name, const string domain, const string wavetype,
             const string sds, const string lds, const string htm) :
			 ARTsimulator(name, domain, sds, lds, htm), wavetype_(wavetype)
{

	/*
	The frequency grids are not supposed to be edited by the user and therefore no properties!
	frqGrid = AppendDataProp("frqGrid", new ARTvariant(C_ART_ndbl), "The list of frequencies (in Hz) for which this simulator will calculate the impedance.");
	wfrqGrid = AppendDataProp("wfrqGrid", new ARTvariant(C_ART_ndbl), "The list of frequencies (angular frequency) for which this simulator will calcualte the impedance.");
	*/
	modes = AppendDataProp("NumberOfModes", 1, "The number of modes for which this simulator will calculate the impedance.");

	ARTdataProp* fmin = AppendDataProp("LowerFrequencyLimit", 50.0, "The lower frequency (in Hz) of the range for which this simulator will calculate the impedance.");
	ARTdataProp* fmax = AppendDataProp("HigherFrequencyLimit", 1800.0, "The higher frequency (in Hz) of the range for which this simulator will calculate the impedance.");
	ARTdataProp* fstep = AppendDataProp("FrequencyStep", 5.0, "The frequency step (in Hz) used to go through the range for which this simulator will calculate the impedance.");

	//add properties to parser
	ARTproperty* prop = GetProperties(NULL);
	while (prop)
	{
		//if it is a data property
		ARTdataProp* dprop = dynamic_cast<ARTdataProp*>(prop);
		if (dprop)
		{
			string varname = dprop->GetName();
			dprop->SetParser(parser_);
			dprop->SetParserVar(varname);
			//std::cout << "Created Parser Var: " << varname << "\n";
		}
		prop = GetProperties(prop);
	}

	frqGrid = new ARTdataContainer("frqGrid", new ARTfrqGridFunc(fmin, fmax, fstep));
	wfrqGrid = new ARTdataContainer("wfrqGrid", new ARTwfrqGridFunc(frqGrid));

}

//**************************************************************************************************************
// ARTtimeSimulator

ARTtimeSimulator::ARTtimeSimulator(const string name, const string domain,
								   const string sds, const string lds, const string htm) :
	ARTsimulator(name, domain, sds, lds, htm)
{
}

void ARTtimeSimulator::SetModulesToCurrentTimeIndex(int idx)
{
	// iterate through all time modules of the simulator
	// and set their time index to the specified one
	if (userElements != NULL)
	{
		ARTobject* iter = userElements->GetObjects(NULL);
		ARTtimeModule* tModule;
		while (iter != NULL)
		{
			tModule = dynamic_cast<ARTtimeModule*>(iter);
			if (tModule)
			{
				tModule->setCurrentIndex(idx);
			}
			iter = userElements->GetObjects(iter);
		}
	}
}

