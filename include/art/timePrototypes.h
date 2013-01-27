/**************************************************************************
*                                                                         *
*                   Acoustic Research Tool (ART)                          *
*                                                                         *
*   A Simulation Framework with Modelling Library for Acoustic Systems    *
*                                                                         *
*         Project of the Workgroup 2 of the Technical Committee           *
*      Musical Acoustics of the European Acoustics Association EAA)       *
*                                                                         *
*   http://www.eaa-fenestra.org/technical-committees/ma/workgroups/wg2    *
*                                                                         *
*  Copyright (C) 2013 by the authors and their organisations:             *
*    Alistair Braden            1)                                        *
*    Wilfried Kausel            2)         kausel(at)mdw.ac.at            *
*    Delphine Cadefaux          2)                                        *
*    Vasileios Chatziioannou    2)                                        *
*    Sadjad Siddiq              2)                                        *
*    Clemens Geyer              2)                                        *
*                                                                         *
*    1) School of Physics and Astronomy, Univ. of Edinburgh, GB           *
*       (http://www.ph.ed.ac.uk/)                                         *
*    2) Inst. of Music Acoustics, Univ. of Music, Vienna, AT              *
*       (http://iwk.mdw.ac.at)                                            *
*                                                                         *
*  This program is free software: you can redistribute it and/or modify   *
*  it under the terms of the GNU General Public License as published by   *
*  the Free Software Foundation, either version 3 of the License, or      *
*  any later version.                                                     *
*                                                                         *
*  This program is distributed in the hope that it will be useful,        *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
*  GNU General Public License for more details.                           *
*                                                                         *
*  You should have received a copy of the GNU General Public License      *
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
*                                                                         *
***************************************************************************/

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
	explicit inputFunctionModule(const string& name="InputFunctionModule", const string& sds="", const string& lds="", const string& htm="");

	virtual ARTItimeModule* Create(const string& name, const string& sds="", const string& lds="", const string& htm="");


	virtual void addIPort(const string& name, const ARTdataProp* refPort);
	virtual void defineOPort(int len, const string& expr);
	virtual ARTdataProp* getPort(const string& name);

//	virtual void addGlobalParameter(const ARTdataProp* parameter);
//	virtual void removeGlobalParameter(const string& name);

	// do nothing
	virtual void setCurrentIndex(int idx) {}
	virtual void simulateCurrentIndex(int idx) {}

	virtual ~inputFunctionModule();
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

class genericDelayModule : public ARTItimeModule
{
public:
	explicit genericDelayModule(const string& name, const string& sds="", const string& lds="", const string& htm="") :
			ARTItimeModule(name, sds, lds, htm) {}
	~genericDelayModule() {}
protected:
	virtual double getLagrangeParams(int n, int N, double D);
	virtual double getThiranParams(int n, int N, double D);

	virtual double fac(int n);
	virtual double binom(int n, int k);
};

class fractionalDelayModule : public genericDelayModule
{
protected:
	OPortType* out_;
	IPortType* in_;
public:
	explicit fractionalDelayModule(const string& name="FractionalDelayModule", const string& sds="", const string& lds="", const string& htm="");
	virtual ARTItimeModule* Create(const string& name, const string& sds="", const string& lds="", const string& htm="");

	virtual void addIPort(const string& name, const ARTdataProp* refPort);
	virtual ARTdataProp* getPort(const string& name);

	virtual void setCurrentIndex(int idx);
	virtual void simulateCurrentIndex(int idx);

	virtual ~fractionalDelayModule() {}
protected:
	virtual void initLocalParams();
	virtual void initSimulation();


};

class DWGcylinderModule : public genericDelayModule
{
protected:
	IPortType* p1p_;
	OPortType* p2p_;
	OPortType* p1m_;
	IPortType* p2m_;

	OPortType* p1pbuf_;
	OPortType* p2mbuf_;

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

class DWGcylinderJunctionModule : public ARTItimeModule
{
protected:
	IPortType* p1p_;
	OPortType* p2p_;
	OPortType* p1m_;
	IPortType* p2m_;

	localParameterType* r1_;
	localParameterType* r2_;

public:
	explicit DWGcylinderJunctionModule(const string& name="DWGCylinderJunctionModule", const string& sds="", const string& lds="", const string& htm="");
	virtual ARTItimeModule* Create(const string& name, const string& sds="", const string& lds="", const string& htm="");

	virtual void addIPort(const string& name, const ARTdataProp* refPort);
	virtual ARTdataProp* getPort(const string& name);

	virtual void setCurrentIndex(int idx);
	virtual void simulateCurrentIndex(int idx);

	virtual ~DWGcylinderJunctionModule() {}
protected:
	virtual void initLocalParams();

};

class DWGconeModule : public DWGcylinderModule
{

public:
	explicit DWGconeModule(const string& name="DWGConeModule", const string& sds="", const string& lds="", const string& htm="");
	virtual ARTItimeModule* Create(const string& name, const string& sds="", const string& lds="", const string& htm="");

	virtual ~DWGconeModule() {}
protected:
	virtual void initLocalParams();
	virtual void initSimulation();
	virtual void calculateConeApex();

};

class DWGconeJunctionModule : public ARTItimeModule
{
protected:
	IPortType* p1p_;
	OPortType* p2p_;
	OPortType* p1m_;
	IPortType* p2m_;

	OPortType* rz_;

	localParameterType* r1_;
	localParameterType* r2_;
	localParameterType* S1_;
	localParameterType* S2_;

public:
	explicit DWGconeJunctionModule(const string& name="DWGConeJunctionModule", const string& sds="", const string& lds="", const string& htm="");
	virtual ARTItimeModule* Create(const string& name, const string& sds="", const string& lds="", const string& htm="");

	virtual void addIPort(const string& name, const ARTdataProp* refPort);
	virtual ARTdataProp* getPort(const string& name);

	virtual void setCurrentIndex(int idx);
	virtual void simulateCurrentIndex(int idx);

	virtual ~DWGconeJunctionModule() {}
protected:
	virtual void initLocalParams();
	virtual void initSimulation();

	virtual void calculateConeApex();

	virtual double getB0(const string& method);
	virtual double getB1(const string& method);
	virtual double getA1(const string& method);

};


#endif /* TIMEPROTOTYPES_H_ */
