
#ifndef ARTDATACONTAINER_CPP
#define ARTDATACONTAINER_CPP

#include <iostream>
#include "Interface.h"
#include "ARTdataContainer.h"
#include "ARTlink.h"


namespace std
{
	#include <time.h>
}


ARTdataContainer::ARTdataContainer(const T_ART_Type dtyp, const int dlen, const string varname) : ARTvariant(dtyp, dlen),
		valid_(false), counted_(false), func_(NULL), clientList_(list<ARTdataContainer*>()), citer_(clientList_.begin()),
		dependencyList_(list<ARTdataContainer*>()), diter_(dependencyList_.begin()), definition_(""),
		parser_(NULL), varname_(varname), parserVarDefined(false)//, complexity_(1)
{
	//cout << "ARTdataContainer(typ,len)\n";
	eval_started = false;
}

ARTdataContainer::ARTdataContainer(std::string name, ARTfunctionoid* func) : ARTvariant(C_ART_undef, -1),
		valid_(false), counted_(false), func_(func), clientList_(list<ARTdataContainer*>()), citer_(clientList_.begin()),
		dependencyList_(list<ARTdataContainer*>()), diter_(dependencyList_.begin()), definition_(""),
		parser_(NULL), varname_(name),	parserVarDefined(false)//, complexity_(1)
{
	//cout << "ARTdataContainer(func)\n";
	if (func_)
	{
		func_->SetOutput(this);
		func->SetDependencies();
	}
	eval_started = false;
}

	///copy constructor
ARTdataContainer::ARTdataContainer(const ARTdataContainer& orig) : ARTvariant(orig)
{
	//copy client and dep lists, but keep pointers to same objects, after all, they are the same
	clientList_	= orig.clientList_;
	dependencyList_ = orig.dependencyList_;
	citer_ = orig.citer_;
	diter_ = orig.diter_;
	valid_ = orig.valid_;
	counted_ = orig.counted_;
	eval_started = false;
	definition_ = orig.definition_;
	scope_ = orig.scope_; //same object
	parser_ = orig.parser_; //same object
	func_ = orig.func_;
	parserVarDefined = false;
	//complexity_ = orig.complexity_;
}

ARTdataContainer::~ARTdataContainer()
{
	//"Clients, you can't depend on this object anymore, because it will cease to exist!"
	for (citer_=clientList_.begin();citer_!=clientList_.end(); citer_++)
	{
		(*citer_)->RemoveDependency(this);
		if ((!func_) && (definition_ != ""))
			(*citer_)->RemoveFromDefinition(this);
		//"But if you were dependent, you have to recalculate your value without this value if you can."
		(*citer_)->Invalidate();
		//Otherwise there's a problem anyway...
	}
	//"Dependencies, do not notify this object anymore!"
	RemoveAllDependencies();

	//if there was a var in the parser, delete it!
	if (parserVarDefined)
	{
		DestroyParserVar();
	}

	delete func_;
}

int ARTdataContainer::GetIterationNumber()
{
	if (func_) return func_->GetIterationNumber();
	else return 1;
}

//**************************************************************************************************************
// ARTdataContainer
/**
 * Description
 * @param b {bool}
 * @returns 
 */
void ARTdataContainer::SetCountedFlag(bool b)
{
	//to avoid circular references, check if we are already considering this dataContainer
	if (eval_started) throw ARTerror("ARTdataContainer::SetCountedFlag", "Circular reference to dataContainer '%s1'.",varname_); 

	//remember we are considering this data container now.
	eval_started = true;

	//walk through dependencies and set the flag there everywhere
	for (diter_=dependencyList_.begin();diter_!=dependencyList_.end(); diter_++) 
	{
		(*diter_)->SetCountedFlag(b);
	}

	counted_ = b;
	eval_started = false; //we leave this data container
} 

/**
 * This function sets the flags of this data container and all dependencies to the value indicating that no evaluation is taking place. This function
 * must be called when evaluation is aborted, since some datacontainers will be marked as evaluating, when the process jumps out of the evaluation
 * cycle. 
 * IMPORTANT: This function must only be called when the data container is known to contain no circular dependencies, such as after aborting
 * an evaluation, since before evaluations circular dependencies always generate an error.
 */
