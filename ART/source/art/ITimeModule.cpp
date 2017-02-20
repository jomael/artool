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


//#include "mpVariable.h"
#include "ITimeModule.h"
using namespace ART;
ITimeModule::localParameterType::localParameterType(const string& name, const string sds, const string lds, const string htm) :
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

ITimeModule::localParameterType::~localParameterType()
{
  // delete parser of local parameters
  delete parser_;
  parser_ = NULL;
}

ITimeModule::globalParameterType::globalParameterType(const string& name, const DataProp* param)
: timeProperty(C_ART_str, 0, name), param_(param)
{

}

const Variable& ITimeModule::globalParameterType::GetParserVar()
{
  return param_->GetParserVar();
}

const Variable& ITimeModule::globalParameterType::GetParserVar() const
{
  return param_->GetParserVar();
}

/*************************************************************
 * ITimeModule::OPortType
 *************************************************************/

ITimeModule::OPortType::OPortType(const T_ART_Type dtyp,
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
  //		throw ARTerror("ITimeModule::OPortType::OPortType", "No valid parser has been created. Please check source code of DataContainer!");
  //	}
}

void ITimeModule::OPortType::initPortValue(const string& expr) const
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

void ITimeModule::OPortType::initPortValue(double value, int idx) const
{
  const DataContainer* tmpContainer = dynamic_cast<const DataContainer*>(this);
  DataContainer& port = const_cast<DataContainer&>(*tmpContainer);
  port[idx] = value;
}

void ITimeModule::OPortType::initPortValue(std::complex<double>& value, int idx) const
{
  const DataContainer* tmpContainer = dynamic_cast<const DataContainer*>(this);
  DataContainer& port = const_cast<DataContainer&>(*tmpContainer);
  port[idx] = value;
}

//IValue& ITimeModule::OPortType::operator[](::size_t idx)
//{
////	cout << "ITimeModule::OPortType::operator[size_t " << idx << "]" << endl;
//	DataContainer& tmpContainer = GetArrayElement(idx);
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
//	return DataContainer::operator[](idx);
//}
//
//IValue& ITimeModule::OPortType::operator[](int idx)
//{
////	cout << "ITimeModule::OPortType::operator[int " << idx << "]" << endl;
//	DataContainer& tmpContainer = GetArrayElement(idx);
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
//	return DataContainer::operator[](idx);
//}

//IValue& ITimeModule::OPortType::GetPortValue(::size_t idx)
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
//	DataContainer& tmpContainer = GetArrayElement(idx);
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
//IValue& ITimeModule::OPortType::GetPortValue(int idx)
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
////	cout << "ITimeModule::OPortType::GetPortValue(" << idx << ")" << endl;
//	DataContainer& tmpContainer = GetArrayElement(idx);
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

ITimeModule::OPortType::~OPortType()
{
  //	delete tVar_;
}

/*************************************************************
 * ITimeModule::FPortType
 *************************************************************/

ITimeModule::FPortType::FPortType(const int dlen,
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

IValue& ITimeModule::FPortType::operator[](std::size_t idx)
{
  array_type* tmpArray = (array_type *) (val->na);
  DataContainer* tmp;
  tmp = dynamic_cast<DataContainer*>((*tmpArray)[idx]);
  return *tmp;
}

IValue& ITimeModule::FPortType::operator[](int idx)
{
  array_type* tmpArray = (array_type *) (val->na);
  DataContainer* tmp;
  tmp = dynamic_cast<DataContainer*>((*tmpArray)[idx]);
  return *tmp;
}

//void ITimeModule::FPortType::initPortValue(const string& expr) const
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
//void ITimeModule::FPortType::initPortValue(double value, int idx) const
//{
//	array_type* tmpArray = (array_type*) (val->na);
//	DataContainer* tmp = dynamic_cast<DataContainer*>((*tmpArray)[idx]);
//	tmp->SetVal(value);
//}
//
//void ITimeModule::FPortType::initPortValue(std::complex<double>& value, int idx) const
//{
//	array_type* tmpArray = (array_type*) (val->na);
//	DataContainer* tmp = dynamic_cast<DataContainer*>((*tmpArray)[idx]);
//	tmp->SetVal(value);
//}

/*************************************************************
 * ITimeModule::IPortType
 *************************************************************/

ITimeModule::IPortType::IPortType(const string& name, const OPortType* param)
: PortType(C_ART_str, 0, name), refPort_(param)
{

}

//ITimeModule::IPortType::IPortType(const string& name, const FPortType* param)
//	: PortType(C_ART_str, 0, name), refPort_(param)
//{
//
//}

const Variable& ITimeModule::IPortType::GetParserVar()
{
  return refPort_->GetParserVar();
}

const Variable& ITimeModule::IPortType::GetParserVar() const
{
  return refPort_->GetParserVar();
}

/*************************************************************
 * ITimeModule
 *************************************************************/

DataProp* ITimeModule::getPort(const string& name)
{
  PortType* port = dynamic_cast<PortType*>(FindProperty(name));
  if (!port)
  {
    throw ARTerror("ITimeModule::getPort", "No port with name '%s1' has been found in current time module '%s2'.",
        name, name_);
  }
  else
  {
    return port;
  }

}

void ITimeModule::setLocalParameter(const string& name, const string& expr)
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
    throw ARTerror("ITimeModule::setLocalParameter", "Local parameter '%s1' could not be found in current time module '%s2'.",
        name, name_);
  }


}

void ITimeModule::setLocalParameter(const string& name, const double val)
{
  localParameterType* lParam = dynamic_cast<localParameterType*>(FindProperty(name));
  if (lParam)
  {
    lParam->SetVal(val);
  }
  else
  {
    throw ARTerror("ITimeModule::setLocalParameter", "Local parameter '%s1' could not be found in current time module '%s2'.",
        name, name_);
  }
}

void ITimeModule::setLocalParameter(const string& name, const std::complex<double>& val)
{
  localParameterType* lParam = dynamic_cast<localParameterType*>(FindProperty(name));
  if (lParam)
  {
    lParam->SetVal(val);
  }
  else
  {
    throw ARTerror("ITimeModule::setLocalParameter", "Local parameter '%s1' could not be found in current time module '%s2'.",
        name, name_);
  }
}

void ITimeModule::addGlobalParameter(const DataProp* parameter)
{
  Property* iter;
  OPortType* oPort;
  localParameterType* lParam;
  if (!parameter)
  {
    throw ARTerror("ITimeModule::addGlobalParameter", "Could not add global parameter to current time module '%s1': No valid parameter.",
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
    throw ARTerror("ITimeModule::addGlobalParameter", "Name '%s1' of global parameter is already in use in current time module '%s2'.",
        parameter->GetName(), name_);
  }
}

void ITimeModule::removeGlobalParameter(const string& name)
{

  Property* iter = GetProperties(NULL);
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


void ITimeModule::setSimulator(Simulator* sim)
{
  _simulator = dynamic_cast<TimeSimulator*>(sim);
  Property* iter = GetProperties(NULL);
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
