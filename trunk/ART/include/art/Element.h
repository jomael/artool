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

#ifndef ARTELEMENT_H
#define ARTELEMENT_H

#include <string>

#include "ModelInterface.h"
#include "DataContainer.h"

using std::string;
namespace ART{
/**
 * Objects of this class represent acoustic elements. They are created in a simulator and
 * must match the calculation domain and wavetype of the simulator. (They are saved in the
 * simulator->userElements list) \par
 * Elements are derived from an ARTprototype. In fact they are copies with added
 * properties like impedance and propagation matrix.
 */
class Element : public ModelInterface {
public:
  ART::DataContainer* z_rad; //Radiaton impedance
  ART::DataContainer* z_inp; //multimode input impedance

        ModelInterface* model; //a clone of the prototype model


        WaveObjectInterface* wavefrontOut; //wave after passing this element
        WaveObjectInterface* wavefrontIn;


        Element(const string name, const string sds="", const string lds="", const string htm="", ModelInterface* prototype=NULL, Simulator* sim=NULL);
//      : ModelInterface(name,sds,lds,htm),
//        //propMatrix(NULL),
//        model(NULL),
//        wavefrontOut(NULL),
//        wavefrontIn(NULL)
//      {
//
//              ParserX* parser = NULL;
//              if (sim) parser = sim->GetParser();
//
//              if (prototype)
//              {
//                  model = prototype->CloneModel();
//                      model->SetSimulator(sim);
//                      model->CopyPropertyListEntries(prototype);
//                      model->CopyMethodListEntries(prototype);
//                      //Create Parser variables
//                      if ( parser )
//                      {
//                              ARTproperty* prop = model->GetProperties(NULL);
//                              while (prop)
//                              {
//                                      //if it is a data property
//                                      ARTdataProp* dprop = dynamic_cast<ARTdataProp*>(prop);
//                                      if (dprop)
//                                      {
//                                              string varname = name_ + "." + dprop->GetName();
//                                              dprop->SetParser(parser);
//                                              dprop->SetParserVar(varname);
//                                              //std::cout << "Created Parser Var: " << varname << "\n";
//                                      }
//                                      prop = model->GetProperties(prop);
//                              }
//                      }
//              }
//              //Matrix und Impedanz ohne Funktion erstmal
//              //propMatrix = AppendDataProp("M_" + name, NULL, "The transmission matrix of this element.");
//
//              //Set the output data container of the wave object to a property we append to the object
//              z_inp = AppendDataProp("Z_" + name, NULL, "The propagated impedance at this element's or circuit's entry.");
//              z_rad = AppendDataProp("ZR_" + name, NULL, "The radiation impedance at this element's or circuit's opening.");
//              wavefrontOut = new WaveObjectMMImpedance(z_inp, NULL, NULL);
//      }


        virtual void Rename(const string newname)
        {
                if (model)
                {
                        //Rename model properties
                        ARTproperty* prop = model->GetProperties(NULL);
                        while (prop)
                        {
                                //if it is a data property
                                ARTdataProp* dprop = dynamic_cast<ARTdataProp*>(prop);
                                if (dprop)
                                {
                                        string varname = newname + "." + dprop->GetName();
                                        dprop->Rename(varname);
                                }
                                prop = model->GetProperties(prop);
                        }
                }
//              z_inp->Rename("Z_" + newname);
//              z_rad->Rename("ZR_" + newname);
                z_inp->Rename(newname + ".Z_");
                z_rad->Rename(newname + ".ZR_");
                SetName(newname);
        }

        virtual void PrepareCalculation();

        virtual void RadiationImpedance(WaveObjectInterface*& out)
        {
                model->RadiationImpedance(out);
        }

        virtual void InputImpedance(WaveObjectInterface* in, WaveObjectInterface*& out)
        {
                model->InputImpedance(in, out);
        }

        virtual void Pressure(WaveObjectInterface*, WaveObjectInterface*&) {throw ARTerror("Element::Pressure","The function is not implemented.");};

    void SetScope(Simulator* sim);
//      {
//              if (sim == NULL)
//                      throw ARTerror("Element::SetScope", "The specified simulator is invalid.");
//
//              piter_ = propertyList_.begin();
//              ARTdataProp* p;
//
//              while (piter_ != propertyList_.end())
//              {
//                      p = dynamic_cast<ARTdataProp*>(*piter_);
//                      if (p) ((DataContainer*)p)->SetParser(sim->GetParser());
//                      piter_++;
//              }
//      }

        virtual ModelInterface* CloneModel() {return NULL;}

        bool IsPrototypeProperty(ARTproperty* p)
        {
                ARTproperty* found = NULL;
                found = model->FindProperty(p->GetName());
                if (found) return true;
                else return false;
        }

        virtual bool HasBends();
        //HornElement* GetImplementation() {std::cout << "Function GetImplementation in Element no longer supported. It's now in the model!!\n"; return NULL;}

        virtual ~Element ()
        {
                delete model;
                //delete propMatrix;    is part of properties and will be deleted in ~ARTobject
        }

};
}

#endif /* ARTELEMENT_H */
