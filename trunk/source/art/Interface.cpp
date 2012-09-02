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
        (http://www.ph.ed.ac.uk/)                                         *
*    2) Inst. of Music Acoustics, Univ. of Music, Vienna, AT              *
        (http://iwk.mdw.ac.at)                                            *
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

#ifndef INTERFACE_CPP
#define INTERFACE_CPP


#include <time.h>
#include "Interface.h"
#include "ARTsimulator.h"
#include "ARTwaveObject.h"
#include "ARTmodel.h"
#include "ARTdataContainer.h"
#include "timePrototypes.h"

//**************************************************************************************************************

// ARTobject
ARTobject::ARTobject(const ARTobject& orig) //copy constructor 
	: ARTcell(orig) //copy inherited stuff from ARTcell with its own copy constructor
	{
		//std::cout << "ARTobject::ARTobject("<<GetName()<<"):copyconstr\n";
		propertyList_ = orig.propertyList_;
		piter_ = propertyList_.begin();
		while (piter_ != propertyList_.end()) 
		{
			*piter_ = (ARTproperty*)((*piter_)->clone());
			//std::cout << "ARTobject::ARTobject: " << (*piter_)->GetName() << " kopiert\n";
			++piter_;
		}

		methodList_ = orig.methodList_;
		miter_ = methodList_.begin();
		while (miter_ != methodList_.end()) 
		{
			*miter_ = (ARTmethod*)((*miter_)->clone());
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
ARTproperty* ARTobject::AppendProperty(const string name, const string sds, const string lds, const string htm) 
{
	if (FindProperty(name)) throw ARTerror("ARTobject::AppendProperty","A property with the name '%s1' has already been appended to object '%s2'.",name,name_);
	propertyList_.push_back(new ARTproperty(name,sds,lds,htm));
	piter_ = propertyList_.end();
	--piter_;
	return (ARTproperty*)(*piter_);
}

// append new data property with given name
ARTdataProp* ARTobject::AppendDataProp(const string name, ARTvariant* val, const string sds, const string lds, const string htm) 
{
	if (FindProperty(name)) throw ARTerror("ARTobject::AppendProperty","A property with the name '%s1' has already been appended to object '%s2'.",name,name_);
	//cout << "AppendDataProp: " << name << "\n";
	if (val)
	{
		propertyList_.push_back(new ARTdataProp(val->typ,val->len,name,sds,lds,htm));
		piter_ = propertyList_.end();
		--piter_;
		((ARTdataProp*)(*piter_))->SetValue(val);
		return (ARTdataProp*)(*piter_);
	}
	else
	{
		propertyList_.push_back(new ARTdataProp(C_ART_undef,-1,name,sds,lds,htm));
		piter_ = propertyList_.end();
		--piter_;
		((ARTdataProp*)(*piter_))->SetValue(NULL);
		return (ARTdataProp*)(*piter_);
	}
}

// append new data property with given name
ARTdataProp* ARTobject::AppendDataProp(ARTdataProp* dataProp) 
{
	if (FindProperty(dataProp->GetName())) throw ARTerror("ARTobject::AppendProperty","A property with the name '%s1' has already been appended to object '%s2'.",dataProp->GetName(),name_);
	propertyList_.push_back(dataProp);
	piter_ = propertyList_.end();
	--piter_;
	return (ARTdataProp*)(*piter_);
}

ARTdataProp* ARTobject::AppendDataProp(const string name, double val, const string sds, const string lds, const string htm) 
{
	if (FindProperty(name)) throw ARTerror("ARTobject::AppendProperty","A property with the name '%s1' has already been appended to object '%s2'.",name,name_);
	ARTvariant* av = new ARTvariant(val);
	propertyList_.push_back(new ARTdataProp(av->typ,av->len,name,sds,lds,htm));
	piter_ = propertyList_.end();
	--piter_;
	((ARTdataProp*)(*piter_))->SetValue(av);
	return (ARTdataProp*)(*piter_);
}
ARTdataProp* ARTobject::AppendDataProp(const string name, float val, const string sds, const string lds, const string htm) 
{
	if (FindProperty(name)) throw ARTerror("ARTobject::AppendProperty","A property with the name '%s1' has already been appended to object '%s2'.",name,name_);
	ARTvariant* av = new ARTvariant(val);
	propertyList_.push_back(new ARTdataProp(av->typ,av->len,name,sds,lds,htm));
	piter_ = propertyList_.end();
	--piter_;
	((ARTdataProp*)(*piter_))->SetValue((av));
	return (ARTdataProp*)(*piter_);
}
ARTdataProp* ARTobject::AppendDataProp(const string name, int val, const string sds, const string lds, const string htm) 
{
	if (FindProperty(name)) throw ARTerror("ARTobject::AppendProperty","A property with the name '%s1' has already been appended to object '%s2'.",name,name_);
	ARTvariant* av = new ARTvariant(val);
	propertyList_.push_back(new ARTdataProp(av->typ,av->len,name,sds,lds,htm));
	piter_ = propertyList_.end();
	--piter_;
	((ARTdataProp*)(*piter_))->SetValue((av));
	return (ARTdataProp*)(*piter_);
}
ARTdataProp* ARTobject::AppendDataProp(const string name, const string val, const string sds, const string lds, const string htm) 
{
	if (FindProperty(name)) throw ARTerror("ARTobject::AppendProperty","A property with the name '%s1' has already been appended to object '%s2'.",name,name_);
	ARTvariant* av = new ARTvariant(val.c_str());
	propertyList_.push_back(new ARTdataProp(av->typ,av->len,name,sds,lds,htm));
	piter_ = propertyList_.end();
	--piter_;
	((ARTdataProp*)(*piter_))->SetValue((av));
	return (ARTdataProp*)(*piter_);
}


// append new listableProperty with given name
ARTlistProp* ARTobject::AppendListProp(const string name, const string sds, const string lds, const string htm) {
	if (FindProperty(name)) throw ARTerror("ARTobject::AppendListProperty","A property with the name '%s1' has already been appended to object '%s2'.",name,name_);
	propertyList_.push_back(new ARTlistProp(name,sds,lds,htm));
	piter_ = propertyList_.end();
	--piter_;
	return (ARTlistProp*)(*piter_);
}

// append new method with given name
ARTmethod* ARTobject::AppendMethod(const string name, const string sds, const string lds, const string htm) {
	if (FindMethod(name)) throw ARTerror("ARTobject::AppendMethod","A method with the name '%s1' has already been appended to object '%s2'.",name,name_);
	methodList_.push_back(new ARTmethod(name,sds,lds,htm));
	miter_ = methodList_.end();
	miter_--;
	return (*miter_);
}

// find and return named property (or return NULL if no match)
ARTproperty* ARTobject::FindProperty(const string nam) 
{
	for (piter_ = propertyList_.begin(); (piter_ != propertyList_.end()); ++piter_) {
		if ((*piter_)->GetName() == nam) {
			return *piter_;
		}
	}
	return NULL;
}

// find and return named method (or return NULL if no match)
ARTmethod* ARTobject::FindMethod(const string nam) {
	for (miter_ = methodList_.begin(); (miter_ != methodList_.end()); ++miter_) {
		if ((*miter_)->GetName() == nam) {
			return *miter_;
		}
	}
	return NULL;
}

	ARTproperty* ARTobject::GetProperties(ARTproperty* pos) 
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

	bool ARTobject::DeleteProperty(ARTproperty* prp) 
	{
		::size_t sizeBefore = propertyList_.size();
		//remove all obj
		propertyList_.remove(prp);
		//if the size has changed something was removed
		if (sizeBefore > propertyList_.size()) return true;
		return false;
	}

	bool ARTobject::DeleteMethod(ARTmethod* mtd) 
	{
		::size_t sizeBefore = methodList_.size();
		//remove all obj
		methodList_.remove(mtd);
		//if the size has changed something was removed
		if (sizeBefore > methodList_.size()) return true;
		return false;
	}

// iterate through method list (pass NULL to restart iteration, receive NULL after last element)
	ARTmethod* ARTobject::GetMethods	(ARTmethod* pos) {
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
		*piter_ = (ARTproperty*)((*piter_)->clone());

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
		*miter_ = (ARTmethod*)((*miter_)->clone());
		++miter_;
	}
}

void ARTmodelInterface::SetSimulator(ARTsimulator* sim)
{
//	if (sim->GetDomain()->GetName() == "FrequencyDomain")
//	{
		simulator = dynamic_cast<ARTfreqSimulator*>(sim);
//	}
}


//**************************************************************************************************************
// ARTlistProp

// append new object with given name
ARTobject* ARTlistProp::AppendObject(const string name, const string sds, const string lds, const string htm) 
{
	objectList_.push_back(new ARTobject(name,sds,lds,htm));
	oiter_ = objectList_.end();
	--oiter_;
	return (*oiter_);
}

// append object which is already created
ARTobject* ARTlistProp::AppendObject(ARTobject* object) 
{
	objectList_.push_back(object);
	oiter_ = objectList_.end();
	--oiter_;
	return (*oiter_);
}

// find and return named object (or return NULL if no match)
ARTobject* ARTlistProp::FindObject(const string nam) 
{
	for (oiter_ = objectList_.begin(); (oiter_ != objectList_.end()); ++oiter_) {
		if ((*oiter_)->GetName() == nam) {
			return *oiter_;
		}
	}
	return NULL;
}

ARTobject* ARTlistProp::GetObjects(ARTobject* pos) 
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

bool ARTlistProp::DeleteObject(ARTobject* obj) 
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

int ARTlistProp::ReplaceObject(ARTobject* obj,ARTobject* newobj) 
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

ARTelement::ARTelement(const string name, const string sds, const string lds, const string htm, ARTmodelInterface* prototype, ARTsimulator* sim)
: ARTmodelInterface(name,sds,lds,htm),
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
			ARTproperty* prop = model->GetProperties(NULL);
			while (prop)
			{
				//if it is a data property
				ARTdataProp* dprop = dynamic_cast<ARTdataProp*>(prop);
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
	z_inp = AppendDataProp("Z_" + name, NULL, "The propagated impedance at this element's or circuit's entry.");
	z_rad = AppendDataProp("ZR_" + name, NULL, "The radiation impedance at this element's or circuit's opening.");
	wavefrontOut = new WaveObjectMMImpedance(z_inp, NULL, NULL);
}

//removes all dependencies
void ARTelement::PrepareCalculation()
{
	ARTdataProp* dprop;
	ARTproperty* prop = NULL;
	do
	{
		//if a property is found...
		if ((prop = this->GetProperties(prop)))
			//if its no PROTOTYPE prop, but a data property...
			if (!(this->IsPrototypeProperty(prop)) &&
				(dprop = dynamic_cast<ARTdataProp*>(prop)))
				{
					dprop->RemoveAllDependencies();
				}
	} 
	while(prop); //until no prop. is found, it's NULL then...
}

void ARTelement::SetScope(ARTsimulator* sim)
{
	if (sim == NULL)
		throw ARTerror("ARTelement::SetScope", "The specified simulator is invalid.");

	piter_ = propertyList_.begin();
	ARTdataProp* p;

	while (piter_ != propertyList_.end())
	{
		p = dynamic_cast<ARTdataProp*>(*piter_);
		if (p) ((ARTdataContainer*)p)->SetParser(sim->GetParser());
		piter_++;
	}
}

bool ARTelement::HasBends()
{
	if (model->FindProperty("bent")) return true;
	else return false;
}

//**************************************************************************************************************
// ARTcircuit
ARTcircuit::ARTcircuit(ARTsimulator* simulator, const string name, const string sds, const string lds, const string htm) 
	: ARTelement(name,sds,lds,htm), references(), impedanceCurve_(NULL), simulator_(simulator), wavefrontRadiation(NULL) // , wavefrontOut(NULL), wavefrontIn(NULL)

{
	//Every circuit has its impedance as data property. impedanceCurve is a shortcut pointer:
	impedanceCurve_ = AppendDataProp("InputImpedanceCurve" , NULL , "The plot of this circuit's acoustic input impedance as a vector of complex numbers");

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
//	if (!found) throw ARTerror("ARTcircuit::removeElement", "The element specified as parameter 'element' was not found.");
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
//	if (!found) throw ARTerror("ARTcircuit::replaceElement", "The element specified as parameter 'search' was not found.");
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

	// remove all dependencies 	of attached elements. This is VERY important!!! Otherwise old dependencies will be mixed with new ones, resulting in circular references and other ugly things...
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



////**************************************************************************************************************
//// ARTsimulator
//ARTsimulator::ARTsimulator(const string name,const string domain, const string wavetype,
//             const string sds, const string lds, const string htm) :
//			 ARTobject(name,sds,lds,htm), domain_(domain), wavetype_(wavetype)
//{
//	parser_ = new ParserX(mup::pckCOMPLEX_NO_STRING);
//	/*
//	The frequency grids are not supposed to be edited by the user and therefore no properties!
//	frqGrid = AppendDataProp("frqGrid", new ARTvariant(C_ART_ndbl), "The list of frequencies (in Hz) for which this simulator will calculate the impedance.");
//	wfrqGrid = AppendDataProp("wfrqGrid", new ARTvariant(C_ART_ndbl), "The list of frequencies (angular frequency) for which this simulator will calcualte the impedance.");
//	*/
//	modes = AppendDataProp("NumberOfModes", 1, "The number of modes for which this simulator will calculate the impedance.");
//
//	ARTdataProp* fmin = AppendDataProp("LowerFrequencyLimit", 50.0, "The lower frequency (in Hz) of the range for which this simulator will calculate the impedance.");
//	ARTdataProp* fmax = AppendDataProp("HigherFrequencyLimit", 1800.0, "The higher frequency (in Hz) of the range for which this simulator will calculate the impedance.");
//	ARTdataProp* fstep = AppendDataProp("FrequencyStep", 5.0, "The frequency step (in Hz) used to go through the range for which this simulator will calculate the impedance.");
//
//	//add properties to parser
//	ARTproperty* prop = GetProperties(NULL);
//	while (prop)
//	{
//		//if it is a data property
//		ARTdataProp* dprop = dynamic_cast<ARTdataProp*>(prop);
//		if (dprop)
//		{
//			string varname = dprop->GetName();
//			dprop->SetParser(parser_);
//			dprop->SetParserVar(varname);
//			//std::cout << "Created Parser Var: " << varname << "\n";
//		}
//		prop = GetProperties(prop);
//	}
//
//	frqGrid = new ARTdataContainer("frqGrid", new ARTfrqGridFunc(fmin, fmax, fstep));
//	wfrqGrid = new ARTdataContainer("wfrqGrid", new ARTwfrqGridFunc(frqGrid));
//
//}
//
//
///// Finds the data property represented by the string exp; This can be a data property of an element or model in the simulator (then the string is something like "Cyl.length") or a data property of the simulator itself.
//ARTdataProp* ARTsimulator::FindDataPropInSimulator(string exp)
//{
//	ARTdataProp* prop;
//	//try to find a property of the simulator with name *exp*
//	prop = dynamic_cast<ARTdataProp*>( FindProperty( strcrop( exp ) ));
//	//if not found, try to find a property *exp* in the element list
//	if (!prop)
//	{
//		vector<string> nameparts = strsplit(exp,'.');
//		ARTelement* element = dynamic_cast<ARTelement*>(userElements->FindObject( strcrop(nameparts[0]).c_str() ));
//		if (element == NULL) throw ARTerror("FindDataPropInSimulator", "An element of the specified name '%s1' does not exist and '%s1' is not recognized as a data property of the simulator.", strcrop(nameparts[0]).c_str() );
//		prop = dynamic_cast<ARTdataProp*>(element->model->FindProperty( strcrop(nameparts[1]).c_str() ));
//		if (prop == NULL) throw ARTerror("FindDataPropInSimulator", "The element '%s1' does not have the specified data property '%s2'.",  strcrop(nameparts[0]).c_str() ,  strcrop(nameparts[1]).c_str() );
//	}
//	return prop;
//}

//**************************************************************************************************************
// AcousticResearchTool (top level object)

AcousticResearchTool::AcousticResearchTool() : ARTobject("ART","Acoustic Research Tool","","ART.htm") 
{
//!### Developers: Update this text to reflect most recent changes !
	longDesc_  = "ART is a general purpose simulation framework and includes a growing \n"
	             "\tlibrary of acoustic modelling elements. Currently only frequency domain\n"
	             "\tmodelling is supported, work on time domain simulation is in progress.\n"
	             "\tSupported wave types are PlaneWave, SphericalWave and MultiModalWave. \n"
	             "\tAvailable elements are BoreDiscontinuity, CylindricalTube, ConicalTube,\n"
	             "\tBesselHorn, CylindricalTubeBend and ConicalTubeBend";

// -------------------------------------------------------------------------------------------------------------
// Top level properties:
	ARTdataProp* authors = new ARTdataProp(C_ART_nstr, 6, "Authors");
	ARTSetString(authors,0,"Wilfried Kausel");
	ARTSetString(authors,1,"Alistair Braden");
	ARTSetString(authors,2,"Delphine Cadefaux");
	ARTSetString(authors,3,"Vasileios Chatziioannou");
	ARTSetString(authors,4,"Sadjad Siddiq");
	ARTSetString(authors,5,"Clemens Geyer");
	AppendDataProp(authors);

	AppendDataProp("Copyright","This program is licenced according to GPL-Library ()");
	// modified by CBG: support time domain
	AppendDataProp("SimulationDomain", new ARTvariant("FrequencyDomain", "TimeDomain"));
	AppendDataProp("WaveType",new ARTvariant("SphericalWave", "PlaneWave" ,"MultiModal"));
	AppendDataProp("LosslessModel",1);

// -------------------------------------------------------------------------------------------------------------
// Top level methods:

	ARTmethod*
	met = AppendMethod("ExecuteScript","Read text file with script commands and execute them");
	met->AppendDataProp("FileName", "");

	met = AppendMethod("ReadBoreList","Read standard x,y instrument definition");
	met->AppendDataProp("FileName", "");

// -------------------------------------------------------------------------------------------------------------
// Top level listprops:
	menuGroups = AppendListProp("MenuCommands");

	ARTobject*
	obj =menuGroups->AppendObject("FileMenu");
	met =obj->AppendMethod("SaveImpedanceList");
	met->AppendDataProp("FileName","");
	met =obj->AppendMethod("ExecuteScript");
	met->AppendDataProp("FileName","");
	met =obj->AppendMethod("ReadBoreList");
	met->AppendDataProp("FileName","");

	//this is not used right now...
/*
	obj =menuGroups->AppendObject("Simulation");
	met =obj->AppendMethod("CalculateImpedanceGrid");
	met->AppendDataProp("LowerFrequencyLimit", 50.0); //0.0
	met->AppendDataProp("HigherFrequencyLimit", 90.0); //1800.0
	met->AppendDataProp("FrequencyStep", 5.0);
	met->AppendDataProp("NumberOfModes", 1);
	met =obj->AppendMethod("RefineResonances");
	met->AppendDataProp("AccuracyTarget", 0.001);

	obj =menuGroups->AppendObject("Optimisation");
	met =obj->AppendMethod("EditParameters");
	met->AppendDataProp("Parameter1", 0.0); 
	met->AppendDataProp("Parameter2", 0.0);
	met->AppendDataProp("Parameter3", 0.0);
	met->AppendDataProp("Parameter4", 0.0);
	met =obj->AppendMethod("ReadTemplate");
	met->AppendDataProp("FileName", "");
	met =obj->AppendMethod("RunRosenbrock");
	met->AppendDataProp("Iterations", 3);
	met =obj->AppendMethod("RunGenetic");
	met->AppendDataProp("Generations", 99);

	ARTlistProp*
	sublst =obj->AppendListProp("SettingsSubmenu");
	obj = sublst->AppendObject("Entry1");
	met =obj->AppendMethod("Command1");
	met->AppendDataProp("Parm1", 0.0);
	met->AppendDataProp("Parm2", 0.0);
	met =obj->AppendMethod("Command2");
	met->AppendDataProp("Parm3", 0.0);
	met->AppendDataProp("Parm4", 0.0);
	obj = sublst->AppendObject("Entry2");
	met =obj->AppendMethod("Command1");
	met->AppendDataProp("Parm5", 0.0);
	met =obj->AppendMethod("Command2");
	met->AppendDataProp("Parm6", 0.0);
	obj = sublst->AppendObject("Entry3");
	met =obj->AppendMethod("Command1");
	met->AppendDataProp("Parm7", 0.0);
	met =obj->AppendMethod("Command2");
	met->AppendDataProp("Parm8", 0.0);
	*/
	//Add a list to the object. Call it "prototypeModels" and keep the pointer to it in prototypeModels
	prototypeModels = AppendListProp("prototypeModels");
	//Add an object to the list and keep its reference obj...
	obj = prototypeModels->AppendObject(new HornElementPrototype_FD<BoreJumpSection>("BoreJump", "sound propagation in a cylindrical duct"));
	//...to add properties of the object
	obj->AppendDataProp("r1", 1.0,"left side radius [cm]");
	obj->AppendDataProp("r2", 2.0,"right side radius [cm]");
	obj->AppendDataProp("temp", 21.0,"local temperature [degC]");
	obj->AppendDataProp("humidity", 0.8,"air humidity (molar fraction)");
	obj->AppendDataProp("xc", 0.000382,"CO2 molar fraction");
	obj->AppendProperty("canPropagate","this element can propagate impedance"); 
	obj->AppendProperty("canRadiate","this radiation impedance of this element can be calculated"); 
	obj->AppendMethod("SphericalWave", "actually nothing to compute here");
	obj->AppendMethod("PlaneWave", "actually nothing to compute here");
	obj->AppendMethod("MultiModal", "modal decomposition up to n modes (Kemp02)");
	obj->AppendMethod("FrequencyDomain", "simulation in frq domain");
 
	obj = prototypeModels->AppendObject(new HornElementPrototype_FD<CylinderSection>("Cylinder", "sound propagation in a cylindrical duct"));
	obj->AppendDataProp("length", 100.0,"axial length [cm]"); 
	obj->AppendDataProp("r", 1.0,"duct radius [cm]"); 
	obj->AppendDataProp("temp", 21.0,"local temperature [degC]");
	obj->AppendDataProp("loss", 1.0,"loss factor (defaults to 1)");
	obj->AppendDataProp("humidity", 0.8,"air humidity (molar fraction)");
	obj->AppendDataProp("xc", 0.000382,"CO2 (molar fraction)");
	obj->AppendProperty("canPropagate","this element can propagate impedance"); 
	obj->AppendProperty("canRadiate","this radiation impedance of this element can be calculated"); 
	obj->AppendMethod("SphericalWave", "MapesRiordan Model");
	obj->AppendMethod("PlaneWave", "first mode only");
	obj->AppendMethod("MultiModal", "modal decomposition up to n modes (Kemp02)");
	obj->AppendMethod("FrequencyDomain", "simulation in frq domain");

	obj = prototypeModels->AppendObject(new HornElementPrototype_FD<CylinderBendSection>("BentCylinder", "sound propagation in a cylindrical duct"));
	obj->AppendDataProp("length", 100.0,"axial length [cm]");
	obj->AppendDataProp("r", 1.0,"duct radius [cm]"); 
	obj->AppendDataProp("temp", 21.0,"local temperature [degC]");
	obj->AppendDataProp("loss", 1.0,"loss factor (defaults to 1)");
	obj->AppendDataProp("bendradius", 50.0,"bend radius");
	obj->AppendDataProp("humidity", 0.8,"air humidity (molar fraction)");
	obj->AppendDataProp("xc", 0.000382,"CO2 molar fraction");
	obj->AppendProperty("bent","marks this object as bent"); //only add this prop if element is bent!!!
	obj->AppendProperty("canPropagate","this element can propagate impedance"); 
	obj->AppendProperty("canRadiate","this radiation impedance of this element can be calculated"); 
	obj->AppendMethod("SphericalWave", "MapesRiordan Model");
	obj->AppendMethod("PlaneWave", "first mode only");
	obj->AppendMethod("MultiModal", "modal decomposition up to n modes (Kemp02)");
	obj->AppendMethod("FrequencyDomain", "simulation in frq domain");

	obj = prototypeModels->AppendObject(new HornElementPrototype_FD<ConeSection>("Cone", "sound propagation in a cylindrical duct"));
	obj->AppendDataProp("length", 50.0,"axial length [cm]");
	obj->AppendDataProp("r1", 1.0,"left side radius [cm]");
	obj->AppendDataProp("r2", 5.0,"right side radius [cm]");
	obj->AppendDataProp("temp", 21.0,"local temperature [degC]");
	obj->AppendDataProp("loss", 1.0,"loss factor (defaults to 1)");
	obj->AppendDataProp("humidity", 0.8,"air humidity (molar fraction)");
	obj->AppendDataProp("xc", 0.000382,"CO2 molar fraction");
	obj->AppendProperty("canPropagate","this element can propagate impedance"); 
	obj->AppendProperty("canRadiate","this radiation impedance of this element can be calculated"); 
	obj->AppendMethod("SphericalWave", "MapesRiordan Model");
	obj->AppendMethod("PlaneWave", "first mode only");
	obj->AppendMethod("MultiModal", "modal decomposition up to n modes (Braden06)");
	obj->AppendMethod("FrequencyDomain", "simulation in frq domain");

	obj = prototypeModels->AppendObject(new HornElementPrototype_FD<ConeBendSection>("BentCone", "sound propagation in a cylindrical duct"));
	obj->AppendDataProp("length", 50.0,"axial length [cm]");
	obj->AppendDataProp("r1", 1.0,"left side radius [cm]");
	obj->AppendDataProp("r2", 5.0,"right side radius [cm]");
	obj->AppendDataProp("temp", 21.0,"local temperature [degC]");
	obj->AppendDataProp("loss", 1.0,"loss factor (defaults to 1)");
	obj->AppendDataProp("bendradius", 10.0,"bend radius");
	obj->AppendDataProp("humidity", 0.8,"air humidity (molar fraction)");
	obj->AppendDataProp("xc", 0.000382,"CO2 molar fraction");
	obj->AppendProperty("canPropagate","this element can propagate impedance"); 
	obj->AppendProperty("canRadiate","this radiation impedance of this element can be calculated"); 
	obj->AppendProperty("bent","marks this object as bent"); //only add this prop if element is bent!!!
	obj->AppendMethod("SphericalWave", "MapesRiordan Model");
	obj->AppendMethod("PlaneWave", "first mode only");
	obj->AppendMethod("MultiModal", "modal decomposition up to n modes (Braden06)");
	obj->AppendMethod("FrequencyDomain", "simulation in frq domain");


	obj = prototypeModels->AppendObject(new HornElementPrototype_FD<BesselSection>("Besselhorn", "sound propagation in a cylindrical duct"));
	obj->AppendDataProp("length", 50.0,"axial length [cm]");
	obj->AppendDataProp("r1", 1.0,"left side radius [cm]");
	obj->AppendDataProp("r2", 5.0,"right side radius [cm]");
	obj->AppendDataProp("flare", 0.4,"flare parameter");
	obj->AppendDataProp("temp", 21.0,"local temperature [degC]");
	obj->AppendDataProp("loss", 1.0,"loss factor (defaults to 1)");
	obj->AppendDataProp("humidity", 0.8,"air humidity (molar fraction)");
	obj->AppendDataProp("xc", 0.000382,"CO2 molar fraction");
	obj->AppendProperty("canPropagate","this element can propagate impedance"); 
	obj->AppendProperty("canRadiate","this radiation impedance of this element can be calculated"); 
	obj->AppendMethod("SphericalWave", "MapesRiordan Model");
	obj->AppendMethod("PlaneWave", "first mode only");
	obj->AppendMethod("MultiModal", "modal decomposition up to n modes (Braden06)");
	obj->AppendMethod("FrequencyDomain", "simulation in frq domain");

	//Append custom prototypes. All properties are set in the constructors
	obj = prototypeModels->AppendObject(new ToneHole());
	obj = prototypeModels->AppendObject(new TerminationModel());

	// The branch is not a model, it is used by ARTcircuit objects, when other ARTcircuits are inserted into the instrument.
	//obj = prototypeModels->AppendObject(new BranchModel());
	
	// register prototypes of time modules
	obj = prototypeModels->AppendObject(new ARTtimeModule());
	obj = prototypeModels->AppendObject(new impulseModule());
	obj = prototypeModels->AppendObject(new heavisideModule());
	obj = prototypeModels->AppendObject(new rectengularModule());
	obj = prototypeModels->AppendObject(new amplificationModule());
	obj = prototypeModels->AppendObject(new simpleDelayModule());
	obj = prototypeModels->AppendObject(new addModule());
	obj = prototypeModels->AppendObject(new multiplicationModule());
	obj = prototypeModels->AppendObject(new sinewaveModule());
	obj = prototypeModels->AppendObject(new fractionalDelayModule());
	obj = prototypeModels->AppendObject(new inputFunctionModule());
	obj = prototypeModels->AppendObject(new DWGcylinderModule());
	obj = prototypeModels->AppendObject(new DWGcylinderJunctionModule());
	obj = prototypeModels->AppendObject(new DWGconeModule());
	obj = prototypeModels->AppendObject(new DWGconeJunctionModule());

	//the following list has to be filled by the user
	simulators = AppendListProp("Simulators"); 

}

//--------------------------------------------------------------------------------------------------------------

AcousticResearchTool::~AcousticResearchTool() {}

//**************************************************************************************************************

#endif
