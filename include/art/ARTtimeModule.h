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
 * ARTtimeModule.h
 *
 *  Created on: Jul 4, 2012
 *      Author: cbg
 */

#ifndef ARTTIMEMODULE_H_
#define ARTTIMEMODULE_H_

#include <string>
#include <complex>

#include "Interface.h"
#include "ARTsimulator.h"
#include "mpParser.h"

using std::string;
using namespace mup;

// forward declaration
class ARTdataContainer;

class ARTItimeModule : public ARTobject
{

public:

	class timeProperty : public ARTdataProp
	{
	public:
		timeProperty(const T_ART_Type dtyp, const int dlen, const string& name, const string sds="", const string lds="", const string htm="")
				: ARTdataProp(dtyp, dlen, name, sds, lds, htm) {}
		virtual ~timeProperty() {}
	};

	class localParameterType : public timeProperty
	{
	public:
		localParameterType(const string& name, const string sds="", const string lds="", const string htm="");
		virtual ~localParameterType();
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
//	protected:
//		Value tVal_;
//		Variable* tVar_;
	public:
		OPortType(const T_ART_Type dtyp, const int dlen, const string name, const string sds="", const string lds="", const string htm="");
		virtual void initPortValue(const string& expr) const;
		virtual void initPortValue(double value, int idx) const;
		virtual void initPortValue(std::complex<double>& value, int idx) const;
//		virtual IValue& operator[](std::size_t idx);
//		virtual IValue& operator[](int idx);
//		virtual IValue& GetPortValue(std::size_t idx);
//		virtual IValue& GetPortValue(int idx);
		virtual ~OPortType();
	};

	class FPortType : public OPortType
	{
	public:
		FPortType(const int dlen, const string name, const string sds="", const string lds="", const string htm="");
//		virtual void initPortValue(const string& expr) const;
//		virtual void initPortValue(double value, int idx) const;
//		virtual void initPortValue(std::complex<double>& value, int idx) const;
		virtual IValue& operator[](std::size_t idx);
		virtual IValue& operator[](int idx);
		virtual ~FPortType() {}
	};

	class IPortType : public PortType
	{
	protected:
		const OPortType* refPort_;
	public:
		IPortType(const string& name, const OPortType* refPort);
//		IPortType(const string& name, const FPortType* refPort);
		virtual const Variable& GetParserVar();
		virtual const Variable& GetParserVar() const;
		virtual ~IPortType() {}
	};

public:

	explicit ARTItimeModule(const string& name, const string& sds="", const string& lds="", const string& htm="") :
		ARTobject(name, sds, lds, htm), _simulator(NULL)
	{
		// all time modules exist in the time domain
		AppendMethod("TimeDomain", "simulation in time domain");
	}

	ARTItimeModule(const ARTItimeModule& orig) :
		ARTobject(orig), _simulator(orig._simulator)
	{}

	virtual ARTItimeModule* Create(const string& name, const string& sds="", const string& lds="", const string& htm="") = 0;

	virtual void addIPort(const string& name, const ARTdataProp* refPort) = 0;
	virtual ARTdataProp* getPort(const string& name);

	virtual void setLocalParameter(const string& name, const string& expr);
	virtual void setLocalParameter(const string& name, const std::complex<double>& val);
	virtual void setLocalParameter(const string& name, const double val);

	virtual void addGlobalParameter(const ARTdataProp* parameter);
	virtual void removeGlobalParameter(const string& name);

	virtual void setSimulator(ARTsimulator* sim);
	virtual void setCurrentIndex(int idx) = 0;
	virtual void simulateCurrentIndex(int idx) = 0;

	virtual ~ARTItimeModule() {}

protected:

	ARTtimeSimulator* _simulator;
};


class ARTtimeModule : public ARTItimeModule
{
public:

	explicit ARTtimeModule(const string& name="TimeModule", const string& sds="", const string& lds="", const string& htm="");
//	explicit ARTtimeModule(const ARTtimeModule& orig);

	virtual ARTItimeModule* Create(const string& name, const string& sds="", const string& lds="", const string& htm="");

	virtual void addIPort(const string& name, const ARTdataProp* refPort);
	virtual void addOPort(const string& name, const string& expr, unsigned int size = 20);

	virtual void addLocalParameter(const string& name, const string& expr);
	virtual void addLocalParameter(const string& name, const double val);
	virtual void addLocalParameter(const string& name, const std::complex<double>& val);

	virtual void addGlobalParameter(const ARTdataProp* parameter);
	virtual void removeGlobalParameter(const string& name);

	virtual void setCurrentIndex(int idx);
	virtual void simulateCurrentIndex(int idx);

//	virtual ARTtimeModule& operator=(const ARTtimeModule& orig);

	virtual ~ARTtimeModule();

protected:

	inline virtual void addVariableToParsers(const string& name, const Variable& var);
	inline virtual void removeVariableFromParsers(const string& name);

	inline virtual void registerAllVariablesToParser(ParserX* parser);

	inline virtual void clean();
//	inline virtual void copy(const ARTtimeModule& orig);

};


#endif /* ARTTIMEMODULE_H_ */
