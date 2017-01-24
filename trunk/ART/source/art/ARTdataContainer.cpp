
#ifndef ARTDATACONTAINER_CPP
#define ARTDATACONTAINER_CPP

#include <iostream>
#include "ARTdataContainer.h"
#include "ARTSimulator.h"
#include "ARTlink.h"
#include "ARTfunctionoid.h"
#include "ARTobject.h"
#include "ARTelement.h"


//#define _DBG_MSG(a) {cout << __FILE__ << "::" <<__func__ << "("<< a << ")" << endl;}
#define _DBG_MSG(a)
#define _DBG_MSG2(a) {cout << __FILE__ << "::" <<__func__ << "("<< a << ")" << endl;}
//#define _DBG_MSG2(a)


namespace std
{
	#include <time.h>
}

ARTdataContainer::ARTdataContainer() :
		ARTvariant(),
		IValue(cmVAL),
		valid_(false),
		counted_(false),
		eval_started(false),
		func_(NULL),
		clientList_(list<ARTdataContainer*>()),
		citer_(clientList_.begin()),
		dependencyList_(list<ARTdataContainer*>()),
		diter_(dependencyList_.begin()),
		parentContainer_(NULL),
		definition_(""),
		parser_(NULL),
		scope_(NULL),
		varname_(""),
		avar_(NULL),
		parserVarDefined_(false),
		tempDef_(""),
		parentModuleName_(""),
		//arrayVals_(NULL),
		m_pCache_(NULL)
{
	_DBG_MSG("");
}

ARTdataContainer::ARTdataContainer(const T_ART_Type dtyp, const int dlen, const string varname) :
		ARTvariant(dtyp, dlen),
		IValue(cmVAL),
		valid_(false),
		counted_(false),
		eval_started(false),
		func_(NULL),
		clientList_(list<ARTdataContainer*>()),
		citer_(clientList_.begin()),
		dependencyList_(list<ARTdataContainer*>()),
		diter_(dependencyList_.begin()),
		parentContainer_(NULL),
		definition_(""),
		parser_(NULL),
		scope_(NULL),
		varname_(varname),
		avar_(NULL),
		parserVarDefined_(false),
		tempDef_(""),
		parentModuleName_(""),
		//arrayVals_(NULL),
		m_pCache_(NULL)
{
	_DBG_MSG("const T_ART_Type, const int, const string");
	// we have to handle array of data containers
	if (typ == C_ART_na)
	{
		array_type* tmpArray = new array_type(len, true);
		ARTdataContainer* tmpARTdataContainer;
//		CBG_DBL
//		parser_ = new ParserX(mup::pckCOMPLEX_NO_STRING);
		parser_ = new ParserX(mup::pckALL_NON_COMPLEX);
		for (int i = 0; i < len; ++i)
		{
			// create empty ARTdataContainers of type complex
			tmpARTdataContainer = new ARTdataContainer(varname);
//			CBG_DBL
//			tmpARTdataContainer->SetType(C_ART_cpx);
			tmpARTdataContainer->SetType(C_ART_dbl);
			// initialize all values of ARTdataContainer to zero...
//			tmpARTdataContainer->SetVal(std::complex<double>(0,0));
			tmpARTdataContainer->SetVal(static_cast<double>(0));
			// set all values to invalid
			tmpARTdataContainer->valid_ = false;
			tmpARTdataContainer->parser_ = parser_;
			tmpARTdataContainer->parentContainer_ = this;
			tmpARTdataContainer->varname_ = varname_;
			(*tmpArray)[i] = tmpARTdataContainer;
		}
		// set current index to 0 indicating that it is now
		// ready for external access
		// don't forget to do this, otherwise you will get
		// a segmentation fault!
		tmpArray->setCurrentIdx(0);
		// save pointer to na field of val
		val->na = (void *) tmpArray;

		if (varname_ != "")
		{
			SetParserVar(varname_);
		}
	}
}

ARTdataContainer::ARTdataContainer(std::string name, ARTfunctionoid* func) :
		ARTvariant(),
		IValue(cmVAL),
		valid_(false),
		counted_(false),
		eval_started(false),
		func_(func),
		clientList_(list<ARTdataContainer*>()),
		citer_(clientList_.begin()),
		dependencyList_(list<ARTdataContainer*>()),
		diter_(dependencyList_.begin()),
		parentContainer_(NULL),
		definition_(""),
		parser_(NULL),
		scope_(NULL),
		varname_(name),
		avar_(NULL),
		parserVarDefined_(false),
		tempDef_(""),
		parentModuleName_(""),
		//arrayVals_(NULL),
		m_pCache_(NULL)
{
	_DBG_MSG("string, ARTfunctionoid*");
	if (func_)
	{
		func_->SetOutput(this);
		func_->SetDependencies();
	}
}

ARTdataContainer::ARTdataContainer(const int i) :
		ARTvariant(i),
		IValue(cmVAL),
		valid_(true),
		counted_(false),
		eval_started(false),
		func_(NULL),
		clientList_(list<ARTdataContainer*>()),
		citer_(clientList_.begin()),
		dependencyList_(list<ARTdataContainer*>()),
		diter_(dependencyList_.begin()),
		parentContainer_(NULL),
		definition_(""),
		parser_(NULL),
		scope_(NULL),
		varname_(""),
		avar_(NULL),
		parserVarDefined_(false),
		tempDef_(""),
		parentModuleName_(""),
		//arrayVals_(NULL),
		m_pCache_(NULL)
{
	_DBG_MSG("int");
}

ARTdataContainer::ARTdataContainer(const double d) :
		ARTvariant(d),
		IValue(cmVAL),
		valid_(true),
		counted_(false),
		eval_started(false),
		func_(NULL),
		clientList_(list<ARTdataContainer*>()),
		citer_(clientList_.begin()),
		dependencyList_(list<ARTdataContainer*>()),
		diter_(dependencyList_.begin()),
		parentContainer_(NULL),
		definition_(""),
		parser_(NULL),
		scope_(NULL),
		varname_(""),
		avar_(NULL),
		parserVarDefined_(false),
		tempDef_(""),
		parentModuleName_(""),
		//arrayVals_(NULL),
		m_pCache_(NULL)
{
	_DBG_MSG("double");
}

