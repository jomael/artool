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
 * timePrototypes.cpp
 *
 *  Created on: Aug 12, 2012
 *      Author: cbg
 */

#include <sstream>
#include <cmath>
#include "timePrototypes.h"
#include "constants.hpp"
using namespace ART;
/*******************************************************************************************
 * inputFunctionModule
 *******************************************************************************************/

inputFunctionModule::inputFunctionModule(const string& name, const string& sds, const string& lds, const string& htm) :
ITimeModule(name, sds, lds, htm),
out_(NULL)
{
}

ITimeModule* inputFunctionModule::Create(const string& name, const string& sds, const string& lds, const string& htm)
{
  return new inputFunctionModule(name, sds, lds, htm);
}


void inputFunctionModule::addIPort(const string& name, const DataProp* refPort)
{
  throw ARTerror("InputFunctionModule::addIPort", "Operation not permitted for time module '%s1'.",
      name_);
}

void inputFunctionModule::defineOPort(int len, const string& expr)
{
  Property* iter;
  globalParameterType* gParam;
  // TODO check whether out_ already references an object and removes it if that
  // is the case
  if (len > 0)
  {
    // create new function port of specified length
    out_ = new FPortType(len, "out", "Output port of InputFunctionModule");
    out_->SetParentModuleName(this->name_);
    out_->SetDefinition(expr);
    out_->SetScope(_simulator);

    // add all existing global parameters
    iter = GetProperties(NULL);
    while (iter)
    {
      gParam = dynamic_cast<globalParameterType*>(iter);
      if (gParam)
      {
        out_->GetParser()->DefineVar(gParam->GetName(), gParam->GetParserVar());
      }
      iter = GetProperties(iter);
    }

    AppendDataProp(out_);

  }
  else
  {
    throw ARTerror("inputFunctionModule::defineOPort", "Error when creating new output port for module '%s1': invalid specified length.",
        name_);
  }
}

DataProp* inputFunctionModule::getPort(const string& name)
{
  if (name != "out" || !out_)
  {
    throw ARTerror("InputFunctionModule::getPort", "Time module '%s1' has no port '%s2'.",
        name_, name);
  }
  return out_;
}

inputFunctionModule::~inputFunctionModule()
{
  //	delete out_;
}

/*******************************************************************************************
 * impulseModule
 *******************************************************************************************/

impulseModule::impulseModule(const string& name, const string& sds, const string& lds, const string& htm) :
		    ITimeModule(name, sds, lds, htm),
		    out_(NULL)
{
  out_ = new OPortType(C_ART_na, 5, "out");
  out_->SetDefinition("out[t] = (t == 0) ? A : 0");
  out_->SetParentModuleName(name_);
  AppendDataProp(out_);
  initLocalParams();
}

ITimeModule* impulseModule::Create(const string& name, const string& sds, const string& lds, const string& htm)
{
  return new impulseModule(name, sds, lds, htm);
}

void impulseModule::addIPort(const string& name, const DataProp* refPort)
{
  throw ARTerror("impulseModule::addIPort", "Operation not permitted for time module '%s1'.",
      name_);
}

DataProp* impulseModule::getPort(const string& name)
{
  if (name != "out")
  {
    throw ARTerror("impulseModule::getPort", "Time module '%s1' has no port '%s2'.",
        name_, name);
  }
  return out_;
}

void impulseModule::setCurrentIndex(int idx)
{
  out_->SetCurrentIndex(idx);
}

void impulseModule::simulateCurrentIndex(int idx)
{
  //	cout << "evaluating module " << name_ << endl;
  out_->GetArrayElement(idx).EvaluateIfInvalid();
}

void impulseModule::initLocalParams()
{
  localParameterType* tmpParam;

  // save standard value for output amplitude
  tmpParam = new localParameterType("A");
  tmpParam->SetVal(1);
  AppendDataProp(tmpParam);

  out_->GetParser()->DefineVar(tmpParam->GetName(), tmpParam->GetParserVar());
}

/*******************************************************************************************
 * heavisideModule
 *******************************************************************************************/

heavisideModule::heavisideModule(const string& name, const string& sds, const string& lds, const string& htm) :
		    ITimeModule(name, sds, lds, htm),
		    out_(NULL)
{
  out_ = new OPortType(C_ART_na, 5, "out");
  out_->SetDefinition("out[t] = (t >= 0) ? A : 0");
  out_->SetParentModuleName(name_);
  AppendDataProp(out_);
  initLocalParams();
}

ITimeModule* heavisideModule::Create(const string& name, const string& sds, const string& lds, const string& htm)
{
  return new heavisideModule(name, sds, lds, htm);
}

void heavisideModule::addIPort(const string& name, const DataProp* refPort)
{
  throw ARTerror("heavisideModule::addIPort", "Operation not permitted for time module '%s1'.",
      name_);
}

DataProp* heavisideModule::getPort(const string& name)
{
  if (name != "out")
  {
    throw ARTerror("heavisideModule::getPort", "Time module '%s1' has no port '%s2'.",
        name_, name);
  }
  return out_;
}

void heavisideModule::setCurrentIndex(int idx)
{
  out_->SetCurrentIndex(idx);
}

void heavisideModule::simulateCurrentIndex(int idx)
{
  //	cout << "evaluating module " << name_ << endl;
  out_->GetArrayElement(idx).EvaluateIfInvalid();
}

void heavisideModule::initLocalParams()
{
  localParameterType* tmpParam;

  // save standard value for output amplitude
  tmpParam = new localParameterType("A");
  tmpParam->SetVal(1);
  AppendDataProp(tmpParam);

  out_->GetParser()->DefineVar(tmpParam->GetName(), tmpParam->GetParserVar());
}


/*******************************************************************************************
 * rectengularModule
 *******************************************************************************************/

rectangularModule::rectangularModule(const string& name, const string& sds, const string& lds, const string& htm) :
		    ITimeModule(name, sds, lds, htm),
		    out_(NULL)
{
  out_ = new OPortType(C_ART_na, 5, "out");
  out_->SetDefinition("out[t] = (((t*T) >= S) and ((t*T) <= E)) ? A : 0");
  out_->SetParentModuleName(name_);
  AppendDataProp(out_);
  initLocalParams();
}

ITimeModule* rectangularModule::Create(const string& name, const string& sds, const string& lds, const string& htm)
{
  return new rectangularModule(name, sds, lds, htm);
}

void rectangularModule::addIPort(const string& name, const DataProp* refPort)
{
  throw ARTerror("rectangularModule::addIPort", "Operation not permitted for time module '%s1'.",
      name_);
}

DataProp* rectangularModule::getPort(const string& name)
{
  if (name != "out")
  {
    throw ARTerror("rectangularModule::getPort", "Time module '%s1' has no port '%s2'.",
        name_, name);
  }
  return out_;
}

void rectangularModule::setCurrentIndex(int idx)
{
  out_->SetCurrentIndex(idx);
}

void rectangularModule::simulateCurrentIndex(int idx)
{
  //	cout << "evaluating module " << name_ << endl;
  out_->GetArrayElement(idx).EvaluateIfInvalid();
}

void rectangularModule::initLocalParams()
{
  localParameterType* tmpParam;

  // save standard value for output amplitude
  tmpParam = new localParameterType("A");
  tmpParam->SetVal(1);
  AppendDataProp(tmpParam);

  out_->GetParser()->DefineVar(tmpParam->GetName(), tmpParam->GetParserVar());

  // save standard value for begin value
  tmpParam = new localParameterType("S");
  tmpParam->SetVal(0);
  AppendDataProp(tmpParam);

  out_->GetParser()->DefineVar(tmpParam->GetName(), tmpParam->GetParserVar());

  // save standard value for end value
  tmpParam = new localParameterType("E");
  tmpParam->SetVal(1);
  AppendDataProp(tmpParam);

  out_->GetParser()->DefineVar(tmpParam->GetName(), tmpParam->GetParserVar());
}

