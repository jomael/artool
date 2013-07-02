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
    ARTobject(name,sds,lds,htm), domain_(domain), parser_(new ParserX(mup::pckCOMPLEX_NO_STRING)),
    userElements(NULL)
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
    if (element == NULL) throw ARTerror("ARTsimulator::FindDataPropInSimulator", "An element of the specified name '%s1' does not exist and '%s1' is not recognized as a data property of the simulator.", strcrop(nameparts[0]).c_str() );
    prop = dynamic_cast<ARTdataProp*>(element->model->FindProperty( strcrop(nameparts[1]).c_str() ));
    if (prop == NULL) throw ARTerror("ARTsimulator::FindDataPropInSimulator", "The element '%s1' does not have the specified data property '%s2'.",  strcrop(nameparts[0]).c_str() ,  strcrop(nameparts[1]).c_str() );
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

ARTfreqSimulator::ARTfreqSimulator(const string name, const string wavetype,
    const string sds, const string lds, const string htm) :
    ARTsimulator(name, "FrequencyDomain", sds, lds, htm), wavetype_(wavetype)
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

ARTtimeSimulator::ARTtimeSimulator(const string name, const string sds,
    const string lds, const string htm) :
    ARTsimulator(name, "TimeDomain", sds, lds, htm)
//	_simulParams()
{
  initStandardSimulParams();
}

void ARTtimeSimulator::AddTimeModule(ARTItimeModule* timeModule)
{
  if (!timeModule)
  {
    throw ARTerror("ARTtimeSimulator::AddTimeModule", "Could not add time module to current simulator '%s1': invalid time module!",
        name_);
  }
  if (!userElements)
  {
    throw ARTerror("ARTtimeSimulator::AddTimeModule", "Could not add time module '%s1' to current simulator '%s2': no memory allocated for userElements!",
        timeModule->GetName(), name_);
  }

  const string& moduleName = timeModule->GetName();
  if(userElements->FindObject(moduleName) != NULL)
  {
    throw ARTerror("ARTtimeSimulator::addTimeModule", "An element of the specified name '%s1' is already registered in the current simulator '%s2'.", moduleName, name_);
  }
  // if we are here, no module with the same name has been found
  // => register module in current simulator
  userElements->AppendObject(timeModule);
  // add all global properties to time module
  addParamsToModule(timeModule);
  // register simulator to time module
  timeModule->setSimulator(this);
}

void ARTtimeSimulator::AddSimulationParameter(const string& name, const string& expr)
{
  //	if (_simulParams.find(name) == _simulParams.end())
  //	{
  //		simulParameterType* newParam = new simulParameterType();
  //		newParam->_parser = new ParserX(mup::pckCOMPLEX_NO_STRING);
  //		newParam->_val = new ARTdataContainer(C_ART_cpx, 0, name);
  //		newParam->_val->SetParser(newParam->_parser);
  //		newParam->_val->SetDefinition(expr);
  //		_simulParams[name] = newParam;
  //		addParamToCurrentModules(name, newParam);
  //	}
  ARTdataProp* prop = NULL;
  try
  {
    prop = FindDataPropInSimulator(name);
  }
  catch(ARTerror& e)
  {
    prop = new ARTdataProp(C_ART_cpx, 0, name);
    prop->SetParser(parser_);
    prop->SetDefinition(expr, this);
    AppendDataProp(prop);
    addParamToCurrentModules(prop);
    return;
  }
  throw ARTerror("ARTtimeSimulator::AddGlobalParameter",
      "Could not add global parameter '%s1': Parameter with same name already exists in current simulator.",
      name);
}

void ARTtimeSimulator::AddSimulationParameter(const string& name, const std::complex<double>& val)
{
  //	if (_simulParams.find(name) == _simulParams.end())
  //	{
  //		simulParameterType* newParam = new simulParameterType();
  //		newParam->_parser = new ParserX(mup::pckCOMPLEX_NO_STRING);
  //		newParam->_val = new ARTdataContainer(C_ART_cpx, 0, name);
  //		newParam->_val->SetParser(newParam->_parser);
  //		newParam->_val->SetVal(val);
  //		_simulParams[name] = newParam;
  //		addParamToCurrentModules(name, newParam);
  //	}
  ARTdataProp* prop = NULL;
  try
  {
    prop = FindDataPropInSimulator(name);
  }
  catch(ARTerror& e)
  {
    prop = new ARTdataProp(C_ART_cpx, 0, name);
    prop->SetParser(parser_);
    prop->SetVal(val);
    AppendDataProp(prop);
    addParamToCurrentModules(prop);
    return;
  }
  throw ARTerror("ARTtimeSimulator::AddGlobalParameter",
      "Could not add global parameter '%s1': Parameter with same name already exists in current simulator.",
      name);

}

