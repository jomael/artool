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
 * ARTtimeModule.cpp
 *
 *  Created on: Jul 5, 2012
 *      Author: cbg
 */


//#include "mpVariable.h"
#include "ARTtimeModule.h"

using namespace mup;

/* create a new implementation for convolution function which will be registered
 * to all parsers of each ARTtimeModule */
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

ARTItimeModule::localParameterType::localParameterType(const string& name, const string sds, const string lds, const string htm) :
    //    CBG_DBL
    //		timeProperty(C_ART_cpx, 0, name, sds, lds, htm)
          timeProperty(C_ART_dbl, 0, name, sds, lds, htm)
{
  // create new parser for local parameters
  //  CBG_DBL
  //	parser_ = new ParserX(mup::pckCOMPLEX_NO_STRING);
  parser_ = new ParserX(mup::pckCOMMON_NO_STR);
  SetParserVar(name);
}

ARTItimeModule::localParameterType::~localParameterType()
{
  // delete parser of local parameters
  delete parser_;
  parser_ = NULL;
}

ARTItimeModule::globalParameterType::globalParameterType(const string& name, const ARTdataProp* param)
: timeProperty(C_ART_str, 0, name), param_(param)
{

}

const Variable& ARTItimeModule::globalParameterType::GetParserVar()
{
  return param_->GetParserVar();
}

const Variable& ARTItimeModule::globalParameterType::GetParserVar() const
{
  return param_->GetParserVar();
}

/*************************************************************
 * ARTItimeModule::OPortType
 *************************************************************/

ARTItimeModule::OPortType::OPortType(const T_ART_Type dtyp,
    const int dlen,
    const string& name,
    const string sds,
    const string lds,
    const string htm)
: PortType(dtyp, dlen, name, sds, lds, htm)
//  tVal_(0),
//  tVar_(new Variable(&tVal_))
{
  //	if (parser_)
  //	{
  //		parser_->DefineVar("t", *tVar_);
  //	}
  //	else
  //	{
  //		throw ARTerror("ARTItimeModule::OPortType::OPortType", "No valid parser has been created. Please check source code of ARTdataContainer!");
  //	}
}

void ARTItimeModule::OPortType::initPortValue(const string& expr) const
{
  //  CBG_DBL
  //	ParserX* tmpParser = new ParserX(mup::pckCOMPLEX_NO_STRING);
  ParserX* tmpParser = new ParserX(mup::pckCOMMON_NO_STR);
  try
  {
    tmpParser->DefineVar(GetVarName(), GetParserVar());
    tmpParser->SetExpr(expr);
    tmpParser->Eval();
    delete tmpParser;
  }
  catch (ParserError& error)
  {
    delete tmpParser;
    throw ARTerror("ARTPortType::initPortValue", "Error in evaluation of parser expression: %s1",
        error.GetMsg());
  }
}

void ARTItimeModule::OPortType::initPortValue(double value, int idx) const
{
  const ARTdataContainer* tmpContainer = dynamic_cast<const ARTdataContainer*>(this);
  ARTdataContainer& port = const_cast<ARTdataContainer&>(*tmpContainer);
  port[idx] = value;
}

void ARTItimeModule::OPortType::initPortValue(std::complex<double>& value, int idx) const
{
  const ARTdataContainer* tmpContainer = dynamic_cast<const ARTdataContainer*>(this);
  ARTdataContainer& port = const_cast<ARTdataContainer&>(*tmpContainer);
  port[idx] = value;
}

//IValue& ARTItimeModule::OPortType::operator[](::size_t idx)
//{
////	cout << "ARTItimeModule::OPortType::operator[size_t " << idx << "]" << endl;
//	ARTdataContainer& tmpContainer = GetArrayElement(idx);
//	if (!tmpContainer.IsValid())
//	{
////		cout << idx << " INVALID!" << endl;
//		tVal_ = (int) idx;
//	}
//	else
//	{
////		cout << idx << " VALID!" << endl;
//	}
//
//	return ARTdataContainer::operator[](idx);
//}
//
//IValue& ARTItimeModule::OPortType::operator[](int idx)
//{
////	cout << "ARTItimeModule::OPortType::operator[int " << idx << "]" << endl;
//	ARTdataContainer& tmpContainer = GetArrayElement(idx);
//	if (!tmpContainer.IsValid())
//	{
////		cout << idx << " INVALID!" << endl;
//		tVal_ = idx;
//	}
//	else
//	{
////		cout << idx << " VALID!" << endl;
//	}
//
//	return ARTdataContainer::operator[](idx);
//}