/*******************************************************************************************
 * amplificationModule
 *******************************************************************************************/

amplificationModule::amplificationModule(const string& name, const string& sds, const string& lds, const string& htm) :
		    ITimeModule(name, sds, lds, htm),
		    out_(NULL)
{
  out_ = new OPortType(C_ART_na, 5, "out");
  out_->SetDefinition("out[t] = A*in[t]");
  out_->SetParentModuleName(name_);
  AppendDataProp(out_);
  initLocalParams();
}

ITimeModule* amplificationModule::Create(const string& name, const string& sds, const string& lds, const string& htm)
{
  return new amplificationModule(name, sds, lds, htm);
}

void amplificationModule::addIPort(const string& name, const DataProp* refPort)
{
  if (name != "in")
  {
    throw ARTerror("amplificationModule::addIPort",
        "It is only possible to add an input port with name 'in' to this module!");
  }
  const OPortType* oPort = dynamic_cast<const OPortType*>(refPort);
  if (!refPort)
  {
    throw ARTerror("amplificationModule::addIPort", "Port '%s1' is no valid output port",
        refPort->GetName());
  }
  in_ = new IPortType(name, oPort);
  out_->GetParser()->DefineVar(name, in_->GetParserVar());
  AppendDataProp(in_);
}

DataProp* amplificationModule::getPort(const string& name)
{
  if (name != "out")
  {
    throw ARTerror("amplificationModule::getPort", "Time module '%s1' has no port '%s2'.",
        name_, name);
  }
  return out_;
}

void amplificationModule::setCurrentIndex(int idx)
{
  out_->SetCurrentIndex(idx);
}

void amplificationModule::simulateCurrentIndex(int idx)
{
  //	cout << "evaluating module " << name_ << endl;
  out_->GetArrayElement(idx).EvaluateIfInvalid();
}

void amplificationModule::initLocalParams()
{
  localParameterType* tmpParam;

  // save standard value for output amplitude
  tmpParam = new localParameterType("A");
  tmpParam->SetVal(1);
  AppendDataProp(tmpParam);

  out_->GetParser()->DefineVar(tmpParam->GetName(), tmpParam->GetParserVar());
}

/*******************************************************************************************
 * simpleDelayModule
 *******************************************************************************************/

simpleDelayModule::simpleDelayModule(const string& name, const string& sds, const string& lds, const string& htm) :
		    ITimeModule(name, sds, lds, htm),
		    out_(NULL)
{
  out_ = new OPortType(C_ART_na, 5, "out");
  out_->SetDefinition("out[t] = in[t - ((round)(Delay/T))]");
  out_->SetParentModuleName(name_);
  AppendDataProp(out_);
  initLocalParams();
}

ITimeModule* simpleDelayModule::Create(const string& name, const string& sds, const string& lds, const string& htm)
{
  return new simpleDelayModule(name, sds, lds, htm);
}

void simpleDelayModule::addIPort(const string& name, const DataProp* refPort)
{
  if (name != "in")
  {
    throw ARTerror("simpleDelayModule::addIPort",
        "It is only possible to add an input port with name 'in' to this module!");
  }
  const OPortType* oPort = dynamic_cast<const OPortType*>(refPort);
  if (!refPort)
  {
    throw ARTerror("simpleDelayModule::addIPort", "Port '%s1' is no valid output port",
        refPort->GetName());
  }
  in_ = new IPortType(name, oPort);
  out_->GetParser()->DefineVar(name, in_->GetParserVar());
  AppendDataProp(in_);
}

DataProp* simpleDelayModule::getPort(const string& name)
{
  if (name != "out")
  {
    throw ARTerror("simpleDelayModule::getPort", "Time module '%s1' has no port '%s2'.",
        name_, name);
  }
  return out_;
}

void simpleDelayModule::setCurrentIndex(int idx)
{
  out_->SetCurrentIndex(idx);
}

void simpleDelayModule::simulateCurrentIndex(int idx)
{
  //	cout << "evaluating module " << name_ << endl;
  out_->GetArrayElement(idx).EvaluateIfInvalid();
}

void simpleDelayModule::initLocalParams()
{
  localParameterType* tmpParam;

  // save standard value for output amplitude
  tmpParam = new localParameterType("Delay");
  tmpParam->SetVal(1);
  AppendDataProp(tmpParam);

  out_->GetParser()->DefineVar(tmpParam->GetName(), tmpParam->GetParserVar());
}

/*******************************************************************************************
 * addModule
 *******************************************************************************************/

addModule::addModule(const string& name, const string& sds, const string& lds, const string& htm) :
		    ITimeModule(name, sds, lds, htm),
		    out_(NULL)
{
  out_ = new OPortType(C_ART_na, 5, "out");
  out_->SetDefinition("out[t] = in1[t] + in2[t]");
  out_->SetParentModuleName(name_);
  AppendDataProp(out_);
}

ITimeModule* addModule::Create(const string& name, const string& sds, const string& lds, const string& htm)
{
  return new addModule(name, sds, lds, htm);
}

void addModule::addIPort(const string& name, const DataProp* refPort)
{
  IPortType* iPort;
  if (name != "in1" && name != "in2")
  {
    throw ARTerror("addModule::addIPort",
        "It is only possible to add an input port with name 'in1' or 'in2' to this module! %s1", name);
  }
  const OPortType* oPort = dynamic_cast<const OPortType*>(refPort);
  if (!refPort)
  {
    throw ARTerror("addModule::addIPort", "Port '%s1' is no valid output port",
        refPort->GetName());
  }
  iPort = new IPortType(name, oPort);
  out_->GetParser()->DefineVar(name, iPort->GetParserVar());
  AppendDataProp(iPort);
}

DataProp* addModule::getPort(const string& name)
{
  if (name != "out")
  {
    throw ARTerror("simpleDelayModule::getPort", "Time module '%s1' has no port '%s2'.",
        name_, name);
  }
  return out_;
}

void addModule::setCurrentIndex(int idx)
{
  out_->SetCurrentIndex(idx);
}

void addModule::simulateCurrentIndex(int idx)
{
  //	cout << "evaluating module " << name_ << endl;
  out_->GetArrayElement(idx).EvaluateIfInvalid();
}

/*******************************************************************************************
 * multiplicationModule
 *******************************************************************************************/

multiplicationModule::multiplicationModule(const string& name, const string& sds, const string& lds, const string& htm) :
		    ITimeModule(name, sds, lds, htm),
		    out_(NULL)
{
  out_ = new OPortType(C_ART_na, 5, "out");
  out_->SetDefinition("out[t] = in1[t] * in2[t]");
  out_->SetParentModuleName(name_);
  AppendDataProp(out_);
}

ITimeModule* multiplicationModule::Create(const string& name, const string& sds, const string& lds, const string& htm)
{
  return new multiplicationModule(name, sds, lds, htm);
}

void multiplicationModule::addIPort(const string& name, const DataProp* refPort)
{
  IPortType* iPort;
  if (name != "in1" && name != "in2")
  {
    throw ARTerror("multiplicationModule::addIPort",
        "It is only possible to add an input port with name 'in1' or 'in2' to this module! %s1", name);
  }
  const OPortType* oPort = dynamic_cast<const OPortType*>(refPort);
  if (!refPort)
  {
    throw ARTerror("addModule::addIPort", "Port '%s1' is no valid output port",
        refPort->GetName());
  }
  iPort = new IPortType(name, oPort);
  out_->GetParser()->DefineVar(name, iPort->GetParserVar());
  AppendDataProp(iPort);
}

