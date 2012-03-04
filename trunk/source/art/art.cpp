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

//IMPORTANT: Compile with run-time type information (RTTI), it's needed by the ART-interface

#ifndef ART_CPP
#define ART_CPP

#define ARTNOERROR 0
#define ARTERROR -1
#define ARTDebug 0
#define MAIN

#if defined (_MSC_VER) && _MSC_VER <= 1200
	#include "common_vs6.hpp"
#else
	#include "common.hpp"
#endif
#include "art.clp"

#include "ART.h"
#include "ARTmodel.h"
#include "ARTdataContainer.h"

#define NOERROR_ 0
#define ERROR_ -1

FILE* logfil;

#ifdef DLL
	//Exceptions can not be thrown accross dll boundaries, so we will wrap the code in each function in
	//try-catch blocks. If an exception is caught, 0 will be returned and the user can learn more about the
	//error by calling ARTGetLastErrorMessage
	#define DLL_ERRORHANDLING_BEGIN try {
	#define DLL_ERRORHANDLING_END }	   catch (ARTerror e)	{	lastError = e.GetErrorMessage();	return NULL;	}
	#define DLL_ERRORHANDLING_END_CUSTOM_RETURN(r)	}	   catch (ARTerror e)	{	lastError = e.GetErrorMessage();	return ( r ) ;	}
#else
	#define DLL_ERRORHANDLING_BEGIN
	#define DLL_ERRORHANDLING_END
	#define DLL_ERRORHANDLING_END_CUSTOM_RETURN(r)
#endif

/**************************************************************************************************/
/*                                         Static Objects                                         */
/**************************************************************************************************/
FatalError LengthErr ("empty input stream");
FatalError NumError ("numerical calculation problem");

ibinary   bin;                     // the binary input stream, usually attached to stdin
obinary   bout;                    // the binary output stream, usually attached to stdout
Statistic statistic;               // class for generating statistical information at program exit
string lastError;

//**************************************************************************************************************

//internal function! --> forward decl is here and not in header
void ARTsetLastError(string errormessage);

//**************************************************************************************************************

int             __stdcall begin_trace           (char* filename)
{
  if (art == NULL) art = new AcousticResearchTool();

#ifdef TRACE
  logfil = freopen( filename, "a+", stdout );
#endif
  return ARTNOERROR;
}

//**************************************************************************************************************

int             __stdcall end_trace             (int dummy)
{
  if (art == NULL) art = new AcousticResearchTool();

#ifdef TRACE
  fclose(logfil);
#endif
  return ARTNOERROR;
}


char *		__stdcall ARTGetLastErrorMessage	()
{
	char *s = (char*)malloc ( strlen(lastError.c_str())+1  ); //+1 for 0 character
	strcpy(s, lastError.c_str()); 
	lastError = ""; //The error was queried, empty the string!
	return s;
}

//**************************************************************************************************************

