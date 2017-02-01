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

#include "ITimeModule.h"

/**
 * @brief Represents a time module with a single function output port which is
 *        used as lookup table.
 * @details The main difference to other time modules is that the value of the
 * output ports are predefined and not simulated. This module is perfectly
 * feasible to save transfer functions of instruments which will later be input
 * for a convolution function.
 * @see ART::ITimeModule::FPortType
 */
class inputFunctionModule : public ART::ITimeModule
{
protected:
  /** Pointer to the output function port.*/
  FPortType* out_;
public:

  /**
   * @brief Constructor of the inputFunctionModule class.
   * @param[in] name Represents the unique identifier of the input function
   *            module.
   * @param[in] sds Short description (single line) of the input function
   *            module.
   * @param[in] lds Long description of the input function module.
   * @param[in] htm Path to help file in HTML format.
   */
  explicit inputFunctionModule(const string& name="InputFunctionModule", const string& sds="", const string& lds="", const string& htm="");

  /**
   * @brief Factory function which creates a new function time module object
   *        with the given input parameters.
   * @param[in] name Represents the unique identifier of the input function
   *            module.
   * @param[in] sds Short description (single line) of the input function
   *            module.
   * @param[in] lds Long description of the input function module.
   * @param[in] htm Path to help file in HTML format.
   */
  virtual ART::ITimeModule* Create(const string& name, const string& sds="", const string& lds="", const string& htm="");


  /**
   * @copydoc ART::ITimeModule::addIPort()
   */
  virtual void addIPort(const string& name, const ART::DataProp* refPort);
  /**
   * @brief Creates a new function output port with the given length and the
   *        initial expression.
   * @param[in] len The length of the function port. The length will not be
   *            adapted during the simulation.
   * @param[in] expr An initial evaluation expression for the function port.
   * @exception ARTerror If the given length is less or equal zero.
   */
  virtual void defineOPort(int len, const string& expr);
  /**
   * @brief Returns a pointer to the port object with the given name.
   * @details The input function module only has a single valid name for an
   *          output port which is called "out". Trying to get any other
   *          property with another name will trigger an exception.
   * @exception ARTerror If no port with the given name could be found in the
   *            current module.
   */
  virtual ART::DataProp* getPort(const string& name);

  /**
   * @brief This method does not do anything as this module does not evaluate
   *        any expressions during the simulation.
   */
  virtual void setCurrentIndex(int idx) {}
  /**
   * @brief This method does not do anything as this module does not evaluate
   *        any expressions during the simulation.
   */
  virtual void simulateCurrentIndex(int idx) {}

  /**
   * @brief Destructor of the inptFunctionModule class.
   */
  virtual ~inputFunctionModule();
};

/**
 * @brief This module creates a Dirac impulse and has a single output port.
 * @details The default amplitude value of the impulse is "1", but can be
 *          manually set via the local parameter "A".
 */
class impulseModule : public ART::ITimeModule
{
protected:
  /** Pointer to the single output port of the module. */
  OPortType* out_;
public:
  /**
   * @brief Constructor of the impulseModule class.
   * @param[in] name Represents the unique identifier of the impulse module.
   * @param[in] sds Short description (single line) of the impulse module.
   * @param[in] lds Long description of the impulse module.
   * @param[in] htm Path to help file in HTML format.
   */
  explicit impulseModule(const string& name = "ImpulseModule", const string& sds="", const string& lds="", const string& htm="");
  /**
   * @brief Factory function which creates a new impulse module object with the
   *        given input parameters.
   * @param[in] name Represents the unique identifier of the impulse module.
   * @param[in] sds Short description (single line) of the impulse module.
   * @param[in] lds Long description of the impulse module.
   * @param[in] htm Path to help file in HTML format.
   */
  virtual ART::ITimeModule* Create(const string& name, const string& sds="", const string& lds="", const string& htm="");

  /**
   * @brief Always throws an ARTerror exception as this module does not allow
   *        to add any input ports.
   * @param[in] name Locally unique identifier of the new input port - will
   *            be ignored.
   * @param[in] refPort Pointer to an existing output port which will be
   *            associated with the new input port - will be ignored.
   */
  virtual void addIPort(const string& name, const ART::DataProp* refPort);
  /**
   * @brief Returns a pointer to the port object with the given name.
   * @details The impulse module only has a single valid name for an output
   *          port which is called "out". Trying to get any other property with
   *          another name will trigger an exception.
   * @exception ARTerror If no port with the given name could be found in the
   *            current module.
   */
  virtual ART::DataProp* getPort(const string& name);

  /**
   * @copydoc ART::ITimeModule::setCurrentIndex()
   */
  virtual void setCurrentIndex(int idx);
  /**
   * @copydoc ART::ITimeModule::simulateCurrentIndex()
   */
  virtual void simulateCurrentIndex(int idx);

  /**
   * @brief Destructor of the impulseModule class.
   */
  virtual ~impulseModule() {}
protected:
  /**
   * @brief Internal method which creates and initializes all needed local
   *        parameters.
   */
  virtual void initLocalParams();
};

/**
 * @brief This module implements the Heaviside step function and only has a
 *        single output port.
 * @details Like the impulseModule, the amplitude of the step function can be
 *          set via the local parameter "A".
 */
class heavisideModule : public ART::ITimeModule
{
protected:
  /** Pointer to the single output port of the module. */
  OPortType* out_;
public:
  /**
   * @brief Constructor of the heavisideModule class.
   * @param[in] name Represents the unique identifier of the Heaviside module.
   * @param[in] sds Short description (single line) of the Heaviside module.
   * @param[in] lds Long description of the Heaviside module.
   * @param[in] htm Path to help file in HTML format.
   */
  explicit heavisideModule(const string& name = "HeavisideModule", const string& sds="", const string& lds="", const string& htm="");
  /**
   * @brief Factory function which creates a new Heaviside module object with
   *        the given input parameters.
   * @param[in] name Represents the unique identifier of the Heaviside module.
   * @param[in] sds Short description (single line) of the Heaviside module.
   * @param[in] lds Long description of the Heaviside module.
   * @param[in] htm Path to help file in HTML format.
   */
  virtual ART::ITimeModule* Create(const string& name, const string& sds="", const string& lds="", const string& htm="");