void ARTdataContainer::ResetEvaluation()
{
	//reset the flag of this data container...
	eval_started = false;
	//go through dependencies and set the flag there everywhere
	for (diter_=dependencyList_.begin();diter_!=dependencyList_.end(); diter_++) 
		(*diter_)->ResetEvaluation();
} 

/**
 * Just for debugging: to print the dependency tree...
 */
void ARTdataContainer::DebugDepTree(const string intend, const string linebreak)
{
	//to avoid circular references, check if we are already considering this dataContainer
	if (eval_started) 
	{
		cout << intend << "CIRCULAR REF: " << varname_;
		throw ARTerror("ARTdataContainer::DebugDebTree", "Circular reference to dataContainer '%s1'.",varname_); 
	}
	//remember we are considering this data container now.
	eval_started = true;
	cout << intend << varname_ << " " << this ;
	if (!valid_) cout << " (invalid)" << linebreak; else cout << linebreak;
	//walk through dependencies and print all dependencies
	for (diter_=dependencyList_.begin();diter_!=dependencyList_.end(); diter_++) 
	{
		(*diter_)->DebugDepTree(intend + ". ", linebreak);
	}
	eval_started = false; //we leave this data container
} 

string ARTdataContainer::WriteDepTree(const string intend, const string linebreak)
{
	string thisbranch = "";
	//to avoid circular references, check if we are already considering this dataContainer
	if (eval_started) 
	{
		thisbranch += intend + "CIRCULAR REF: " + varname_;
		throw ARTerror("ARTdataContainer::DebugDebTree", "Circular reference to dataContainer '%s1'.",varname_); 
	}
	//remember we are considering this data container now.
	eval_started = true;
	thisbranch += intend + varname_ ;
	if (!valid_) thisbranch +=  "(invalid)";
	thisbranch +=  linebreak;
	//walk through dependencies and print all dependencies
	for (diter_=dependencyList_.begin();diter_!=dependencyList_.end(); diter_++) 
	{
		thisbranch += (*diter_)->WriteDepTree(intend + ". ", linebreak);
	}
	eval_started = false; //we leave this data container
	return thisbranch;
} 

void ARTdataContainer::AddPropertiesAsDependency(ARTobject* obj)
{
	//go through all DATAproperties of element and add as dependencies
	ARTproperty* prop = obj->GetProperties(NULL);
	while (prop)
	{	
		ARTdataProp* dprop = dynamic_cast<ARTdataProp*>(prop);
		//only if it is a dataProperty ...
		if (dprop)
		{	
			//...and (if obj is an ARTelement) only if it is also found in the elements prototype (impedance matrices are
			//not found there and if we added them here the calculation of one impedance matrix would depend on itself)
			bool b = true;
			ARTelement* e = dynamic_cast<ARTelement*>(obj);
			if (e) b = e->IsPrototypeProperty(prop);
			if (b) AddDependency(dprop);
		}
		prop = obj->GetProperties(prop); //next property
	}
}

ARTprogressIndicator ARTdataContainer::progressIndicator = ARTprogressIndicator(NULL,0);

//**************************************************************************************************************
// ARTdataContainer

void ARTdataContainer::SetDefinition(const string& s, ARTsimulator* scope)
{
	scope_ = scope;					
	parser_ = scope_->GetParser();
 	if (parser_ == NULL) throw ARTerror("ARTdataContainer::SetDefinition", "The simulator's parser is NULL");
	//if there is a definition, only redefine if the new definition differs
	if (definition_ != s)
	{
		//remove old dependencies
		RemoveAllDependencies();
		// change in definition needs to be communicated: notify clients
		Invalidate();
		definition_ = s;
		ARTdataProp* dp;
		try
		{
			parser_->SetExpr(definition_);
			//for all variables in the expression
			mup::var_maptype map = parser_->GetExprVar();
			mup::var_maptype::const_iterator v;
			for(v = map.begin(); v != map.end(); ++v)
			{
				if (this->varname_ != v->first)
				{
					//TODO: Warning instead of error if a variable is not found. The same should be implemented in function below (rewrite testcase to check warings instead of error)
					dp = scope_->FindDataPropInSimulator(v->first);
					//make sure this dataContainer is notified when one of the variables changes
					//and remember this dataContainer depends on the variable v, ie. dataproperty dp
					AddDependency(dp);
				}
			}
		}
		catch( mup::ParserError e )
		{
			throw ARTerror("ARTdataContainer::SetDefinition", "Error in Parser: '%s1'", e.GetMsg().c_str());;
		}

	};
}

