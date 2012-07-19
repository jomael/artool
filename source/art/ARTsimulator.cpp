/*
 * ARTSimulator.cpp
 *
 *  Created on: Jul 11, 2012
 *      Author: cbg
 */

#include <sstream>
#include "ARTsimulator.h"
#include "ARTtimeModule.h"

using namespace mup;

using std::stringstream;

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
	ARTsimulator(name, domain, sds, lds, htm),
	_simulParams()
{
	initStandardSimulParams();
}

void ARTtimeSimulator::AddTimeModule(ARTtimeModule* timeModule)
{

	if (timeModule != NULL && userElements != NULL)
	{
		const string& moduleName = timeModule->GetName();
		ARTobject* iter = userElements->GetObjects(NULL);

		// go through user elements list
		while (iter)
		{
			// if we find a module with the same name, throw an error
			if (iter->GetName() == moduleName)
			{
				throw ARTerror("ARTtimeSimulator::addTimeModule", "An element of the specified name '%s1' is already registered in the current simulator '%s2'.", moduleName, name_);
			}
			iter = userElements->GetObjects(iter);
		}
		// if we are here, no module with the same name has been found
		// => register module in current simulator
		userElements->AppendObject(timeModule);
		// add all global properties to time module
		addParamsToModule(timeModule);
		// register simulator to time module
		timeModule->setSimulator(this);
	}
}

void ARTtimeSimulator::AddGlobalParameter(const string& name, const string& expr)
{
	if (_simulParams.find(name) == _simulParams.end())
	{
		simulParameterType* newParam = new simulParameterType();
		newParam->_parser = new ParserX(mup::pckCOMPLEX_NO_STRING);
		newParam->_val = new ARTdataContainer(C_ART_cpx, 0, name);
		newParam->_val->SetParser(newParam->_parser);
		newParam->_val->SetDefinition(expr);
		_simulParams[name] = newParam;
		addParamToCurrentModules(name, newParam);
	}
	else
	{
		throw ARTerror("ARTtimeSimulator::AddGlobalParameter",
				"Could not add global parameter '%s1': Parameter with same name already exists in current simulator.",
				name);
	}

}

void ARTtimeSimulator::AddGlobalParameter(const string& name, const std::complex<double>& val)
{
	if (_simulParams.find(name) == _simulParams.end())
	{
		simulParameterType* newParam = new simulParameterType();
		newParam->_parser = new ParserX(mup::pckCOMPLEX_NO_STRING);
		newParam->_val = new ARTdataContainer(C_ART_cpx, 0, name);
		newParam->_val->SetParser(newParam->_parser);
		newParam->_val->SetVal(val);
		_simulParams[name] = newParam;
		addParamToCurrentModules(name, newParam);
	}
	else
	{
		throw ARTerror("ARTtimeSimulator::AddGlobalParameter",
				"Could not add global parameter '%s1': Parameter with same name already exists in current simulator.",
				name);
	}

}

void ARTtimeSimulator::AddGlobalParameter(const string& name, double val)
{
	if (_simulParams.find(name) == _simulParams.end())
	{
		simulParameterType* newParam = new simulParameterType();
		newParam->_parser = new ParserX(mup::pckCOMPLEX_NO_STRING);
		newParam->_val = new ARTdataContainer(C_ART_cpx, 0, name);
		newParam->_val->SetParser(newParam->_parser);
		newParam->_val->SetVal(val);
		_simulParams[name] = newParam;
		addParamToCurrentModules(name, newParam);
	}
	else
	{
		throw ARTerror("ARTtimeSimulator::AddGlobalParameter",
				"Could not add global parameter '%s1': Parameter with same name already exists in current simulator.",
				name);
	}

}