  /**
   * @brief Always throws an ARTerror exception as this module does not allow
   *        to add any input ports.
   * @param[in] name Locally unique identifier of the new input port - will
   *            be ignored.
   * @param[in] refPort Pointer to an existing output port which will be
   *            associated with the new input port - will be ignored.
   */
  virtual void addIPort(const string& name, const ART::DataProp* refPort);
  /**
   * @brief Returns a pointer to the port object with the given name.
   * @details The Heaviside module only has a single valid name for an output
   *          port which is called "out". Trying to get any other property with
   *          another name will trigger an exception.
   * @exception ARTerror If no port with the given name could be found in the
   *            current module.
   */
  virtual ART::DataProp* getPort(const string& name);

  /**
   * @copydoc ART::ITimeModule::setCurrentIndex()
   */
  virtual void setCurrentIndex(int idx);
  /**
   * @copydoc ART::ITimeModule::simulateCurrentIndex()
   */
  virtual void simulateCurrentIndex(int idx);

  /**
   * @brief Destructor of the heavisideModule.
   */
  virtual ~heavisideModule() {}
protected:
  /**
   * @brief Internal method which creates and initializes all needed local
   *        parameters.
   */
  virtual void initLocalParams();
};

/**
 * @brief Implements a module creating a rectangular signal which can be further
 *        processed by other time modules.
 * @details Three local parameters can be set:
 *   - <b> \c S </b> defines the starting time of the rectangular function in
 *     seconds. Defaults to "0".
 *   - <b> \c E </b> defines the end time of the rectangular function in
 *     seconds. Defaults to "1".
 *   - <b> \c A </b> defines the amplitude of the rectangular function. Defaults
 *     to "1".
 */
class rectangularModule : public ART::ITimeModule
{
protected:
  /** Pointer to the single output port of the module. */
  OPortType* out_;
public:
  /**
   * @brief Constructor of the rectangularModule class.
   * @param[in] name Represents the unique identifier of the rectangular module.
   * @param[in] sds Short description (single line) of the rectangular module.
   * @param[in] lds Long description of the rectangular module.
   * @param[in] htm Path to help file in HTML format.
   */
  explicit rectangularModule(const string& name = "RectangularModule", const string& sds="", const string& lds="", const string& htm="");
  /**
   * @brief Factory function which creates a new rectangular module object with
   *        the given input parameters.
   * @param[in] name Represents the unique identifier of the rectangular module.
   * @param[in] sds Short description (single line) of the rectangular module.
   * @param[in] lds Long description of the rectangular module.
   * @param[in] htm Path to help file in HTML format.
   */
  virtual ART::ITimeModule* Create(const string& name, const string& sds="", const string& lds="", const string& htm="");

  /**
   * @brief Always throws an ARTerror exception as this module does not allow
   *        to add any input ports.
   * @param[in] name Locally unique identifier of the new input port - will
   *            be ignored.
   * @param[in] refPort Pointer to an existing output port which will be
   *            associated with the new input port - will be ignored.
   */
  virtual void addIPort(const string& name, const ART::DataProp* refPort);
  /**
   * @brief Returns a pointer to the port object with the given name.
   * @details The rectangular module only has a single valid name for an output
   *          port which is called "out". Trying to get any other property with
   *          another name will trigger an exception.
   * @exception ARTerror If no port with the given name could be found in the
   *            current module.
   */
  virtual ART::DataProp* getPort(const string& name);

  /**
   * @copydoc ART::ITimeModule::setCurrentIndex()
   */
  virtual void setCurrentIndex(int idx);
  /**
   * @copydoc ART::ITimeModule::simulateCurrentIndex()
   */
  virtual void simulateCurrentIndex(int idx);

  /**
   * @brief Destructor of the rectangularModule class.
   */
  virtual ~rectangularModule() {}
protected:
  /**
   * @brief Internal method which creates and initializes all needed local
   *        parameters.
   */
  virtual void initLocalParams();
};

/**
 * @brief Implements a simple module which amplifies the signal connected to the
 *        input port with a given amplification factor.
 * @details The class has a single input port called "in" and a single output
 *          port called "out". The local parameter "A" defines the amplification
 *          factor and can be set via the setLocalParameter() methods.
 */
class amplificationModule : public ART::ITimeModule
{
protected:
  /** Pointer to the single output port of the module. */
  OPortType* out_;
  /** Pointer to the single input port of the module. */
  IPortType* in_;
public:
  /**
   * @brief Constructor of the amplificationModule class.
   * @param[in] name Represents the unique identifier of the amplification
   *            module.
   * @param[in] sds Short description (single line) of the amplification module.
   * @param[in] lds Long description of the amplification module.
   * @param[in] htm Path to help file in HTML format.
   */
  explicit amplificationModule(const string& name = "AmplificationModule", const string& sds="", const string& lds="", const string& htm="");
  /**
   * @brief Factory function which creates a new amplification module object
   *        with the given input parameters.
   * @param[in] name Represents the unique identifier of the amplification
   *            module.
   * @param[in] sds Short description (single line) of the amplification module.
   * @param[in] lds Long description of the amplification module.
   * @param[in] htm Path to help file in HTML format.
   */
  virtual ART::ITimeModule* Create(const string& name, const string& sds="", const string& lds="", const string& htm="");

