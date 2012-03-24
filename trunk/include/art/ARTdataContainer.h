
#ifndef ARTDATACONTAINER_H
#define ARTDATACONTAINER_H


#include <string>
#include <complex>
#include "mpParser.h" //braucht ARTvariant
//#include "ARTerror.h"
//#include "ARTlink.h"
//#include "matrix.h"
//#include "ARTfunctionoid.h"

#include "Interface.h"
using mup::ParserX;

class ARTsimulator;
class ARTobject;
// cbg: forward declaration
class ARTfunctionoid;

/** \addtogroup ARTimpl
 * @{
 */


/**
 * The user can specify a callback function, which is called repeatedly whenever data 
 * containers are evaluated. The callback function must be of the following form:
 * @code
 *    bool myCallbackFunction(double myProgressInfo);
 * @endcode
 * If the evaluation cost has been determined before, the callback function receives a 
 * floating-point number between 0 and 1, where 1 means the process is complete. 
 * The callback function must return true to continue the calculation or false to abort it. 
 * In that case an ARTabort exception is thrown, which must be catched at some higher 
 * level in the programme.
 */ 
typedef bool (*TprogressFunction) (double, const char*);

/**
 * The static instance of this class in ARTdataContainer calls the 
 * TprogressFunction, if the user has specified one. See TprogressFunction for more details. 
 * @see TprogressFunction
 */
class ARTprogressIndicator
{
public:
	ARTprogressIndicator()
	: progressFunction_(NULL), nfuncCalls_(1), count_(0), active_(false) 
	{ }

	ARTprogressIndicator(TprogressFunction progressFunction, int nfuncCalls)
	: progressFunction_(progressFunction), nfuncCalls_(nfuncCalls), count_(0), active_(false) 
	{
		//to avoid division by 0
		if (nfuncCalls_ == 0) nfuncCalls_ = 1;
	}

	void SetProgressFunction(TprogressFunction progressFunction) {progressFunction_ = progressFunction;}
	void SetNFunctionCalls(int nfuncCalls) {nfuncCalls_ = nfuncCalls; if (nfuncCalls_ == 0) nfuncCalls_ = 1;}
	void CallProgressfunction(bool call) {active_ = call;}
	void Reset() {count_ = 0;}
	void Reset(int nfuncCalls = 1) 
	{	
		count_ = 0;
		SetNFunctionCalls(nfuncCalls);
	}

	void Continue(double d,std::string s)
	{
		//count_++;
		count_ = count_ + d;
		//std::cout << " (c: " << count_ << " / " << nfuncCalls_ << ") in " << s << "\n";
		//cout << count_ << "/" << nfuncCalls_ << "\n";
		//if a progress function is defined...
		if ((progressFunction_)&&(active_))
			//...call it and abort if it returns false
			if (!progressFunction_(count_/float(nfuncCalls_), s.c_str()) )
				throw ARTabort( count_/float(nfuncCalls_) );
	}

private:
	TprogressFunction progressFunction_;
	double count_;
	double nfuncCalls_;
	bool active_;

};

/**
 * In addition to a value (ARTvariant) of some kind, this class can also contain 
 * the formula to (re)calculate its value. The formula can be a parser expression or a 
 * functionoid. The ARTdataContainer also implements the client observer pattern, ensuring 
 * that if a value is changed, all values that depend on the changed one are invalidated 
 * and subject to recalculation, when their value is queried. Users must be carful to 
 * avoid circular references. 
 * 
 * Use ARTdataContainer for values which have a small scope and when the name of the value 
 * you are processing is unimportant, because you can keep track of it. Use ARTdataProp 
 * when you want to add values to an ARTobject, they can then be added to the list of 
 * properties of the object. ARTdataProp is also useful when you have several values, and 
 * you can not keep track of their names easily without storing them as strings within the
 * object.
 */ 
class ARTdataContainer : public ARTvariant {
protected:
	bool valid_;		///< true if value is consistent with definition
	bool counted_;		
	bool eval_started;
	ARTfunctionoid* func_;
	// double complexity_; ///< after a calculation the amount of needed CPU ticks divided by the number of iterations is saved to estimate future evaluation cost more accurately

	list<ARTdataContainer*>	clientList_;			///< other dataContainers depending on this one (need invalidation when value changes) 
	list<ARTdataContainer*>::iterator citer_;			
	list<ARTdataContainer*>	dependencyList_;		///< other dataContainers this one depends on (variables in expression): need lookup whenever value is evaluated
	list<ARTdataContainer*>::iterator diter_;		  

