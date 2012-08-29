/*
 * timePrototypes.cpp
 *
 *  Created on: Aug 12, 2012
 *      Author: cbg
 */

#include <sstream>
#include "timePrototypes.h"

/*******************************************************************************************
 * inputFunctionModule
 *******************************************************************************************/

inputFunctionModule::inputFunctionModule(const string& name, const int len, const string& sds, const string& lds, const string& htm) :
	ARTItimeModule(name, sds, lds, htm),
	out_(NULL)
{
	if (len > 0)
	{
		out_ = new FPortType(len, "out", "Output port of InputFunctionModule");
	}
	else
	{
		throw ARTerror("inputFunctionModule::inputFunctionModule", "Error when creating new inputFunction module '%s1': invalid specified length.",
				name);
	}
}

ARTItimeModule* inputFunctionModule::Create(const string& name, const string& sds, const string& lds, const string& htm)
{
	return new inputFunctionModule(name, 20, sds, lds, htm);
}


void inputFunctionModule::addIPort(const string& name, const ARTdataProp* refPort)
{
	throw ARTerror("InputFunctionModule::addIPort", "Operation not permitted for time module '%s1'.",
			name_);
}


void inputFunctionModule::addGlobalParameter(const ARTdataProp* parameter)
{
	ParserX* tmpParser = out_->GetParser();
	tmpParser->DefineVar(parameter->GetName(), parameter->GetParserVar());
}

void inputFunctionModule::removeGlobalParameter(const string& name)
{
	ParserX* tmpParser = out_->GetParser();
	tmpParser->RemoveVar(name);
}

ARTdataProp* inputFunctionModule::getPort(const string& name)
{
	if (name != "out")
	{
		throw ARTerror("InputFunctionModule::getPort", "Time module '%s1' has no port '%s2'.",
				name_, name);
	}
	return out_;
}

inputFunctionModule::~inputFunctionModule()
{
	delete out_;
}


/*******************************************************************************************
 * fractionDelayModule
 *******************************************************************************************/

fractionalDelayModule::fractionalDelayModule(const string& name, const string& sds, const string& lds, const string& htm) :
		ARTItimeModule(name, sds, lds, htm),
		out_(NULL),
		in_(NULL)
{
	initLocalParams();
	out_ = new OPortType(C_ART_na, 20, "out");
	AppendDataProp(out_);
}

ARTItimeModule* fractionalDelayModule::Create(const string& name, const string& sds, const string& lds, const string& htm)
{
	return new fractionalDelayModule(name, sds, lds, htm);
}

void fractionalDelayModule::addIPort(const string& name, const ARTdataProp* refPort)
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

ARTdataProp* fractionalDelayModule::getPort(const string& name)
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

double fractionalDelayModule::getLagrangeParams(int n, int N, double D)
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

double fractionalDelayModule::getThiranParams(int n, int N, double D)
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

double fractionalDelayModule::fac(int n)
{
	double result = 1;
	int i;
	for (i = 1; i <= n; ++i)
	{
		result *= i;
	}
	return result;
}

double fractionalDelayModule::binom(int n, int k)
{
	double numerator = fac(n);
	double denominator = fac(k) * fac(n -k);
	return numerator / denominator;
}

/*******************************************************************************************
 * DWGcylinderModule
 *******************************************************************************************/

DWGcylinderModule::DWGcylinderModule(const string& name, const string& sds, const string& lds, const string& htm) :
		fractionalDelayModule(name, sds, lds, htm),
		p1p_(NULL),
		p2p_(NULL),
		p1m_(NULL),
		p2m_(NULL)
{
	initLocalParams();
	p2p_ = new OPortType(C_ART_na, 20, "p2p");
	AppendDataProp(p2p_);
	p1m_ = new OPortType(C_ART_na, 20, "p1m");
	AppendDataProp(p1m_);
}

ARTItimeModule* DWGcylinderModule::Create(const string& name, const string& sds, const string& lds, const string& htm)
{
	return new DWGcylinderModule(name, sds, lds, htm);
}

