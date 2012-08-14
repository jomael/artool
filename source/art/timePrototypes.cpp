/*
 * timePrototypes.cpp
 *
 *  Created on: Aug 12, 2012
 *      Author: cbg
 */

#include "timePrototypes.h"

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