DataProp* multiplicationModule::getPort(const string& name)
{
  if (name != "out")
  {
    throw ARTerror("multiplicationModule::getPort", "Time module '%s1' has no port '%s2'.",
        name_, name);
  }
  return out_;
}

void multiplicationModule::setCurrentIndex(int idx)
{
  out_->SetCurrentIndex(idx);
}

void multiplicationModule::simulateCurrentIndex(int idx)
{
  //	cout << "evaluating module " << name_ << endl;
  out_->GetArrayElement(idx).EvaluateIfInvalid();
}


/*******************************************************************************************
 * sinewaveModule
 *******************************************************************************************/

sinewaveModule::sinewaveModule(const string& name, const string& sds, const string& lds, const string& htm) :
		    ITimeModule(name, sds, lds, htm),
		    out_(NULL)
{
  out_ = new OPortType(C_ART_na, 5, "out");
  out_->SetDefinition("out[t] = A*sin(2*pi*f*(t*T - Delta))");
  out_->SetParentModuleName(name_);
  AppendDataProp(out_);
  initLocalParams();
}

ITimeModule* sinewaveModule::Create(const string& name, const string& sds, const string& lds, const string& htm)
{
  return new sinewaveModule(name, sds, lds, htm);
}

void sinewaveModule::addIPort(const string& name, const DataProp* refPort)
{
  throw ARTerror("sinewaveModule::addIPort", "Operation not permitted for time module '%s1'.",
      name_);
}

DataProp* sinewaveModule::getPort(const string& name)
{
  if (name != "out")
  {
    throw ARTerror("sinewaveModule::getPort", "Time module '%s1' has no port '%s2'.",
        name_, name);
  }
  return out_;
}

void sinewaveModule::setCurrentIndex(int idx)
{
  out_->SetCurrentIndex(idx);
}

void sinewaveModule::simulateCurrentIndex(int idx)
{
//  cout << "evaluating module " << name_ << "," << idx << endl;
  out_->GetArrayElement(idx).EvaluateIfInvalid();
}

void sinewaveModule::initLocalParams()
{
  localParameterType* tmpParam;

  // save standard value for output amplitude
  tmpParam = new localParameterType("A");
  tmpParam->SetVal(1);
  AppendDataProp(tmpParam);

  out_->GetParser()->DefineVar(tmpParam->GetName(), tmpParam->GetParserVar());

  // save standard value for begin value
  tmpParam = new localParameterType("f");
  tmpParam->SetVal(10);
  AppendDataProp(tmpParam);

  out_->GetParser()->DefineVar(tmpParam->GetName(), tmpParam->GetParserVar());

  // save standard value for end value
  tmpParam = new localParameterType("Delta");
  tmpParam->SetVal(0);
  AppendDataProp(tmpParam);

  out_->GetParser()->DefineVar(tmpParam->GetName(), tmpParam->GetParserVar());
}

/*******************************************************************************************
 * genericDelayModule
 *******************************************************************************************/

double genericDelayModule::getLagrangeParams(int n, int N, double D)
{
  int k = 0;

  double result = 1;

  for (k = 0; k <= N; ++k)
  {
    if (k != n)
    {
      result *= ((D - k)/(n - k));
    }
  }

  return result;
}

double genericDelayModule::getThiranParams(int n, int N, double D)
{
  int k = 0;
  double result = (n%2 == 0) ? 1 : -1;
  result *= binom(N, n);

  for (k = 0; k <= N; ++k)
  {
    result *= ((D - N + k)/(D - N  + n + k));
  }

  return result;
}

double genericDelayModule::fac(int n)
{
  double result = 1;
  int i;
  for (i = 1; i <= n; ++i)
  {
    result *= i;
  }
  return result;
}

double genericDelayModule::binom(int n, int k)
{
  double numerator = fac(n);
  double denominator = fac(k) * fac(n -k);
  return numerator / denominator;
}


/*******************************************************************************************
 * fractionDelayModule
 *******************************************************************************************/

fractionalDelayModule::fractionalDelayModule(const string& name, const string& sds, const string& lds, const string& htm) :
		    genericDelayModule(name, sds, lds, htm),
		    out_(NULL),
		    in_(NULL)
{
  initLocalParams();
  out_ = new OPortType(C_ART_na, 20, "out");
  out_->SetParentModuleName(name_);
  AppendDataProp(out_);
}

ITimeModule* fractionalDelayModule::Create(const string& name, const string& sds, const string& lds, const string& htm)
{
  return new fractionalDelayModule(name, sds, lds, htm);
}

void fractionalDelayModule::addIPort(const string& name, const DataProp* refPort)
{
  if (name != "in")
  {
    throw ARTerror("fractionalDelayModule::addIPort",
        "It is only possible to add an input port with name 'in' to this module!");
  }
  const OPortType* oPort = dynamic_cast<const OPortType*>(refPort);
  if (!refPort)
  {
    throw ARTerror("fractionalDelayModule::addIPort", "Port '%s1' is no valid output port",
        refPort->GetName());
  }
  in_ = new IPortType(name, oPort);
  out_->GetParser()->DefineVar(name, in_->GetParserVar());
  AppendDataProp(in_);
}

DataProp* fractionalDelayModule::getPort(const string& name)
{
  if (name != "out")
  {
    throw ARTerror("fractionalDelayModule::getPort", "Time module '%s1' has no port '%s2'.",
        name_, name);
  }
  return out_;
}

void fractionalDelayModule::setCurrentIndex(int idx)
{
  if (idx == 0)
  {
    initSimulation();
  }
  out_->SetCurrentIndex(idx);
}

void fractionalDelayModule::simulateCurrentIndex(int idx)
{
  //	cout << "evaluating module " << name_ << endl;
  out_->GetArrayElement(idx).EvaluateIfInvalid();
}

void fractionalDelayModule::initLocalParams()
{
  localParameterType* tmpParam;

  // save standard type of filter
  tmpParam = new localParameterType("type", "type of the fractional delay filter, may be either 'lagrangre' or 'thiran'");
  tmpParam->SetType(C_ART_str, 0);
  tmpParam->SetVal("lagrange");
  AppendDataProp(tmpParam);

  // save standard order of implemented filter
  tmpParam = new localParameterType("order", "order of the filter");
  tmpParam->SetVal(5);
  AppendDataProp(tmpParam);

  // save standard delay of current module => currently 0 seconds
  tmpParam = new localParameterType("Delay", "delay in seconds");
  tmpParam->SetVal(0.0);
  AppendDataProp(tmpParam);
}

