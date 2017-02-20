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

#ifndef ARTDATAPROPERTY_H
#define ARTDATAPROPERTY_H

#include <stdlib.h>
#include <list>

#include "Property.h"
#include "DataContainer.h"

using std::string;
namespace ART {
/**
 * A value with a concrete name belonging to an ARTobject is represented by an
 * ARTdataProperty. Just like an DataContainer it can specify some way of recalculating
 * itself. (see DataContainer for details)
 */
class DataProp : public ART::Property, public ART::DataContainer {
public:
        DataProp(const T_ART_Type dtyp, const int dlen, const string name, const string sds="", const string lds="", const string htm="") :
  ART::Property(name,sds,lds,htm,false), ART::DataContainer(dtyp, dlen, name), range(NULL) {}

        /** copy constructor */
        DataProp(const DataProp& orig)
	  : ART::Property(orig), ART::DataContainer(orig)
        {
                if (orig.range)
                        range = orig.range->clone();
                else
                        range = NULL;
        }

        ~DataProp()
        {
                        delete range;
        }

        ARTvariant* GetRange() {return range;}
        void SetRange(ARTvariant* r) {range = r;}

        virtual Cell* clone() {return new DataProp(*this);}
private:
        ARTvariant* range;
};
}
#endif /* ARTDATAPROPERTY_H */