// return definition string
const string& ARTdataContainer::GetDefinition()
{
	//return the definition string - if it exists...
	if (definition_ != "")	return definition_;
	//otherwise: create a definition string and return it.
	tempDef = varname_ + " = "; //this is not a local variable, so that the string created will continue to exist after this method was called (as return value)
	std::stringstream Num;
    switch(typ)
	{
		//simple types
		case C_ART_int: Num << val->i; tempDef += Num.str(); break;
		case C_ART_flo: Num << val->f; tempDef += Num.str(); break;
		case C_ART_dbl: Num << val->d; tempDef += Num.str(); break;
		
		//string
		case C_ART_str: 
			tempDef += string(val->s);
			break;
		
		//array or pointer types, convert value only when length of array is 1
		case C_ART_nint:
			if (len > 1) throw ARTerror("ARTdataContainer::GetValueAsDouble", "Invalid type conversion: nint with len>1 to double.");
			if (len < 0) throw ARTerror("ARTdataContainer::GetValueAsDouble", "The data structure is an array type but was not initialized.");
			Num << *(val->ni);
			tempDef += Num.str();
			break;

		case C_ART_nflo: 
			if (len > 1) throw ARTerror("ARTdataContainer::GetValueAsDouble", "Invalid type conversion: nflo with len>1 to double.");
			if (len < 0) throw ARTerror("ARTdataContainer::GetValueAsDouble", "The data structure is an array type but was not initialized.");
			Num << *(val->nf);
			tempDef += Num.str();
			break;

		case C_ART_ndbl: 
			if (len > 1) throw ARTerror("ARTdataContainer::GetValueAsDouble", "Invalid type conversion: ndbl with len>1 to double.");
			if (len < 0) throw ARTerror("ARTdataContainer::GetValueAsDouble", "The data structure is an array type but was not initialized.");
			Num << *(val->nd);
			tempDef += Num.str();
			break;

		case C_ART_nstr: 
			if (len > 1) throw ARTerror("ARTdataContainer::GetValueAsDouble", "Invalid type conversion: ndbl with len>1 to double.");
			if (len < 0) throw ARTerror("ARTdataContainer::GetValueAsDouble", "The data structure is an array type but was not initialized.");
			tempDef += string(*(val->ns));
			break;

		default: throw ARTerror("ARTdataContainer::GetValueAsDouble", "Invalid type conversion to double: type is non-numeric, complex or triple.");
	}//end switch
	return tempDef;
}

void ARTdataContainer::RedoDefinitionDependencies()
{
	//only do something if there is a definition
	if (definition_ != "")
	{
		//remove old dependencies
		RemoveAllDependencies();

		ARTdataProp* dp;
		try
		{
			parser_->SetExpr(definition_);
			//for all variables in the expression
			mup::var_maptype map = parser_->GetExprVar();
			mup::var_maptype::const_iterator v;
			for(v = map.begin(); v != map.end(); ++v)
			{
				if (this->varname_ != v->first)
				{
					try
					{
						//TODO: Warning instead of error if a variable is not found. The same should be implemented in function above (rewrite testcase to check warings instead of error)
						dp = scope_->FindDataPropInSimulator(v->first);
						//make sure this dataContainer is notified when one of the variables changes
						//and remember this dataContainer depends on the variable v, ie. dataproperty dp
						AddDependency(dp);
					} 
					catch (ARTerror e) 
					{
						//issue warning: A variable in a definition string is undefined!
					}
				}
			}
		}
		catch( mup::ParserError e )
		{
			throw ARTerror("ARTdataContainer::SetDefinition", "Error in Parser: '%s1'", e.GetMsg().c_str());;
		}

	}
}


