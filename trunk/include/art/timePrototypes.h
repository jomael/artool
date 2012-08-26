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
	virtual void simulateCurrentIndex(int idx) {}

	virtual ~inputFunctionModule();
};

class fractionalDelayModule : public ARTItimeModule
{
protected:
	OPortType* out_;
	IPortType* in_;
public:
	explicit fractionalDelayModule(const string& name, const string& sds="", const string& lds="", const string& htm="");

	virtual void addIPort(const string& name, const ARTdataProp* refPort);
	virtual ARTdataProp* getPort(const string& name);

//	virtual void addGlobalParameter(const ARTdataProp* parameter);
//	virtual void removeGlobalParameter(const string& name);

	virtual void setCurrentIndex(int idx);
	virtual void simulateCurrentIndex(int idx);

	virtual ~fractionalDelayModule() {}
protected:
	virtual void initLocalParams();
	virtual void initSimulation();

	virtual double getLagrangeParams(int n, int N, double D);
	virtual double getThiranParams(int n, int N, double D);

	virtual double fac(int n);
	virtual double binom(int n, int k);
};

class impulseModule : public ARTItimeModule
{
protected:
	OPortType* out_;
public:
	explicit impulseModule(const string& name, const string& sds="", const string& lds="", const string& htm="");

	virtual ARTdataProp* getPort(const string& name);

	virtual void setCurrentIndex(int idx);
	virtual void simulateCurrentIndex(int idx);

	virtual ~impulseModule() {}
protected:
	virtual void initLocalParams();
};


#endif /* TIMEPROTOTYPES_H_ */