void ARTtimeSimulator::AddSimulationParameter(const string& name, double val)
{
  ARTdataProp* prop = NULL;
  try
  {
    prop = FindDataPropInSimulator(name);
  }
  catch(ARTerror& e)
  {
    prop = new ARTdataProp(C_ART_cpx, 0, name);
    prop->SetParser(parser_);
    prop->SetVal(val);
    AppendDataProp(prop);
    addParamToCurrentModules(prop);
    return;
  }
  throw ARTerror("ARTtimeSimulator::AddGlobalParameter",
      "Could not add global parameter '%s1': Parameter with same name already exists in current simulator.",
      name);

  //	if (_simulParams.find(name) == _simulParams.end())
  //	{
  //		simulParameterType* newParam = new simulParameterType();
  //		newParam->_parser = new ParserX(mup::pckCOMPLEX_NO_STRING);
  //		newParam->_val = new ARTdataContainer(C_ART_cpx, 0, name);
  //		newParam->_val->SetParser(newParam->_parser);
  //		newParam->_val->SetVal(val);
  //		_simulParams[name] = newParam;
  //		addParamToCurrentModules(name, newParam);
  //	}

}

void ARTtimeSimulator::SimulateTimeStep(int idx)
{

  // set current time index parameter if it exists
  ARTdataProp* prop = FindDataPropInSimulator("t");
  //	if (_simulParams.find("t") != _simulParams.end())
  //	{
  //		ARTdataContainer& tmpContainer = *(_simulParams["t"]->_val);
  //		tmpContainer.SetVal(idx);
  //	}
  if (prop != NULL)
  {
    //		std::cout << "Setting current global parameter 't' to " << idx << "." << std::endl;
    prop->SetVal(idx);
  }
  else
  {
    throw ARTerror("ARTtimeSimulator::SetModulesToCurrentTimeIndex", "Could not find global simulation parameter 't' in simulator '%s1' which is needed for time domain simulation.",
        name_);
  }

  // iterate through all time modules of the simulator
  // and set their time index to the specified one
  if (userElements != NULL)
  {
    ARTobject* iter = userElements->GetObjects(NULL);
    ARTItimeModule* tModule;
    while (iter != NULL)
    {
      tModule = dynamic_cast<ARTItimeModule*>(iter);
      if (tModule)
      {
        //				std::cout << "Setting time module '" << tModule->GetName() << "' to time index " << idx << "." << std::endl;
        tModule->setCurrentIndex(idx);
      }
      iter = userElements->GetObjects(iter);
    }

    iter = userElements->GetObjects(NULL);
    while (iter != NULL)
    {
      tModule = dynamic_cast<ARTItimeModule*>(iter);
      if (tModule)
      {
        //				std::cout << "Setting time module '" << tModule->GetName() << "' to time index " << idx << "." << std::endl;
        tModule->simulateCurrentIndex(idx);
      }
      iter = userElements->GetObjects(iter);
    }
  }
}

