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
 * TimeModule.cpp
 *
 *  Created on: Jul 5, 2012
 *      Author: cbg
 */


//#include "mpVariable.h"
#include "TimeModule.h"

using namespace mup;
using namespace ART;
/* create a new implementation for convolution function which will be registered
 * to all parsers of each TimeModule */
class convCallback : public ICallback {
public:
  convCallback() : ICallback(cmFUNC, "conv", "c:aac") {}

  virtual void Eval(ptr_val_type& ret, const ptr_val_type* inArg, int argc) {

    assert(argc==3);

    const array_type& a = inArg[0]->GetArray();
    const array_type& b = inArg[1]->GetArray();
    int_type t = (int_type) (inArg[2]->GetFloat());

    int lowerBound = 0;
    int upperBound = (int) t;

    cmplx_type result = 0;

    if ((t - (int) b.size()) >= 0)
    {
      lowerBound = (t - (int) b.size()) + 1;
    }
    else
    {
      lowerBound = 0;
    }


    //		cout << "Expression of a: " << tmpContainer << endl;

    // (a + bi) * (c + di) = ac + adi + bci - bd = (ac - bd) + (ad + bc)i
    //		cerr << "conv: t = " << t << ", lowerBound = " << lowerBound << ", upperBound = " << upperBound << endl;
    for (int k = lowerBound; k <= upperBound; ++k)
    {
      //			cerr << "index = " << index << endl;
      //			std::cerr << "a[" << k << "] = " << (*portA)[k].GetFloat() << std::endl;
      //			std::cerr << "b[" << t << "-" << k << "] = " << (*portB)[t-k].GetFloat() << std::endl;
      //			std::cerr << "b[" << t << "-" << k << "] = " << b[t-k]->GetFloat() << std::endl;

      float_type z1_real = a[k]->GetFloat();
      float_type z1_imag = a[k]->GetImag();
      float_type z2_real = b[t-k]->GetFloat();
      float_type z2_imag = b[t-k]->GetImag();

      result += cmplx_type((z1_real*z2_real - z1_imag*z2_imag), (z1_real*z2_imag + z1_imag*z2_real));

      /*result.real() += (z1_real*z2_real - z1_imag*z2_imag);
			result.imag() += (z1_real*z2_imag + z1_imag*z2_real);*/

    }


    *ret = result;

    //cerr << endl << endl;

    //cerr << "Conv: lowerBound = " << lowerBound << " upperBound = " << upperBound << endl;

  }

  const char_type* GetDesc() const {
    return "conv(a, b, t) - Returns the convolution of input values a and b at time t.";
  }

  IToken* Clone() const {
    return new convCallback(*this);
  }

};

/*************************************************************
 * TimeModule
 *************************************************************/

TimeModule::TimeModule(const string& name, const string& sds, const string& lds, const string& htm) :
	    ITimeModule(name, sds, lds, htm)
{

}

ITimeModule* TimeModule::Create(const string& name, const string& sds, const string& lds, const string& htm)
{
  return new TimeModule(name, sds, lds, htm);
}

void TimeModule::addIPort(const string& name, const DataProp* refPort)
{
  // the given name of the input port has to be unique
  if (!FindProperty(name))
  {
    const OPortType* oPort = dynamic_cast<const OPortType*>(refPort);
    const FPortType* fPort = dynamic_cast<const FPortType*>(refPort);
    IPortType* newIPort;
    if (oPort)
    {
      newIPort = new IPortType(name, oPort);
      addVariableToParsers(name, newIPort->GetParserVar());
      AppendDataProp(newIPort);
    }
    else if (fPort)
    {
      newIPort = new IPortType(name, fPort);
      addVariableToParsers(name, newIPort->GetParserVar());
      AppendDataProp(newIPort);
    }
    else
    {
      throw ARTerror("TimeModule::addIPort", "Port '%s1' is no valid output port.",
          refPort->GetName());
    }

  }
  else
  {
    throw ARTerror("TimeModule::addIPort", "Port name '%s1' is already in use in current time module '%s2'.",
        name, name_);
  }
}

