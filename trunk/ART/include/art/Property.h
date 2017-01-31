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

#ifndef ARTPROPERTY_H
#define ARTPROPERTY_H

#include <stdlib.h>
#include <list>

#include "Cell.h"

/**
 * Objects of this class are sets of strings (see Cell for details), that represent the
 * property of an ARTobject. Typically each ARTobject contains several properties that are
 * saved in a list. This list can be queried* by the programme, to check if a given object
 * has a certain property.
 */
namespace ART{
  class Property : public Cell {
  private:
    bool listable_;
  public:
  Property(const string name, const string sds="", const string lds="", const string htm="", const bool listable=false) :
    Cell(name,sds,lds,htm), listable_(listable) {}

  Property(const Property& orig) /**<copy constructor*/
    : Cell(orig)
    {
      listable_ = orig.listable_;
    }

    virtual ~Property() {}


    virtual Cell* clone() {return new Property(*this);}

    bool IsListable() { return listable_;}
  };
}
#endif /* ARTPROPERTY_H */