ARTdataContainer::ARTdataContainer(const float f) :
		ARTvariant(f),
		IValue(cmVAL),
		valid_(true),
		counted_(false),
		eval_started(false),
		func_(NULL),
		clientList_(list<ARTdataContainer*>()),
		citer_(clientList_.begin()),
		dependencyList_(list<ARTdataContainer*>()),
		diter_(dependencyList_.begin()),
		parentContainer_(NULL),
		definition_(""),
		parser_(NULL),
		scope_(NULL),
		varname_(""),
		avar_(NULL),
		parserVarDefined_(false),
		tempDef_(""),
		parentModuleName_(""),
		//arrayVals_(NULL),
		m_pCache_(NULL)
{
	_DBG_MSG("float");
}

ARTdataContainer::ARTdataContainer(const char* s) :
		ARTvariant(s),
		IValue(cmVAL),
		valid_(true),
		counted_(false),
		eval_started(false),
		func_(NULL),
		clientList_(list<ARTdataContainer*>()),
		citer_(clientList_.begin()),
		dependencyList_(list<ARTdataContainer*>()),
		diter_(dependencyList_.begin()),
		parentContainer_(NULL),
		definition_(""),
		parser_(NULL),
		scope_(NULL),
		varname_(""),
		avar_(NULL),
		parserVarDefined_(false),
		tempDef_(""),
		parentModuleName_(""),
		//arrayVals_(NULL),
		m_pCache_(NULL)
{
	_DBG_MSG("const char*");
}

ARTdataContainer::ARTdataContainer(const char* s1, const char* s2) :
		ARTvariant(s1, s2),
		IValue(cmVAL),
		valid_(true),
		counted_(false),
		eval_started(false),
		func_(NULL),
		clientList_(list<ARTdataContainer*>()),
		citer_(clientList_.begin()),
		dependencyList_(list<ARTdataContainer*>()),
		diter_(dependencyList_.begin()),
		parentContainer_(NULL),
		definition_(""),
		parser_(NULL),
		scope_(NULL),
		varname_(""),
		avar_(NULL),
		parserVarDefined_(false),
		tempDef_(""),
		parentModuleName_(""),
		//arrayVals_(NULL),
		m_pCache_(NULL)
{
	_DBG_MSG("const char*, const char*");
}

ARTdataContainer::ARTdataContainer(const char* s1, const char* s2, const char* s3) :
		ARTvariant(s1, s2, s3),
		IValue(cmVAL),
		valid_(true),
		counted_(false),
		eval_started(false),
		func_(NULL),
		clientList_(list<ARTdataContainer*>()),
		citer_(clientList_.begin()),
		dependencyList_(list<ARTdataContainer*>()),
		diter_(dependencyList_.begin()),
		parentContainer_(NULL),
		definition_(""),
		parser_(NULL),
		scope_(NULL),
		varname_(""),
		avar_(NULL),
		parserVarDefined_(false),
		tempDef_(""),
		parentModuleName_(""),
		//arrayVals_(NULL),
		m_pCache_(NULL)
{
	_DBG_MSG("const char*, const char*, const char*");
}

ARTdataContainer::ARTdataContainer(const char* s1, const char* s2, const char* s3, const char* s4) :
		ARTvariant(s1, s2, s3, s4),
		IValue(cmVAL),
		valid_(true),
		counted_(false),
		eval_started(false),
		func_(NULL),
		clientList_(list<ARTdataContainer*>()),
		citer_(clientList_.begin()),
		dependencyList_(list<ARTdataContainer*>()),
		diter_(dependencyList_.begin()),
		parentContainer_(NULL),
		definition_(""),
		parser_(NULL),
		scope_(NULL),
		varname_(""),
		avar_(NULL),
		parserVarDefined_(false),
		tempDef_(""),
		parentModuleName_(""),
		//arrayVals_(NULL),
		m_pCache_(NULL)
{
	_DBG_MSG("const char*, const char*, const char*, const char*");
}

	///copy constructor
ARTdataContainer::ARTdataContainer(const ARTdataContainer& orig) :
	ARTvariant(orig),
	IValue(cmVAL),
	valid_(orig.valid_),
	counted_(orig.counted_),
	eval_started(false),
	func_(orig.func_),
	clientList_(orig.clientList_),
	citer_(orig.citer_),
	dependencyList_(orig.dependencyList_),
	diter_(orig.diter_),
	parentContainer_(orig.parentContainer_),
	definition_(orig.definition_),
	parser_(orig.parser_),
	scope_(orig.scope_),
	varname_(orig.varname_),
	avar_(NULL),
	parserVarDefined_(false),
	tempDef_(orig.tempDef_),
	parentModuleName_(orig.parentModuleName_),
	//arrayVals_(NULL), // TODO copy values!
	m_pCache_(NULL)
{
	_DBG_MSG("const ARTdataContainer&");
	// we have to handle array of data containers
	if (typ == C_ART_na)
	{
		array_type* tmpArray = new array_type(len, true);
		array_type* oldArray = (array_type*) (orig.val->na);

		ARTdataContainer* tmpARTdataContainer;
		ARTdataContainer* oldARTdataContainer;

//		CBG_DBL
//		parser_ = new ParserX(mup::pckCOMPLEX_NO_STRING);
		parser_ = new ParserX(mup::pckCOMMON_NO_STR);


		for (int i = 0; i < len; ++i)
		{
			oldARTdataContainer = dynamic_cast<ARTdataContainer*>((*oldArray)[i]);
			// create empty ARTdataContainers of type complex
			tmpARTdataContainer = new ARTdataContainer(*oldARTdataContainer);
//			CBG_DBL
//			tmpARTdataContainer->SetType(C_ART_cpx);
			tmpARTdataContainer->SetType(C_ART_dbl);
			// initialize all values of ARTdataContainer to zero...
//			tmpARTdataContainer->SetVal(std::complex<double>(0,0));
			tmpARTdataContainer->SetVal(static_cast<double>(0));
			// set all values to invalid
			tmpARTdataContainer->valid_ = false;
			tmpARTdataContainer->parser_ = parser_;
			tmpARTdataContainer->parentContainer_ = this;
			tmpARTdataContainer->varname_ = varname_;
			tmpArray->at(i) = tmpARTdataContainer;
		}
		// save pointer to na field of val
		val->na = (void *) tmpArray;
	}
}

