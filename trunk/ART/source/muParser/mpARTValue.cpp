#ifdef __XXX___
#error "THIS FILE SHOULD NOT BE USED IN ART!"

#include "mpARTValue.h"
#include "mpError.h"
#include "mpValueCache.h"
#include "mpValue.h"
#include "DataContainer.h"

//#define _DBG_MSG(a) {cout << __FILE__ << "::" <<__func__ << "("<< a << ")" << endl;}
#define _DBG_MSG(a)

MUP_NAMESPACE_START

	//------------------------------------------------------------------------------
	ARTValue::ARTValue()
		:IValue(cmVAL)
		,arrayVals(NULL)
		,m_iFlags(flNONE)
		,m_pCache(NULL)
	{
		var = new DataContainer(0.0);
		own = true;
		_DBG_MSG("");
	}

	//---------------------------------------------------------------------------
	ARTValue::ARTValue(int_type a_iVal)
		:IValue(cmVAL)
		,arrayVals(NULL)
		,m_iFlags(flNONE)
		,m_pCache(NULL)
	{
		var = new DataContainer(a_iVal);
		own = true;
		_DBG_MSG("int_type");
	}

	//---------------------------------------------------------------------------
	ARTValue::ARTValue(bool_type a_bVal)
		:IValue(cmVAL)
		,arrayVals(NULL)
		,m_iFlags(flNONE)
		,m_pCache(NULL)
	{
		//we don't have a boolean type...
		var = new DataContainer((int)(a_bVal));
		own = true;
		_DBG_MSG("bool_type");
	}

	//---------------------------------------------------------------------------
	ARTValue::ARTValue(string_type a_sVal)
		:IValue(cmVAL)
		,arrayVals(NULL)
		,m_iFlags(flNONE)
		,m_pCache(NULL)
	{
		var = new DataContainer(a_sVal.c_str());
		own = true;
		_DBG_MSG("string_type");
	}

	//---------------------------------------------------------------------------
	ARTValue::ARTValue(int_type array_size, float_type val, T_ART_Type type)
		:IValue(cmVAL)
		,arrayVals(NULL)
		,m_iFlags(flNONE)
		,m_pCache(NULL)
	{
		var = new DataContainer(type);
		var->SetArrayLength(array_size);
		int i;
		switch (type)
		{
			case C_ART_ndbl:
				for (i = 0; i < array_size; i++)
					var->val->nd[i] = static_cast<double>(val);
				own = true;
				break;
			case C_ART_nflo:
				for (i = 0; i < array_size; i++)
					var->val->nf[i] = static_cast<float>(val);
				own = true;
				break;
			case C_ART_nint:
				for (i = 0; i < array_size; i++)
					var->val->ni[i] = static_cast<int>(val);
				own = true;
				break;
			case C_ART_ncpx:
				for (i = 0; i < array_size; i++)
				{
					var->val->nc[i].re = static_cast<double>(val);
					var->val->nc[i].im = 0;
				}
				own = true;
				break;
			case C_ART_na:
				// create new DataContainer array
				var->val->na = new DataContainer[array_size];
				// create new ARTValue array, containing references
				// to DataContainer values
				arrayVals = new ARTValue[array_size];
				for (i = 0; i < array_size; i++)
				{
					// set and initialize all elements
					var->val->na[i].SetType(C_ART_dbl);
					var->val->na[i].SetVal(static_cast<double>(val));
					// remove local DataContainer object
					arrayVals[i].deleteVar();
					// set reference of ARTValue to DataContainer
					arrayVals[i].var = dynamic_cast<DataContainer*>(&(var->val->na[i]));
				}
				own = true;
				break;
			default:
				throw ParserError();

		}
		_DBG_MSG("int_type, float, T_ART_TYPE");
	}
/* brauchma net */
	//---------------------------------------------------------------------------
	ARTValue::ARTValue(int_type m, int_type n, float_type val)
		:IValue(cmVAL)
		,arrayVals(NULL)
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
		_DBG_MSG("int_type, int_type, float_type")
	}
	//---------------------------------------------------------------------------
	ARTValue::ARTValue(const char_type *a_szVal)
		:IValue(cmVAL)
		,arrayVals(NULL)
		,m_iFlags(flNONE)
		,m_pCache(NULL)
	{
		var = new DataContainer(a_szVal);
		own = true;
		_DBG_MSG("const char_type*");
	}

	//---------------------------------------------------------------------------
	ARTValue::ARTValue(const cmplx_type &v)
		:IValue(cmVAL)
		,arrayVals(NULL)
		,m_iFlags(flNONE)
		,m_pCache(NULL)
	{
		var = new DataContainer(C_ART_cpx);
		var->val->c.re = v.real();
		var->val->c.im =v.imag();
		own = true;
		_DBG_MSG("const cmplx_type&");
	}

	//---------------------------------------------------------------------------
	ARTValue::ARTValue(float_type val)
		:IValue(cmVAL)
		,arrayVals(NULL)
		,m_iFlags(flNONE)
		,m_pCache(NULL)
	{
		var = new DataContainer(val);
		own = true;
		_DBG_MSG("float_type");
	}