P_ART_Object    __stdcall ARTRootObject         ()
{
	DLL_ERRORHANDLING_BEGIN
	#if ARTDebug > 5
		std::cout << "ARTRootObject()\n";
	#endif
	lastError = "";
  if (art == NULL) art = new AcousticResearchTool();
  return art;
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************

bool            __stdcall ARTRootDestroy        ()
{
	DLL_ERRORHANDLING_BEGIN
  delete art;
  art = NULL;
  return 1; //No error
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************
bool             __stdcall ARTSetProgressFunction        (TprogressFunction f)
{
	DLL_ERRORHANDLING_BEGIN
	//if (art == NULL) throw ARTerror("ARTCreateSimulator", "ARTRootObject not created"); 
	if (art == NULL) art = new AcousticResearchTool();
	art->ReplaceProgressFunction(f);
	return 1; //No error
	DLL_ERRORHANDLING_END
}

bool __stdcall ARTCheckPropertyCapability(const char* property, const char* capability)
{
	DLL_ERRORHANDLING_BEGIN
	//check if art exists
	//if (art == NULL) throw ARTerror("ARTCreateSimulator", "ARTRootObject not created"); 
	if (art == NULL) art = new AcousticResearchTool();

	//check if capability is part of property
	char* possibleCapability;
	bool hasCapability = false;
	ARTdataProp* capabilities = static_cast<ARTdataProp*>(art->FindProperty(property));
	int i = 0;
	while (possibleCapability = ARTGetString(capabilities, i))
	{
		if (0 == strcmp(possibleCapability, capability))
		{
			hasCapability = true;
			break;
		}
		i++;
	}
	return hasCapability;
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************
P_ART_Simulator __stdcall ARTCreateSimulator    (const char* name, const char* domain, const char* wavetype)
{
	DLL_ERRORHANDLING_BEGIN
	#if ARTDebug > 5
		std::cout << "ARTCreateSimulator("<< name << "," << domain << "," << wavetype << ");\n";
	#endif
	//check if art exists
	//if (art == NULL) throw ARTerror("ARTCreateSimulator", "ARTRootObject not created"); 
	if (art == NULL) art = new AcousticResearchTool();

	//check if simulator with name already exists, if so throw exception. There must not be two simulators with the same name.
	ARTsimulator* simulator = dynamic_cast<ARTsimulator*>(art->simulators->FindObject(name));
	if (simulator) throw ARTerror("ARTCreateSimulator", "A simulator with the name '%s1' already exists.", name);

	//check if domain is valid 
	if (! ARTCheckPropertyCapability("SimulationDomain", domain)) 
		throw ARTerror("ARTCreateSimulator", "The specified domain is invalid.");
	
	//check if wavetype is valid
	if (! ARTCheckPropertyCapability("WaveType", wavetype)) 
		throw ARTerror("ARTCreateSimulator", "The specified wave type is invalid.");

	//create simulator
	simulator = new ARTsimulator(name,domain,wavetype);

	simulator->userElements = simulator->AppendListProp("UserElements");
	simulator->circuits = simulator->AppendListProp("Circuits"); 

	//add simulator to the list of simulators in the root object
	art->simulators->AppendObject(simulator);
	//return simulator
	return simulator;
	DLL_ERRORHANDLING_END
}

bool    __stdcall ARTDestroySimulator     (P_ART_Simulator simulator)
{
	DLL_ERRORHANDLING_BEGIN
	#if ARTDebug > 5
		std::cout << "ARTDestroySimulator("<< simulator << ");\n";
	#endif

	if (art == NULL) throw ARTerror("ARTDestroySimulator", "ART root object is NULL.");

	if (!art->simulators->DeleteObject(simulator))
		throw ARTerror("ARTDestroySimulator", "The simulator specified was not found.");
	delete simulator;
	return 1; //No error
	DLL_ERRORHANDLING_END
}

bool __stdcall ARTSetFrequencyRange    (ARTsimulator* sim, double f_min, double f_max, double f_step)
{
	DLL_ERRORHANDLING_BEGIN
	if (art == NULL) art = new AcousticResearchTool();

	ARTdataProp* fmin = static_cast<ARTdataProp*>(sim->FindProperty("LowerFrequencyLimit"));
	if (fmin == NULL) throw ARTerror("ARTSetFrequencyRange", "LowerFrequencyLimit is empty."); 
	ARTdataProp* fmax = static_cast<ARTdataProp*>(sim->FindProperty("HigherFrequencyLimit"));
	if (fmax == NULL) throw ARTerror("ARTSetFrequencyRange", "HigherFrequencyLimit is empty."); 
	ARTdataProp* fstep = static_cast<ARTdataProp*>(sim->FindProperty("FrequencyStep"));
	if (fstep == NULL) throw ARTerror("ARTSetFrequencyRange", "FrequencyStep is empty."); 
	fmin->SetValue(new ARTvariant(f_min));
	fmax->SetValue(new ARTvariant(f_max));
	fstep->SetValue(new ARTvariant(f_step));
	return 1; //No error
	DLL_ERRORHANDLING_END
}



bool __stdcall ARTSetNModes    (ARTsimulator* sim, int Nmodes)
{
	DLL_ERRORHANDLING_BEGIN
	ARTdataProp* modes = static_cast<ARTdataProp*>(sim->FindProperty("NumberOfModes"));
	if (modes == NULL) throw ARTerror("ARTSetNModes", "Property 'modes' is empty."); 
	modes->SetValue(new ARTvariant(Nmodes));
	return 1; //No error
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************
 
P_ART_Element    __stdcall ARTCreateElement     (P_ART_Simulator simulator, char* name, char* type)
{
	DLL_ERRORHANDLING_BEGIN
	#if ARTDebug > 5
		std::cout << "ARTCreateElement("<< simulator << "," << name << "," << type << ");\n";
	#endif
	//check if art and simulator are a valid objects
	//if (art == NULL) throw ARTerror("ARTCreateElement", "ARTRootObject not created"); 
	if (art == NULL) art = new AcousticResearchTool();
	if (simulator == NULL) throw ARTerror("ARTCreateElement", "Invalid simulator"); 

	//Elements with the same name will cause problems later on, so check that the name is free
	ARTelement* newElement_a = static_cast<ARTelement*>(simulator->userElements->FindObject(name));
	if (newElement_a) throw ARTerror("ARTCreateElement", "An element with the name '%s1' already exists.", name);

	ARTprototype* prototype = (ARTprototype*)(art->prototypeModels->FindObject(type));

	if (prototype != NULL)
	{
		//check if model features simulator's domain and wavetype
		ARTmethod* method = prototype->FindMethod(simulator->GetDomain()->GetName().c_str());
		if (method == NULL) throw ARTerror("ARTCreateElement", "The prototype model does not support the domain of the simulator.");

		method = prototype->FindMethod(simulator->GetWavetype()->GetName().c_str());
		if (method == NULL) throw ARTerror("ARTCreateElement", "The prototype model does not support the wavetype of the simulator.");

		ARTelement* newElement = new ARTelement(name,"","","",(ARTmodelInterface*)prototype, simulator);
		
		simulator->userElements->AppendObject(newElement);
		return newElement;
	      
	}
	else //if a model of "name" was not found in the list: 
		throw ARTerror("ARTCreateElement", "A prototype model of the specified type does not exist.");
	DLL_ERRORHANDLING_END
}

P_ART_Element    __stdcall ARTFindElement     (P_ART_Simulator simulator, char* name)
{
	//Find the element
	ARTelement* element = static_cast<ARTelement*>(simulator->userElements->FindObject(name));
	return element;
}

P_ART_Circuit    __stdcall ARTFindCircuit     (P_ART_Simulator simulator, char* name)
{
	//Find the circuit
	ARTcircuit* circuit = static_cast<ARTcircuit*>(simulator->circuits->FindObject(name));
	return circuit;
}

//P_ART_Element    __stdcall ARTChangeElementModel     (P_ART_Simulator simulator, char* name, char* type)
P_ART_Element    __stdcall ARTChangeElementModel     (P_ART_Simulator simulator, P_ART_Element element, char* type)
{
	DLL_ERRORHANDLING_BEGIN
	#if ARTDebug > 5
		std::cout << "ARTChangeElementType("<< simulator << "," << name << "," << type << ");\n";
	#endif
	//check if art and simulator are valid objects
	if (art == NULL) art = new AcousticResearchTool();
	if (simulator == NULL) throw ARTerror("ARTChangeElementType", "Invalid simulator"); 
	if (element == NULL) throw ARTerror("ARTChangeElementType", "Element is NULL");

	//Find the prototype
	ARTmodelInterface* prototype = (ARTmodelInterface*)(art->prototypeModels->FindObject(type));

	//if the prototype was found and if it is not the same as the prototype of the element, then continue
	if ((prototype != NULL) && (strcmp(prototype->GetName().c_str(),element->model->GetName().c_str()) != 0))
	{
		//check if model features simulator's domain and wavetype
		ARTmethod* method = prototype->FindMethod(simulator->GetDomain()->GetName().c_str());
		if (method == NULL) throw ARTerror("ARTChangeElementType", "The prototype model does not support the domain of the simulator.");

		method = prototype->FindMethod(simulator->GetWavetype()->GetName().c_str());
		if (method == NULL) throw ARTerror("ARTChangeElementType", "The prototype model does not support the wavetype of the simulator.");

		//save list of all datacontainers dependent on properties of this one
		list<ARTdataContainer*> allclients;
		ARTproperty* prop = NULL;
		while (prop = element->model->GetProperties(prop))
		{
			//if it is a data property 
			ARTdataProp* dprop = dynamic_cast<ARTdataProp*>(prop);
			if (dprop)
			{
				list<ARTdataContainer*> tmplist = dprop->GetClientList();
				//get copy of clients, append to allclients list
				allclients.splice(allclients.end(),tmplist);
			}
		}
		//different datacontainer
		//Change the prototype
		//1.remove dependencies and remove parser vars
		//-> happens in destructor of datacontainer!

		//2.destroy old model
		delete element->model;

		//3.create new model
	    element->model = prototype->CloneModel();
		element->model->SetSimulator(simulator);
		element->model->CopyPropertyListEntries(prototype);
		element->model->CopyMethodListEntries(prototype);
		//4.create parservars for new model
		ParserX* parser = NULL;
		parser = simulator->GetParser();

		if ( parser )
		{
			ARTproperty* prop = element->model->GetProperties(NULL);
			while (prop)
			{
				//if it is a data property 
				ARTdataProp* dprop = dynamic_cast<ARTdataProp*>(prop);
				if (dprop)
				{
					string varname = string(element->GetName()) + "." + dprop->GetName();
					dprop->SetParser(parser);
					dprop->CreateParserVar(varname);
					//std::cout << "Created Parser Var: " << varname << "\n";
				}
				prop = element->model->GetProperties(prop);
			}
		}

		//now go through definition strings of all clients and renew dependencies
		//This is necessary because: If some DC was dependent on a data property R1 of this element's model, the data property
		//was removed from its dependency list when the model was destroyed (by the DC destructor). If the new model also has
		//a data property R1 (eg. if a Cone is changed into a Bessel horn), the Parser will still know a variable Elementname.R1
		//and the data property dependent on R1 will be evaluated *but* R1 won't be in its dependency list any more. So it has to
		//be reinserted!
		list<ARTdataContainer*>::iterator it;
		for (it=allclients.begin(); it!=allclients.end(); it++)
		{
			(*it)->RedoDefinitionDependencies();
		} 

		return element;
	}
	else //if a model of "name" was not found in the list: 
		throw ARTerror("ARTChangeElementType", "A prototype model of the specified type does not exist.");
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************

P_ART_Object	__stdcall	ARTGetModel	(P_ART_Element  element)
{
	DLL_ERRORHANDLING_BEGIN
  if (element == NULL) throw ARTerror("ARTGetProperties", "element is NULL.");
  else return element->model;
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************

P_ART_Element    __stdcall ARTChangeName     (P_ART_Element element, char* newName)
{
	DLL_ERRORHANDLING_BEGIN
	#if ARTDebug > 5
		std::cout << "ARTChangeName("<< simulator << "," << name << "," << newName << ");\n";
	#endif
	//check if art and simulator are valid objects
	if (art == NULL) art = new AcousticResearchTool();
	if (element == NULL) throw ARTerror("ARTChangeName", "Element is NULL");

	element->Rename(string(newName));
	return element;

	DLL_ERRORHANDLING_END
}



bool    __stdcall ARTDestroyElement     (P_ART_Simulator simulator,P_ART_Element element)
{
	DLL_ERRORHANDLING_BEGIN
	#if ARTDebug > 5
		std::cout << "ARTDestroyElement("<< element << ");\n";
	#endif

	//check for references to this element (in circuits) and set them to NULL
	int c = 0;
	ARTcircuit* circuit = (ARTcircuit*)simulator->circuits->GetObjects(NULL);
	while (circuit)
	{
		c += circuit->DeleteElement( (ARTelement*) element);
		//get next circuit
		circuit = (ARTcircuit*)simulator->circuits->GetObjects(circuit);
	}

	//if (c > 0) Warning: Elements that are parts of circuits were deleted!

	if (!simulator->userElements->DeleteObject(element))
		throw ARTerror("ARTDestroyElement", "The element specified was not found. Did you already delete it?");

	delete element;
	return 1; //No error
	DLL_ERRORHANDLING_END
}


//**************************************************************************************************************
P_ART_DataProp    __stdcall ARTSetParameter     (P_ART_Simulator simulator, char* command)
{
	DLL_ERRORHANDLING_BEGIN
	#if ARTDebug > 5
		std::cout << "ARTSetParameter("<< simulator << "," << command << ");\n";
	#endif
	if (simulator == NULL) throw ARTerror("ARTSetParameter", "Invalid simulator"); 
 	if (simulator->GetParser() == NULL) throw ARTerror("ARTSetParameter", "The simulator's parser is NULL");
	//command string can contain more than one command sperated by ; -> cut string into commands
	vector<string> commands = strsplit(command,';');
	ARTdataProp* prop;
	for (int i = 0; i < commands.size(); i++)
	{
		vector<string> expressions = strsplit(commands[i],'=');
		string s = strcrop(expressions[0]);
		//check if the string has some content
		if (s != "")
		{
			prop = simulator->FindDataPropInSimulator(s); //this func. will throw error if not found
			ARTdataContainer* dc = (ARTdataContainer*)prop;
			//if the datacontainer is of type C_ART_str or C_ART_str, set the value of the string
			if ((dc->GetDatatype() == C_ART_str) || (dc->GetDatatype() == C_ART_nstr))
			{
				ARTSetString(prop, 0, strcrop(expressions[1]).c_str());
				dc->NotifyClients(); //and tell dependent data containers there's a new value here!
			}
			//otherwise set the parser definition of the string
			else
				dc->SetDefinition(commands[i].c_str(), simulator);
		}
	}
	return prop;
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************

P_ART_Circuit    __stdcall ARTCreateCircuit     (P_ART_Simulator simulator, char* name)
{
	DLL_ERRORHANDLING_BEGIN
	//check if the simulator is a valid object
	if (simulator == NULL) throw ARTerror("ARTCreateCircuit", "Invalid simulator"); 

	//Circuits with the same name will cause problems later on, so check that the name is free
	ARTcircuit* newCircuit = static_cast<ARTcircuit*>(simulator->circuits->FindObject(name));
	if (newCircuit) throw ARTerror("ARTCreateCircuit", "A circuit with the name '%s1' already exists.", name);
	
	newCircuit = new ARTcircuit(simulator,name,"","","");

	simulator->circuits->AppendObject(newCircuit);

	return newCircuit;
	DLL_ERRORHANDLING_END
	
}

bool    __stdcall ARTDestroyCircuit     (P_ART_Simulator simulator,P_ART_Circuit circuit)
{
	DLL_ERRORHANDLING_BEGIN
	#if ARTDebug > 5
		std::cout << "ARTDestroyCircuit("<< circuit << ");\n";
	#endif

	if (!simulator->circuits->DeleteObject(circuit))
		throw ARTerror("ARTDestroyCircuit", "The circuit specified was not found.");
	delete circuit;
	return 1; //No error
	DLL_ERRORHANDLING_END
}


//int	__stdcall	ARTGetReferencePosition	(P_ART_Simulator simulator, P_ART_Circuit circuit, char* name)
int	__stdcall	ARTGetReferencePosition	(P_ART_Circuit circuit, P_ART_Element element)
{
	DLL_ERRORHANDLING_BEGIN

	if (circuit == NULL) throw ARTerror("ARTAppendReference", "Invalid circuit"); 

	if (element == NULL) throw ARTerror("ARTAppendReference", "Invalid element");

	return circuit->GetElementPosition(element);
	DLL_ERRORHANDLING_END_CUSTOM_RETURN( -1 )
}
//**************************************************************************************************************

//P_ART_Object    __stdcall ARTAppendReference     (P_ART_Simulator simulator, P_ART_Circuit circuit, char* name)
P_ART_Object	__stdcall	ARTAppendReference	( P_ART_Circuit circuit, P_ART_Element reference)
{
	DLL_ERRORHANDLING_BEGIN
	//check if the circuit is valid
	if (circuit == NULL) throw ARTerror("ARTAppendReference", "Invalid circuit"); 
	if (reference == NULL) throw ARTerror("ARTAppendReference", "Invalid element"); 

	//append the element or circuit
	circuit->AppendElement(reference);

	return reference;
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************

P_ART_Object	__stdcall	ARTAppendReferenceBefore	(P_ART_Circuit circuit, P_ART_Element referenceAfter, P_ART_Element reference)
//P_ART_Object    __stdcall ARTAppendReferenceBefore     (P_ART_Simulator simulator, P_ART_Circuit circuit, char*elementBefore, char* name)
{
	DLL_ERRORHANDLING_BEGIN
	//check if the circuit is valid
	if (circuit == NULL) throw ARTerror("ARTAppendReference", "Invalid circuit"); 
	if (reference == NULL) throw ARTerror("ARTAppendReference", "Invalid reference passed in parameter reference"); 
	if (referenceAfter == NULL) throw ARTerror("ARTAppendReference", "Invalid reference passed in parameter referenceBefore"); 

	circuit->AppendElementBefore(referenceAfter, reference);

	return reference;
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************

P_ART_Object	__stdcall	ARTAppendReferenceAfter	(P_ART_Circuit circuit, P_ART_Element referenceBefore, P_ART_Element reference)
//P_ART_Object    __stdcall ARTAppendReferenceAfter     (P_ART_Simulator simulator, P_ART_Circuit circuit, char*elementAfter, char* name)
{
	DLL_ERRORHANDLING_BEGIN
	//check if the circuit is valid
	if (circuit == NULL) throw ARTerror("ARTAppendReference", "Invalid circuit"); 

	if (reference == NULL) throw ARTerror("ARTAppendReference", "reference is NULL"); 
	if (referenceBefore == NULL) throw ARTerror("ARTAppendReference", "referenceBefore is NULL"); 

	circuit->AppendElementAfter(referenceBefore, reference);

	return reference;
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************
//int    __stdcall      ARTRemoveReference     (P_ART_Simulator simulator, P_ART_Circuit circuit, char*element)
int	__stdcall	ARTRemoveReference	(P_ART_Circuit circuit, P_ART_Element reference)
{
	DLL_ERRORHANDLING_BEGIN
	//check if the circuit is valid
	if (circuit == NULL) throw ARTerror("ARTAppendReference", "Invalid circuit"); 
	if (reference == NULL) throw ARTerror("ARTAppendReference", "reference is NULL"); 

	return circuit->RemoveElement(reference);
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************
//int    __stdcall ARTReplaceReference     (P_ART_Simulator simulator, P_ART_Circuit circuit, char*search, char* replace)
int	__stdcall	ARTReplaceReference	(P_ART_Circuit circuit, P_ART_Element search, P_ART_Element replace)
{
	DLL_ERRORHANDLING_BEGIN
	//check if the circuit is valid
	if (circuit == NULL) throw ARTerror("ARTAppendReference", "Invalid circuit"); 
	if (search == NULL) throw ARTerror("ARTAppendReference", "search is NULL"); 
	if (replace == NULL) throw ARTerror("ARTAppendReference", "replace is NULL"); 

	return circuit->ReplaceElement(search, replace);
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************
int    __stdcall ARTRemoveAllReferences     (P_ART_Circuit circuit)
{
	DLL_ERRORHANDLING_BEGIN
	//check if the circuit is valid
	if (circuit == NULL) throw ARTerror("ARTAppendReference", "Invalid circuit"); 

	return circuit->RemoveAllElements();
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************

P_ART_DataProp    __stdcall ARTInputImpedance     (P_ART_Circuit circuit)
{
	DLL_ERRORHANDLING_BEGIN
	if (circuit == NULL) throw ARTerror("ARTInputImpedance", "Invalid circuit"); 

	ARTdataProp* impCurveProp = static_cast<ARTdataProp*>(circuit->FindProperty("InputImpedanceCurve"));
	if (impCurveProp == NULL) throw ARTerror("ARTInputImpedance", "InputImpedanceCurve of circuit '%s1' is NULL.", circuit->GetName()); 
	
	circuit->PrepareCalculation(); 

	//Estimate evaluation cost
	//get evaluation cost for var in functionoid/parser calls; this cost depends on the number of frq. considered
	int ec = impCurveProp->GetEvaluationCost();

	//the evaluate functions call a static progress indicator which calls the progressFunction and lets it know how much percent of the calculation are done
	ARTdataContainer::progressIndicator.Reset(ec);
	ARTdataContainer::progressIndicator.CallProgressfunction(true);
	try
	{
		impCurveProp->GetValue();
	}
	catch (ARTabort a)
	{ 
		ARTdataContainer::progressIndicator.CallProgressfunction(false);
		lastError = "User aborted";
		//Reset evaluation flags so no data container continues to think we are evaluating its value
		impCurveProp->ResetEvaluation();
		return NULL;
		//std::cout << "User aborted\n";
	}
	catch (ARTerror e)
	{ 
		ARTdataContainer::progressIndicator.CallProgressfunction(false);
		//Reset evaluation flags so no data container continues to think we are evaluating its value
		impCurveProp->ResetEvaluation();
		//...and rethrow the error
		throw e;
	}
	ARTdataContainer::progressIndicator.CallProgressfunction(false);
	//cout << "Evaluation Cost was: " << ec <<  " function calls / evaluated parser expressions\n";

	return  impCurveProp;
	DLL_ERRORHANDLING_END
}


const char*           __stdcall ARTGetName            (P_ART_Cell  pobj)
{
	DLL_ERRORHANDLING_BEGIN
	if (pobj == NULL) throw ARTerror("ARTGetName","pobj is NULL");
	else return (char*)pobj->GetName().c_str(); 
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************

const char*           __stdcall ARTGetShortDescription(P_ART_Cell  pobj)
{
	DLL_ERRORHANDLING_BEGIN
  if (pobj == NULL) throw ARTerror("ARTGetShortDescription","pobj is NULL");
  else return (char*)pobj->GetShortDescription().c_str();
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************

const char*           __stdcall ARTGetLongDescription (P_ART_Cell  pobj)
{
	DLL_ERRORHANDLING_BEGIN
  if (pobj == NULL) throw ARTerror("ARTGetLongDescription","pobj is NULL");
  else return (char*)pobj->GetLongDescription().c_str();
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************

const char*           __stdcall ARTGetHelpFilename    (P_ART_Cell  pobj)
{
	DLL_ERRORHANDLING_BEGIN
  if (pobj == NULL) throw ARTerror("ARTGetHelpFilename","pobj is NULL");
  else return (char*)pobj->GetHelpFilename().c_str();
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************

bool            __stdcall ARTIsListable         (P_ART_Property pprp)
{
	DLL_ERRORHANDLING_BEGIN
  if (pprp == NULL) throw ARTerror("ARTIsListable","pobj is NULL");
  else return pprp->IsListable();
	DLL_ERRORHANDLING_END
}


//**************************************************************************************************************

bool	__stdcall	ARTIsDataProp	(P_ART_Property pprp) 
{
	if (dynamic_cast<P_ART_DataProp>(pprp)) return true;
		else return false;
}

//**************************************************************************************************************

T_ART_Type      __stdcall ARTGetDatatype        (P_ART_Variant pprp)
{
	DLL_ERRORHANDLING_BEGIN
	if (pprp == NULL) throw ARTerror("ARTGetDatatype","pprp is NULL");
	else return pprp->typ;
	DLL_ERRORHANDLING_END_CUSTOM_RETURN( C_ART_undef )
}

//**************************************************************************************************************

int             __stdcall ARTGetLength          (P_ART_Variant pprp)
{
	DLL_ERRORHANDLING_BEGIN
	if (pprp == NULL) throw ARTerror("ARTGetLength","pprp is NULL");
	else return pprp->len;
	DLL_ERRORHANDLING_END_CUSTOM_RETURN( -1 )
}

//**************************************************************************************************************

P_ART_Variant   __stdcall ARTGetValue           (P_ART_DataProp pprp)
{
	DLL_ERRORHANDLING_BEGIN
  if (pprp == NULL) throw ARTerror("ARTGetValue","pprp is NULL");
  return (ARTvariant*)pprp;
	//Warum wurde hier kopiert??
/*  P_ART_Variant res = new ARTvariant;
  res->len = pprp->len;
  res->typ = pprp->typ;
  T_ART_Var* v=pprp->val; 
  if                (res->typ == C_ART_str) res->val->s = v->s;
  else if   (res->typ == C_ART_int) res->val->i = v->i;
  else if   (res->typ == C_ART_flo) res->val->f = v->f;
  else if   (res->typ == C_ART_dbl) res->val->f = v->d;
  else if   (res->typ == C_ART_cpx) res->val->c = v->c;
  else if   (res->typ == C_ART_tri) res->val->t = v->t;
  else if   (res->typ == C_ART_mat) res->val->m = v->m;
  else if   (res->typ == C_ART_matx) res->val->mx = v->mx;
  else if   (res->typ == C_ART_nstr) res->val->ns = v->ns;
  else if   (res->typ == C_ART_nint) res->val->ni = v->ni;
  else if   (res->typ == C_ART_nflo) res->val->nf = v->nf;
  else if   (res->typ == C_ART_ndbl) res->val->nd = v->nd;
  else if   (res->typ == C_ART_ncpx) res->val->nc = v->nc;
  else if   (res->typ == C_ART_ntri) res->val->nt = v->nt;
  else if   (res->typ == C_ART_nmat) res->val->nm = v->nm;
  else if   (res->typ == C_ART_nmatx) res->val->nmx = v->nmx;
  return res;*/
	DLL_ERRORHANDLING_END
}

P_ART_Variant   __stdcall ARTGetRange           (P_ART_DataProp pprp)
{
	DLL_ERRORHANDLING_BEGIN
	if (pprp == NULL) throw ARTerror("ARTGetRange","pprp is NULL");
	return pprp->GetRange();
	DLL_ERRORHANDLING_END
}

char*          __stdcall ARTGetDefinitionString          (P_ART_DataProp dc)
{
	DLL_ERRORHANDLING_BEGIN
	if (dc == NULL) throw ARTerror("ARTGetDefinitionString","dc is NULL");
	string s = dc->GetDefinition();
	if (s == "") return NULL;
	else 
	{
		// cbg: workaround for VS2010
		#if defined(_MSC_VER) && _MSC_VER > 1200
		return (char*)strdup(s.c_str()); //this points to the string the data container holds!
		#else
		return (char*)s.c_str(); //this points to the string the data container holds!
		#endif
	}
	DLL_ERRORHANDLING_END
}

char*          __stdcall ARTGetString          (P_ART_Variant pprp, int idx)
{
	DLL_ERRORHANDLING_BEGIN
	if (pprp == NULL) throw ARTerror("ARTGetString","pprp is NULL");

	else if (pprp->typ == C_ART_str)
	{
	  return (pprp->val->s);
	}
	else if (pprp->typ == C_ART_nstr)
	{
	  if (idx < pprp->len)
		return (pprp->val->ns[idx]);
	  else
		throw ARTerror("ARTGetString","The index is out of bounds.");
	}
	else throw ARTerror("ARTGetString","The property is not of type C_ART_str or C_ART_nstr.");
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************

bool            __stdcall ARTSetString          (P_ART_Variant pprp, int idx, const char* s)
{
	DLL_ERRORHANDLING_BEGIN
	if (pprp != NULL) 
	{
		if (pprp->typ == C_ART_str) pprp->SetVal(s);
		else 
			if (pprp->typ == C_ART_nstr) pprp->SetVal(s,idx); //range checking happens in this function
				else throw ARTerror("ARTSetString","The property pprp is not of type C_ART_str or C_ART_nstr.");
		return 1; //success
	}
	else
		throw ARTerror("ARTSetString","pprp is NULL");
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************

int            __stdcall ARTGetInteger         (P_ART_Variant pprp, int idx)
{
	DLL_ERRORHANDLING_BEGIN
  if (pprp == NULL) throw ARTerror("ARTGetInteger","pprp is NULL");
  else if (pprp->typ == C_ART_int)    return (pprp->val->i);
  else if (pprp->typ == C_ART_nint)
  {   
	  if (idx < pprp->len)
		return (pprp->val->ni[idx]);
	  else
		throw ARTerror("ARTGetInteger","The index is out of bounds.");
  }
  else throw ARTerror("ARTGetInteger","pprp is not of type C_ART_int or C_ART_nint");
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************

bool            __stdcall ARTSetInteger         (P_ART_Variant pprp, int idx, int i)
{
	DLL_ERRORHANDLING_BEGIN
	if (pprp != NULL) pprp->SetVal(i,idx);
		else throw ARTerror("ARTSetInteger", "pprp is NULL."); //checks vor validity of args in that function
	return 1; //No error
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************

float         __stdcall ARTGetFloat           (P_ART_Variant pprp, int idx)
{
	DLL_ERRORHANDLING_BEGIN
  if (pprp == NULL) return NULL;
  else if (pprp->typ == C_ART_flo)    return (pprp->val->f);
  else if (pprp->typ == C_ART_nflo)
  {   
	  if (idx < pprp->len)
		return (pprp->val->nf[idx]);
	  else
		throw ARTerror("ARTGetFloat","The index is out of bounds.");
  }
  else return NULL;
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************

bool            __stdcall ARTSetFloat           (P_ART_Variant pprp, int idx, float f)
{
	DLL_ERRORHANDLING_BEGIN
	if (pprp != NULL) pprp->SetVal(f,idx);
		else throw ARTerror("ARTSetFloat", "pprp is NULL."); //checks vor validity of args in that function
	return 1; //No error
//	pprp->NotifyClients(); //and tell dependent data containers there's a new value here!
	DLL_ERRORHANDLING_END
}
//**************************************************************************************************************

double         __stdcall ARTGetDouble           (P_ART_Variant pprp, int idx)
{
	DLL_ERRORHANDLING_BEGIN
  if (pprp == NULL) return NULL;
  else if (pprp->typ == C_ART_dbl)    return (pprp->val->d);
  else if (pprp->typ == C_ART_ndbl)
  {   
	  if (idx < pprp->len)
		return (pprp->val->nd[idx]);
	  else
		throw ARTerror("ARTGetDouble","The index is out of bounds.");
  }
  else return NULL;
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************

bool            __stdcall ARTSetDouble           (P_ART_Variant pprp, int idx, double d)
{
	DLL_ERRORHANDLING_BEGIN
	if (pprp != NULL) pprp->SetVal(d,idx);
		else throw ARTerror("ARTSetDouble", "pprp is NULL."); //checks vor validity of args in that function
	return 1; //No error
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************

T_ART_Cmplx    __stdcall ARTGetComplex         (P_ART_Variant pprp, int idx)
{
	T_ART_Cmplx r;
	#ifdef DLL
		try 
		{
	#endif
			if (pprp == NULL) 
				throw ARTerror("ARTGetComplex", "pprp is NULL.");
			if (pprp->typ == C_ART_cpx)
				return (pprp->val->c);
			else if (pprp->typ == C_ART_ncpx)   
			  {   
				  if (idx < pprp->len)
					return (pprp->val->nc[idx]);
				  else
					throw ARTerror("ARTGetComplex","The index is out of bounds.");
			  }

			else 
				throw ARTerror("ARTGetComplex", "The property's type is not C_ART_cpx nor C_ART_ncpx.");
	#ifdef DLL
		}	
		catch (ARTerror e)	
		{
			lastError = e.GetErrorMessage();
			r.im=0;
			r.re=0;
		}
	#endif
	//return some dummy value...
	return ( r ); //Does not work if it's in catch block ... Why???*/
}

//**************************************************************************************************************
bool            __stdcall ARTSetComplex         (P_ART_Variant pprp, int idx, T_ART_Cmplx c)
{
	DLL_ERRORHANDLING_BEGIN
	if (pprp != NULL) pprp->SetVal(c.re,c.im,idx);
		else throw ARTerror("ARTSetComplex", "pprp is NULL."); //checks vor validity of args in that function
	return 1; //No error
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************

T_ART_Tripl    __stdcall ARTGetTriple          (P_ART_Variant pprp, int idx)
{
	DLL_ERRORHANDLING_BEGIN
  if (pprp == NULL) throw ARTerror("ARTGetTriple", "The property is invalid.");
  else if (pprp->typ == C_ART_tri)    return (pprp->val->t);
  else if (pprp->typ == C_ART_ntri)
	{   
	  if (idx < pprp->len)
	     return (pprp->val->nt[idx]);
	  else
		throw ARTerror("ARTGetTriple","The index is out of bounds.");
	}
  else throw ARTerror("ARTGetTriple", "The property's type is not C_ART_ntri.");
	DLL_ERRORHANDLING_END_CUSTOM_RETURN( T_ART_Tripl() )
}

//**************************************************************************************************************

bool            __stdcall ARTSetTriple          (P_ART_Variant pprp, int idx, T_ART_Tripl t)
{
	DLL_ERRORHANDLING_BEGIN
  if (pprp != NULL) {
    if      (pprp->typ == C_ART_tri)  pprp->val->t = t;
    else if (pprp->typ == C_ART_ntri) 
	{
	  if (idx < pprp->len)
		pprp->val->nt[idx] = t;
	  else
		throw ARTerror("ARTSetMatrix","The index is out of bounds.");
	}	
  }
	return 1; //No error
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************

T_ART_Matrix   __stdcall ARTGetMatrix          (P_ART_Variant pprp, int idx)
{
	DLL_ERRORHANDLING_BEGIN
  if (pprp == NULL) throw ARTerror("ARTGetMatrix", "The property is invalid.");
  else if (pprp->typ == C_ART_mat)    return (pprp->val->m);
  else if (pprp->typ == C_ART_nmat)
	{   
	  if (idx < pprp->len)
	     return  (pprp->val->nm[idx]);
	  else
		throw ARTerror("ARTGetMatrix","The index is out of bounds.");
	}
  else throw ARTerror("ARTGetMatrix", "The property's type is not C_ART_nmat.");
	DLL_ERRORHANDLING_END_CUSTOM_RETURN( T_ART_Matrix() )

}

//**************************************************************************************************************

bool            __stdcall ARTSetMatrix          (P_ART_Variant pprp, int idx, T_ART_Matrix m)
{
	DLL_ERRORHANDLING_BEGIN
  if (pprp != NULL) {
    if      (pprp->typ == C_ART_mat)  pprp->val->m = m;
    else if (pprp->typ == C_ART_nmat) 
	{
	  if (idx < pprp->len)
		pprp->val->nm[idx] = m;
	  else
		throw ARTerror("ARTSetMatrix","The index is out of bounds.");
	}	
  }
	return 1; //No error
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************

P_ART_Property  __stdcall ARTFindProperty       (P_ART_Object  host, char* nam)
{
	DLL_ERRORHANDLING_BEGIN
  if (host == NULL) throw ARTerror("ARTFindProperty", "host is NULL.");
  else return host->FindProperty(nam);
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************

P_ART_DataProp  __stdcall ARTFindDataProperty       (P_ART_Object  host, char* nam)
{
	DLL_ERRORHANDLING_BEGIN
  if (host == NULL) throw ARTerror("ARTFindDataProperty", "host is NULL.");
  else return dynamic_cast<ARTdataProp*>(host->FindProperty(nam));
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************

P_ART_Method    __stdcall ARTFindMethod         (P_ART_Object  host, char* nam)
{
	DLL_ERRORHANDLING_BEGIN
  if (host == NULL) throw ARTerror("ARTFindMethod", "host is NULL.");
  else return host->FindMethod(nam);
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************

P_ART_Object    __stdcall ARTFindObject         (P_ART_ListProp host, char* nam)
{
	DLL_ERRORHANDLING_BEGIN
  if (host == NULL) throw ARTerror("ARTFindObject", "host is NULL.");
  else return host->FindObject(nam);
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************

P_ART_Property  __stdcall ARTGetProperties      (P_ART_Object  host, P_ART_Property pos)
{
	DLL_ERRORHANDLING_BEGIN
  if (host == NULL) throw ARTerror("ARTGetProperties", "host is NULL.");
  else return host->GetProperties(pos);
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************

P_ART_DataProp  __stdcall ARTGetDataProperties      (P_ART_Object  host, P_ART_DataProp pos)
{
	DLL_ERRORHANDLING_BEGIN
  if (host == NULL) throw ARTerror("ARTGetDataProperties", "host is NULL.");
  else 
	{
		ARTproperty* p = pos;
		ARTdataProp* dp;
		//cycle though properties until a data property is found, but only as long as there are properties!
		do 
		{
			p = host->GetProperties(p);
		} 
		while ( !(dp = dynamic_cast<ARTdataProp*>(p) ) && p);
		return dp;
	}
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************

P_ART_Method    __stdcall ARTGetMethods         (P_ART_Object  host, P_ART_Method pos)
{
	DLL_ERRORHANDLING_BEGIN
  if (host == NULL) throw ARTerror("ARTGetMethods", "host is NULL.");
  else return host->GetMethods(pos);
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************

P_ART_Object    __stdcall ARTGetObjects         (P_ART_ListProp host, P_ART_Object  pos)
{
	DLL_ERRORHANDLING_BEGIN
  if (host == NULL) throw ARTerror("ARTGetObjects", "host is NULL.");
  else return host->GetObjects(pos);
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************

P_ART_DataProp  __stdcall ARTAppendDataProp     (P_ART_Object host, P_ART_Variant val, char* nam, char* sds, char* lds, char* htm)
{
	DLL_ERRORHANDLING_BEGIN
  if (host == NULL) throw ARTerror("ARTAppendDataProp", "host is NULL.");
  else return host->AppendDataProp(nam, val, sds, lds, htm);
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************

P_ART_ListProp  __stdcall ARTAppendListProp     (P_ART_Object host, char* nam, char* sds, char* lds, char* htm)
{
	DLL_ERRORHANDLING_BEGIN
  if (host == NULL) throw ARTerror("ARTAppendListProp", "host is NULL.");
  else return host->AppendListProp(nam, sds, lds, htm);
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************

P_ART_Method    __stdcall ARTAppendMethod       (P_ART_Object host, char* nam, char* sds, char* lds, char* htm)
{
	DLL_ERRORHANDLING_BEGIN
  if (host == NULL) throw ARTerror("ARTAppendMethod", "host is NULL.");
  else return host->AppendMethod(nam, sds, lds, htm);
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************

P_ART_Object    __stdcall ARTAppendObject       (P_ART_ListProp host, char* nam, char* sds, char* lds, char* htm)
{
	DLL_ERRORHANDLING_BEGIN
  if (host == NULL) throw ARTerror("ARTAppendObject", "host is NULL.");
  else return host->AppendObject(nam, sds, lds, htm);
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************

bool            __stdcall ARTDeleteProperty     (P_ART_Object  host, P_ART_Property prp)
{
	DLL_ERRORHANDLING_BEGIN
  if (host == NULL) throw ARTerror("ARTDeleteProperty", "host is NULL.");
  else return host->DeleteProperty(prp);
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************

bool        __stdcall ARTDeleteMethod           (P_ART_Object  host, P_ART_Method mtd)
{
	DLL_ERRORHANDLING_BEGIN
  if (host == NULL) throw ARTerror("ARTDeleteMethod", "host is NULL.");
  else return host->DeleteMethod(mtd);
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************

bool        __stdcall ARTDeleteObject           (P_ART_ListProp host, P_ART_Object pobj)
{
	DLL_ERRORHANDLING_BEGIN
  if (host == NULL) throw ARTerror("ARTDeleteObject", "host is NULL.");
  else return host->DeleteObject(pobj);
	DLL_ERRORHANDLING_END
}

//**************************************************************************************************************
char*	__stdcall	ARTGetDependencyTree	(P_ART_DataProp pprp, char* linebreak)
{
	string dt = pprp->WriteDepTree("", linebreak);
	const char* c = dt.c_str();
	char* s = (char*)malloc ( strlen(c)+1  ); //+1 for 0 character
	strcpy(s, c); 
	return s;
}

//**************************************************************************************************************
 
void listobjs(ARTlistProp* lstprp, string ind) {
  for (ARTobject* obj = lstprp->GetObjects(NULL); obj != NULL; obj = lstprp->GetObjects(obj)) {

    //cout << ind << "Object   " << obj->GetName() << " ("<< obj->GetShortDescription() << ")" << endl;
    //WASILAKIS include long description of objects:
    std::cout << ind << "Object   " << obj->GetName() << " ("<< obj->GetShortDescription() << obj->GetLongDescription() << ")" << endl;

        listprops(obj,ind+"  ");
        listmets(obj,ind+"  ");
    }
}

void listprops(ARTobject* obj, string ind) {
  for (ARTproperty* prp = obj->GetProperties(NULL); prp != NULL; prp = obj->GetProperties(prp)) {
        std::cout << ind << "Property " << prp->GetName() << " ("<< prp->GetShortDescription() << ")" << endl;
        if (!prp->IsListable()) 
		{
			if (ARTdataProp* dprop = dynamic_cast<ARTdataProp*>(prp))
			{
				std::cout << dprop->GetName() << "\n";
				int len = (dprop)->GetCount();
				T_ART_Type typ = (dprop)->GetDatatype();
				T_ART_Var* val = (dprop)->GetValue();
				if          (typ == C_ART_str) std::cout << ind << "  StrVal=" << val->s << endl; 
				else if (typ == C_ART_int) std::cout << ind << "  IntVal=" << val->i << endl; 
				else if (typ == C_ART_flo) std::cout << ind << "  FloVal=" << val->f << endl; 
				else if (typ == C_ART_dbl) std::cout << ind << "  DblVal=" << val->d << endl; 
				else if (typ == C_ART_cpx) std::cout << ind << "  CpxVal=(" << val->c.re << "," << val->c.im << ")" << endl; 
				else if (typ == C_ART_tri) std::cout << ind << "  TriVal=(" << val->t.f  << ":" << val->t.re << "," << val->t.im << ")" << endl; 
				else if (typ == C_ART_mat) std::cout << ind << "  MatVal=(" << val->m.a11.re << "," << val->m.a11.im << ")..." << endl; 
				else if (typ == C_ART_nstr) for (int j=0;j<len;j++) std::cout << ind << "  NStrVal" << "[" << j << "]=" << val->ns[j] << endl; 
				else if (typ == C_ART_nint) for (int j=0;j<len;j++) std::cout << ind << "  NIntVal" << "[" << j << "]=" << val->ni[j] << endl; 
				else if (typ == C_ART_nflo) for (int j=0;j<len;j++) std::cout << ind << "  NFloVal" << "[" << j << "]=" << val->nf[j] << endl; 
				else if (typ == C_ART_ndbl) for (int j=0;j<len;j++) std::cout << ind << "  NDblVal" << "[" << j << "]=" << val->nd[j] << endl; 
				else if (typ == C_ART_ncpx) for (int j=0;j<len;j++) std::cout << ind << "  NCpxVal" << "[" << j << "]=(" << val->nc[j].re << "," << val->nc[j].im << ")" << endl;
				else if (typ == C_ART_ntri) for (int j=0;j<len;j++) std::cout << ind << "  NTriVal" << "[" << j << "]=(" << val->nt[j].f  << ":" << val->nt[j].re << "," << val->nt[j].im << ")" << endl; 
				else if (typ == C_ART_nmat) for (int j=0;j<len;j++) std::cout << ind << "  NMatVal" << "[" << j << "]=(" << val->nm[j].a11.re << "," << val->nm[j].a11.im << ")..." << endl; 
				else std::cout << ind << "  Undef" << endl;
			}
        }
        else listobjs((ARTlistProp*)prp, ind+"  ");
    }
}

void listmets(ARTobject* obj, string ind) {
  for (ARTmethod* met = obj->GetMethods(NULL); met != NULL; met = obj->GetMethods(met)) {
        std::cout << ind << "Method   " << met->GetName() << " ("<< met->GetShortDescription() << ")" << endl;
        listprops((ARTobject*)met,ind+"  ");
    }
}

#ifdef _CONSOLE

//#include "io.h"

//**************************************************************************************************************

int load_net(int N, int* m, pChar* p0, double* p1, double* p2, double* p3, double* p4, double* p5, double* p6, double* p7, double* p8, int termination, const char *ImpedanceFile, int Contrib) 
{
  if (ins == NULL) ins = new WindInstrument();
  if (ins->getSize() > 0) ins->cleanElements();
  if (ins->impedanceCurve.getSize() > 0) ins->impedanceCurve.clear();

//ofstream file("load_net.log");
  for (int i=0; i<N; i++) {
    if        (m[i]==BOREJUMPSECTION_TYPE)    {
//file << "appending JUMP " << p1[i] << " " << p2[i] << " " << p3[i] << " " << p4[i] << endl;
        ins->append(new BoreJumpSection      (p1[i], p2[i], p3[i], p4[i],p5[i],p6[i], p0[i]), true); 
    } else if (m[i]==CYLINDERSECTION_TYPE)    {
//file << "appending CYL  " << p1[i] << " " << p2[i] << " " << p3[i] << " " << p4[i] << endl;
        ins->append(new CylinderSection      (false, false, p1[i], p2[i], p3[i], p4[i],p5[i],p6[i], p0[i]), true);
    } else if (m[i]==CONESECTION_TYPE)        {
//file << "appending CONE " << p1[i] << " " << p2[i] << " " << p3[i] << " " << p4[i] << " " << p5[i] << endl;
        ins->append(new ConeSection          (false, false, p1[i], p2[i], p3[i], p4[i], p5[i],p6[i],p7[i], p0[i]), true); 
    } else if (m[i]==BESSELSECTION_TYPE)      {
        ins->append(new BesselSection        (false, false, p1[i], p2[i], p3[i], p4[i], p5[i], p6[i],p7[i],p8[i], p0[i]), true); 
    } else if (m[i]==LISTSECTION_TYPE)        {       
        WindInstrument* tempIns = new WindInstrument();
        if( tempIns->load(p0[i]) == false ) return ERROR_;
        ins->append (tempIns, true);
    } else if (m[i]==CYLINDERBENDSECTION_TYPE){
        ins->append(new CylinderBendSection  (false, false, p1[i], p2[i], p3[i], p4[i], p5[i], p6[i],p7[i], p0[i]), true); 
    } else if (m[i]==CONEBENDSECTION_TYPE)    {
        ins->append(new ConeBendSection      (false, false, p1[i], p2[i], p3[i], p4[i], p5[i], p6[i],p7[i],p8[i], p0[i]), true); 
    } else if (m[i]==TERMINATIONELEMENT_TYPE) {
    if(termination==3) ins->append(new TerminationElement      (false, false, 0, Contrib,p1[i], p2[i], p3[i], termination,ImpedanceFile), true);  
    else {
      const char* name = " ";
      ins->append(new TerminationElement      (false, false, 0, Contrib,p1[i], p2[i], p3[i], termination,name), true); 
    }
  };
  }

  return NOERROR_;
}

int set_nModes(int nModes, int lossy, int simplified, double boreResolution, double fmin, double fmax, double df, const char *ImpedanceFile, int CalcType, int Contrib)
{
  if (ins == NULL) ins = new WindInstrument();
  ins->setnModes(nModes);
  ins->setlossy(lossy==1);
  ins->setsimplified(simplified==1);
  ins->setboreresolution(boreResolution);
  ins->setfmin(fmin);
  ins->setfmax(fmax);
  ins->setdf(df);
  vector <Matrix> Z;

  if(CalcType>0 && ImpedanceFile!=NULL){ins->SetImpedanceFile(ImpedanceFile,0,Contrib,Z);}//Input Impedance File -> RadHole = 0 
  return NOERROR_;
}

//to get the input impedance 
int calc_imp(double f, vector <double> &mag, vector <double> & pha, int IndFreq, int term, double gain, int nModes,int Contrib, int accumulate)
{

  if (ins == NULL) ins = new WindInstrument();

  if (ins->getSize() > 0) {
  vector <dcomp> res;
  if (ins->impedanceCurve.getSize() > 0) {
    res.push_back(ins->impedanceCurve.getImpedance(f) / dcomp(gain * 1.0E-5,0));
  }
  else  {
    list<Matrix> ImpedanceList;
    vector<int> IndHole;
    for(int p=0;p<4;p++){IndHole.push_back(0);}
    res = ins->getInputImpedance(f,term,IndFreq,Contrib,gain*1.0E-5,0,&ImpedanceList,0,IndHole,accumulate); //scales for Ohms
  }

  for(int i=0;i<res.size();i++){
    //res[i] = res[i]/dcomp(1.0E6); //Input impedance in MOhm
    mag.push_back(abs(res[i])); //Magnitude
    pha.push_back(arg(res[i])); //Phase
  }

    return NOERROR_;
  }
  else return ERROR_;
}

//to get the radiation impedance (based on "calc_imp" form)
int calc_rad(const double Freq, const double IndFreq, vector <double> &mag, vector <double> & pha, dcomp mem, int nModes,int Contrib)
{
  if (ins == NULL) ins = new WindInstrument();
  if (ins->getSize() > 0) {
  vector <dcomp> res;
  list<Matrix> ImpedanceList;
  vector<int> IndHole;
  for(int p=0;p<4;p++){IndHole.push_back(0);}
  res = ins->getRadiationImpedance(Freq,IndFreq,Contrib,0,&ImpedanceList,0,IndHole);
  if(res[1]==dcomp(0,0)) res[0] = mem;

  if(res[0]==dcomp(0,0)) {res[0]+= dcomp(1,0);}
  for(int i=1;i<res.size();i++){
      res[i] = res[i]/pow(10,abs(res[0]));
      mag.push_back(abs(res[i]));
      pha.push_back(arg(res[i]));
  }
  return NOERROR_;
  }
  else return ERROR_;
}

//to get the radiated pressure, impedance and flow (based on "calc_imp" form)
int calc_Param(const double Freq, const double IndFreq, vector <double> &mag, vector <double> & pha, dcomp mem, int nModes,int Contrib)
{
  if (ins == NULL) ins = new WindInstrument();
  if (ins->getSize() > 0) {

  vector <dcomp> res;
  list<Matrix> ImpedanceList;
  vector<int> IndHole;
  for(int p=0;p<4;p++){IndHole.push_back(0);}
  res = ins->getParam(Freq,IndFreq,Contrib, 0,&ImpedanceList,0,IndHole);
  if(res[0]==dcomp(0,0)) res[0] = mem;
  for(int i=0;i<res.size();i++){
    mag.push_back(abs(res[i]));
    pha.push_back(arg(res[i]));
  }
    return NOERROR_;
  }
  else return ERROR_;
}



//**************************************************************************************************************
bool __stdcall percent(double p, const char* s)
{
	static int pr;
	int	i = p*100;
	//print new percent if it's a different number
	if (pr != i) 
	{
		pr = i; 
		std::cout << s << " total progress:" << pr << "%" << "                                           \r";
	}
	//std::cout << pr << "%" << "\n";
	return true;
}

int main(int argc, char **argv) 
{
#define FILE_EXISTS(path) (0==access(path,0))
  double f;
  vector<double> r, ph;
  int len = 0;
  int     m[100];  m[0]  = 2;
  /*
  double p1[100];  p1[0] = 1000;
  double p2[100];  p2[0] = 20;
  double p3[100];  p3[0] = 50;
  double p4[100];  p4[0] = 21;
  double p5[100];  p5[0] = 1;
  double p6[100];  p6[0] = 1;
  double p7[100];  p7[0] = 80;
  double p8[100];  p8[0] = 382;
 */
  double p1[100];  p1[0] = 0;
  double p2[100];  p2[0] = 0;
  double p3[100];  p3[0] = 0;
  double p4[100];  p4[0] = 0;
  double p5[100];  p5[0] = 0;
  double p6[100];  p6[0] = 0;
  double p7[100];  p7[0] = 0;
  double p8[100];  p8[0] = 0;

  const char *ImpedanceFile = NULL;

#ifdef __GNUG__
  // cbg: g++ needs that commandline initialization is called explicitly
  cmdln.init(argv);
#endif

  int termination = cmdln.termination; //type of radiation (single element case)
  int Contrib = 1;
  int CalcType = -1;
  //bool hasbends;
  clock_t t_in, t_out; //usefull to get the execution duration

  pChar p0[100]; 
  char buf[1024];
  buf[0] = '\0'; p0[0] = &buf[0];

  bin  <= cmdln.isignal;
  bout <= cmdln.osignal;

   statistic << "\nProcess statistics for " << cmdln.pgmname << ":\n";
  if(cmdln.columns==0){std::cout<<"ERROR_: columns must be >=1 "<<endl;}
  if (IsDef (cmdln.stat)) {
     if (cmdln.stat) statistic.attach (cmdln.stat);
     else            statistic.attach (::cerr);
  } /* endif */


  if (IsDef(cmdln.borejump)) {
    m[0] = 0; p1[0] = cmdln.borejump.rin; p2[0] = cmdln.borejump.rout; p3[0] = cmdln.borejump.temperature; p4[0] = cmdln.borejump.loss; p5[0] = cmdln.borejump.humidity; p6[0] = cmdln.borejump.CO2; 
    if(!IsDef(cmdln.radiationfile)) {m[1] = 7; p1[1] = p2[0];         p2[1] = m[0];        p3[1] = p1[0];}     
  } else if (IsDef(cmdln.cylinder)) {m[0] = 1; p1[0] = cmdln.cylinder.len; p2[0] = cmdln.cylinder.rin; p3[0] = cmdln.cylinder.temperature; p4[0] = cmdln.cylinder.loss; p5[0] = cmdln.cylinder.humidity; p6[0] = cmdln.cylinder.CO2;
   if(IsDef(cmdln.radiationfile)) {m[1] = 7; p1[1] = p2[0];         p2[1] = m[0];         p3[1] = p2[0];}      
  } else if (IsDef(cmdln.cone)) {m[0] = 2; p1[0] = cmdln.cone.len; p2[0] = cmdln.cone.rin; p3[0] = cmdln.cone.rout; p4[0] = cmdln.cone.temperature; p5[0] = cmdln.cone.loss; p6[0] = cmdln.cone.humidity; p7[0] = cmdln.cone.CO2;
   if(IsDef(cmdln.radiationfile)) {m[1] = 7; p1[1] = p3[0];       p2[1] = m[0];       p3[1] = p2[0]; } 
  } else if (IsDef(cmdln.bessel)) {m[0] = 3; p1[0] = cmdln.bessel.len; p2[0] = cmdln.bessel.rin; p3[0] = cmdln.bessel.rout; p4[0] = cmdln.bessel.flare; p5[0] = cmdln.bessel.temperature; p6[0] = cmdln.bessel.loss; p7[0] = cmdln.bessel.humidity; p8[0] = cmdln.bessel.CO2;
     if(IsDef(cmdln.radiationfile)) {m[1] = 7; p1[1] = p3[0];       p2[1] = m[0];         p3[1] = p2[0]; }    
  } else if (IsDef(cmdln.list)) {m[0] = 4; p0[0] = cmdln.list.filename; 
  } else if (IsDef(cmdln.bcylinder)) {m[0] = 5; p1[0] = cmdln.bcylinder.len; p2[0] = cmdln.bcylinder.rin; p3[0] = cmdln.bcylinder.rbend; p4[0] = cmdln.bcylinder.temperature; p5[0] = cmdln.bcylinder.loss; p6[0] = cmdln.bcylinder.humidity; p7[0] = cmdln.bcylinder.CO2;
   if(IsDef(cmdln.radiationfile)) {m[1] = 7; p1[1] = p2[0];          p2[1] = m[0];        p3[1] = p2[0]; }       
  } else if (IsDef(cmdln.bcone)) {
   m[0] = 6; p1[0] = cmdln.bcone.len; p2[0] = cmdln.bcone.rin; p3[0] = cmdln.bcone.rout; p4[0] = cmdln.bcone.rbend;  p5[0] = cmdln.bcone.temperature; p6[0] = cmdln.bcone.loss; p7[0] = cmdln.bcone.humidity; p8[0] = cmdln.bcone.CO2;
   if(IsDef(cmdln.radiationfile)) {m[1] = 7; p1[1] = p3[0];        p2[1] = m[0];      p3[1] = p2[0]; }      
  }

  //values to get the radiation impedance
  if(IsDef(cmdln.impedancefile)) {
     ImpedanceFile = cmdln.impedancefile.filename;
     CalcType = 1;
     if(cmdln.impedancefile.columns==0) {std::cout<<"ERROR_: columns must be >=1 "<<endl;}
   Contrib = cmdln.impedancefile.columns;
  }
  //values to get the input impedance
  if(IsDef(cmdln.radiationfile)) { 
   ImpedanceFile = cmdln.radiationfile.filename; 
   termination = 3;
   if(cmdln.radiationfile.columns==0) {std::cout<<"ERROR_: columns must be >=1 "<<endl;}
   Contrib = cmdln.radiationfile.columns;
  }
  //values to get the radiated parameters
  if(IsDef(cmdln.interiorwave)) { 
     ImpedanceFile = cmdln.interiorwave.impedancefile;
     CalcType = 1;
  }

 
  set_nModes(cmdln.modes, !IsDef(cmdln.lossless), IsDef(cmdln.simplified), cmdln.accuracy, cmdln.frequencies.f0,cmdln.frequencies.f1,cmdln.frequencies.df,ImpedanceFile,CalcType,Contrib);
  load_net(2, m, p0, p1, p2, p3, p4, p5, p6, p7, p8, termination, ImpedanceFile, Contrib);

  if (IsDef(cmdln.bends))
    WindInstrument::hasBends_ = cmdln.bends;


  if (IsDef(cmdln.termination))
    WindInstrument::fixedTerm_ = true;


  t_in = clock();
  if (IsDef(cmdln.capabilities)) {
    art = new AcousticResearchTool();
    std::cout << "AcousticResearchTool created..." << endl;
 
    listprops(art,"");
    listmets(art,"");

    return ARTNOERROR;
  }
  else if (IsDef(cmdln.run)) {
    if (FILE_EXISTS(cmdln.run.filename)) {
      std::cout << "**Executing script..." << endl;
      ins->runScript(cmdln.run.filename);
      return ARTNOERROR;
     }
  }
  else if (IsDef(cmdln.frequencies) && cmdln.frequencies.refine) {
    ins->print();
    ins->roughPlot(cmdln.modes, cmdln.frequencies.f0, cmdln.frequencies.f1, floor((cmdln.frequencies.f1-cmdln.frequencies.f0) / cmdln.frequencies.df),IsDef(cmdln.accumulate),cmdln.termination);
    ins->refinePeaks(cmdln.frequencies.target, cmdln.termination, IsDef(cmdln.accumulate)); //wasilakis changed 2 to cmdln.termination
    ins->impedanceCurve.printPeaks();
    ins->printDescription();
  }

// Input Impedance (binary output)
  else if(!IsDef(cmdln.impedancefile) && !IsDef(cmdln.interiorwave) ) {
    //single element case or bore list with hole's terminaison = 0,1 or 2
    if (IsDef(cmdln.frequencies)) {
      for (f = cmdln.frequencies.f0; f <= cmdln.frequencies.f1; f+=cmdln.frequencies.df) len++; 
        //the number of columns in the output file depends on the multimodal contributions definite (by the user)
		//Sadjad: original	
		if(cmdln.columns<=cmdln.modes || cmdln.modes==0) {Checked (bout << (1+2*cmdln.columns) * len);}
        else {Checked (bout << (1+2*cmdln.modes*cmdln.modes) * len);}
	  /*
		//Sadjad: text output
	  if(cmdln.columns<=cmdln.modes || cmdln.modes==0) {std::cout << (1+2*cmdln.columns) * len;}
        else {std::cout << (1+2*cmdln.modes*cmdln.modes) * len;}
	*/
      int k=0;
      for (double f = cmdln.frequencies.f0; f <= cmdln.frequencies.f1; f+=cmdln.frequencies.df) {
        ph.clear();
        r.clear();
        if (0 ==calc_imp(f, r, ph, k, cmdln.termination, 1, cmdln.modes, cmdln.columns, IsDef(cmdln.accumulate))) 
		{

			//Sadjad: original!
		  Checked (bout << f << r[0] << ph[0]);
          for(int a=1;a<r.size();a++){Checked (bout << r[a] << ph[a]);}
/*
		  //SADJAD:Text output
          std::cout <<"\n"<< f <<"Hz: "<< r[0] <<" "<< ph[0];
          for(int a=1;a<r.size();a++){std::cout <<" "<< r[a] <<" "<< ph[a];}
 */
        } else throw NumError; k++; 
      }
    }
   //Bore list case with specification of all radiation impedance files 
    else if (IsDef(cmdln.list) && !IsDef(cmdln.radiationfile)) {                                         // ???
      len = ins->Freq.size();
      if(cmdln.columns<=cmdln.modes  || cmdln.modes==0) Checked (bout << (1+2*cmdln.columns) * len);
      else Checked (bout << (1+2*cmdln.modes*cmdln.modes) * len);
      for (double k = 0; k < ins->Freq.size(); k+=1) {
        ph.clear();
        r.clear();
        double freq = ins->Freq[k];
        if (0 ==calc_imp(freq, r, ph, k, termination, 1,cmdln.modes, cmdln.columns, IsDef(cmdln.accumulate))) {
          Checked (bout << freq << r[0] << ph[0]);
          for(int a=1;a<r.size();a++){Checked (bout << r[a] << ph[a]);}
        } else throw NumError;
      }
    }

    else if (IsDef(cmdln.radiationfile)) { //radiationfile command line is just used with a single element
      len = ins->Freq.size();
      if(cmdln.columns<=cmdln.modes  || cmdln.modes==0) Checked (bout << (1+2*cmdln.columns) * len);
      else Checked (bout << (1+2*cmdln.modes*cmdln.modes) * len);
      for (double k = 0; k < ins->Freq.size(); k+=1) {
        ph.clear();
        r.clear();
        double freq = ins->Freq[k];
        if (0 ==calc_imp(freq, r, ph, k, termination, 1, cmdln.modes, cmdln.columns, IsDef(cmdln.accumulate))) {
          Checked (bout << freq << r[0] << ph[0]);
          for(int a=1;a<r.size();a++){Checked (bout << r[a] << ph[a]);}
        } else throw NumError;
      }
    }
    else {
      Checked (bin >> len);
      if (!len) throw LengthErr;                         // zero file size?

      Checked (bout << 3 * len);
      for (int i = 0; i < len; i++) {
        Checked (bin >> f);
        ph.clear();
        r.clear();
        if (0 ==calc_imp(f, r, ph, 0, cmdln.termination, 1, cmdln.modes, cmdln.columns, IsDef(cmdln.accumulate))) {
          Checked (bout << f << r[0] << ph[0]);
        } else throw NumError;
      }
    }
  }
// Radiation Impedance
  else if (IsDef(cmdln.impedancefile)) { 
    len = ins->Freq.size();
    if(cmdln.columns<=cmdln.modes  || cmdln.modes==0) { Checked (bout << (1+2*cmdln.columns) * len);}
    else Checked (bout << (1+2*cmdln.modes*cmdln.modes) * len);
    dcomp mem = dcomp(0,0);
    for (double k = 0; k < ins->Freq.size(); k+=1) {
      ph.clear();
      r.clear();
      double freq = ins->Freq[k];
      if (0 ==calc_rad(freq,k, r, ph,mem,cmdln.modes, cmdln.columns)){
        mem = r[0]*exp(J*ph[0]);
        Checked (bout << freq << r[0] << ph[0]);
        for(int a=1;a<r.size();a++){Checked (bout << r[a] << ph[a]);}
      } else throw NumError;
    }
  }
// Radiated parameters (Pressure, Radiation impedance, Flow)
  else if (IsDef(cmdln.interiorwave)) {
    len = ins->Freq.size();
    if(cmdln.columns<=cmdln.modes  || cmdln.modes==0) Checked (bout << (1+6*cmdln.columns) * len);
    else Checked (bout << (1+6*cmdln.modes*cmdln.modes) * len);
    dcomp mem = dcomp(0,0);

    for (double k = 0; k < ins->Freq.size(); k+=1) {
      ph.clear();
      r.clear();
      double freq = ins->Freq[k];
      if (0 ==calc_Param(freq,k, r, ph,mem,cmdln.modes, cmdln.columns)){ 
        mem = r[0]*exp(J*ph[0]);
        Checked (bout << freq << r[0] << ph[0]);
        for(int a=1;a<r.size();a++){Checked (bout << r[a] << ph[a]);}
      } else throw NumError;
    }
  }

  t_out = clock();
  double duration = ((double)t_out - t_in) / CLOCKS_PER_SEC;

  statistic << "\nSettings\n";
  if (IsDef(cmdln.borejump)) {
    statistic << "  borejump: r1=" << cmdln.borejump.rin << " r2=" << cmdln.borejump.rout << " temp=" << cmdln.borejump.temperature << " loss=" << cmdln.borejump.loss  << endl;
  } else if (IsDef(cmdln.cylinder)) {
    statistic << "  cylinder: len=" << cmdln.cylinder.len << " r=" << cmdln.cylinder.rin << " temp=" << cmdln.cylinder.temperature << " loss=" << cmdln.cylinder.loss  << endl;
  } else if (IsDef(cmdln.cone)) {
    statistic << "  cone: len=" << cmdln.cone.len << " r1=" << cmdln.cone.rin << " r2=" << cmdln.cone.rout << " temp=" << cmdln.cone.temperature << " loss=" << cmdln.cone.loss  << endl;
  } else if (IsDef(cmdln.bessel)) {
    statistic << "  bessel: len=" << cmdln.bessel.len << " r1=" << cmdln.bessel.rin << " r2=" << cmdln.bessel.rout << " flare=" << cmdln.bessel.flare << " temp=" << cmdln.bessel.temperature << " loss=" << cmdln.bessel.loss  << endl;
  } else if (IsDef(cmdln.list)) {
    statistic << "  list: filename=" << cmdln.list.filename  << endl;
  } else if (IsDef(cmdln.bcylinder)) {
    statistic << "  bcylinder: len=" << cmdln.bcylinder.len << " r=" << cmdln.bcylinder.rin << " rb=" << cmdln.bcone.rbend << " temp=" << cmdln.bcylinder.temperature << " loss=" << cmdln.bcylinder.loss  << endl;
  } else if (IsDef(cmdln.bcone)) {
    statistic << "  bcone: len=" << cmdln.bcone.len << " r1=" << cmdln.bcone.rin << " r2=" << cmdln.bcone.rout << " rb=" << cmdln.bcone.rbend << " temp=" << cmdln.bcone.temperature << " loss=" << cmdln.bcone.loss  << endl;
  }

  statistic << " termination=" << cmdln.termination << endl;
  statistic << " modes=" << cmdln.modes << endl;
  statistic << " lossless=" << IsDef(cmdln.lossless) << endl;
  statistic << " simplified=" << IsDef(cmdln.simplified) << endl;

  statistic << 3*len << " values in output stream\n";
  statistic << 1000*duration << " ms used\n";
  return ARTNOERROR;
}
#endif //console

#endif //file