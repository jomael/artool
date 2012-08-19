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
}

void fractionalDelayModule::addIPort(const string& name, const ARTdataProp* refPort)
{
	ParserX* tmpParser;
	const OPortType* oPort = dynamic_cast<const OPortType*>(refPort);
	if (!refPort)
	{
		throw ARTerror("fractionalDelayModule::addIPort", "Port '%s1' is no valid output port",
				refPort->GetName());
	}
	in_ = new IPortType(name, oPort);
	tmpParser = out_->GetParser();
	tmpParser->DefineVar(name, in_->GetParserVar());
}

void fractionalDelayModule::addGlobalParameter(const ARTdataProp* parameter)
{
	ParserX* tmpParser = out_->GetParser();
	if (!parameter)
	{
		throw ARTerror("fractionalDelayModule::addGlobalParameter", "Could not add global parameter to current time module '%s1': No valid parameter.",
				name_);
	}
	if (!FindProperty(parameter->GetName()))
	{
		globalParameterType* newParam =	new globalParameterType(parameter->GetName(), parameter);
		// register global variable to output port
		tmpParser->DefineVar(parameter->GetName(), parameter->GetParserVar());
		AppendDataProp(newParam);
	}
	else
	{
		throw ARTerror("fractionalDelayModule::addGlobalParameter", "Name '%s1' of global parameter is already in use in current time module '%s2'.",
				parameter->GetName(), name_);
	}

}

void fractionalDelayModule::removeGlobalParameter(const string& name)
{
	ParserX* tmpParser = out_->GetParser();
	tmpParser->RemoveVar(name);
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

void fractionalDelayModule::setSimulator(ARTsimulator* sim)
{
	_simulator = dynamic_cast<ARTtimeSimulator*>(sim);
	ARTproperty* iter = GetProperties(NULL);
	OPortType* oPort;
	localParameterType* lType;
	while (iter)
	{
		// set simulator for all registered output ports and use parser for local parameters
		oPort = dynamic_cast<OPortType*>(iter);
		lType = dynamic_cast<localParameterType*>(iter);
		if (oPort)
		{
			oPort->SetScope(_simulator);
		}
		else if (lType)
		{
			lType->SetParser(sim->GetParser());
		}
		iter = GetProperties(iter);
	}
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

fractionalDelayModule::~fractionalDelayModule()
{
	delete in_;
	delete out_;
}

void fractionalDelayModule::initLocalParams()
{
	localParameterType* tmpParam;

	// save standard type of filter
	tmpParam = new localParameterType("type");
	tmpParam->SetType(C_ART_str, 0);
	tmpParam->SetVal("lagrange");
	AppendDataProp(tmpParam);

	// save standard order of implemented filter
	tmpParam = new localParameterType("order");
	tmpParam->SetVal(5);
	AppendDataProp(tmpParam);

	// save standard delay of current module => currently 0 seconds
	tmpParam = new localParameterType("Delay");
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

	// init output port to zero
	for (n = 1; n <= N; ++n)
	{
		(*out_)[-n] = 0;
	}

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
