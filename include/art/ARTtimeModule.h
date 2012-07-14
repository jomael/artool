/*
 * ARTtimeModule.h
 *
 *  Created on: Jul 4, 2012
 *      Author: cbg
 */

#ifndef ARTTIMEMODULE_H_
#define ARTTIMEMODULE_H_

#include <string>
#include <vector>
#include <utility>
#include <complex>

#include "Interface.h"
#include "ARTsimulator.h"
#include "mpParser.h"

using std::string;
using namespace mup;

// forward declaration
class ARTdataContainer;
class ARTItimeModule;
//class IValue;

class ARTPortType
{
public:
	virtual void initPortValue(std::complex<double> value, int idx) const;

	// make class ARTPortType polymorphic
	virtual ~ARTPortType() {}
	friend class ARTItimeModule;
protected:
	const ARTdataContainer* _port;
	ARTtimeSimulator** _simulator;
};

class ARTIPortType : public ARTPortType
{
public:
	virtual ~ARTIPortType() {}
	friend class ARTItimeModule;

};

class ARTOPortType : public ARTPortType
{
public:
	virtual std::complex<double> operator[](int idx) const;
	virtual ~ARTOPortType() {}
	friend class ARTItimeModule;
protected:
	ParserX* _parser;
};


class ARTItimeModule : public ARTobject
{

public:

	explicit ARTItimeModule(const string& name, const string& sds="", const string& lds="", const string& htm="") :
		ARTobject(name, sds, lds, htm), _simulator(NULL)
	{}

	ARTItimeModule(const ARTItimeModule& orig) :
		ARTobject(orig), _simulator(orig._simulator)
	{}

	virtual void addIPort(const string& name, const ARTPortType& port) = 0;
	virtual const ARTPortType& getPort(const string& name) = 0;

	virtual void setLocalParameter(const string& name, const string& expr) = 0;
	virtual void setLocalParameter(const string& name, const double val) = 0;

	virtual void addGlobalParameter(const string& name, const Variable& parameter) = 0;
	virtual void removeGlobalParameter(const string& name) = 0;

	virtual void setSimulator(ARTsimulator* sim);
	virtual void setCurrentIndex(int idx) = 0;

	virtual ~ARTItimeModule() {}

protected:

	ARTtimeSimulator* _simulator;

	inline static ARTdataContainer* getContainerFromPort(const ARTPortType& port)
	{
		ARTdataContainer* tmpContainer = const_cast<ARTdataContainer*>(port._port);
		return tmpContainer;
	}

	inline static void setContainerForPort(ARTPortType& port, const ARTdataContainer* container)
	{
		port._port = container;
	}

	inline static void setSimulatorForPort(ARTPortType& port, ARTtimeSimulator*& sim)
	{
		port._simulator = &sim;
	}

	inline static ParserX* getParserFromOPort(const ARTOPortType& port)
	{
		ParserX* tmpParser = NULL;
		tmpParser = const_cast<ParserX*>(port._parser);
		return tmpParser;
	}

	inline static void setParserForOPort(ARTOPortType& port, const ParserX* parser)
	{
		port._parser = const_cast<ParserX*>(parser);
	}

};


class ARTtimeModule : public ARTItimeModule
{
public:

	explicit ARTtimeModule(const string& name, const string& sds="", const string& lds="", const string& htm="");
	explicit ARTtimeModule(const ARTtimeModule& orig);

	virtual void addIPort(const string& name, const ARTPortType& port);
	virtual void addOPort(const string& name, const string& expr);
	virtual const ARTPortType& getPort(const string& name);
	//virtual const ARTOPortType& getOPort(const string& name);

	virtual void setLocalParameter(const string& name, const string& expr);
	virtual void setLocalParameter(const string& name, const double val);
	virtual void setLocalParameter(const string& name, const std::complex<double>& val);

	virtual void addGlobalParameter(const string& name, const Variable& parameter);
	virtual void removeGlobalParameter(const string& name);

	virtual void setCurrentIndex(int idx);

	virtual ARTtimeModule& operator=(const ARTtimeModule& orig);

	virtual ~ARTtimeModule();

protected:

	inline virtual bool checkVarNameExists(const string& name);

	inline virtual void addVariableToParsers(const string& name, const Variable& var);
	inline virtual void removeVariableFromParsers(const string& name);

	inline virtual void registerAllVariablesToParser(ParserX* parser);

	inline virtual void clean();
	inline virtual void copy(const ARTtimeModule& orig);

//	struct IPortType
//	{
//		const ARTdataContainer* iPort;
//		//Variable* portVariable;
//	};
	typedef std::map<const string, ARTIPortType*> iPortMap;
	typedef iPortMap::iterator iPortIterator;

//	class OPortType
//	{
//	private:
//		ARTdataContainer* oPort;
//		//Variable* portVariable;
//		ParserX* Parser;
//	};
	typedef std::map<const string, ARTOPortType*> oPortMap;
	typedef oPortMap::iterator oPortIterator;

	struct LocalParameterType
	{
		Value* val;
		Variable* var;
	};
	typedef std::map<const string, LocalParameterType*> localParameterMap;
	typedef localParameterMap::iterator localParameterIterator;

	typedef std::map<const string, const Variable*> globalParameterMap;
	typedef globalParameterMap::iterator globalParameterIterator;

	//typedef std::map

	iPortMap _iPorts;
	oPortMap _oPorts;
	localParameterMap _lParams;
	globalParameterMap _gParams;
};


#endif /* ARTTIMEMODULE_H_ */