  /**
   * @brief Adds a new input port to the current time module.
   * @param[in] name Locally unique identifier of the new input port. Always has
   *            to be "in" for the amplification module.
   * @param[in] refPort Pointer to an existing output port which will be
   *            associated with the new input port.
   * @exception ARTerror If the given name is different to "in" or the provided
   *            port is no valid output port object.
   */
  virtual void addIPort(const string& name, const ART::DataProp* refPort);
  /**
   * @brief Returns a pointer to the port object with the given name.
   * @details The amplification module only has a single valid name for an
   *          output port which is called "out". Trying to get any other
   *          property with another name will trigger an exception.
   * @exception ARTerror If no port with the given name could be found in the
   *            current module.
   */
  virtual ART::DataProp* getPort(const string& name);

  /**
   * @copydoc ART::ITimeModule::setCurrentIndex()
   */
  virtual void setCurrentIndex(int idx);
  /**
   * @copydoc ART::ITimeModule::simulateCurrentIndex()
   */
  virtual void simulateCurrentIndex(int idx);

  /**
   * @brief Destructor of the amplificationModule class.
   */
  virtual ~amplificationModule() {}
protected:
  /**
   * @brief Internal method which creates and initializes all needed local
   *        parameters.
   */
  virtual void initLocalParams();
};

/**
 * @brief This module delays the input signal by the given delay in seconds, but
 *        rounds to the next closes sample.
 * @details The local parameter "Delay" specifies the delay in seconds and can
 *          be set via the setLocalParameter() functions. The module has a
 *          single input port called "in" and a single output port called "out".
 *          This module should only be used if the delay is either an exact
 *          multiple of the sampling period or precision does not matter.
 *          Otherwise, it is highly recommended to use the
 *          fractionalDelayModule.
 */
class simpleDelayModule : public ART::ITimeModule
{
protected:
  /** Pointer to the single output port of the module. */
  OPortType* out_;
  /** Pointer to the single input port of the module. */
  IPortType* in_;
public:
  /**
   * @brief Constructor of the simpleDelayModule class.
   * @param[in] name Represents the unique identifier of the simple delay
   *            module.
   * @param[in] sds Short description (single line) of the simple delay module.
   * @param[in] lds Long description of the simple delay module.
   * @param[in] htm Path to help file in HTML format.
   */
  explicit simpleDelayModule(const string& name = "SimpleDelayModule", const string& sds="", const string& lds="", const string& htm="");
  /**
   * @brief Factory function which creates a new simple delay module object
   *        with the given input parameters.
   * @param[in] name Represents the unique identifier of the simple delay module.
   * @param[in] sds Short description (single line) of the simple delay module.
   * @param[in] lds Long description of the simple delay module.
   * @param[in] htm Path to help file in HTML format.
   */
  virtual ART::ITimeModule* Create(const string& name, const string& sds="", const string& lds="", const string& htm="");

  /**
   * @brief Adds a new input port to the current time module.
   * @param[in] name Locally unique identifier of the new input port. Always has
   *            to be "in" for the simple delay module.
   * @param[in] refPort Pointer to an existing output port which will be
   *            associated with the new input port.
   * @exception ARTerror If the given name is different to "in" or the provided
   *            port is no valid output port object.
   */
  virtual void addIPort(const string& name, const ART::DataProp* refPort);
  /**
   * @brief Returns a pointer to the port object with the given name.
   * @details The simple delay module only has a single valid name for an output
   *          port which is called "out". Trying to get any other property with
   *          another name will trigger an exception.
   * @exception ARTerror If no port with the given name could be found in the
   *            current module.
   */
  virtual ART::DataProp* getPort(const string& name);

  /**
   * @copydoc ART::ITimeModule::setCurrentIndex()
   */
  virtual void setCurrentIndex(int idx);
  /**
   * @copydoc ART::ITimeModule::simulateCurrentIndex()
   */
  virtual void simulateCurrentIndex(int idx);

  /**
   * @brief Destructor of the simpleDelayModule class.
   */
  virtual ~simpleDelayModule() {}
protected:
  /**
   * @brief Internal method which creates and initializes all needed local
   *        parameters.
   */
  virtual void initLocalParams();
};

/**
 * @brief Implements a module which simply adds two input signals and presents
 *        the result at its single output port.
 * @details There are no local parameters which can be set. The only requirement
 *          before simulating is to connect the input signals to either input
 *          port "in1" or "in2".
 */
class addModule : public ART::ITimeModule
{
protected:
  /** Pointer to the single output port of the module. */
  OPortType* out_;
public:
  /**
   * @brief Constructor of the addModule class.
   * @param[in] name Represents the unique identifier of the add module.
   * @param[in] sds Short description (single line) of the add module.
   * @param[in] lds Long description of the add module.
   * @param[in] htm Path to help file in HTML format.
   */
  explicit addModule(const string& name = "AddModule", const string& sds="", const string& lds="", const string& htm="");
  /**
   * @brief Factory function which creates a new add module object with the
   *        given input parameters.
   * @param[in] name Represents the unique identifier of the add module.
   * @param[in] sds Short description (single line) of the add module.
   * @param[in] lds Long description of the add module.
   * @param[in] htm Path to help file in HTML format.
   */
  virtual ART::ITimeModule* Create(const string& name, const string& sds="", const string& lds="", const string& htm="");

  /**
   * @brief Adds a new input port to the current time module.
   * @param[in] name Locally unique identifier of the new input port. Has either
   *            to be "in1" or "in2" for the add module.
   * @param[in] refPort Pointer to an existing output port which will be
   *            associated with the new input port.
   * @exception ARTerror If the given name is different to "in1" or "in2" or the
   *            provided port is no valid output port object.
   */
  virtual void addIPort(const string& name, const ART::DataProp* refPort);
  /**
   * @brief Returns a pointer to the port object with the given name.
   * @details The add module only has a single valid name for an output
   *          port which is called "out". Trying to get any other property with
   *          another name will trigger an exception.
   * @exception ARTerror If no port with the given name could be found in the
   *            current module.
   */
  virtual ART::DataProp* getPort(const string& name);

