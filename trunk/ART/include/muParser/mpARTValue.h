
#ifndef MUP_ART_VALUE_H
#define MUP_ART_VALUE_H

//--- Standard includes ------------------------------------------------------------
#include <complex>
#include <list>

//--- Parser framework -------------------------------------------------------------
#include "mpIValue.h"
#include "mpTypes.h"

//--- ART Interface ------------------------------------------------------------------
#include "ARTlink.h"

namespace ART{
// forward declaration
struct DataContainer;
}
//struct ARTvariant;

/** \addtogroup ARTimpl
 * @{
 */

MUP_NAMESPACE_START

  /**
   * This class implements the muParser's IValue interface and lets the parser 
   * write directly to an ARTvariant structure. So variables can be directly 
   * accessed by the parser, the parser does not store the values in any other 
   * place than we do (well, except for caching or whatever).
   */   	
  class ARTValue : public IValue
	{
	public:
		
		ARTValue();
		ARTValue(int_type val);
		ARTValue(bool_type val);
		ARTValue(float_type val);
		ARTValue(string_type val);
		ARTValue(const char_type *val);
		ARTValue(const cmplx_type &v);
		ARTValue(const array_type &val);
		ARTValue(ART::DataContainer* av);

		/** Array constructor */
		ARTValue(int_type m, float_type v, T_ART_Type type = C_ART_ndbl);

		/** Matrix constructor */
		ARTValue(int_type m, int_type n, float_type v);

		ARTValue(const ARTValue &a_Val );
		ARTValue(const IValue &a_Val);
		ARTValue& operator=(const ARTValue &a_Val);

		virtual ~ARTValue();

		void deleteVar();

		virtual IValue& operator[](std::size_t idx);

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

		virtual string_type AsciiDump() const;
		void BindToCache(ValueCache *pCache);

	private:

		//ARTvariant* var;
		ART::DataContainer* var;
		ARTValue* arrayVals; ///< Needed for arrays of DataContainers
		EFlags 			m_iFlags; ///< Additional flags
		ValueCache	*m_pCache; ///< Pointer to the ARTValue Cache
		bool own; ///< does the var pointer only belong to this object?

		void CheckType(char_type a_cType) const;
		void Assign(const ARTValue &a_Val);
		void Reset();

		virtual void Release();
	}; //end: class ARTValue

MUP_NAMESPACE_END

/** 
 * @}
 */

#endif


