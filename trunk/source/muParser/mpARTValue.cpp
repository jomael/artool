
#include "mpARTValue.h"
#include "mpError.h"
#include "mpValueCache.h"
#include "mpValue.h"
	

MUP_NAMESPACE_START

	//------------------------------------------------------------------------------
	ARTValue::ARTValue()
		:IValue(cmVAL)
		,m_iFlags(flNONE)
		,m_pCache(NULL)
	{
		var = new ARTvariant(0.0);
		own = true;
	}

	//---------------------------------------------------------------------------
	ARTValue::ARTValue(int_type a_iVal)
		:IValue(cmVAL)
		,m_iFlags(flNONE)
		,m_pCache(NULL)
	{
		var = new ARTvariant(a_iVal);
		own = true;
	}

	//---------------------------------------------------------------------------
	ARTValue::ARTValue(bool_type a_bVal)
		:IValue(cmVAL)
		,m_iFlags(flNONE)
		,m_pCache(NULL)
	{
		//we don't have a boolean type...
		var = new ARTvariant((int)(a_bVal));
		own = true;
	}

	//---------------------------------------------------------------------------
	ARTValue::ARTValue(string_type a_sVal)
		:IValue(cmVAL)
		,m_iFlags(flNONE)
		,m_pCache(NULL)
	{
		var = new ARTvariant(a_sVal.c_str());
		own = true;
	}

	//---------------------------------------------------------------------------
	ARTValue::ARTValue(int_type array_size, float_type val)
		:IValue(cmVAL)
		,m_iFlags(flNONE)
		,m_pCache(NULL)
	{
		var = new ARTvariant(C_ART_ndbl);
		var->SetArrayLength(array_size);
		for (int i = 0; i < array_size; i++)
			var->val->nd[i] = val;
		own = true;
	}
/* brauchma net */
	//---------------------------------------------------------------------------
	ARTValue::ARTValue(int_type m, int_type n, float_type val)
		:IValue(cmVAL)
/*		,m_val()
		,m_psVal(NULL)
		,m_pvVal(NULL)
		,m_cType('a')*/
		,m_iFlags(flNONE)
		,m_pCache(NULL)
	{
/*		// m - spalten
		// n - reihen
		m_pvVal = new array_type(n, ARTValue()); 
		for (int_type i=0; i<n; ++i)
		{
			(*m_pvVal)[i] = ARTValue(m, val);
		}
		*/
		std::cout << "ARTValue::ARTValue(int_type m, int_type n, float_type val) not implemented\n";
	}
	//---------------------------------------------------------------------------
	ARTValue::ARTValue(const char_type *a_szVal)
		:IValue(cmVAL)
		,m_iFlags(flNONE)
		,m_pCache(NULL)
	{
		var = new ARTvariant(a_szVal);
		own = true;
	}

	//---------------------------------------------------------------------------
	ARTValue::ARTValue(const cmplx_type &v)
		:IValue(cmVAL)
		,m_iFlags(flNONE)
		,m_pCache(NULL)
	{
		var = new ARTvariant(C_ART_cpx);
		var->val->c.re = v.real();
		var->val->c.im =v.imag();
		own = true;
	}

	//---------------------------------------------------------------------------
	ARTValue::ARTValue(float_type val)
		:IValue(cmVAL)
		,m_iFlags(flNONE)
		,m_pCache(NULL)
	{
		var = new ARTvariant(val);
		own = true;
	}
/**/
	//---------------------------------------------------------------------------
	ARTValue::ARTValue(const array_type &val)
		:IValue(cmVAL)
/*		,m_val()
		,m_psVal(NULL)
		,m_pvVal(new array_type(val))
		,m_cType('a') */
		,m_iFlags(flNONE)
		,m_pCache(NULL)
	{
		std::cout << "ARTValue::ARTValue(const array_type &val) not implemented\n";
	}

	ARTValue::ARTValue(ARTvariant* av)
		:IValue(cmVAL)
		,m_iFlags(flNONE)
		,m_pCache(NULL)
	{
		var = av;
		own = false; //var is connected to foreign ARTvariant, so var must not be destroyed by this object
	}