void ARTdataContainer::RemoveFromDefinition(ARTdataContainer *dependency)
{
/*	string s = "[Invalid reference to " + dependency->varname_ + "]";
//	definition_.find("dependency->varname_");
	alle vorkommen ausbessern!
	es muss abstand oder operator davor sein!

	std::replace(definition_.begin(),definition_.end(),dependency->varname_,s);
	while(x = target.find(oldstr), x != std::string::npos) 
	{
		target.erase(x, oldstr.length());
		target.insert(x, newstr);
	}
	*/
}


// expression evaluation cost
int ARTdataContainer::EvaluationCost() 
{
	//std::cout << "EvaluationCost() for " << varname_ << "\n";
	//to avoid circular references, check if we are already counting calculations for this dataContainer
	if (eval_started) throw ARTerror("ARTdataContainer::EvaluationCost", "Circular reference to dataContainer '%s1'.",varname_); 
	//if this one was considered already (->it would be valid now) or is valid anyway, return 0
	if (counted_ || valid_) 
	{
		//cout << varname_ <<" : 0\n";
		return 0;
	}
	//remember we are considering this data container now.
	eval_started = true;

	int calculations = 0;
	//check all dependencies for validity; if invalid: evaluate
	for (diter_=dependencyList_.begin();diter_!=dependencyList_.end(); diter_++) 
	{
		//This function has no access to ARTdataContainer::EvaluationCost(), since that function is protected and this is the function
		//of an external object (even if of a class derived from ARTdataContainer). However, since this object is derived from the prototype
		//it has also inherited the function EvaluationCost(). Since it is a virtual function it is guaranteed, that it will be called for the right object.
		calculations += ((ARTdataContainer*)(*diter_))->EvaluationCost();
	}
	
	//std::cout << "Evaluation estimate for '" << varname_ << "': " << calculations << " + " << GetIterationNumber() <<" = " << calculations + GetIterationNumber() << "\n";

	//now cost of this one in number of evaluations, that is the number of iterations (over a frq-range) times the observed complexity
	calculations += GetIterationNumber(); // * complexity_; if you want to reintroduce the complexity, make the counter a double value!!! and turn on complexity in GetComplexity() and Evaluate

	eval_started = false; //calculation stopped
	counted_ = true;

	//cout << varname_ <<" : "<<calculations<<"\n";
	return calculations;
}

// expression evaluation
void ARTdataContainer::Evaluate() 
{
	//std::cout << "ARTdataContainer::Evaluate() " << val << " // " << varname_ << "////////////////////\n";
	//to avoid circular references, check if we are already calcualting this dataContainer
	if (eval_started) throw ARTerror("ARTdataContainer::Evaluate", "Circular reference to dataContainer '%s1'.",varname_); 
	//... and if we are not, remember we do so now.
	eval_started = true;

	//check all dependencies for validity; if invalid: evaluate
	for (diter_=dependencyList_.begin();diter_!=dependencyList_.end(); diter_++) 
	{
		//if (!(*diter_)->valid_) cout << "   -> evaluate dependency " << (*diter_)->GetVarName() << "\n";
		((ARTdataContainer*)(*diter_))->EvaluateIfInvalid();
	}
	
	//if functionoid AND parser are specified > error, because we don't know which one to evaluate
	if ((func_) && (definition_ != "")) throw ARTerror("ARTdataContainer::Evaluate", "Function and parser expression specified for dataContainer '%s1'. Only either of them can be used.", varname_);

	//cout << "evaluated: " << varname_ << "\n";
	//clock_t start = clock();
	//if a functionoid is specified, use this to evaluate
	if (func_)
		func_->ApplyFunction();	
	else if (definition_ != "")
	{
		if (parser_ == NULL) throw ARTerror("ARTdataContainer::Evaluate", "Parser of dataContainer is null."); 
		try
		{
//			progressIndicator.Continue(complexity_,varname_);//count this evaluation
			progressIndicator.Continue(1,varname_);//count this evaluation
			//evaluate value of this data container and save result
			parser_->SetExpr(definition_);
			parser_->Eval();
		}
		catch( mup::ParserError e )
		{
			throw ARTerror("ARTdataContainer::Evaluate", "Error in Parser when processing dataContainer: '%s1'", e.GetMsg().c_str());
		}
	}
	else //function and parser are NULL 
		throw ARTerror("ARTdataContainer::Evaluate", "No evaluating expression or functionoid specified for dataContainer '%s1'.", varname_); 
	//clock_t finish = clock();
	
	//complexity_ = (double)(finish - start) / GetIterationNumber();
	//if (!complexity_) complexity_ = 0.001;

	//cout << "*** Duration (" << varname_ << "): " << complexity_ << " CPU ticks\n";
	//this value is valid
	valid_ = true;
	//calculation stopped
	eval_started = false;

}

