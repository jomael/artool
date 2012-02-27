/** \file
    \brief Implementation of the token reader used to break the expression string up 
           into tokens.

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

#include "mpTokenReader.h"

#include <cassert>

#include "mpParserBase.h"
#include "mpIValReader.h"
#include "mpOverload.h"
#include "mpIfThenElse.h"


MUP_NAMESPACE_START

  //---------------------------------------------------------------------------
  /** \brief Copy constructor.
      \sa Assign
      \throw nothrow
  */
  TokenReader::TokenReader(const TokenReader &a_Reader) 
  { 
    Assign(a_Reader);
  }
    
  //---------------------------------------------------------------------------
  /** \brief Assignement operator.
      \param a_Reader Object to copy to this token reader.
      \throw nothrow
    
      Self assignement will be suppressed otherwise #Assign is called.
  */
  TokenReader& TokenReader::operator=(const TokenReader &a_Reader) 
  {
    if (&a_Reader!=this)
      Assign(a_Reader);

    return *this;
  }

  //---------------------------------------------------------------------------
  /** \brief Assign state of a token reader to this token reader. 
      \param a_Reader Object from which the state should be copied.
      \throw nothrow
  */
  void TokenReader::Assign(const TokenReader &obj)
  {
    m_pParser             = obj.m_pParser;
    m_sExpr            = obj.m_sExpr;
    m_iPos                = obj.m_iPos;
    m_iNumBra             = obj.m_iNumBra;
    m_iSynFlags           = obj.m_iSynFlags;
    m_UsedVar             = obj.m_UsedVar;
    m_pVarDef             = obj.m_pVarDef;
    m_pPostOprtDef        = obj.m_pPostOprtDef;
    m_pInfixOprtDef       = obj.m_pInfixOprtDef;
    m_pOprtDef            = obj.m_pOprtDef;
    m_pFunDef             = obj.m_pFunDef;
    m_pConstDef           = obj.m_pConstDef;
    m_pDynVarShadowValues = obj.m_pDynVarShadowValues;
    m_vTokens             = obj.m_vTokens;

    // Reader klassen klonen 
    DeleteValReader();
    std::size_t i, iSize = obj.m_vValueReader.size();
    for (i=0; i<iSize; ++i)
    {
      m_vValueReader.push_back(obj.m_vValueReader[i]->Clone(this));
    }
  }

  //---------------------------------------------------------------------------
  /** \brief Constructor. 
        
      Create a Token reader and bind it to a parser object. 

      \pre [assert] a_pParser may not be NULL
      \post #m_pParser==a_pParser
      \param a_pParent Parent parser object of the token reader.
  */
  TokenReader::TokenReader(ParserXBase *a_pParent)
    :m_pParser(a_pParent)
    ,m_sExpr()
    ,m_iPos(0)
    ,m_iSynFlags(0)
    ,m_vTokens()
    ,m_eLastTokCode(cmUNKNOWN)
    ,m_pFunDef(NULL)
    ,m_pOprtDef(NULL)
    ,m_pInfixOprtDef(NULL)
    ,m_pPostOprtDef(NULL)
    ,m_pConstDef(NULL)
    ,m_pDynVarShadowValues(NULL)
    ,m_pVarDef(NULL)
    ,m_vValueReader()
    ,m_UsedVar()
    ,m_fZero(0)
  {
    assert(m_pParser);
    SetParent(m_pParser);
  }
    
  //---------------------------------------------------------------------------
  /** \brief Destructor (trivial).
    
      \throw nothrow
  */
  TokenReader::~TokenReader()
  {
    DeleteValReader();
  }

  //---------------------------------------------------------------------------
  void TokenReader::DeleteValReader()
  {
    int iSize = (int)m_vValueReader.size();
    for (int i=0; i<iSize; ++i)
      delete m_vValueReader[i];
    
    m_vValueReader.clear();
  }

  //---------------------------------------------------------------------------
  /** \brief Create instance of a ParserTokenReader identical with this 
            and return its pointer. 

      This is a factory method the calling function must take care of the object destruction.

      \return A new ParserTokenReader object.
      \throw nothrow
  */
  TokenReader* TokenReader::Clone(ParserXBase *a_pParent) const
  {
    std::auto_ptr<TokenReader> ptr(new TokenReader(*this));
    ptr->SetParent(a_pParent);
    return ptr.release();
  }

  //---------------------------------------------------------------------------
  void TokenReader::AddValueReader(IValueReader *a_pReader)
  {
    a_pReader->SetParent(this);
    m_vValueReader.push_back(a_pReader);
  }

  //---------------------------------------------------------------------------
  void TokenReader::AddSynFlags(int flag)
  {
    m_iSynFlags |= flag;
  }

  //---------------------------------------------------------------------------
  const TokenReader::token_buf_type& TokenReader::GetTokens() const
  {
    return m_vTokens;
  }

  //---------------------------------------------------------------------------
  /** \brief Return the current position of the token reader in the formula string. 

      \return #m_iPos
      \throw nothrow
  */
  int TokenReader::GetPos() const
  {
    return m_iPos;
  }

  //---------------------------------------------------------------------------
  /** \brief Return a reference to the formula. 

      \return #m_sExpr
      \throw nothrow
  */
  const string_type& TokenReader::GetExpr() const
  {
    return m_sExpr;
  }

  //---------------------------------------------------------------------------
  /** \brief Return a map containing the used variables only. */
  const var_maptype& TokenReader::GetUsedVar() const
  {
    return m_UsedVar;
  }

  //---------------------------------------------------------------------------
  /** \brief Initialize the token Reader. 

      Sets the expression position index to zero and set Syntax flags to 
      default for initial parsing. 
  */
  void TokenReader::SetExpr(const string_type &a_sFormula)
  {
    m_sExpr = a_sFormula;
    m_vTokens.clear();
    ReInit();
  }


  //---------------------------------------------------------------------------
  /** \brief Reset the token reader to the start of the formula. 
      \post #m_iPos==0, #m_iSynFlags = noOPT | noBC | noPOSTOP | noSTR
      \throw nothrow
      \sa ESynCodes

      The syntax flags will be reset to a value appropriate for the 
      start of a formula.
  */
  void TokenReader::ReInit()
  {
    m_iPos = 0;
    m_iNumBra = 0;
    m_iSynFlags = noOPT | noBC | noPFX | noCOMMA | noIO | noIC;
    m_UsedVar.clear();
    m_eLastTokCode = cmUNKNOWN;
  }

  //---------------------------------------------------------------------------
  const ptr_tok_type& TokenReader::Store(const ptr_tok_type &t)
  {
    m_eLastTokCode = t->GetCode();
    m_vTokens.push_back(t);
    return t;
  }

  //---------------------------------------------------------------------------
  /** \brief Read the next token from the string. */ 
  ptr_tok_type TokenReader::ReadNextToken()
  {
    assert(m_pParser);
    const char_type *szFormula = m_sExpr.c_str();

    while (szFormula[m_iPos]==' ') 
      ++m_iPos;

    ptr_tok_type pTok;

    // 1.) At first we test only for tokens that we expect at the given location
    if ( !(m_iSynFlags & noEND) && IsEOF(pTok))    
    {
      Store(pTok); 
      return Store(pTok); // Check for end of expression
    }

    /* oprt charset */
    if ( !(m_iSynFlags & noOPT) && IsOprt(pTok))       
      return Store(pTok); // Check for user defined binary operator

    /* infix charset */
    if ( !(m_iSynFlags & noIFX) && IsInfixOpTok(pTok)) 
      return Store(pTok); // Check for unary operators

    if ( !(m_iSynFlags & noVAL) && IsValTok(pTok))     
      return Store(pTok); // Check for values / constant tokens

    if ( !( (m_iSynFlags & (noCOMMA|noBC|noBO|noIC|noIO) ) == (noCOMMA|noBC|noBO|noIC|noIO) ) && IsBuiltIn(pTok) )    
      return Store(pTok); // Check built in operators / tokens

    if ( !(m_iSynFlags & noVAR) && IsVarOrConstTok(pTok))     
      return Store(pTok); // Check for variable tokens

    if ( !(m_iSynFlags & noFUN) && IsFunTok(pTok))     
      return Store(pTok);

    if ( !(m_iSynFlags & noPFX) && IsPostOpTok(pTok))  
      return Store(pTok); // Check for unary operators

    // 2.) We have found no token, maybe there is a token that we don't expect here.
    //     Again call the Identifier functions but this time only thos we dont expect to succeed.
    if ( (m_iSynFlags & noEND) && IsEOF(pTok))         
      return Store(pTok); // Check for end of expression
    
    if ( (m_iSynFlags & noOPT) && IsOprt(pTok))        
      return Store(pTok); // Check for user defined binary operator

    if ( (m_iSynFlags & noIFX) && IsInfixOpTok(pTok))  
      return Store(pTok); // Check for unary operators

    if ( (m_iSynFlags & noVAL) && IsValTok(pTok))      
      return Store(pTok); // Check for values / constant tokens

    if ( (m_iSynFlags & noVAR) && IsVarOrConstTok(pTok))      
      return Store(pTok); // Check for variable tokens

    if ( ( (m_iSynFlags & (noCOMMA|noBC|noBO|noIO|noIC) ) == (noCOMMA|noBC|noBO|noIO|noIC) ) && IsBuiltIn(pTok) )    
      return Store(pTok); // Check built in operators / tokens

    if ( (m_iSynFlags & noFUN) && IsFunTok(pTok))      
      return Store(pTok);

    if ( (m_iSynFlags & noPFX) && IsPostOpTok(pTok))   
      return Store(pTok); // Check for unary operators

    // Now we are in trouble because there is something completely unknown....

    // Check the string for an undefined variable token. This is done 
    // only if a flag is set indicating to ignore undefined variables.
    // This is a way to conditionally avoid an error if undefined variables 
    // occur. The GetExprVar function must supress the error for undefined 
    // variables in order to collect all variable names including the 
    // undefined ones.
    if ( (m_pParser->m_bIsQueryingExprVar || m_pParser->m_bAutoCreateVar) && IsUndefVarTok(pTok) )  
      return Store(pTok);

    // Check for unknown token
    // 
    // !!! From this point on there is no exit without an exception possible...
    // 
    string_type sTok;
    int iEnd = ExtractToken(m_pParser->ValidNameChars(), sTok, m_iPos);

    ErrorContext err;
    err.Errc  = ecUNASSIGNABLE_TOKEN;
    err.Expr  = m_sExpr;
    err.Pos   = m_iPos;

    if (iEnd!=m_iPos)
      err.Ident = sTok;
    else
      err.Ident = m_sExpr.substr(m_iPos);

    throw ParserError(err);
  }

  //---------------------------------------------------------------------------
  void TokenReader::SetParent(ParserXBase *a_pParent)
  {
    m_pParser             =  a_pParent; 
    m_pFunDef             = &a_pParent->m_FunDef;
    m_pOprtDef            = &a_pParent->m_OprtDef;
    m_pInfixOprtDef       = &a_pParent->m_InfixOprtDef;
    m_pPostOprtDef        = &a_pParent->m_PostOprtDef;
    m_pVarDef             = &a_pParent->m_VarDef;
    m_pConstDef           = &a_pParent->m_valConst;
    m_pDynVarShadowValues = &a_pParent->m_valDynVarShadow;
  }

  //---------------------------------------------------------------------------
  /** \brief Extract all characters that belong to a certain charset.
      \param a_szCharSet [in] Const char array of the characters allowed in the token. 
      \param a_strTok [out]  The string that consists entirely of characters listed in a_szCharSet.
      \param a_iPos [in] Position in the string from where to start reading.
      \return The Position of the first character not listed in a_szCharSet.
      \throw nothrow
  */
  int TokenReader::ExtractToken(const char_type *a_szCharSet, 
                                string_type &a_sTok, 
                                int a_iPos) const
  {
    int iEnd = (int)m_sExpr.find_first_not_of(a_szCharSet, a_iPos);

    if (iEnd==(int)string_type::npos)
      iEnd = (int)m_sExpr.length();  

    if (iEnd!=a_iPos)
      a_sTok.assign(m_sExpr.begin()+a_iPos, m_sExpr.begin()+iEnd);

    return iEnd;
  }

  //---------------------------------------------------------------------------
  /** \brief Check if a built in operator or other token can be found.
  */
  bool TokenReader::IsBuiltIn(ptr_tok_type &a_Tok)
  {
    const char_type **pOprtDef = m_pParser->GetOprtDef(),
                     *szFormula = m_sExpr.c_str();
    int i;

    try
    {
      // Compare token with function and operator strings
      // check string for operator/function
      for (i=0; pOprtDef[i]; i++)
      {
        std::size_t len( std::char_traits<char_type>::length(pOprtDef[i]) );
        if ( string_type(pOprtDef[i]) == string_type(szFormula + m_iPos, szFormula + m_iPos + len) )
        {
          switch(i)
          {
          case cmARG_SEP:
		            if (m_iSynFlags & noCOMMA)
                  throw ecUNEXPECTED_COMMA;

                m_iSynFlags = noBC | noOPT | noEND | noCOMMA | noPFX | noIC | noIO;
                a_Tok = ptr_tok_type(new GenericToken((ECmdCode)i, pOprtDef[i]));
	              break;

          case cmELSE:
               m_iSynFlags = noBC | noIO | noIC | noPFX | noEND | noCOMMA | noOPT | noIF | noELSE;
               a_Tok = ptr_tok_type(new TokenElse());
               break;

          case cmIF:
               if (m_eLastTokCode!=cmBC)
                 throw ecUNEXPECTED_CONDITIONAL;

               m_iSynFlags = noBC | noIO | noPFX | noIC | noEND | noCOMMA | noOPT | noIF | noELSE;
               a_Tok = ptr_tok_type(new TokenIf());
               break;

          case cmBO:
                if (m_iSynFlags & noBO)
                  throw ecUNEXPECTED_PARENS;

                if (m_eLastTokCode==cmFUNC)
                  m_iSynFlags = noOPT | noEND | noCOMMA | noPFX | noIC | noIO /*| noASSIGN*/;
                else
                  m_iSynFlags = noBC | noOPT | noEND | noCOMMA | noPFX | noIC | noIO /*| noASSIGN*/;

		            m_iNumBra++;
                a_Tok = ptr_tok_type(new GenericToken((ECmdCode)i, pOprtDef[i]));
                break;

          case cmBC:   
                if (m_iSynFlags & noBC)
                  throw ecUNEXPECTED_PARENS;

                m_iSynFlags  = noBO | noVAR | noVAL | noFUN | noIFX | noIO;
		            m_iNumBra--;

                if (m_iNumBra<0)
                  throw ecUNEXPECTED_PARENS;

                a_Tok = ptr_tok_type(new GenericToken((ECmdCode)i, pOprtDef[i]));
                break;
          
          case cmIO:
                if (m_iSynFlags & noIO)
                  throw ecUNEXPECTED_SQR_BRACKET;

                m_iSynFlags  = noIC | noIO | noOPT | noPFX | noBC;
                a_Tok = ptr_tok_type(new GenericToken((ECmdCode)i, pOprtDef[i]));
                break;

          case cmIC:
                if (m_iSynFlags & noIC)
                  throw ecUNEXPECTED_SQR_BRACKET;

                m_iSynFlags  = noBO | noIFX;
                a_Tok = ptr_tok_type(new GenericToken((ECmdCode)i, pOprtDef[i]));
                break;

          default:  // The operator is listed in c_DefaultOprt, but not here. This is a bad thing...
                throw ecINTERNAL_ERROR;
          } // switch operator id

          m_iPos += (int)len;
//          a_Tok = ptr_tok_type(new GenericToken((ECmdCode)i, pOprtDef[i]));
          return true;
        } // if operator string found
      } // end of for all operator strings
    }
    catch (EErrorCodes e)
    {
      ErrorContext err;
      err.Errc  = e;
      err.Expr  = m_sExpr;
      err.Ident = pOprtDef[i];
      err.Pos   = m_iPos;
      throw ParserError(err);
    }

    return false;
  }

  //---------------------------------------------------------------------------
  /** \brief Check for End of Formula.
  */
  bool TokenReader::IsEOF(ptr_tok_type &a_Tok)
  {
    bool bRet(false);
    try
    {
      if ( m_sExpr.length() && m_iPos>=(int)m_sExpr.length())
      {
        if ( m_iSynFlags & noEND )
          throw ecUNEXPECTED_EOF;

        if (m_iNumBra>0)
          throw ecMISSING_PARENS;

        m_iSynFlags = 0;
        a_Tok = ptr_tok_type(new GenericToken(cmEOE));
        bRet = true;
      }
    }
    catch(EErrorCodes e)
    {
      ErrorContext err;
      err.Errc = e;
      err.Ident = _T("");
      err.Expr = m_sExpr;
      err.Pos = m_iPos;
      throw ParserError(err);
    }

    return bRet;
  }

  //---------------------------------------------------------------------------
  /** \brief Check if a string position contains a unary infix operator. 
      \return true if a function token has been found false otherwise.
  */
  bool TokenReader::IsInfixOpTok(ptr_tok_type &a_Tok)
  {
    string_type sTok;
    int iEnd = ExtractToken(m_pParser->ValidInfixOprtChars(), sTok, m_iPos);
    
    if (iEnd==m_iPos)
      return false;
    
    try
    {
      // iteraterate over all postfix operator strings
      oprt_ifx_maptype::const_iterator item = m_pInfixOprtDef->begin();
      for (item=m_pInfixOprtDef->begin(); item!=m_pInfixOprtDef->end(); ++item)
      {
        if (sTok.find(item->first)!=0)
          continue;

        a_Tok = item->second;
        m_iPos += (int)item->first.length();

        if (m_iSynFlags & noIFX)
          throw ecUNEXPECTED_OPERATOR;

        m_iSynFlags = noPFX | noIFX | noOPT | noBC | noIC | noIO; 
        return true;
      }
      
      return false;
    }
    catch(EErrorCodes e)
    {
      ErrorContext err;
      err.Errc = e;
      err.Pos = m_iPos;
      err.Ident = a_Tok->GetIdent();
      err.Expr = m_sExpr;
      throw ParserError(err);
    }
  }


  //---------------------------------------------------------------------------
  /** \brief Check expression for function tokens. */
  bool TokenReader::IsFunTok(ptr_tok_type &a_Tok)
  {
    if (m_pFunDef->size()==0)
      return false;

    string_type sTok;
    int iEnd = ExtractToken(m_pParser->ValidNameChars(), sTok, m_iPos);
    if (iEnd==m_iPos)
      return false;
    
    try
    {
      fun_maptype::iterator item = m_pFunDef->find(sTok);
      if (item==m_pFunDef->end())
        return false;

      m_iPos = (int)iEnd;
      a_Tok = item->second;
      if (m_iSynFlags & noFUN)
        throw ecUNEXPECTED_FUN;

      m_iSynFlags = noANY ^ noBO;
      return true;
    }
    catch(EErrorCodes e)
    {
      ErrorContext err;
      err.Errc = e;
      err.Pos = m_iPos - (int)a_Tok->GetIdent().length();
      err.Ident = a_Tok->GetIdent();
      err.Expr = m_sExpr;
      throw ParserError(err);
    }
  }

  //---------------------------------------------------------------------------
  /** \brief Check if a string position contains a unary post value operator. */
  bool TokenReader::IsPostOpTok(ptr_tok_type &a_Tok)
  {
    // Tricky problem with equations like "3m+5":
    //     m is a postfix operator, + is a valid sign for postfix operators and 
    //     for binary operators parser detects "m+" as operator string and 
    //     finds no matching postfix operator.
    // 
    // This is a special case so this routine slightly differs from the other
    // token readers.
    
    // Test if there could be a postfix operator
    string_type sTok;
    int iEnd = ExtractToken(m_pParser->ValidOprtChars(), sTok, m_iPos);
    if (iEnd==m_iPos)
      return false;

    try
    {
      // iteraterate over all postfix operator strings
      oprt_pfx_maptype::const_iterator item;
      for (item=m_pPostOprtDef->begin(); item!=m_pPostOprtDef->end(); ++item)
      {
        if (sTok.find(item->first)!=0)
          continue;

        a_Tok = item->second;
        m_iPos += (int)item->first.length();

        if (m_iSynFlags & noPFX)
          throw ecUNEXPECTED_OPERATOR;

        m_iSynFlags = noVAL | noVAR | noFUN | noBO | noPFX | noIO;
        return true;
      }
      
      return false;
    }
    catch(EErrorCodes e)
    {
      ErrorContext err;
      err.Errc = e;
      err.Pos = m_iPos - (int)a_Tok->GetIdent().length();
      err.Ident = a_Tok->GetIdent();
      err.Expr = m_sExpr;
      throw ParserError(err);
    }
  }

  //---------------------------------------------------------------------------
  /** \brief Check if a string position contains a binary operator. */
  bool TokenReader::IsOprt(ptr_tok_type &a_Tok)
  {
    string_type sTok;
    int iEnd = ExtractToken(m_pParser->ValidOprtChars(), sTok, m_iPos);
    if (iEnd==m_iPos)
      return false;

    oprt_bin_multimap::reverse_iterator item;
    try
    {
      // Note:
      // All tokens in oprt_bin_maptype are have been sorted by their length
      // Long operators must come first! Otherwise short names (like: "add") that
      // are part of long token names (like: "add123") will be found instead 
      // of the long ones.
      // Length sorting is done with ascending length so we use a reverse iterator here.
      for (item=m_pOprtDef->rbegin(); item!=m_pOprtDef->rend(); ++item)
      {
        if (sTok.find(item->first)!=0)
          continue;

        // operator found, check if we expect one...
        if (m_iSynFlags & noOPT) 
        {
          // An operator was found but is not expected to occur at
          // this position of the formula, maybe it is an infix 
          // operator, not a binary operator. Both operator types
          // can use the same characters in their identifiers.
          if (IsInfixOpTok(a_Tok))
            return true;

          // nope, it's no infix operator and we dont expect 
          // an operator
          throw ecUNEXPECTED_OPERATOR;
        }
        else
        {
          // 20100921: Operatorüberlagerung ist nicht mehr aktuell...
          //// resolve overloaded operator
          //if (m_pOprtDef->count(sTok)>1)
          //{
          //  MUP_FAIL(OVERLOADING_NOT_SUPPORTED);
          //  std::auto_ptr<OprtBinOverloadProxy> pOprtProxy(new OprtBinOverloadProxy(sTok));
          //  oprt_bin_multimap::iterator it,
          //                              itFirstOverload = m_pOprtDef->equal_range(sTok).first,
          //                              itLastOverload = m_pOprtDef->equal_range(sTok).second;
          //  for (it=itFirstOverload; it!=itLastOverload; ++it)
          //    pOprtProxy->AddOverload(it->second);
          //  
          //  a_Tok = ptr_tok_type(pOprtProxy.release());
          //  // 20081005 operator overload is unfinished
          //}
          //else
          a_Tok = ptr_tok_type(item->second->Clone());

          m_iPos += (int)a_Tok->GetIdent().length(); 
          m_iSynFlags  = noBC | noIO | noIC | noOPT | noCOMMA | noPFX | noEND;
          return true;
        }
      }

      return false;
    }
    catch(EErrorCodes e)
    {
      ErrorContext err;
      err.Errc = e;
      err.Pos = m_iPos; // - (int)item->first.length();
      err.Ident = item->first;
      err.Expr = m_sExpr;
      throw ParserError(err);
    }
  }

  //---------------------------------------------------------------------------
  /** \brief Check whether the token at a given position is a value token.

    Value tokens are either values or constants.

    \param a_Tok [out] If a value token is found it will be placed here.
    \return true if a value token has been found.
  */
  bool TokenReader::IsValTok(ptr_tok_type &a_Tok)
  {
    if (m_vValueReader.size()==0)
      return false;

    stringstream_type stream(m_sExpr.c_str() + m_iPos);
    string_type sTok;
    
    try
    {
      // call the value recognition functions provided by the user
      // Call user defined value recognition functions
      int iSize = (int)m_vValueReader.size();
      Value val;
      for (int i=0; i<iSize; ++i)
      {
        int iStart = m_iPos;
        if ( m_vValueReader[i]->IsValue(m_sExpr.c_str(), m_iPos, val) )
        {
          sTok.assign(m_sExpr.c_str(), iStart, m_iPos);
          if (m_iSynFlags & noVAL)
            throw ecUNEXPECTED_VAL;

          m_iSynFlags = noVAL | noVAR | noFUN | noBO | noIFX | noIO;
          a_Tok = ptr_tok_type(val.Clone());
          a_Tok->SetIdent( string_type(sTok.begin(), sTok.begin()+(m_iPos-iStart)) );
          return true;
        }
      }
    }
    catch(EErrorCodes e)
    {
      ErrorContext err;
      err.Errc = e;
      err.Pos = m_iPos;
      err.Ident = sTok;
      err.Expr = m_sExpr;
      err.Pos = m_iPos - (int)sTok.length();
      throw ParserError(err);
    }

    return false;
  }

  //---------------------------------------------------------------------------
  /** \brief Check wheter a token at a given position is a variable token. 
      \param a_Tok [out] If a variable token has been found it will be placed here.
      \return true if a variable token has been found.
  */
  bool TokenReader::IsVarOrConstTok(ptr_tok_type &a_Tok)
  {
    if (!m_pVarDef->size() && !m_pConstDef->size() && !m_pFunDef->size())
      return false;

    string_type sTok;
    int iEnd;
    try
    {
      iEnd = ExtractToken(m_pParser->ValidNameChars(), sTok, m_iPos);
      if (iEnd==m_iPos)
        return false;

      // Check for variables
      var_maptype::const_iterator item =  m_pVarDef->find(sTok);
      if (item!=m_pVarDef->end())
      {
        if (m_iSynFlags & noVAR)
          throw ecUNEXPECTED_VAR;
        
        m_iPos = iEnd;
        m_iSynFlags = noVAL | noVAR | noFUN | noBO | noIFX;
        a_Tok = item->second;
        a_Tok->SetIdent(sTok);
        m_UsedVar[item->first] = item->second;  // Add variable to used-var-list
        return true;
      }

      // Check for constants
      item = m_pConstDef->find(sTok);
      if (item!=m_pConstDef->end())
      {
        if (m_iSynFlags & noVAL)
          throw ecUNEXPECTED_VAL;

        m_iPos = iEnd;
        m_iSynFlags = noVAL | noVAR | noFUN | noBO | noIFX | noIO;
        a_Tok = item->second;
        a_Tok->SetIdent(sTok);
        return true;
      }

      return false;
    }
    catch(EErrorCodes e)
    {
      ErrorContext err;
      err.Errc = e;
      err.Pos = m_iPos;
      err.Ident = sTok;
      err.Expr = m_sExpr;
      err.Pos = m_iPos;
      throw ParserError(err);
    }

    return true;
  }

  //---------------------------------------------------------------------------
  /** \brief Check wheter a token at a given position is an undefined variable. 
      \param a_Tok [out] If a variable tom_pParser->m_vStringBufken has been found it will be placed here.
      \return true if a variable token has been found.
      \throw nothrow
  */
  bool TokenReader::IsUndefVarTok(ptr_tok_type &a_Tok)
  {
    string_type sTok;
    int iEnd = ExtractToken(m_pParser->ValidNameChars(), sTok, m_iPos);
    if (iEnd==m_iPos)
      return false;

    if (m_iSynFlags & noVAR)
    {
      ErrorContext err;
      err.Errc = ecUNEXPECTED_VAR;
      err.Pos = m_iPos;
      err.Ident = sTok;
      err.Expr = m_sExpr;
      err.Pos = m_iPos - (int)sTok.length();
      throw ParserError(err);
    }

    // Create a variable token
    if (m_pParser->m_bAutoCreateVar)
    {
      ptr_val_type val(new Value);                   // Create new value token
      m_pDynVarShadowValues->push_back(val);         // push to the vector of shadow values 
      a_Tok = ptr_tok_type(new Variable(val.Get())); // bind variable to the new value item
      (*m_pVarDef)[sTok] = a_Tok;                    // add new variable to the variable list
    }
    else
      a_Tok = ptr_tok_type(new Variable(NULL));      // bind variable to empty variable

    a_Tok->SetIdent(sTok);
    m_UsedVar[sTok] = a_Tok;     // add new variable to used-var-list

    m_iPos = iEnd;
    m_iSynFlags = noVAL | noVAR | noFUN | noBO | noIFX;
    return true;
  }
} // namespace mu
