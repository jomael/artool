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
 * TimeModule.h
 *
 *  Created on: Jul 4, 2012
 *      Author: cbg
 */

#ifndef ARTTIMEMODULE_H_
#define ARTTIMEMODULE_H_

#include <string>
#include <complex>

#include "ARTobject.h"
#include "TimeSimulator.h"
#include "ITimeModule.h"
#include "mpParser.h"

using std::string;
using namespace mup;

namespace ART{
  // forward declaration
class DataContainer;


/**
 * @brief This class is a generic time module and allows the user to create
 *        individual ports which can be connected to any other class
 *        implementing the ITimeModule interface.
 *
 * @details It implements all abstract methods of the ITimeModule class and
 *          additionally provides methods to add output ports and local
 *          parameters to the current time module. Output ports of a module can
 *          access local and global paremters as well as other output and input
 *          ports of the same time module. Output ports of other time modules
 *          can be accessed by creating an input port and referencing the output
 *          port of the other time module.
 */
class TimeModule : public ITimeModule
{
public:

  /**
   * @brief Constructor of the TimeModule class.
   * @param[in] name Represents the unique identifier of the time module.
   * @param[in] sds Short description (single line) of the time module.
   * @param[in] lds Long description of the time module.
   * @param[in] htm Path to help file in HTML format.
   */
  explicit TimeModule(const string& name="TimeModule", const string& sds="", const string& lds="", const string& htm="");


  /**
   * @brief Factory function which creates a new generic time module object
   *        with the given input parameters.
   * @param[in] name Represents the unique identifier of the time module.
   * @param[in] sds Short description (single line) of the time module.
   * @param[in] lds Long description of the time module.
   * @param[in] htm Path to help file in HTML format.
   */
  virtual ITimeModule* Create(const string& name, const string& sds="", const string& lds="", const string& htm="");


  /**
   * @copydoc ITimeModule::addIPort()
   */
  virtual void addIPort(const string& name, const DataProp* refPort);
  /**
   * @brief Creates a new output port to the
   * @details
   * @param[in] name The locally unique name of the port.
   * @param[in] expr A parser expression
   * @param[in] size Optional parameter defining the size of the ring buffer.
   *                 Default is 20.
   * @exception ARTerror If another object (parameter or port) with the same
   *            name already exists in the current time module.
   */
  virtual void addOPort(const string& name, const string& expr, unsigned int size = 20);

  /**
   * @brief Adds a new local parameter with the given name and evaluation
   *        expression to the current time module.
   * @param[in] name Locally unique identifier.
   * @param[in] expr A parser expression defining the value of the local
   *                 parameter.
   * @exception ARTerror If another object (parameter or port) with the same
   *            name already exists in the current time module.
   */
  virtual void addLocalParameter(const string& name, const string& expr);
  /**
   * @brief Adds a new local parameter with the given name and initial value
   *        to the current time module.
   * @param[in] name Locally unique identifier.
   * @param[in] val Real initial value of the local parameter.
   * @exception ARTerror If another object (parameter or port) with the same
   *            name already exists in the current time module.
   */
  virtual void addLocalParameter(const string& name, const double val);
  /**
   * @brief Adds a new local parameter with the given name and initial value
   *        to the current time module.
   * @param[in] name Locally unique identifier.
   * @param[in] val Complex initial value of the local parameter..
   * @exception ARTerror If another object (parameter or port) with the same
   *            name already exists in the current time module.
   */
  virtual void addLocalParameter(const string& name, const std::complex<double>& val);

  /**
   * @copydoc ITimeModule::addGlobalParameter()
   */
  virtual void addGlobalParameter(const DataProp* parameter);
  /**
   * @copydoc ITimeModule::removeGlobalParameter()
   */
  virtual void removeGlobalParameter(const string& name);

  /**
   * @copydoc ITimeModule::setCurrentIndex()
   */
  virtual void setCurrentIndex(int idx);
  /**
   * @copydoc ITimeModule::simulateCurrentIndex()
   */
  virtual void simulateCurrentIndex(int idx);

  /**
   * @brief Destructor of the TimeModule class.
   */
  virtual ~TimeModule();

protected:

  /**
   * @brief Registers the variable with the given name to the parsers of all
   *        output ports and local parameters.
   * @param[in] name Name of the variable. Must be identical to the name used in
   *            all parser expressions.
   * @param[in] var Reference to the variable object which will be registered.
   */
  inline virtual void addVariableToParsers(const string& name, const Variable& var);
  /**
   * @brief Removes the reference of the variable with the given name from the
   *        parsers of all output ports and local parameters.
   * @param[in] name The identifier of the variable which shall be removed.
   */
  inline virtual void removeVariableFromParsers(const string& name);

  /**
   * @brief Register all locally available variables (from input ports, output
   *        ports, local and global parameters) to the given parser object.
   * @param[in] parser Pointer to the parser to which all current variables will
   *            be registered.
   */
  inline virtual void registerAllVariablesToParser(ParserX* parser);

  /**
   * @brief Cleans up the class before it is destroyed.
   */
  inline virtual void clean();
  //	inline virtual void copy(const TimeModule& orig);

};
}

#endif /* ARTTIMEMODULE_H_ */
