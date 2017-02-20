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
 * Simulator.cpp
 *
 *  Created on: Jul 11, 2012
 *      Author: cbg
 */

#include <sstream>
#include "strparsing.h"
#include "Simulator.h"
#include "ListProp.h"
#include "TimeModule.h"
#include "Functionoid.h"

using namespace mup;

using std::stringstream;
using namespace ART;
//**************************************************************************************************************
// ARTsimulator

Simulator::Simulator(const string name, const string domain,
    const string sds, const string lds, const string htm) :
    ARTobject(name,sds,lds,htm), domain_(domain), parser_(new ParserX(mup::pckCOMPLEX_NO_STRING)),
    pointerMap_(new ARTdataPropMap()), userElements(NULL)
{
}


/// Finds the data property represented by the string exp; This can be a data property of an element or model in the simulator (then the string is something like "Cyl.length") or a data property of the simulator itself.
DataProp* Simulator::FindDataPropInSimulator(string exp)
{
  DataProp* dataProp = NULL;

  dataProp = (*pointerMap_)[exp];

  if (dataProp == NULL) throw ARTerror("ARTsimulator::FindDataPropInSimulator", "Simulator '%s1' does not contain any object with the name '%s2'.",  this->name_, exp);

  /*
  //try to find a property of the simulator with name *exp*
  prop = dynamic_cast<ARTdataProp*>( FindProperty( strcrop( exp ) ));
  //if not found, try to find a property *exp* in the element list
  if (!prop)
  {
    vector<string> nameparts = strsplit(exp,'.');
    Element* element = dynamic_cast<Element*>(userElements->FindObject( strcrop(nameparts[0]).c_str() ));
    if (element == NULL) throw ARTerror("ARTsimulator::FindDataPropInSimulator", "An element of the specified name '%s1' does not exist and '%s1' is not recognized as a data property of the simulator.", strcrop(nameparts[0]).c_str() );
    prop = dynamic_cast<ARTdataProp*>(element->model->FindProperty( strcrop(nameparts[1]).c_str() ));
    if (prop == NULL) throw ARTerror("ARTsimulator::FindDataPropInSimulator", "The element '%s1' does not have the specified data property '%s2'.",  strcrop(nameparts[0]).c_str() ,  strcrop(nameparts[1]).c_str() );
  }
  */
  return dataProp;
}

DataProp* Simulator::AppendDataProp(const string name, ARTvariant* val, const string sds, const string lds, const string htm)
{
  DataProp* dataProp = ARTobject::AppendDataProp(name, val, sds, lds, htm);
//  std::cout << "pointerMap[" << dataProp->GetName() << "] = " << dataProp << std::endl;
  (*pointerMap_)[dataProp->GetName()] = dataProp;
  return dataProp;
}

DataProp* Simulator::AppendDataProp(const string name, const double val, const string sds, const string lds, const string htm)
{
  DataProp* dataProp = ARTobject::AppendDataProp(name, val, sds, lds, htm);
//  std::cout << "pointerMap[" << dataProp->GetName() << "] = " << dataProp << std::endl;
  (*pointerMap_)[dataProp->GetName()] = dataProp;
  return dataProp;
}

DataProp* Simulator::AppendDataProp(const string name, const float  val, const string sds, const string lds, const string htm)
{
  DataProp* dataProp = ARTobject::AppendDataProp(name, val, sds, lds, htm);
//  std::cout << "pointerMap[" << dataProp->GetName() << "] = " << dataProp << std::endl;
  (*pointerMap_)[dataProp->GetName()] = dataProp;
  return dataProp;
}

DataProp* Simulator::AppendDataProp(const string name, const string val, const string sds, const string lds, const string htm)
{
  DataProp* dataProp = ARTobject::AppendDataProp(name, val, sds, lds, htm);
//  std::cout << "pointerMap[" << dataProp->GetName() << "] = " << dataProp << std::endl;
  (*pointerMap_)[dataProp->GetName()] = dataProp;
  return dataProp;
}

DataProp* Simulator::AppendDataProp(const string name, const int val, const string sds, const string lds, const string htm)
{
  DataProp* dataProp = ARTobject::AppendDataProp(name, val, sds, lds, htm);
//  std::cout << "pointerMap[" << dataProp->GetName() << "] = " << dataProp << std::endl;
  (*pointerMap_)[dataProp->GetName()] = dataProp;
  return dataProp;
}

DataProp* Simulator::AppendDataProp(DataProp* dataProp)
{
//  std::cout << "pointerMap[" << dataProp->GetName() << "] = " << dataProp << std::endl;
  (*pointerMap_)[dataProp->GetName()] = dataProp;
  return ARTobject::AppendDataProp(dataProp);
}

void Simulator::RegisterDataProp(DataProp* dataProp)
{
  if (dataProp)
  {
//    std::cout << "pointerMap[" << dataProp->GetName() << "] = " << dataProp << std::endl;
    (*pointerMap_)[dataProp->GetName()] = dataProp;
  }
  else
  {
    ARTerror* e = new ARTerror("Error in simulator '%s1' - no valid dataProp in method RegisterDataProp().",
                               name_);
    throw e;
  }
}

void Simulator::RegisterDataProp(DataProp* dataProp, string& name)
{
  if (dataProp)
  {
//    std::cout << "pointerMap[" << name << "] = " << dataProp << std::endl;
    (*pointerMap_)[name] = dataProp;
  }
  else
  {
    ARTerror* e = new ARTerror("Error in simulator '%s1' - no valid dataProp in method RegisterDataProp().",
                               name_);
    throw e;
  }
}

void Simulator::UnregisterDataProp(DataProp* dataProp)
{
  if (dataProp)
  {
    (*pointerMap_).erase(dataProp->GetName());
  }
  else
  {
    ARTerror* e = new ARTerror("Error in simulator '%s1' - no valid dataProp in method RemoveDataProp().",
			       name_);
    throw e;
  }
}

Simulator::~Simulator()
{
  // IMPORTANT: do NOT free the memory of the parser here!
  //delete (parser_);

  // delete pointerMap_ as we are assuming the map itself is empty because all
  // items should have been removed by the destructors of the saved variables
  delete(pointerMap_);
}