bool ARTdataContainer::CheckValidity() 
{
	//to avoid circular references, check if we are already considering this dataContainer
	if (eval_started) throw ARTerror("ARTdataContainer::Evaluate", "Circular reference to dataContainer '%s1'.",varname_); 
	//... and if we are not, remember we do so now.
	eval_started = true;
	//std::cout << "check " << varname_ << " is " << valid_ << endl;
	if (!valid_)
	{
		eval_started = false;
		return false;
	}

	//check all dependencies for validity; if invalid: this one is invalid too!
	for (diter_=dependencyList_.begin();diter_!=dependencyList_.end(); diter_++) 
	{
		if (! ((ARTdataContainer*)(*diter_))->CheckValidity()) 
		{
			eval_started = false;
			valid_ = false;
			return false;
		}
	}

	//calculation stopped
	eval_started = false;
	return true;
}

void ARTdataContainer::SetFunction(ARTfunctionoid* func)
{
	//std::cout << "\n\nSTART (varname: " << varname_ << endl; DebugDepTree(""); std::cout << "END\n\n";

	if (!func) throw ARTerror("ARTdataContainer::SetFunction", "Argument '%s1' is NULL.","func");
	//check if the function constructed and passed as argument is same as the one we have already
	if (func_)
		if (func_->IsSameFunctionoid(func))
		{
			//Make sure that still all dependencies are set
			//remove all old dependencies
			RemoveAllDependencies();
			func_->SetOutput(this);
			func_->SetDependencies();
			//check if the datacontainer is supposed to be valid
			CheckValidity();
			//and if it's not, tell everyone
			if (! valid_) Invalidate();
			
			//std::cout << (this) << " " << varname_ << " is " << valid_ << endl;
			return;
		}
	//remove all old dependencies
	RemoveAllDependencies();
	//Invalidate this data container (possibly we have to re-evaluate)
	Invalidate();
	//delete the old function (pointers are only stored in the datacontainers)
	delete func_;
	func_ = func;
	func_->SetOutput(this);
	func_->SetDependencies();
}

double ARTdataContainer::GetValueAsDouble() 
{
	if (!valid_) Evaluate(); 
	double d;
    switch(typ)
	{
		//simple types: copy orig.value
		case C_ART_int: d = val->i; break;
		case C_ART_flo: d = val->f; break;
		case C_ART_dbl: d = val->d; break;

		case C_ART_nint:
			if (len > 1) throw ARTerror("ARTdataContainer::GetValueAsDouble", "Invalid type conversion: nint with len>1 to double.");
			else d = *(val->ni);
			break;

		case C_ART_nflo: 
			if (len > 1) throw ARTerror("ARTdataContainer::GetValueAsDouble", "Invalid type conversion: nflo with len>1 to double.");
			else d = *(val->nf);
			break;

		case C_ART_ndbl: 
			if (len > 1) throw ARTerror("ARTdataContainer::GetValueAsDouble", "Invalid type conversion: ndbl with len>1 to double.");
			else d = *(val->nd);
			break;

		default: throw ARTerror("ARTdataContainer::GetValueAsDouble", "Invalid type conversion to double: type is non-numeric, complex or triple.");
	}//end switch
	return d;
}


