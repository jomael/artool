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

#ifndef INTERFACE_H
#define INTERFACE_H

#include "ARTmodelinterface.h"
#include "ARTcell.h"
#include "ARTobject.h"


#include <stdlib.h>
#include <list>
#include <string>
#include "mpParser.h"
#include "WindInstrument.h"
#include "mpARTValue.h"
#include "strparsing.h"
#include "ARTlink.h"
#include "ARTfunctionoid.h"
#include "ARTdataContainer.h"
//#include "ARTsimulator.h"

using mup::ParserX;

/**
 * \defgroup ARTimpl ART implementation 
 * \addtogroup ARTimpl ART implementation
 * The ART interface uses the classes in this module to simulate instruments. If you wish
 * to write your own application for the simulation of wind instruments, refer to \ref
 * sp_API. If you wish to extend the functionality of the interface, have a look at
 * the following classes.     
 *
 * @{
 */

// forward declarations
//class ARTdataContainer; -> ARTlink
class ARTobject;		
class ARTproperty;		
class ARTfunctionoid;		
class ARTmethod;		
class ARTdataProp;		
class ARTlistProp;		
class ARTelement;		
class ARTsimulator;		 
class ARTcircuit;		 

 //braucht ARTobject!

/**
 * Objects of this class are sets of strings (see ARTcell for details), that represent the 
 * property of an ARTobject. Typically each ARTobject contains several properties that are 
 * saved in a list. This list can be queried* by the programme, to check if a given object 
 * has a certain property.
 */
class ARTproperty : public ARTcell {
private:
	bool listable_;
public:
	ARTproperty(const string name, const string sds="", const string lds="", const string htm="", const bool listable=false) : 
				ARTcell(name,sds,lds,htm), listable_(listable) {}

	ARTproperty(const ARTproperty& orig) /**<copy constructor*/
	: ARTcell(orig)
	{
		listable_ = orig.listable_;
	}

	virtual ~ARTproperty() {}


	virtual ARTcell* clone() {return new ARTproperty(*this);}

	bool IsListable() { return listable_;}
};

/**
 * A value with a concrete name belonging to an ARTobject is represented by an 
 * ARTdataProperty. Just like an ARTdataContainer it can specify some way of recalculating 
 * itself. (see ARTdataContainer for details)
 */
class ARTdataProp : public ARTproperty, public ARTdataContainer {
public:
	ARTdataProp(const T_ART_Type dtyp, const int dlen, const string name, const string sds="", const string lds="", const string htm="") : 
					ARTproperty(name,sds,lds,htm,false), ARTdataContainer(dtyp, dlen, name), range(NULL) {}

	/** copy constructor */
	ARTdataProp(const ARTdataProp& orig) 
	 : ARTproperty(orig), ARTdataContainer(orig) 
	{
		if (orig.range)
			range = orig.range->clone();
		else
			range = NULL;
	}

	~ARTdataProp() 
	{
			delete range;
	}

	ARTvariant* GetRange() {return range;}
	void SetRange(ARTvariant* r) {range = r;}

	virtual ARTcell* clone() {return new ARTdataProp(*this);}
private:
	ARTvariant* range;
};

/**
 * Objects of this class represent user actions.The impedance calculation for example is 
 * an ARTmethod object, and it contains minimum and maximum frequency as well as delta 
 * frequency. When simulating, these parameters are read from there. 
 * \remark One ARTmethod will probably represent one menu command and point to a function 
 * executing the command. 
 */
class ARTmethod : public ARTobject {
protected:
	void (*methodptr_)();
public:

	ARTmethod(const string name, const string sds="", const string lds="", const string htm="")
	 : ARTobject(name,sds,lds,htm), methodptr_(NULL) 
	{}

	ARTmethod(const ARTmethod& orig) ///< copy constructor
	: ARTobject(orig)
	{}

	virtual ~ARTmethod() 
	{}

	virtual ARTcell* clone() {return new ARTmethod(*this);}

	void ApplyMethod() 
	{}
};


/**
 * An object of this class is like the blueprint of an ARTelement. It contains all properties and 
 * data properties with default values the elements created from it will have. If an 
 * ARTprototype is changed, its former descendents are not changed. Although ARTprototypes 
 * are not restricted to one simulator, calculation domain or wavetype, their properties 
 * might not allow all simulation types. An ARTprototype contains an object of a certain 
 * subclass of HornElement. That object is the implementation of the element and copies of 
 * it will be invoked when derivated ARTelements are used in calculation.
 */

class ARTprototype : public ARTobject {
protected:
	HornElement* implementation; //eine Instanz des Objekts als Schablone 
public:
	ARTprototype(const string name, const string sds="", const string lds="", const string htm="") 
	: ARTobject(name,sds,lds,htm), implementation(NULL) {}

	ARTprototype(HornElement* impl, const string name, const string sds="", const string lds="", const string htm="") 
	: ARTobject(name,sds,lds,htm), implementation(impl) {}