  /**
   * @copydoc ART::ITimeModule::setCurrentIndex()
   */
  virtual void setCurrentIndex(int idx);
  /**
   * @copydoc ART::ITimeModule::simulateCurrentIndex()
   */
  virtual void simulateCurrentIndex(int idx);

  /**
   * @brief Destructor of the addModule class.
   */
  virtual ~addModule() {}
};

/**
 * @brief Implements a module which simply multiplies two input signals and
 *        presents the result at its single output port.
 * @details There are no local parameters which can be set. The only requirement
 *          before simulating is to connect the input signals to either input
 *          port "in1" or "in2".
 */
class multiplicationModule : public ART::ITimeModule
{
protected:
  /** Pointer to the single output port of the module. */
  OPortType* out_;
public:
  /**
   * @brief Constructor of the multiplicationModule class.
   * @param[in] name Represents the unique identifier of the multiplication
   *            module.
   * @param[in] sds Short description (single line) of the multiplication
   *            module.
   * @param[in] lds Long description of the multiplication module.
   * @param[in] htm Path to help file in HTML format.
   */
  explicit multiplicationModule(const string& name = "MultiplicationModule", const string& sds="", const string& lds="", const string& htm="");
  /**
   * @brief Factory function which creates a new multiplication module object
   *        with the given input parameters.
   * @param[in] name Represents the unique identifier of the multiplication
   *            module.
   * @param[in] sds Short description (single line) of the multiplication
   *            module.
   * @param[in] lds Long description of the multiplication module.
   * @param[in] htm Path to help file in HTML format.
   */
  virtual ART::ITimeModule* Create(const string& name, const string& sds="", const string& lds="", const string& htm="");

  /**
   * @brief Adds a new input port to the current time module.
   * @param[in] name Locally unique identifier of the new input port. Has either
   *            to be "in1" or "in2" for the multiplication module.
   * @param[in] refPort Pointer to an existing output port which will be
   *            associated with the new input port.
   * @exception ARTerror If the given name is different to "in1" or "in2" or the
   *            provided port is no valid output port object.
   */
  virtual void addIPort(const string& name, const ART::DataProp* refPort);
  /**
   * @brief Returns a pointer to the port object with the given name.
   * @details The multiplication module only has a single valid name for an
   *          output port which is called "out". Trying to get any other
   *          property with another name will trigger an exception.
   * @exception ARTerror If no port with the given name could be found in the
   *            current module.
   */
  virtual ART::DataProp* getPort(const string& name);

  /**
   * @copydoc ART::ITimeModule::setCurrentIndex()
   */
  virtual void setCurrentIndex(int idx);
  /**
   * @copydoc ART::ITimeModule::simulateCurrentIndex()
   */
  virtual void simulateCurrentIndex(int idx);

  /**
   * @brief Destructor of the multiplicationModule class.
   */
  virtual ~multiplicationModule() {}
};

/**
 * @brief Implements a module creating a sine wave signal which can be further
 *        processed by other time modules.
 * @details Three local parameters can be set:
 *   - <b> \c A </b> defines the amplitude of the sine wave. Defaults to "1".
 *   - <b> \c f </b> defines the frequency of the sine wave in Hz. Defaults
 *     to "10".
 *   - <b> \c Delta </b> defines the phase (delay) of the sine wave in seconds.
 *     Defaults to "0".
 */
class sinewaveModule : public ART::ITimeModule
{
protected:
  /** Pointer to the single output port of the module. */
  OPortType* out_;
public:
  /**
   * @brief Constructor of the sinewaveModule class.
   * @param[in] name Represents the unique identifier of the sine wave module.
   * @param[in] sds Short description (single line) of the sine wave module.
   * @param[in] lds Long description of the sine wave module.
   * @param[in] htm Path to help file in HTML format.
   */
  explicit sinewaveModule(const string& name = "SinewaveModule", const string& sds="", const string& lds="", const string& htm="");
  /**
   * @brief Factory function which creates a new sine wave module object
   *        with the given input parameters.
   * @param[in] name Represents the unique identifier of the sine wave module.
   * @param[in] sds Short description (single line) of the sine wave module.
   * @param[in] lds Long description of the sine wave module.
   * @param[in] htm Path to help file in HTML format.
   */
  virtual ART::ITimeModule* Create(const string& name, const string& sds="", const string& lds="", const string& htm="");

  /**
   * @brief Always throws an ARTerror exception as this module does not allow
   *        to add any input ports.
   * @param[in] name Locally unique identifier of the new input port - will
   *            be ignored.
   * @param[in] refPort Pointer to an existing output port which will be
   *            associated with the new input port - will be ignored.
   */
  virtual void addIPort(const string& name, const ART::DataProp* refPort);
  /**
   * @brief Returns a pointer to the port object with the given name.
   * @details The sine wave module only has a single valid name for an
   *          output port which is called "out". Trying to get any other
   *          property with another name will trigger an exception.
   * @exception ARTerror If no port with the given name could be found in the
   *            current module.
   */
  virtual ART::DataProp* getPort(const string& name);

  /**
   * @copydoc ART::ITimeModule::setCurrentIndex()
   */
  virtual void setCurrentIndex(int idx);
  /**
   * @copydoc ART::ITimeModule::simulateCurrentIndex()
   */
  virtual void simulateCurrentIndex(int idx);

  /**
   * @brief Destructor of the multiplicationModule class.
   */
  virtual ~sinewaveModule() {}
protected:
  /**
   * @brief Internal method which creates and initializes all needed local
   *        parameters.
   */
  virtual void initLocalParams();
};

/**
 * @brief Abstract class which provides internal methods which can be re-used
 *        by all modules requiring fractional delay filters.
 * @details This class cannot be instantiated and only provides constructor,
 *          destructor and internal functions to calculate parameters for
 *          Lagrange FIR and Thiran IIR filters.
 */
