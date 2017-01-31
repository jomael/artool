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

#include "ListProp.h"
using namespace ART;
//**************************************************************************************************************
// ListProp

// append new object with given name
ARTobject* ListProp::AppendObject(const string name, const string sds, const string lds, const string htm)
{
        objectList_.push_back(new ARTobject(name,sds,lds,htm));
        oiter_ = objectList_.end();
        --oiter_;
        return (*oiter_);
}

// append object which is already created
ARTobject* ListProp::AppendObject(ARTobject* object)
{
        objectList_.push_back(object);
        oiter_ = objectList_.end();
        --oiter_;
        return (*oiter_);
}

// find and return named object (or return NULL if no match)
ARTobject* ListProp::FindObject(const string nam)
{
        for (oiter_ = objectList_.begin(); (oiter_ != objectList_.end()); ++oiter_) {
                if ((*oiter_)->GetName() == nam) {
                        return *oiter_;
                }
        }
        return NULL;
}

ARTobject* ListProp::GetObjects(ARTobject* pos)
{
        if (pos == NULL) {
                oiter_ = objectList_.begin();
                return (oiter_ == objectList_.end()) ? NULL : *oiter_++;
        }
        else if (oiter_ == objectList_.end()) {
                return NULL;
        } else {
                return *oiter_++;
        };
}

bool ListProp::DeleteObject(ARTobject* obj)
{
        ::size_t sizeBefore = objectList_.size();
        //remove all obj
        objectList_.remove(obj);
        //if the size has changed something was removed
        if (sizeBefore > objectList_.size()) {
                return true;
        }
        return false;
}

int ListProp::ReplaceObject(ARTobject* obj,ARTobject* newobj)
{
        int i = 0;
        for (oiter_ = objectList_.begin(); (oiter_ != objectList_.end()); ++oiter_)
        {
                if (obj == (*oiter_))
                {
                        *oiter_ = newobj;
                        i++;
                }
        }
        return i;
}

ListProp::size_type ListProp::Size()
{
        return objectList_.size();
}
