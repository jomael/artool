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

#ifndef ARTDATACONTAINER_H
#define ARTDATACONTAINER_H


#include <string>
#include <complex>

#include "ARTlink.h"

// includes from muParser
#include "mpParser.h" //braucht ARTvariant
#include "mpARTValue.h"
#include "mpIValue.h"
#include "mpTypes.h"

#include "ARTerror.h"

using namespace mup;

class ARTSimulator;
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
	: progressFunction_(NULL), count_(0), nfuncCalls_(1), active_(false)
	{ }

	ARTprogressIndicator(TprogressFunction progressFunction, int nfuncCalls)
	: progressFunction_(progressFunction), count_(0), nfuncCalls_(nfuncCalls), active_(false)
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
class ARTdataContainer : public ARTvariant, public IValue {
//class ARTdataContainer : public ARTvariant {
protected:
	mutable bool valid_;		///< true if value is consistent with definition
	bool counted_;		
	mutable bool eval_started;
	ARTfunctionoid* func_;
	// double complexity_; ///< after a calculation the amount of needed CPU ticks divided by the number of iterations is saved to estimate future evaluation cost more accurately

	list<ARTdataContainer*>	clientList_;			///< other dataContainers depending on this one (need invalidation when value changes)
	list<ARTdataContainer*>::iterator citer_;
	mutable list<ARTdataContainer*>	dependencyList_;		///< other dataContainers this one depends on (variables in expression): need lookup whenever value is evaluated
	mutable list<ARTdataContainer*>::iterator diter_;

	ARTdataContainer* parentContainer_;

	void SetCountedFlag(bool b);

public:
	void EvaluateIfInvalid()
	{
		if (!valid_) Evaluate();
	}

protected:
	string definition_;	///< textual representation of property value (symbolic expression)
	ParserX* parser_;	///< expression parser instance
	ARTSimulator* scope_;	///< scope of variable names
	string varname_; ///< name of corresponding variable in parser
	virtual int EvaluationCost();
 	virtual void Evaluate() const;
	/**
	 * Add a new client to this dataContainer. Enter client into clientList if it is not
	 * already there. If the dependency relation is mutual (as in most cases) consider using 
	 * AddDependency.
	 */	 
	void AddNotify(ARTdataContainer* client);
	/**
	 * @brief Resizes the used array in case the current data container is
	 *        of type C_ART_na.
	 * @param[in] newSize New size of the array.
	 */
	virtual void resizeArray(int newSize);

protected:
	//used for parser
	mup::ARTValue* aval_;
	mup::Variable* avar_;
	bool parserVarDefined_;
	string tempDef_;
	string parentModuleName_;

private:

	//array_type* arrayVals_;
	//EFlags 			m_iFlags_; ///< Additional flags
	ValueCache	*m_pCache_; ///< Pointer to the ARTValue Cache

	void CheckType(char_type a_cType) const;
	//void Assign(const ARTValue &a_Val);
	void Reset();

	//virtual void Release();

public:
	void DebugDepTree(const string intend, const string linebreak = "\n");
	string WriteDepTree(const string intend, const string linebreak = "\n");

	static ARTprogressIndicator progressIndicator;

	ARTdataContainer(); /**< default constructor */

	ARTdataContainer(const T_ART_Type dtyp, const int dlen, const string varname = "");
	ARTdataContainer(std::string name, ARTfunctionoid* func = NULL);

	ARTdataContainer(const int i);
	ARTdataContainer(const double d);
	ARTdataContainer(const float f);
	ARTdataContainer(const char* s);
	ARTdataContainer(const char* s1, const char* s2);
	ARTdataContainer(const char* s1, const char* s2, const char* s3);
	ARTdataContainer(const char* s1, const char* s2, const char* s3, const char* s4);

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
		if ((!valid_) && (typ != C_ART_str) && (typ != C_ART_nstr) && (typ != C_ART_na))
			Evaluate(); 
		return val;
	}

	//float_type GetFloat();

	//double GetValueAsDoubleFromIndex(std::size_t ind);

	//void SetInvalid(int st, int end);

	bool IsValid() {return valid_;}

	int GetInt(); 

	const string& GetDefinition();

	/** 
	 * direct change of data value (ignore definition string); set value pointer and force
	 * value to be valid.	 
	 */
	void SetValue(ARTvariant* var);

	virtual void SetVal(const int i, const int ind = 0);
	virtual void SetVal(const double d, const int ind = 0);
	virtual void SetVal(const float f, const int ind = 0);
	virtual void SetVal(std::complex<double>, const int ind = 0);
	virtual void SetVal(const double re, const double im, const int ind = 0);
	virtual void SetVal(const char* s);
	virtual void SetVal(const char* s, int ind);

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

	void SetParser(ParserX* p);
	virtual ParserX* GetParser() {return parser_;}
	virtual ARTdataContainer* GetParent() {return parentContainer_;}

	void Rename(const string& newname);

	void SetParserVar(const string& varname);
	virtual const Variable& GetParserVar() const;
	virtual const Variable& GetParserVar();

	void DestroyParserVar();

	string GetVarName() const {return varname_;}

	// change definition string (no immediate re-evaluation)
	void SetDefinition(const string& s, ARTSimulator* scope);

	// set definition string without setting the parser
	void SetDefinition(const string& s);

	//if there is a definition string, redo all dependencies
	void RedoDefinitionDependencies();

	//see c-file
	void ResetEvaluation();
	//needed for debugging only
	void SetScope(ARTSimulator* sim) {scope_ = sim;}
	ARTSimulator* GetScope() {return scope_;}

	void SetFunction(ARTfunctionoid* func);

	string DebugInfo();

	list<ARTdataContainer*>	GetClientList(){return clientList_;}
	bool CheckValidity() ;

	ARTdataContainer& GetArrayElement(int idx);
	void SetCurrentIndex(int idx);

	void SetParentModuleName(const string& name)
	{
		parentModuleName_ = name;
	}

	const string& GetParentModuleName() const
	{
		return parentModuleName_;
	}

	// former ARTValue

	/** Array constructor */
	//ARTValue(int_type m, float_type v, T_ART_Type type = C_ART_ndbl);

	/** Matrix constructor */
	//ARTValue(int_type m, int_type n, float_type v);
	//ARTdataContainer& operator=(const ARTdataContainer &a_Val);

	void deleteVar();

	virtual IValue& operator[](std::size_t idx);
	virtual IValue& operator[](int idx);

	virtual IValue& operator=(int_type a_iVal);
	virtual IValue& operator=(float_type a_fVal);
	virtual IValue& operator=(string_type a_sVal);
	virtual IValue& operator=(bool val);
	virtual IValue& operator=(const array_type &a_vVal);
	virtual IValue& operator=(const cmplx_type &val);
	virtual IValue& operator=(const char_type *a_szVal);
	virtual char_type GetType() const;
	virtual float_type GetFloat() const;
	virtual float_type GetImag() const;
	virtual bool GetBool() const;
	virtual const cmplx_type& GetComplex() const;
	virtual const string_type& GetString() const;
	virtual const array_type& GetArray() const;
	virtual bool IsVolatile() const;
	virtual IToken* Clone() const;
	virtual Value* AsValue();
	virtual IValue* AsIValue();

	//virtual ARTdataContainer* GetContainer() const;

//	virtual string_type AsciiDump() const;
//	void BindToCache(ValueCache *pCache);

};

/** 
 * @}
 */

#endif