class genericDelayModule : public ART::ITimeModule
{
public:
  /**
   * @brief Constructor of the genericDelayModule class.
   * @param[in] name Represents the unique identifier of the generic delay
   *            module.
   * @param[in] sds Short description (single line) of the generic delay module.
   * @param[in] lds Long description of the generic delay module.
   * @param[in] htm Path to help file in HTML format.
   */
  explicit genericDelayModule(const string& name, const string& sds="", const string& lds="", const string& htm="") :
  ART::ITimeModule(name, sds, lds, htm) {}
  /**
   * @brief Destructor of the genericDelayModule class.
   */
  ~genericDelayModule() {}
protected:

  /**
   * @brief Calculates the Lagrange filter parameters for the given input
   *        values.
   * @param[in] n Defines the position of the filter parameter. 0 <= n <= N.
   * @param[in] N Defines the order of the filter.
   * @param[in] D Defines the delay in samples (not in seconds).
   */
  virtual double getLagrangeParams(int n, int N, double D);
  /**
   * @brief Calculates the Thiran filter parameters for the given input
   *        values.
   * @param[in] n Defines the position of the filter parameter. 0 <= n <= N.
   * @param[in] N Defines the order of the filter.
   * @param[in] D Defines the delay in samples (not in seconds).
   */
  virtual double getThiranParams(int n, int N, double D);

  /**
   * @brief Returns the faculty (n!) of the given number.
   * @param[in] n The integer for which the faculty will be calculated.
   * @note For invalid input, i.e., n < 0, the function will return 1.
   */
  virtual double fac(int n);
  /**
   * @brief Returns the binomial coefficient for "n choose k".
   * @param[in] n Factor "n" of the binomial coefficient.
   * @param[in] k Factor "k" of the binomial coefficient.
   * @note The function does not do any boundary checks, i.e., whether k <= n.
   *       This may cause useless results of the function although no exception
   *       will be thrown.
   */
  virtual double binom(int n, int k);
};

/**
 * @brief This module delays the input signal by the given delay in seconds by
 *        using FIR or IIR filters.
 * @details You can specify the filter type, the filter order and the delay of
 *          the signal. Bear in mind that these filters may not be stable
 *          depending on the given parameters. Recommended boundaries for Delay
 *          D (in samples) and filter order N:
 *   - (N/2 + 1) <= D <= (N/2 - 1) for Lagrange filters with even order,
 *   - (N-1)/2 <= D <= (N+1)/2 for Lagrange filters with odd order and
 *   - N - 0.5 <= D <= N + 0.5 for Thiran filters.
 *
 * Three local parameters can be set:
 *   - <b> \c type </b> defines the type of the filter. "lagrange" results in
 *     and FIR, "thiran" in an IIR filter. Defaults to "lagrange".
 *   - <b> \c order </b> defines the order of the filter. Defaults to "5".
 *   - <b> \c Delay </b> defines the delay of the signal in seconds. Defaults
 *     to "0".
 */
class fractionalDelayModule : public genericDelayModule
{
protected:
  /** Pointer to the single output port of the module. */
  OPortType* out_;
  /** Pointer to the single input port of the module. */
  IPortType* in_;
public:
  /**
   * @brief Constructor of the fractionalDelayModule class.
   * @param[in] name Represents the unique identifier of the fractional delay
   *            module.
   * @param[in] sds Short description (single line) of the fractional delay
   *            module.
   * @param[in] lds Long description of the fractional delay module.
   * @param[in] htm Path to help file in HTML format.
   */
  explicit fractionalDelayModule(const string& name="FractionalDelayModule", const string& sds="", const string& lds="", const string& htm="");
  /**
   * @brief Factory function which creates a new fractional delay module object
   *        with the given input parameters.
   * @param[in] name Represents the unique identifier of the fractional delay
   *            module.
   * @param[in] sds Short description (single line) of the fractional delay
   *            module.
   * @param[in] lds Long description of the fractional delay module.
   * @param[in] htm Path to help file in HTML format.
   */
  virtual ART::ITimeModule* Create(const string& name, const string& sds="", const string& lds="", const string& htm="");

  /**
   * @brief Adds a new input port to the current time module.
   * @param[in] name Locally unique identifier of the new input port. Always has
   *            to be "in" for the fraction delay module.
   * @param[in] refPort Pointer to an existing output port which will be
   *            associated with the new input port.
   * @exception ARTerror If the given name is different to "in" or the provided
   *            port is no valid output port object.
   */
  virtual void addIPort(const string& name, const ART::DataProp* refPort);
  /**
   * @brief Returns a pointer to the port object with the given name.
   * @details The fractional delay module only has a single valid name for an
   *          output port which is called "out". Trying to get any other
   *          property with another name will trigger an exception.
   * @exception ARTerror If no port with the given name could be found in the
   *            current module.
   */
  virtual ART::DataProp* getPort(const string& name);

  /**
   * @copydoc ART::ITimeModule::setCurrentIndex()
   */
  virtual void setCurrentIndex(int idx);
  /**
   * @copydoc ART::ITimeModule::simulateCurrentIndex()
   */
  virtual void simulateCurrentIndex(int idx);

  /**
   * @brief Destructor of the fractionalDelayModule.
   */
  virtual ~fractionalDelayModule() {}
protected:
  /**
   * @brief Internal method which creates and initializes all needed local
   *        parameters.
   */
  virtual void initLocalParams();
  /**
   * @brief Internal method which prepares the filter function based on the
   *        given filter type and delay by referring to the global sampling
   *        period parameter "T".
   * @exception ARTerror If the defined filter type does neither match
   *            "lagrange" nor "thiran".
   */
  virtual void initSimulation();

};

