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

#include "Element.h"

using namespace ART;

Element::Element(const string name, const string sds, const string lds, const string htm, ModelInterface* prototype, Simulator* sim)
: ModelInterface(name,sds,lds,htm),
  //propMatrix(NULL),
  model(NULL),
  wavefrontOut(NULL),
  wavefrontIn(NULL)
{

        ParserX* parser = NULL;
        if (sim) parser = sim->GetParser();

        if (prototype)
        {
                model = prototype->CloneModel();
                model->SetSimulator(sim);
                model->CopyPropertyListEntries(prototype);
                model->CopyMethodListEntries(prototype);
                //Create Parser variables
                if ( parser )
                {
                        Property* prop = model->GetProperties(NULL);
                        while (prop)
                        {
                                //if it is a data property
                                DataProp* dprop = dynamic_cast<DataProp*>(prop);
                                if (dprop)
                                {
                                        string varname = name_ + "." + dprop->GetName();
                                        dprop->SetParser(parser);
                                        dprop->SetParserVar(varname);
                                        //std::cout << "Created Parser Var: " << varname << "\n";
                                }
                                prop = model->GetProperties(prop);
                        }
                }
        }
        //Matrix und Impedanz ohne Funktion erstmal
        //propMatrix = AppendDataProp("M_" + name, NULL, "The transmission matrix of this element.");

        //Set the output data container of the wave object to a property we append to the object
        z_inp = AppendDataProp("Z_" + name, 0, "The propagated impedance at this element's or circuit's entry.");
        z_rad = AppendDataProp("ZR_" + name, 0, "The radiation impedance at this element's or circuit's opening.");
//      z_inp = AppendDataProp(name + ".Z_", 0, "The propagated impedance at this element's or circuit's entry.");
//      z_rad = AppendDataProp(name + ".ZR_", 0, "The radiation impedance at this element's or circuit's opening.");
        wavefrontOut = new WaveObjectMMImpedance(z_inp, NULL, NULL);
}

//removes all dependencies
void Element::PrepareCalculation()
{
        DataProp* dprop;
        Property* prop = NULL;
        do
        {
                //if a property is found...
                if ((prop = this->GetProperties(prop)))
                        //if its no PROTOTYPE prop, but a data property...
                        if (!(this->IsPrototypeProperty(prop)) &&
                                (dprop = dynamic_cast<DataProp*>(prop)))
                                {
                                        dprop->RemoveAllDependencies();
                                }
        }
        while(prop); //until no prop. is found, it's NULL then...
}

void Element::SetScope(Simulator* sim)
{
        if (sim == NULL)
                throw ARTerror("Element::SetScope", "The specified simulator is invalid.");

        piter_ = propertyList_.begin();
        DataProp* p;

        while (piter_ != propertyList_.end())
        {
                p = dynamic_cast<DataProp*>(*piter_);
                if (p) ((DataContainer*)p)->SetParser(sim->GetParser());
                piter_++;
        }
}

bool Element::HasBends()
{
        if (model->FindProperty("bent")) return true;
        else return false;
}
