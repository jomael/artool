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

#include "ARTcircuit.h"
#include "ARTmodel.h"

//**************************************************************************************************************
// ARTcircuit
ARTcircuit::ARTcircuit(ARTsimulator* simulator, const string name, const string sds, const string lds, const string htm)
        : ARTelement(name,sds,lds,htm), references(), impedanceCurve_(NULL), simulator_(simulator), wavefrontRadiation(NULL) // , wavefrontOut(NULL), wavefrontIn(NULL)

{
        //Every circuit has its impedance as data property. impedanceCurve is a shortcut pointer:
        impedanceCurve_ = AppendDataProp("InputImpedanceCurve" , 0, "The plot of this circuit's acoustic input impedance as a vector of complex numbers");

        //every circuit owns a branch model, but it is not added to properties
        model = new BranchModel(name_ + "_Branch");
        model->SetSimulator(simulator);
}

void ARTcircuit::AppendElement(ARTelement* element)
{
        //try to find the element in the circuit. it is not legal to append an element twice!
        if (GetElementPosition(element) != -1) throw ARTerror("ARTcircuit::appendElement", "The element is already appended to the circuit!");
        //append element pointed to by element to vector
        references.push_back(element);
}

int ARTcircuit::GetElementPosition(ARTelement* el)
{
        int pos;
        pos = -1;
        //find element
        ::size_t i;
        for (i = 0; i < references.size(); ++i) {
                if (references[i] == el)
                {
                        pos = i;
                        break;
                }
        }
        return pos;
}

void ARTcircuit::AppendElementBefore(ARTelement* before, ARTelement* element)
{
        //try to find the element in the circuit. it is not legal to append an element twice!
        if (GetElementPosition(element) != -1) throw ARTerror("ARTcircuit::appendElement", "The element is already appended to the circuit!");

        vector<ARTelement*>::iterator pos;
        pos = references.end();
        //find element before
        vector<ARTelement*>::iterator it;
        for (it = references.begin(); it < references.end(); ++it) {
                if (*it == before)
                {
                        pos = it;
                        break;
                }
        }
        if (pos == references.end()) throw ARTerror("ARTcircuit::appendElementBefore", "The element specified as parameter 'before' was not found.");

        references.insert(pos, element);
}

void ARTcircuit::AppendElementAfter(ARTelement* after, ARTelement* element)
{
        //try to find the element in the circuit. it is not legal to append an element twice!
        if (GetElementPosition(element) != -1) throw ARTerror("ARTcircuit::appendElement", "The element is already appended to the circuit!");

        vector<ARTelement*>::iterator pos;
        pos = references.begin();
        //find element after
        vector<ARTelement*>::iterator it;
        for (it = references.begin(); it < references.end(); ++it ) {
                if (*it == after)
                {
                        pos = ++it; //move to the next position
                        break;
                }
        }
        if (pos == references.begin()) throw ARTerror("ARTcircuit::appendElementafter", "The element specified as parameter 'after' was not found.");
        //insert element *before* pos (that's why we've moved it...)
        references.insert(pos, element);
}

int ARTcircuit::DeleteElement(ARTelement* element)
{
        int found = 0;
        //find element
        vector<ARTelement*>::iterator it;
        for (it = references.begin(); it < references.end(); ++it )
                if (*it == element)
                {
                        *it = NULL;
                        found++;
                }
        return found;
}

int ARTcircuit::RemoveElement(ARTelement* element)
{
        int found = 0;
        //find element
        vector<ARTelement*>::iterator it = references.begin();
        while (it < references.end()) {
                if (*it == element)
                {
                        it = references.erase(it); //returns position after removed element
                        found++;
                        continue;
                }
                ++it;
        }
//      if (!found) throw ARTerror("ARTcircuit::removeElement", "The element specified as parameter 'element' was not found.");
        return found;
}

int ARTcircuit::ReplaceElement(ARTelement* search, ARTelement* replace)
{
        int found = 0;
        //find element
        vector<ARTelement*>::iterator it;
        for (it = references.begin(); it < references.end(); it++ )
                if (*it == search)
                {
                        *it = replace; //replace the pointer
                        found++;
                }
//      if (!found) throw ARTerror("ARTcircuit::replaceElement", "The element specified as parameter 'search' was not found.");
        return found;
}

int ARTcircuit::RemoveAllElements()
{
        int s = references.size();
        references.clear();
        return s;
}

bool ARTcircuit::HasBends()
{
        bool hasBends = false;
        //cycle through all references, check if they have bends
        for (::size_t i = 0; i < references.size(); i++)
        {
                if (references.at(i)->HasBends()) hasBends = true;
        }
        return hasBends;
}