	virtual ~ARTprototype() 
	{
		delete implementation;
	}

	void SetImplementation(HornElement* modImp) {implementation = modImp;}
	HornElement* GetImplementation() {return implementation;}
};

/**
 * A list of objects (like all ARTelement:s as userElements) that is added as property to 
 * another ARTobject. prototypeModels for ex. is such a list, belonging to the root 
 * object, and to this list all ARTprototype:s are added. (List is then part of ARTobject's 
 * property list)
 */
class ARTlistProp : public ARTproperty {
private:
	list<ARTobject*> objectList_;
	list<ARTobject*>::iterator oiter_;
public:

	typedef list<ARTobject*>::size_type size_type;

	ARTlistProp(const string name, const string sds="", const string lds="", const string htm="") : 
				ARTproperty(name,sds,lds,htm,true),	
				objectList_(list<ARTobject*>()), 
				oiter_(objectList_.begin()) 
				{}

	virtual ~ARTlistProp () 
	{
		for (oiter_ = objectList_.begin(); (oiter_ != objectList_.end()); oiter_++) 
		{
			delete *oiter_;
		}
	}

/// iterate through object list (pass NULL to restart iteration, receive NULL after last element)
	virtual ARTobject* GetObjects(ARTobject* pos);

/// find and return named object (or return NULL if no match)
	virtual ARTobject* FindObject(const string nam) ;

/// append new object with given name
	virtual ARTobject* AppendObject(const string name, const string sds="", const string lds="", const string htm="");

/// append object which is already created
	virtual ARTobject* AppendObject(ARTobject* object);

/// delete object pos
	virtual bool DeleteObject(ARTobject* pos);

/// replace all occurences of obj with newobj @returns the number of replacements
	virtual int ReplaceObject(ARTobject* obj,ARTobject* newobj);

/// return the size of current list
	virtual size_type Size();

};


///**
// * Simulators belong to a simulation domain (frequency, time) and simulate waves of a
// * certain type (plain, spherical, multimodal).
// */
//class ARTsimulator : public ARTobject {
//private:
//	ARTproperty domain_; //frequency or (some day) time
//	ARTproperty wavetype_; //plain, spherical, multimodal
//	ParserX* parser_;
//	ARTdataContainer* frqGrid;
//	ARTdataContainer* wfrqGrid;
//	ARTdataContainer* modes;
//
//public:
//
//	ARTsimulator(const string name,const string domain="FrequencyDomain", const string wavetype="MultiModal",
//                 const string sds="", const string lds="", const string htm="");
//
//	//void SetMultimodeParameters(ARTdataProp* fmin, ARTdataProp* fmax, ARTdataProp* fstep, ARTdataProp* modes);
//
//	ARTdataContainer* GetFrequencyGrid() {return frqGrid;}
//	ARTdataContainer* GetAngularFrequencyGrid() {return wfrqGrid;}
//	ARTdataContainer* GetNumberOfModes() {return modes;}
//
//	ARTproperty* GetDomain() {return &domain_;}
//	ARTproperty* GetWavetype() {return &wavetype_;}
//
//
//	ParserX* GetParser() {return parser_;}
//
//	ARTdataProp* FindDataPropInSimulator(string exp);
//	virtual ~ARTsimulator() {}
//	//these lists point into list of AcousticResearchTool Object
//	ARTlistProp* userElements;
//	ARTlistProp* circuits;
//};

/**
 * Objects of this class represent acoustic elements. They are created in a simulator and 
 * must match the calculation domain and wavetype of the simulator. (They are saved in the 
 * simulator->userElements list) \par
 * ARTelements are derived from an ARTprototype. In fact they are copies with added 
 * properties like impedance and propagation matrix.
 */
class ARTelement : public ARTmodelInterface {
public:
	ARTdataContainer* z_rad; //Radiaton impedance
	ARTdataContainer* z_inp; //multimode input impedance

	ARTmodelInterface* model; //a clone of the prototype model
	