ARTdataContainer::~ARTdataContainer()
{
	_DBG_MSG("");
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

	// "Dependencies, do not notify this object anymore!"
	RemoveAllDependencies();

	//if there was a var in the parser, delete it!
	if (parserVarDefined_)
	{
		DestroyParserVar();
	}

	// Release memory of all array elements in case we have
	// a container of type C_ART_na
	if (typ == C_ART_na)
	{
		array_type* tmpArray = (array_type *) (val->na);
		ARTdataContainer* tmpARTdataContainer;
		for (int i = 0; i < tmpArray->size(); ++i)
		{
			// we have to use the "at"-operator, otherwise
			// we might get an out of range error
			tmpARTdataContainer = dynamic_cast<ARTdataContainer*>(tmpArray->at(i));
			delete tmpARTdataContainer;
		}
		delete parser_;
	}



	// in case of an array, remove all child elements
	/*if (typ == C_ART_na)
	{
		delete[] dynamic_cast<ARTdataContainer*>(val->na);
	}*/

	delete func_;
}

int ARTdataContainer::GetIterationNumber()
{
	_DBG_MSG("");
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
	_DBG_MSG("bool");
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
	_DBG_MSG("");
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
	_DBG_MSG("const string, const string");
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
	_DBG_MSG("const string, const string");
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
	_DBG_MSG("ARTobject*");
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

void ARTdataContainer::SetDefinition(const string& s, ARTSimulator* scope)
{
	_DBG_MSG("const string&, ARTsimulator");
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
					//TODO: Warning instead of error if a variable is not found. The same should be implemented in function below (rewrite testcase to check warnings instead of error)
					dp = scope_->FindDataPropInSimulator(v->first);
					//make sure this dataContainer is notified when one of the variables changes
					//and remember this dataContainer depends on the variable v, ie. dataproperty dp
					AddDependency(dp);
				}
			}
		}
		catch( mup::ParserError& e )
		{
			throw ARTerror("ARTdataContainer::SetDefinition", "Error in Parser: '%s1'", e.GetMsg().c_str());;
		}

	};
}

void ARTdataContainer::SetDefinition(const string& s)
{
	_DBG_MSG("const string&");
	if (parser_ == NULL) throw ARTerror("ARTdataContainer::SetDefinition", "The current parser is set to NULL");
	//if there is a definition, only redefine if the new definition differs
	if (definition_ != s)
	{
		//remove old dependencies
		//RemoveAllDependencies();
		// change in definition needs to be communicated: notify clients
		Invalidate();
		definition_ = s;
		// in case we have an array, set definition to all child elements
		if (typ == C_ART_na)
		{
			array_type* tmpArray = (array_type *) (val->na);
			ARTdataContainer* tmp;
			for (int i = 0; i < len; ++i)
			{
				tmp = dynamic_cast<ARTdataContainer*>(tmpArray->at(i));
				tmp->definition_ = s;
			}
		}

	}
}

// return definition string
const string& ARTdataContainer::GetDefinition()
{
	_DBG_MSG("");
	//return the definition string - if it exists...
	if (definition_ != "")	return definition_;
	//otherwise: create a definition string and return it.
	tempDef_ = varname_ + " = "; //this is not a local variable, so that the string created will continue to exist after this method was called (as return value)
	std::stringstream Num;
    switch(typ)
	{
		//simple types
		case C_ART_int: Num << val->i; tempDef_ += Num.str(); break;
		case C_ART_flo: Num << val->f; tempDef_ += Num.str(); break;
		case C_ART_dbl: Num << val->d; tempDef_ += Num.str(); break;
		
		//string
		case C_ART_str: 
			tempDef_ += string(val->s);
			break;
		
		//array or pointer types, convert value only when length of array is 1
		case C_ART_nint:
			if (len > 1) throw ARTerror("ARTdataContainer::GetValueAsDouble", "Invalid type conversion: nint with len>1 to double.");
			if (len < 0) throw ARTerror("ARTdataContainer::GetValueAsDouble", "The data structure is an array type but was not initialized.");
			Num << *(val->ni);
			tempDef_ += Num.str();
			break;

		case C_ART_nflo: 
			if (len > 1) throw ARTerror("ARTdataContainer::GetValueAsDouble", "Invalid type conversion: nflo with len>1 to double.");
			if (len < 0) throw ARTerror("ARTdataContainer::GetValueAsDouble", "The data structure is an array type but was not initialized.");
			Num << *(val->nf);
			tempDef_ += Num.str();
			break;

		case C_ART_ndbl: 
			if (len > 1) throw ARTerror("ARTdataContainer::GetValueAsDouble", "Invalid type conversion: ndbl with len>1 to double.");
			if (len < 0) throw ARTerror("ARTdataContainer::GetValueAsDouble", "The data structure is an array type but was not initialized.");
			Num << *(val->nd);
			tempDef_ += Num.str();
			break;

		case C_ART_nstr: 
			if (len > 1) throw ARTerror("ARTdataContainer::GetValueAsDouble", "Invalid type conversion: ndbl with len>1 to double.");
			if (len < 0) throw ARTerror("ARTdataContainer::GetValueAsDouble", "The data structure is an array type but was not initialized.");
			tempDef_ += string(*(val->ns));
			break;

		default: throw ARTerror("ARTdataContainer::GetValueAsDouble", "Invalid type conversion to double: type is non-numeric, complex or triple.");
	}//end switch
	return tempDef_;
}

void ARTdataContainer::RedoDefinitionDependencies()
{
	_DBG_MSG("");
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
		catch( mup::ParserError& e )
		{
			throw ARTerror("ARTdataContainer::SetDefinition", "Error in Parser: '%s1'", e.GetMsg().c_str());;
		}

	}
}


