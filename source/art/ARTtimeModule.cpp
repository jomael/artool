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
		int_type t = (int) (inArg[2]->GetFloat());

		ARTdataContainer* tmpContainer;
		ARTItimeModule::PortType* portA = NULL;
		ARTItimeModule::PortType* portB = NULL;

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

		tmpContainer = dynamic_cast<ARTdataContainer*>(a[lowerBound]);

		// get parent container / port of first argument
		if (tmpContainer)
		{
			portA = dynamic_cast<ARTItimeModule::PortType*>(tmpContainer->GetParent());
		}

		if (!portA)
		{
			throw ParserError("The first argument of function 'conv' is no valid port type!");
		}

		tmpContainer = dynamic_cast<ARTdataContainer*>(b[lowerBound]);

		// get parent container / port of first argument
		if (tmpContainer)
		{
			portB = dynamic_cast<ARTItimeModule::PortType*>(tmpContainer->GetParent());
		}

		if (!portB)
		{
			throw ParserError("The second argument of function 'conv' is no valid port type!");
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
			float_type z1_real = (*portA)[k].GetFloat();
			float_type z1_imag = (*portA)[k].GetImag();
			float_type z2_real = (*portB)[t-k].GetFloat();
			float_type z2_imag = (*portB)[t-k].GetImag();

			result.real() += (z1_real*z2_real - z1_imag*z2_imag);
			result.imag() += (z1_real*z2_imag + z1_imag*z2_real);

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

//void ARTPortType::initPortValue(const string& expr) const
//{
//	if (_port != NULL)
//	{
//		ParserX* tmpParser = new ParserX(mup::pckCOMPLEX_NO_STRING);
//		try
//		{
//			ARTdataContainer& port = const_cast<ARTdataContainer&>(*_port);
//			tmpParser->DefineVar(port.GetVarName(), port.GetParserVar());
//			tmpParser->SetExpr(expr);
//			tmpParser->Eval();
//			delete tmpParser;
//		}
//		catch (ParserError& error)
//		{
//			delete tmpParser;
//			throw ARTerror("ARTPortType::initPortValue", "Error in evaluation of parser expression: %s1",
//					error.GetMsg());
//		}
//	}
//	else
//	{
//		throw ARTerror("ARTPortType::initPortValue", "No associated data container found for current port.");
//	}
//}
//
//void ARTPortType::initPortValue(double value, int idx) const
//{
//	if (_port != NULL)
//	{
//		ARTdataContainer& port = const_cast<ARTdataContainer&>(*_port);
//		port[idx] = value;
//	}
//	else
//	{
//		throw ARTerror("ARTPortType::initPortValue", "No associated data container found for current port.");
//	}
//}
//
//void ARTPortType::initPortValue(std::complex<double>& value, int idx) const
//{
//	if (_port != NULL)
//	{
//		ARTdataContainer& port = const_cast<ARTdataContainer&>(*_port);
//		port[idx] = value;
//	}
//	else
//	{
//		throw ARTerror("ARTPortType::initPortValue", "No associated data container found for current port.");
//	}
//}
//
//std::complex<double> ARTOPortType::operator[](int idx) const
//{
//
//	if (_port != NULL)
//	{
//		ARTdataContainer& port = const_cast<ARTdataContainer&>(*_port);
//		if (*_simulator)
//		{
//			(*_simulator)->SetModulesToCurrentTimeIndex(idx);
//		}
//		else
//		{
//			throw ARTerror("ARTPortType::initPortValue", "No simulator set for current port.");
//		}
//
//		return port.GetArrayElement(idx).GetComplex();
//	}
//	else
//	{
//		throw ARTerror("ARTPortType::operator[]", "No associated data container found for current port.");
//	}
//}

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


ARTItimeModule::OPortType::OPortType(const T_ART_Type dtyp,
		const int dlen,
		const string name,
		const string sds,
		const string lds,
		const string htm)
: PortType(dtyp, dlen, name, sds, lds, htm),
  tVal_(0),
  tVar_(new Variable(&tVal_))
{
	if (parser_)
	{
		parser_->DefineVar("t", *tVar_);
	}
	else
	{
		throw ARTerror("ARTItimeModule::OPortType::OPortType", "No valid parser has been created. Please check source code of ARTdataContainer!");
	}
}

void ARTItimeModule::OPortType::initPortValue(const string& expr) const
{
	ParserX* tmpParser = new ParserX(mup::pckCOMPLEX_NO_STRING);
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

IValue& ARTItimeModule::OPortType::operator[](::size_t idx)
{
//	cout << "ARTItimeModule::OPortType::operator[size_t " << idx << "]" << endl;
	ARTdataContainer& tmpContainer = GetArrayElement(idx);
	if (!tmpContainer.IsValid())
	{
//		cout << idx << " INVALID!" << endl;
		tVal_ = (int) idx;
	}
	else
	{
//		cout << idx << " VALID!" << endl;
	}

	return ARTdataContainer::operator[](idx);
}

IValue& ARTItimeModule::OPortType::operator[](int idx)
{
//	cout << "ARTItimeModule::OPortType::operator[int " << idx << "]" << endl;
	ARTdataContainer& tmpContainer = GetArrayElement(idx);
	if (!tmpContainer.IsValid())
	{
//		cout << idx << " INVALID!" << endl;
		tVal_ = idx;
	}
	else
	{
//		cout << idx << " VALID!" << endl;
	}

	return ARTdataContainer::operator[](idx);
}

IValue& ARTItimeModule::OPortType::GetPortValue(::size_t idx)
{
	ARTtimeSimulator* simulator = dynamic_cast<ARTtimeSimulator*>(scope_);
	if (simulator)
	{
		simulator->SetModulesToCurrentTimeIndex(idx);
	}
	else
	{
		throw ARTerror("ARTPortType::initPortValue", "No valid time simulator set for current port '%s1'.", name_);
	}

	ARTdataContainer& tmpContainer = GetArrayElement(idx);

	if (tVal_.GetFloat() != (float_type) idx)
	{
		tVal_ = (int) idx;
		tmpContainer.Invalidate();
		SetCurrentIndex(idx);
	}

	return tmpContainer;
}

IValue& ARTItimeModule::OPortType::GetPortValue(int idx)
{
	ARTtimeSimulator* simulator = dynamic_cast<ARTtimeSimulator*>(scope_);
	if (simulator)
	{
		simulator->SetModulesToCurrentTimeIndex(idx);
	}
	else
	{
		throw ARTerror("ARTPortType::initPortValue", "No valid time simulator set for current port '%s1'.", name_);
	}

//	cout << "ARTItimeModule::OPortType::GetPortValue(" << idx << ")" << endl;
	ARTdataContainer& tmpContainer = GetArrayElement(idx);

	if (tVal_.GetFloat() != (float_type) idx)
	{
		tVal_ = (int) idx;
		tmpContainer.Invalidate();
		SetCurrentIndex(idx);
	}

	return tmpContainer;
}

ARTItimeModule::OPortType::~OPortType()
{
	delete tVar_;
}

ARTItimeModule::IPortType::IPortType(const string& name, const OPortType* param)
	: PortType(C_ART_str, 0, name), refPort_(param)
{

}

const Variable& ARTItimeModule::IPortType::GetParserVar()
{
	return refPort_->GetParserVar();
}

const Variable& ARTItimeModule::IPortType::GetParserVar() const
{
	return refPort_->GetParserVar();
}

void ARTItimeModule::setSimulator(ARTsimulator* sim)
{
	_simulator = dynamic_cast<ARTtimeSimulator*>(sim);
}

ARTtimeModule::ARTtimeModule(const string& name, const string& sds, const string& lds, const string& htm) :
	ARTItimeModule(name, sds, lds, htm)
{

}

//void ARTtimeModule::addIPort(const string& name, const ARTPortType& port)
//{
//	// the given name of the input port has to be unique
//	if (!checkVarNameExists(name))
//	{
//		ARTIPortType* newIPort = new ARTIPortType();
//		const ARTdataContainer* tmpContainer = getContainerFromPort(port);
//		setContainerForPort(*newIPort, tmpContainer);
//		setSimulatorForPort(*newIPort, _simulator);
//		_iPorts[name] = newIPort;
//
//		addVariableToParsers(name, tmpContainer->GetParserVar());
//	}
//	else
//	{
//		throw ARTerror("ARTtimeModule::addIPort", "Port name '%s1' is already in use in current time module '%s2'.",
//				name, name_);
//	}
//}

void ARTtimeModule::addIPort(const string& name, const ARTdataProp* refPort)
{
	// the given name of the input port has to be unique
	if (!FindProperty(name))
	{
		const OPortType* oPort = dynamic_cast<const OPortType*>(refPort);
		if (!oPort)
		{
			throw ARTerror("ARTtimeModule::addIPort", "Port '%s1' is no valid output port.",
					refPort->GetName());
		}
		IPortType* newIPort = new IPortType(name, oPort);
		addVariableToParsers(name, newIPort->GetParserVar());
		AppendDataProp(newIPort);
	}
	else
	{
		throw ARTerror("ARTtimeModule::addIPort", "Port name '%s1' is already in use in current time module '%s2'.",
				name, name_);
	}
}

//void ARTtimeModule::addOPort(const string& name, const string& expr, unsigned int size)
//{
//	// the name of the given output port has to be unique
//	if (!checkVarNameExists(name))
//	{
//		ARTOPortType* newOPort = new ARTOPortType();
//		// create new ARTdataContainer with 20 elements array size
//		ARTdataContainer* tmpContainer = new ARTdataContainer(C_ART_na, size, name);
//
//		// create new parser for output port
//		ParserX* newParser = new ParserX(mup::pckCOMPLEX_NO_STRING);
//		// setting parser of new output port
//		tmpContainer->SetParser(newParser);
//		// set definition of new output port
//		tmpContainer->SetDefinition(expr);
//		// register all variables of module to parser
//		registerAllVariablesToParser(newParser);
//
//		// save parser for port
//		setParserForOPort(*newOPort, newParser);
//		// save data container for port
//		setContainerForPort(*newOPort, tmpContainer);
//		// save simulator of current module to output port
//		setSimulatorForPort(*newOPort, _simulator);
//
//		// register new output port
//		_oPorts[name] = newOPort;
//
//		// add variable to all available parsers
//		addVariableToParsers(name, tmpContainer->GetParserVar());
//
//	}
//	else
//	{
//		throw ARTerror("ARTtimeModule::addOPort", "Port name '%s1' is already in use in current time module '%s2'.",
//				name, name_);
//	}
//}

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

ARTdataProp* ARTtimeModule::getPort(const string& name)
{
	PortType* port = dynamic_cast<PortType*>(FindProperty(name));
	if (!port)
	{
		throw ARTerror("ARTtimeModule::getPort", "No port with name '%s1' has been found in current time module '%s2'.",
				name, name_);
	}
	else
	{
		return port;
	}

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
	localParameterType* lParam = dynamic_cast<localParameterType*>(FindProperty(name));
	if (lParam)
	{
		lParam->SetDefinition(expr);
	}
	else
	{
		throw ARTerror("ARTtimeModule::setLocalParameter", "Local parameter '%s1' could not be found in current time module '%s2'.",
				name, name_);
	}


}

void ARTtimeModule::setLocalParameter(const string& name, const double val)
{
	localParameterType* lParam = dynamic_cast<localParameterType*>(FindProperty(name));
	if (lParam)
	{
		lParam->SetVal(val);
	}
	else
	{
		throw ARTerror("ARTtimeModule::setLocalParameter", "Local parameter '%s1' could not be found in current time module '%s2'.",
				name, name_);
	}
}

void ARTtimeModule::setLocalParameter(const string& name, const std::complex<double>& val)
{
	localParameterType* lParam = dynamic_cast<localParameterType*>(FindProperty(name));
	if (lParam)
	{
		lParam->SetVal(val);
	}
	else
	{
		throw ARTerror("ARTtimeModule::setLocalParameter", "Local parameter '%s1' could not be found in current time module '%s2'.",
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
			newParam->SetParser(_simulator->GetParser());
			newParam->SetDefinition(expr);

		}
		catch (ARTerror& error)
		{
			delete newParam;
			throw error;
		}
		AppendDataProp(newParam);
		addVariableToParsers(name, newParam->GetParserVar());
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

		newParam->SetParser(_simulator->GetParser());
		newParam->SetVal(val);
		AppendDataProp(newParam);
		addVariableToParsers(name, newParam->GetParserVar());
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
		newParam->SetParser(_simulator->GetParser());
		newParam->SetVal(val);
		AppendDataProp(newParam);
		addVariableToParsers(name, newParam->GetParserVar());
	}
	else
	{
		throw ARTerror("ARTtimeModule::setLocalParameter", "Name '%s1' of local parameter is already in use in current time module '%s2'.",
			name, name_);
	}
}

//void ARTtimeModule::addGlobalParameter(const string& name, const Variable& parameter)
//{
//	if (!checkVarNameExists(name))
//	{
//		_gParams[name] = &parameter;
//		addVariableToParsers(name, parameter);
//	}
//	else
//	{
//		throw ARTerror("ARTtimeModule::addGlobalParameter", "Name '%s1' of global parameter is already in use in current time module '%s2'.",
//				name, name_);
//	}
//}

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
		DeleteProperty(parameter);
		delete parameter;
	}
	else
	{
		throw ARTerror("ARTtimeModule::removeGlobalParameter", "Name '%s1' of global parameter could not be found in current time module '%s2'.",
				name, name_);
	}
}

void ARTtimeModule::setSimulator(ARTsimulator* sim)
{
	_simulator = dynamic_cast<ARTtimeSimulator*>(sim);
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
			oPort->GetArrayElement(idx).Invalidate();
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
	while (propIter)
	{
		oPort = dynamic_cast<OPortType*>(propIter);
		// only add variables to parsers of output ports
		if (oPort)
		{
			oPort->GetParser()->DefineVar(name, var);
		}
		propIter = GetProperties(propIter);
	}

}

void ARTtimeModule::removeVariableFromParsers(const string& name)
{
	// go through all parsers of output ports and remove name declaration
	ARTproperty* propIter = GetProperties(NULL);
	OPortType* oPort;
	while (propIter)
	{
		oPort = dynamic_cast<OPortType*>(propIter);
		// only add variables to parsers of output ports
		if (oPort)
		{
			oPort->GetParser()->RemoveVar(name);
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




