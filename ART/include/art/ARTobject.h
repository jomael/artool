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
*  Copyright (C) 2011 by the authors and their organisations:             *
*    Alistair Braden            1)                                        *
*    Wilfried Kausel            2)         kausel(at)mdw.ac.at            *
*    Delphine Cadefaux          2)                                        *
*    Vasileios Chatziioannou    2)                                        *
*    Sadjad Siddiq              2)                                        *
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

#ifndef ARTOBJECT_H
#define ARTOBJECT_H

#include <stdlib.h>
#include <list>
#include <string>
#include "ARTlink.h"
//#include "Functionoid.h"

#include "Cell.h"

/* forward declarations */
namespace ART{
class Property;
class ListProp;
class CalculationMethod;
class DataProp;
}



namespace ART{
/**
 * This is the base class for all acoustic objects. It contains fields for name,
 * descriptions and properties.
 */
class ARTobject : public Cell {
private:
protected:
        list<ART::Property*>      propertyList_;
        list<ART::Property*>::iterator piter_;
        list<CalculationMethod*>        methodList_;
        list<CalculationMethod*>::iterator miter_;
public:
        /// objects must have a name, the rest is optional
        ARTobject(const string name, const string sds="", const string lds="", const string htm="") : Cell(name,sds,lds,htm),
                propertyList_(list<ART::Property*>()),    piter_(propertyList_.begin()),
                methodList_(list<CalculationMethod*>()),                miter_(methodList_.begin()) {}

        ARTobject(const ARTobject& orig); ///<copy constructor


        virtual ~ARTobject();

        virtual Cell* clone() {return new ARTobject(*this);}

  /// iterate through property list (pass NULL to restart iteration, receive NULL after last element)
        virtual ART::Property* GetProperties(ART::Property* pos);

  /// find and return named property (or return NULL if no match)
        virtual ART::Property* FindProperty(const string nam) ;

        ///Append new property with given name to ARTobject
        virtual ART::Property* AppendProperty(const string name, const string sds="", const string lds="", const string htm="") ;

  /// append new data property with given name to ARTobject
        virtual DataProp* AppendDataProp(const string name, ARTvariant* val, const string sds="", const string lds="", const string htm="");
        virtual DataProp* AppendDataProp(const string name, const double val, const string sds="", const string lds="", const string htm="");
        virtual DataProp* AppendDataProp(const string name, const float  val, const string sds="", const string lds="", const string htm="");
        virtual DataProp* AppendDataProp(const string name, const string  val, const string sds="", const string lds="", const string htm="");
        virtual DataProp* AppendDataProp(const string name, const int    val, const string sds="", const string lds="", const string htm="");
//      virtual ARTdataProp* AppendDataProp(const string name, const string* val, const string sds="", const string lds="", const string htm="");
        virtual DataProp* AppendDataProp(DataProp* dataProp) ;

  /// append new listableProperty with given name
        virtual ART::ListProp* AppendListProp(const string name, const string sds="", const string lds="", const string htm="");

  /// delete current property (which was recently accessed by GetProperties, FindProperty or AppendProperty)
        virtual bool DeleteProperty(ART::Property* prp);

  /// iterate through method list (pass NULL to restart iteration, receive NULL after last element)
        virtual CalculationMethod* GetMethods   (CalculationMethod* pos);

  /// find and return named method (or return NULL if no match)
        virtual CalculationMethod* FindMethod(const string nam) ;

  /// append new method with given name
        virtual CalculationMethod* AppendMethod(const string name, const string sds="", const string lds="", const string htm="");

  /// delete current method (which was recently accessed by GetProperties, FindProperty or AppendProperty)
        virtual bool DeleteMethod(CalculationMethod* mtd);

        virtual void SetPropertyList(list<ART::Property*> &l)
        {
                propertyList_ = l;
        }

        virtual void CopyPropertyListEntries(ARTobject* obj);
        virtual void CopyMethodListEntries(ARTobject* obj);

        virtual list<ART::Property*> GetPropertyList(){return propertyList_;}
        virtual list<CalculationMethod*> GetMethodList(){return methodList_;}

};
}
#endif /* ARTOBJECT_H */
