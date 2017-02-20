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

#ifndef CALCULATIONMETHOD_H
#define CALCULATIONMETHOD_H

#include <string>

#include "ARTobject.h"

using std::string;
namespace ART {
/**
 * Objects of this class represent user actions.The impedance calculation for example is
 * an ARTmethod object, and it contains minimum and maximum frequency as well as delta
 * frequency. When simulating, these parameters are read from there.
 * \remark One ARTmethod will probably represent one menu command and point to a function
 * executing the command.
 */
class CalculationMethod : public ARTobject {
protected:
        void (*methodptr_)();
public:

        CalculationMethod(const string name, const string sds="", const string lds="", const string htm="")
         : ART::ARTobject(name,sds,lds,htm), methodptr_(NULL)
        {}

        CalculationMethod(const CalculationMethod& orig) ///< copy constructor
        : ART::ARTobject(orig)
        {}

        virtual ~CalculationMethod()
        {}

        virtual Cell* clone() {return new CalculationMethod(*this);}

        void ApplyMethod()
        {}
};
}
#endif /* CALCULATIONMETHOD_H */