void fractionalDelayModule::initSimulation()
{
  localParameterType* type = dynamic_cast<localParameterType*>(FindProperty("type"));
  localParameterType* order = dynamic_cast<localParameterType*>(FindProperty("order"));
  localParameterType* Delay = dynamic_cast<localParameterType*>(FindProperty("Delay"));
  globalParameterType* T = dynamic_cast<globalParameterType*>(FindProperty("T"));

  int n;
  bool warn = false;

  std::stringstream expr;
  //	expr.precision(10);

  //	cout << "Delay = " << Delay->GetFloat() << ", T = " << T->GetParserVar().GetFloat() << endl;

  // calculate normalized delay
  double D = (Delay->GetFloat()) / (T->GetParserVar().GetFloat());
  int N = order->GetInt();

  // make sure that normalized delay is below one sampling period such =>
  // use simple delay and fractional delay in combination
  //	if (D >= 1.0)
  //	{
  //		throw ARTerror("fractionalDelayModule::initSimulation",
  //				"The current value for 'Delay' of module '%s1' is greater than a single sampling period. Please use a delay module and the fractional delay module to get this effect!",
  //				name_);
  //	}

  // init expression
  expr << "out[t] = ";

  if (type->GetString() == "lagrange")
  {
    // print out warning if order does not fit delay time
    if (N % 2 == 0)
    {
      if (((N/2 - 1) > D) || ((N/2 + 1) < D))
      {
        warn = true;
      }
    }
    else
    {
      if ((((N-1)/2) > D) || (((N+1)/2) < D))
      {
        warn = true;
      }
    }

    for (n = 0; n <= N; ++n)
    {
      if (n != 0)
      {
        expr << " + ";
      }
      expr << getLagrangeParams(n, N, D);
      expr << "*in[t - " << n << "]";
    }
  }
  else if (type->GetString() == "thiran")
  {
    // print out warning if order does not fit delay time
    if (((N - 0.5) > D) || ((N + 0.5) < D))
    {
      warn = true;
    }

    for (n = 1; n <= N; ++n)
    {
      // init output port to zero for thiran IIR
      (*out_)[-n] = 0;

      if (n != 1)
      {
        expr << " + ";
      }
      expr << getThiranParams(n, N, D);
      expr << "*(in[t - " << N - n << "] - out[t - " << n << "])";
    }

    expr << " + in[t - " << N << "]";
  }
  else
  {
    throw ARTerror("fractionalDelayModule::initSimulation",
        "Unknown type '%s1' of module '%s2': Current implementations provide 'lagrange' and 'thiran' fractional delay filters!",
        type->GetString(), name_);
  }

  if (warn)
  {
    cerr << "Warning: In fractionalDelayModule::initSimulation() of module '"
        << name_ << "': The specified filter order does not provide a stable result."
        << " Please change either the filter order or the delay time." << endl;
  }

  //	cout << "D = " << D << "\nExpression for FD: " << expr.str() << endl;

  // save whole definition to output port
  out_->SetDefinition(expr.str());
}

/*******************************************************************************************
 * DWGcylinderModule
 *******************************************************************************************/

DWGcylinderModule::DWGcylinderModule(const string& name, const string& sds, const string& lds, const string& htm) :
		    genericDelayModule(name, sds, lds, htm),
		    p1p_(NULL),
		    p2p_(NULL),
		    p1m_(NULL),
		    p2m_(NULL),
		    p1pbuf_(NULL),
		    p2mbuf_(NULL)
{
  initLocalParams();
  p2p_ = new OPortType(C_ART_na, 20, "p2p");
  p2p_->SetParentModuleName(name_);
  AppendDataProp(p2p_);
  p1m_ = new OPortType(C_ART_na, 20, "p1m");
  p1m_->SetParentModuleName(name_);
  AppendDataProp(p1m_);
  // internal buffer ports for input ports
  p1pbuf_ = new OPortType(C_ART_na, 20, "p1pbuf");
  p1pbuf_->SetDefinition("p1pbuf[t] = p1p[t - 1]");
  p1pbuf_->SetParentModuleName(name_);
  AppendDataProp(p1pbuf_);
  p2mbuf_ = new OPortType(C_ART_na, 20, "p2mbuf");
  p2mbuf_->SetDefinition("p2mbuf[t] = p2m[t - 1]");
  p2mbuf_->SetParentModuleName(name_);
  AppendDataProp(p2mbuf_);

  // register buffers to output ports
  p2p_->GetParser()->DefineVar("p1p", p1pbuf_->GetParserVar());
  p1m_->GetParser()->DefineVar("p2m", p2mbuf_->GetParserVar());
}

ITimeModule* DWGcylinderModule::Create(const string& name, const string& sds, const string& lds, const string& htm)
{
  return new DWGcylinderModule(name, sds, lds, htm);
}

void DWGcylinderModule::addIPort(const string& name, const DataProp* refPort)
{
  if (name != "p1p" && name != "p2m")
  {
    throw ARTerror("DWGcylinderModule::addIPort",
        "It is only possible to add input ports with names 'p1p' and 'p2m' to this module!");
  }

  if (FindProperty(name))
  {
    throw ARTerror("DWGcylinderModule::addIPort",
        "An input port with name '%s1' already exists in module '%s2'!",
        name, name_);
  }

  const OPortType* oPort = dynamic_cast<const OPortType*>(refPort);
  if (!refPort)
  {
    throw ARTerror("DWGcylinderModule::addIPort", "Port '%s1' is no valid output port",
        refPort->GetName());
  }

  if (name == "p1p")
  {
    p1p_ = new IPortType(name, oPort);
    p1pbuf_->GetParser()->DefineVar(name, p1p_->GetParserVar());
    AppendDataProp(p1p_);
  }
  else
  {
    p2m_ = new IPortType(name, oPort);
    p2mbuf_->GetParser()->DefineVar(name, p2m_->GetParserVar());
    AppendDataProp(p2m_);
  }
}

DataProp* DWGcylinderModule::getPort(const string& name)
{
  if (name != "p2p" && name != "p1m")
  {
    throw ARTerror("DWGcylinderModule::getPort", "Time module '%s1' has no port '%s2'.",
        name_, name);
  }
  else
  {
    return dynamic_cast<DataProp*>(FindProperty(name));
  }
}

void DWGcylinderModule::setCurrentIndex(int idx)
{
  if (idx == 0)
  {
    initSimulation();
  }
  p1pbuf_->SetCurrentIndex(idx);
  p2mbuf_->SetCurrentIndex(idx);

  p2p_->SetCurrentIndex(idx);
  p1m_->SetCurrentIndex(idx);
}

void DWGcylinderModule::simulateCurrentIndex(int idx)
{
  //	cout << "evaluating module " << name_ << endl;

  p1pbuf_->SetCurrentIndex(idx);
  p2mbuf_->SetCurrentIndex(idx);

  p2p_->GetArrayElement(idx).EvaluateIfInvalid();
  p1m_->GetArrayElement(idx).EvaluateIfInvalid();
}

void DWGcylinderModule::initLocalParams()
{
  localParameterType* tmpParam;

  // save standard type of filter
  tmpParam = new localParameterType("type", "type of the fractional delay filter, may be either 'lagrangre' or 'thiran'");
  tmpParam->SetType(C_ART_str, 0);
  tmpParam->SetVal("lagrange");
  AppendDataProp(tmpParam);

  // save standard length of implemented filter
  tmpParam = new localParameterType("length", "length of the cylinder in m");
  tmpParam->SetVal(0.05);
  AppendDataProp(tmpParam);
}