void DWGcylinderModule::addIPort(const string& name, const ARTdataProp* refPort)
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
		p2p_->GetParser()->DefineVar(name, p1p_->GetParserVar());
		AppendDataProp(p1p_);
	}
	else
	{
		p2m_ = new IPortType(name, oPort);
		p1m_->GetParser()->DefineVar(name, p2m_->GetParserVar());
		AppendDataProp(p2m_);
	}
}

ARTdataProp* DWGcylinderModule::getPort(const string& name)
{
	if (name != "p2p" && name != "p1m")
	{
		throw ARTerror("DWGcylinderModule::getPort", "Time module '%s1' has no port '%s2'.",
				name_, name);
	}
	else
	{
		return dynamic_cast<ARTdataProp*>(FindProperty(name));
	}
}

void DWGcylinderModule::setCurrentIndex(int idx)
{
	if (idx == 0)
	{
		initSimulation();
	}
	p2p_->SetCurrentIndex(idx);
	p1m_->SetCurrentIndex(idx);
}

void DWGcylinderModule::simulateCurrentIndex(int idx)
{
	p2p_->GetArrayElement(idx).EvaluateIfInvalid();
	p1m_->GetArrayElement(idx).EvaluateIfInvalid();
}

void DWGcylinderModule::initLocalParams()
{
	localParameterType* tmpParam;

	// save standard order of implemented filter
	tmpParam = new localParameterType("length", "length of the cylinder in mm");
	tmpParam->SetVal(50);
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
	// divide by 1000 as original length is given in mm
	D = (length->GetFloat()) / ((c->GetParserVar().GetFloat() * T->GetParserVar().GetFloat())) / 1000;

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

//	cout << "D = " << D << "\nExpression for FD: " << expr1.str() << endl;

	// save whole definitions to output ports
	p2p_->SetDefinition(expr1.str());
	p1m_->SetDefinition(expr2.str());
}

/*******************************************************************************************
 * impulseModule
 *******************************************************************************************/

impulseModule::impulseModule(const string& name, const string& sds, const string& lds, const string& htm) :
		ARTItimeModule(name, sds, lds, htm),
		out_(NULL)
{
	out_ = new OPortType(C_ART_na, 5, "out");
	out_->SetDefinition("out[t] = (t == 0) ? A : 0");
	AppendDataProp(out_);
	initLocalParams();
}

ARTItimeModule* impulseModule::Create(const string& name, const string& sds, const string& lds, const string& htm)
{
	return new impulseModule(name, sds, lds, htm);
}

void impulseModule::addIPort(const string& name, const ARTdataProp* refPort)
{
	throw ARTerror("impulseModule::addIPort", "Operation not permitted for time module '%s1'.",
			name_);
}

ARTdataProp* impulseModule::getPort(const string& name)
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
		ARTItimeModule(name, sds, lds, htm),
		out_(NULL)
{
	out_ = new OPortType(C_ART_na, 5, "out");
	out_->SetDefinition("out[t] = (t >= 0) ? A : 0");
	AppendDataProp(out_);
	initLocalParams();
}

ARTItimeModule* heavisideModule::Create(const string& name, const string& sds, const string& lds, const string& htm)
{
	return new heavisideModule(name, sds, lds, htm);
}

void heavisideModule::addIPort(const string& name, const ARTdataProp* refPort)
{
	throw ARTerror("heavisideModule::addIPort", "Operation not permitted for time module '%s1'.",
			name_);
}

ARTdataProp* heavisideModule::getPort(const string& name)
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

rectengularModule::rectengularModule(const string& name, const string& sds, const string& lds, const string& htm) :
		ARTItimeModule(name, sds, lds, htm),
		out_(NULL)
{
	out_ = new OPortType(C_ART_na, 5, "out");
	out_->SetDefinition("out[t] = (((t*T) >= S) and ((t*T) <= E)) ? A : 0");
	AppendDataProp(out_);
	initLocalParams();
}

ARTItimeModule* rectengularModule::Create(const string& name, const string& sds, const string& lds, const string& htm)
{
	return new rectengularModule(name, sds, lds, htm);
}

void rectengularModule::addIPort(const string& name, const ARTdataProp* refPort)
{
	throw ARTerror("rectengularModule::addIPort", "Operation not permitted for time module '%s1'.",
			name_);
}