void TimeModule::addOPort(const string& name, const string& expr, unsigned int size)
{
  // the name of the given output port has to be unique
  if (!FindProperty(name))
  {
    OPortType* newOPort = new OPortType(C_ART_na, size, name);

    // register convolution function to new parser
    newOPort->GetParser()->DefineFun(new convCallback());

    // set definition of new output port
    newOPort->SetDefinition(expr);
    // register all variables of module to parser
    registerAllVariablesToParser(newOPort->GetParser());
    // set current simulator to output port
    newOPort->SetScope(_simulator);
    // set parent name of output port
    newOPort->SetParentModuleName(name_);

    // add variable to all available parsers
    addVariableToParsers(name, newOPort->GetParserVar());
    AppendDataProp(newOPort);

  }
  else
  {
    throw ARTerror("TimeModule::addOPort", "Port name '%s1' is already in use in current time module '%s2'.",
        name, name_);
  }
}



void TimeModule::addLocalParameter(const string& name, const string& expr)
{
  if (!FindProperty(name))
  {
    if (!_simulator)
    {
      throw ARTerror("TimeModule::setLocalParameter", "Could not create local parameter '%s1' for current time module '%s2' because no simulator is set!",
          name, name_);
    }
    localParameterType* newParam = new localParameterType(name);
    try
    {
      //			newParam->SetParser(_simulator->GetParser());
      newParam->SetDefinition(expr);


    }
    catch (ARTerror& error)
    {
      delete newParam;
      throw error;
    }
    registerAllVariablesToParser(newParam->GetParser());
    addVariableToParsers(name, newParam->GetParserVar());
    AppendDataProp(newParam);
  }
  else
  {
    throw ARTerror("TimeModule::addLocalParameter", "Name '%s1' of local parameter is already in use in current time module '%s2'.",
        name, name_);
  }

}

void TimeModule::addLocalParameter(const string& name, const double val)
{
  if (!FindProperty(name))
  {
    if (!_simulator)
    {
      throw ARTerror("TimeModule::setLocalParameter", "Could not create local parameter '%s1' for current time module '%s2' because no simulator is set!",
          name, name_);
    }
    localParameterType* newParam = new localParameterType(name);

    //		newParam->SetParser(_simulator->GetParser());
    newParam->SetVal(val);
    //		AppendDataProp(newParam);
    //		addVariableToParsers(name, newParam->GetParserVar());
    registerAllVariablesToParser(newParam->GetParser());
    addVariableToParsers(name, newParam->GetParserVar());
    AppendDataProp(newParam);
  }
  else
  {
    throw ARTerror("TimeModule::setLocalParameter", "Name '%s1' of local parameter is already in use in current time module '%s2'.",
        name, name_);
  }
}

void TimeModule::addLocalParameter(const string& name, const std::complex<double>& val)
{
  if (!FindProperty(name))
  {
    if (!_simulator)
    {
      throw ARTerror("TimeModule::setLocalParameter", "Could not create local parameter '%s1' for current time module '%s2' because no simulator is set!",
          name, name_);
    }
    localParameterType* newParam = new localParameterType(name);
    //		newParam->SetParser(_simulator->GetParser());
    newParam->SetVal(val);
    //		AppendDataProp(newParam);
    //		addVariableToParsers(name, newParam->GetParserVar());
    registerAllVariablesToParser(newParam->GetParser());
    addVariableToParsers(name, newParam->GetParserVar());
    AppendDataProp(newParam);
  }
  else
  {
    throw ARTerror("TimeModule::setLocalParameter", "Name '%s1' of local parameter is already in use in current time module '%s2'.",
        name, name_);
  }
}

