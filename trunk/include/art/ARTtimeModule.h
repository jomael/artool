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

#include "Interface.h"
#include "mpParser.h";

using std::string;
using namespace mup;

// forward declaration
class ARTdataContainer;
//class IValue;

class ARTItimeModule : public ARTobject
{
public:

	explicit ARTItimeModule(const string& name, const string& sds="", const string& lds="", const string& htm="") :
		ARTobject(name, sds, lds, htm)
	{}

	ARTItimeModule(const ARTItimeModule& orig) :
		ARTobject(orig)
	{}

	virtual void addIPort(const string& name, const ARTdataContainer& port) = 0;
	virtual const ARTdataContainer& getPort(const string& name) = 0;

	virtual void setLocalParameter(const string& name, const string& expr) = 0;
	virtual void setLocalParameter(const string& name, const double val) = 0;

	virtual void addGlobalParameter(const string& name, const IValue& parameter) = 0;
	virtual void removeGlobalParameter(const string& name) = 0;

	virtual ~ARTItimeModule() {}
};


class ARTtimeModule : public ARTItimeModule
{
public:

	explicit ARTtimeModule(const string& name, const string& sds="", const string& lds="", const string& htm="");
	explicit ARTtimeModule(const ARTtimeModule& orig);

	virtual void addIPort(const string& name, const ARTdataContainer& port);
	virtual void addOPort(const string& name, const string& expr);
	virtual const ARTdataContainer& getPort(const string& name);
	virtual ARTdataContainer& getOPort(const string& name);

	virtual void setLocalParameter(const string& name, const string& expr);
	virtual void setLocalParameter(const string& name, const double val);

	virtual void addGlobalParameter(const string& name, const IValue& parameter);
	virtual void removeGlobalParameter(const string& name);

	virtual ARTtimeModule& operator=(const ARTtimeModule& orig);

	virtual ~ARTtimeModule();

protected:

	virtual void addVariableToParsers(const string& name, const Variable& var);
	virtual void removeVariableFromParsers(const string& name);

	virtual void clean();
	virtual void copy(const ARTtimeModule& orig);

	struct IPortType
	{
		const ARTdataContainer* iPort;
		//Variable* portVariable;
	};
	typedef std::map<const string, IPortType*> iPortMap;
	typedef iPortMap::iterator iPortIterator;

	struct OPortType
	{
		ARTdataContainer* oPort;
		//Variable* portVariable;
		ParserX* Parser;
	};

	typedef std::map<const string, OPortType*> oPortMap;
	typedef oPortMap::iterator oPortIterator;

	iPortMap _iPorts;
	oPortMap _oPorts;
};


#endif /* ARTTIMEMODULE_H_ */
