/*
<pre>
               __________                                 ____  ___
    _____  __ _\______   \_____ _______  ______ __________\   \/  /
   /     \|  |  \     ___/\__  \\_  __ \/  ___// __ \_  __ \     / 
  |  Y Y  \  |  /    |     / __ \|  | \/\___ \\  ___/|  | \/     \ 
  |__|_|  /____/|____|    (____  /__|  /____  >\___  >__| /___/\  \
        \/                     \/           \/     \/           \_/

  muParserX - A C++ math parser library with array and string support
  Copyright 2010 Ingo Berg

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU LESSER GENERAL PUBLIC LICENSE
  as published by the Free Software Foundation, either version 3 of 
  the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program.  If not, see http://www.gnu.org/licenses.
</pre>
*/
#include "mpValue.h"
#include "mpError.h"
#include "mpValueCache.h"

//#define _DBG_MSG(a) {std::cerr << __FILE__ << "::" <<__func__ << "("<< a << ")" << std::endl;}
#define _DBG_MSG(a) {}

MUP_NAMESPACE_START

  //------------------------------------------------------------------------------
  Value::Value()
    :IValue(cmVAL)
    ,m_val(0,0)
    ,m_psVal(NULL)
    ,m_pvVal(NULL)
    ,m_cType('f')
    ,m_iFlags(flNONE)
    ,m_pCache(NULL)
  {}

  //---------------------------------------------------------------------------
  Value::Value(int_type a_iVal)
    :IValue(cmVAL)
    ,m_val((float_type)a_iVal, 0)
    ,m_psVal(NULL)
    ,m_pvVal(NULL)
    ,m_cType('i')
    ,m_iFlags(flNONE)
    ,m_pCache(NULL)
  {}

  //---------------------------------------------------------------------------
  Value::Value(bool_type a_bVal)
    :IValue(cmVAL)
    ,m_val((float_type)a_bVal, 0)
    ,m_psVal(NULL)
    ,m_pvVal(NULL)
    ,m_cType('b')
    ,m_iFlags(flNONE)
    ,m_pCache(NULL)
  {}

  //---------------------------------------------------------------------------
  Value::Value(string_type a_sVal)
    :IValue(cmVAL)
    ,m_val()
    ,m_psVal(new string_type(a_sVal))
    ,m_pvVal(NULL)
    ,m_cType('s')
    ,m_iFlags(flNONE)
    ,m_pCache(NULL)
  {}

  //---------------------------------------------------------------------------
  Value::Value(int_type array_size, float_type val)
    :IValue(cmVAL)
    ,m_val()
    ,m_psVal(NULL)
    ,m_pvVal(new array_type(array_size))
    ,m_cType('a')
    ,m_iFlags(flNONE)
    ,m_pCache(NULL)
  {
      _DBG_MSG("int_type, float_type")
	  //m_pvVal = new array_type(array_size);
	  for (int_type i = 0; i < array_size; ++i)
	  {
		  (*m_pvVal)[i] = new Value(val);
	  }
  }

  //---------------------------------------------------------------------------
  Value::Value(int_type m, int_type n, float_type val)
    :IValue(cmVAL)
    ,m_val()
    ,m_psVal(NULL)
    ,m_pvVal(new array_type(n))
    ,m_cType('a')
    ,m_iFlags(flNONE)
    ,m_pCache(NULL)
  {
    _DBG_MSG("int_type, int_type, float_type");
    // m - spalten
    // n - reihen
    //m_pvVal = new array_type(n);
    for (int_type i=0; i<n; ++i)
    {
      (*m_pvVal)[i] = new Value(m, val);
    }
  }

  //---------------------------------------------------------------------------
  Value::Value(const char_type *a_szVal)
    :IValue(cmVAL)
    ,m_val()
    ,m_psVal(new string_type(a_szVal))
    ,m_pvVal(NULL)
    ,m_cType('s')
    ,m_iFlags(flNONE)
    ,m_pCache(NULL)
  {}

  //---------------------------------------------------------------------------
  Value::Value(const cmplx_type &v)
    :IValue(cmVAL)
    ,m_val(v)
    ,m_psVal(NULL)
    ,m_pvVal(NULL)
    ,m_cType('c')
    ,m_iFlags(flNONE)
    ,m_pCache(NULL)
  {
    _DBG_MSG("cmplx_type");
    if ( (m_val.real()==(int_type)m_val.real()) && (m_val.imag()==0) )
      m_cType = 'i';
    else
      m_cType = (m_val.imag()==0) ? 'f' : 'c';
  }

  //---------------------------------------------------------------------------
  Value::Value(float_type val)
    :IValue(cmVAL)
    ,m_val(val, 0)
    ,m_psVal(NULL)
    ,m_pvVal(NULL)
    ,m_cType((val==(int_type)val) ? 'i' : 'f')
    ,m_iFlags(flNONE)
    ,m_pCache(NULL)
  {}

  //---------------------------------------------------------------------------
  Value::Value(const array_type &val)
    :IValue(cmVAL)
    ,m_val()
    ,m_psVal(NULL)
    ,m_pvVal(new array_type(val.size()))
    ,m_cType('a')
    ,m_iFlags(flNONE)
    ,m_pCache(NULL)
  {
    for (int_type i = 0; i < val.size(); ++i)
    {
      Value* tmp = dynamic_cast<Value*>(val[i]);
      (*m_pvVal)[i] = new Value(*tmp);
    }
  }

  //---------------------------------------------------------------------------
  Value::Value(const Value &a_Val)
    :IValue(cmVAL)
    ,m_psVal(NULL)
    ,m_pvVal(NULL)
    ,m_pCache(NULL)
  {
    _DBG_MSG("const Value&");
    Assign(a_Val);
  }

  //---------------------------------------------------------------------------
  Value::Value(const IValue &a_Val)
    :IValue(cmVAL)
    ,m_psVal(NULL)
    ,m_pvVal(NULL)
    ,m_pCache(NULL)
  {
    _DBG_MSG("const IValue&");
    Reset();
    const array_type& tmpArray = (a_Val.GetType() == 'a') ? a_Val.GetArray() : array_type(0);
	int_type i;

    switch(a_Val.GetType())
    {
    case 'i': 
    case 'f': 
    case 'b': m_val = cmplx_type(a_Val.GetFloat(), 0);
              break;


    case 'c': m_val = cmplx_type(a_Val.GetFloat(), a_Val.GetImag());
              break;

    case 's': //if (!m_psVal)
                m_psVal = new string_type(a_Val.GetString());
              /*else
               *m_psVal = a_Val.GetString();*/
              break;

    case 'a':// if (!m_pvVal)
                //m_pvVal = new array_type(a_Val.GetArray());
                m_pvVal = new array_type(tmpArray.size());

                for (i = 0; i < tmpArray.size(); ++i)
                {
                  Value* tmp = dynamic_cast<Value*>(tmpArray[i]);
                  (*m_pvVal)[i] = new Value(*tmp);
                }
              /*else
               *m_pvVal  = a_Val.GetArray();  */
              break;

    case 'v': break;
    default:  MUP_FAIL(INVALID_TYPE_CODE);
    }
    
    m_cType = a_Val.GetType();
  }

  //---------------------------------------------------------------------------
  Value& Value::operator=(const Value &a_Val)
  {
    _DBG_MSG("const Value&");
    Assign(a_Val);
    return *this;
  }

  //---------------------------------------------------------------------------
  IValue& Value::operator[](std::size_t i)
  {
    _DBG_MSG("std::size_t");
    if (m_cType!='a' || m_pvVal==NULL)
      throw ParserError( ErrorContext(ecAPI_NOT_AN_ARRAY) ); 

    if (i>=m_pvVal->size())
      throw ParserError( ErrorContext(ecINDEX_OUT_OF_BOUNDS) ); 

    return *((*m_pvVal)[i]);
  }

  //---------------------------------------------------------------------------
  Value::~Value()
  {
    _DBG_MSG("");
	Reset();
	/*
    delete m_psVal;
    delete m_pvVal;
    */
  }

  //---------------------------------------------------------------------------
  IToken* Value::Clone() const
  {
    _DBG_MSG("")
    return new Value(*this);
  }

  //---------------------------------------------------------------------------
  Value* Value::AsValue()
  {
    _DBG_MSG("")
    return this;
  }

  //---------------------------------------------------------------------------
  void Value::Assign(const Value &ref)
  {
    _DBG_MSG("const Value&")
    if (this==&ref)
      return;

    Reset();

    m_val    = ref.m_val;

    // allocate room for a string
    if (ref.m_psVal)
    {
	  m_psVal = new string_type(*ref.m_psVal);
	  /*
      if (!m_psVal)
        m_psVal = new string_type(*ref.m_psVal);
      else
       *m_psVal = *ref.m_psVal;

    }
    else
    {
      delete m_psVal;
      m_psVal = NULL;
      */
    }

    // allocate room for a vector
    if (ref.m_pvVal)
    {
	  m_pvVal = new array_type(ref.m_pvVal->size());
      for (int_type i = 0; i < ref.m_pvVal->size(); ++i)
      {
        (*m_pvVal)[i] = new Value(*((*(ref.m_pvVal))[i]));
      }
      /*
      if (m_pvVal==NULL)
        m_pvVal = new array_type(*ref.m_pvVal);
      else
       *m_pvVal = *ref.m_pvVal;
    }
    else
    {
      delete m_pvVal;
      m_pvVal = NULL;
      */
    }

    m_cType  = ref.m_cType;
    m_iFlags = ref.m_iFlags;

    // Do not copy the value cache pointer!
    // Value cache should be assigned explicitely and
    // not implicitely (i.e. when retrieving the final result.)
    //m_pCache = ref.m_pCache;
  }

  //---------------------------------------------------------------------------
  void Value::Reset()
  {
    _DBG_MSG("");
    m_val = cmplx_type(0,0);

    delete m_psVal;
    m_psVal = NULL;

    if (m_pvVal != NULL)
    {
      for (int_type i = 0; i < m_pvVal->size(); ++i)
      {
        delete dynamic_cast<Value*>((*m_pvVal)[i]);
      }
      delete m_pvVal;
      m_pvVal = NULL;
    }

    m_cType = 'f';
    m_iFlags = flNONE;
  }

  //---------------------------------------------------------------------------
  IValue& Value::operator=(bool val)
  {
    _DBG_MSG("bool");
	Reset();
    m_val = cmplx_type((float_type)val,0);

    /*
    delete m_psVal;
    m_psVal = NULL;

    delete m_pvVal;
    m_pvVal = NULL;
    */
    m_cType = 'b';
    m_iFlags = flNONE;

    return *this;
  }

  //---------------------------------------------------------------------------
  IValue& Value::operator=(int_type a_iVal)
  {
    _DBG_MSG("int_type");
	Reset();
    m_val = cmplx_type(a_iVal,0);

    /*
    delete m_psVal;
    m_psVal = NULL;

    delete m_pvVal;
    m_pvVal = NULL;
    */
    m_cType = 'i';
    m_iFlags = flNONE;

    return *this;
  }

  //---------------------------------------------------------------------------
  IValue& Value::operator=(float_type val)
  {
    _DBG_MSG("float_type");
	Reset();
    m_val = cmplx_type(val, 0);
    /*
    delete m_psVal;
    m_psVal = NULL;

    delete m_pvVal;
    m_pvVal = NULL;
    */
    m_cType = (val==(int_type)val) ? 'i' : 'f';
    m_iFlags = flNONE;

    return *this;
  }

  //---------------------------------------------------------------------------
  IValue& Value::operator=(string_type a_sVal)
  {
    _DBG_MSG("string_type");
	Reset();
    m_val = cmplx_type();
    m_psVal = new string_type(a_sVal);
    /*
    if (!m_psVal)
      m_psVal = new string_type(a_sVal);
    else
     *m_psVal = a_sVal;

    delete m_pvVal;
    m_pvVal = NULL;
	*/
    m_cType = 's';
    m_iFlags = flNONE;
    return *this;
  }

  //---------------------------------------------------------------------------
  IValue& Value::operator=(const char_type *a_szVal)
  {
    _DBG_MSG("const char_type*");
	Reset();
    m_val = cmplx_type();
    m_psVal = new string_type(a_szVal);
    /*
    if (!m_psVal)
      m_psVal = new string_type(a_szVal);
    else
     *m_psVal = a_szVal;

    delete m_pvVal;
    m_pvVal = NULL;
	*/
    m_cType = 's';
    m_iFlags = flNONE;
    return *this;
  }

  //---------------------------------------------------------------------------
  IValue& Value::operator=(const array_type &a_vVal)
  {
    _DBG_MSG("const array_type&");
	Reset();
    m_val = cmplx_type(0,0);

    m_pvVal = new array_type(a_vVal.size());
    for (int_type i = 0; i < a_vVal.size(); ++i)
    {
    	(*m_pvVal)[i] = new Value(*(a_vVal[i]));
    }

    /*
    delete m_psVal;
    m_psVal = NULL;
		
    if (m_pvVal==NULL)
      m_pvVal = new array_type(a_vVal);
    else
      *m_pvVal = a_vVal;
    */
    m_cType = 'a';
    m_iFlags = flNONE;
    return *this;
  }

  //---------------------------------------------------------------------------
  IValue& Value::operator=(const cmplx_type &val)
  {
    _DBG_MSG("const cmplx_type&");
	Reset();
    m_val = val;
    /*
    delete m_psVal;
    m_psVal = NULL;

    delete m_pvVal;
    m_pvVal = NULL;
	*/
    m_cType = (m_val.imag()==0) ? ( (m_val.real()==(int)m_val.real()) ? 'i' : 'f' ) : 'c';
    m_iFlags = flNONE;
    return *this;
  }

  //---------------------------------------------------------------------------
  char_type Value::GetType() const
  {
    _DBG_MSG("");
    return m_cType;
  }

  //---------------------------------------------------------------------------
  float_type Value::GetFloat() const
  {
    _DBG_MSG("");
    if (!IsScalar() && m_cType!='b')
    {
      ErrorContext err;
      err.Errc  = ecTYPE_CONFLICT;
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

  //---------------------------------------------------------------------------
  float_type Value::GetImag() const
  {
    _DBG_MSG("");
    if (!IsScalar())
    {
      ErrorContext err;
      err.Errc  = ecTYPE_CONFLICT;
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

    return m_val.imag();
  }

  //---------------------------------------------------------------------------
  const cmplx_type& Value::GetComplex() const
  {
    _DBG_MSG("");
    return m_val;
  }

  //---------------------------------------------------------------------------
  const string_type& Value::GetString() const
  {
    _DBG_MSG("");
    CheckType('s');
    assert(m_psVal!=NULL);
    return *m_psVal;
  }

  //---------------------------------------------------------------------------
  bool Value::GetBool() const
  {
    _DBG_MSG("");
    CheckType('b');
    return m_val.real()==1;
  }

  //---------------------------------------------------------------------------
  const array_type& Value::GetArray() const
  {
    _DBG_MSG("");
    CheckType('a');
    assert(m_pvVal!=NULL);
    return *m_pvVal;
  }

  //---------------------------------------------------------------------------
  void Value::CheckType(char_type a_cType) const
  {
    _DBG_MSG("char_type");
    if (m_cType!=a_cType)
    {
      ErrorContext err;
      err.Errc  = ecTYPE_CONFLICT;
      err.Type1 = m_cType;
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
  bool Value::IsVolatile() const
  {
    _DBG_MSG("");
    return IsFlagSet(IValue::flVOLATILE);
//    return true;
  }

  //---------------------------------------------------------------------------
  string_type Value::AsciiDump() const
  {
    _DBG_MSG("");
    stringstream_type ss;

    ss << g_sCmdCode[ GetCode() ];
    ss << _T(" [addr=0x") << std::hex << this << std::dec;
    ss << _T("; type=\"") << GetType() << _T("\"");
    ss << _T("; val=");

    switch(m_cType)
    {
    case 'i': ss << (int_type)m_val.real(); break;
    case 'f': ss << m_val.real(); break;
    case 'a': ss << _T("(array)"); break;
    case 's': 
              assert(m_psVal!=NULL);
              ss << _T("\"") << m_psVal << _T("\""); break;
    }

    ss << ((IsFlagSet(IToken::flVOLATILE)) ? _T("; ") : _T("; not ")) << _T("volatile");
    ss << _T("]");

    return ss.str();
  }

  //---------------------------------------------------------------------------
  void Value::Release()
  {
    _DBG_MSG("");
    if (m_pCache)
      m_pCache->ReleaseToCache(this);
    else
      delete this;
  }

  //---------------------------------------------------------------------------
  void Value::BindToCache(ValueCache *pCache)
  {
    _DBG_MSG("ValueCache*");
    m_pCache = pCache;
  }
}  // namespace mu
