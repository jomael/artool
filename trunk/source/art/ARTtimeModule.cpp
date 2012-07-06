/*
 * ARTtimeModule.cpp
 *
 *  Created on: Jul 5, 2012
 *      Author: cbg
 */


//#include "mpVariable.h"
#include "ARTtimeModule.h"

using namespace mup;

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

void ARTtimeModule::addIPort(const string& name, const ARTdataContainer& port)
{
	// the given name of the input port has to be unique
	if (_oPorts.find(name) == _oPorts.end() && _iPorts.find(name) == _iPorts.end())
	{
		IPortType* newIPort = new IPortType();
		newIPort->iPort = &port;
		//newIPort->portVariable = new Variable(&port);
		_iPorts[name] = newIPort;

		addVariableToParsers(name, port.GetParserVar());

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
	if (_oPorts.find(name) == _oPorts.end() && _iPorts.find(name) == _iPorts.end())
	{
		OPortType* newOPort = new OPortType();
		// create new ARTdataContainer with 20 elements array size
		newOPort->oPort = new ARTdataContainer(C_ART_na, 20, name);
		newOPort->Parser = new ParserX();
		// setting parser of new output port
		newOPort->oPort->SetParser(newOPort->Parser);
		// set definition of new output port
		newOPort->oPort->SetDefinition(expr);

		// register new output port
		_oPorts[name] = newOPort;

		// add variable to all available parsers
		addVariableToParsers(name, newOPort->oPort->GetParserVar());

	}
	else
	{
		// TODO: use a feasible exception type
		throw string("Output port name already in use");
	}
}

const ARTdataContainer& ARTtimeModule::getPort(const string& name)
{
	if (_iPorts.find(name) != _iPorts.end())
	{
		return *(_iPorts[name]->iPort);
	}

	if (_oPorts.find(name) != _oPorts.end())
	{
		return *(_oPorts[name]->oPort);
	}

	// TODO: use a feasible exception type
	throw string("Output port name has not been found");

}

ARTdataContainer& ARTtimeModule::getOPort(const string& name)
{
	if (_oPorts.find(name) != _oPorts.end())
	{
		return *(_oPorts[name]->oPort);
	}

	// TODO: use a feasible exception type
	throw string("Output port name has not been found");
}

void ARTtimeModule::setLocalParameter(const string& name, const string& expr)
{

}

// TODO: implement same function with std::cmplx
void ARTtimeModule::setLocalParameter(const string& name, const double val)
{

}

void ARTtimeModule::addGlobalParameter(const string& name, const IValue& parameter)
{

}

void ARTtimeModule::removeGlobalParameter(const string& name)
{

}

ARTtimeModule& ARTtimeModule::operator=(const ARTtimeModule& orig)
{

}

ARTtimeModule::~ARTtimeModule()
{
	clean();
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
			iter->second->Parser->DefineVar(name, var);
		}
	}

}

void ARTtimeModule::removeVariableFromParsers(const string& name)
{
	// go through all parsers of output port and remove name declaration
	oPortIterator iter;
	for (iter = _oPorts.begin(); iter != _oPorts.end(); ++iter)
	{
		iter->second->Parser->RemoveVar(name);
	}
}


void ARTtimeModule::clean()
{

	oPortIterator oIter;
	iPortIterator iIter;
	// delete all elements from the output port map
	for (oIter = _oPorts.begin(); oIter != _oPorts.end(); ++oIter)
	{
		delete (oIter->second->oPort);
		delete (oIter->second->Parser);
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

}

void ARTtimeModule::copy(const ARTtimeModule& orig)
{
	/* TODO: implement copy function */
}




