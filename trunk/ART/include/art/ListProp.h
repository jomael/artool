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

#ifndef ARTLISTPROPERTY_H
#define ARTLISTPROPERTY_H

#include <string>
#include <list>

#include "Property.h"
#include "ARTobject.h"

using std::string;

/**
 * A list of objects (like all Element:s as userElements) that is added as property to
 * another ARTobject. prototypeModels for ex. is such a list, belonging to the root
 * object, and to this list all Prototype:s are added. (List is then part of ARTobject's
 * property list)
 */
namespace ART{
  class ListProp : public Property {
  private:
    list<ARTobject*> objectList_;
    list<ARTobject*>::iterator oiter_;
  public:

    typedef list<ARTobject*>::size_type size_type;

  ListProp(const string name, const string sds="", const string lds="", const string htm="") :
    ART::Property(name,sds,lds,htm,true),
      objectList_(list<ARTobject*>()),
      oiter_(objectList_.begin())
	{}

    virtual ~ListProp ()
      {
	for (oiter_ = objectList_.begin(); (oiter_ != objectList_.end()); oiter_++)
	  {
	    delete *oiter_;
	  }
      }

    /// iterate through object list (pass NULL to restart iteration, receive NULL after last element)
    virtual ARTobject* GetObjects(ARTobject* pos);

    /// find and return named object (or return NULL if no match)
    virtual ARTobject* FindObject(const string nam) ;

    /// append new object with given name
    virtual ARTobject* AppendObject(const string name, const string sds="", const string lds="", const string htm="");

    /// append object which is already created
    virtual ARTobject* AppendObject(ARTobject* object);

    /// delete object pos
    virtual bool DeleteObject(ARTobject* pos);

    /// replace all occurences of obj with newobj @returns the number of replacements
    virtual int ReplaceObject(ARTobject* obj,ARTobject* newobj);

    /// return the size of current list
    virtual size_type Size();

  };
}
#endif /* ARTLISTPROPERTY_H */