//IValue& ARTItimeModule::OPortType::GetPortValue(::size_t idx)
//{
//	ARTtimeSimulator* simulator = dynamic_cast<ARTtimeSimulator*>(scope_);
//	if (simulator)
//	{
//		simulator->SimulateTimeStep(idx);
//	}
//	else
//	{
//		throw ARTerror("ARTPortType::initPortValue", "No valid time simulator set for current port '%s1'.", name_);
//	}
//
//	ARTdataContainer& tmpContainer = GetArrayElement(idx);
//
//	if (tVal_.GetFloat() != (float_type) idx)
//	{
//		tVal_ = (int) idx;
//		tmpContainer.Invalidate();
//		SetCurrentIndex(idx);
//	}
//
//	return tmpContainer;
//}
//
//IValue& ARTItimeModule::OPortType::GetPortValue(int idx)
//{
//	ARTtimeSimulator* simulator = dynamic_cast<ARTtimeSimulator*>(scope_);
//	if (simulator)
//	{
//		simulator->SimulateTimeStep(idx);
//	}
//	else
//	{
//		throw ARTerror("ARTPortType::initPortValue", "No valid time simulator set for current port '%s1'.", name_);
//	}
//
////	cout << "ARTItimeModule::OPortType::GetPortValue(" << idx << ")" << endl;
//	ARTdataContainer& tmpContainer = GetArrayElement(idx);
//
//	if (tVal_.GetFloat() != (float_type) idx)
//	{
//		tVal_ = (int) idx;
//		tmpContainer.Invalidate();
//		SetCurrentIndex(idx);
//	}
//
//	return tmpContainer;
//}

ARTItimeModule::OPortType::~OPortType()
{
  //	delete tVar_;
}

/*************************************************************
 * ARTItimeModule::FPortType
 *************************************************************/

ARTItimeModule::FPortType::FPortType(const int dlen,
    const string& name,
    const string sds,
    const string lds,
    const string htm)
: OPortType(C_ART_na, dlen, name, sds, lds, htm)
{
  // change the ring buffer to be a vector
  array_type* tmpArray = (array_type *) (val->na);
  tmpArray->setVector();

  avar_ = new Variable(this);
  parserVarDefined_ = true;
}

IValue& ARTItimeModule::FPortType::operator[](std::size_t idx)
{
  array_type* tmpArray = (array_type *) (val->na);
  ARTdataContainer* tmp;
  tmp = dynamic_cast<ARTdataContainer*>((*tmpArray)[idx]);
  return *tmp;
}

IValue& ARTItimeModule::FPortType::operator[](int idx)
{
  array_type* tmpArray = (array_type *) (val->na);
  ARTdataContainer* tmp;
  tmp = dynamic_cast<ARTdataContainer*>((*tmpArray)[idx]);
  return *tmp;
}

//void ARTItimeModule::FPortType::initPortValue(const string& expr) const
//{
//	ParserX* tmpParser = new ParserX(mup::pckCOMPLEX_NO_STRING);
//	try
//	{
//		tmpParser->DefineVar(GetVarName(), GetParserVar());
//		tmpParser->SetExpr(expr);
//		tmpParser->Eval();
//		delete tmpParser;
//	}
//	catch (ParserError& error)
//	{
//		delete tmpParser;
//		throw ARTerror("ARTPortType::initPortValue", "Error in evaluation of parser expression: %s1",
//				error.GetMsg());
//	}
//}
//
//void ARTItimeModule::FPortType::initPortValue(double value, int idx) const
//{
//	array_type* tmpArray = (array_type*) (val->na);
//	ARTdataContainer* tmp = dynamic_cast<ARTdataContainer*>((*tmpArray)[idx]);
//	tmp->SetVal(value);
//}
//
//void ARTItimeModule::FPortType::initPortValue(std::complex<double>& value, int idx) const
//{
//	array_type* tmpArray = (array_type*) (val->na);
//	ARTdataContainer* tmp = dynamic_cast<ARTdataContainer*>((*tmpArray)[idx]);
//	tmp->SetVal(value);
//}

/*************************************************************
 * ARTItimeModule::IPortType
 *************************************************************/

ARTItimeModule::IPortType::IPortType(const string& name, const OPortType* param)
: PortType(C_ART_str, 0, name), refPort_(param)
{

}

//ARTItimeModule::IPortType::IPortType(const string& name, const FPortType* param)
//	: PortType(C_ART_str, 0, name), refPort_(param)
//{
//
//}

const Variable& ARTItimeModule::IPortType::GetParserVar()
{
  return refPort_->GetParserVar();
}

