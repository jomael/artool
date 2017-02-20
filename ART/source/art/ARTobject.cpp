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

#include "ARTobject.h"

#include "CalculationMethod.h"
#include "ListProp.h"
#include "Simulator.h"

using namespace ART;
//**************************************************************************************************************
// ARTobject
ARTobject::ARTobject(const ARTobject& orig) //copy constructor
        : Cell(orig) //copy inherited stuff from Cell with its own copy constructor
        {
                //std::cout << "ARTobject::ARTobject("<<GetName()<<"):copyconstr\n";
                propertyList_ = orig.propertyList_;
                piter_ = propertyList_.begin();
                while (piter_ != propertyList_.end())
                {
                        *piter_ = (Property*)((*piter_)->clone());
                        //std::cout << "ARTobject::ARTobject: " << (*piter_)->GetName() << " kopiert\n";
                        ++piter_;
                }

                methodList_ = orig.methodList_;
                miter_ = methodList_.begin();
                while (miter_ != methodList_.end())
                {
                        *miter_ = (CalculationMethod*)((*miter_)->clone());
                        ++miter_;
                }

                // use same values for iterators as in original
                piter_ = orig.piter_;
                miter_ = orig.miter_;
        }

// ARTobject destructor
ARTobject::~ARTobject()
{
        for (piter_ = propertyList_.begin(); (piter_ != propertyList_.end()); ++piter_)
        {
                delete *piter_;
        }

        for (miter_ = methodList_.begin(); (miter_ != methodList_.end()); ++miter_)
        {
                delete *miter_;
        }

        propertyList_.clear();
        methodList_.clear();
}

// append new property with given name (just property without data!!)
Property* ARTobject::AppendProperty(const string name, const string sds, const string lds, const string htm)
{
        if (FindProperty(name)) throw ARTerror("ARTobject::AppendProperty","A property with the name '%s1' has already been appended to object '%s2'.",name,name_);
        propertyList_.push_back(new Property(name,sds,lds,htm));
        piter_ = propertyList_.end();
        --piter_;
        return (Property*)(*piter_);
}

// append new data property with given name
DataProp* ARTobject::AppendDataProp(const string name, ARTvariant* val, const string sds, const string lds, const string htm)
{
        if (FindProperty(name)) throw ARTerror("ARTobject::AppendProperty","A property with the name '%s1' has already been appended to object '%s2'.",name,name_);
        //cout << "AppendDataProp: " << name << "\n";
        if (val)
        {
                propertyList_.push_back(new DataProp(val->typ,val->len,name,sds,lds,htm));
                piter_ = propertyList_.end();
                --piter_;
                ((DataProp*)(*piter_))->SetValue(val);
                return (DataProp*)(*piter_);
        }
        else
        {
                propertyList_.push_back(new DataProp(C_ART_undef,-1,name,sds,lds,htm));
                piter_ = propertyList_.end();
                --piter_;
                ((DataProp*)(*piter_))->SetValue(NULL);
                return (DataProp*)(*piter_);
        }
}

// append new data property with given name
DataProp* ARTobject::AppendDataProp(DataProp* dataProp)
{
        if (FindProperty(dataProp->GetName())) throw ARTerror("ARTobject::AppendProperty","A property with the name '%s1' has already been appended to object '%s2'.",dataProp->GetName(),name_);
        propertyList_.push_back(dataProp);
        piter_ = propertyList_.end();
        --piter_;
        return (DataProp*)(*piter_);
}

DataProp* ARTobject::AppendDataProp(const string name, double val, const string sds, const string lds, const string htm)
{
        if (FindProperty(name)) throw ARTerror("ARTobject::AppendProperty","A property with the name '%s1' has already been appended to object '%s2'.",name,name_);
        ARTvariant* av = new ARTvariant(val);
        propertyList_.push_back(new DataProp(av->typ,av->len,name,sds,lds,htm));
        piter_ = propertyList_.end();
        --piter_;
        ((DataProp*)(*piter_))->SetValue(av);
        return (DataProp*)(*piter_);
}
DataProp* ARTobject::AppendDataProp(const string name, float val, const string sds, const string lds, const string htm)
{
        if (FindProperty(name)) throw ARTerror("ARTobject::AppendProperty","A property with the name '%s1' has already been appended to object '%s2'.",name,name_);
        ARTvariant* av = new ARTvariant(val);
        propertyList_.push_back(new DataProp(av->typ,av->len,name,sds,lds,htm));
        piter_ = propertyList_.end();
        --piter_;
        ((DataProp*)(*piter_))->SetValue((av));
        return (DataProp*)(*piter_);
}
DataProp* ARTobject::AppendDataProp(const string name, int val, const string sds, const string lds, const string htm)
{
        if (FindProperty(name)) throw ARTerror("ARTobject::AppendProperty","A property with the name '%s1' has already been appended to object '%s2'.",name,name_);
        ARTvariant* av = new ARTvariant(val);
        propertyList_.push_back(new DataProp(av->typ,av->len,name,sds,lds,htm));
        piter_ = propertyList_.end();
        --piter_;
        ((DataProp*)(*piter_))->SetValue((av));
        return (DataProp*)(*piter_);
}
DataProp* ARTobject::AppendDataProp(const string name, const string val, const string sds, const string lds, const string htm)
{
        if (FindProperty(name)) throw ARTerror("ARTobject::AppendProperty","A property with the name '%s1' has already been appended to object '%s2'.",name,name_);
        ARTvariant* av = new ARTvariant(val.c_str());
        propertyList_.push_back(new DataProp(av->typ,av->len,name,sds,lds,htm));
        piter_ = propertyList_.end();
        --piter_;
        ((DataProp*)(*piter_))->SetValue((av));
        return (DataProp*)(*piter_);
}


