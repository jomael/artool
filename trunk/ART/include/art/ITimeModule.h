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

#ifndef ARTITIMEMODULE_H_
#define ARTITIMEMODULE_H_

#include <string>
#include <complex>

#include "ARTobject.h"
#include "TimeSimulator.h"
#include "mpParser.h"

using std::string;
using namespace mup;

// forward declaration
namespace ART{
  class DataContainer;

/**
 * @brief This class provides a generic interface for all time-domain simulations
 *        and is used by the ARTtimeSimulator class.
 *
 * @details All time modules have to be derived from this class and have to
 *          implement the addIPort(), setCurrentIndex() and
 *          simulateCurrentIndex() methods.
 */
class ITimeModule : public ARTobject
{

public:

  /**
   * This is the generic class representing all types of attributes which can
   * be added to a time module.
   */
  class timeProperty : public DataProp
  {
  public:
    /**
     * @brief Constructor of the timeProperty class.
     * @param[in] dtyp Defines the type of the property and will be used by the
     *            DataContainer class in order to allocate the right amount
     *            of memory.
     * @param[in] dlen Defines the size/array length of the time property.
     * @param[in] name Represents the locally unique identifier of the property.
     * @param[in] sds Short description (single line) of the time property.
     * @param[in] lds Long description of the time property.
     * @param[in] htm Path to help file in HTML format.
     */
    timeProperty(const T_ART_Type dtyp, const int dlen, const string& name, const string sds="", const string lds="", const string htm="")
    : DataProp(dtyp, dlen, name, sds, lds, htm) {}
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
    const DataProp* param_;
  public:
    /**
     * @brief Constructor of the global parameter class.
     * @param[in] name Identifier of the global parameter.
     * @param[in] param Pointer to the actual global simulation parameter this
     *            class is referencing.
     */
    globalParameterType(const string& name, const DataProp* param);
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
     *            DataContainer class in order to allocate the right amount
     *            of memory.
     * @param[in] dlen Defines the size/array length of the port.
     * @param[in] name Represents the locally unique identifier of the port.
     * @param[in] sds Short description (single line) of the port.
     * @param[in] lds Long description of the port.
     * @param[in] htm Path to help file in HTML format.
     */
    PortType(const T_ART_Type dtyp, const int dlen, const string& name, const string sds="", const string lds="", const string htm="")
    : timeProperty(dtyp, dlen, name, sds, lds, htm) {}
    /**
     * @brief Destructor of the PortType class.
     */
    virtual ~PortType() {}
  };

  /**
   * This class represents all output ports of time modules and saves the actual
   * values of each time index and the evaluation expression.
   */
  class OPortType : public PortType
  {
  public:
    /**
     * @brief Constructor of the OPortType class.
     * @param[in] dtyp Defines the type of the port and will be used by the
     *            DataContainer class in order to allocate the right amount
     *            of memory.
     * @param[in] dlen Defines the size/array length of the port.
     * @param[in] name Represents the locally unique identifier of the port.
     * @param[in] sds Short description (single line) of the port.
     * @param[in] lds Long description of the port.
     * @param[in] htm Path to help file in HTML format.
     */
    OPortType(const T_ART_Type dtyp, const int dlen, const string& name, const string sds="", const string lds="", const string htm="");

    /**
     * @brief Initializes a past time sample of the port with a given value.
     * @param[in] expr A parser expression assigning a value, e.g. "x[-1] = 0".
     */
    virtual void initPortValue(const string& expr) const;
    /**
     * @brief Initializes a past time sample of the port with a given value.
     * @param[in] value A real number which will be assigned to the given index.
     * @param[in] idx The time index of the port which will be initialized.
     */
    virtual void initPortValue(double value, int idx) const;
    /**
     * @brief Initializes a past time sample of the port with a given value.
     * @param[in] value A comples number which will be assigned to the given
     *            index.
     * @param[in] idx The time index of the port which will be initialized.
     */
    virtual void initPortValue(std::complex<double>& value, int idx) const;

    /**
     * @brief Destructor of the OPortType class.
     */
    virtual ~OPortType();
  };

  /**
   * @brief This class represents an input function or lookup table.
   * @details It is mainly used as inputs for simulations. For example, the
   *          transfer function of an instrument can be saved in the function
   *          port of a module. Another module with a convolution function may
   *          just refer to two output ports from which one is a function port
   *          saving the transfer function. It has the same interface as the
   *          OPortType class, but uses an array instead of the resizable ring
   *          buffer in order to save the values. A function port is usually
   *          only initialized before the simulation starts and not changed
   *          later.
   */
  class FPortType : public OPortType
  {
  public:

    /**
     * @brief Constructor of the FPortType class.
     * @param[in] dlen Defines the size/array length of the port.
     * @param[in] name Represents the locally unique identifier of the port.
     * @param[in] sds Short description (single line) of the port.
     * @param[in] lds Long description of the port.
     * @param[in] htm Path to help file in HTML format.
     */
    FPortType(const int dlen, const string& name, const string sds="", const string lds="", const string htm="");


    /**
     * @brief Returns the value saved at the given index.
     * @details Overrides the same method of the DataContainer class as it
     *          internally uses an array instead of a ring buffer to save all
     *          elements.
     * @param[in] idx The index of the element which shall be returned.
     */
    virtual IValue& operator[](std::size_t idx);
    /**
     * @brief Returns the value saved at the given index.
     * @details Overrides the same method of the DataContainer class as it
     *          internally uses an array instead of a ring buffer to save all
     *          elements.
     * @param[in] idx The index of the element which shall be returned.
     */
    virtual IValue& operator[](int idx);
    /**
     * @brief Destructor of the FPortType class.
     */
    virtual ~FPortType() {}
  };