const Variable& ARTItimeModule::IPortType::GetParserVar() const
{
  return refPort_->GetParserVar();
}

/*************************************************************
 * ARTItimeModule
 *************************************************************/

ARTdataProp* ARTItimeModule::getPort(const string& name)
{
  PortType* port = dynamic_cast<PortType*>(FindProperty(name));
  if (!port)
  {
    throw ARTerror("ARTItimeModule::getPort", "No port with name '%s1' has been found in current time module '%s2'.",
        name, name_);
  }
  else
  {
    return port;
  }

}

void ARTItimeModule::setLocalParameter(const string& name, const string& expr)
{
  localParameterType* lParam = dynamic_cast<localParameterType*>(FindProperty(name));
  if (lParam)
  {
    if (lParam->GetType() == 's')
    {
      //			cout << "setting string of local parameter '" << name << "' to '" << expr << "'" << endl;
      lParam->SetVal(expr.c_str());
    }
    else
    {
      // set definition and invalidate current parameter to force re-evaluation
      lParam->SetDefinition(expr);
      //			lParam->Invalidate();
    }
  }
  else
  {
    throw ARTerror("ARTItimeModule::setLocalParameter", "Local parameter '%s1' could not be found in current time module '%s2'.",
        name, name_);
  }


}

void ARTItimeModule::setLocalParameter(const string& name, const double val)
{
  localParameterType* lParam = dynamic_cast<localParameterType*>(FindProperty(name));
  if (lParam)
  {
    lParam->SetVal(val);
  }
  else
  {
    throw ARTerror("ARTItimeModule::setLocalParameter", "Local parameter '%s1' could not be found in current time module '%s2'.",
        name, name_);
  }
}

void ARTItimeModule::setLocalParameter(const string& name, const std::complex<double>& val)
{
  localParameterType* lParam = dynamic_cast<localParameterType*>(FindProperty(name));
  if (lParam)
  {
    lParam->SetVal(val);
  }
  else
  {
    throw ARTerror("ARTItimeModule::setLocalParameter", "Local parameter '%s1' could not be found in current time module '%s2'.",
        name, name_);
  }
}

void ARTItimeModule::addGlobalParameter(const ARTdataProp* parameter)
{
  ARTproperty* iter;
  OPortType* oPort;
  localParameterType* lParam;
  if (!parameter)
  {
    throw ARTerror("ARTItimeModule::addGlobalParameter", "Could not add global parameter to current time module '%s1': No valid parameter.",
        name_);
  }
  if (!FindProperty(parameter->GetName()))
  {
    globalParameterType* newParam =	new globalParameterType(parameter->GetName(), parameter);
    iter = GetProperties(NULL);
    while (iter)
    {
      oPort = dynamic_cast<OPortType*>(iter);
      lParam = dynamic_cast<localParameterType*>(iter);
      // register global variable to output port
      if (oPort)
      {
        oPort->GetParser()->DefineVar(parameter->GetName(), parameter->GetParserVar());
      }
      // register global variable to local parameter
      else if (lParam)
      {
        lParam->GetParser()->DefineVar(parameter->GetName(), parameter->GetParserVar());
      }
      iter = GetProperties(iter);
    }
    AppendDataProp(newParam);
  }
  else
  {
    throw ARTerror("ARTItimeModule::addGlobalParameter", "Name '%s1' of global parameter is already in use in current time module '%s2'.",
        parameter->GetName(), name_);
  }
}

void ARTItimeModule::removeGlobalParameter(const string& name)
{

  ARTproperty* iter = GetProperties(NULL);
  localParameterType* lParam;
  OPortType* oPort;

  globalParameterType* gParam = dynamic_cast<globalParameterType*>(FindProperty(name));
  if (gParam)
  {
    DeleteProperty(gParam);
    delete gParam;
  }
  else
  {
    throw ARTerror("fractionalDelayModule::addGlobalParameter", "No global parameter with name '%s1' found in current time module '%s2'.",
        name, name_);
  }

  while (iter)
  {
    oPort = dynamic_cast<OPortType*>(iter);
    lParam = dynamic_cast<localParameterType*>(iter);
    // remove global parameter from output port
    if (oPort)
    {
      oPort->GetParser()->RemoveVar(name);
    }
    // remove global parameter from local parameters
    else if (lParam)
    {
      lParam->GetParser()->RemoveVar(name);
    }
    iter = GetProperties(iter);
  }
}


