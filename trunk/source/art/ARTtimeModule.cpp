/*
 * ARTtimeModule.cpp
 *
 *  Created on: Jul 5, 2012
 *      Author: cbg
 */


//#include "mpVariable.h"
#include "ARTtimeModule.h"

using namespace mup;

void ARTPortType::initPortValue(const string& expr) const
{
	if (_port != NULL)
	{
		ParserX* tmpParser = new ParserX(mup::pckCOMPLEX_NO_STRING);
		try
		{
			ARTdataContainer& port = const_cast<ARTdataContainer&>(*_port);
			tmpParser->DefineVar(port.GetVarName(), port.GetParserVar());
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
	else
	{
		throw ARTerror("ARTPortType::initPortValue", "No associated data container found for current port.");
	}
}

void ARTPortType::initPortValue(double value, int idx) const
{
	if (_port != NULL)
	{
		ARTdataContainer& port = const_cast<ARTdataContainer&>(*_port);
		port[idx] = value;
	}
	else
	{
		throw ARTerror("ARTPortType::initPortValue", "No associated data container found for current port.");
	}
}

void ARTPortType::initPortValue(std::complex<double>& value, int idx) const
{
	if (_port != NULL)
	{
		ARTdataContainer& port = const_cast<ARTdataContainer&>(*_port);
		port[idx] = value;
	}
	else
	{
		throw ARTerror("ARTPortType::initPortValue", "No associated data container found for current port.");
	}
}

std::complex<double> ARTOPortType::operator[](int idx) const
{

	if (_port != NULL)
	{
		ARTdataContainer& port = const_cast<ARTdataContainer&>(*_port);
		if (*_simulator)
		{
			(*_simulator)->SetModulesToCurrentTimeIndex(idx);
		}
		else
		{
			throw ARTerror("ARTPortType::initPortValue", "No simulator set for current port.");
		}

		return port.GetArrayElement(idx).GetComplex();
	}
	else
	{
		throw ARTerror("ARTPortType::operator[]", "No associated data container found for current port.");
	}
}

void ARTItimeModule::setSimulator(ARTsimulator* sim)
{
	_simulator = dynamic_cast<ARTtimeSimulator*>(sim);
}

ARTtimeModule::ARTtimeModule(const string& name, const string& sds, const string& lds, const string& htm) :
	ARTItimeModule(name, sds, lds, htm),
	_iPorts(),
	_oPorts(),
	_lParams(),
	_gParams()
{

}

//ARTtimeModule::ARTtimeModule(const ARTtimeModule& orig) :
//	ARTItimeModule(orig),
//	_iPorts(),
//	_oPorts(),
//	_lParams(),
//	_gParams()
//{
//	copy(orig);
//}

void ARTtimeModule::addIPort(const string& name, const ARTPortType& port)
{
	// the given name of the input port has to be unique
	if (!checkVarNameExists(name))
	{
		ARTIPortType* newIPort = new ARTIPortType();
		const ARTdataContainer* tmpContainer = getContainerFromPort(port);
		setContainerForPort(*newIPort, tmpContainer);
		setSimulatorForPort(*newIPort, _simulator);
		_iPorts[name] = newIPort;

		addVariableToParsers(name, tmpContainer->GetParserVar());
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
	if (!checkVarNameExists(name))
	{
		ARTOPortType* newOPort = new ARTOPortType();
		// create new ARTdataContainer with 20 elements array size
		ARTdataContainer* tmpContainer = new ARTdataContainer(C_ART_na, size, name);

		// create new parser for output port
		ParserX* newParser = new ParserX(mup::pckCOMPLEX_NO_STRING);
		// setting parser of new output port
		tmpContainer->SetParser(newParser);
		// set definition of new output port
		tmpContainer->SetDefinition(expr);
		// register all variables of module to parser
		registerAllVariablesToParser(newParser);

		// save parser for port
		setParserForOPort(*newOPort, newParser);
		// save data container for port
		setContainerForPort(*newOPort, tmpContainer);
		// save simulator of current module to output port
		setSimulatorForPort(*newOPort, _simulator);

		// register new output port
		_oPorts[name] = newOPort;

		// add variable to all available parsers
		addVariableToParsers(name, tmpContainer->GetParserVar());

	}
	else
	{
		throw ARTerror("ARTtimeModule::addOPort", "Port name '%s1' is already in use in current time module '%s2'.",
				name, name_);
	}
}

const ARTPortType& ARTtimeModule::getPort(const string& name)
{
	if (_iPorts.find(name) != _iPorts.end())
	{
		return *(_iPorts[name]);
	}

	if (_oPorts.find(name) != _oPorts.end())
	{
		return *(_oPorts[name]);
	}

	throw ARTerror("ARTtimeModule::getPort", "No port with name '%s1' has been found in current time module '%s2'.",
			name, name_);

}

//ARTdataContainer& ARTtimeModule::getOPort(const string& name)
//{
//	if (_oPorts.find(name) != _oPorts.end())
//	{
//		return *(_oPorts[name]->oPort);
//	}
//
//	// TODO: use a feasible exception type
//	throw string("Output port name has not been found");
//}

void ARTtimeModule::setLocalParameter(const string& name, const string& expr)
{

	if (_lParams.find(name) != _lParams.end())
	{
		ParserX tmpParser(mup::pckCOMPLEX_NO_STRING);

		LocalParameterType* localParam = _lParams[name];

		tmpParser.DefineVar(name, *(localParam->var));
		tmpParser.SetExpr(expr);
		try
		{
			*(localParam->val) = tmpParser.Eval();
		}
		catch (ParserError& error)
		{
			// pass error on to next instance
			throw ARTerror("ARTtimeModule::setLocalParameter", "Could not set local parameter '%s1' of time module '%s2': %s3",
					name, name_, error.GetMsg());
		}
	}
	else
	{
		throw ARTerror("ARTtimeModule::setLocalParameter", "Name '%s1' of local parameter is already in use in current time module '%s2'.",
				name, name_);
	}


}

void ARTtimeModule::setLocalParameter(const string& name, const double val)
{
	if (_lParams.find(name) != _lParams.end())
	{
		LocalParameterType* localParam = _lParams[name];
		*(localParam->val) = val;
	}
	else
	{
		throw ARTerror("ARTtimeModule::setLocalParameter", "Local parameter '%s1' could not be found in current time module '%s2'.",
				name, name_);
	}
}

void ARTtimeModule::setLocalParameter(const string& name, const std::complex<double>& val)
{
	if (_lParams.find(name) != _lParams.end())
	{
		LocalParameterType* localParam = _lParams[name];
		*(localParam->val) = val;
	}
	else
	{
		throw ARTerror("ARTtimeModule::setLocalParameter", "Local parameter '%s1' could not be found in current time module '%s2'.",
				name, name_);
	}

}

void ARTtimeModule::addLocalParameter(const string& name, const string& expr)
{

	if (!checkVarNameExists(name))
	{
		ParserX tmpParser(mup::pckCOMPLEX_NO_STRING);
		LocalParameterType* newParam = new LocalParameterType();
		newParam->val = new Value();
		newParam->var = new Variable(newParam->val);

		tmpParser.DefineVar(name, *(newParam->var));
		tmpParser.SetExpr(expr);
		try
		{
			*(newParam->val) = tmpParser.Eval();
		}
		catch (ParserError& error)
		{
			// delete already allocated objects
			delete (newParam->val);
			delete (newParam->var);
			// pass error on to next instance
			throw ARTerror("ARTtimeModule::setLocalParameter", "Could not set local parameter '%s1' of time module '%s2': %s3",
					name, name_, error.GetMsg());
		}

		_lParams[name] = newParam;
		addVariableToParsers(name, *(newParam->var));
	}
	else
	{
		throw ARTerror("ARTtimeModule::addLocalParameter", "Name '%s1' of local parameter is already in use in current time module '%s2'.",
				name, name_);
	}

}

void ARTtimeModule::addLocalParameter(const string& name, const double val)
{
	if (!checkVarNameExists(name))
	{
		LocalParameterType* newParam = new LocalParameterType();
		newParam->val = new Value(val);
		newParam->var = new Variable(newParam->val);
		_lParams[name] = newParam;
		addVariableToParsers(name, *(newParam->var));
	}
	else
	{
		throw ARTerror("ARTtimeModule::setLocalParameter", "Name '%s1' of local parameter is already in use in current time module '%s2'.",
			name, name_);
	}
}

void ARTtimeModule::addLocalParameter(const string& name, const std::complex<double>& val)
{
	if (!checkVarNameExists(name))
	{
		LocalParameterType* newParam = new LocalParameterType();
		newParam->val = new Value(val);
		newParam->var = new Variable(newParam->val);
		_lParams[name] = newParam;
		addVariableToParsers(name, *(newParam->var));
	}
	else
	{
		throw ARTerror("ARTtimeModule::setLocalParameter", "Name '%s1' of local parameter is already in use in current time module '%s2'.",
			name, name_);
	}
}

void ARTtimeModule::addGlobalParameter(const string& name, const Variable& parameter)
{
	if (!checkVarNameExists(name))
	{
		_gParams[name] = &parameter;
		addVariableToParsers(name, parameter);
	}
	else
	{
		throw ARTerror("ARTtimeModule::addGlobalParameter", "Name '%s1' of global parameter is already in use in current time module '%s2'.",
				name, name_);
	}
}

void ARTtimeModule::removeGlobalParameter(const string& name)
{
	if (_gParams.find(name) != _gParams.end())
	{
		_gParams.erase(name);
		removeVariableFromParsers(name);
	}
	else
	{
		throw ARTerror("ARTtimeModule::removeGlobalParameter", "Name '%s1' of global parameter could not be found in current time module '%s2'.",
				name, name_);
	}
}

void ARTtimeModule::setCurrentIndex(int idx)
{
	oPortIterator iter;
	ARTdataContainer* tmpContainer;
	for (iter = _oPorts.begin(); iter != _oPorts.end(); ++iter)
	{
		tmpContainer = getContainerFromPort(*(iter->second));
		tmpContainer->SetCurrentIndex(idx);
		tmpContainer->GetArrayElement(idx).Invalidate();
	}
}

//ARTtimeModule& ARTtimeModule::operator=(const ARTtimeModule& orig)
//{
//	clean();
//	_simulator = orig._simulator;
//	copy(orig);
//	return *this;
//}

ARTtimeModule::~ARTtimeModule()
{
	clean();
}


bool ARTtimeModule::checkVarNameExists(const string& name)
{
	if (_oPorts.find(name) != _oPorts.end())
	{
		return true;
	}
	else if (_iPorts.find(name) != _iPorts.end())
	{
		return true;
	}
	else if (_lParams.find(name) != _lParams.end())
	{
		return true;
	}
	else if (_gParams.find(name) != _gParams.end())
	{
		return true;
	}
	return false;

}

void ARTtimeModule::addVariableToParsers(const string& name, const Variable& var)
{

	// add the given variable to all currently registered parsers
	oPortIterator iter;
	for (iter = _oPorts.begin(); iter != _oPorts.end(); ++iter)
	{
		// only add new variable name to parsers of other output ports
		if (iter->first != name)
		{
			getParserFromOPort(*(iter->second))->DefineVar(name, var);
		}
	}

}

void ARTtimeModule::removeVariableFromParsers(const string& name)
{
	// go through all parsers of output port and remove name declaration
	oPortIterator iter;
	for (iter = _oPorts.begin(); iter != _oPorts.end(); ++iter)
	{
		getParserFromOPort(*(iter->second))->RemoveVar(name);
	}
}

void ARTtimeModule::registerAllVariablesToParser(ParserX* parser)
{
	oPortIterator oIter;
	iPortIterator iIter;
	globalParameterIterator gPIter;
	localParameterIterator lPIter;
	const Variable* var;

	for (oIter = _oPorts.begin(); oIter != _oPorts.end(); ++oIter)
	{
		var = &(getContainerFromPort(*(oIter->second))->GetParserVar());
		parser->DefineVar(oIter->first, *var);
	}

	for (iIter = _iPorts.begin(); iIter != _iPorts.end(); ++iIter)
	{
		var = &(getContainerFromPort(*(iIter->second))->GetParserVar());
		parser->DefineVar(iIter->first, *var);
	}

	for (gPIter = _gParams.begin(); gPIter != _gParams.end(); ++gPIter)
	{
		var = gPIter->second;
		parser->DefineVar(gPIter->first, *var);
	}

	for (lPIter = _lParams.begin(); lPIter != _lParams.end(); ++lPIter)
	{
		var = lPIter->second->var;
		parser->DefineVar(lPIter->first, *var);
	}
}

void ARTtimeModule::clean()
{

	oPortIterator oIter;
	iPortIterator iIter;
	localParameterIterator lPIter;
	// delete all elements from the output port map
	for (oIter = _oPorts.begin(); oIter != _oPorts.end(); ++oIter)
	{
		delete (getContainerFromPort(*(oIter->second)));
		delete (getParserFromOPort(*(oIter->second)));
		//delete (oIter->second->portVariable);
		delete (oIter->second);
	}

	_oPorts.clear();

	// delete all elements from the input port map
	for (iIter = _iPorts.begin(); iIter != _iPorts.end(); ++iIter)
	{
		// we only have to delete the variable as the port will be deleted
		// by the corresponding other time module
		//delete (iIter->second->portVariable);
		delete (iIter->second);
	}

	_iPorts.clear();

	// delete all elements from the parameter map
	for (lPIter = _lParams.begin(); lPIter != _lParams.end(); ++lPIter)
	{
		delete (lPIter->second->val);
		delete (lPIter->second->var);

		delete (lPIter->second);
	}

	_lParams.clear();

	// just clear the map for global parameters
	_gParams.clear();

}

//void ARTtimeModule::copy(const ARTtimeModule& orig)
//{
//	/* TODO: implement copy function */
//}