	WaveObjectInterface* wavefrontOut; //wave after passing this element
	WaveObjectInterface* wavefrontIn;

   
	ARTelement(const string name, const string sds="", const string lds="", const string htm="", ARTmodelInterface* prototype=NULL, ARTsimulator* sim=NULL);
//	: ARTmodelInterface(name,sds,lds,htm),
//	  //propMatrix(NULL),
//	  model(NULL),
//	  wavefrontOut(NULL),
//	  wavefrontIn(NULL)
//	{
//
//		ParserX* parser = NULL;
//		if (sim) parser = sim->GetParser();
//
//		if (prototype)
//		{
//		    model = prototype->CloneModel();
//			model->SetSimulator(sim);
//			model->CopyPropertyListEntries(prototype);
//			model->CopyMethodListEntries(prototype);
//			//Create Parser variables
//			if ( parser )
//			{
//				ARTproperty* prop = model->GetProperties(NULL);
//				while (prop)
//				{
//					//if it is a data property
//					ARTdataProp* dprop = dynamic_cast<ARTdataProp*>(prop);
//					if (dprop)
//					{
//						string varname = name_ + "." + dprop->GetName();
//						dprop->SetParser(parser);
//						dprop->SetParserVar(varname);
//						//std::cout << "Created Parser Var: " << varname << "\n";
//					}
//					prop = model->GetProperties(prop);
//				}
//			}
//		}
//		//Matrix und Impedanz ohne Funktion erstmal
//		//propMatrix = AppendDataProp("M_" + name, NULL, "The transmission matrix of this element.");
//
//		//Set the output data container of the wave object to a property we append to the object
//		z_inp = AppendDataProp("Z_" + name, NULL, "The propagated impedance at this element's or circuit's entry.");
//		z_rad = AppendDataProp("ZR_" + name, NULL, "The radiation impedance at this element's or circuit's opening.");
//		wavefrontOut = new WaveObjectMMImpedance(z_inp, NULL, NULL);
//	}


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
//		z_inp->Rename("Z_" + newname);
//		z_rad->Rename("ZR_" + newname);
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

	virtual void Pressure(WaveObjectInterface*, WaveObjectInterface*&) {throw ARTerror("ARTelement::Pressure","The function is not implemented.");};

    void SetScope(ARTsimulator* sim);
//	{
//		if (sim == NULL)
//			throw ARTerror("ARTelement::SetScope", "The specified simulator is invalid.");
//
//		piter_ = propertyList_.begin();
//		ARTdataProp* p;
//
//		while (piter_ != propertyList_.end())
//		{
//			p = dynamic_cast<ARTdataProp*>(*piter_);
//			if (p) ((ARTdataContainer*)p)->SetParser(sim->GetParser());
//			piter_++;
//		}
//	}

	virtual ARTmodelInterface* CloneModel() {return NULL;}

	bool IsPrototypeProperty(ARTproperty* p)
	{
		ARTproperty* found = NULL;
		found = model->FindProperty(p->GetName());
		if (found) return true;
		else return false;
	}
	
	virtual bool HasBends();
	//HornElement* GetImplementation() {std::cout << "Function GetImplementation in ARTelement no longer supported. It's now in the model!!\n"; return NULL;}

	virtual ~ARTelement () 
	{
		delete model;
		//delete propMatrix;	is part of properties and will be deleted in ~ARTobject
	}

};


/**
 * This class represents a wind instrument, that is built from several ARTelement:s 
 * which are put together in a certain order. ARTcircuit does not have a direct connection 
 * to a hornelement, so	prototype_ is NULL, its default value.
 */ 
class ARTcircuit : public ARTelement {
private:
	vector<ARTelement*> references; 
	ARTdataContainer* impedanceCurve_; //Input impedance as triple vector (f, re, im)
	ARTsimulator* simulator_;
	WaveObjectInterface* wavefrontRadiation; //Wave object representing the radiation impedance of the circuit. This object is constructed in prepareCalculation

public:
	ARTcircuit(ARTsimulator* simulator, const string name, const string sds="", const string lds="", const string htm="");
	
	virtual void Rename(const string newname)
	{
		model->SetName(newname);
		SetName(newname);
	}

	int GetElementPosition(ARTelement* el);
	void AppendElement(ARTelement* element);
	void AppendElementBefore(ARTelement* before, ARTelement* element);
	void AppendElementAfter(ARTelement* after, ARTelement* element);
 
	///deletes element from circuits element list, when the element itself is destroyed. This will leave a NULL-pointer in the vector, so the user knows something is missing. Always remove elements before destroying them!
	int DeleteElement(ARTelement* element);

	///removes element from circuits element list
	int RemoveElement(ARTelement* element);
	int ReplaceElement(ARTelement* search, ARTelement* replace);
	int RemoveAllElements();

	/// propagated impedance using functionoids
	virtual void PrepareCalculation();

	virtual void RadiationImpedance(WaveObjectInterface*& out);
	virtual void InputImpedance(WaveObjectInterface* in, WaveObjectInterface*& out);
	virtual bool HasBends();
	virtual ~ARTcircuit () 
	{
		//destroy branchmodel
		delete model;
		model = NULL;
	}
};


/**
 * The acoustic research tool contains all simulators, prototypes and elements, as well as 
 * menu commands.
 */
class AcousticResearchTool : public ARTobject {
protected:
public:
	ARTlistProp* menuGroups; ///<shortcut pointers inside list
	ARTlistProp* prototypeModels;///<prototype models
	ARTlistProp* simulators; 

	AcousticResearchTool();
	virtual ~AcousticResearchTool();

//	virtual ARTproperty* FindProperty(const string nam);

	void ReplaceProgressFunction(TprogressFunction f)
	{
		ARTdataContainer::progressIndicator.SetProgressFunction(f);
	}

	list<HornElement*>::iterator iter;
	ImpedanceCurve impedanceCurve;
	int *paramRefs_; 
};

/** @}
 * End of documentation group ART implementation
 */

#endif