ARTdataProp* rectengularModule::getPort(const string& name)
{
	if (name != "out")
	{
		throw ARTerror("rectengularModule::getPort", "Time module '%s1' has no port '%s2'.",
				name_, name);
	}
	return out_;
}

void rectengularModule::setCurrentIndex(int idx)
{
	out_->SetCurrentIndex(idx);
}

void rectengularModule::simulateCurrentIndex(int idx)
{
	out_->GetArrayElement(idx).EvaluateIfInvalid();
}

void rectengularModule::initLocalParams()
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
		ARTItimeModule(name, sds, lds, htm),
		out_(NULL)
{
	out_ = new OPortType(C_ART_na, 5, "out");
	out_->SetDefinition("out[t] = A*in[t]");
	AppendDataProp(out_);
	initLocalParams();
}

ARTItimeModule* amplificationModule::Create(const string& name, const string& sds, const string& lds, const string& htm)
{
	return new amplificationModule(name, sds, lds, htm);
}

void amplificationModule::addIPort(const string& name, const ARTdataProp* refPort)
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

ARTdataProp* amplificationModule::getPort(const string& name)
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
		ARTItimeModule(name, sds, lds, htm),
		out_(NULL)
{
	out_ = new OPortType(C_ART_na, 5, "out");
	out_->SetDefinition("out[t] = in[t - ((round)(Delay/T))]");
	AppendDataProp(out_);
	initLocalParams();
}

ARTItimeModule* simpleDelayModule::Create(const string& name, const string& sds, const string& lds, const string& htm)
{
	return new simpleDelayModule(name, sds, lds, htm);
}

void simpleDelayModule::addIPort(const string& name, const ARTdataProp* refPort)
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

ARTdataProp* simpleDelayModule::getPort(const string& name)
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
		ARTItimeModule(name, sds, lds, htm),
		out_(NULL)
{
	out_ = new OPortType(C_ART_na, 5, "out");
	out_->SetDefinition("out[t] = in1[t] + in2[t]");
	AppendDataProp(out_);
}

ARTItimeModule* addModule::Create(const string& name, const string& sds, const string& lds, const string& htm)
{
	return new addModule(name, sds, lds, htm);
}

void addModule::addIPort(const string& name, const ARTdataProp* refPort)
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

ARTdataProp* addModule::getPort(const string& name)
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
	out_->GetArrayElement(idx).EvaluateIfInvalid();
}

/*******************************************************************************************
 * multiplicationModule
 *******************************************************************************************/

multiplicationModule::multiplicationModule(const string& name, const string& sds, const string& lds, const string& htm) :
		ARTItimeModule(name, sds, lds, htm),
		out_(NULL)
{
	out_ = new OPortType(C_ART_na, 5, "out");
	out_->SetDefinition("out[t] = in1[t] * in2[t]");
	AppendDataProp(out_);
}

ARTItimeModule* multiplicationModule::Create(const string& name, const string& sds, const string& lds, const string& htm)
{
	return new multiplicationModule(name, sds, lds, htm);
}

void multiplicationModule::addIPort(const string& name, const ARTdataProp* refPort)
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

ARTdataProp* multiplicationModule::getPort(const string& name)
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
	out_->GetArrayElement(idx).EvaluateIfInvalid();
}


/*******************************************************************************************
 * sinewaveModule
 *******************************************************************************************/

sinewaveModule::sinewaveModule(const string& name, const string& sds, const string& lds, const string& htm) :
		ARTItimeModule(name, sds, lds, htm),
		out_(NULL)
{
	out_ = new OPortType(C_ART_na, 5, "out");
	out_->SetDefinition("out[t] = A*sin(2*pi*(t*T*f - Delta))");
	AppendDataProp(out_);
	initLocalParams();
}

ARTItimeModule* sinewaveModule::Create(const string& name, const string& sds, const string& lds, const string& htm)
{
	return new sinewaveModule(name, sds, lds, htm);
}

void sinewaveModule::addIPort(const string& name, const ARTdataProp* refPort)
{
	throw ARTerror("sinewaveModule::addIPort", "Operation not permitted for time module '%s1'.",
			name_);
}

ARTdataProp* sinewaveModule::getPort(const string& name)
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