void ARTtimeSimulator::SetModulesToCurrentTimeIndex(int idx)
{

	// set current time index parameter if it exists
	if (_simulParams.find("t") != _simulParams.end())
	{
		ARTdataContainer& tmpContainer = *(_simulParams["t"]->_val);
		tmpContainer.SetVal(idx);
	}

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

void ARTtimeSimulator::SetSimulationParameter(const string& name, const string& expr)
{
	if (_simulParams.find(name) != _simulParams.end())
	{
		ARTdataContainer* tmpContainer = (_simulParams[name])->_val;
		tmpContainer->SetDefinition(expr);
	}
	else
	{
		throw ARTerror("ARTtimeSimulator::SetSimulationParameter",
				"Could not set parameter '%s1' to expression '%s2': No such parameter exists in current simulator.",
				name,
				expr);
	}
}

void ARTtimeSimulator::SetSimulationParameter(const string& name, const std::complex<double>& val)
{
	if (_simulParams.find(name) != _simulParams.end())
	{
		ARTdataContainer* tmpContainer = (_simulParams[name])->_val;
		tmpContainer->SetVal(val);
	}
	else
	{
		stringstream tmpString;
		tmpString << "(" << val.real() << "," << val.imag() << ")";
		throw ARTerror("ARTtimeSimulator::SetSimulationParameter",
				"Could not set parameter '%s1' to complex value '%s2': No such parameter exists in current simulator.",
				name,
				tmpString.str());
	}
}

void ARTtimeSimulator::SetSimulationParameter(const string& name, double val)
{
	if (_simulParams.find(name) != _simulParams.end())
	{
		ARTdataContainer* tmpContainer = (_simulParams[name])->_val;
		tmpContainer->SetVal(val);
	}
	else
	{
		stringstream tmpString;
		tmpString << val;
		throw ARTerror("ARTtimeSimulator::SetSimulationParameter",
				"Could not set parameter '%s1' to double value '%s2': No such parameter exists in current simulator.",
				name,
				tmpString.str());
	}
}

ARTtimeSimulator::~ARTtimeSimulator()
{
	clean();
}

void ARTtimeSimulator::initStandardSimulParams()
{
	ARTdataContainer* tmpContainer;
	ParserX* tmpParser;
	simulParameterType* tmpParam;

	// create new simulation parameter for sampling period
	tmpContainer = new ARTdataContainer(C_ART_cpx, 0, "T");
	tmpParser = new ParserX(mup::pckCOMPLEX_NO_STRING);
	tmpContainer->SetParser(tmpParser);
	tmpContainer->SetVal(1/44100);

	tmpParam = new simulParameterType();
	tmpParam->_val = tmpContainer;
	tmpParam->_parser = tmpParser;

	_simulParams["T"] = tmpParam;

	// create new simulation parameter time index
	tmpContainer = new ARTdataContainer(C_ART_int, 0, "t");
	tmpParser = new ParserX(mup::pckCOMPLEX_NO_STRING);
	tmpContainer->SetParser(tmpParser);
	tmpContainer->SetVal(0);

	tmpParam = new simulParameterType();
	tmpParam->_val = tmpContainer;
	tmpParam->_parser = tmpParser;

	_simulParams["t"] = tmpParam;
}

void ARTtimeSimulator::clean()
{
	simulParameterMapIterator iter;

	// deallocate memory for all saved simulation parameters
	for (iter = _simulParams.begin(); iter != _simulParams.end(); ++iter)
	{
		delete (iter->second->_val);
		delete (iter->second->_parser);
		delete (iter->second);
	}

	_simulParams.clear();
}


void ARTtimeSimulator::addParamsToModule(ARTtimeModule* timeModule)
{
	simulParameterMapIterator iter;
	for (iter = _simulParams.begin(); iter != _simulParams.end(); ++iter)
	{
		timeModule->addGlobalParameter(iter->first,
				iter->second->_val->GetParserVar());
	}
}

void ARTtimeSimulator::addParamToCurrentModules(const string& name, simulParameterType* newParam)
{
	ARTtimeModule* iter = dynamic_cast<ARTtimeModule*>(userElements->GetObjects(NULL));
	while (iter != NULL)
	{
		iter->addGlobalParameter(name, newParam->_val->GetParserVar());
		iter = dynamic_cast<ARTtimeModule*>(userElements->GetObjects(iter));
	}
}