  /**
   * This class represents all input ports of time modules and actually refers
   * to an existing output/function port. An input port can only point to a
   * single output port whereas an output port can be referenced by multiple
   * input ports.
   */
  class IPortType : public PortType
  {
  protected:
    /** Pointer to the existing output port. */
    const OPortType* refPort_;
  public:

    /**
     * @brief Constructor of the IPortType class.
     * @param[in] name Identifier of the input port.
     * @param[in] refPort Output or function port which will be referenced by
     *            the current input port.
     */
    IPortType(const string& name, const OPortType* refPort);
    /**
     * @brief Returns the variable object of the referenced output port.
     */
    virtual const Variable& GetParserVar();
    /**
     * @brief Returns the constant variable object of the referenced output
     * port.
     */
    virtual const Variable& GetParserVar() const;
    /**
     * @brief Destructor of the IPortType class.
     */
    virtual ~IPortType() {}
  };

  public:

  /**
   * @brief Constructor of the ITimeModule class.
   * @param[in] name Represents the unique identifier of the time module.
   * @param[in] sds Short description (single line) of the time module.
   * @param[in] lds Long description of the time module.
   * @param[in] htm Path to help file in HTML format.
   */
  explicit ITimeModule(const string& name, const string& sds="", const string& lds="", const string& htm="") :
  ARTobject(name, sds, lds, htm), _simulator(NULL)
  {
    // all time modules exist in the time domain
    AppendMethod("TimeDomain", "simulation in time domain");
  }

  /**
   * @brief Copy constructor which creates a new time module based on the given
   *        time module.
   * @param[in] orig The original time module to copy.
   */
  ITimeModule(const ITimeModule& orig) :
    ARTobject(orig), _simulator(orig._simulator)
  {}

  /**
   * @brief Factory function which creates a new interface time module object
   *        based on the current child class implementation.
   * @param[in] name Represents the unique identifier of the time module.
   * @param[in] sds Short description (single line) of the time module.
   * @param[in] lds Long description of the time module.
   * @param[in] htm Path to help file in HTML format.
   */
  virtual ITimeModule* Create(const string& name, const string& sds="", const string& lds="", const string& htm="") = 0;

  /**
   * @brief Adds a new input port to the current time module.
   * @param[in] name Locally unique identifier of the new input port.
   * @param[in] refPort Pointer to an existing output port which will be
   *            associated with the new input port.
   * @exception ARTerror If the given name already exists for another object in
   *            the current time module.
   */
  virtual void addIPort(const string& name, const DataProp* refPort) = 0;
  /**
   * @brief Returns a pointer to the port object with the given name.
   * @exception ARTerror If no port with the given name could be found in the
   *            current module.
   */
  virtual DataProp* getPort(const string& name);

  /**
   * @brief Sets the definition of the local parameter with the given name.
   * @param[in] name Name of the parameter.
   * @param[in] expr The new evaluation expression of the local parameter.
   * @exception If no local parameter with the given name could be found within
   *            the current time module.
   */
  virtual void setLocalParameter(const string& name, const string& expr);
  /**
   * @brief Sets the definition of the local parameter with the given name.
   * @param[in] name Name of the parameter.
   * @param[in] val The new complex value of the local parameter.
   * @exception If no local parameter with the given name could be found within
   *            the current time module.
   */
  virtual void setLocalParameter(const string& name, const std::complex<double>& val);
  /**
   * @brief Sets the definition of the local parameter with the given name.
   * @param[in] name Name of the parameter.
   * @param[in] val The new complex value of the local parameter.
   * @exception If no local parameter with the given name could be found within
   *            the current time module.
   */
  virtual void setLocalParameter(const string& name, const double val);

  /**
   * @brief Adds a global parameter to the current time module by registering it
   *        to all output ports and local parameters.
   * @param[in] parameter Pointer to the global parameter object.
   * @exception ARTerror If the given object is no valid global parameter or
   *            there is another property with the same name.
   */
  virtual void addGlobalParameter(const DataProp* parameter);
  /**
   * @brief Removes the definition of the given global parameter from all
   *        output ports and local parameters.
   * @param[in] name The name of the global parameter.
   * @exception ARTerror In case no global parameter with the given name could
   *            be found in the current time module.
   */
  virtual void removeGlobalParameter(const string& name);

  /**
   * @brief Sets the simulator of the current time module and all its output
   * ports.
   * @param[in] sim A pointer to a valid ARTtimeSimulator object.
   */
  virtual void setSimulator(ART::Simulator* sim);
  /**
   * @brief Sets the current time module and all output ports to the given time
   *        index.
   * @param[in] idx The time index which will be set.
   */
  virtual void setCurrentIndex(int idx) = 0;
  /**
   * @brief Triggers the evaluation of all output ports of the time module for
   *        the given time index.
   * @param[in] idx The time index which will be simulated.
   */
  virtual void simulateCurrentIndex(int idx) = 0;

  /**
   * @brief Destructor of the ITimeModule class.
   */
  virtual ~ITimeModule() {}

  protected:

  /** Simulator object to which the current time module belongs. */
  ART::TimeSimulator* _simulator;
};
}
#endif /* ARTITIMEMODULE_H_ */