void DWGcylinderModule::initSimulation()
{

  localParameterType* type = dynamic_cast<localParameterType*>(FindProperty("type"));
  localParameterType* length = dynamic_cast<localParameterType*>(FindProperty("length"));
  globalParameterType* T = dynamic_cast<globalParameterType*>(FindProperty("T"));
  globalParameterType* c = dynamic_cast<globalParameterType*>(FindProperty("c"));

  int n;

  std::stringstream expr1;
  std::stringstream expr2;
  //	expr.precision(10);

  // calculate normalized delay
  double D;
  int N;

  if (c == NULL)
  {
    throw ARTerror("DWGcylinderModule::initSimulation",
        "No global parameter 'c' for the speed of sound has been found. Please add it to your current simulator!");
  }

  // depending on the length of cylinder, calculate value for delay
  D = (length->GetFloat()) / ((c->GetParserVar().GetFloat() * T->GetParserVar().GetFloat()));

  // decrease delay by one because we will introduce a separate delay buffer
  D--;

  // calculate filter order depending on filter type
  if (type->GetString() == "lagrange")
  {
    N = (int) (2*D + 1);
  }
  else if (type->GetString() == "thiran")
  {
    N = (int) (D + 0.5);
  }
  else
  {
    throw ARTerror("DWGcylinderModule::initSimulation",
        "Unknown type '%s1' of module '%s2': Current implementations provide 'lagrange' and 'thiran' fractional delay filters!",
        type->GetString(), name_);
  }

  // init expression
  expr1 << "p2p[t] = ";
  expr2 << "p1m[t] = ";

  if (type->GetString() == "lagrange")
  {
    for (n = 0; n <= N; ++n)
    {


      if (n != 0)
      {
        // init buffer of input ports
        (*p1pbuf_)[-n] = 0;
        (*p2mbuf_)[-n] = 0;

        expr1 << " + ";
        expr2 << " + ";
      }

      expr1 << getLagrangeParams(n, N, D);
      expr1 << "*p1p[t - " << n << "]";
      expr2 << getLagrangeParams(n, N, D);
      expr2 << "*p2m[t - " << n << "]";
    }
  }
  else if (type->GetString() == "thiran")
  {

    for (n = 1; n <= N; ++n)
    {
      // init output ports to zero for thiran IIR
      (*p2p_)[-n] = 0;
      (*p1m_)[-n] = 0;
      // init buffer of input ports
      (*p1pbuf_)[-n] = 0;
      (*p2mbuf_)[-n] = 0;

      // set up calculation expression
      if (n != 1)
      {
        expr1 << " + ";
        expr2 << " + ";
      }
      expr1 << getThiranParams(n, N, D);
      expr1 << "*(p1p[t - " << N - n << "] - p2p[t - " << n << "])";
      expr2 << getThiranParams(n, N, D);
      expr2 << "*(p2m[t - " << N - n << "] - p1m[t - " << n << "])";
    }

    expr1 << " + p1p[t - " << N << "]";
    expr2 << " + p2m[t - " << N << "]";
  }

  //	cout << "Expression for p2p: " << expr1.str() << endl;
  //	cout << "Expression for p1m: " << expr2.str() << endl;

  // save whole definitions to output ports
  p2p_->SetDefinition(expr1.str());
  p1m_->SetDefinition(expr2.str());
}

/*******************************************************************************************
 * DWGcylinderJunctionModule
 *******************************************************************************************/

DWGcylinderJunctionModule::DWGcylinderJunctionModule(const string& name, const string& sds, const string& lds, const string& htm) :
		    ITimeModule(name, sds, lds, htm),
		    p1p_(NULL),
		    p2p_(NULL),
		    p1m_(NULL),
		    p2m_(NULL),
		    r1_(NULL),
		    r2_(NULL)
{
  initLocalParams();
  p2p_ = new OPortType(C_ART_na, 5, "p2p");
  p2p_->SetDefinition("p2p[t] = p1p[t] + (((r1)^2 - (r2)^2)/((r1)^2 + (r2)^2)) * (p1p[t] - p2m[t])");
  p2p_->GetParser()->DefineVar("r1", r1_->GetParserVar());
  p2p_->GetParser()->DefineVar("r2", r2_->GetParserVar());
  p2p_->SetParentModuleName(name_);
  AppendDataProp(p2p_);

  p1m_ = new OPortType(C_ART_na, 5, "p1m");
  p1m_->SetDefinition("p1m[t] = p2m[t] + (((r1)^2 - (r2)^2)/((r1)^2 + (r2)^2)) * (p1p[t] - p2m[t])");
  p1m_->GetParser()->DefineVar("r1", r1_->GetParserVar());
  p1m_->GetParser()->DefineVar("r2", r2_->GetParserVar());
  p1m_->SetParentModuleName(name_);
  AppendDataProp(p1m_);
}

ITimeModule* DWGcylinderJunctionModule::Create(const string& name, const string& sds, const string& lds, const string& htm)
{
  return new DWGcylinderJunctionModule(name, sds, lds, htm);
}

void DWGcylinderJunctionModule::addIPort(const string& name, const DataProp* refPort)
{
  if (name != "p1p" && name != "p2m")
  {
    throw ARTerror("DWGcylinderJunctionModule::addIPort",
        "It is only possible to add input ports with names 'p1p' and 'p2m' to this module!");
  }

  if (FindProperty(name))
  {
    throw ARTerror("DWGcylinderJunctionModule::addIPort",
        "An input port with name '%s1' already exists in module '%s2'!",
        name, name_);
  }

  const OPortType* oPort = dynamic_cast<const OPortType*>(refPort);
  if (!refPort)
  {
    throw ARTerror("DWGcylinderJunctionModule::addIPort", "Port '%s1' is no valid output port",
        refPort->GetName());
  }

  if (name == "p1p")
  {
    p1p_ = new IPortType(name, oPort);
    p2p_->GetParser()->DefineVar(name, p1p_->GetParserVar());
    p1m_->GetParser()->DefineVar(name, p1p_->GetParserVar());
    AppendDataProp(p1p_);
  }
  else
  {
    p2m_ = new IPortType(name, oPort);
    p2p_->GetParser()->DefineVar(name, p2m_->GetParserVar());
    p1m_->GetParser()->DefineVar(name, p2m_->GetParserVar());
    AppendDataProp(p2m_);
  }
}

DataProp* DWGcylinderJunctionModule::getPort(const string& name)
{
  if (name != "p2p" && name != "p1m")
  {
    throw ARTerror("DWGcylinderJunctionModule::getPort", "Time module '%s1' has no port '%s2'.",
        name_, name);
  }
  else
  {
    return dynamic_cast<DataProp*>(FindProperty(name));
  }
}

void DWGcylinderJunctionModule::setCurrentIndex(int idx)
{
  p2p_->SetCurrentIndex(idx);
  p1m_->SetCurrentIndex(idx);
}

void DWGcylinderJunctionModule::simulateCurrentIndex(int idx)
{
  //	cout << "evaluating module " << name_ << endl;
  p2p_->GetArrayElement(idx).EvaluateIfInvalid();
  p1m_->GetArrayElement(idx).EvaluateIfInvalid();
}

void DWGcylinderJunctionModule::initLocalParams()
{

  // save standard radius of left cylinder
  r1_ = new localParameterType("r1", "radius of left cylinder in m");
  r1_->SetVal(0.010);
  AppendDataProp(r1_);

  // save standard radius of right cylinder
  r2_ = new localParameterType("r2", "radius of right cylinder in m");
  r2_->SetVal(0.015);
  AppendDataProp(r2_);

}

/*******************************************************************************************
 * DWGconeModule
 *******************************************************************************************/

DWGconeModule::DWGconeModule(const string& name, const string& sds, const string& lds, const string& htm) :
		    DWGcylinderModule(name, sds, lds, htm)
{
  initLocalParams();
}

ITimeModule* DWGconeModule::Create(const string& name, const string& sds, const string& lds, const string& htm)
{
  return new DWGconeModule(name, sds, lds, htm);
}

void DWGconeModule::initLocalParams()
{
  localParameterType* tmpParam;

  //	// save standard type of filter
  //	tmpParam = new localParameterType("type", "type of the fractional delay filter, may be either 'lagrangre' or 'thiran'");
  //	tmpParam->SetType(C_ART_str, 0);
  //	tmpParam->SetVal("lagrange");
  //	AppendDataProp(tmpParam);
  //
  //	// save standard length of implemented filter
  //	tmpParam = new localParameterType("length", "length of the cylinder in mm");
  //	tmpParam->SetVal(50);
  //	AppendDataProp(tmpParam);
  tmpParam = new localParameterType("r1", "radius of the left wave sphere in m (mode = 'default')/ radius of the left cone ending (mode = 'boreprofile')");
  tmpParam->SetVal(50);
  AppendDataProp(tmpParam);

  p2p_->GetParser()->DefineVar(tmpParam->GetName(), tmpParam->GetParserVar());
  p1m_->GetParser()->DefineVar(tmpParam->GetName(), tmpParam->GetParserVar());

  tmpParam = new localParameterType("r2", "radius of the right wave sphere in m (mode = 'default')/ radius of the right cone ending (mode = 'boreprofile')");
  tmpParam->SetVal(100);
  AppendDataProp(tmpParam);

  p2p_->GetParser()->DefineVar(tmpParam->GetName(), tmpParam->GetParserVar());
  p1m_->GetParser()->DefineVar(tmpParam->GetName(), tmpParam->GetParserVar());

  tmpParam = new localParameterType("mode", "input mode of radii and length: 'default' means that r1 and r2 are interpreted as cone apex distances, whereas in 'boreprofile' "
      " the cone apex distances are calculated based on the given radii of the left (r1) and right (r2) cone opening and the specified length of the cone.");
  tmpParam->SetType(C_ART_str, 0);
  tmpParam->SetVal("default");
  AppendDataProp(tmpParam);
}

