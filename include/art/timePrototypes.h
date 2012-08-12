/*
 * timePrototypes.h
 *
 *  Created on: Aug 12, 2012
 *      Author: cbg
 */

#ifndef TIMEPROTOTYPES_H_
#define TIMEPROTOTYPES_H_

#include "ARTtimeModule.h"

class inputFunctionModule : public ARTItimeModule
{
protected:
	FPortType* out_;
public:
	explicit inputFunctionModule(const string& name, const int len = 0, const string& sds="", const string& lds="", const string& htm="");

	virtual void addIPort(const string& name, const ARTdataProp* refPort);
	virtual ARTdataProp* getPort(const string& name);

	virtual void addGlobalParameter(const ARTdataProp* parameter);
	virtual void removeGlobalParameter(const string& name);

	// do nothing
	virtual void setCurrentIndex(int idx) {}

	virtual ~inputFunctionModule();
};


#endif /* TIMEPROTOTYPES_H_ */
