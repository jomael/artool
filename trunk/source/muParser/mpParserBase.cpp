/** \file
    \brief Implementation of the muParserX engine.

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
#include "mpParserBase.h"

#include <cassert>
#include <cmath>

//see mpParserBase.h for details on this:
#if defined(_MSC_VER) && (_MSC_VER <= 1200)
	#include "memory.h"
#else
	#include <memory>
#endif


#include <vector>
#include <sstream>

#include "utGeneric.h"
#include "mpDefines.h"
#include "mpIfThenElse.h"

MUP_NAMESPACE_START

//Sadjad: war ueber MUP_NAMESPACE_START
//using namespace std;
using std::make_pair;

  //------------------------------------------------------------------------------
  const char_type *g_sTypeShortCut = _T("bifcsav.");

  //------------------------------------------------------------------------------
  const char_type *g_sCmdCode[] = { _T("BRCK. OPEN  "),
                                    _T("BRCK. CLOSE "),
                                    _T("IDX OPEN    "),
                                    _T("IDX CLOSE   "),
                                    _T("COMMA       "),
                                    _T("IF          "),
                                    _T("ELSE        "),
                                    _T("ENDIF       "),
                                    _T("JMP         "),
                                    _T("VAR         "),
                                    _T("VAL         "),
                                    _T("FUNC        "),
                                    _T("OPRT_BIN    "),
                                    _T("OPRT_IFX    "),
                                    _T("OPRT_PFX    "),
                                    _T("END         "),
                                    _T("BIN_OVERLOAD"),
                                    _T("JMP         "),
                                    _T("UNKNOWN     ") };

  //------------------------------------------------------------------------------
  /** \brief Identifiers for built in binary operators. 

      When defining custom binary operators with AddOprt(...) make sure not to choose 
      names conflicting with these definitions. 
  */
  const char_type* ParserXBase::c_DefaultOprt[] = { _T("("), 
                                                    _T(")"), 
                                                    _T("["), 
                                                    _T("]"), 
                                                    _T(","),
                                                    _T("?"),
                                                    _T(":"),
                                                    0 };

  //------------------------------------------------------------------------------
  /** \brief Default constructor. */
  ParserXBase::ParserXBase()
    :m_pParserEngine(&ParserXBase::ParseFromString)
    ,m_pTokenReader()
    ,m_FunDef()
    ,m_PostOprtDef()
    ,m_InfixOprtDef()
    ,m_OprtDef()
    ,m_valConst()
    ,m_valDynVarShadow()
    ,m_VarDef()
    ,m_sNameChars()
    ,m_sOprtChars()
    ,m_sInfixOprtChars()
    ,m_nIfElseCounter(0)
    ,m_nFinalResultIdx(0)
    ,m_bIsQueryingExprVar(false)
    ,m_bAutoCreateVar(false)
    ,m_bEnableOptimization(true)
    ,m_rpn()
    ,m_vStackBuffer()
    ,m_BufferValue()
  {
    InitTokenReader();
  }

  //---------------------------------------------------------------------------
  /** \brief Copy constructor.
      \param a_Parser Reference to the other parser object

    Implemented by calling Assign(a_Parser)
  */
  ParserXBase::ParserXBase(const ParserXBase &a_Parser)
    :m_pParserEngine(&ParserXBase::ParseFromString)
    ,m_pTokenReader()
    ,m_FunDef()
    ,m_PostOprtDef()
    ,m_InfixOprtDef()
    ,m_OprtDef()
    ,m_valConst()
    ,m_valDynVarShadow()
    ,m_VarDef()
    ,m_sNameChars()
    ,m_sOprtChars()
    ,m_sInfixOprtChars()
    ,m_nIfElseCounter(0)
    ,m_nFinalResultIdx(0)
    ,m_bAutoCreateVar()
    ,m_bEnableOptimization(true)
    ,m_rpn()
    ,m_vStackBuffer()
    ,m_BufferValue()
  {
    m_pTokenReader.reset(new TokenReader(this));
    Assign(a_Parser);
  }

  //---------------------------------------------------------------------------
  /** \brief Destructor.
      \throw nothrow
  */
  ParserXBase::~ParserXBase()
  {
    // It is important to release the stack buffer before
    // releasing the value cache. Since it may contain
    // Values referencing the cache.
    m_vStackBuffer.clear();
    m_cache.ReleaseAll();
  }

  //---------------------------------------------------------------------------
  /** \brief Assignement operator. 
      \param a_Parser Object to copy to this.
      \return *this
      \throw nothrow
      
    Implemented by calling Assign(a_Parser). Self assignement is suppressed.
  */
  ParserXBase& ParserXBase::operator=(const ParserXBase &a_Parser)
  {
    Assign(a_Parser);
    return *this;
  }

  //---------------------------------------------------------------------------
  /** \brief Copy state of a parser object to this. 
      \param a_Parser the source object.

    Clears Variables and Functions of this parser.
    Copies the states of all internal variables.
    Resets parse function to string parse mode.
  */
  void ParserXBase::Assign(const ParserXBase &ref)
  {
    if (&ref==this)
      return;

    // Don't copy bytecode instead cause the parser to create new bytecode 
    // by resetting the parse function.
    ReInit();

    m_pTokenReader.reset(ref.m_pTokenReader->Clone(this));

    m_OprtDef         = ref.m_OprtDef;
    m_FunDef          = ref.m_FunDef;
    m_PostOprtDef     = ref.m_PostOprtDef;
    m_InfixOprtDef    = ref.m_InfixOprtDef;
    m_valConst        = ref.m_valConst;
    m_valDynVarShadow = ref.m_valDynVarShadow;
    m_VarDef          = ref.m_VarDef;             // Copy user defined variables
    m_nIfElseCounter  = ref.m_nIfElseCounter;
    m_nFinalResultIdx = ref.m_nFinalResultIdx;

    // Copy charsets
    m_sNameChars = ref.m_sNameChars;
    m_sOprtChars = ref.m_sOprtChars;
    m_sInfixOprtChars = ref.m_sInfixOprtChars;

    m_bAutoCreateVar = ref.m_bAutoCreateVar;
    m_bEnableOptimization = ref.m_bEnableOptimization;

    // Things that should not be copied:
    // - m_vStackBuffer
    // - m_cache
    // - m_rpn
  }

  //---------------------------------------------------------------------------
  /** \brief Evaluate the expression.
      \pre A formula must be set.
      \pre Variables must have been set (if needed)
      \sa SetExpr
      \return The evaluation result
      \throw ParseException if no Formula is set or in case of any other error related to the formula.

      A note on const correctness: 
      I consider it important that Calc is a const function.
      Due to caching operations Calc changes only the state of internal variables with one exception
      m_UsedVar this is reset during string parsing and accessible from the outside. Instead of making
      Calc non const GetExprVar is non const because it explicitely calls Eval() forcing this update. 
  */
  const IValue& ParserXBase::Eval() const
  {
    return (this->*m_pParserEngine)(); 
  }
  
  //---------------------------------------------------------------------------
  /** \brief Return the strings of all Operator identifiers. 
      \return Returns a pointer to the c_DefaultOprt array of const char *.
      \throw nothrow

      GetOprt is a const function returning a pinter to an array of const char pointers.
  */
  const char_type** ParserXBase::GetOprtDef() const
  {
    return (const char_type **)(&c_DefaultOprt[0]);
  }

  //---------------------------------------------------------------------------
  /** \brief Define the set of valid characters to be used in names of
              functions, variables, constants.
  */
  void ParserXBase::DefineNameChars(const char_type *a_szCharset)
  {
    m_sNameChars = a_szCharset;
  }

  //---------------------------------------------------------------------------
  /** \brief Define the set of valid characters to be used in names of
             binary operators and postfix operators.
      \param a_szCharset A string containing all characters that can be used 
                         in operator identifiers.
  */
  void ParserXBase::DefineOprtChars(const char_type *a_szCharset)
  {
    m_sOprtChars = a_szCharset;
  }

  //---------------------------------------------------------------------------
  /** \brief Define the set of valid characters to be used in names of
             infix operators.
      \param a_szCharset A string containing all characters that can be used 
                         in infix operator identifiers.
  */
  void ParserXBase::DefineInfixOprtChars(const char_type *a_szCharset)
  {
    m_sInfixOprtChars = a_szCharset;
  }

  //---------------------------------------------------------------------------
  /** \brief Virtual function that defines the characters allowed in name identifiers. 
      \sa #ValidOprtChars, #ValidPrefixOprtChars
  */ 
  const char_type* ParserXBase::ValidNameChars() const
  {
    assert(m_sNameChars.size());
    return m_sNameChars.c_str();
  }

  //---------------------------------------------------------------------------
  /** \brief Virtual function that defines the characters allowed in operator definitions. 
      \sa #ValidNameChars, #ValidPrefixOprtChars
  */
  const char_type* ParserXBase::ValidOprtChars() const
  {
    assert(m_sOprtChars.size());
    return m_sOprtChars.c_str();
  }

  //---------------------------------------------------------------------------
  /** \brief Virtual function that defines the characters allowed in infix operator definitions.
      \sa #ValidNameChars, #ValidOprtChars
  */
  const char_type* ParserXBase::ValidInfixOprtChars() const
  {
    assert(m_sInfixOprtChars.size());
    return m_sInfixOprtChars.c_str();
  }


  //---------------------------------------------------------------------------
  /** \brief Initialize the token reader. 
      \post m_pTokenReader.Get()!=0
      \throw nothrow

    Create new token reader object and submit pointers to function, operator,
    constant and variable definitions.
  */
  void ParserXBase::InitTokenReader()
  {
    m_pTokenReader.reset(new TokenReader(this));
  }

  //---------------------------------------------------------------------------
  /** \brief Reset parser to string parsing mode and clear internal buffers.
      \throw nothrow

      Resets the token reader.
  */
  void ParserXBase::ReInit() const
  {
    m_pParserEngine = &ParserXBase::ParseFromString;
    m_pTokenReader->ReInit();
    m_rpn.Reset();
    m_vStackBuffer.clear();
  }

  //---------------------------------------------------------------------------
  /** \brief Adds a new package to the parser. 
  
    The parser becomes the owner of the package pointer and is responsible for 
    its deletion.
  */
  void ParserXBase::AddPackage(IPackage *p)
  {
    p->AddToParser(this);
  }

  //---------------------------------------------------------------------------
  void ParserXBase::RemovePackage(IPackage *p)
  {
  }

  //---------------------------------------------------------------------------
  /** \brief Add a value reader object to muParserX. 
      \param a_pReader Pointer to the value reader object.
  */
  void ParserXBase::AddValueReader(IValueReader *a_pReader)
  {
    m_pTokenReader->AddValueReader(a_pReader);
  }

  //---------------------------------------------------------------------------
  /** \brief Check if a given name contains invalid characters. 
      \param a_strName The name to check
      \param a_szCharSet The characterset
      \throw ParserException if the name contains invalid charakters.
  */
  void ParserXBase::CheckName(const string_type &a_strName, 
                              const string_type &a_szCharSet) const
  {
    if ( !a_strName.length() || 
        (a_strName.find_first_not_of(a_szCharSet)!=string_type::npos) ||
        (a_strName[0]>=(char_type)'0' && a_strName[0]<=(char_type)'9'))
    {
      Error(ecINVALID_NAME);
    }
  }

  //---------------------------------------------------------------------------
  /** \brief Set the mathematical expression. 
      \param a_sExpr String with the expression
      \throw ParserException in case of syntax errors.

      Triggers first time calculation thus the creation of the bytecode and 
      scanning of used variables.
  */
  void ParserXBase::SetExpr(const string_type &a_sExpr)
  {
    m_pTokenReader->SetExpr(a_sExpr);
    ReInit();
  }

  //---------------------------------------------------------------------------
  /** \brief Add a user defined operator. 
      \post Will reset the Parser to string parsing mode.
      \param a_pOprt Pointer to a unary postfix operator object. The parser will
                     become the new owner of this object hence will destroy it.
  */
  void ParserXBase::DefinePostfixOprt(IOprtPostfix *a_pOprt)
  {
    m_PostOprtDef[a_pOprt->GetIdent()] = ptr_tok_type(a_pOprt);
    ReInit();
  }

  //---------------------------------------------------------------------------
  /** \brief Add a user defined operator. 
      \param a_pOprt Pointer to a unary postfix operator object. The parser will
             become the new owner of this object hence will destroy it.
  */
  void ParserXBase::DefineInfixOprt(IOprtInfix *a_pOprt)
  {
    m_InfixOprtDef[a_pOprt->GetIdent()] = ptr_tok_type(a_pOprt);
    ReInit();
  }

  //---------------------------------------------------------------------------
  /** \brief Add a user defined variable. 
      \param a_sName The variable name
      \param a_Var The variable to be added to muParserX
  */
  void ParserXBase::DefineVar(const string_type &a_sName, const Variable &a_Var)
  {
    CheckName(a_sName, ValidNameChars());
    m_VarDef[a_sName] = ptr_tok_type( a_Var.Clone() );
    ReInit();
  }
  

  //---------------------------------------------------------------------------
  /** \brief Define a parser Constant. 
      \param a_sName The name of the constant
      \param a_Val Const reference to the constants value

    Parser constants are handed over by const reference as opposed to variables 
    which are handed over by reference. Consequently the parser can not change 
    their value.
  */
  void ParserXBase::DefineConst(const string_type &a_sName, const Value &a_Val)
  {
    CheckName(a_sName, ValidNameChars());
    m_valConst[a_sName] = ptr_tok_type( a_Val.Clone() );
    ReInit();
  }

  //---------------------------------------------------------------------------
  /** \brief Add a callback object to the parser. 
      \param a_pFunc Pointer to the intance of a parser callback object 
                     representing the function.
      \sa GetFunDef, functions

    The parser takes ownership over the callback object.
  */
  void ParserXBase::DefineFun(ICallback *a_pFunc)
  {
    a_pFunc->SetParent(this);
    m_FunDef[ a_pFunc->GetIdent() ] = ptr_tok_type(a_pFunc);
  }

  //---------------------------------------------------------------------------
  /** \brief Define a binara operator. 
      \param a_pCallback Pointer to the callback object
  */
  void ParserXBase::DefineOprt(IOprtBin *a_pCallback)
  {
    a_pCallback->SetParent(this);
    m_OprtDef.insert(make_pair(a_pCallback->GetIdent(), ptr_tok_type(a_pCallback)));
  }

  //---------------------------------------------------------------------------
  /** \brief Return a map containing the used variables only. */
  const var_maptype& ParserXBase::GetExprVar() const
  {
//    utils::scoped_setter<bool> guard1(m_bAutoCreateVar, false);
    utils::scoped_setter<bool> guard2(m_bIsQueryingExprVar, true);
    ParseFromString(); // implicitly create or update the map with the
                       // used variables stored in the token reader if not already done
    
    // Make sure to stay in string parse mode, cause it
    // can't be guaranteed that the variables even exist.
    // Don't call ReInit() because it deletes the array 
    // with the used variables
    m_pParserEngine = &ParserXBase::ParseFromString;
    
    return m_pTokenReader->GetUsedVar();
  }

  //---------------------------------------------------------------------------
  /** \brief Return a map containing the used variables only. */
  const var_maptype& ParserXBase::GetVar() const
  {
    return m_VarDef;
  }

  //---------------------------------------------------------------------------
  /** \brief Return a map containing all parser constants. */
  const val_maptype& ParserXBase::GetConst() const
  {
    return m_valConst;
  }

  //---------------------------------------------------------------------------
  /** \brief Return prototypes of all parser functions.
      \return #m_FunDef
      \sa FunProt, functions
      \throw nothrow

    The return type is a map of the public type #funmap_type containing the prototype
    definitions for all numerical parser functions. String functions are not part of 
    this map. The Prototype definition is encapsulated in objects of the class FunProt
    one per parser function each associated with function names via a map construct.
  */
  const fun_maptype& ParserXBase::GetFunDef() const
  {
    return m_FunDef;
  }

  //---------------------------------------------------------------------------
  /** \brief Retrieve the mathematical expression. */
  const string_type& ParserXBase::GetExpr() const
  {
    return m_pTokenReader->GetExpr();
  }

  //---------------------------------------------------------------------------
  /** \brief Get the version number of muParserX. 
      \return A string containing the version number of muParserX.
  */
  string_type ParserXBase::GetVersion() 
  {
    return MUP_PARSER_VERSION;
  }

  //---------------------------------------------------------------------------
  void ParserXBase::ApplyBinAndTernaryOprt(Stack<ptr_tok_type> &stOpt,
                                           Stack<ptr_val_type> &stVal) const


  {
    while (stOpt.size() && 
           stOpt.top()->GetCode() != cmBO && 
           stOpt.top()->GetCode() != cmIO && 
           stOpt.top()->GetCode() != cmIF)
    {
      ptr_tok_type &op = stOpt.top();
                      
      switch(op->GetCode())
      {
        case  cmOPRT_BIN:
              MUP_ASSERT(stOpt.top()->GetCode()==cmOPRT_BIN || stOpt.top()->GetCode()==cmOPRT_BIN_OVERLOAD);
              ApplyFunc(stOpt, stVal, 2);
              break;

        case  cmELSE:
              {
                ApplyIfElse(stOpt, stVal);

                // Compute the jump distance to skip the else branch and add it
                // to the token.
                TokenElse *pElseToken = MUP_TOK_CAST(TokenElse*, op.Get());
                int offset = m_rpn.GetSize() - op->GetRPNIdx();
                pElseToken->SetOffset(offset);
                m_rpn.Add(ptr_tok_type(new GenericToken(cmENDIF, _T("endif"))));
              }
              break;

        default:
              Error(ecINTERNAL_ERROR);
      } // switch operator token type
    } // While operator stack not empty
  }

  //---------------------------------------------------------------------------
  /** \brief Calls a parser function with its corresponding arguments. 
      \param a_stOpt The operator stack
      \param a_stVal The value stack
      \param a_iArgCount The number of function arguments
  */
  void ParserXBase::ApplyFunc(Stack<ptr_tok_type> &a_stOpt,
                              Stack<ptr_val_type> &a_stVal, 
                              int a_iArgCount) const
  { 
    if (a_stOpt.empty())
      return;

    ptr_tok_type tok = a_stOpt.pop();
    ICallback *pFun = tok->AsICallback();

    int iArgCount = (pFun->GetArgc()>=0) ? pFun->GetArgc() : a_iArgCount;
    int iOffset = a_stVal.size() - iArgCount;
    MUP_ASSERT(iOffset>=0);

    // The paramater stack may be empty since functions may not
    // have a parameter. They do always have a return value though.
    // If the param stack is empty create an entry for the function 
    // return value.
    if (iArgCount==0 && a_stVal.size()==0)
      a_stVal.push(ptr_val_type(new Value()));

    ptr_val_type *pArg = a_stVal.get_data() + iOffset;

    try
    {
      CheckArgTypes(pArg, iArgCount, pFun);

      // Check if RPN optimization is possible
      bool bOptimize = m_bEnableOptimization;
      for (int i=0; i<iArgCount && bOptimize==true; ++i)
      {
        if (pArg[i]->IsFlagSet(IToken::flVOLATILE))
          bOptimize = false;
      }

      // Evaluate binary Operator and store the result in the 
      // Value Stack overwriting the first argument
      ptr_val_type buf(new Value);
      // added by CBG: do not evaluate any functions when just
      // querying expression variables
      if (!m_bIsQueryingExprVar)
      {
        pFun->Eval(buf, const_cast<ptr_val_type*>(pArg), iArgCount);
        *pArg = buf;

        pFun->SetNumArgsPresent(iArgCount);
      }
      
      if (!bOptimize)
      {
        (*pArg)->AddFlags(IToken::flVOLATILE);
        m_rpn.Add(tok);
      }
      else
      {
        m_rpn.Pop(iArgCount);
        m_rpn.Add(ptr_tok_type(pArg->Get()));
      }
    }
    catch(ParserError &e)
    {
      // This are type related errors caused by undefined
      // variables. They must be ignored...
      if (!m_bIsQueryingExprVar)
      {
        ErrorContext &err = e.GetContext();
        err.Pos   = m_pTokenReader->GetPos();
        err.Expr  = m_pTokenReader->GetExpr();

        if (err.Ident.empty())
          err.Ident = pFun->GetIdent();

        throw;
      }
    }

    if (iArgCount>0)
      a_stVal.pop(iArgCount-1); // remove the arguments
  }

  //---------------------------------------------------------------------------
  void ParserXBase::ApplyIfElse(Stack<ptr_tok_type> &a_stOpt,
                                Stack<ptr_val_type> &a_stVal) const
  {
    MUP_ASSERT(a_stOpt.size()>0);
    MUP_ASSERT(a_stVal.size()>=3);

    // it then else is a ternary operator Pop all three values from the value s
    // tack and just return the right value
    ptr_val_type vVal2 = a_stVal.pop();
    ptr_val_type vVal1 = a_stVal.pop();
    ptr_val_type bExpr = a_stVal.pop();
    // added by CBG: no need to evaluate expression when we are just checking variable names
    if (!m_bIsQueryingExprVar)
    {
      a_stVal.push( (bExpr->GetBool()) ? vVal1 : vVal2);
    }
    else
    {
      a_stVal.push(vVal1);
    }
    
    // Result of if then else is always volatile, the
    // function optimizer won't handle it properly
    a_stVal.top()->AddFlags(IToken::flVOLATILE);

    ptr_tok_type opElse = a_stOpt.pop();
    ptr_tok_type opIf = a_stOpt.pop();
    m_nIfElseCounter--;
    
    // added by CBG: no need to evaluate expression when we are just checking variable names
    if (!m_bIsQueryingExprVar)
    {
      assert(opIf->GetCode()==cmIF);
      TokenIf *pIfToken = MUP_TOK_CAST(TokenIf*, opIf.Get());
      int offset = opElse->GetRPNIdx() - opIf->GetRPNIdx();
      pIfToken->SetOffset(offset);
    }
  }

  //---------------------------------------------------------------------------
  void ParserXBase::DumpRPN() const
  {
    m_rpn.AsciiDump();
  }

  //---------------------------------------------------------------------------
  /** \brief One of the two main parse functions.
      \sa ParseCmdCode(), ParseValue()

    Parse expression from input string. Perform syntax checking and create bytecode.
    After parsing the string and creating the bytecode the function pointer 
    #m_pParseFormula will be changed to the second parse routine the uses bytecode instead of string parsing.
  */
  const IValue& ParserXBase::ParseFromString() const
  { 
    if (!m_pTokenReader->GetExpr().length()) 
      Error(ecUNEXPECTED_EOF, 0);

    // The Stacks take the ownership over the tokens
    Stack<ptr_tok_type> stOpt;
    Stack<ptr_val_type> stVal;
    Stack<ICallback*>   stFunc;  
    Stack<string_type>  stArgTypes;
    ptr_tok_type pTok;
    Value val;    

    ReInit();
    m_nIfElseCounter = 0;

    // The outermost counter counts the number of seperated items
    // such as in "a=10,b=20,c=c+a"
    stArgTypes.push( _T("") );

    for(bool bLoop=true; bLoop;)
    {
      pTok = m_pTokenReader->ReadNextToken();

#if defined(MUP_DUMP_TOKENS)
      cout << pTok->AsciiDump() << endl;
#endif
      ECmdCode eCmd = pTok->GetCode();
      switch (eCmd)
      {
      case  cmVAR:
      case  cmVAL:
            {
              IValue *pVal = pTok->AsIValue();
              if (stFunc.empty() && pVal->GetType()=='n')
              {
                ErrorContext err;
                err.Errc  = ecUNEXPECTED_PARENS;
                err.Ident = _T(")");
                err.Pos   = m_pTokenReader->GetPos();
                throw ParserError(err);
              }

              stVal.push( ptr_val_type(pVal) );
              
              // Arrays can't be added directly to the reverse polish notation
              // since there may be an index operator following next...
              m_rpn.Add(pTok);

              // Apply infix operator if existant
              if (stOpt.size() && stOpt.top()->GetCode()==cmOPRT_INFIX) 
                ApplyFunc(stOpt, stVal, 1);
            }
            break;

      case  cmARG_SEP:
            {
              // Commas without functions are allowed now... (i.e.: "a=1,b=2,c=a+b")
              //if (stFunc.empty()) 
              //Error(ecUNEXPECTED_COMMA, m_pTokenReader->GetPos());

              if (stVal.size()) // increase argument counter
                stArgTypes.top() += stVal.top()->GetType();

              ApplyBinAndTernaryOprt(stOpt, stVal);
            }
            break;

      case  cmIC:
            {
              ApplyBinAndTernaryOprt(stOpt, stVal);

              // if opt is "]" and opta is "[" the bracket content has been evaluated.
              // Now its time to check if there is either a function or a sign pending.
              // - Neither the opening nor the closing bracket will be pushed back to
              //   the operator stack
              // - Check if a function is standing in front of the opening bracket, 
              //   if so evaluate it afterwards to apply an infix operator.
              if ( stOpt.size() && stOpt.top()->GetCode()==cmIO )
              {
                m_rpn.Add(pTok);
                stOpt.pop(); // Take opening bracket from stack
                
                // It's time to apply the index operator
                MUP_ASSERT(stVal.size()>=2);
                ptr_val_type idx = stVal.pop();

                // added by CBG: skip all following checks and simply continue
                // parsing when just querying all variables
                if (m_bIsQueryingExprVar)
                  break;

                if (!idx->IsInteger()) // || idx->GetFloat()<0)
                  Error(ecTYPE_CONFLICT_IDX, m_pTokenReader->GetPos(), stVal.top().Get());

                ptr_val_type val = stVal.pop();
                if (!val->IsArray())
                  Error(ecNOT_AN_ARRAY, m_pTokenReader->GetPos(), val.Get()); 

                if ((idx->GetFloat() >= val->GetArray().size()) || (!(val->GetArray().isRingBuffer()) && idx->GetFloat() < 0))
                  Error(ecINDEX_OUT_OF_BOUNDS, m_pTokenReader->GetPos(), val.Get()); 

                if (val->AsValue()!=NULL)
                {
                  // Index of a value item
                  ptr_val_type pValue( new Value( (*val)[(int_type)idx->GetFloat()]) );
                  pValue->AddFlags(IToken::flVOLATILE);
                  stVal.push(pValue);
                }
                else
                {
                  // index of a variable item
                  IValue &v = (*val)[(int_type)idx->GetFloat()];
                  ptr_val_type pVar( new Variable(&v) );
                  stVal.push(pVar);
                }
              } // if opening index bracket is on top of operator stack
            }
            break;

      case  cmBC:
            {
              if (stVal.size()) // increase argument counter
                stArgTypes.top() += stVal.top()->GetType();

              ApplyBinAndTernaryOprt(stOpt, stVal);

              // if opt is ")" and opta is "(" the bracket content has been evaluated.
              // Now its time to check if there is either a function or a sign pending.
              // - Neither the opening nor the closing bracket will be pushed back to
              //   the operator stack
              // - Check if a function is standing in front of the opening bracket, 
              //   if so evaluate it afterwards to apply an infix operator.
              if ( stOpt.size() && stOpt.top()->GetCode()==cmBO )
              {
                //
                // Here is the stuff to evaluate a function token
                //
                string_type sProt = stArgTypes.pop();
                int iArgc = (int)sProt.length();

                stOpt.pop(); // Take opening bracket from stack
                if ( stOpt.empty() )
                  break;
                  
                if ( (stOpt.top()->GetCode()!=cmFUNC) && (stOpt.top()->GetCode()!=cmOPRT_INFIX) )
                  break;

                ICallback *pFun = stOpt.top()->AsICallback();
                stFunc.pop(); 

                if (pFun->GetArgc()!=-1 && iArgc > pFun->GetArgc())
                  Error(ecTOO_MANY_PARAMS, m_pTokenReader->GetPos(), pFun);

                if (iArgc < pFun->GetArgc())
                  Error(ecTOO_FEW_PARAMS, m_pTokenReader->GetPos(), pFun);

                // Evaluate the function
                ApplyFunc(stOpt, stVal, iArgc);

                // Apply an infix operator, if present
                if (stOpt.size() && stOpt.top()->GetCode()==cmOPRT_INFIX) 
                  ApplyFunc(stOpt, stVal, 1);
              }
            }
            break;

      case  cmEOE:
            ApplyBinAndTernaryOprt(stOpt, stVal);
            break;

      case  cmOPRT_BIN_OVERLOAD:
      case  cmOPRT_BIN:
            {
              while ( stOpt.size() && 
                      stOpt.top()->GetCode() != cmBO && 
                      stOpt.top()->GetCode() != cmIO && 
                      stOpt.top()->GetCode() != cmELSE &&
                      stOpt.top()->GetCode() != cmIF)
              {
                MUP_ASSERT(dynamic_cast<IOprtBin*>(pTok.Get()));  
                MUP_ASSERT(dynamic_cast<IOprtBin*>(stOpt.top().Get()));  
                IOprtBin *pOprt1 = static_cast<IOprtBin*>(stOpt.top().Get());  
                IOprtBin *pOprt2 = static_cast<IOprtBin*>(pTok.Get());  

                int nPrec1 = pOprt1->GetPri(),
                    nPrec2 = pOprt2->GetPri();

                if (pOprt1->GetCode()==pOprt2->GetCode())
                {
                  // Deal with operator associativity
                  EOprtAsct eOprtAsct = pOprt1->GetAssociativity();
                  if ( (eOprtAsct==oaRIGHT && (nPrec1 <= nPrec2)) || 
                       (eOprtAsct==oaLEFT  && (nPrec1 <  nPrec2)) )
                  {
                    break;
                  }
                }
                else if (nPrec1 < nPrec2)
                {
                  break;
                }
                
                // apply the operator now 
                // (binary operators are identic to functions with two arguments)
                ApplyFunc(stOpt, stVal, 2);
              } // while ( ... )

              stOpt.push(pTok);
            }
            break;

      //
      //  Postfix Operators
      //
      case  cmOPRT_POSTFIX:
            {
              ICallback *pOprt = pTok->AsICallback();
              MUP_ASSERT(stVal.size());
              #ifdef __GNUG__
              ptr_val_type &pVal(stVal.top());
              #else
			  //Sadjad: shorthand above is not understood by all compilers, so: 
			  ptr_val_type &pVal = ptr_val_type(stVal.top());
			  //S e
              #endif
              try
              {
                CheckArgTypes(&pVal, 1, pOprt);
                bool bOptimize = !pVal->IsFlagSet(IToken::flVOLATILE);
                
                ptr_val_type buf(new Value);
                pOprt->Eval(buf, &pVal, 1);
                pVal = buf;

                if (!bOptimize)
                {
                  pVal->AddFlags(IToken::flVOLATILE);
                  m_rpn.Add(pTok);
                }
                else
                {
                  m_rpn.Pop(1);
                  m_rpn.Add(ptr_tok_type(pVal.Get()));
                }
              }
              catch(ParserError &)
              {
                if (!m_bIsQueryingExprVar)
                  throw;
              }
            }
  		      break;

      case  cmIO:
      case  cmBO:    
            stOpt.push(pTok);
            stArgTypes.push(_T(""));
            break;

      case  cmIF:
            m_nIfElseCounter++;
            m_rpn.Add(pTok);
            stOpt.push(pTok);
            break;

      case  cmELSE:
            if (m_nIfElseCounter<1)
              Error(ecMISPLACED_COLON, m_pTokenReader->GetPos());

            ApplyBinAndTernaryOprt(stOpt, stVal);

            if (m_nIfElseCounter==0)
              Error(ecMISPLACED_COLON, m_pTokenReader->GetPos());

            m_rpn.Add(pTok);
            stOpt.push(pTok);
            break;

      //
      // Functions
      //
      case  cmOPRT_INFIX:
      case  cmFUNC:
            {
              ICallback *pFunc = pTok->AsICallback();
              MUP_ASSERT(pFunc);

              // Check if this function is a argument to another function
              // if so check if the the return type fits.
              if (!stFunc.empty() && stFunc.top()->GetCode()==cmFUNC)
              {
                MUP_ASSERT(stArgTypes.size());
                int iArgc = (int)stArgTypes.top().length() + 1;

                ICallback *pOuterFunc = stFunc.top();
                if (pOuterFunc->GetArgc()!=-1 && iArgc>pOuterFunc->GetArgc())
		              Error(ecTOO_MANY_PARAMS, m_pTokenReader->GetPos());

                MUP_ASSERT(pOuterFunc->GetArgc()==-1 || iArgc<=pOuterFunc->GetArgc());

                // The function we have here is the iArgc'th argument
                // of the function pointed to by pFunc.
                // Check for type conflicts.
                if (pOuterFunc->GetArgType(iArgc)!='.' &&
                    pOuterFunc->GetArgType(iArgc)!=pFunc->GetRetType())
                {
                  ErrorContext err;
                  err.Pos   = m_pTokenReader->GetPos();
                  err.Expr  = m_pTokenReader->GetExpr();
                  err.Errc  = ecTYPE_CONFLICT_FUN;
                  err.Ident = pOuterFunc->GetIdent();
                  err.Arg   = iArgc;
                  err.Type1 = pFunc->GetRetType();
                  err.Type2 = pOuterFunc->GetArgType(iArgc);
                  throw ParserError(err);
                }
              }

              stOpt.push(pTok);  
              stFunc.push(pFunc); // to collect runtime type information
            }
	          break;

      default: 
            Error(ecINTERNAL_ERROR);
      } // switch Code

#if defined(MUP_DUMP_STACK) 
      StackDump( stVal, stOpt );
#endif

      if ( pTok->GetCode() == cmEOE )
        bLoop = false;

#if defined(MUP_DUMP_CMDCODE)
      m_rpn.AsciiDump();
#endif
    } // for (all tokens)

  //#if defined(MUP_DUMP_CMDCODE)
  //  m_rpn.AsciiDump();
  //#endif

    if (m_nIfElseCounter>0)
      Error(ecMISSING_ELSE_CLAUSE);

    m_nFinalResultIdx = stArgTypes.top().length();

    // Umsachalten auf RPN
    m_vStackBuffer.assign(m_rpn.GetRequiredStackSize(), ptr_val_type());
    for (std::size_t i=0; i<m_vStackBuffer.size(); ++i)
    {
      Value *pValue = new Value;
      pValue->BindToCache(&m_cache);
      m_vStackBuffer[i].Reset(pValue);
    }

    m_pParserEngine = &ParserXBase::ParseFromRPN;

    // Return a reference to the result by its IValue interface,
    // the first position in m_vStackBuffer is merely used in order
    // to have a safe place to use fpr the returned const reference
    // It is later overwritten in the RPN parse function.
    MUP_ASSERT(stVal.size());
    m_BufferValue = stVal.top();
    return *(m_BufferValue.Get());
  } 

  //---------------------------------------------------------------------------
  const IValue& ParserXBase::ParseFromRPN() const
  {
    ptr_val_type *pStack = &m_vStackBuffer[0];
    const ptr_tok_type *pRPN = &(m_rpn.GetData()[0]);

    int sidx = -1;
    std::size_t lenRPN = m_rpn.GetSize();
    for (std::size_t i=0; i<lenRPN; ++i)
    {
      IToken *pTok = pRPN[i].Get();
      ECmdCode eCode = pTok->GetCode();

      switch (eCode)
      {
      case cmVAR:
           {
             sidx++;
             assert(sidx<(int)m_vStackBuffer.size());
             pStack[sidx].Reset(static_cast<IValue*>(pTok));
           }
           continue;

      case cmVAL:
           {
             sidx++;
             assert(sidx<(int)m_vStackBuffer.size());
             ptr_val_type &val = pStack[sidx];
             if (val->GetCode()==cmVAR)
               val.Reset(m_cache.CreateFromCache());

             *val = *(static_cast<IValue*>(pTok));
           }
           continue;

      case cmIC:
           {
             // apply the index operator
             ptr_val_type &idx = pStack[sidx--];
             ptr_val_type &val = pStack[sidx];
             if (val->GetCode()==cmVAR)
               val.Reset(new Variable( &(*val)[(int_type)idx->GetFloat()]));
             else
               *val = (*val)[(int_type)idx->GetFloat()];
           }
           continue;

      case cmOPRT_POSTFIX:
      case cmFUNC:
      case cmOPRT_BIN:
      case cmOPRT_INFIX:
      case cmOPRT_BIN_OVERLOAD:
           {
             ICallback *pFun = static_cast<ICallback*>(pTok);
             int nArgs = pFun->GetArgsPresent();
             sidx -= nArgs - 1;
             assert(sidx>=0);

             ptr_val_type &val = pStack[sidx];
             if (val->GetCode()==cmVAR)
             {
               ptr_val_type buf(m_cache.CreateFromCache());
               pFun->Eval(buf, &val, nArgs);
               val = buf;
             }
             else
               pFun->Eval(val, &val, nArgs);
           }
           continue;

      case cmIF:
           if (pStack[sidx--]->GetBool()==false)
             i+=static_cast<TokenIf*>(pTok)->GetOffset();
           continue;

      case cmELSE:
      case cmJMP:
           i += static_cast<TokenElse*>(pTok)->GetOffset();
           continue;

      case cmENDIF:
           continue;

			default:
					Error(ecINTERNAL_ERROR);
      } // switch token
    } // for all RPN tokens

    return *pStack[m_nFinalResultIdx];
  }

  //---------------------------------------------------------------------------
  /** \brief Verify the types in an argument vector to be passed 
             to a callback object. 
  */
  void ParserXBase::CheckArgTypes(const ptr_val_type *pArg, 
                                  int iNumArg, 
                                  ICallback *pFunc ) const
  {
    if (iNumArg==0 || pArg==NULL || pFunc->GetArgType(1)=='*')
      return;

    for (int iArg=0; iArg<iNumArg; ++iArg)
    {
      if (pFunc->GetArgType(iArg+1)=='.')
        continue;

      if (pFunc->GetArgType(iArg+1)==pArg[iArg]->GetType())
        continue;

      if (pFunc->GetArgType(iArg+1)=='f' && pArg[iArg]->GetType()=='i')
        continue;

      if (pFunc->GetArgType(iArg+1)=='c' && pArg[iArg]->GetType()=='f')
        continue;

      if (pFunc->GetArgType(iArg+1)=='c' && pArg[iArg]->GetType()=='i')
        continue;

      if (pArg[iArg]->GetType() != pFunc->GetArgType(iArg+1))
      {
        ErrorContext err;
        err.Pos     = m_pTokenReader->GetPos();
        err.Expr    = m_pTokenReader->GetExpr();
        err.Errc    = ecTYPE_CONFLICT_FUN;
        err.Ident   = pFunc->GetIdent();
        err.Arg     = iArg+1;
        err.Type1   = pArg[iArg]->GetType();
        err.Type2   = pFunc->GetArgType(err.Arg);
        throw ParserError(err);
      } // if argument conflict
    } // for all arguments
  }

  //---------------------------------------------------------------------------
  void  ParserXBase::Error(EErrorCodes a_iErrc, int a_iPos, const IToken *a_pTok) const
  {
    ErrorContext err;
    err.Errc = a_iErrc;
    err.Pos = a_iPos;
    err.Expr = m_pTokenReader->GetExpr();
    err.Ident = (a_pTok) ? a_pTok->GetIdent() : _T("");
    throw ParserError(err);
  }

  //------------------------------------------------------------------------------
  /** \brief Clear all user defined variables.
      \throw nothrow
      
      Resets the parser to string parsing mode by calling #ReInit.
  */
  void ParserXBase::ClearVar()
  {
    m_VarDef.clear();
    m_valDynVarShadow.clear();
    ReInit();
  }

  //------------------------------------------------------------------------------
  /** \brief Remove a variable from internal storage.
      \param a_strVarName Name of the variable to be removed.
      \throw nothrow

    Removes a variable if it exists. If the Variable does not exist 
    nothing will be done.
  */
  void ParserXBase::RemoveVar(const string_type &a_strVarName)
  {
    var_maptype::iterator item = m_VarDef.find(a_strVarName);
    if (item!=m_VarDef.end())
    {
      m_VarDef.erase(item);
      ReInit();
    }
  }

  //------------------------------------------------------------------------------
  /** \brief Clear the expression. 
      \throw nothrow

      Clear the expression and existing bytecode.
  */
  void ParserXBase::ClearExpr()
  {
    m_pTokenReader->SetExpr(_T(""));
    ReInit();
  }

  //------------------------------------------------------------------------------
  /** \brief Clear all function definitions. 
      \throw nothrow
  */
  void ParserXBase::ClearFun()
  {
    m_FunDef.clear();
    ReInit();
  }

  //------------------------------------------------------------------------------
  /** \brief Clear all user defined constants.
      \throw nothrow

      Both numeric and string constants will be removed from the internal storage.
  */
  void ParserXBase::ClearConst()
  {
    m_valConst.clear();
    ReInit();
  }

  //------------------------------------------------------------------------------
  /** \brief Clear all user defined postfix operators.
      \throw nothrow
  */
  void ParserXBase::ClearPostfixOprt()
  {
    m_PostOprtDef.clear();
    ReInit();
  }

  //------------------------------------------------------------------------------
  /** \brief Clear all user defined binary operators. 
      \throw nothrow
  */
  void ParserXBase::ClearOprt()
  {
    m_OprtDef.clear();
    ReInit();
  }

  //------------------------------------------------------------------------------
  /** \brief Clear the user defined Prefix operators. 
      \throw nothrow
  */
  void ParserXBase::ClearInfixOprt()
  {
    m_InfixOprtDef.clear();
    ReInit();
  }

  //------------------------------------------------------------------------------
  void ParserXBase::EnableAutoCreateVar(bool bStat)
  {
    m_bAutoCreateVar = bStat;
  }

  //------------------------------------------------------------------------------
  void ParserXBase::EnableOptimizer(bool bStat)
  {
    m_bEnableOptimization = bStat;
  }

  //------------------------------------------------------------------------------
  bool ParserXBase::IsAutoCreateVarEnabled() const
  {
    return m_bAutoCreateVar;
  }