/**
 * @brief Implements a digital waveguide module for cylinders.
 * @details The module has two input and two output ports:
 *   - <b> \c p1p </b> Input port for the forwards traveling wave.
 *   - <b> \c p2p </b> Output port for the forwards traveling wave.
 *   - <b> \c p1m </b> Input port for the backwards traveling wave.
 *   - <b> \c p2m </b> Input port for the backwards traveling wave.
 *
 * The user can specify the following local parameters:
 *   - <b> \c type </b> defines the type of the fractional delay filter and can
 *     either be "lagrange" or "thiran". Defaults to "lagrange".
 *   - <b> \c length </b> defines the length of the cylinder in m. Defaults to
 *     0.05, i.e. 50 mm.
 *
 * The main advantage of using this module instead of the fractionalDelayModule
 * if the fact that every parameter will be defined automatically - there is no
 * need the define the filter order manually and check boundary conditions.
 */
class DWGcylinderModule : public genericDelayModule
{
protected:
  /** Forwards traveling wave left input port. */
  IPortType* p1p_;
  /** Forwards traveling wave right output port. */
  OPortType* p2p_;
  /** Backwards traveling wave left output port. */
  OPortType* p1m_;
  /** Backwards traveling wave right input port. */
  IPortType* p2m_;

  /** Internal buffer for time delays - forwards traveling wave. */
  OPortType* p1pbuf_;
  /** Internal buffer for time delays - backwards traveling wave. */
  OPortType* p2mbuf_;

public:
  /**
   * @brief Constructor of the DWGcylinderModule class.
   * @param[in] name Represents the unique identifier of the DWG cylinder
   *            module.
   * @param[in] sds Short description (single line) of the DWG cylinder
   *            module.
   * @param[in] lds Long description of the DWG cylinder module.
   * @param[in] htm Path to help file in HTML format.
   */
  explicit DWGcylinderModule(const string& name="DWGCylinderModule", const string& sds="", const string& lds="", const string& htm="");
  /**
   * @brief Factory function which creates a new DWG cylinder module object
   *        with the given input parameters.
   * @param[in] name Represents the unique identifier of the DWG cylinder
   *            module.
   * @param[in] sds Short description (single line) of the DWG cylinder
   *            module.
   * @param[in] lds Long description of the DWG cylinder module.
   * @param[in] htm Path to help file in HTML format.
   */
  virtual ART::ITimeModule* Create(const string& name, const string& sds="", const string& lds="", const string& htm="");

  /**
   * @brief Adds a new input port to the current time module.
   * @param[in] name Locally unique identifier of the new input port. Either has
   *            to be "p1p" or "p2m" for the DWG cylinder module.
   * @param[in] refPort Pointer to an existing output port which will be
   *            associated with the new input port.
   * @exception ARTerror If the given name is different to "p1p" or "p2m" or the
   *            provided port is no valid output port object.
   */
  virtual void addIPort(const string& name, const ART::DataProp* refPort);
  /**
   * @brief Returns a pointer to the port object with the given name.
   * @details The DWG cylinder module only has valid names for
   *          output ports which are called "p2p" or "p1m". Trying to get any
   *          other property with another name will trigger an exception.
   * @exception ARTerror If no port with the given name could be found in the
   *            current module.
   */
  virtual ART::DataProp* getPort(const string& name);

  /**
   * @copydoc ART::ITimeModule::setCurrentIndex
   */
  virtual void setCurrentIndex(int idx);
  /**
   * @copydoc ART::ITimeModule::simulateCurrentIndex
   */
  virtual void simulateCurrentIndex(int idx);

  /**
   * @brief Destructor of the DWGcylinderModule class.
   */
  virtual ~DWGcylinderModule() {}
protected:
  /**
   * @brief Internal method which creates and initializes all needed local
   *        parameters.
   */
  virtual void initLocalParams();
  /**
   * @brief Internal method which prepares the cylinder function based on the
   *        given filter type and delay by referring to the global sampling
   *        period parameter "T".
   * @exception ARTerror If the global parameter "c" for sound velocity cannot
   *            be found and the method was neither "lagrange" nor "thiran".
   */
  virtual void initSimulation();

};

/**
 * @brief Implements a digital waveguide module for cylinder junctions.
 * @details The module has two input and two output ports:
 *   - <b> \c p1p </b> Input port for the forwards traveling wave.
 *   - <b> \c p2p </b> Output port for the forwards traveling wave.
 *   - <b> \c p1m </b> Input port for the backwards traveling wave.
 *   - <b> \c p2m </b> Input port for the backwards traveling wave.
 *
 * To provide more flexibility, the module allows the user to specify all
 * parameters in two ways: The traditional way is providing the radius of the
 * wave spheres of the left and right end of the cone. The "boreprofile" mode
 * is more flexible and lets the user define all measurements based on bore
 * profiles, i.e., the radius at the left and the right end of the cone.
 * The following local parameters can be specified:
 *
 *   - <b> \c r1 </b> Radius of the cylinder on the left side of the junction in
 *     m. Defaults to 0.01, i.e., 10 mm.
 *   - <b> \c r2 </b> Radius of the cylinder on the right side of the junction
 *     in m. Defaults to 0.015, i.e., 15 mm.
 */
class DWGcylinderJunctionModule : public ART::ITimeModule
{
protected:
  /** Forwards traveling wave left input port. */
  IPortType* p1p_;
  /** Forwards traveling wave right output port. */
  OPortType* p2p_;
  /** Backwards traveling wave left output port. */
  OPortType* p1m_;
  /** Backwards traveling wave right input port. */
  IPortType* p2m_;