/**/
	//---------------------------------------------------------------------------
	ARTValue::ARTValue(const array_type &val)
		:IValue(cmVAL)
		,arrayVals(NULL)
/*		,m_val()
		,m_psVal(NULL)
		,m_pvVal(new array_type(val))
		,m_cType('a') */
		,m_iFlags(flNONE)
		,m_pCache(NULL)
	{
		std::cout << "ARTValue::ARTValue(const array_type &val) not implemented\n";
		_DBG_MSG("array_type&");
	}

	ARTValue::ARTValue(DataContainer* av)
		:IValue(cmVAL)
		,arrayVals(NULL)
		,m_iFlags(flNONE)
		,m_pCache(NULL)
	{
		var = av;
		own = false; //var is connected to foreign DataContainer, so var must not be destroyed by this object
		// if data container is of type array, we have to initialize the local value array
		if (var->typ == C_ART_na)
		{
			arrayVals = new ARTValue[var->len];
			DataContainer* arrayElements = dynamic_cast<DataContainer*>(var->val->na);
			for (int i = 0; i < var->len; ++i)
			{
				// delete standard var object
				arrayVals[i].deleteVar();
				// set reference to array element of the parent
				// data container
				arrayVals[i].var = &(arrayElements[i]);
			}
		}
		_DBG_MSG("DataContainer*");
	}

	//---------------------------------------------------------------------------
	ARTValue::ARTValue(const ARTValue &a_Val)
		:IValue(cmVAL)
		,arrayVals(NULL)
		,m_pCache(NULL)
	{
		Assign(a_Val);
		_DBG_MSG("const ARTValue&");
	}

	//---------------------------------------------------------------------------
	ARTValue::ARTValue(const IValue &a_Val)
		:IValue(cmVAL)
		,arrayVals(NULL)
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
		_DBG_MSG("const IValue&");
	}

	//---------------------------------------------------------------------------
	ARTValue& ARTValue::operator=(const ARTValue &a_Val)
	{
		Assign(a_Val);
		_DBG_MSG("const ARTValue&");
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
		switch (var->typ)
		{
			case C_ART_na:
				return arrayVals[i % (var->len)];
				break;
			default:
				throw ParserError();
				break;
		}
		//std::cout << "IValue& ARTValue::operator[](std::size_t i) not implemented\n";
		_DBG_MSG("std::size_t");
		return *this;
	}

	//---------------------------------------------------------------------------
	ARTValue::~ARTValue()
	{
		// if we are saving an array, we have to delete all DataContainers
		// we have to free the inner data containers before we can delete the
		// variable
		if (arrayVals != NULL)
		{
			delete[] arrayVals;
			if (own)
			{
				delete[] dynamic_cast<DataContainer*>(var->val->na);
			}
		}
		if (own)
		{
			delete var;
			own = false;
		}
		_DBG_MSG("");
	}

	//---------------------------------------------------------------------------
	IToken* ARTValue::Clone() const
	{
		_DBG_MSG("");
		return new ARTValue(*this);
	}

	//---------------------------------------------------------------------------
	Value* ARTValue::AsValue()
	{
		_DBG_MSG("");
		return new Value(this);
	}

	//---------------------------------------------------------------------------
	IValue* ARTValue::AsIValue()
	{
		_DBG_MSG("");
		return this;
	}

	//---------------------------------------------------------------------------
	void ARTValue::Assign(const ARTValue &ref)
	{
		if (this==&ref)
			return;

		// delete variable first
		deleteVar();

		//copy value
		var = new DataContainer(*ref.var);
		own = true;

		m_iFlags = ref.m_iFlags;

		_DBG_MSG("const ARTValue&");
		// Do not copy the ARTValue cache pointer!
		// ARTValue cache should be assigned expplicitely and
		// not implicitely (i.e. when retrieving the final result.)
		//m_pCache = ref.m_pCache;
	}

	//---------------------------------------------------------------------------
	void ARTValue::deleteVar()
	{
		if (arrayVals != NULL)
		{
			delete[] arrayVals;
			if (own)
			{
				delete[] var->val->na;
			}
		}
		if (own)
		{
			delete var;
		}
		var = NULL;
		own = false;
		_DBG_MSG("");
	}

	//---------------------------------------------------------------------------
	void ARTValue::Reset()
	{
		deleteVar();
		var = new DataContainer();
		own = true;

		m_iFlags = flNONE;
		_DBG_MSG("");
	}

	//---------------------------------------------------------------------------
	IValue& ARTValue::operator=(bool val) //should I even care about booleans?
	{
		var->SetVal(val);
		_DBG_MSG("bool");
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
		_DBG_MSG("int_type");
		return *this;
	}

	//---------------------------------------------------------------------------
	IValue& ARTValue::operator=(float_type val)
	{
		var->SetVal(val);
		_DBG_MSG("float_type");
		return *this;
	}

	//---------------------------------------------------------------------------
	IValue& ARTValue::operator=(string_type a_sVal)
	{
		var->SetVal(a_sVal.c_str());
		_DBG_MSG("string_type");
		return *this;
	}

	//---------------------------------------------------------------------------
	IValue& ARTValue::operator=(const char_type *a_szVal)
	{
		var->SetVal(a_szVal);
		_DBG_MSG("const char_type*");
		return *this;
	}

	//---------------------------------------------------------------------------
	IValue& ARTValue::operator=(const array_type &a_vVal)
	{
		_DBG_MSG("const array_type&");
		return *this;
	}

	//---------------------------------------------------------------------------
	IValue& ARTValue::operator=(const cmplx_type &val)
	{
		var->SetVal(val);
		_DBG_MSG("const cmplx_type&");
		return *this;
	}

	//---------------------------------------------------------------------------
	char_type ARTValue::GetType() const
	{
		_DBG_MSG("");
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
			case C_ART_na: return 'a';
			case C_ART_undef: return ' ';  
		}
	}

	//---------------------------------------------------------------------------
	float_type ARTValue::GetFloat() const
	{
		_DBG_MSG("");

		DataContainer* tmp = dynamic_cast<DataContainer*>(var);
		//cout << "in GetFloat()" << endl;
		return static_cast<float_type>(tmp->GetValueAsDouble());
		/*if (tmp != NULL)
		{
			cout << "erfolgreich nach artdatacontainer gecastet!" << endl;
		}
		else
		{
			cout << "konnte nicht nach artdatacontainer casten!" << endl;
		}



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

				case C_ART_na:
					if ((var->len > 1) || (var->val->na->typ != C_ART_dbl)) throw ParserError();
					else return var->val->na->val->d;
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
			
		} */

		/* if (!IsScalar() && m_cType!='b')
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

		return m_val.real(); */
	}

	//---------------------------------------------------------------------------
	float_type ARTValue::GetImag() const
	{
		_DBG_MSG("");
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

				case C_ART_na:
					if (var->len > 1) throw ParserError();
					else
					{
						DataContainer* tmp;
						tmp = dynamic_cast<DataContainer*>(var);
						return tmp->GetValueAsDouble();
					}

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
		_DBG_MSG("");
//		return m_val;
		static cmplx_type tmp = cmplx_type(1,0);
		return tmp;
	}

	//---------------------------------------------------------------------------
	const string_type& ARTValue::GetString() const
	{
		_DBG_MSG("");
/*		CheckType('s');
		assert(m_psVal!=NULL);
		return *m_psVal;*/
		static string tmp = "test";
		return tmp;

	}

	//---------------------------------------------------------------------------
	bool ARTValue::GetBool() const
	{
		_DBG_MSG("");
/*		CheckType('b');
		return m_val.real()==1;*/
		return false;
	}

	//---------------------------------------------------------------------------
	const array_type& ARTValue::GetArray() const
	{
		_DBG_MSG("");
		/*CheckType('a');
		assert(m_pvVal!=NULL);
		return *m_pvVal;*/
		if (var->typ == C_ART_na)
		{
			static array_type testArray = array_type(var->len);
			for (int i = 0; i < var->len; ++i)
			{
				testArray[i] = &(arrayVals[i]);
			}
			return testArray;
		}
		else
		{
			//static std::vector<class mup::IValue*> dummy;
			static array_type dummy;
			return dummy;
		}
	}

	//---------------------------------------------------------------------------
	void ARTValue::CheckType(char_type a_cType) const
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
	bool ARTValue::IsVolatile() const
	{
		_DBG_MSG("");
		return IsFlagSet(IValue::flVOLATILE);
//		return true;
	}

	//---------------------------------------------------------------------------
	string_type ARTValue::AsciiDump() const
	{
		_DBG_MSG("");
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
			case C_ART_na: ss << _T("(array)"); break;
			case C_ART_undef: ss << _T("(undef)"); break;
		}

		ss << ((IsFlagSet(IToken::flVOLATILE)) ? _T("; ") : _T("; not ")) << _T("volatile");
		ss << _T("]");

		return ss.str();
	}

	//---------------------------------------------------------------------------
	void ARTValue::Release()
	{
		_DBG_MSG("");
		if (m_pCache)
			std::cout << "ReleaseToCache not implemented\n";
			//m_pCache->ReleaseToCache(this);
		else
			delete this;
	}

	//---------------------------------------------------------------------------
	void ARTValue::BindToCache(ValueCache *pCache)
	{
		_DBG_MSG("");
		m_pCache = pCache;
	}
}	// namespace mu

#endif