void ARTtimeSimulator::SetSimulationParameter(const string& name, const string& expr)
{
  ARTdataProp* prop = FindDataPropInSimulator(name);
  //	if (_simulParams.find(name) != _simulParams.end())
  //	{
  //		ARTdataContainer* tmpContainer = (_simulParams[name])->_val;
  //		tmpContainer->SetDefinition(expr);
  //	}
  if (prop != NULL)
  {
    prop->SetDefinition(expr);
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
  ARTdataProp* prop = FindDataPropInSimulator(name);
  //	if (_simulParams.find(name) != _simulParams.end())
  //	{
  //		ARTdataContainer* tmpContainer = (_simulParams[name])->_val;
  //		tmpContainer->SetVal(val);
  //	}
  if (prop != NULL)
  {
    prop->SetVal(val);
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
  ARTdataProp* prop = FindDataPropInSimulator(name);
  //	if (_simulParams.find(name) != _simulParams.end())
  //	{
  //		ARTdataContainer* tmpContainer = (_simulParams[name])->_val;
  //		tmpContainer->SetVal(val);
  //	}
  if (prop != NULL)
  {
    prop->SetVal(val);
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

ARTdataProp* ARTtimeSimulator::FindDataPropInSimulator(string exp)
{
  ARTdataProp* prop;
  //try to find a property of the simulator with name *exp*
  prop = dynamic_cast<ARTdataProp*>( FindProperty( strcrop( exp ) ));
  // if we have not found a property of the simulator,
  // perhaps we find a property of an internal time module
  if (!prop)
  {
    if (userElements)
    {
      vector<string> names = strsplit(exp, '.');
      ::size_t pos;
      ARTItimeModule* timeModule = FindTimeModuleInSimulator(strcrop(names[0]));
      if (!timeModule)
      {
        throw ARTerror("ARTtimeSimulator::FindDataPropInSimulator", "The specified time module '%s1' does not exist in current simulator.",  strcrop(names[0]));
      }
      // remove any array accesses in case of a port
      pos = names[1].find('[');
      if (pos != names[1].npos)
      {
        names[1].erase(pos);
      }
      prop = dynamic_cast<ARTdataProp*>(timeModule->FindProperty(strcrop(names[1])));
      if (!prop)
      {
        throw ARTerror("ARTtimeSimulator::FindDataPropInSimulator", "The specified data property '%s1' does not exist in time module '%s2'.",
            strcrop(names[1]), strcrop(names[0]));
      }
    }
    else
    {
      throw ARTerror("ARTtimeSimulator::FindDataPropInSimulator", "The specified data property '%s1' does not exist in current simulator.",  exp);
    }
  }
  return prop;
}

ARTItimeModule* ARTtimeSimulator::FindTimeModuleInSimulator(string exp)
{
  if (!userElements)
  {
    throw ARTerror("ARTtimeSimulator::FindTimeModuleInSimulator", "No memory has been allocated for userElements of time simulator '%s1'.",
        name_);
  }
  ARTItimeModule* tmpModule = dynamic_cast<ARTItimeModule*>(userElements->FindObject(exp));
  if (tmpModule == NULL)
  {
    throw ARTerror("ARTtimeSimulator::FindTimeModuleInSimulator", "No time module with name '%s1' exists in simulator '%s2'.",
        exp, name_);
  }
  return tmpModule;
}

ARTtimeSimulator::~ARTtimeSimulator()
{
  clean();
}

void ARTtimeSimulator::initStandardSimulParams()
{
  //	ARTdataContainer* tmpContainer;
  //	ParserX* tmpParser;
  //	simulParameterType* tmpParam;

  ARTdataProp* tmpProp;

  // create new simulation parameter for sampling period
  tmpProp = new ARTdataProp(C_ART_dbl, 0, "T");
  //	tmpParser = new ParserX(mup::pckCOMPLEX_NO_STRING);
  tmpProp->SetParser(parser_);
  tmpProp->SetVal(1.0/44100.0);

  AppendDataProp(tmpProp);

  //	tmpParam = new simulParameterType();
  //	tmpParam->_val = tmpContainer;
  //	tmpParam->_parser = tmpParser;

  //	_simulParams["T"] = tmpParam;

  // create new simulation parameter time index
  tmpProp = new ARTdataProp(C_ART_int, 0, "t");
  //	tmpParser = new ParserX(mup::pckCOMPLEX_NO_STRING);
  //	tmpContainer->SetParser(tmpParser);
  tmpProp->SetParser(parser_);
  tmpProp->SetVal(0);
  //
  //	tmpParam = new simulParameterType();
  //	tmpParam->_val = tmpContainer;
  //	tmpParam->_parser = tmpParser;
  //
  //	_simulParams["t"] = tmpParam;

  //	tmpProp = new ARTdataProp(C_ART_int, 0, "t");
  //	tmpProp->SetParser(parser_);
  //	tmpProp->SetVal(0);

  AppendDataProp(tmpProp);
}

void ARTtimeSimulator::clean()
{
  //	simulParameterMapIterator iter;

  // deallocate memory for all saved simulation parameters
  //	for (iter = _simulParams.begin(); iter != _simulParams.end(); ++iter)
  //	{
  //		delete (iter->second->_val);
  //		delete (iter->second->_parser);
  //		delete (iter->second);
  //	}
  //
  //	_simulParams.clear();
}


void ARTtimeSimulator::addParamsToModule(ARTItimeModule* timeModule)
{
  ARTproperty* iter = GetProperties(NULL);
  ARTdataProp* prop;
  while (iter != NULL)
  {
    prop = dynamic_cast<ARTdataProp*>(iter);
    if (prop)
    {
      timeModule->addGlobalParameter(prop);
    }
    iter = GetProperties(iter);
  }
}

//void ARTtimeSimulator::addParamToCurrentModules(const string& name, simulParameterType* newParam)
void ARTtimeSimulator::addParamToCurrentModules(ARTdataProp* newParam)
{
  if (userElements)
  {
    ARTItimeModule* iter = dynamic_cast<ARTItimeModule*>(userElements->GetObjects(NULL));
    while (iter != NULL)
    {
      //			std::cout << "Adding parameter '" << newParam->GetName() << "' to time module '" << iter->GetName() << "'." << std::endl;
      //			iter->addGlobalParameter(newParam->GetName(), newParam->GetParserVar());
      iter->addGlobalParameter(newParam);
      iter = dynamic_cast<ARTItimeModule*>(userElements->GetObjects(iter));
    }
  }
}