  /** Pointer to the parameter representing the radius of the left cylinder. */
  localParameterType* r1_;
  /** Pointer to the parameter representing the radius of the right cylinder. */
  localParameterType* r2_;

public:
  /**
   * @brief Constructor of the DWGCylinderJunctionModule class.
   * @param[in] name Represents the unique identifier of the DWG cylinder
   *            junction module.
   * @param[in] sds Short description (single line) of the DWG cylinder junction
   *            module.
   * @param[in] lds Long description of the DWG cylinder junction module.
   * @param[in] htm Path to help file in HTML format.
   */
  explicit DWGcylinderJunctionModule(const string& name="DWGCylinderJunctionModule", const string& sds="", const string& lds="", const string& htm="");
  /**
   * @brief Factory function which creates a new DWG cylinder junction module
   *        object with the given input parameters.
   * @param[in] name Represents the unique identifier of the DWG cylinder
   *            junction module.
   * @param[in] sds Short description (single line) of the DWG cylinder junction
   *            module.
   * @param[in] lds Long description of the DWG cylinder junction module.
   * @param[in] htm Path to help file in HTML format.
   */
  virtual ART::ITimeModule* Create(const string& name, const string& sds="", const string& lds="", const string& htm="");

  /**
   * @brief Adds a new input port to the current time module.
   * @param[in] name Locally unique identifier of the new input port. Either has
   *            to be "p1p" or "p2m" for the DWG cylinder junction module.
   * @param[in] refPort Pointer to an existing output port which will be
   *            associated with the new input port.
   * @exception ARTerror If the given name is different to "p1p" or "p2m" or the
   *            provided port is no valid output port object.
   */
  virtual void addIPort(const string& name, const ART::DataProp* refPort);
  /**
   * @brief Returns a pointer to the port object with the given name.
   * @details The DWG cylinder junction module only has valid names
   *          for output ports which are called "p2p" or "p1m". Trying to get
   *          any other property with another name will trigger an exception.
   * @exception ARTerror If no port with the given name could be found in the
   *            current module.
   */
  virtual ART::DataProp* getPort(const string& name);

  /**
   * @copydoc ART::ITimeModule::setCurrentIndex
   */
  virtual void setCurrentIndex(int idx);
  /**
   * @copydoc ART::ITimeModule::simulateCurrentIndex
   */
  virtual void simulateCurrentIndex(int idx);

  /**
   * @brief Destructor of the DWGcylinderJunctionModule class.
   */
  virtual ~DWGcylinderJunctionModule() {}
protected:
  /**
   * @brief Internal method which creates and initializes all needed local
   *        parameters.
   */
  virtual void initLocalParams();

};

/**
 * @brief Implements a digital waveguide module for conical bores.
 * @details The module has two input and two output ports:
 *   - <b> \c p1p </b> Input port for the forwards traveling wave.
 *   - <b> \c p2p </b> Output port for the forwards traveling wave.
 *   - <b> \c p1m </b> Input port for the backwards traveling wave.
 *   - <b> \c p2m </b> Input port for the backwards traveling wave.
 *
 * The user can specify the following local parameters:
 *   - <b> \c type </b> defines the type of the fractional delay filter and can
 *     either be "lagrange" or "thiran". Defaults to "lagrange".
 *   - <b> \c length </b> defines the length of the cone in m. Defaults to
 *     0.05, i.e. 50 mm.
 *   - <b> \c mode </b> defines the interpretation of the parameters: "default"
 *     means that r1 and r2 are interpreted as cone apex distances, whereas for
 *     "boreprofile", the cone apex distances are calculated based on the given
 *     radii of the left (r1) and right (r2) cone opening and the specified
 *     length of the cone. Defaults to "default".
 *   - <b> \c r1 </b> Radius of the left wave sphere in m in default mode or
 *     radius of the left cone ending in boreprofile mode.
 *   - <b> \c r2 </b> Radius of the right wave sphere in m in default mode or
 *     radius of the right cone ending in boreprofile mode.
 *
 */
class DWGconeModule : public DWGcylinderModule
{

public:
  /**
   * @brief Constructor of the DWGconeModule class.
   * @param[in] name Represents the unique identifier of the DWG cone module.
   * @param[in] sds Short description (single line) of the DWG cone module.
   * @param[in] lds Long description of the DWG cone module.
   * @param[in] htm Path to help file in HTML format.
   */
  explicit DWGconeModule(const string& name="DWGConeModule", const string& sds="", const string& lds="", const string& htm="");
  /**
   * @brief Factory function which creates a new DWG cone module
   *        object with the given input parameters.
   * @param[in] name Represents the unique identifier of the DWG cone module.
   * @param[in] sds Short description (single line) of the DWG cone module.
   * @param[in] lds Long description of the DWG cone module.
   * @param[in] htm Path to help file in HTML format.
   */
  virtual ART::ITimeModule* Create(const string& name, const string& sds="", const string& lds="", const string& htm="");

  /**
   * @brief Destructor of the DWGconeModule class.
   */
  virtual ~DWGconeModule() {}
protected:
  /**
   * @brief Internal method which creates and initializes all needed local
   *        parameters.
   */
  virtual void initLocalParams();
  /**
   * @brief Internal method which prepares the cone function based on the
   *        given filter type and delay by referring to the global sampling
   *        period parameter "T".
   * @exception ARTerror If the global parameter "c" for sound velocity cannot
   *            be found and the method was neither "lagrange" nor "thiran".
   */
  virtual void initSimulation();
  /**
   * @brief Calculates the cone apex radii and surface values if the parameter
   *        values are given as a bore list.
   */
  virtual void calculateConeApex();

};