/**/
	//---------------------------------------------------------------------------
	ARTValue::ARTValue(const ARTValue &a_Val)
		:IValue(cmVAL)
		,m_pCache(NULL)
	{
		Assign(a_Val);
	}

	//---------------------------------------------------------------------------
	ARTValue::ARTValue(const IValue &a_Val)
		:IValue(cmVAL)
		,m_pCache(NULL)
	{
		Reset();
/*
		switch(a_Val.GetType())
		{
		case 'i': 
		case 'f': 
		case 'b': m_val = cmplx_type(a_Val.GetFloat(), 0);
							break;


		case 'c': m_val = cmplx_type(a_Val.GetFloat(), a_Val.GetImag());
							break;

		case 's': if (!m_psVal)
								m_psVal = new string_type(a_Val.GetString());
							else
							 *m_psVal = a_Val.GetString();
							break;

		case 'a': if (!m_pvVal) 
								m_pvVal = new array_type(a_Val.GetArray());
							else
							 *m_pvVal	= a_Val.GetArray();	
							break;

		case 'v': break;
		default:	MUP_FAIL(INVALID_TYPE_CODE);
		}
		
		m_cType = a_Val.GetType();
		*/
	}

	//---------------------------------------------------------------------------
	ARTValue& ARTValue::operator=(const ARTValue &a_Val)
	{
		Assign(a_Val);
		return *this;
	}

	//---------------------------------------------------------------------------
	IValue& ARTValue::operator[](std::size_t i)
	{
/*
		if ((var->len<1) || (m_pvVal==NULL))
			throw ParserError( ErrorContext(ecAPI_NOT_AN_ARRAY) ); 

		if (i>=var->len)
			throw ParserError( ErrorContext(ecINDEX_OUT_OF_BOUNDS) ); 

	case var->typ
	...
		var->val->nc
	 // return (*m_pvVal)[i];
	 */
		std::cout << "IValue& ARTValue::operator[](std::size_t i) not implemented\n";
		return *this;
	}

	//---------------------------------------------------------------------------
	ARTValue::~ARTValue()
	{
		if (own)
		{
			delete var;
			own = false;
		}
	}

	//---------------------------------------------------------------------------
	IToken* ARTValue::Clone() const
	{
		return new ARTValue(*this);
	}

	//---------------------------------------------------------------------------
	Value* ARTValue::AsValue()
	{
		return new Value(this);
	}

	//---------------------------------------------------------------------------
	IValue* ARTValue::AsIValue()
	{
		return this;
	}

	//---------------------------------------------------------------------------
	void ARTValue::Assign(const ARTValue &ref)
	{
		if (this==&ref)
			return;

		//copy value
		var = new ARTvariant(*ref.var);
		own = true;

		m_iFlags = ref.m_iFlags;

		// Do not copy the ARTValue cache pointer!
		// ARTValue cache should be assigned expplicitely and
		// not implicitely (i.e. when retrieving the final result.)
		//m_pCache = ref.m_pCache;
	}

	//---------------------------------------------------------------------------
	void ARTValue::deleteVar()
	{
		if (own) delete var;
		var = NULL;
		own = false;
	}

	//---------------------------------------------------------------------------
	void ARTValue::Reset()
	{
		deleteVar();
		var = new ARTvariant();
		own = true;

		m_iFlags = flNONE;
	}

	//---------------------------------------------------------------------------
	IValue& ARTValue::operator=(bool val) //should I even care about booleans?
	{
		var->SetVal(val);
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
	IValue& ARTValue::operator=(int_type a_iVal)
	{
		var->SetVal(a_iVal);
		return *this;
	}

	//---------------------------------------------------------------------------
	IValue& ARTValue::operator=(float_type val)
	{
		var->SetVal(val);
		return *this;
	}

	//---------------------------------------------------------------------------
	IValue& ARTValue::operator=(string_type a_sVal)
	{
		var->SetVal(a_sVal.c_str());
		return *this;
	}

	//---------------------------------------------------------------------------
	IValue& ARTValue::operator=(const char_type *a_szVal)
	{
		var->SetVal(a_szVal);
		return *this;
	}

	//---------------------------------------------------------------------------
	IValue& ARTValue::operator=(const array_type &a_vVal)
	{
		return *this;
	}

	//---------------------------------------------------------------------------
	IValue& ARTValue::operator=(const cmplx_type &val)
	{
		var->SetVal(val);
		return *this;
	}

	//---------------------------------------------------------------------------
	char_type ARTValue::GetType() const
	{
		switch (var->typ)
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
			case C_ART_undef: return ' ';  
		}
	}

	//---------------------------------------------------------------------------
	float_type ARTValue::GetFloat() const
	{
		try
		{
			switch(var->typ)
			{
				//simple types: copy orig.value
				case C_ART_int: return var->val->i; break;
				case C_ART_flo: return var->val->f; break;
				case C_ART_dbl: return var->val->d; break;
				case C_ART_cpx: return var->val->c.re; break; //Realteil der komplexen Zahl

				case C_ART_nint:
					if (var->len > 1) throw ParserError();
					else return *(var->val->ni);
					break;

				case C_ART_nflo: 
					if (var->len > 1) throw ParserError();
					else return *(var->val->nf);
					break;

				case C_ART_ndbl: 
					if (var->len > 1) throw ParserError();
					else return *(var->val->nd);
					break;

				case C_ART_ncpx: 
					if (var->len > 1) throw ParserError();
					else return var->val->nc->re;
					break;

				default: throw ParserError();
			}
		}
		catch (ParserError dummy)
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
	}