int ARTdataContainer::GetValueAsInt() 
{
	if (!valid_) Evaluate(); 
	int i;
    switch(typ)
	{
		//simple types: copy orig.value
		case C_ART_int: i = val->i; break;
		case C_ART_flo: i = val->f; break;
		case C_ART_dbl: i = val->d; break;

		case C_ART_nint:
			if (len > 1) throw ARTerror("ARTdataContainer::GetValueAsDouble", "Invalid type conversion: nint with len>1 to int.");
			else i = *(val->ni);
			break;

		case C_ART_nflo: 
			if (len > 1) throw ARTerror("ARTdataContainer::GetValueAsDouble", "Invalid type conversion: nflo with len>1 to int.");
			else i = *(val->nf);
			break;

		case C_ART_ndbl: 
			if (len > 1) throw ARTerror("ARTdataContainer::GetValueAsDouble", "Invalid type conversion: ndbl with len>1 to int.");
			else i = *(val->nd);
			break;

		default: throw ARTerror("ARTdataContainer::GetValueAsDouble", "Invalid type conversion to int: type is non-numeric, complex or triple.");
	}//end switch
	return i;
}

// direct change of data value (ignore definition string); set value pointer
void ARTdataContainer::SetValue(ARTvariant* var) 
{	
	if (var)
	{
		if ( !IsEqual(var) ) Invalidate();	// change in value needs to be communicated
		//delete old val, save pointer to new value
		delete val;
		val = var->val; 
		valid_ = true;
	}
	else //if the passed value is NULL
	{
		Invalidate();	// this value is invalid -> communicate this
		//delete old val, save pointer to new value
		delete val;
		val = NULL; 
		valid_ = false;
	}
}											

// mark value as invalid and invalidate all dependent data containers
void ARTdataContainer::Invalidate() 
{
//	if (eval_started) throw ARTerror("ARTdataContainer::Invalidate", "Circular reference to dataContainer '%s1'.",varname_); 
//	eval_started = true;
	//std::cout << varname_ << " invalidated.\n";										
	valid_ = false;											// re-evaluation necessary, invalidate clients
	NotifyClients();
//	eval_started = false;
}										

// invalidate clients
void ARTdataContainer::NotifyClients() 
{
	for (citer_=clientList_.begin();citer_!=clientList_.end(); citer_++) {(*citer_)->Invalidate();};
}	

int ARTdataContainer::GetEvaluationCost() 
{
	//Display debug information
	//DebugDepTree("|-");
	//first set the counted_ flag in all dependencies to false
	SetCountedFlag(false);
	//get the evaluation cost
	return EvaluationCost();
}

// notification about new client
// enter calling object into clientList if it is not already there
void ARTdataContainer::AddNotify(ARTdataContainer* client) 
{		
	for (citer_=clientList_.begin();citer_!=clientList_.end(); citer_++) {if ((*citer_) == client) break;}
	if (citer_ == clientList_.end()) clientList_.push_back(client);
}	 

//observe AND be notified
void ARTdataContainer::AddDependency(ARTdataContainer* dependency) 
{		
	for (diter_=dependencyList_.begin();diter_!=dependencyList_.end(); diter_++) 
	{
		if ((*diter_) == dependency) break;
	}
	if (diter_ == dependencyList_.end()) dependencyList_.push_back(dependency);
	//always also ensure this obj is notified when dependency changes
	dependency->AddNotify(this);
}	 

void ARTdataContainer::RemoveAllDependencies()
{
	//std::cout << "DC " << this->varname_ << " (" << this << ") remove all dependencies:\n";
	//alte abh�ngigkeiten austragen:
	//allen elementen in dependency list bescheid sagen, dass sie �nderung nicht mehr melden m�ssen
	for (diter_=dependencyList_.begin();diter_!=dependencyList_.end(); diter_++) 
	{
		//std::cout << "    " << (*diter_)->varname_ << "(" << (*diter_) << ")\n";
		(*diter_)->DoNotNotify(this);
	}
	//std::cout << "\n";
	//und dependencyList_ l�schen
	//std::cout << (this)->varname_ << " h�ngt von nix mehr ab.\n";
	dependencyList_.clear();
}

//void ARTdataContainer::AddPropertiesAsDependency(ARTobject* obj);