// append new listableProperty with given name
ListProp* ARTobject::AppendListProp(const string name, const string sds, const string lds, const string htm) {
        if (FindProperty(name)) throw ARTerror("ARTobject::AppendListProperty","A property with the name '%s1' has already been appended to object '%s2'.",name,name_);
        propertyList_.push_back(new ListProp(name,sds,lds,htm));
        piter_ = propertyList_.end();
        --piter_;
        return (ListProp*)(*piter_);
}

// append new method with given name
CalculationMethod* ARTobject::AppendMethod(const string name, const string sds, const string lds, const string htm) {
        if (FindMethod(name)) throw ARTerror("ARTobject::AppendMethod","A method with the name '%s1' has already been appended to object '%s2'.",name,name_);
        methodList_.push_back(new CalculationMethod(name,sds,lds,htm));
        miter_ = methodList_.end();
        miter_--;
        return (*miter_);
}

// find and return named property (or return NULL if no match)
Property* ARTobject::FindProperty(const string nam)
{
        for (piter_ = propertyList_.begin(); (piter_ != propertyList_.end()); ++piter_) {
                if ((*piter_)->GetName() == nam) {
                        return *piter_;
                }
        }
        return NULL;
}

// find and return named method (or return NULL if no match)
CalculationMethod* ARTobject::FindMethod(const string nam) {
        for (miter_ = methodList_.begin(); (miter_ != methodList_.end()); ++miter_) {
                if ((*miter_)->GetName() == nam) {
                        return *miter_;
                }
        }
        return NULL;
}

        Property* ARTobject::GetProperties(Property* pos)
        {

                if (pos == NULL)
                {
                        piter_ = propertyList_.begin();
                        return (piter_ == propertyList_.end()) ? NULL : *piter_++;
                }
                else if (piter_ == propertyList_.end())
                        {
                                return NULL;
                        }
                        else
                        {
                                return *piter_++;
                        };
        }

        bool ARTobject::DeleteProperty(Property* prp)
        {
                ::size_t sizeBefore = propertyList_.size();
                //remove all obj
                propertyList_.remove(prp);
                //if the size has changed something was removed
                if (sizeBefore > propertyList_.size()) return true;
                return false;
        }

        bool ARTobject::DeleteMethod(CalculationMethod* mtd)
        {
                ::size_t sizeBefore = methodList_.size();
                //remove all obj
                methodList_.remove(mtd);
                //if the size has changed something was removed
                if (sizeBefore > methodList_.size()) return true;
                return false;
        }

// iterate through method list (pass NULL to restart iteration, receive NULL after last element)
        CalculationMethod* ARTobject::GetMethods        (CalculationMethod* pos) {
                if (pos == NULL) {
                        miter_ = methodList_.begin();
                        return (miter_ == methodList_.end()) ? NULL : *miter_++;
                } else if (miter_ == methodList_.end()) {
                        return NULL;
                } else {
                        return *miter_++;
                }
        }


///copies properties from other objects (also copying the objects of all list-entries). OVERWRITES list own list!
void ARTobject::CopyPropertyListEntries(ARTobject* obj)
{
        //overwrites own list!
        propertyList_ = obj->GetPropertyList();
        //Now we have a copy of the list, but the entries are not copied (pointers in list are the same)
        piter_ = propertyList_.begin();
        while (piter_ != propertyList_.end())
        {
                //clone original object, then overwrite its list entry by setting the iterator to a pointer of the created clone
                *piter_ = (Property*)((*piter_)->clone());

                //ARTdataProp* dp;

                ++piter_;
        }
}

///copies methods from other objects (also copying the objects of all list-entries). OVERWRITES list own list!
void ARTobject::CopyMethodListEntries(ARTobject* obj)
{
        //see copyPropertyListEntries for code comments
        methodList_ = obj->GetMethodList();
        miter_ = methodList_.begin();
        while (miter_ != methodList_.end())
        {
                *miter_ = (CalculationMethod*)((*miter_)->clone());
                ++miter_;
        }
}
