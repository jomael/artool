/*
 * ARTtimeModule.cpp
 *
 *  Created on: Jul 5, 2012
 *      Author: cbg
 */


//#include "mpVariable.h"
#include "ARTtimeModule.h"

using namespace mup;

void ARTPortType::initPortValue(std::complex<double> value, int idx) const
{
	if (_port != NULL)
	{
		ARTdataContainer& port = const_cast<ARTdataContainer&>(*_port);
//		std::cout << "initPortValue of container at address " << &port << std::endl;
		port[idx] = value;
	}
	else
	{
		// TODO: use a feasible exception type
		throw string("No associated data container found");
	}
}

std::complex<double> ARTOPortType::operator[](int idx) const
{

	if (_port != NULL)
	{
		ARTdataContainer& port = const_cast<ARTdataContainer&>(*_port);
		port.SetCurrentIndex(idx);
		port.GetArrayElement(idx).Invalidate();

//		std::cout << "index operator of container at address " << &port << std::endl;
//		std::cerr << "OPort::Operator[](" << idx << ") = " << port.GetArrayElement(idx).GetComplex().imag() <<
//				";" << std::endl;

		return port.GetArrayElement(idx).GetComplex();
	}
	else
	{
		// TODO: use a feasible exception type
		throw string("No associated data container found");
	}
}

ARTtimeModule::ARTtimeModule(const string& name, const string& sds, const string& lds, const string& htm) :
	ARTItimeModule(name, sds, lds, htm),
	_iPorts(),
	_oPorts()
{

}

ARTtimeModule::ARTtimeModule(const ARTtimeModule& orig) :
	ARTItimeModule(orig),
	_iPorts(),
	_oPorts()
{
	copy(orig);
}

void ARTtimeModule::addIPort(const string& name, const ARTPortType& port)
{
	// the given name of the input port has to be unique
	if (!checkVarNameExists(name))
	{
		ARTIPortType* newIPort = new ARTIPortType();
		const ARTdataContainer* tmpContainer = getContainerFromPort(port);
		setContainerForPort(*newIPort, tmpContainer);
		//newIPort->_port = port._port;
		//newIPort->portVariable = new Variable(&port);
		_iPorts[name] = newIPort;

		addVariableToParsers(name, tmpContainer->GetParserVar());

		std::cout << "Address of new IPort \"" << name << "\": " << tmpContainer << std::endl;
		std::cout << "Address of variable: " << &(tmpContainer->GetParserVar()) << std::endl;

	}
	else
	{
		// TODO: use a feasible exception type
		throw string("Input port name already in use");
	}
}

void ARTtimeModule::addOPort(const string& name, const string& expr)
{
	// the name of the given output port has to be unique
	if (!checkVarNameExists(name))
	{
		ARTOPortType* newOPort = new ARTOPortType();
		// create new ARTdataContainer with 20 elements array size
		ARTdataContainer* tmpContainer = new ARTdataContainer(C_ART_na, 20, name);

		std::cout << "Address of new OPort \"" << name << "\": " << tmpContainer << std::endl;

		// create new parser for output port
		ParserX* newParser = new ParserX();
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

		// register new output port
		_oPorts[name] = newOPort;

		// add variable to all available parsers
		addVariableToParsers(name, tmpContainer->GetParserVar());
		std::cout << "Address of variable: " << &(tmpContainer->GetParserVar()) << std::endl;

	}
	else
	{
		// TODO: use a feasible exception type
		throw string("Output port name already in use");
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

	// TODO: use a feasible exception type
	throw string("Output port name has not been found");

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
	if (!checkVarNameExists(name))
	{
		ParserX tmpParser;

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
			throw;
		}

		_lParams[name] = newParam;
		addVariableToParsers(name, *(newParam->var));
	}
	else
	{
		// TODO: use a feasible exception type
		throw string("Name of local parameter already in use");
	}

}

void ARTtimeModule::setLocalParameter(const string& name, const double val)
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
		// TODO: use a feasible exception type
		throw string("Name of local parameter already in use");
	}
}

void ARTtimeModule::setLocalParameter(const string& name, const std::complex<double>& val)
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
		// TODO: use a feasible exception type
		throw string("Name of local parameter already in use");
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
		// TODO: use a feasible exception type
		throw string("Name of global parameter already in use");
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
		// TODO: use a feasible exception type
		throw string("Name of specified global parameter is currently not in use");
	}
}

ARTtimeModule& ARTtimeModule::operator=(const ARTtimeModule& orig)
{
	copy(orig);
	return *this;
}

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

void ARTtimeModule::copy(const ARTtimeModule& orig)
{
	/* TODO: implement copy function */
}