#if defined(MUP_DUMP_STACK) 

  //------------------------------------------------------------------------------
  /** \brief Dump stack content. 

      This function is used for debugging only.
  */
  void ParserXBase::StackDump(const Stack<ptr_val_type> &a_stVal, 
			             			      const Stack<ptr_tok_type> &a_stOprt) const
  {
    using std::cout;
    Stack<ptr_tok_type>  stOprt(a_stOprt); 
    Stack<ptr_val_type>  stVal(a_stVal);

    string_type sInfo = _T("StackDump>  ");
    console() << _T("\n") << sInfo << _T("Value stack:\n");
    console() << sInfo;
    while ( !stVal.empty() ) 
    {
      ptr_val_type val = stVal.pop();
      console() << _T("  ") << *(val.Get()) << _T(" ") << ((val->AsValue()!=NULL) ? _T("(Val)") : _T("(Var)"));
    }

    if (stOprt.empty())
      console() << _T("\n") << sInfo << _T("Operator stack is empty.\n");
    else
      console() << _T("\n") << sInfo << _T("Operator stack:\n");

    while ( !stOprt.empty() )
    {
      ptr_tok_type tok = stOprt.pop();
      console() << sInfo << _T(" ") << g_sCmdCode[tok->GetCode()] << _T(" \"") << tok->GetIdent() << _T("\" \n");
    }

    console() <<endl;
  }

#endif

} // namespace mu