// notification about loss of client
// remove calling object from clientList if it is there
void ARTdataContainer::DoNotNotify(ARTdataContainer* client) 
{		
	/*for (citer_=clientList_.begin();citer_!=clientList_.end(); citer_++) {
		if ((*citer_) == client) {clientList_.erase(citer_); break;}
	}*/
	//Isn't it better like this:
	clientList_.remove(client);
}

void ARTdataContainer::RemoveDependency(ARTdataContainer* dependency) 
{		
/*	for (diter_=dependencyList_.begin();diter_!=dependencyList_.end(); diter_++) 
	{
		if ((*diter_) == dependency) {dependencyList_.erase(diter_); break;}
	}*/
	//Isn't it better like this:
	dependencyList_.remove(dependency);
}	 

void ARTdataContainer::Rename(const string& newname)
{
	//if there is a definition, change the name in the definition (replace old name with new name)
	if (definition_ != "")
	{
		size_t pos = definition_.find(varname_);
		if (pos != std::string::npos) definition_.replace(pos, varname_.length(), newname);
	}
	//if there was a parser variable rename it by recreating it
	if (parserVarDefined)
	{
		DestroyParserVar();	
		CreateParserVar(newname);
	}
	//and make sure that the data container remembers its new name
	varname_ = newname;
}

void ARTdataContainer::CreateParserVar(const string& varname)
{
	if (!parser_) throw ARTerror("ARTdataContainer::CreateParserVar","A parser variable for datacontainer '%s1' can not be created, because the datacontainer's parser pointer is NULL. Please use SetParser() to specify which parser the data container should use.",varname_);
	try 
	{
		//connect dataproperty to custom value type (ARTValue is only used by parser).
		//although we don't need to access the ARTValue again, we have to create it using
		//new, otherwise it will be destroyed and not usable in the parser after leaving this scope
		aval = new mup::ARTValue( this );
		//create variable out of value
		avar = new mup::Variable( aval );
		//and define the variable with the name created above
		parser_->DefineVar(varname, *avar);
		parserVarDefined = true;
		//remember variable name
		varname_ = varname;
		//cout << "varname: " << varname << " defined = ";

		parser_->SetExpr(varname);
		//cout << parser->Eval() << "\n";
		
		//since the dp is now connected to the parser variable called varname, changes are
		//written and read directely to/from the dp. nice, isn't it!

	}
	catch(mup::ParserError e)
	{
		throw ARTerror("ARTdataContainer::CreateParserVar", "Error in Parser: '%s1'", e.GetMsg());
	}
}


void ARTdataContainer::DestroyParserVar()
{
	if (!parser_) throw ARTerror("ARTdataContainer::DestroyParserVar","A parser variable for datacontainer '%s1' was created, but the datacontainers parser pointer is NULL. Please use SetParser() to specify which parser the data container should use.",varname_);
	try 
	{
		//delete value
		parser_->RemoveVar(varname_);
		//delete associated storage fields
		delete aval;
		delete avar;
	}
	catch(mup::ParserError e)
	{
		throw ARTerror("ARTdataContainer::DestroyParserVar", "Error in Parser: '%s1'", e.GetMsg());
	}
}

string ARTdataContainer::DebugInfo()
{
	ARTdataContainer* cd;
	ARTdataContainer* dc;
	string t = varname_ + "\n[meldet Aenderungen bei:";
	for (citer_=clientList_.begin();citer_!=clientList_.end(); citer_++) 
	{
		cd = dynamic_cast<ARTdataContainer*>(*citer_);
		if (cd) t = t  + " " + cd->GetVarName();
		else
		{
			dc = dynamic_cast<ARTdataContainer*>(*citer_);
			if (dc) t = t  + " " + dc->GetVarName();
		}
	}
	t= t + "]\n[abhaengig von:";
	for (diter_=dependencyList_.begin();diter_!=dependencyList_.end(); diter_++) 
	{
		cd = dynamic_cast<ARTdataContainer*>(*diter_);
		if (cd) t = t + " " + cd->GetVarName();
		else
		{
			dc = dynamic_cast<ARTdataContainer*>(*diter_);
			if (dc) t = t  + " " + dc->GetVarName();
		}
	}
	t = t + "]\n";
	return t;
}


#endif

