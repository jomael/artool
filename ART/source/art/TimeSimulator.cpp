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

#include <sstream>
#include "strparsing.h"
#include "TimeSimulator.h"
#include "ListProp.h"
#include "TimeModule.h"
#include "Functionoid.h"

using namespace mup;

using std::stringstream;
using namespace ART;

//**************************************************************************************************************
// ARTtimeSimulator

TimeSimulator::TimeSimulator(const string name, const string sds,
    const string lds, const string htm) :
    Simulator(name, "TimeDomain", sds, lds, htm)
//	_simulParams()
{
  initStandardSimulParams();
}

void TimeSimulator::AddTimeModule(ITimeModule* timeModule)
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

void TimeSimulator::AddSimulationParameter(const string& name, const string& expr)
{
  //	if (_simulParams.find(name) == _simulParams.end())
  //	{
  //		simulParameterType* newParam = new simulParameterType();
  //		newParam->_parser = new ParserX(mup::pckCOMPLEX_NO_STRING);
  //		newParam->_val = new DataContainer(C_ART_cpx, 0, name);
  //		newParam->_val->SetParser(newParam->_parser);
  //		newParam->_val->SetDefinition(expr);
  //		_simulParams[name] = newParam;
  //		addParamToCurrentModules(name, newParam);
  //	}
  DataProp* prop = NULL;
  try
  {
    prop = FindDataPropInSimulator(name);
  }
  catch(ARTerror& e)
  {
//    prop = new ARTdataProp(C_ART_cpx, 0, name);
    prop = new DataProp(C_ART_dbl, 0, name);
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

void TimeSimulator::AddSimulationParameter(const string& name, const std::complex<double>& val)
{
  //	if (_simulParams.find(name) == _simulParams.end())
  //	{
  //		simulParameterType* newParam = new simulParameterType();
  //		newParam->_parser = new ParserX(mup::pckCOMPLEX_NO_STRING);
  //		newParam->_val = new DataContainer(C_ART_cpx, 0, name);
  //		newParam->_val->SetParser(newParam->_parser);
  //		newParam->_val->SetVal(val);
  //		_simulParams[name] = newParam;
  //		addParamToCurrentModules(name, newParam);
  //	}
  DataProp* prop = NULL;
  try
  {
    prop = FindDataPropInSimulator(name);
  }
  catch(ARTerror& e)
  {
    prop = new DataProp(C_ART_cpx, 0, name);
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

void TimeSimulator::AddSimulationParameter(const string& name, double val)
{
  DataProp* prop = NULL;
  try
  {
    prop = FindDataPropInSimulator(name);
  }
  catch(ARTerror& e)
  {
    prop = new DataProp(C_ART_cpx, 0, name);
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
  //		newParam->_val = new DataContainer(C_ART_cpx, 0, name);
  //		newParam->_val->SetParser(newParam->_parser);
  //		newParam->_val->SetVal(val);
  //		_simulParams[name] = newParam;
  //		addParamToCurrentModules(name, newParam);
  //	}

}

void TimeSimulator::SimulateTimeStep(int idx)
{

  // set current time index parameter if it exists
  DataProp* prop = FindDataPropInSimulator("t");
  //	if (_simulParams.find("t") != _simulParams.end())
  //	{
  //		DataContainer& tmpContainer = *(_simulParams["t"]->_val);
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
    ITimeModule* tModule;
    while (iter != NULL)
    {
      tModule = dynamic_cast<ITimeModule*>(iter);
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
      tModule = dynamic_cast<ITimeModule*>(iter);
      if (tModule)
      {
        //				std::cout << "Setting time module '" << tModule->GetName() << "' to time index " << idx << "." << std::endl;
        tModule->simulateCurrentIndex(idx);
      }
      iter = userElements->GetObjects(iter);
    }
  }
}

void TimeSimulator::SetSimulationParameter(const string& name, const string& expr)
{
  DataProp* prop = FindDataPropInSimulator(name);
  //	if (_simulParams.find(name) != _simulParams.end())
  //	{
  //		DataContainer* tmpContainer = (_simulParams[name])->_val;
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

void TimeSimulator::SetSimulationParameter(const string& name, const std::complex<double>& val)
{
  DataProp* prop = FindDataPropInSimulator(name);
  //	if (_simulParams.find(name) != _simulParams.end())
  //	{
  //		DataContainer* tmpContainer = (_simulParams[name])->_val;
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

void TimeSimulator::SetSimulationParameter(const string& name, double val)
{
  DataProp* prop = FindDataPropInSimulator(name);
  //	if (_simulParams.find(name) != _simulParams.end())
  //	{
  //		DataContainer* tmpContainer = (_simulParams[name])->_val;
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

DataProp* TimeSimulator::FindDataPropInSimulator(string exp)
{
  DataProp* prop;
  //try to find a property of the simulator with name *exp*
  prop = dynamic_cast<DataProp*>( FindProperty( strcrop( exp ) ));
  // if we have not found a property of the simulator,
  // perhaps we find a property of an internal time module
  if (!prop)
  {
    if (userElements)
    {
      vector<string> names = strsplit(exp, '.');
      ::size_t pos;
      ITimeModule* timeModule = FindTimeModuleInSimulator(strcrop(names[0]));
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
      prop = dynamic_cast<DataProp*>(timeModule->FindProperty(strcrop(names[1])));
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

ITimeModule* TimeSimulator::FindTimeModuleInSimulator(string exp)
{
  if (!userElements)
  {
    throw ARTerror("ARTtimeSimulator::FindTimeModuleInSimulator", "No memory has been allocated for userElements of time simulator '%s1'.",
        name_);
  }
  ITimeModule* tmpModule = dynamic_cast<ITimeModule*>(userElements->FindObject(exp));
  if (tmpModule == NULL)
  {
    throw ARTerror("ARTtimeSimulator::FindTimeModuleInSimulator", "No time module with name '%s1' exists in simulator '%s2'.",
        exp, name_);
  }
  return tmpModule;
}

TimeSimulator::~TimeSimulator()
{
  clean();
}

void TimeSimulator::initStandardSimulParams()
{
  //	DataContainer* tmpContainer;
  //	ParserX* tmpParser;
  //	simulParameterType* tmpParam;

  DataProp* tmpProp;

  // create new simulation parameter for sampling period
  tmpProp = new DataProp(C_ART_dbl, 0, "T");
  //	tmpParser = new ParserX(mup::pckCOMPLEX_NO_STRING);
  tmpProp->SetParser(parser_);
  tmpProp->SetVal(1.0/44100.0);

  AppendDataProp(tmpProp);

  //	tmpParam = new simulParameterType();
  //	tmpParam->_val = tmpContainer;
  //	tmpParam->_parser = tmpParser;

  //	_simulParams["T"] = tmpParam;

  // create new simulation parameter time index
  tmpProp = new DataProp(C_ART_int, 0, "t");
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

void TimeSimulator::clean()
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


void TimeSimulator::addParamsToModule(ITimeModule* timeModule)
{
  Property* iter = GetProperties(NULL);
  DataProp* prop;
  while (iter != NULL)
  {
    prop = dynamic_cast<DataProp*>(iter);
    if (prop)
    {
      timeModule->addGlobalParameter(prop);
    }
    iter = GetProperties(iter);
  }
}

//void ARTtimeSimulator::addParamToCurrentModules(const string& name, simulParameterType* newParam)
void TimeSimulator::addParamToCurrentModules(DataProp* newParam)
{
  if (userElements)
  {
    ITimeModule* iter = dynamic_cast<ITimeModule*>(userElements->GetObjects(NULL));
    while (iter != NULL)
    {
      //			std::cout << "Adding parameter '" << newParam->GetName() << "' to time module '" << iter->GetName() << "'." << std::endl;
      //			iter->addGlobalParameter(newParam->GetName(), newParam->GetParserVar());
      iter->addGlobalParameter(newParam);
      iter = dynamic_cast<ITimeModule*>(userElements->GetObjects(iter));
    }
  }
}
