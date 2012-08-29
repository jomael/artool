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

	virtual ARTItimeModule* Create(const string& name, const string& sds="", const string& lds="", const string& htm="");

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
	explicit fractionalDelayModule(const string& name="FractionalDelayModule", const string& sds="", const string& lds="", const string& htm="");
	virtual ARTItimeModule* Create(const string& name, const string& sds="", const string& lds="", const string& htm="");

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

class DWGcylinderModule : public fractionalDelayModule
{
protected:
	OPortType* outm_;

	IPortType* p1p_;
	OPortType* p2p_;
	OPortType* p1m_;
	IPortType* p2m_;

public:
	explicit DWGcylinderModule(const string& name="DWGCylinderModule", const string& sds="", const string& lds="", const string& htm="");
	virtual ARTItimeModule* Create(const string& name, const string& sds="", const string& lds="", const string& htm="");

	virtual void addIPort(const string& name, const ARTdataProp* refPort);
	virtual ARTdataProp* getPort(const string& name);

	virtual void setCurrentIndex(int idx);
	virtual void simulateCurrentIndex(int idx);

	virtual ~DWGcylinderModule() {}
protected:
	virtual void initLocalParams();
	virtual void initSimulation();

};

class impulseModule : public ARTItimeModule
{
protected:
	OPortType* out_;
public:
	explicit impulseModule(const string& name = "ImpulseModule", const string& sds="", const string& lds="", const string& htm="");
	virtual ARTItimeModule* Create(const string& name, const string& sds="", const string& lds="", const string& htm="");

	virtual void addIPort(const string& name, const ARTdataProp* refPort);
	virtual ARTdataProp* getPort(const string& name);

	virtual void setCurrentIndex(int idx);
	virtual void simulateCurrentIndex(int idx);

	virtual ~impulseModule() {}
protected:
	virtual void initLocalParams();
};

class heavisideModule : public ARTItimeModule
{
protected:
	OPortType* out_;
public:
	explicit heavisideModule(const string& name = "HeavisideModule", const string& sds="", const string& lds="", const string& htm="");
	virtual ARTItimeModule* Create(const string& name, const string& sds="", const string& lds="", const string& htm="");

	virtual void addIPort(const string& name, const ARTdataProp* refPort);
	virtual ARTdataProp* getPort(const string& name);

	virtual void setCurrentIndex(int idx);
	virtual void simulateCurrentIndex(int idx);

	virtual ~heavisideModule() {}
protected:
	virtual void initLocalParams();
};

class rectengularModule : public ARTItimeModule
{
protected:
	OPortType* out_;
public:
	explicit rectengularModule(const string& name = "RectengularModule", const string& sds="", const string& lds="", const string& htm="");
	virtual ARTItimeModule* Create(const string& name, const string& sds="", const string& lds="", const string& htm="");

	virtual void addIPort(const string& name, const ARTdataProp* refPort);
	virtual ARTdataProp* getPort(const string& name);

	virtual void setCurrentIndex(int idx);
	virtual void simulateCurrentIndex(int idx);

	virtual ~rectengularModule() {}
protected:
	virtual void initLocalParams();
};

class amplificationModule : public ARTItimeModule
{
protected:
	OPortType* out_;
	IPortType* in_;
public:
	explicit amplificationModule(const string& name = "AmplificationModule", const string& sds="", const string& lds="", const string& htm="");
	virtual ARTItimeModule* Create(const string& name, const string& sds="", const string& lds="", const string& htm="");

	virtual void addIPort(const string& name, const ARTdataProp* refPort);
	virtual ARTdataProp* getPort(const string& name);

	virtual void setCurrentIndex(int idx);
	virtual void simulateCurrentIndex(int idx);

	virtual ~amplificationModule() {}
protected:
	virtual void initLocalParams();
};

class simpleDelayModule : public ARTItimeModule
{
protected:
	OPortType* out_;
	IPortType* in_;
public:
	explicit simpleDelayModule(const string& name = "SimpleDelayModule", const string& sds="", const string& lds="", const string& htm="");
	virtual ARTItimeModule* Create(const string& name, const string& sds="", const string& lds="", const string& htm="");

	virtual void addIPort(const string& name, const ARTdataProp* refPort);
	virtual ARTdataProp* getPort(const string& name);

	virtual void setCurrentIndex(int idx);
	virtual void simulateCurrentIndex(int idx);

	virtual ~simpleDelayModule() {}
protected:
	virtual void initLocalParams();
};

class addModule : public ARTItimeModule
{
protected:
	OPortType* out_;
public:
	explicit addModule(const string& name = "AddModule", const string& sds="", const string& lds="", const string& htm="");
	virtual ARTItimeModule* Create(const string& name, const string& sds="", const string& lds="", const string& htm="");

	virtual void addIPort(const string& name, const ARTdataProp* refPort);
	virtual ARTdataProp* getPort(const string& name);

	virtual void setCurrentIndex(int idx);
	virtual void simulateCurrentIndex(int idx);

	virtual ~addModule() {}
};

class multiplicationModule : public ARTItimeModule
{
protected:
	OPortType* out_;
public:
	explicit multiplicationModule(const string& name = "MultiplicationModule", const string& sds="", const string& lds="", const string& htm="");
	virtual ARTItimeModule* Create(const string& name, const string& sds="", const string& lds="", const string& htm="");

	virtual void addIPort(const string& name, const ARTdataProp* refPort);
	virtual ARTdataProp* getPort(const string& name);

	virtual void setCurrentIndex(int idx);
	virtual void simulateCurrentIndex(int idx);

	virtual ~multiplicationModule() {}
};

class sinewaveModule : public ARTItimeModule
{
protected:
	OPortType* out_;
public:
	explicit sinewaveModule(const string& name = "SinewaveModule", const string& sds="", const string& lds="", const string& htm="");
	virtual ARTItimeModule* Create(const string& name, const string& sds="", const string& lds="", const string& htm="");

	virtual void addIPort(const string& name, const ARTdataProp* refPort);
	virtual ARTdataProp* getPort(const string& name);

	virtual void setCurrentIndex(int idx);
	virtual void simulateCurrentIndex(int idx);

	virtual ~sinewaveModule() {}
protected:
	virtual void initLocalParams();
};


#endif /* TIMEPROTOTYPES_H_ */
