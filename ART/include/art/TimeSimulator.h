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


#ifndef ARTTIMESIMULATOR_H_
#define ARTTIMESIMULATOR_H_

//#include <list>
#include <map>
#include "mpParser.h"
#include "ARTobject.h"
#include "Property.h"
#include "DataProp.h"
#include "Simulator.h"

using namespace mup;
//using std::list;

namespace ART{
// forward declaration
class ITimeModule;

 /**
 * @brief Implementation of a simulator in the time domain
 */
class TimeSimulator : public ART::Simulator
{
public:

  /**
   * @brief Creates a new Simulator object.
   * @param[in] name Defines the unique identifier of the simulator.
   * @param[in] sds Short description (single line) of the created simulator.
   * @param[in] lds Long description of the created simulator.
   * @param[in] htm Path to help file in HTML format.
   */
  TimeSimulator(const string name, const string sds="",
      const string lds="", const string htm="");

  /**
   * @brief Adds a time module to the current simulation.
   * @exception ARTerror If a module with the same name is already part of the
   *            current simulator.
   */
  virtual void AddTimeModule(ITimeModule* timeModule);
  /**
   * @brief Adds a global parameter to the current simulator.
   * @param[in] name A unique identifier for the simulation parameter.
   * @param[in] expr A calculation expression for the paramenter.
   * @exception ARTerror If a parameter with the same name is already part of
   *            the current simulator.
   */
  virtual void AddSimulationParameter(const string& name, const string& expr);
  /**
   * @brief Adds a global parameter to the current simulator.
   * @param[in] name A unique identifier for the simulation parameter.
   * @param[in] val A complex number to initialize the parameter.
   * @exception ARTerror If a parameter with the same name is already part of
   *            the current simulator.
   */
  virtual void AddSimulationParameter(const string& name, const std::complex<double>& val);
  /**
   * @brief Adds a global parameter to the current simulator.
   * @param[in] name A unique identifier for the simulation parameter.
   * @param[in] val A real number to initialize the parameter.
   * @exception ARTerror If a parameter with the same name is already part of
   *            the current simulator.
   */
  virtual void AddSimulationParameter(const string& name, double val);

  /**
   * @brief Simulates the time step of the given index.
   * @param[in] idx The index to simulate.
   * @exception ARTerror If no global parameter "t" could be found in the
   *            current simulator.
   */
  virtual void SimulateTimeStep(int idx);

  /**
   * @brief Sets the value of a global parameter of the current simulator.
   * @param[in] name The unique identifier of the global parameter.
   * @param[in] expr A calculation expression for the parameter.
   * @exception ARTerror If no parameter with the specified name could be found
   *            in the current simulator.
   */
  virtual void SetSimulationParameter(const string& name, const string& expr);
  /**
   * @brief Sets the value of a global parameter of the current simulator.
   * @param[in] name The unique identifier of the global parameter.
   * @param[in] val The complex value to set the parameter.
   * @exception ARTerror If no parameter with the specified name could be found
   *            in the current simulator.
   */
  virtual void SetSimulationParameter(const string& name, const std::complex<double>& val);
  /**
   * @brief Sets the value of a global parameter of the current simulator.
   * @param[in] name The unique identifier of the global parameter.
   * @param[in] val The real value to set the parameter.
   * @exception ARTerror If no parameter with the specified name could be found
   *            in the current simulator.
   */
  virtual void SetSimulationParameter(const string& name, double val);

  /**
   * @copydoc ARTsimulator::FindDataPropInSimulator()
   */
  virtual DataProp* FindDataPropInSimulator(string exp);

  /**
   * @brief Returns a time module with the given name if it is part of the
   *        current simulator.
   * @param[in] exp Name of the time module to find.
   * @exception ARTerror If no time valid module with the given name could be
   *            found.
   */
  virtual ITimeModule* FindTimeModuleInSimulator(string exp);

  /**
   * @brief Destructor of the ARTtimeSimulator class.
   */
  virtual ~TimeSimulator();
protected:

  /**
   * @brief Adds the default global parameters (sampling frequency "T" and
   *        global time "t") to the current simulator. Will be called by the
   *        class constructor.
   */
  virtual void initStandardSimulParams();

  /**
   * @brief Deallocates all previously allocated objects. Currently not used.
   */
  virtual void clean();

  /**
   * @brief Adds all global parameters to the given time module.
   * @param[in] timeModule Time module to which the parameters will be added.
   */
  virtual void addParamsToModule(ITimeModule* timeModule);

  /**
   * @brief Adds a new global parameter to all currently registered time
   *        modules.
   * @param[in] newParam The new global parameter which will be registered to
   *            all current time modules.
   */
  virtual void addParamToCurrentModules(DataProp* newParam);

};
}
#endif /* ARTTIMESIMULATOR_H_ */