void ARTItimeModule::setSimulator(ARTsimulator* sim)
{
  _simulator = dynamic_cast<ARTTimeSimulator*>(sim);
  ARTproperty* iter = GetProperties(NULL);
  OPortType* oPort;
  while (iter)
  {
    // set simulator for all registered output ports
    oPort = dynamic_cast<OPortType*>(iter);
    if (oPort)
    {
      oPort->SetScope(_simulator);
    }
    iter = GetProperties(iter);
  }
}

/*************************************************************
 * ARTtimeModule
 *************************************************************/

ARTtimeModule::ARTtimeModule(const string& name, const string& sds, const string& lds, const string& htm) :
	    ARTItimeModule(name, sds, lds, htm)
{

}

ARTItimeModule* ARTtimeModule::Create(const string& name, const string& sds, const string& lds, const string& htm)
{
  return new ARTtimeModule(name, sds, lds, htm);
}

void ARTtimeModule::addIPort(const string& name, const ARTdataProp* refPort)
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
      throw ARTerror("ARTtimeModule::addIPort", "Port '%s1' is no valid output port.",
          refPort->GetName());
    }

  }
  else
  {
    throw ARTerror("ARTtimeModule::addIPort", "Port name '%s1' is already in use in current time module '%s2'.",
        name, name_);
  }
}

void ARTtimeModule::addOPort(const string& name, const string& expr, unsigned int size)
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
    throw ARTerror("ARTtimeModule::addOPort", "Port name '%s1' is already in use in current time module '%s2'.",
        name, name_);
  }
}



void ARTtimeModule::addLocalParameter(const string& name, const string& expr)
{
  if (!FindProperty(name))
  {
    if (!_simulator)
    {
      throw ARTerror("ARTtimeModule::setLocalParameter", "Could not create local parameter '%s1' for current time module '%s2' because no simulator is set!",
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
    throw ARTerror("ARTtimeModule::addLocalParameter", "Name '%s1' of local parameter is already in use in current time module '%s2'.",
        name, name_);
  }

}

void ARTtimeModule::addLocalParameter(const string& name, const double val)
{
  if (!FindProperty(name))
  {
    if (!_simulator)
    {
      throw ARTerror("ARTtimeModule::setLocalParameter", "Could not create local parameter '%s1' for current time module '%s2' because no simulator is set!",
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
    throw ARTerror("ARTtimeModule::setLocalParameter", "Name '%s1' of local parameter is already in use in current time module '%s2'.",
        name, name_);
  }
}

void ARTtimeModule::addLocalParameter(const string& name, const std::complex<double>& val)
{
  if (!FindProperty(name))
  {
    if (!_simulator)
    {
      throw ARTerror("ARTtimeModule::setLocalParameter", "Could not create local parameter '%s1' for current time module '%s2' because no simulator is set!",
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
    throw ARTerror("ARTtimeModule::setLocalParameter", "Name '%s1' of local parameter is already in use in current time module '%s2'.",
        name, name_);
  }
}

void ARTtimeModule::addGlobalParameter(const ARTdataProp* parameter)
{
  if (!parameter)
  {
    throw ARTerror("ARTtimeModule::addGlobalParameter", "Could not add global parameter to current time module '%s1': No valid parameter.",
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
    throw ARTerror("ARTtimeModule::addGlobalParameter", "Name '%s1' of global parameter is already in use in current time module '%s2'.",
        parameter->GetName(), name_);
  }
}

void ARTtimeModule::removeGlobalParameter(const string& name)
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
    throw ARTerror("ARTtimeModule::removeGlobalParameter", "Name '%s1' of global parameter could not be found in current time module '%s2'.",
        name, name_);
  }
}

void ARTtimeModule::setCurrentIndex(int idx)
{
  ARTproperty* propIter = GetProperties(NULL);
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

void ARTtimeModule::simulateCurrentIndex(int idx)
{
  ARTproperty* propIter = GetProperties(NULL);
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

ARTtimeModule::~ARTtimeModule()
{
  clean();
}

void ARTtimeModule::addVariableToParsers(const string& name, const Variable& var)
{

  // add the given variable to all currently registered parsers of output ports
  ARTproperty* propIter = GetProperties(NULL);
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

void ARTtimeModule::removeVariableFromParsers(const string& name)
{
  // go through all parsers of output ports and remove name declaration
  ARTproperty* propIter = GetProperties(NULL);
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

void ARTtimeModule::registerAllVariablesToParser(ParserX* parser)
{
  ARTproperty* propIter;
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

void ARTtimeModule::clean()
{
  // currently nothing to do...
}

//void ARTtimeModule::copy(const ARTtimeModule& orig)
//{
//	/* TODO: implement copy function */
//}




