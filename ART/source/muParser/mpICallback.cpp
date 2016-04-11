/** \file
    \brief Implementation of the interface for parser callback objects.

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
#include "mpICallback.h"
#include <cassert>

#include "mpParserBase.h"


MUP_NAMESPACE_START

  //------------------------------------------------------------------------------
  ICallback::ICallback(ECmdCode a_iCode, 
                       const char_type *a_szName, 
                       const char_type *a_szArgDef,
                       const IPackage *a_pPackage)
    :IToken(a_iCode, a_szName)
    ,m_pParent(NULL)
    ,m_pPackage(a_pPackage)
    ,m_sArgTypes()
    ,m_cRetType()
    ,m_nArgc(-1)
    ,m_nArgsPresent(-1)
  {
    CheckPrototype(a_szArgDef);
  }

  //------------------------------------------------------------------------------
  ICallback::~ICallback()
  {}

  //---------------------------------------------------------------------------
  ICallback* ICallback::AsICallback()
  {
    return this;
  }

  //---------------------------------------------------------------------------
  IValue* ICallback::AsIValue()
  {
    return NULL;
  }

  //------------------------------------------------------------------------------
  bool ICallback::IsVolatile() const
  {
    return false;
  }

  //------------------------------------------------------------------------------
  /** \brief Returns a pointer to the parser object owning this callback. */
  ParserXBase* ICallback::GetParent()
  {
    assert(m_pParent);
    return m_pParent;
  }

  //------------------------------------------------------------------------------
  /** \brief Returns the m´number of arguments required by this callback. 
      \return Number of arguments or -1 if the number of arguments is variable.  
  */
  int ICallback::GetArgc() const
  {
    return m_nArgc;
  }

  //------------------------------------------------------------------------------
  /** \brief Return the type of a certain function argument. 
  
      \param a_iIdx number of Argument to return the type of. Must be greater 
             than zero and less or equal than #m_nArgc.
  */
  char_type ICallback::GetArgType(int a_iIdx) const
  {
    if (m_nArgc==-1)
      return '.';
    else
    {
      MUP_ASSERT(a_iIdx>=0 && a_iIdx<=m_nArgc);
      return m_sArgTypes[a_iIdx-1];
    }
  }

  //------------------------------------------------------------------------------
  char_type ICallback::GetRetType() const
  {
    return m_cRetType;
  }

  //------------------------------------------------------------------------------
  /** \brief Return the function object prototype. 
  
    For Debugging purposes only.
  */
  string_type ICallback::GetArgDef() const
  {
    stringstream_type ss;
    ss << m_cRetType << _T(":") << m_sArgTypes;
    return ss.str().c_str();
  }

  //------------------------------------------------------------------------------
  /** \brief Assign a parser object to the callback.
      \param a_pParent The parser that belongs to this callback object.

    The parent object can be used in order to access internals of the parser
    from within a callback object. Thus enabling callbacks to delete 
    variables or functions if this is desired.
  */
  void ICallback::SetParent(parent_type *a_pParent)
  {
    assert(a_pParent);
    m_pParent = a_pParent;
  }

  //------------------------------------------------------------------------------
  string_type ICallback::AsciiDump() const
  {
    stringstream_type ss;

    ss << g_sCmdCode[ GetCode() ];
    ss << _T(" [addr=0x") << std::hex << this << std::dec;
    ss << _T("; ident=\"") << GetIdent() << "\"";
    ss << _T("; argc=") << GetArgc() << " (present: " << m_nArgsPresent << ")";
    ss << _T("; prot=") << GetArgDef();
    ss << _T("]");

    return ss.str();
  }

  //------------------------------------------------------------------------------
  void ICallback::SetNumArgsPresent(int argc)
  {
    m_nArgsPresent = argc;
  }

  //------------------------------------------------------------------------------
  int ICallback::GetArgsPresent() const
  {
    if (m_nArgc!=-1)
      return m_nArgc;
    else
      return m_nArgsPresent;
  }

  //------------------------------------------------------------------------------
  /** \brief Verify the prototype supplied to the callback object.
      \param a_sProt String containing the prototype definition
  */
  void ICallback::CheckPrototype(const string_type &a_sProt)
  {
    std::size_t iEnd(0);
    if (a_sProt[1]==':' && a_sProt.length()>=3)
    {
      string_type sParam(&a_sProt[2]);

      // The prototype must consist entirely of characters defined in g_sTypeShortCut.
      // check return value
      iEnd = a_sProt.find_first_not_of(g_sTypeShortCut, 0);
      if (iEnd!=1 || a_sProt[0]=='.')
        throw ParserError( ErrorContext(ecAPI_INVALID_PROTOTYPE, -1, GetIdent() ) ); 

      // check parameters
      iEnd = sParam.find_first_not_of(g_sTypeShortCut, 2);
      if (iEnd!=string_type::npos)
        throw ParserError( ErrorContext(ecAPI_INVALID_PROTOTYPE, -1, GetIdent() ) ); 

      // If there is a void listed as a parameter there may not be other type
      // identifier present
      if (sParam.find('v')!=string_type::npos && sParam.length()!=1)
        throw ParserError( ErrorContext(ecAPI_INVALID_PROTOTYPE, -1, GetIdent() ) ); 


      // Prototype is now verified, finally copy the prototype data
      m_sArgTypes = &a_sProt[2];  // from 2nd character on are argument types 
      m_cRetType = a_sProt[0];    // first  character specifies the return type
      m_nArgc = (sParam[0]=='*') ? -1 : (m_sArgTypes[0]=='v') ? 0 : sParam.length();
    }
    else
      throw ParserError( ErrorContext(ecAPI_INVALID_PROTOTYPE, -1, GetIdent() ) ); 
  }
} // namespace mu