void ARTdataContainer::RemoveFromDefinition(ARTdataContainer *dependency)
{
	_DBG_MSG("");
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
	_DBG_MSG("");
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
void ARTdataContainer::Evaluate() const
{
//	_DBG_MSG2( definition_ );
	//std::cout << "ARTdataContainer::Evaluate() " << val << " // " << varname_ << "////////////////////\n";
	//to avoid circular references, check if we are already calculating this dataContainer
	if (eval_started)
	{
		if (parentContainer_ && parentContainer_->parentModuleName_ != "")
		{
			throw ARTerror("ARTdataContainer::Evaluate", "Circular reference to dataContainer '%s1' of module '%s2'.", varname_, parentContainer_->parentModuleName_);
		}
		else
		{
			throw ARTerror("ARTdataContainer::Evaluate", "Circular reference to dataContainer '%s1'.", varname_);
		}
	}

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
			// only set expression of parser if it has not been done yet
			if (parser_->GetExpr() != definition_)
			{
				parser_->SetExpr(definition_);
			}
			//evaluate value of this data container and save result
//			cout << "Evaluate the following expression: " << parser_->GetExpr() << endl;
			parser_->Eval();
		}
		catch( mup::ParserError& e )
		{
			if (parentContainer_ && parentContainer_->parentModuleName_ != "")
			{
				throw ARTerror("ARTdataContainer::Evaluate", "Error in Parser when processing dataContainer '%s1' of module '%s2': '%s3'",
						varname_, parentContainer_->parentModuleName_,  e.GetMsg().c_str());
			}
			else
			{
				throw ARTerror("ARTdataContainer::Evaluate", "Error in Parser when processing dataContainer '%s1': '%s2'", varname_, e.GetMsg().c_str());
			}
		}
		catch(std::out_of_range& oor)
		{
			if (parentContainer_ && parentContainer_->parentModuleName_ != "")
			{
				throw ARTerror("ARTdataContainer::Evaluate", "Error in Parser when processing dataContainer '%s1' of module '%s2': '%s3'",
						varname_, parentContainer_->parentModuleName_,  oor.what());
			}
			else
			{
				throw ARTerror("ARTdataContainer::Evaluate", "Error in Parser when processing dataContainer '%s1': '%s2'", varname_, oor.what());
			}
		}
	}
	else //function and parser are NULL 
	{
		if (parentContainer_ && parentContainer_->parentModuleName_ != "")
		{
			throw ARTerror("ARTdataContainer::Evaluate", "No evaluating expression or functionoid specified for dataContainer '%s1' of module '%s2'.",
					varname_, parentContainer_->parentModuleName_);
		}
		else
		{
			throw ARTerror("ARTdataContainer::Evaluate", "No evaluating expression or functionoid specified for dataContainer '%s1'.", varname_);
		}
	}
	//clock_t finish = clock();
	
	//complexity_ = (double)(finish - start) / GetIterationNumber();
	//if (!complexity_) complexity_ = 0.001;

	//cout << "*** Duration (" << varname_ << "): " << complexity_ << " CPU ticks\n";
	//this value is valid
	valid_ = true;
	//calculation stopped
	eval_started = false;

//	cout << "DONE with " << definition_ << endl;

}