void DWGconeModule::initSimulation()
{

  localParameterType* type = dynamic_cast<localParameterType*>(FindProperty("type"));
  globalParameterType* T = dynamic_cast<globalParameterType*>(FindProperty("T"));
  globalParameterType* c = dynamic_cast<globalParameterType*>(FindProperty("c"));
  const string& mode = dynamic_cast<localParameterType*>(FindProperty("mode"))->GetString();

  int n;

  std::stringstream expr1;
  std::stringstream expr2;
  //	expr.precision(10);

  // calculate normalized delay
  double D, length;
  int N;

  if (c == NULL)
  {
    throw ARTerror("DWGconeModule::initSimulation",
        "No global parameter 'c' for the speed of sound has been found. Please add it to your current simulator!");
  }

  // if radii and length are given in bore profile mode,
  // we first have to calculate the cone apex distances
  if (mode == "boreprofile")
  {
    calculateConeApex();
  }

  // calculate length of cone module based on shperical appex radii
  length = std::abs(dynamic_cast<localParameterType*>(FindProperty("r1"))->GetFloat() -
      dynamic_cast<localParameterType*>(FindProperty("r2"))->GetFloat());

  // depending on the length of cylinder, calculate value for delay
  D = (length) / ((c->GetParserVar().GetFloat() * T->GetParserVar().GetFloat()));

  // decrease delay by one because we will introduce a separate delay buffer
  D--;

  // calculate filter order depending on filter type
  if (type->GetString() == "lagrange")
  {
    N = (int) (2*D + 1);
  }
  else if (type->GetString() == "thiran")
  {
    N = (int) (D + 0.5);
  }
  else
  {
    throw ARTerror("DWGconeModule::initSimulation",
        "Unknown type '%s1' of module '%s2': Current implementations provide 'lagrange' and 'thiran' fractional delay filters!",
        type->GetString(), name_);
  }

  // init expression
  expr1 << "p2p[t] = (r1/r2)*(";
  expr2 << "p1m[t] = (r2/r1)*(";

  if (type->GetString() == "lagrange")
  {
    for (n = 0; n <= N; ++n)
    {

      if (n != 0)
      {
        // init buffer of input ports
        (*p1pbuf_)[-n] = 0;
        (*p2mbuf_)[-n] = 0;

        expr1 << " + ";
        expr2 << " + ";
      }
      expr1 << getLagrangeParams(n, N, D);
      expr1 << "*p1p[t - " << n << "]";
      expr2 << getLagrangeParams(n, N, D);
      expr2 << "*p2m[t - " << n << "]";
    }
    expr1 << ")";
    expr2 << ")";
  }
  else if (type->GetString() == "thiran")
  {

    for (n = 1; n <= N; ++n)
    {
      // init output ports to zero for thiran IIR
      (*p2p_)[-n] = 0;
      (*p1m_)[-n] = 0;

      // init buffer of input ports
      (*p1pbuf_)[-n] = 0;
      (*p2mbuf_)[-n] = 0;

      // set up calculation expression
      if (n != 1)
      {
        expr1 << " + ";
        expr2 << " + ";
      }
      expr1 << getThiranParams(n, N, D);
      expr1 << "*(p1p[t - " << N - n << "] - p2p[t - " << n << "])";
      expr2 << getThiranParams(n, N, D);
      expr2 << "*(p2m[t - " << N - n << "] - p1m[t - " << n << "])";
    }

    expr1 << " + p1p[t - " << N << "])";
    expr2 << " + p2m[t - " << N << "])";
  }

  //	cout << "D = " << D << "\nExpression for FD: " << expr1.str() << endl;
  //	cout << "D = " << D << "\nExpression for FD: " << expr2.str() << endl;

  // save whole definitions to output ports
  p2p_->SetDefinition(expr1.str());
  p1m_->SetDefinition(expr2.str());
}

void DWGconeModule::calculateConeApex()
{
  double r1 = dynamic_cast<localParameterType*>(FindProperty("r1"))->GetFloat();
  double r2 = dynamic_cast<localParameterType*>(FindProperty("r2"))->GetFloat();
  double length = dynamic_cast<localParameterType*>(FindProperty("length"))->GetFloat();

  double Rl, Rr, L;

  // calculate cone apex distances
  // case left radius is greater than right one => converging cone
  if (r1 > r2)
  {
    L = (length*r1)/(r1 - r2);
    Rl = std::sqrt(r1*r1 + L*L);
    Rr = std::sqrt(r2*r2 + (L - length)*(L - length));
  }
  // case right radius is greater than left one => diverging cone
  else
  {
    L = (length*r2)/(r2 - r1);
    Rl = std::sqrt(r1*r1 + (L - length)*(L - length));
    Rr = std::sqrt(r2*r2 + L*L);
  }

  // save calculated radii
  dynamic_cast<localParameterType*>(FindProperty("r1"))->SetVal(Rl);
  dynamic_cast<localParameterType*>(FindProperty("r2"))->SetVal(Rr);

}

/*******************************************************************************************
 * DWGconeJunctionModule
 *******************************************************************************************/

DWGconeJunctionModule::DWGconeJunctionModule(const string& name, const string& sds, const string& lds, const string& htm) :
		    ITimeModule(name, sds, lds, htm),
		    p1p_(NULL),
		    p2p_(NULL),
		    p1m_(NULL),
		    p2m_(NULL),
		    rz_(NULL),
		    r1_(NULL),
		    r2_(NULL),
		    S1_(NULL),
		    S2_(NULL)
{
  initLocalParams();

  rz_ = new OPortType(C_ART_na, 5, "rz");
  rz_->SetParentModuleName(name_);
  AppendDataProp(rz_);

  p2p_ = new OPortType(C_ART_na, 20, "p2p");
  p2p_->GetParser()->DefineVar("rz", rz_->GetParserVar());
  p2p_->SetParentModuleName(name_);
  AppendDataProp(p2p_);
  p1m_ = new OPortType(C_ART_na, 20, "p1m");
  p1m_->GetParser()->DefineVar("rz", rz_->GetParserVar());
  p1m_->SetParentModuleName(name_);
  AppendDataProp(p1m_);

}

ITimeModule* DWGconeJunctionModule::Create(const string& name, const string& sds, const string& lds, const string& htm)
{
  return new DWGconeJunctionModule(name, sds, lds, htm);
}