void ARTcircuit::PrepareCalculation()
{
        //cout << " **************** ARTcircuit::prepareCalculation() " << GetName() << " *********\n" ;
        //ARTdataContainer* frqGrid = simulator_->GetAngularFrequencyGrid();
        //ARTdataContainer* wfrqGrid = simulator_->GetFrequencyGrid();
        //ARTdataContainer* modes = simulator_->GetNumberOfModes();

        ::size_t i;
        int j;

        // remove all dependencies of own properties
        impedanceCurve_->RemoveAllDependencies();
        z_inp->RemoveAllDependencies();
        z_rad->RemoveAllDependencies();
        //m_inst->RemoveAllDependencies();

        // remove all dependencies      of attached elements. This is VERY important!!! Otherwise old dependencies will be mixed with new ones, resulting in circular references and other ugly things...
        // this happens in the elements prepareCalculation function
        // if the reference is indeed a circuit, the circuits prepareCalculation will be called, of course...
        for (i = 0; i < references.size(); i++)
        {
                if (references.at(i) == NULL) throw ARTerror("ARTcircuit::prepareCalculation", "An element belonging to the circuit is NULL. Was it destroyed but not removed from circuit?");
                references.at(i)->PrepareCalculation();
        }


        /* Uncomment to see the circuit structure
        std::cout << "***************************************************************\n ";
                std::cout << " (Mund) ";
        for (i = 0; i < references.size(); i++)
        {
                std::cout << references.at(i)->GetName() << " ";
        }
                std::cout << " (Luft) ";
        std::cout << "\n***************************************************************\n ";
        */

        //set circuit of each element
        for (i = 0; i < references.size(); i++)
        {
                //if the reference is an element, set the circuit.
                if (dynamic_cast<ARTelement*>(references.at(i)))
                        references.at(i)->model->SetCircuit(this);
        }


        int terminatingElement = references.size() - 1;

        // RADIATION IMPEDANCE OF TERMINATING ELEMENT

        if (wavefrontRadiation) delete wavefrontRadiation;
        WaveObjectInterface* wavefrontRadiation = new WaveObjectMMImpedance(references.at(terminatingElement)->z_rad, NULL, NULL);
        references.at(terminatingElement)->RadiationImpedance(wavefrontRadiation);

        // if the terminating element can propagate impedance, the radiation imp. needs to be propagated through its model.
        if (references.at(terminatingElement)->model->FindProperty("canPropagate") )
        {
                //propagate the radiation impedance (wavefrontRadiation) through the terminating element; save the result in the terminating elements wavefrontOut structure...
                references.at(terminatingElement)->InputImpedance(wavefrontRadiation, references.at(terminatingElement)->wavefrontOut);
                //...and remember to propagate this data structure through the next model, if there is a next model
                if ((terminatingElement - 1) >= 0)
                        references.at(terminatingElement - 1)->wavefrontIn = references.at(terminatingElement)->wavefrontOut;
        }
        else // if the terminating model can not propagate impedance
        {
                //remember that the next element (if there is any) needs to propagate the terminating element's radiation impedance (= the circuits radiation imp.)
                if ((terminatingElement - 1) >= 0)
                        references.at(terminatingElement - 1)->wavefrontIn = wavefrontRadiation;
        }

        // PROPAGATION OF IMPEDANCE THROUGH CIRCUIT

        // calculate the input impedance of every element (i) from the impedance wave front of previous element (i+1... note that we are going backwards)
        for (j = terminatingElement-1; j >= 0; j--) //impedance before first element is already input imp., so only down to i=1
        {
                //set the input of the element i to the last element's output (except for the element right after the termination, it has been set already)
                if (j != terminatingElement-1)
                        references.at(j)->wavefrontIn = references.at(j+1)->wavefrontOut;
                //propagate the impedance through the element i
                references.at(j)->InputImpedance(references.at(j)->wavefrontIn, references.at(j)->wavefrontOut);
                // BTW, if references.at(i) is a circuit, the circuits InputImpedance will be called and take care of branching
        }

        // CONVERSION OF THE MULTIMODE IMPEDANCE TO IMPEDANCE CURVE

        // The output of the last element (nearest to mouth) is the circuits input impedance. We will convert it to an impedance matrix and
        // save it in the circuits impedanceCurve_ data container
        references.at(0)->wavefrontOut->ImpedanceMatrix(impedanceCurve_);

        //uncomment if you want to see the dependency tree...
        //impedanceCurve_->DebugDepTree(""); std::cout << "END\n\n";
}


void ARTcircuit::RadiationImpedance(WaveObjectInterface*& out)
{
        throw ARTerror("ARTcircuit::RadiationImpedance", "This function is not implemented. Do not use circuits as outermost elements in an instrument. Use them to simulate branches, which implies: not as outermost element!");
}

void ARTcircuit::InputImpedance(WaveObjectInterface* in, WaveObjectInterface*& out)
{
        //it is guaranteed for circuits that prepare calculation is called first, so references.at(0) contains the inputimpedance of the circuit.
        //we will take this input impedance, use the the branch model (every circuit has), plug it in - along with the other input impedance
        //passed as the in parameter and get the combined impedance in out...
        vector<WaveObjectInterface*> impedanceList;
        impedanceList.push_back(in);
        impedanceList.push_back(references.at(0)->wavefrontOut);
        model->InputImpedance(impedanceList, out);
}

