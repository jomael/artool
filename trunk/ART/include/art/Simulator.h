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
 * Simulator.h
 *
 *  Created on: Jul 11, 2012
 *      Author: cbg
 */

#ifndef ARTSIMULATOR_H_
#define ARTSIMULATOR_H_

//#include <list>
#include <map>
#include "mpParser.h"
#include "ARTobject.h"
#include "Property.h"
#include "DataProp.h"

using namespace mup;
//using std::list;

namespace ART{
// forward declaration
class ITimeModule;

/**
 * Simulators belong to a simulation domain (frequency, time) and simulate waves
 * of a certain type (plain, spherical, multimodal). This is the parent class
 * for all implementations of a simulator in ART.
 */
class Simulator : public ARTobject {
protected:

  /** Defines the domain type (frequency or time) of the current simulator. */
  Property domain_;

  /**
   * A parser object which can be used for assignment calculations of global
   * simulation parameters.
   */
  ParserX* parser_;

//  typedef std::map<string,ARTobject*, std::less<std::basic_string<char> >,
//      std::allocator<std::pair<const std::basic_string<char>, ARTobject*> > > ARTobjectMap;

  /// Datatype for saving references to ARTdataProp objects with the given name
  typedef std::map<string,DataProp*> ARTdataPropMap;

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
  Simulator(const string name, const string domain="invalidDomain",
      const string sds="", const string lds="", const string htm="");

  /**
   * @brief Returns the current domain of the simulator.
   */
  virtual Property* GetDomain() {return &domain_;}
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
  virtual DataProp* FindDataPropInSimulator(string exp);
  /**
   * @brief The destructor of the simulator class is doing nothing as all saved
   *        properties will be deallocated by the destructor of the ARTObject
   *        class.
   */
  virtual ~Simulator();

  //these lists point into list of AcousticResearchTool Object
  /**
   * Pointer to the user elements of the simulator, mainly used by the
   * frequency Simulator.
   */
  ListProp* userElements;
  /**
   * Pointer to all internal circuits, mainly used by the frequency Simulator.
   */
  ListProp* circuits;

  virtual DataProp* AppendDataProp(const string name, ARTvariant* val, const string sds="", const string lds="", const string htm="");
  virtual DataProp* AppendDataProp(const string name, const double val, const string sds="", const string lds="", const string htm="");
  virtual DataProp* AppendDataProp(const string name, const float  val, const string sds="", const string lds="", const string htm="");
  virtual DataProp* AppendDataProp(const string name, const string  val, const string sds="", const string lds="", const string htm="");
  virtual DataProp* AppendDataProp(const string name, const int    val, const string sds="", const string lds="", const string htm="");
  virtual DataProp* AppendDataProp(DataProp* dataProp);

  virtual void RegisterDataProp(DataProp* dataProp);
  virtual void RegisterDataProp(DataProp* dataProp, string& name);
  virtual void UnregisterDataProp(DataProp* dataProp);

};
}
#endif /* ARTSIMULATOR_H_ */