void DWGconeJunctionModule::addIPort(const string& name, const DataProp* refPort)
{
  if (name != "p1p" && name != "p2m")
  {
    throw ARTerror("DWGconeJunctionModule::addIPort",
        "It is only possible to add input ports with names 'p1p' and 'p2m' to this module!");
  }

  if (FindProperty(name))
  {
    throw ARTerror("DWGconeJunctionModule::addIPort",
        "An input port with name '%s1' already exists in module '%s2'!",
        name, name_);
  }

  const OPortType* oPort = dynamic_cast<const OPortType*>(refPort);
  if (!refPort)
  {
    throw ARTerror("DWGconeJunctionModule::addIPort", "Port '%s1' is no valid output port",
        refPort->GetName());
  }

  if (name == "p1p")
  {
    p1p_ = new IPortType(name, oPort);
    p2p_->GetParser()->DefineVar(name, p1p_->GetParserVar());
    p1m_->GetParser()->DefineVar(name, p1p_->GetParserVar());
    rz_->GetParser()->DefineVar(name, p1p_->GetParserVar());
    AppendDataProp(p1p_);
  }
  else
  {
    p2m_ = new IPortType(name, oPort);
    p2p_->GetParser()->DefineVar(name, p2m_->GetParserVar());
    p1m_->GetParser()->DefineVar(name, p2m_->GetParserVar());
    rz_->GetParser()->DefineVar(name, p2m_->GetParserVar());
    AppendDataProp(p2m_);
  }
}

DataProp* DWGconeJunctionModule::getPort(const string& name)
{
  if (name != "p2p" && name != "p1m")
  {
    throw ARTerror("DWGconeJunctionModule::getPort", "Time module '%s1' has no port '%s2'.",
        name_, name);
  }
  else
  {
    return dynamic_cast<DataProp*>(FindProperty(name));
  }
}

void DWGconeJunctionModule::setCurrentIndex(int idx)
{
  if (idx == 0)
  {
    initSimulation();
  }
  p2p_->SetCurrentIndex(idx);
  p1m_->SetCurrentIndex(idx);
  rz_->SetCurrentIndex(idx);
}

void DWGconeJunctionModule::simulateCurrentIndex(int idx)
{
  //	cout << "evaluating module " << name_ << endl;
  p2p_->GetArrayElement(idx).EvaluateIfInvalid();
  p1m_->GetArrayElement(idx).EvaluateIfInvalid();
  rz_->GetArrayElement(idx).EvaluateIfInvalid();
}

void DWGconeJunctionModule::initLocalParams()
{
  localParameterType* tmpParam;

  // save standard type of filter
  tmpParam = new localParameterType("method", "method of calculating the internal filter; may be either 'IIM', 'BT' or 'TICM'");
  tmpParam->SetType(C_ART_str, 0);
  tmpParam->SetVal("IIM");
  AppendDataProp(tmpParam);

  // save standard radius of left cone
  r1_ = new localParameterType("r1", "radius of the left wave sphere in m (used in default mode)");
  r1_->SetVal(0.050);
  AppendDataProp(r1_);

  // save standard radius of right cone
  r2_ = new localParameterType("r2", "radius of the right wave sphere in m (used in default mode)");
  r2_->SetVal(0.050);
  AppendDataProp(r2_);

  // save standard wave area of left cone
  S1_ = new localParameterType("S1", "area of the left wave sphere in m (used in default mode)");
  S1_->SetVal(0.050);
  AppendDataProp(S1_);

  // save standard wave area of right cone
  S2_ = new localParameterType("S2", "area of the right wave sphere in m (used in default mode)");
  S2_->SetVal(0.050);
  AppendDataProp(S2_);

  // save standard value for left end radius of the left cone
  tmpParam = new localParameterType("lr1", "radius of the left end of the left cone in m (used in boreprofile mode)");
  tmpParam->SetVal(0.01);
  AppendDataProp(tmpParam);

  // save standard value for right end radius of the left cone
  tmpParam = new localParameterType("lr2", "radius of the right end of the left cone in m (used in boreprofile mode)");
  tmpParam->SetVal(0.015);
  AppendDataProp(tmpParam);

  // save standard value for length of the left cone
  tmpParam = new localParameterType("llength", "length of the left cone in m (used in boreprofile mode)");
  tmpParam->SetVal(0.1);
  AppendDataProp(tmpParam);

  // save standard value for left end radius of the right cone
  tmpParam = new localParameterType("rr1", "radius of the left end of the right cone in m (used in boreprofile mode)");
  tmpParam->SetVal(0.01);
  AppendDataProp(tmpParam);

  // save standard value for right end radius of the right cone
  tmpParam = new localParameterType("rr2", "radius of the right end of the right cone in m (used in boreprofile mode)");
  tmpParam->SetVal(0.015);
  AppendDataProp(tmpParam);

  // save standard value for length of the right cone
  tmpParam = new localParameterType("rlength", "length of the right cone in m (used in boreprofile mode)");
  tmpParam->SetVal(0.1);
  AppendDataProp(tmpParam);

  tmpParam = new localParameterType("mode", "input mode of radii and length: 'default' means that r1, r2, S1 and S2 have to be specifed as cone"
      " apex distances and area of the corresponding wave spheres whereas in 'boreprofile' mode, the radii and length of the left and right cone"
      " have to be specified.");
  tmpParam->SetType(C_ART_str, 0);
  tmpParam->SetVal("default");
  AppendDataProp(tmpParam);
}

void DWGconeJunctionModule::initSimulation()
{
  string method = dynamic_cast<localParameterType*>(FindProperty("method"))->GetString();
  string mode = dynamic_cast<localParameterType*>(FindProperty("mode"))->GetString();

  double S1, S2, B, C1, C2, C3;

  if (mode == "boreprofile")
  {
    calculateConeApex();
  }

  S1 = S1_->GetFloat();
  S2 = S2_->GetFloat();

  B = S1/S2;

  //	cout << "B = " << B << ", S1 = " << S1 << ", S2 = " << S2 << endl;

  C1 = 2*B/(B+1);
  C2 = 2/(B+1);
  C3 = (B-1)/(B+1);
  //	C1 = 1;
  //	C2 = 1;
  //	C3 = 0;

  std::stringstream exprrz;

  std::stringstream expr1;
  std::stringstream expr2;

  // set expression for R(z)
  exprrz << "rz[t] = ";
  exprrz << getB0(method) << "*(" << C1 << "*p1p[t] + " << C2 << "*p2m[t]) + ";
  exprrz << getB1(method) << "*(" << C1 << "*p1p[t-1] + " << C2 << "*p2m[t-1]) - ";
  exprrz << getA1(method) << "*rz[t-1]";

  //	cout << "b0 = " << getB0(method) << ", b1 = " << getB1(method) << ", a1 = " << getA1(method) << endl;

  // init value of rz[-1] for IIR filter
  (*rz_)[-1] = 0.0;

  //	cout << "Expression for rz: " << exprrz.str() << endl;

  // set expression for p2p[t]
  expr1 << "p2p[t] = p1p[t] + rz[t] + " << C3 << "*(p1p[t] - p2m[t])";
  expr2 << "p1m[t] = p2m[t] + rz[t] + " << C3 << "*(p1p[t] - p2m[t])";

  //	cout << "Expression for p2p: " << expr1.str() << endl;
  //	cout << "Expression for p1m: " << expr2.str() << endl;

  // save whole definitions to output ports
  rz_->SetDefinition(exprrz.str());
  p2p_->SetDefinition(expr1.str());
  p1m_->SetDefinition(expr2.str());
}

