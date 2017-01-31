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

#ifndef ARTPROTOTYPE_H
#define ARTPROTOTYPE_H

#include <string>

#include "ARTobject.h"
#include "HornElement.h"

using std::string;

/**
 * An object of this class is like the blueprint of an Element. It contains all properties and
 * data properties with default values the elements created from it will have. If an
 * Prototype is changed, its former descendents are not changed. Although Prototypes
 * are not restricted to one simulator, calculation domain or wavetype, their properties
 * might not allow all simulation types. An Prototype contains an object of a certain
 * subclass of HornElement. That object is the implementation of the element and copies of
 * it will be invoked when derivated Elements are used in calculation.
 */
namespace ART{
class Prototype : public ARTobject {
protected:
        HornElement* implementation; //eine Instanz des Objekts als Schablone
public:
        Prototype(const string name, const string sds="", const string lds="", const string htm="")
        : ARTobject(name,sds,lds,htm), implementation(NULL) {}

        Prototype(HornElement* impl, const string name, const string sds="", const string lds="", const string htm="")
        : ARTobject(name,sds,lds,htm), implementation(impl) {}

        virtual ~Prototype()
        {
                delete implementation;
        }

        void SetImplementation(HornElement* modImp) {implementation = modImp;}
        HornElement* GetImplementation() {return implementation;}
};
}
#endif /* ARTPROTOTYPE_H */