void TimeModule::addGlobalParameter(const DataProp* parameter)
{
  if (!parameter)
  {
    throw ARTerror("TimeModule::addGlobalParameter", "Could not add global parameter to current time module '%s1': No valid parameter.",
        name_);
  }
  if (!FindProperty(parameter->GetName()))
  {
    globalParameterType* newParam =	new globalParameterType(parameter->GetName(), parameter);
    addVariableToParsers(parameter->GetName(), parameter->GetParserVar());
    AppendDataProp(newParam);
  }
  else
  {
    throw ARTerror("TimeModule::addGlobalParameter", "Name '%s1' of global parameter is already in use in current time module '%s2'.",
        parameter->GetName(), name_);
  }
}

void TimeModule::removeGlobalParameter(const string& name)
{
  globalParameterType* parameter = dynamic_cast<globalParameterType*>(FindProperty(name));
  if (parameter)
  {
    removeVariableFromParsers(name);
    DeleteProperty(parameter);
    delete parameter;
  }
  else
  {
    throw ARTerror("TimeModule::removeGlobalParameter", "Name '%s1' of global parameter could not be found in current time module '%s2'.",
        name, name_);
  }
}

void TimeModule::setCurrentIndex(int idx)
{
  Property* propIter = GetProperties(NULL);
  OPortType* oPort;
  while (propIter)
  {
    oPort = dynamic_cast<OPortType*>(propIter);
    // only set current index of output ports of current time module
    if (oPort)
    {
      oPort->SetCurrentIndex(idx);
      //			oPort->GetArrayElement(idx).Invalidate();
    }
    propIter = GetProperties(propIter);
  }
}

void TimeModule::simulateCurrentIndex(int idx)
{
  Property* propIter = GetProperties(NULL);
  OPortType* oPort;
  while (propIter)
  {
    oPort = dynamic_cast<OPortType*>(propIter);
    // only evaluate current output port if needed
    if (oPort)
    {
      oPort->GetArrayElement(idx).EvaluateIfInvalid();
    }
    propIter = GetProperties(propIter);
  }
}

TimeModule::~TimeModule()
{
  clean();
}

void TimeModule::addVariableToParsers(const string& name, const Variable& var)
{

  // add the given variable to all currently registered parsers of output ports
  Property* propIter = GetProperties(NULL);
  OPortType* oPort;
  localParameterType* lParam;
  while (propIter)
  {
    oPort = dynamic_cast<OPortType*>(propIter);
    lParam = dynamic_cast<localParameterType*>(propIter);
    // only add variables to parsers of output ports
    // and local parameters
    if (oPort)
    {
      oPort->GetParser()->DefineVar(name, var);
    }
    else if (lParam)
    {
      lParam->GetParser()->DefineVar(name, var);
    }
    propIter = GetProperties(propIter);
  }

}

void TimeModule::removeVariableFromParsers(const string& name)
{
  // go through all parsers of output ports and remove name declaration
  Property* propIter = GetProperties(NULL);
  OPortType* oPort;
  localParameterType* lParam;
  while (propIter)
  {
    oPort = dynamic_cast<OPortType*>(propIter);
    lParam = dynamic_cast<localParameterType*>(propIter);
    // only remove variables from parsers of output ports and
    // local parameters
    if (oPort)
    {
      oPort->GetParser()->RemoveVar(name);
    }
    else if (lParam)
    {
      lParam->GetParser()->RemoveVar(name);
    }
    propIter = GetProperties(propIter);
  }
}

void TimeModule::registerAllVariablesToParser(ParserX* parser)
{
  Property* propIter;
  timeProperty* paramIter;

  propIter = GetProperties(NULL);
  // go through all properties of time module
  while(propIter)
  {
    paramIter = dynamic_cast<timeProperty*>(propIter);
    // only register variables of time properties to given parser
    if (paramIter)
    {
      parser->DefineVar(paramIter->GetName(), paramIter->GetParserVar());
    }
    propIter = GetProperties(propIter);
  }


}

void TimeModule::clean()
{
  // currently nothing to do...
}

//void TimeModule::copy(const TimeModule& orig)
//{
//	/* TODO: implement copy function */
//}




