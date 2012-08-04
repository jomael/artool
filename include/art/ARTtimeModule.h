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

//class ARTPortType
//{
//public:
//	virtual void initPortValue(const string& expr) const;
//	virtual void initPortValue(double value, int idx) const;
//	virtual void initPortValue(std::complex<double>& value, int idx) const;
//
//	// make class ARTPortType polymorphic
//	virtual ~ARTPortType() {}
//	friend class ARTItimeModule;
//protected:
//	const ARTdataContainer* _port;
//	ARTtimeSimulator** _simulator;
//};
//
//class ARTIPortType : public ARTPortType
//{
//public:
//	virtual ~ARTIPortType() {}
//	friend class ARTItimeModule;
//
//};
//
//class ARTOPortType : public ARTPortType
//{
//public:
//	virtual std::complex<double> operator[](int idx) const;
//	virtual ~ARTOPortType() {}
//	friend class ARTItimeModule;
//protected:
//	ParserX* _parser;
//};

class ARTItimeModule : public ARTobject
{

public:

	class timeProperty : public ARTdataProp
	{
	public:
		timeProperty(const T_ART_Type dtyp, const int dlen, const string name, const string sds="", const string lds="", const string htm="")
				: ARTdataProp(dtyp, dlen, name, sds, lds, htm) {}
		virtual ~timeProperty() {}
	};

	class localParameterType : public timeProperty
	{
	public:
		localParameterType(const string& name) : timeProperty(C_ART_cpx, 0, name) {}
		virtual ~localParameterType() {}
	};

	class globalParameterType : public timeProperty
	{
	protected:
		const ARTdataProp* param_;
	public:
		globalParameterType(const string& name, const ARTdataProp* param);
		virtual const Variable& GetParserVar();
		virtual const Variable& GetParserVar() const;
		virtual ~globalParameterType() {}
	};

	class PortType : public timeProperty
	{
	public:
		PortType(const T_ART_Type dtyp, const int dlen, const string name, const string sds="", const string lds="", const string htm="")
				: timeProperty(dtyp, dlen, name, sds, lds, htm) {}
		virtual ~PortType() {}
	};

	class OPortType : public PortType
	{
	protected:
		Value tVal_;
		Variable* tVar_;
	public:
		OPortType(const T_ART_Type dtyp, const int dlen, const string name, const string sds="", const string lds="", const string htm="");
		virtual void initPortValue(const string& expr) const;
		virtual void initPortValue(double value, int idx) const;
		virtual void initPortValue(std::complex<double>& value, int idx) const;
		virtual IValue& operator[](std::size_t idx);
		virtual IValue& operator[](int idx);
		virtual IValue& GetPortValue(std::size_t idx);
		virtual IValue& GetPortValue(int idx);
		virtual ~OPortType();
	};

	class IPortType : public PortType
	{
	protected:
		const OPortType* refPort_;
	public:
		IPortType(const string& name, const OPortType* refPort);
		virtual const Variable& GetParserVar();
		virtual const Variable& GetParserVar() const;
		virtual ~IPortType() {}
	};

public:

	explicit ARTItimeModule(const string& name, const string& sds="", const string& lds="", const string& htm="") :
		ARTobject(name, sds, lds, htm), _simulator(NULL)
	{}

	ARTItimeModule(const ARTItimeModule& orig) :
		ARTobject(orig), _simulator(orig._simulator)
	{}

	virtual void addIPort(const string& name, const ARTdataProp* refPort) = 0;
	virtual ARTdataProp* getPort(const string& name) = 0;

	virtual void setLocalParameter(const string& name, const string& expr) = 0;
	virtual void setLocalParameter(const string& name, const std::complex<double>& val) = 0;
	virtual void setLocalParameter(const string& name, const double val) = 0;

//	virtual void addGlobalParameter(const string& name, const Variable& parameter) = 0;
	virtual void addGlobalParameter(const ARTdataProp* parameter) = 0;
	virtual void removeGlobalParameter(const string& name) = 0;

	virtual void setSimulator(ARTsimulator* sim);
	virtual void setCurrentIndex(int idx) = 0;

	virtual ~ARTItimeModule() {}

protected:

	ARTtimeSimulator* _simulator;

//	inline static ARTdataContainer* getContainerFromPort(const ARTPortType& port)
//	{
//		ARTdataContainer* tmpContainer = const_cast<ARTdataContainer*>(port._port);
//		return tmpContainer;
//	}
//
//	inline static void setContainerForPort(ARTPortType& port, const ARTdataContainer* container)
//	{
//		port._port = container;
//	}
//
//	inline static void setSimulatorForPort(ARTPortType& port, ARTtimeSimulator*& sim)
//	{
//		port._simulator = &sim;
//	}
//
//	inline static ParserX* getParserFromOPort(const ARTOPortType& port)
//	{
//		ParserX* tmpParser = NULL;
//		tmpParser = const_cast<ParserX*>(port._parser);
//		return tmpParser;
//	}
//
//	inline static void setParserForOPort(ARTOPortType& port, const ParserX* parser)
//	{
//		port._parser = const_cast<ParserX*>(parser);
//	}

};


class ARTtimeModule : public ARTItimeModule
{
public:

	explicit ARTtimeModule(const string& name, const string& sds="", const string& lds="", const string& htm="");
//	explicit ARTtimeModule(const ARTtimeModule& orig);

	virtual void addIPort(const string& name, const ARTdataProp* refPort);
	virtual void addOPort(const string& name, const string& expr, unsigned int size = 20);
	virtual ARTdataProp* getPort(const string& name);
//	virtual const ARTOPortType& getOPort(const string& name);

	virtual void setLocalParameter(const string& name, const string& expr);
	virtual void setLocalParameter(const string& name, const double val);
	virtual void setLocalParameter(const string& name, const std::complex<double>& val);

	virtual void addLocalParameter(const string& name, const string& expr);
	virtual void addLocalParameter(const string& name, const double val);
	virtual void addLocalParameter(const string& name, const std::complex<double>& val);

//	virtual void addGlobalParameter(const string& name, const Variable& parameter);
	virtual void addGlobalParameter(const ARTdataProp* parameter);
	virtual void removeGlobalParameter(const string& name);

	virtual void setSimulator(ARTsimulator* sim);
	virtual void setCurrentIndex(int idx);

//	virtual ARTtimeModule& operator=(const ARTtimeModule& orig);

	virtual ~ARTtimeModule();

protected:

//	inline virtual bool checkVarNameExists(const string& name);

	inline virtual void addVariableToParsers(const string& name, const Variable& var);
	inline virtual void removeVariableFromParsers(const string& name);

	inline virtual void registerAllVariablesToParser(ParserX* parser);

	inline virtual void clean();
//	inline virtual void copy(const ARTtimeModule& orig);


//	typedef std::map<const string, ARTIPortType*> iPortMap;
//	typedef iPortMap::iterator iPortIterator;
//
//	typedef std::map<const string, ARTOPortType*> oPortMap;
//	typedef oPortMap::iterator oPortIterator;
//
//	iPortMap _iPorts;
//	oPortMap _oPorts;

};


#endif /* ARTTIMEMODULE_H_ */