	void SetCountedFlag(bool b);

	void EvaluateIfInvalid()
	{
		if (!valid_) Evaluate();
	}

	string definition_;	///< textual representation of property value (symbolic expression)
	ParserX* parser_;	///< expression parser instance
	ARTsimulator* scope_;	///< scope of variable names
	string varname_; ///< name of corresponding variable in parser
	virtual int EvaluationCost();
 	virtual void Evaluate();
	/**
	 * Add a new client to this dataContainer. Enter client into clientList if it is not
	 * already there. If the dependency relation is mutual (as in most cases) consider using 
	 * AddDependency.
	 */	 
	void AddNotify(ARTdataContainer* client);

private:
	//used for parser
	mup::ARTValue* aval;
	mup::Variable* avar;
	bool parserVarDefined;
	string tempDef;
public:
	void DebugDepTree(const string intend, const string linebreak = "\n");
	string WriteDepTree(const string intend, const string linebreak = "\n");

	static ARTprogressIndicator progressIndicator;

	ARTdataContainer(const T_ART_Type dtyp, const int dlen, const string varname = "");
	ARTdataContainer(std::string name, ARTfunctionoid* func = NULL);

	///copy constructor
	ARTdataContainer(const ARTdataContainer& orig);

	virtual ~ARTdataContainer();

	ARTdataContainer* clone() {return new ARTdataContainer(*this);}

	/// @returns variant data type
	T_ART_Type GetDatatype() {return typ;}

	/// @returns variant vector length
	int GetCount() {return len;}

	/// @returns number of necessary iterations over formula to calculate (eg. the number of frq-indices we need to make the calculation for)
	int GetIterationNumber();

	/// This function is not used right now. @returns the complexity of the calculation of this data container as was determined in previous calculations
	double GetComplexity()
	{
		//return complexity_; 
		return 1;
	}

	/// @returns data pointer, re-evaluates parser expression or functionoid if necessary (but only when numeric value)
	T_ART_Var* GetValue() 
	{
		// evaluate if necessary (but only when numeric value) then return value pointer
		if ((!valid_) && (typ != C_ART_str) && (typ != C_ART_nstr))
			Evaluate(); 
		return val;
	}

	double GetValueAsDouble();	

	int GetValueAsInt(); 

	const string& GetDefinition();

	/** 
	 * direct change of data value (ignore definition string); set value pointer and force
	 * value to be valid.	 
	 */
	void SetValue(ARTvariant* var);

	/// mark value as invalid and invalidate all dependent data containers
	void Invalidate();

	/// invalidate all dependent data containers
	void NotifyClients() ;

	int GetEvaluationCost();

	/**
	 * Observe AND be notified. Since most dependency relations are mutual, this function 
	 * will write the dataContainer pointed to by dependency to its list of dependencies 
	 * (ie. the dataContainers that need to be evaluated before this one can be evaluated)
	 * and adds itself as a client to the dataContainer specified by dependency, so it will
	 * be notified wenn the value of the latter changes.    
	 */   	 
	void AddDependency(ARTdataContainer* dependency);

	void RemoveAllDependencies();

  /**
   * Add all data properties of the ARTobject pointed to by obj as dependencies 
   */   
	void AddPropertiesAsDependency(ARTobject* obj);

	// notification about loss of client
	// remove calling object from clientList if it is there
	void DoNotNotify(ARTdataContainer* client);

	void RemoveDependency(ARTdataContainer* dependency);

	void RemoveFromDefinition(ARTdataContainer* dependency);

	void SetParser(ParserX* p) 
	{
		parser_ = p;
	}

	void Rename(const string& newname);

	void CreateParserVar(const string& varname);

	void DestroyParserVar();

	string GetVarName() {return varname_;}

	// change definition string (no immediate re-evaluation)
	void SetDefinition(const string s, ARTsimulator* scope);

	//if there is a definition string, redo all dependencies
	void RedoDefinitionDependencies();

	//see c-file
	void ResetEvaluation();
	//needed for debugging only
	ARTsimulator* GetScope() {return scope_;}

	void SetFunction(ARTfunctionoid* func);

	string DebugInfo();

	list<ARTdataContainer*>	GetClientList(){return clientList_;}
	bool CheckValidity() ;
};

/** 
 * @}
 */

#endif