bool ARTdataContainer::CheckValidity() 
{
	_DBG_MSG("");
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
	_DBG_MSG("ARTfunctionoid*");
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

float_type ARTdataContainer::GetFloat() const
{
	_DBG_MSG("");
	//cout << __func__ << "(): valid_ = " << valid_ << endl;
	if (!valid_) Evaluate();
	float_type d;
	array_type* tmpArray = (array_type *) (val->na);
	ARTdataContainer* tmp;
	switch(typ)
	{
		//simple types: copy orig.value
		case C_ART_int: d = val->i; break;
		case C_ART_flo: d = val->f; break;
		case C_ART_dbl: d = val->d; break;
		case C_ART_cpx: d = val->c.re; break;

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

		case C_ART_ncpx:
			if (len > 1) throw ARTerror("ARTdataContainer::GetValueAsDouble", "Invalid type conversion: ncpx with len>1 to double.");
			else d = val->nc->re;
			break;


		case C_ART_na:
			if (len > 1)
			{
				throw ARTerror("ARTdataContainer::GetValueAsDouble", "Invalid type conversion: ndbl with len>1 to double.");
			}
			else
			{
				tmp = dynamic_cast<ARTdataContainer*>((*tmpArray)[0]);
				d = tmp->GetFloat();
			}

			break;

		default: throw ARTerror("ARTdataContainer::GetValueAsDouble", "Invalid type conversion to double: type is non-numeric, complex or triple.");
	}//end switch
	return d;
}

//double ARTdataContainer::GetValueAsDoubleFromIndex(std::size_t ind)
//{
//	double retVal = 0;
//	if (typ == C_ART_na)
//	{
//		/*ARTdataContainer* arrayElements = dynamic_cast<ARTdataContainer*>(val->na);
//		if ((int) ind < len)
//		{
//			if (!arrayElements[ind].valid_)
//			{
//				cout << "Evaluate array element " << ind << endl;
//				arrayElements[ind].Evaluate();
//			}
//			retVal = arrayElements[ind].val->d;
//		}
//		else
//		{
//			throw ARTerror("ARTdataContainer::GetValueAsDoubleFromIndex", "Index out of bounds.");
//		}*/
//		retVal = (*aval)[ind].GetFloat();
//	}
//	return retVal;
//
//}

//void ARTdataContainer::SetInvalid(int st, int end)
//{
//	if (typ == C_ART_na)
//	{
//		//ARTdataContainer* arrayElements = dynamic_cast<ARTdataContainer*>(val->na);
//		for (int i = st; i <= end; ++i)
//		{
//			//arrayElements[i].valid_ = false;
//			ARTdataContainer* tmp = dynamic_cast<mup::ARTValue&>((*aval)[i]).GetContainer();
//			tmp->valid_ = false;
//		}
//	}
//}

int ARTdataContainer::GetInt() 
{
	_DBG_MSG("");
	if (!valid_) Evaluate(); 
	int i;
	array_type* tmpArray = (array_type *) (val->na);
	ARTdataContainer* tmp;
    switch(typ)
	{
		//simple types: copy orig.value
		case C_ART_int: i = val->i; break;
		case C_ART_flo: i = val->f; break;
		case C_ART_dbl: i = val->d; break;
		case C_ART_cpx: i = val->c.re; break;

		case C_ART_nint:
			if (len > 1) throw ARTerror("ARTdataContainer::GetValueAsInt", "Invalid type conversion: nint with len>1 to int.");
			else i = *(val->ni);
			break;

		case C_ART_nflo: 
			if (len > 1) throw ARTerror("ARTdataContainer::GetValueAsInt", "Invalid type conversion: nflo with len>1 to int.");
			else i = *(val->nf);
			break;

		case C_ART_ndbl: 
			if (len > 1) throw ARTerror("ARTdataContainer::GetValueAsInt", "Invalid type conversion: ndbl with len>1 to int.");
			else i = *(val->nd);
			break;

		case C_ART_ncpx:
			if (len > 1) throw ARTerror("ARTdataContainer::GetValueAsInt", "Invalid type conversion: ncpx with len>1 to int.");
			else i = val->nc->re;
			break;

		case C_ART_na:
			if (len > 1)
			{
				throw ARTerror("ARTdataContainer::GetValueAsInt", "Invalid type conversion: na with len>1 to int.");
			}
			else
			{
				tmp = dynamic_cast<ARTdataContainer*>((*tmpArray)[0]);
				//i = tmp->GetValueAsInt();
				i = tmp->GetInt();
			}
			break;
		default: throw ARTerror("ARTdataContainer::GetValueAsInt", "Invalid type conversion to int: type is non-numeric, complex or triple.");
	}//end switch
	return i;
}

// direct change of data value (ignore definition string); set value pointer
void ARTdataContainer::SetValue(ARTvariant* var) 
{	
	_DBG_MSG("ARTvariant*");
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

void ARTdataContainer::SetVal(const int i, const int ind)
{
	_DBG_MSG("const int, const int");
	// in case we have an array, we have to handle that separately
	if (typ == C_ART_na)
	{
		array_type* tmpArray = (array_type *) (val->na);
		ARTdataContainer* tmp = dynamic_cast<ARTdataContainer*>(tmpArray->at(ind));
		tmp->SetVal(std::complex<double>((double) i,0));
	}
	else
	{
		ARTvariant::SetVal(i, ind);
		valid_ = true;
	}
	NotifyClients();

}

void ARTdataContainer::SetVal(const double d, const int ind)
{
//	_DBG_MSG2("const double = " << d << ", const int = " << ind << ", name = " << varname_);
	_DBG_MSG("const double, const int");
	if (typ == C_ART_na)
	{
		array_type* tmpArray = (array_type *) (val->na);
		ARTdataContainer* tmp = dynamic_cast<ARTdataContainer*>(tmpArray->at(ind));
		tmp->SetVal(std::complex<double>(d,0));
	}
	else
	{
		ARTvariant::SetVal(d, ind);
		valid_ = true;
	}
	NotifyClients();

}

void ARTdataContainer::SetVal(const float f, const int ind)
{
	_DBG_MSG("const float, const int");
	if (typ == C_ART_na)
	{
		array_type* tmpArray = (array_type *) (val->na);
		ARTdataContainer* tmp = dynamic_cast<ARTdataContainer*>(tmpArray->at(ind));
		tmp->SetVal(std::complex<double>(f,0));
	}
	else
	{
		ARTvariant::SetVal(f, ind);
		valid_ = true;
	}
	NotifyClients();

}

void ARTdataContainer::SetVal(std::complex<double> c, const int ind)
{
	_DBG_MSG("std::complex, int");
//	_DBG_MSG2("std::complex = ("<< c.real() << ", " << c.imag() << "), int");
	//cout << "setVal(cmplx<" << c.real() << "," << c.imag() << ">, " << ind << ")" << endl;
	if (typ == C_ART_na)
	{
		array_type* tmpArray = (array_type *) (val->na);
		ARTdataContainer* tmp = dynamic_cast<ARTdataContainer*>(tmpArray->at(ind));
		tmp->SetVal(c);
	}
	else
	{
		ARTvariant::SetVal(c, ind);
		valid_ = true;
	}
	NotifyClients();

}

void ARTdataContainer::SetVal(const double re, const double im, const int ind)
{
	_DBG_MSG("const double, const double, const int");
	if (typ == C_ART_na)
	{
		array_type* tmpArray = (array_type *) (val->na);
		ARTdataContainer* tmp = dynamic_cast<ARTdataContainer*>(tmpArray->at(ind));
		tmp->SetVal(re, im);
	}
	else
	{
		ARTvariant::SetVal(re, im, ind);
		valid_ = true;
	}
	NotifyClients();

}

void ARTdataContainer::SetVal(const char* s)
{
	_DBG_MSG("const char*");
	ARTvariant::SetVal(s);
	NotifyClients();
	valid_ = true;
}

void ARTdataContainer::SetVal(const char* s, int ind)
{
	_DBG_MSG("const char* s, int");
	ARTvariant::SetVal(s, ind);
	NotifyClients();
	valid_ = true;
}

// mark value as invalid and invalidate all dependent data containers
void ARTdataContainer::Invalidate()
{
	_DBG_MSG( varname_ );
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
	_DBG_MSG("");
	for (citer_=clientList_.begin();citer_!=clientList_.end(); citer_++) {(*citer_)->Invalidate();};
}	

int ARTdataContainer::GetEvaluationCost() 
{
	_DBG_MSG("");
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
	_DBG_MSG("ARTdataContainer*");
	for (citer_=clientList_.begin();citer_!=clientList_.end(); citer_++) {if ((*citer_) == client) break;}
	if (citer_ == clientList_.end()) clientList_.push_back(client);
}	 

//observe AND be notified
void ARTdataContainer::AddDependency(ARTdataContainer* dependency) 
{		
	_DBG_MSG("ARTdataContainer*");
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
	_DBG_MSG("");
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
	_DBG_MSG("ARTdataContainer*");
	/*for (citer_=clientList_.begin();citer_!=clientList_.end(); citer_++) {
		if ((*citer_) == client) {clientList_.erase(citer_); break;}
	}*/
	//Isn't it better like this:
	clientList_.remove(client);
}

void ARTdataContainer::RemoveDependency(ARTdataContainer* dependency) 
{		
	_DBG_MSG("ARTdataContainer*");
/*	for (diter_=dependencyList_.begin();diter_!=dependencyList_.end(); diter_++) 
	{
		if ((*diter_) == dependency) {dependencyList_.erase(diter_); break;}
	}*/
	//Isn't it better like this:
	dependencyList_.remove(dependency);
}	 

void ARTdataContainer::Rename(const string& newname)
{
	_DBG_MSG("const string&");
	//if there is a definition, change the name in the definition (replace old name with new name)
	if (definition_ != "")
	{
		::size_t pos = definition_.find(varname_);
		if (pos != std::string::npos) definition_.replace(pos, varname_.length(), newname);
	}
	//if there was a parser variable rename it by recreating it
	if (parserVarDefined_)
	{
		DestroyParserVar();	
		SetParserVar(newname);
	}
	//and make sure that the data container remembers its new name
	varname_ = newname;
}

void ARTdataContainer::SetParser(mup::ParserX *p){
	_DBG_MSG("ParserX*");
	parser_ = p;
	// if we have a valid name, register it in parser
	if (varname_ != "")
	{
		//cout << "Set parser var: " << varname_ << endl;
		SetParserVar(varname_);
		//cout << "Done." << endl;
	}
	// in case we have children, set the same parser for them
	if (typ == C_ART_na)
	{
		array_type* tmpArray = (array_type *) (val->na);
		ARTdataContainer* tmp;
		for (int i = 0; i < len; ++i)
		{
			tmp = dynamic_cast<ARTdataContainer*>(tmpArray->at(i));
			//cout << "Set parser of element [" << i << "]" << endl;
			tmp->parser_ = p;
			//cout << "Done" << endl;
		}
	}

}


void ARTdataContainer::SetParserVar(const string& varname)
{
	_DBG_MSG("const string&");
//	if (!parser_) throw ARTerror("ARTdataContainer::CreateParserVar","A parser variable for datacontainer '%s1' can not be created, because the datacontainer's parser pointer is NULL. Please use SetParser() to specify which parser the data container should use.",varname_);
	// if we have already a variable created, destroy it
	if (parserVarDefined_)
	{
		DestroyParserVar();
	}
	try 
	{
		// cout << "Create new parser variable..." << endl;
		//connect dataproperty to custom value type (ARTValue is only used by parser).
		//although we don't need to access the ARTValue again, we have to create it using
		//new, otherwise it will be destroyed and not usable in the parser after leaving this scope
		//aval = new mup::ARTValue( this );
		//create variable out of value
		avar_ = new mup::Variable( this );
		//and define the variable with the name created above
		if (parser_)
		{
			parser_->DefineVar(varname, *avar_);
		}
		parserVarDefined_ = true;
		//remember variable name
		varname_ = varname;


		//cout << "varname: " << varname << " defined = ";

		//parser_->SetExpr(varname);
		//cout << parser_->Eval() << "\n";
		
		//since the dp is now connected to the parser variable called varname, changes are
		//written and read directely to/from the dp. nice, isn't it!

	}
	catch(mup::ParserError& e)
	{
		throw ARTerror("ARTdataContainer::CreateParserVar", "Error in Parser: '%s1'", e.GetMsg());
	}
}

const Variable& ARTdataContainer::GetParserVar() const
{
	if (avar_ == NULL)
	{
		throw ARTerror("ARTdataContainer::GetParserVar", "Current variable of data container '%s1' not defined!", varname_);
	}
	return *avar_;
}

const Variable& ARTdataContainer::GetParserVar()
{
	if (avar_ == NULL)
	{
		SetParserVar(varname_);
	}
	return *avar_;
}

void ARTdataContainer::DestroyParserVar()
{
	_DBG_MSG("");
//	if (!parser_) throw ARTerror("ARTdataContainer::DestroyParserVar","A parser variable for datacontainer '%s1' was created, but the datacontainers parser pointer is NULL. Please use SetParser() to specify which parser the data container should use.",varname_);
	try 
	{
		//delete value
		if (parser_)
		{
			parser_->RemoveVar(varname_);
		}
		//delete associated storage fields
		//delete aval;
		delete avar_;
		// we currently have no parser variable defined
		parserVarDefined_ = false;
	}
	catch(mup::ParserError& e)
	{
		throw ARTerror("ARTdataContainer::DestroyParserVar", "Error in Parser: '%s1'", e.GetMsg());
	}
}

string ARTdataContainer::DebugInfo()
{
	_DBG_MSG("");
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

ARTdataContainer& ARTdataContainer::GetArrayElement(int idx)
{
	_DBG_MSG("int");
	array_type* tmpArray = (array_type *) (val->na);
	ARTdataContainer* tmp;
	switch (typ)
	{
		case C_ART_na:
			tmp = dynamic_cast<ARTdataContainer*>((*tmpArray)[idx]);
			return *tmp;
			break;
		default:
			throw ARTerror("ARTdataContainer::GetArrayElement", "Data container '%s1' is no container of other data containers.", varname_);
			break;
	}
	return *this;
}

void ARTdataContainer::SetCurrentIndex(int idx)
{
	_DBG_MSG("int");
	array_type* tmpArray = (array_type *) (val->na);
	array_type::size_type arraySize;
	switch (typ)
	{
		case C_ART_na:
			arraySize = tmpArray->size();
//			cout << "Used buffer size of data container '"<< varname_ << "' = " << tmpArray->getUsedBufferSize() << endl;
			if (arraySize - 3 <= tmpArray->getUsedBufferSize())
			{
				resizeArray(arraySize + 5);
			}
			tmpArray->setCurrentIdx(idx);
			GetArrayElement(idx).Invalidate();
			break;
		default:
			throw ParserError();
			break;
	}
}

void ARTdataContainer::resizeArray(int newSize)
{
	_DBG_MSG("int");
	array_type* tmpArray = (array_type *) (val->na);
	ARTdataContainer* tmp;
	array_type::size_type oldSize, elementPtr, idx;
	oldSize = tmpArray->size();

//	for (idx = 0; idx < tmpArray->size(); ++idx)
//	{
//		tmp = dynamic_cast<ARTdataContainer*>(tmpArray->at(idx));
//		cout << "Content[" << idx << "] = " << tmp->val->c.re << endl;
//	}

	tmpArray->resize(newSize);
	elementPtr = (tmpArray->getFirst() + 1) % newSize;

	for (idx = oldSize; idx < newSize; ++idx)
	{
		tmp = new ARTdataContainer();
//		CBG_DBL
//		tmp->SetType(C_ART_cpx);
		tmp->SetType(C_ART_dbl);
//		tmp->SetVal(std::complex<double>(0,0));
		tmp->valid_ = false;
		tmp->parser_ = parser_;
		tmp->definition_ = definition_;
		tmp->parentContainer_ = this;
		tmp->varname_ = varname_;
		tmpArray->at(elementPtr) = tmp;
		elementPtr = (elementPtr + newSize + 1) % newSize;
	}

	len = newSize;

//	for (idx = 0; idx < tmpArray->size(); ++idx)
//	{
//		tmp = dynamic_cast<ARTdataContainer*>(tmpArray->at(idx));
//		cout << "Content[" << idx << "] = " << tmp->val->c.re << endl;
//	}
//
//	cout << "Resized array '" << varname_ << "' from " << oldSize << " to " << newSize << " elements." << endl;

}

// former ARTValue
//ARTValue& ARTdataContainer::operator=(const ARTdataContainer &a_Val)
//	{
//		_DBG_MSG("const ARTValue&");
//		//Assign(a_Val);
//		// TODO
//		return *this;
//	}

	//---------------------------------------------------------------------------
	IValue& ARTdataContainer::operator[](std::size_t i)
	{
		_DBG_MSG(varname_ << " std::size_t = " << (int) i);
		array_type* tmpArray = (array_type *) (val->na);
		ARTdataContainer* tmp;
		array_type::size_type arraySize;
		switch (typ)
		{
			case C_ART_na:
				arraySize = tmpArray->size();
				if (arraySize - 3 <= tmpArray->getUsedBufferSize())
				{
					resizeArray(arraySize + 5);
				}
				tmp = dynamic_cast<ARTdataContainer*>((*tmpArray)[i]);
				return *tmp;
				break;
			default:
				throw ParserError();
				break;
		}
		return *this;
	}

	//---------------------------------------------------------------------------
	IValue& ARTdataContainer::operator[](int i)
	{
		_DBG_MSG(varname_ << " int i = " << i);
		array_type* tmpArray = (array_type *) (val->na);
		ARTdataContainer* tmp;
		array_type::size_type arraySize;
		switch (typ)
		{
			case C_ART_na:
				// size check
				arraySize = tmpArray->size();
				if (arraySize - 3 <= tmpArray->getUsedBufferSize())
				{
					resizeArray(arraySize + 5);
				}
				tmp = dynamic_cast<ARTdataContainer*>((*tmpArray)[i]);
				return *tmp;
				break;
			default:
				throw ParserError();
				break;
		}
		return *this;
	}

	//---------------------------------------------------------------------------
	IToken* ARTdataContainer::Clone() const
	{
		_DBG_MSG("");
		return new ARTdataContainer(*this);
	}

	//---------------------------------------------------------------------------
	Value* ARTdataContainer::AsValue()
	{
		_DBG_MSG("");
		return new Value(*this);
	}

	//---------------------------------------------------------------------------
	IValue* ARTdataContainer::AsIValue()
	{
		_DBG_MSG("");
		return this;
	}

	//---------------------------------------------------------------------------
//	void ARTdataContainer::Assign(const ARTValue &ref)
//	{
//		_DBG_MSG("const ARTValue&");
//		if (this==&ref)
//			return;
//
//		// delete variable first
//		deleteVar();
//
//		//copy value
//		var = new ARTdataContainer(*ref.var);
//		own = true;
//
//		m_iFlags = ref.m_iFlags;
//
//		// in case the other variable is a container
//		if (var->typ == C_ART_na)
//		{
//			// create a new array and copy contents
//			arrayVals = new array_type(*(ref.arrayVals));
//			for (int i = 0; i < arrayVals->size(); ++i)
//			{
//				ARTValue* tmp = dynamic_cast<ARTValue*>((*(ref.arrayVals))[i]);
//				(*arrayVals)[i] = new ARTValue(*tmp);
//			}
//		}
//
//		// Do not copy the ARTValue cache pointer!
//		// ARTValue cache should be assigned explicitely and
//		// not implicitely (i.e. when retrieving the final result.)
//		//m_pCache = ref.m_pCache;
//	}

	//---------------------------------------------------------------------------
	IValue& ARTdataContainer::operator=(bool val) //should I even care about booleans?
	{
		_DBG_MSG("bool");
		SetVal(val);
		return *this;

		/*		If Parser errors are prefered, catch ARTerrors here and throw parser errors (Design the methods
				as shown here.) Parser errors might contain more information on where to find the problem...

		try
		{
			var->SetVal(val);
		}
		catch (ARTerror dummy)
		{
			ErrorContext err;
			err.Errc	= ecTYPE_CONFLICT;
			err.Type1 = GetType();
			err.Type2 = 'b';
			if (GetIdent().length())
			{
				err.Ident = GetIdent();
			}
			else
			{
				stringstream_type ss;
				ss << *this;
				err.Ident = ss.str();
			}

			throw ParserError(err);

		}
		return *this;
	*/
	}

	//---------------------------------------------------------------------------
	IValue& ARTdataContainer::operator=(int_type a_iVal)
	{
		_DBG_MSG("int_type");
		SetVal(a_iVal);
		return *this;
	}

	//---------------------------------------------------------------------------
	IValue& ARTdataContainer::operator=(float_type val)
	{
		_DBG_MSG("float_type");
		SetVal(val);
		return *this;
	}

	//---------------------------------------------------------------------------
	IValue& ARTdataContainer::operator=(string_type a_sVal)
	{
		_DBG_MSG("string_type");
		SetVal(a_sVal.c_str());
		return *this;
	}

	//---------------------------------------------------------------------------
	IValue& ARTdataContainer::operator=(const char_type *a_szVal)
	{
		_DBG_MSG("const char_type*");
		SetVal(a_szVal);
		return *this;
	}

	//---------------------------------------------------------------------------
	IValue& ARTdataContainer::operator=(const array_type &a_vVal)
	{
		_DBG_MSG("const array_type&");
		// TODO: implement this correctly
		return *this;
	}

	//---------------------------------------------------------------------------
	IValue& ARTdataContainer::operator=(const cmplx_type &val)
	{
		_DBG_MSG(varname_ << " const cmplx_type& " << val.real() << ","<< val.imag());
		SetVal(val);
		return *this;
	}

	//---------------------------------------------------------------------------
	char_type ARTdataContainer::GetType() const
	{
		_DBG_MSG("");
		switch (typ)
		{
			case C_ART_str: return 's';
			case C_ART_enum: return ' ';
			case C_ART_int: return 'i';
			case C_ART_dbl: return 'f';
			case C_ART_flo: return 'f';
			case C_ART_cpx: return 'c';
			case C_ART_tri: return ' ';
			case C_ART_mat: return ' ';
			case C_ART_matx: return ' ';
			case C_ART_nstr: return 'a';
			case C_ART_nint: return 'a';
			case C_ART_nflo: return 'a';
			case C_ART_ndbl: return 'a';
			case C_ART_ncpx: return 'a';
			case C_ART_ntri: return 'a';
			case C_ART_nmat: return 'a';
			case C_ART_nmatx: return 'a';
			case C_ART_na: return 'a';
			case C_ART_undef:
			default: return ' ';
		}

	}

	//---------------------------------------------------------------------------
	float_type ARTdataContainer::GetImag() const
	{
		_DBG_MSG("");
		ARTdataContainer* tmp;
		array_type* tmpArray = (array_type*) (val->na);
		if (!valid_) Evaluate();
		try
		{
			switch(typ)
			{
				//simple types: copy orig.value
				case C_ART_int:
				case C_ART_flo:
				case C_ART_dbl: return 0; break;
				case C_ART_cpx: return val->c.im; break; //Imagin�rteil der komplexen Zahl

				case C_ART_nint:
				case C_ART_nflo:
				case C_ART_ndbl:
					if (len > 1) throw ParserError();
					else return 0;
					break;

				case C_ART_ncpx:
					if (len > 1) throw ParserError();
					else return val->nc->im;
					break;

				// in case of C_ART_na we have to get to first element
				// of array
				case C_ART_na:
					if (len > 1) throw ParserError();
					else
					{
						tmp = dynamic_cast<ARTdataContainer*>((*tmpArray)[0]);
						return tmp->val->c.im;
					}

				default: throw ParserError();
			}
		}
		catch (ParserError& dummy)
		{
			ErrorContext err;
			err.Errc	= ecTYPE_CONFLICT;
			err.Type1 = GetType();
			err.Type2 = 'c';

			if (GetIdent().length())
			{
				err.Ident = GetIdent();
			}
			else
			{
				stringstream_type ss;
				ss << *this;
				err.Ident = ss.str();
			}

			throw ParserError(err);

		}
		// never to be reached, just to suppress compiler warnings
		return 0;
	}

	//---------------------------------------------------------------------------
	const cmplx_type& ARTdataContainer::GetComplex() const
	{
		_DBG_MSG("");
//		return m_val;
		static cmplx_type tmp;
		if (!valid_) Evaluate();
		switch (typ)
		{
		case C_ART_dbl:
			tmp = cmplx_type(val->d,0);
			break;
		case C_ART_flo:
			tmp = cmplx_type(val->f,0);
			break;
		case C_ART_int:
			tmp = cmplx_type(val->i,0);
			break;
		case C_ART_cpx:
			tmp = cmplx_type(val->c.re,val->c.im);
			break;
		default:
			throw ARTerror("ARTdataContainer::GetComplex()", "The current data type cannot be converted into a complex value!");
		}
		return tmp;
	}

	//---------------------------------------------------------------------------
	const string_type& ARTdataContainer::GetString() const
	{
		_DBG_MSG("");
/*		CheckType('s');
		assert(m_psVal!=NULL);
		return *m_psVal;*/
		static string tmp;
		if (typ == C_ART_str)
		{
			tmp = val->s;
		}
		else
		{
			tmp = "";
		}
		return tmp;

	}

	//---------------------------------------------------------------------------
	bool ARTdataContainer::GetBool() const
	{
		_DBG_MSG("");
/*		CheckType('b');
		return m_val.real()==1;*/
		return false;
	}

	//---------------------------------------------------------------------------
	const array_type& ARTdataContainer::GetArray() const
	{
		_DBG_MSG("");
		/*CheckType('a');
		assert(m_pvVal!=NULL);
		return *m_pvVal;*/
		array_type* tmpArray = (array_type *) (val->na);
		if (typ == C_ART_na)
		{
			return *tmpArray;
		}
		else
		{
			ErrorContext err;
			err.Errc	= ecTYPE_CONFLICT;
			err.Type1 = GetType();
			err.Type2 = 'a';

			if (GetIdent().length())
			{
				err.Ident = GetIdent();
			}
			else
			{
				stringstream_type ss;
				ss << *this;
				err.Ident = ss.str();
			}

			throw ParserError(err);

		}
	}

	//---------------------------------------------------------------------------
	void ARTdataContainer::CheckType(char_type a_cType) const
	{
		_DBG_MSG("char_type");
		if (GetType()!=a_cType)
		{
			ErrorContext err;
			err.Errc	= ecTYPE_CONFLICT;
			err.Type1 = GetType();
			err.Type2 = a_cType;

			if (GetIdent().length())
			{
				err.Ident = GetIdent();
			}
			else
			{
				stringstream_type ss;
				ss << *this;
				err.Ident = ss.str();
			}

			throw ParserError(err);
		}
	}

	//---------------------------------------------------------------------------
	bool ARTdataContainer::IsVolatile() const
	{
		_DBG_MSG("");
		return IsFlagSet(IValue::flVOLATILE);
//		return true;
	}

//	//---------------------------------------------------------------------------
//	string_type ARTdataContainer::AsciiDump() const
//	{
//		_DBG_MSG("");
//		stringstream_type ss;
//
//		ss << g_sCmdCode[ GetCode() ];
//		ss << _T(" [addr=0x") << std::hex << this << std::dec;
//		ss << _T("; type=\"") << GetType() << _T("\"");
//		ss << _T("; val=");
//
//		switch(typ)
//		{
//			case C_ART_str: ss << val->s; break;
//			case C_ART_enum: ss << val->e; break;
//			case C_ART_int:  ss << val->i; break;
//			case C_ART_dbl:  ss << val->d; break;
//			case C_ART_flo:  ss << val->f; break;
//			case C_ART_cpx:  ss << val->c.re << "+" << val->c.im << "i"; break;
//			case C_ART_tri:  ss << _T("(tri)"); break;
//			case C_ART_mat:  ss << _T("(matrix)"); break;
//			case C_ART_matx:  ss << _T("(matrix)"); break;
//			case C_ART_nstr: ss << _T("(array)"); break;
//			case C_ART_nint: ss << _T("(array)"); break;
//			case C_ART_nflo: ss << _T("(array)"); break;
//			case C_ART_ndbl: ss << _T("(array)"); break;
//			case C_ART_ncpx: ss << _T("(array)"); break;
//			case C_ART_ntri: ss << _T("(array)"); break;
//			case C_ART_nmat: ss << _T("(array)"); break;
//			case C_ART_nmatx: ss << _T("(array)"); break;
//			case C_ART_na: ss << _T("(array)"); break;
//			case C_ART_undef: ss << _T("(undef)"); break;
//		}
//
//		ss << ((IsFlagSet(IToken::flVOLATILE)) ? _T("; ") : _T("; not ")) << _T("volatile");
//		ss << _T("]");
//
//		return ss.str();
//	}
//
//	//---------------------------------------------------------------------------
//	void ARTdataContainer::Release()
//	{
//		_DBG_MSG("");
//		if (m_pCache_)
//			std::cout << "ReleaseToCache not implemented\n";
//			//m_pCache->ReleaseToCache(this);
//		else
//			delete this;
//	}
//
//	//---------------------------------------------------------------------------
//	void ARTdataContainer::BindToCache(ValueCache *pCache)
//	{
//		_DBG_MSG("");
//		m_pCache_ = pCache;
//	}


#endif
