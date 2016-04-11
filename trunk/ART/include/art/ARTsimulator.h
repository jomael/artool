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
 * ARTSimulator.h
 *
 *  Created on: Jul 11, 2012
 *      Author: cbg
 */

#ifndef ARTSIMULATOR_H_
#define ARTSIMULATOR_H_

//#include <list>
#include <map>
#include "mpParser.h"
#include "Interface.h"

using namespace mup;
//using std::list;

// forward declaration
class ARTItimeModule;

/**
 * Simulators belong to a simulation domain (frequency, time) and simulate waves
 * of a certain type (plain, spherical, multimodal). This is the parent class
 * for all implementations of a simulator in ART.
 */
class ARTsimulator : public ARTobject {
protected:

  /** Defines the domain type (frequency or time) of the current simulator. */
  ARTproperty domain_;

  /**
   * A parser object which can be used for assignment calculations of global
   * simulation parameters.
   */
  ParserX* parser_;

//  typedef std::map<string,ARTobject*, std::less<std::basic_string<char> >,
//      std::allocator<std::pair<const std::basic_string<char>, ARTobject*> > > ARTobjectMap;

  /// Datatype for saving references to ARTdataProp objects with the given name
  typedef std::map<string,ARTdataProp*> ARTdataPropMap;

  /// Datatype to access the referenced ARTdataProp objects saved in the map
  typedef ARTdataPropMap::value_type ARTdataPropMapValueType;

  /**
   * A map to all internally saved ARTdataProp objects (global and local
   * variables, etc.)
   */
  ARTdataPropMap* pointerMap_;

public:

  /**
   * @brief Creates a new Simulator object.
   * @param[in] name Defines the unique identifier of the simulator.
   * @param[in] domain Defines the domain type of the simulator.
   * @param[in] sds Short description (single line) of the created simulator.
   * @param[in] lds Long description of the created simulator.
   * @param[in] htm Path to help file in HTML format.
   */
  ARTsimulator(const string name, const string domain="invalidDomain",
      const string sds="", const string lds="", const string htm="");

  /**
   * @brief Returns the current domain of the simulator.
   */
  virtual ARTproperty* GetDomain() {return &domain_;}
  /**
   * @brief Returns the global parser object of the simulator.
   */
  virtual ParserX* GetParser() {return parser_;}

  /**
   * @brief Returns a data property with the specified name if it is saved in
   *        the current simulator.
   * @param[in] exp The name/identifier of the property. May contain a dot '.'
   *            in order to access hierarchical properties, e.g., "foo.bar".
   * @exception ARTerror In case the specified property could not be found in
   *            the current simulator.
   */
  virtual ARTdataProp* FindDataPropInSimulator(string exp);
  /**
   * @brief The destructor of the simulator class is doing nothing as all saved
   *        properties will be deallocated by the destructor of the ARTObject
   *        class.
   */
  virtual ~ARTsimulator();

  //these lists point into list of AcousticResearchTool Object
  /**
   * Pointer to the user elements of the simulator, mainly used by the
   * frequency Simulator.
   */
  ARTlistProp* userElements;
  /**
   * Pointer to all internal circuits, mainly used by the frequency Simulator.
   */
  ARTlistProp* circuits;

  virtual ARTdataProp* AppendDataProp(const string name, ARTvariant* val, const string sds="", const string lds="", const string htm="");
  virtual ARTdataProp* AppendDataProp(const string name, const double val, const string sds="", const string lds="", const string htm="");
  virtual ARTdataProp* AppendDataProp(const string name, const float  val, const string sds="", const string lds="", const string htm="");
  virtual ARTdataProp* AppendDataProp(const string name, const string  val, const string sds="", const string lds="", const string htm="");
  virtual ARTdataProp* AppendDataProp(const string name, const int    val, const string sds="", const string lds="", const string htm="");
  virtual ARTdataProp* AppendDataProp(ARTdataProp* dataProp);

  virtual void RegisterDataProp(ARTdataProp* dataProp);
  virtual void RegisterDataProp(ARTdataProp* dataProp, string& name);
  virtual void UnregisterDataProp(ARTdataProp* dataProp);

};

/**
 * @brief Implementation of a simulator for the frequency domain.
 */
class ARTfreqSimulator : public ARTsimulator
{
protected:
  /**
   * Defines the wave type of the simulation - plain, spherical or multimodal.
   */
  ARTproperty wavetype_;
  ARTdataContainer* frqGrid;
  ARTdataContainer* wfrqGrid;
  ARTdataContainer* modes;
public:

  ARTfreqSimulator(const string name, const string wavetype="MultiModal",
      const string sds="", const string lds="", const string htm="");

  //void SetMultimodeParameters(ARTdataProp* fmin, ARTdataProp* fmax, ARTdataProp* fstep, ARTdataProp* modes);

  virtual ARTdataContainer* GetFrequencyGrid() {return frqGrid;}
  virtual ARTdataContainer* GetAngularFrequencyGrid() {return wfrqGrid;}
  virtual ARTdataContainer* GetNumberOfModes() {return modes;}

  virtual ARTproperty* GetWavetype() {return &wavetype_;}

  virtual ~ARTfreqSimulator() {}
};

/**
 * @brief Implementation of a simulator in the time domain
 */
class ARTtimeSimulator : public ARTsimulator
{
public:

  /**
   * @brief Creates a new Simulator object.
   * @param[in] name Defines the unique identifier of the simulator.
   * @param[in] sds Short description (single line) of the created simulator.
   * @param[in] lds Long description of the created simulator.
   * @param[in] htm Path to help file in HTML format.
   */
  ARTtimeSimulator(const string name, const string sds="",
      const string lds="", const string htm="");

  /**
   * @brief Adds a time module to the current simulation.
   * @exception ARTerror If a module with the same name is already part of the
   *            current simulator.
   */
  virtual void AddTimeModule(ARTItimeModule* timeModule);
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
  virtual ARTdataProp* FindDataPropInSimulator(string exp);

  /**
   * @brief Returns a time module with the given name if it is part of the
   *        current simulator.
   * @param[in] exp Name of the time module to find.
   * @exception ARTerror If no time valid module with the given name could be
   *            found.
   */
  virtual ARTItimeModule* FindTimeModuleInSimulator(string exp);

  /**
   * @brief Destructor of the ARTtimeSimulator class.
   */
  virtual ~ARTtimeSimulator();
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
  virtual void addParamsToModule(ARTItimeModule* timeModule);

  /**
   * @brief Adds a new global parameter to all currently registered time
   *        modules.
   * @param[in] newParam The new global parameter which will be registered to
   *            all current time modules.
   */
  virtual void addParamToCurrentModules(ARTdataProp* newParam);

};

#endif /* ARTSIMULATOR_H_ */