void DWGconeJunctionModule::calculateConeApex()
{
  double lr1 = dynamic_cast<localParameterType*>(FindProperty("lr1"))->GetFloat();
  double lr2 = dynamic_cast<localParameterType*>(FindProperty("lr2"))->GetFloat();
  double llength = dynamic_cast<localParameterType*>(FindProperty("llength"))->GetFloat();

  double rr1 = dynamic_cast<localParameterType*>(FindProperty("rr1"))->GetFloat();
  double rr2 = dynamic_cast<localParameterType*>(FindProperty("rr2"))->GetFloat();
  double rlength = dynamic_cast<localParameterType*>(FindProperty("rlength"))->GetFloat();

  double Rl, Rr, L, S;

  // calculate cone apex distances for left cone
  // case left radius is greater than right one => converging cone
  if (lr1 > lr2)
  {
    L = (llength*lr1)/(lr1 - lr2);
    Rl = std::sqrt(lr1*lr1 + L*L);
    Rr = std::sqrt(lr2*lr2 + (L - llength)*(L - llength));

    // set negative value for converging cone
    r1_->SetVal(-Rr);
    // calculate area of sphere cap
    S = 2*PI*Rr*(Rr - (L - llength));
    S1_->SetVal(S);
  }
  // case left radius is equal to right one => we have a cylinder
  else if (lr1 == lr2)
  {
    // cone apex distance is about infinity
    r1_->SetVal(1E300);
    // area is just the area of the circle
    S = lr1*lr1*PI;
    S1_->SetVal(S);
  }
  // case right radius is greater than left one => diverging cone
  else
  {
    L = (llength*lr2)/(lr2 - lr1);
    Rl = std::sqrt(lr1*lr1 + (L - llength)*(L - llength));
    Rr = std::sqrt(lr2*lr2 + L*L);

    // set positive value for diverging cone
    r1_->SetVal(Rr);
    // calculate area of sphere cap
    S = 2*PI*Rr*(Rr - L);
    S1_->SetVal(S);
  }

  // calculate cone apex distances for right cone
  // case left radius is greater than right one => converging cone
  if (rr1 > rr2)
  {
    L = (rlength*rr1)/(rr1 - rr2);
    Rl = std::sqrt(rr1*rr1 + L*L);
    Rr = std::sqrt(rr2*rr2 + (L - rlength)*(L - rlength));

    // set negative value for converging cone
    r2_->SetVal(-Rl);
    // calculate area of sphere cap
    S = 2*PI*Rl*(Rl - L);
    S2_->SetVal(S);
  }
  // case left radius is equal to right one => we have a cylinder
  else if (rr1 == rr2)
  {
    // cone apex distance is about infinity
    r2_->SetVal(1E300);
    // area is just the area of the circle
    S = rr1*rr1*PI;
    S2_->SetVal(S);
  }
  // case right radius is greater than left one => diverging cone
  else
  {
    L = (rlength*rr2)/(rr2 - rr1);
    Rl = std::sqrt(rr1*rr1 + (L - rlength)*(L - rlength));
    Rr = std::sqrt(rr2*rr2 + L*L);

    // set positive value for diverging cone
    r2_->SetVal(Rl);
    // calculate area of sphere cap
    S = 2*PI*Rl*(Rl - (L - rlength));
    S2_->SetVal(S);
  }
}

double DWGconeJunctionModule::getB0(const string& method)
{
  double alpha, beta, gamma1, gamma2, S1, S2;
  double result = 0;

  double T = dynamic_cast<globalParameterType*>(FindProperty("T"))->GetParserVar().GetFloat();
  globalParameterType* c = dynamic_cast<globalParameterType*>(FindProperty("c"));

  if (c == NULL)
  {
    throw ARTerror("DWGconeJunctionModule::getB0",
        "No global parameter 'c' for the speed of sound has been found. Please add it to your current simulator!");
  }

  gamma1 = (c->GetParserVar().GetFloat())/(2*(r1_->GetFloat()));
  gamma2 = (c->GetParserVar().GetFloat())/(2*(r2_->GetFloat()));
  //	gamma1 = dynamic_cast<localParameterType*>(FindProperty("gamma1"))->GetFloat();
  //	gamma2 = dynamic_cast<localParameterType*>(FindProperty("gamma2"))->GetFloat();
  S1 = S1_->GetFloat();
  S2 = S2_->GetFloat();

  alpha = 2*(gamma2*S2 - gamma1*S1)/(S2 + S1);
  beta = 2/T;


  if (method == "IIM")
  {
    //		result = -1 + exp(alpha * T);
    result = -1 + exp(-alpha * T);
  }
  else if (method == "TICM")
  {
    result = -(1 + ((exp(-alpha*T) - 1)/(alpha*T)));
  }
  else if (method == "BT")
  {
    result = -alpha/(alpha + beta);
  }
  else
  {
    throw ARTerror("DWGconeJunctionModule::getB0", "Unknown calculation method '%s1'. Possible values are 'IIM', 'TICM' and 'BT'.",
        method);
  }
  return result;
}

double DWGconeJunctionModule::getB1(const string& method)
{
  double alpha, beta, gamma1, gamma2, S1, S2;
  double result = 0;

  double T = dynamic_cast<globalParameterType*>(FindProperty("T"))->GetParserVar().GetFloat();
  globalParameterType* c = dynamic_cast<globalParameterType*>(FindProperty("c"));

  if (c == NULL)
  {
    throw ARTerror("DWGconeJunctionModule::getB1",
        "No global parameter 'c' for the speed of sound has been found. Please add it to your current simulator!");
  }

  gamma1 = (c->GetParserVar().GetFloat())/(2*(r1_->GetFloat()));
  gamma2 = (c->GetParserVar().GetFloat())/(2*(r2_->GetFloat()));
  //	gamma1 = dynamic_cast<localParameterType*>(FindProperty("gamma1"))->GetFloat();
  //	gamma2 = dynamic_cast<localParameterType*>(FindProperty("gamma2"))->GetFloat();
  S1 = S1_->GetFloat();
  S2 = S2_->GetFloat();

  alpha = 2*(gamma2*S2 - gamma1*S1)/(S2 + S1);
  beta = 2/T;

  if (method == "IIM")
  {
    result = 0;
  }
  else if (method == "TICM")
  {
    result = (exp(-alpha*T) + ((exp(-alpha*T) - 1)/(alpha*T)));
  }
  else if (method == "BT")
  {
    result = -alpha/(alpha + beta);
  }
  else
  {
    throw ARTerror("DWGconeJunctionModule::getB1", "Unknown calculation method '%s1'. Possible values are 'IIM', 'TICM' and 'BT'.",
        method);
  }
  return result;

}

double DWGconeJunctionModule::getA1(const string& method)
{
  double alpha, beta, gamma1, gamma2, S1, S2;
  double result = 0;

  double T = dynamic_cast<globalParameterType*>(FindProperty("T"))->GetParserVar().GetFloat();
  globalParameterType* c = dynamic_cast<globalParameterType*>(FindProperty("c"));

  if (c == NULL)
  {
    throw ARTerror("DWGconeJunctionModule::getA1",
        "No global parameter 'c' for the speed of sound has been found. Please add it to your current simulator!");
  }

  gamma1 = (c->GetParserVar().GetFloat())/(2*(r1_->GetFloat()));
  gamma2 = (c->GetParserVar().GetFloat())/(2*(r2_->GetFloat()));
  //	gamma1 = dynamic_cast<localParameterType*>(FindProperty("gamma1"))->GetFloat();
  //	gamma2 = dynamic_cast<localParameterType*>(FindProperty("gamma2"))->GetFloat();
  S1 = S1_->GetFloat();
  S2 = S2_->GetFloat();

  alpha = 2*(gamma2*S2 - gamma1*S1)/(S2 + S1);
  beta = 2/T;

  if (method == "IIM")
  {
    //		result = exp(-alpha * T);
    result = -exp(-alpha * T);
  }
  else if (method == "TICM")
  {
    result = -exp(-alpha * T);
  }
  else if (method == "BT")
  {
    result = (alpha - beta)/(alpha + beta);
  }
  else
  {
    throw ARTerror("DWGconeJunctionModule::getA1", "Unknown calculation method '%s1'. Possible values are 'IIM', 'TICM' and 'BT'.",
        method);
  }
  return result;
}