/**
 * @brief Implements a digital waveguide module for conical junctions.
 * @details The module has two input and two output ports:
 *   - <b> \c p1p </b> Input port for the forwards traveling wave.
 *   - <b> \c p2p </b> Output port for the forwards traveling wave.
 *   - <b> \c p1m </b> Input port for the backwards traveling wave.
 *   - <b> \c p2m </b> Input port for the backwards traveling wave.
 *
 * The user can specify the following local parameters:
 *   - <b> \c method </b> defines the method for calculating
 *     the internal filter parameters. May be either "IIM", "BT" or "TICM".
 *     Defaults to "IIM".
 *   - <b> \c mode </b> defines the interpretation of the parameters: "default"
 *     means that r1 and r2 are interpreted as cone apex distances, whereas for
 *     "boreprofile", other parameters can be specified. Defaults to "default".
 *   - <b> \c r1 </b> Radius of the left wave sphere in m (used in default
 *     mode).
 *   - <b> \c r2 </b> Radius of the right wave sphere in m (used in default
 *     mode).
 *   - <b> \c S1 </b> Area of the left wave sphere in m (used in default mode).
 *   - <b> \c S2 </b> Area of the right wave sphere in m (used in default mode).
 *   - <b> \c lr1 </b> Radius of the left end of the left cone in m (used in
 *     boreprofile mode).
 *   - <b> \c lr2 </b> Radius of the right end of the left cone in m (used in
 *     boreprofile mode).
 *   - <b> \c llength </b> Length of the left cone in m (used in
 *     boreprofile mode).
 *   - <b> \c rr1 </b> Radius of the left end of the right cone in m (used in
 *     boreprofile mode).
 *   - <b> \c rr2 </b> Radius of the right end of the right cone in m (used in
 *     boreprofile mode)
 *   - <b> \c rlength </b> Length of the right cone in m (used in
 *     boreprofile mode).
 *
 */
class DWGconeJunctionModule : public ART::ITimeModule
{
protected:
  /** Forwards traveling wave left input port. */
  IPortType* p1p_;
  /** Forwards traveling wave right output port. */
  OPortType* p2p_;
  /** Backwards traveling wave left output port. */
  OPortType* p1m_;
  /** Backwards traveling wave right input port. */
  IPortType* p2m_;

  /** Pointer to internal signal responsible for the output calculation. */
  OPortType* rz_;

  /** Pointer to the left cone apex radius parameter. */
  localParameterType* r1_;
  /** Pointer to the right cone apex radius parameter. */
  localParameterType* r2_;
  /** Pointer to the left sphere surface parameter. */
  localParameterType* S1_;
  /** Pointer to the right sphere surface parameter. */
  localParameterType* S2_;

public:
  /**
   * @brief Constructor of the DWGconeJunctionModule class.
   * @param[in] name Represents the unique identifier of the DWG cone
   *            junction module.
   * @param[in] sds Short description (single line) of the DWG cone junction
   *            module.
   * @param[in] lds Long description of the DWG cone junction module.
   * @param[in] htm Path to help file in HTML format.
   */
  explicit DWGconeJunctionModule(const string& name="DWGConeJunctionModule", const string& sds="", const string& lds="", const string& htm="");
  /**
   * @brief Factory function which creates a new DWG cone junction module
   *        object with the given input parameters.
   * @param[in] name Represents the unique identifier of the DWG cone
   *            junction module.
   * @param[in] sds Short description (single line) of the DWG cone junction
   *            module.
   * @param[in] lds Long description of the DWG cone junction module.
   * @param[in] htm Path to help file in HTML format.
   */
  virtual ART::ITimeModule* Create(const string& name, const string& sds="", const string& lds="", const string& htm="");

  /**
   * @brief Adds a new input port to the current time module.
   * @param[in] name Locally unique identifier of the new input port. Either has
   *            to be "p1p" or "p2m" for the DWG cone junction module.
   * @param[in] refPort Pointer to an existing output port which will be
   *            associated with the new input port.
   * @exception ARTerror If the given name is different to "p1p" or "p2m" or the
   *            provided port is no valid output port object.
   */
  virtual void addIPort(const string& name, const ART::DataProp* refPort);
  /**
   * @brief Returns a pointer to the port object with the given name.
   * @details The DWG cone junction module only has a valid names for
   *          output ports which are called "p2p" or "p1m". Trying to get any
   *          other property with another name will trigger an exception.
   * @exception ARTerror If no port with the given name could be found in the
   *            current module.
   */
  virtual ART::DataProp* getPort(const string& name);

  /**
   * @copydoc ART::ITimeModule::setCurrentIndex
   */
  virtual void setCurrentIndex(int idx);
  /**
   * @copydoc ART::ITimeModule::simulateCurrentIndex
   */
  virtual void simulateCurrentIndex(int idx);

  /**
   * @brief Destructor of the DWGconeJunctionModule class.
   */
  virtual ~DWGconeJunctionModule() {}
protected:
  /**
   * @brief Internal method which creates and initializes all needed local
   *        parameters.
   */
  virtual void initLocalParams();
  /**
   * @brief Internal method which prepares the cone junction function based on
   *        the calculated filter parameters.
   */
  virtual void initSimulation();

  /**
   * @brief Calculates the cone apex radii and surface values if the parameter
   *        values are given as a bore list.
   */
  virtual void calculateConeApex();

  /**
   * @brief Calculates the b0 parameter of the filter function needed by the
   *        cone junction.
   * @param[in] method defines the method for calculating the filter parameter.
   *            May either be "IIM", "TICM" or "BT".
   * @excetion ARTerror If no global parameter "c" for the sound velocity was
   *           defined in the current simulator or the given method cannot be
   *           found.
   */
  virtual double getB0(const string& method);
  /**
   * @brief Calculates the b1 parameter of the filter function needed by the
   *        cone junction.
   * @param[in] method defines the method for calculating the filter parameter.
   *            May either be "IIM", "TICM" or "BT".
   * @excetion ARTerror If no global parameter "c" for the sound velocity was
   *           defined in the current simulator or the given method cannot be
   *           found.
   */
  virtual double getB1(const string& method);
  /**
   * @brief Calculates the a1 parameter of the filter function needed by the
   *        cone junction.
   * @param[in] method defines the method for calculating the filter parameter.
   *            May either be "IIM", "TICM" or "BT".
   * @excetion ARTerror If no global parameter "c" for the sound velocity was
   *           defined in the current simulator or the given method cannot be
   *           found.
   */
  virtual double getA1(const string& method);

};


#endif /* TIMEPROTOTYPES_H_ */