/*		if (!IsScalar() && m_cType!='b')
		{
			ErrorContext err;
			err.Errc	= ecTYPE_CONFLICT;
			err.Type1 = m_cType;
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

		return m_val.real();
	}
*/
	//---------------------------------------------------------------------------
	float_type ARTValue::GetImag() const
	{
		try
		{
			switch(var->typ)
			{
				//simple types: copy orig.value
				case C_ART_int: 
				case C_ART_flo: 
				case C_ART_dbl: return 0; break;
				case C_ART_cpx: return var->val->c.im; break; //Imaginärteil der komplexen Zahl

				case C_ART_nint:
				case C_ART_nflo: 
				case C_ART_ndbl: 
					if (var->len > 1) throw ParserError();
					else return 0;
					break;

				case C_ART_ncpx: 
					if (var->len > 1) throw ParserError();
					else return var->val->nc->im;
					break;

				default: throw ParserError();
			}
		}
		catch (ParserError dummy)
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
	}

	//---------------------------------------------------------------------------
	const cmplx_type& ARTValue::GetComplex() const
	{
//		return m_val;
		return 1;
	}

	//---------------------------------------------------------------------------
	const string_type& ARTValue::GetString() const
	{
/*		CheckType('s');
		assert(m_psVal!=NULL);
		return *m_psVal;*/
		return "test";

	}

	//---------------------------------------------------------------------------
	bool ARTValue::GetBool() const
	{
/*		CheckType('b');
		return m_val.real()==1;*/
		return false;
	}

	//---------------------------------------------------------------------------
	const array_type& ARTValue::GetArray() const
	{
		/*CheckType('a');
		assert(m_pvVal!=NULL);
		return *m_pvVal;*/
		std::vector<class mup::Value> dummy;
		return dummy;
	}

	//---------------------------------------------------------------------------
	void ARTValue::CheckType(char_type a_cType) const
	{
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
	bool ARTValue::IsVolatile() const
	{
		return IsFlagSet(IValue::flVOLATILE);
//		return true;
	}

	//---------------------------------------------------------------------------
	string_type ARTValue::AsciiDump() const
	{
		stringstream_type ss;

		ss << g_sCmdCode[ GetCode() ];
		ss << _T(" [addr=0x") << std::hex << this << std::dec;
		ss << _T("; type=\"") << GetType() << _T("\"");
		ss << _T("; val=");

		switch(var->typ)
		{
			case C_ART_str: ss << var->val->s; break; 
			case C_ART_enum: ss << var->val->e; break; 
			case C_ART_int:  ss << var->val->i; break;
			case C_ART_dbl:  ss << var->val->d; break;
			case C_ART_flo:  ss << var->val->f; break;
			case C_ART_cpx:  ss << var->val->c.re << "+" << var->val->c.im << "i"; break;
			case C_ART_tri:  ss << _T("(tri)"); break;
			case C_ART_mat:  ss << _T("(matrix)"); break;
			case C_ART_matx:  ss << _T("(matrix)"); break;
			case C_ART_nstr: ss << _T("(array)"); break;
			case C_ART_nint: ss << _T("(array)"); break;
			case C_ART_nflo: ss << _T("(array)"); break;
			case C_ART_ndbl: ss << _T("(array)"); break;
			case C_ART_ncpx: ss << _T("(array)"); break;
			case C_ART_ntri: ss << _T("(array)"); break;
			case C_ART_nmat: ss << _T("(array)"); break;
			case C_ART_nmatx: ss << _T("(array)"); break;
			case C_ART_undef: ss << _T("(undef)"); break;
		}

		ss << ((IsFlagSet(IToken::flVOLATILE)) ? _T("; ") : _T("; not ")) << _T("volatile");
		ss << _T("]");

		return ss.str();
	}

	//---------------------------------------------------------------------------
	void ARTValue::Release()
	{
		if (m_pCache)
			std::cout << "ReleaseToCache not implemented\n";
			//m_pCache->ReleaseToCache(this);
		else
			delete this;
	}

	//---------------------------------------------------------------------------
	void ARTValue::BindToCache(ValueCache *pCache)
	{
		m_pCache = pCache;
	}
}	// namespace mu
