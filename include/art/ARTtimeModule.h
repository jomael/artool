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

/**
 * @brief This class provides a generic interface for all time-domain simulations
 *        and is used by the ARTtimeSimulator class.
 *
 * @details All time modules have to be derived from this class and have to
 *          implement the addIPort(), setCurrentIndex() and
 *          simulateCurrentIndex() methods.
 */
class ARTItimeModule : public ARTobject
{

public:

  /**
   * This is the generic class representing all types of attributes which can
   * be added to a time module.
   */
  class timeProperty : public ARTdataProp
  {
  public:
    /**
     * @brief Constructor of the timeProperty class.
     * @param[in] dtyp Defines the type of the property and will be used by the
     *            ARTdataContainer class in order to allocate the right amount
     *            of memory.
     * @param[in] dlen Defines the size/array length of the time property.
     * @param[in] name Represents the locally unique identifier of the property.
     * @param[in] sds Short description (single line) of the time property.
     * @param[in] lds Long description of the time property.
     * @param[in] htm Path to help file in HTML format.
     */
    timeProperty(const T_ART_Type dtyp, const int dlen, const string& name, const string sds="", const string lds="", const string htm="")
    : ARTdataProp(dtyp, dlen, name, sds, lds, htm) {}
    /**
     * @brief Destructor of the timeProperty class.
     */
    virtual ~timeProperty() {}
  };

  /**
   * This class represents all parameters which only have local scope within the
   * time module they are associated with.
   */
  class localParameterType : public timeProperty
  {

  public:
    /**
     * @brief Constructor of the globalParameterType class.
     * @param[in] name Represents the locally unique identifier of the property.
     * @param[in] sds Short description (single line) of the time property.
     * @param[in] lds Long description of the time property.
     * @param[in] htm Path to help file in HTML format.
     */
    localParameterType(const string& name, const string sds="", const string lds="", const string htm="");
    virtual ~localParameterType();
  };

  /**
   * This class represents all parameters which have global scope within the
   * time module they are associated with. They are actually referencing
   * a global simulation parameter saved in the ARTtimeSimulator class.
   */
  class globalParameterType : public timeProperty
  {
  protected:
    /** Pointer to the actual global simulation parameter object. */
    const ARTdataProp* param_;
  public:
    /**
     * @brief Constructor of the global parameter class.
     * @param[in] name Identifier of the global parameter.
     * @param[in] param Pointer to the actual global simulation parameter this
     *            class is referencing.
     */
    globalParameterType(const string& name, const ARTdataProp* param);
    /**
     * @brief Returns the parser variable of the referenced object.
     */
    virtual const Variable& GetParserVar();
    /**
     * @brief Returns the parser variable of the referenced object.
     */
    virtual const Variable& GetParserVar() const;
    /**
     * @brief Destructor of the global parameter class.
     */
    virtual ~globalParameterType() {}
  };

  /**
   * Parent class of all port types (input, output, function) which can be
   * added to a time module.
   */
  class PortType : public timeProperty
  {
  public:
    /**
     * @brief Constructor of the PortType class.
     * @param[in] dtyp Defines the type of the property and will be used by the
     *            ARTdataContainer class in order to allocate the right amount
     *            of memory.
     * @param[in] dlen Defines the size/array length of the time property.
     * @param[in] name Represents the locally unique identifier of the property.
     * @param[in] sds Short description (single line) of the time property.
     * @param[in] lds Long description of the time property.
     * @param[in] htm Path to help file in HTML format.
     */
    PortType(const T_ART_Type dtyp, const int dlen, const string& name, const string sds="", const string lds="", const string htm="")
    : timeProperty(dtyp, dlen, name, sds, lds, htm) {}
    /**
     * @brief Destructor of the PortType class.
     */
    virtual ~PortType() {}
  };

  class OPortType : public PortType
  {
    //	protected:
    //		Value tVal_;
    //		Variable* tVar_;
  public:
    OPortType(const T_ART_Type dtyp, const int dlen, const string& name, const string sds="", const string lds="", const string htm="");
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
    FPortType(const int dlen, const string& name, const string sds="", const string lds="", const string htm="");
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
