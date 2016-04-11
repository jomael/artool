/** \file
    \brief Implementation of the unit test for muParserX.

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
#include "mpTest.h"
#include "mpValue.h"

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <complex>
#include <algorithm>

#define MUP_CONST_PI  3.141592653589793238462643
#define MUP_CONST_E   2.718281828459045235360287

//Sadjad
#if defined(_MSC_VER) && (_MSC_VER <= 1200)
	namespace std
	{
		typedef size_t size_t;
	}
#endif

MUP_NAMESPACE_START

//Sadjad: war über MUP_NAMESPACE_START
//using namespace std;
using std::endl;

  //---------------------------------------------------------------------------
  int ParserTester::c_iCount = 0;

  //---------------------------------------------------------------------------
  ParserTester::ParserTester()
    :m_vTestFun()
    ,m_stream(&console())
  {
    AddTest(&ParserTester::TestParserValue);
    AddTest(&ParserTester::TestUndefVar);
    AddTest(&ParserTester::TestErrorCodes);
    AddTest(&ParserTester::TestAPI);
    AddTest(&ParserTester::TestEqn);
    AddTest(&ParserTester::TestIfElse);
    AddTest(&ParserTester::TestStringFun);
    AddTest(&ParserTester::TestVector);
    AddTest(&ParserTester::TestBinOp);
    AddTest(&ParserTester::TestPostfix);
    AddTest(&ParserTester::TestInfix);
    AddTest(&ParserTester::TestMultiArg);
    AddTest(&ParserTester::TestNames);
    ParserTester::c_iCount = 0;
  }

  //---------------------------------------------------------------------------
  int ParserTester::TestUndefVar()
  {
    int iNumErr = 0;
    *m_stream << _T("testing implicit definition of undefined variables...");

    // Test 1: No variables defined, test detection of undefined variables
    {
      ParserX p;
      p.SetExpr(_T("a+b+c+d"));
      const mup::var_maptype &expr_var = p.GetExprVar();
      const mup::var_maptype &var = p.GetVar();

      // The expression contains 4 undefined variables
      if (expr_var.size()!=4)
        iNumErr++;

      // There are no variables defined
      if (var.size()!=0)
        iNumErr++;
    }

    // Test 2: Variables were defined explicitely, test detection of undefined variables
    {
      ParserX p;

      // Now define the variables and perform the check again
      Value vVarVal[] = { 1, 2, 3, 4};
      p.DefineVar( _T("a"), Variable(&vVarVal[0]) );
      p.DefineVar( _T("b"), Variable(&vVarVal[1]) );
      p.DefineVar( _T("c"), Variable(&vVarVal[2]) );
      p.DefineVar( _T("d"), Variable(&vVarVal[3]) );

      p.SetExpr(_T("a+b+c+d"));
      const mup::var_maptype &expr_var = p.GetExprVar();
      const mup::var_maptype &var = p.GetVar();

      // The expression contains 4 undefined variables
      if (expr_var.size()!=4)
        iNumErr++;

      // There are no variables defined
      if (var.size()!=4)
        iNumErr++;
    }

    // Test 3: Variables were defined implicitels, test detection of undefined variables
    {
      ParserX p;

      // Now define the variables and perform the check again
      p.EnableAutoCreateVar(true);

      p.SetExpr(_T("a+b+c+d"));
      const mup::var_maptype &expr_var = p.GetExprVar();
      const mup::var_maptype &var = p.GetVar();

      // The expression contains 4 undefined variables
      if (expr_var.size()!=4)
        iNumErr++;

      // There are no variables defined
      if (var.size()!=4)
        iNumErr++;
    }

    Assessment(iNumErr);
    return iNumErr;
  }

  //---------------------------------------------------------------------------
  int ParserTester::TestParserValue()
  {
    int iNumErr = 0;
    *m_stream << _T("testing parser value types...");
    
    // Define values and variables for each type
    Value bVal = true;
    Value iVal = 10;
    Value fVal = (float_type)3.14;
    Value sVal = string_type(_T("hello world"));
    Value sVal1 = _T("hello world");   // Test assignment from const char* to string
    Value cVal = cmplx_type(1,1);
    Value aVal(2,0);
    aVal[0] = (float_type)2.0;
    aVal[1] = (float_type)3.0;

    // Create a 3x3 matrix
    Value matrix(3, 0);
    matrix[0] = Value(3, 0);
    matrix[1] = Value(3, 0);
    matrix[2] = Value(3, 0);

    Variable bVar(&bVal), 
             iVar(&iVal), 
             fVar(&fVal), 
             sVar(&sVal),
             sVar1(&sVal1),
             cVar(&cVal),
             aVar(&aVal);

    // Check the value types
    try
    {
      // Test if matrix values do work
      if (!matrix.IsArray() || matrix.GetDim()!=3)
        iNumErr++;

      std::size_t sz = matrix.GetDim();
      for (std::size_t i=0; i<sz; ++i)
      {
        std::size_t dim_row = matrix[i].GetDim();
        if (dim_row!=3)
        {
          iNumErr++;
          console() << _T("\n  Array dimension mismatch in matrix row ") << i 
                    << _T(" (expected=3; dim=") << dim_row << _T(")");
        }
      }


      // test type checking of values
      if ( !iVal.IsScalar()  ||  iVal.IsArray()  || iVal.GetType()!='i')  iNumErr++;
      if ( !fVal.IsScalar()  ||  fVal.IsArray()  || fVal.GetType()!='f')  iNumErr++;
      if ( !cVal.IsScalar()  ||  cVal.IsArray()  || cVal.GetType()!='c')  iNumErr++;
      if (  aVal.IsScalar()  || !aVal.IsArray()  || aVal.GetType()!='a')  iNumErr++;
      if (  sVal.IsScalar()  ||  sVal.IsArray()  || sVal.GetType()!='s')  iNumErr++;
      if (  sVal1.IsScalar() ||  sVal1.IsArray() || sVal1.GetType()!='s') iNumErr++;
      if (  bVal.IsScalar()  ||  bVal.IsArray()  || bVal.GetType()!='b')  iNumErr++;

      // test type checking of variables
      if ( !iVar.IsScalar()  ||  iVar.IsArray()  || iVar.GetType()!='i')  iNumErr++;
      if ( !fVar.IsScalar()  ||  fVar.IsArray()  || fVar.GetType()!='f')  iNumErr++;
      if ( !cVar.IsScalar()  ||  cVar.IsArray()  || cVar.GetType()!='c')  iNumErr++;
      if (  aVar.IsScalar()  || !aVar.IsArray()  || aVar.GetType()!='a')  iNumErr++;
      if (  sVar.IsScalar()  ||  sVar.IsArray()  || sVar.GetType()!='s')  iNumErr++;
      if (  sVar1.IsScalar() ||  sVar1.IsArray() || sVar1.GetType()!='s') iNumErr++;
      if (  bVar.IsScalar()  ||  bVar.IsArray()  || bVar.GetType()!='b')  iNumErr++;

      // Test type identifier after calling an assignment operator
      
      // a double that can be mapped to an integer without rounding error will become one
      iVal = (float_type)10.0; 
      if (  iVar.GetType()!='i')  iNumErr++;  

      iVal = 10;
      if (  iVar.GetType()!='i')  iNumErr++;  

      // a complex number that can be mapped to an integer without rounding error will become one
      iVal = cmplx_type(10, 0);
      if (  iVar.GetType()!='i')  iNumErr++;  

      iVal = (float_type)10.1;
      if (  iVar.GetType()!='f')  iNumErr++;

      iVal = cmplx_type(10, 1);
      if (  iVar.GetType()!='c')  iNumErr++;

      iVal = _T("test");
      if (  iVar.GetType()!='s')  iNumErr++;

      iVal = string_type(_T("test"));
      if (  iVar.GetType()!='s')  iNumErr++;

      iVal = false;
      if (  iVar.GetType()!='b')  iNumErr++;
    }
    catch(...)
    {
      iNumErr++;
    }

    bool bError;

#define VALUE_THROWCHECK(VAR, FAIL, MEMBER) \
    bError = (FAIL);                        \
    try                                     \
    {                                       \
      VAR.MEMBER();                         \
    }                                       \
    catch(...)                              \
    {                                       \
      bError ^= true ;                      \
    }                                       \
    iNumErr += (bError) ? 1 : 0;            \
    c_iCount++;

    // Check if the getter functions really throw an exception 
    // when used with an incorrect value type
    // Case 1:  test float values
    VALUE_THROWCHECK(fVal, false, GetFloat)
    VALUE_THROWCHECK(fVal, false, GetImag)
    VALUE_THROWCHECK(fVal, true,  GetBool)
    VALUE_THROWCHECK(fVal, true,  GetString)
    VALUE_THROWCHECK(fVal, true,  GetArray)
    // for variables
    VALUE_THROWCHECK(fVar, false, GetFloat)
    VALUE_THROWCHECK(fVar, false, GetImag)
    VALUE_THROWCHECK(fVar, true,  GetBool)
    VALUE_THROWCHECK(fVar, true,  GetString)
    VALUE_THROWCHECK(fVar, true,  GetArray)

    // Case 2:  test bool values
    VALUE_THROWCHECK(bVal, false,  GetFloat)
    VALUE_THROWCHECK(bVal, true,  GetImag)
    VALUE_THROWCHECK(bVal, false, GetBool)
    VALUE_THROWCHECK(bVal, true,  GetString)
    VALUE_THROWCHECK(bVal, true,  GetArray)
    // for variables
    VALUE_THROWCHECK(bVar, false,  GetFloat)
    VALUE_THROWCHECK(bVar, true,  GetImag)
    VALUE_THROWCHECK(bVar, false, GetBool)
    VALUE_THROWCHECK(bVar, true,  GetString)
    VALUE_THROWCHECK(bVar, true,  GetArray)

    // Case 3:  test string values
    VALUE_THROWCHECK(sVal, true,  GetFloat)
    VALUE_THROWCHECK(sVal, true,  GetImag)
    VALUE_THROWCHECK(sVal, true,  GetBool)
    VALUE_THROWCHECK(sVal, false, GetString)
    VALUE_THROWCHECK(sVal, true,  GetArray)
    // for variables
    VALUE_THROWCHECK(sVar, true,  GetFloat)
    VALUE_THROWCHECK(sVar, true,  GetImag)
    VALUE_THROWCHECK(sVar, true,  GetBool)
    VALUE_THROWCHECK(sVar, false, GetString)
    VALUE_THROWCHECK(sVar, true,  GetArray)

    // Case 4:  test array values
    VALUE_THROWCHECK(aVal, true,  GetFloat)
    VALUE_THROWCHECK(aVal, true,  GetImag)
    VALUE_THROWCHECK(aVal, true,  GetBool)
    VALUE_THROWCHECK(aVal, true,  GetString)
    VALUE_THROWCHECK(aVal, false, GetArray)
    // for variables
    VALUE_THROWCHECK(aVar, true,  GetFloat)
    VALUE_THROWCHECK(aVar, true,  GetImag)
    VALUE_THROWCHECK(aVar, true,  GetBool)
    VALUE_THROWCHECK(aVar, true,  GetString)
    VALUE_THROWCHECK(aVar, false, GetArray)

    // Case 5:  test complex values
    VALUE_THROWCHECK(cVal, false,  GetFloat)
    VALUE_THROWCHECK(cVal, false,  GetImag)
    VALUE_THROWCHECK(cVal, true,   GetBool)
    VALUE_THROWCHECK(cVal, true,   GetString)
    VALUE_THROWCHECK(cVal, true,   GetArray)
    // for variables
    VALUE_THROWCHECK(cVar, false,  GetFloat)
    VALUE_THROWCHECK(cVar, false,  GetImag)
    VALUE_THROWCHECK(cVar, true,   GetBool)
    VALUE_THROWCHECK(cVar, true,   GetString)
    VALUE_THROWCHECK(cVar, true,   GetArray)
#undef VALUE_THROWCHECK

    Assessment(iNumErr);
    return iNumErr;
  }


//---------------------------------------------------------------------------
int ParserTester::TestErrorCodes()
{
  int  iNumErr = 0;
  *m_stream << _T("testing error codes...");

  // This is allowed now:
  //iNumErr += ThrowTest(_T("1,2"),   ecUNEXPECTED_COMMA);
  //iNumErr += ThrowTest(_T("(1,2)"), ecUNEXPECTED_COMMA);
  //iNumErr += ThrowTest(_T("2*1,2"), ecUNEXPECTED_COMMA);

  iNumErr += ThrowTest(_T("sin(1,2)"), ecTOO_MANY_PARAMS);

  // Invalid function argument types
  iNumErr += ThrowTest(_T("sin(\"test\")"),        ecTYPE_CONFLICT_FUN);
  iNumErr += ThrowTest(_T("max(1, \"test\")"),     ecTYPE_CONFLICT_FUN);
  iNumErr += ThrowTest(_T("max(1,sin(8), \"t\")"), ecTYPE_CONFLICT_FUN); 
  iNumErr += ThrowTest(_T("str2dbl(sin(3.14))"),   ecTYPE_CONFLICT_FUN); 

  // Invalid unary operator argument types
  iNumErr += ThrowTest(_T("\"test\"{n}"), ecTYPE_CONFLICT_FUN); // (nano can only be applied to floats)
  iNumErr += ThrowTest(_T("(1+3i)/(8*9i)+\"hallo\""), ecTYPE_CONFLICT_FUN); 
  iNumErr += ThrowTest(_T("(1+3i)/(8*9i)-\"hallo\""), ecTYPE_CONFLICT_FUN); 
  iNumErr += ThrowTest(_T("(1+3i)/(8*9i)*\"hallo\""), ecTYPE_CONFLICT_FUN); 
  iNumErr += ThrowTest(_T("(1+3i)/(8*9i)/\"hallo\""), ecTYPE_CONFLICT_FUN); 
  iNumErr += ThrowTest(_T("10+va"), ecTYPE_CONFLICT_FUN); 

  // Type conflicts in binary operators
  iNumErr += ThrowTest(_T("\"test\" // 8"), ecTYPE_CONFLICT_FUN);
  iNumErr += ThrowTest(_T("8//\"test\""),   ecTYPE_CONFLICT_FUN);
  iNumErr += ThrowTest(_T("5//8"),          ecTYPE_CONFLICT_FUN);
  iNumErr += ThrowTest(_T("\"t\"//sin(8)"), ecTYPE_CONFLICT_FUN);
  iNumErr += ThrowTest(_T("sin(8)//\"t\""), ecTYPE_CONFLICT_FUN);

  // Unexpected end of expression
  iNumErr += ThrowTest(_T("3+"),    ecUNEXPECTED_EOF);
  iNumErr += ThrowTest(_T("8*"),    ecUNEXPECTED_EOF);
  iNumErr += ThrowTest(_T("3+("),   ecUNEXPECTED_EOF);
  iNumErr += ThrowTest(_T("3+sin"), ecUNEXPECTED_EOF);
  iNumErr += ThrowTest(_T("(2+"),   ecUNEXPECTED_EOF); 

  iNumErr += ThrowTest(_T("3+)"),       ecUNEXPECTED_PARENS);
  iNumErr += ThrowTest(_T("3)"),        ecUNEXPECTED_PARENS);
  iNumErr += ThrowTest(_T("(3))"),      ecUNEXPECTED_PARENS);
  iNumErr += ThrowTest(_T("()"),        ecUNEXPECTED_PARENS);
  iNumErr += ThrowTest(_T("(2+)"),      ecUNEXPECTED_PARENS);
  iNumErr += ThrowTest(_T("sin(cos)"),  ecUNEXPECTED_PARENS);
  iNumErr += ThrowTest(_T("sin(())"),   ecUNEXPECTED_PARENS);
  iNumErr += ThrowTest(_T("sin()"),     ecTOO_FEW_PARAMS);
  iNumErr += ThrowTest(_T("sin)"),      ecUNEXPECTED_PARENS);
  iNumErr += ThrowTest(_T("pi)"),       ecUNEXPECTED_PARENS);
  iNumErr += ThrowTest(_T("a)"),        ecUNEXPECTED_PARENS);
  iNumErr += ThrowTest(_T("2(-m)"),     ecUNEXPECTED_PARENS);
  iNumErr += ThrowTest(_T("2(m)"),      ecUNEXPECTED_PARENS);

  iNumErr += ThrowTest(_T("(1+2"),      ecMISSING_PARENS);
  iNumErr += ThrowTest(_T("((3)"),      ecMISSING_PARENS);

  // The behaviour in the next function depends on whether
  // implicit variable creation is allowed or not. (momentarily its active)
  iNumErr += ThrowTest(_T("5z)"),       ecUNASSIGNABLE_TOKEN);
  iNumErr += ThrowTest(_T("sin(3)xyz"), ecUNASSIGNABLE_TOKEN);
  iNumErr += ThrowTest(_T("5t6"),       ecUNASSIGNABLE_TOKEN);
  iNumErr += ThrowTest(_T("5 t 6"),     ecUNASSIGNABLE_TOKEN);
  iNumErr += ThrowTest(_T("ksdfj"),     ecUNASSIGNABLE_TOKEN);

  // unexpected operator
  iNumErr += ThrowTest(_T("-{m}"),      ecUNEXPECTED_OPERATOR);
  iNumErr += ThrowTest(_T("{m}4"),      ecUNEXPECTED_OPERATOR);
  iNumErr += ThrowTest(_T("sin({m})"),  ecUNEXPECTED_OPERATOR);
  iNumErr += ThrowTest(_T("{m} {m}"),   ecUNEXPECTED_OPERATOR);
  iNumErr += ThrowTest(_T("{m}(8)"),    ecUNEXPECTED_OPERATOR);
  iNumErr += ThrowTest(_T("4 + {m}"),   ecUNEXPECTED_OPERATOR);
  iNumErr += ThrowTest(_T("5+*3)"),     ecUNEXPECTED_OPERATOR);

  // unexpected comma (used without a function)
  iNumErr += ThrowTest(_T(",3"),           ecUNEXPECTED_COMMA);
  // allowed now:
  //iNumErr += ThrowTest(_T("3,5"),          ecUNEXPECTED_COMMA);
  //iNumErr += ThrowTest(_T("(8,9)"),        ecUNEXPECTED_COMMA);
  iNumErr += ThrowTest(_T("sin(,sin(8))"), ecUNEXPECTED_COMMA);

  iNumErr += ThrowTest(_T("sin(3)cos(3)"), ecUNEXPECTED_FUN);
  iNumErr += ThrowTest(_T("sin(3)3"),      ecUNEXPECTED_VAL);
  
  // value recognition
  iNumErr += ThrowTest(_T("0x"),      ecUNASSIGNABLE_TOKEN);  // incomplete hex value
  iNumErr += ThrowTest(_T("1+0x"),    ecUNASSIGNABLE_TOKEN);  // incomplete hex value  
  iNumErr += ThrowTest(_T("a+0x"),    ecUNASSIGNABLE_TOKEN);  // incomplete hex value  

  // index operator
  iNumErr += ThrowTest(_T("min(3,]"), ecUNEXPECTED_SQR_BRACKET);
  iNumErr += ThrowTest(_T("sin(]"),   ecUNEXPECTED_SQR_BRACKET);
  iNumErr += ThrowTest(_T("va[]"),    ecUNEXPECTED_SQR_BRACKET);
  iNumErr += ThrowTest(_T("3+]"),     ecUNEXPECTED_SQR_BRACKET);
  iNumErr += ThrowTest(_T("sin[a)"),  ecUNEXPECTED_SQR_BRACKET);
  iNumErr += ThrowTest(_T("1+[8]"),   ecUNEXPECTED_SQR_BRACKET);
  iNumErr += ThrowTest(_T("1[8]"),    ecUNEXPECTED_SQR_BRACKET);
  iNumErr += ThrowTest(_T("3{n}[1]"), ecUNEXPECTED_SQR_BRACKET);
  iNumErr += ThrowTest(_T("[1]"),     ecUNEXPECTED_SQR_BRACKET);
  iNumErr += ThrowTest(_T("]1"),      ecUNEXPECTED_SQR_BRACKET);
  iNumErr += ThrowTest(_T("va[[3]]"), ecUNEXPECTED_SQR_BRACKET);

  Assessment(iNumErr);
  return iNumErr;
}


//---------------------------------------------------------------------------
int ParserTester::TestAPI()
{
  int  iStat = 0;
  *m_stream << _T("testing API exceptions...");

  // We need to test for API errors, there is no generic way to do 
  // this... The following class will create an API_PROTOTYPE error.
  // But first we need to create a function object that causes the error.
  struct FooFun : public ICallback
  {
    FooFun(const char_type *a_szArgDef) 
      :ICallback(cmFUNC, _T("sin"), a_szArgDef)
    {}

    void Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc) 
    { 
      *ret = 0;
    }
    
    const char_type* GetDesc() const 
    {
      return _T(""); 
    }
    
    IToken* Clone() const 
    { 
      return NULL;
    }
  };

#define API_CHECK(STRING, FAIL)                        \
  try                                                  \
  {                                                    \
    ParserTester::c_iCount++;                          \
    ParserX p;                                         \
    p.DefineFun(new FooFun(STRING));                   \
    if (FAIL)                                          \
    {                                                  \
      iStat++;                                         \
      *m_stream << _T("\n  Failed for function prototype: ") << STRING;    \
    }                                                  \
  }                                                    \
  catch(ParserError &e)                                \
  {                                                    \
    if (e.GetCode()!=ecAPI_INVALID_PROTOTYPE || !FAIL) \
    {                                                  \
      iStat++;                                         \
      *m_stream << _T("\n  Failed for function prototype: ") << STRING;    \
    }                                                  \
  }

  // invalid prototypes
  // 2nd Parameter true => expected to fail
  API_CHECK( _T("a,jhu"),   true ) // invalid characters / format
  API_CHECK( _T("x:fff"),   true ) // invalid character for return value
  API_CHECK( _T("f:xyzdf"), true ) // invalid character for argument values
  API_CHECK( _T("xsdee"),   true ) // invalid characters / no ":"
  API_CHECK( _T("ff:f"),    true ) // to many return types
  API_CHECK( _T("ffsf"),    true ) // no return type / invalid characters
  API_CHECK( _T("iiii"),    true ) // no return type
  API_CHECK( _T("i:"),      true ) // no argument definition
  API_CHECK( _T(".:..."),   true ) // undefined return value
  API_CHECK( _T("v:vv"),    true )
  API_CHECK( _T("v:ffv"),   true )
  API_CHECK( _T("v:fvi"),   true )
  // next are valid prototypes
  API_CHECK( _T("v:v"),     false )
  API_CHECK( _T("v:ff"),    false )
  API_CHECK( _T("f:fff"),   false )
  API_CHECK( _T("f:..."),   false )
  API_CHECK( _T("i:*"),     false )  
  API_CHECK( _T("s:*"),     false )  
  API_CHECK( _T("f:*"),     false )  
  API_CHECK( _T("b:*"),     false )  
  API_CHECK( _T("f:fsi"),   false )
  API_CHECK( _T("i:iiiii"), false )
  API_CHECK( _T("s:sss"),   false )
  API_CHECK( _T("s:bif"),   false )
  API_CHECK( _T("s:."),     false )
  API_CHECK( _T("s:.."),    false )
  API_CHECK( _T("s:..."),   false )
  API_CHECK( _T("s:...."),  false )
#undef API_CHECK


  //
  // Finally we need to test for API errors, there is no generic way to do 
  // this... The following class will create an API_PROTOTYPE error
  struct FooOp : public IOprtBin
  {
    //-----------------------------------------------------------------------
    FooOp(const char_type *a_szArgDef) 
      :IOprtBin(_T("myop"), a_szArgDef, 3, oaLEFT)
    {}

    //-----------------------------------------------------------------------
    void Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc) 
    { 
      *ret = 0;
    }
    
    //-----------------------------------------------------------------------
    const char_type* GetDesc() const 
    {
      return _T(""); 
    }
    
    //-----------------------------------------------------------------------
    IToken* Clone() const 
    { 
      return NULL; 
    }
  };

#define API_CHECK(STRING, FAIL)                        \
  try                                                  \
  {                                                    \
    ParserTester::c_iCount++;                          \
    ParserX p;                                         \
    p.DefineOprt(new FooOp(STRING));                   \
    if (FAIL)                                          \
    {                                                  \
      iStat++;                                         \
      *m_stream << _T("\n  Failed for operator prototype: ") << STRING;    \
    }                                                  \
  }                                                    \
  catch(ParserError &e)                                \
  {                                                    \
    if (e.GetCode()!=ecAPI_INVALID_PROTOTYPE || !FAIL) \
    {                                                  \
      iStat++;                                         \
      *m_stream << _T("\n  Failed for operator prototype: ") << STRING;    \
    }                                                  \
  }

  // invalid prototypes
  // 2nd Parameter true => expected to fail
  API_CHECK( _T("a,jhu"),   true )  // invalid characters / format
  API_CHECK( _T("x:fff"),   true )  // invalid character for return value
  API_CHECK( _T("f:xyzdf"), true )  // invalid character for argument values
  API_CHECK( _T("xsdee"),   true )  // invalid characters / no ":"
  API_CHECK( _T("ff:f"),    true )  // to many return types
  API_CHECK( _T("ffsf"),    true )  // no return type / invalid characters
  API_CHECK( _T("iiii"),    true )  // no return type
  API_CHECK( _T("i:"),      true )  // no argument definition
  API_CHECK( _T(".:.."),    true )  // undefined return value
  API_CHECK( _T("i:*"),     true )  
  API_CHECK( _T("s:*"),     true )  
  API_CHECK( _T("f:*"),     true )  
  API_CHECK( _T("b:*"),     true )  
  // next are valid prototypes for operators
  API_CHECK( _T("f:is"),    false )
  API_CHECK( _T("s:si"),    false )
  API_CHECK( _T("i:si"),    false )
  API_CHECK( _T("b:.."),    false )
  API_CHECK( _T("f:ii"),    false )
  API_CHECK( _T("f:ss"),    false )
  API_CHECK( _T("f:ff"),    false )
  API_CHECK( _T("s:ss"),    false )
  API_CHECK( _T("i:ii"),    false )
#undef API_CHECK

  if (iStat==0) 
    *m_stream << "passed" << endl;
  else 
    *m_stream << "\n  failed with " << iStat << " errors" << endl;

  return iStat;
}

//---------------------------------------------------------------------------
int ParserTester::TestStringFun()
{
  int  iNumErr = 0;
  *m_stream << _T("testing string functions...");

  iNumErr += EqnTest(_T("strlen(\"12345\")"), 5, true);
  iNumErr += EqnTest(_T("strlen(toupper(\"abcde\"))"), 5, true);
  iNumErr += EqnTest(_T("sin(0)+(float)strlen(\"12345\")"), (float_type)5.0, true);
  iNumErr += EqnTest(_T("10*(float)strlen(toupper(\"12345\"))"), (float_type)50.0, true);
  iNumErr += EqnTest(_T("\"hello \"//\"world\""), string_type(_T("hello world")), true);
  iNumErr += EqnTest(_T("toupper(\"hello \")//\"world\""), string_type(_T("HELLO world")), true);
  iNumErr += EqnTest(_T("\"hello \"//toupper(\"world\")//\" !!!\""), string_type(_T("hello WORLD !!!")), true);

  Assessment(iNumErr);

  return iNumErr;
}

//---------------------------------------------------------------------------
int ParserTester::TestPostfix()
{
  int iNumErr = 0;
  *m_stream << _T("testing postfix operators...");

  // application
  iNumErr += EqnTest(_T("8{n}"), (float_type)8e-9, true);
  iNumErr += EqnTest(_T("8{n}"), (float_type)123.0, false);
  iNumErr += EqnTest(_T("3{m}+5"), (float_type)5.003, true);
  iNumErr += EqnTest(_T("1000{m}"), (float_type)1.0, true);
  iNumErr += EqnTest(_T("1000 {m}"), (float_type)1.0, true);
  iNumErr += EqnTest(_T("a{m}"), (float_type)1e-3, true);
  iNumErr += EqnTest(_T("(a){m}"), (float_type)1e-3, true);
  iNumErr += EqnTest(_T("-(a){m}"), (float_type)-1e-3, true);
  iNumErr += EqnTest(_T("-a{m}"), (float_type)-1e-3, true);
  iNumErr += EqnTest(_T("-2{m}"), (float_type)-2e-3, true);
  iNumErr += EqnTest(_T("a++b"), 3, true);
  iNumErr += EqnTest(_T("a ++ b"), 3, true);
  iNumErr += EqnTest(_T("1++2"), 3, true);
  iNumErr += EqnTest(_T("1 ++ 2"), 3, true);
  //iNumErr += EqnTest(_T("f1of1(1000)m"), 1, true);
  //iNumErr += EqnTest(_T("-f1of1(1000)m"), -1, true);
  //iNumErr += EqnTest(_T("-f1of1(-1000)m"), 1, true);
  //iNumErr += EqnTest(_T("f4of4(0,0,0,1000)m"), 1, true);
  iNumErr += EqnTest(_T("2+(a*1000){m}"), (float_type)3.0, true);
  // some incorrect results
  iNumErr += EqnTest(_T("1000{m}"), (float_type)0.1, false);
  iNumErr += EqnTest(_T("(a){m}"), (float_type)2.0, false);

  Assessment(iNumErr);
  return iNumErr;
}

//---------------------------------------------------------------------------
int ParserTester::TestInfix()
{
  int iNumErr = 0;
  *m_stream << _T("testing parser reader implementations...");

  iNumErr += EqnTest(_T("-1"),    (float_type)-1.0, true);
  iNumErr += EqnTest(_T("-(-1)"),  (float_type)1.0, true);
  iNumErr += EqnTest(_T("-(-1)*2"),  (float_type)2.0, true);
  iNumErr += EqnTest(_T("-(-2)*sqrt(4)"),  (float_type)4.0, true);
  iNumErr += EqnTest(_T("-a"),  (float_type)-1.0, true);
  iNumErr += EqnTest(_T("-(a)"),  (float_type)-1.0, true);
  iNumErr += EqnTest(_T("-(-a)"),  (float_type)1.0, true);
  iNumErr += EqnTest(_T("-(-a)*2"),  (float_type)2.0, true);
  iNumErr += EqnTest(_T("-(8)"), (float_type)-8.0, true);
  iNumErr += EqnTest(_T("-8"), (float_type)-8.0, true);
  iNumErr += EqnTest(_T("-(2+1)"), (float_type)-3.0, true);
  //iNumErr += EqnTest("-(f1of1(1+2*3)+1*2)", -9.0, true);
  //iNumErr += EqnTest("-(-f1of1(1+2*3)+1*2)", 5.0, true);
  iNumErr += EqnTest(_T("-sin(8)"), (float_type)-0.989358, true);
  iNumErr += EqnTest(_T("-sin(8)"),  (float_type)0.989358, false);
  iNumErr += EqnTest(_T("3-(-a)"), (float_type)4.0, true);
  iNumErr += EqnTest(_T("3--a"), (float_type)4.0, true);
  iNumErr += EqnTest(_T("2++4"), (float_type)6.0, true);
  iNumErr += EqnTest(_T("--1"), (float_type)1.0, true);

  // Postfix / infix priorities
  //iNumErr += EqnTest("~2#", 12, true);
  //iNumErr += EqnTest("~f1of1(2)#", 12, true);
  //iNumErr += EqnTest("~(b)#", 12, true);
  //iNumErr += EqnTest("(~b)#", 12, true);
  //iNumErr += EqnTest("~(2#)", 8, true);
  //iNumErr += EqnTest("~(f1of1(2)#)", 8, true);

  Assessment(iNumErr);
  return iNumErr;
}

//---------------------------------------------------------------------------
int ParserTester::TestMultiArg()
{
  int iNumErr = 0;
  *m_stream << _T("testing multiarg functions...");

  // application
  iNumErr += EqnTest(_T("max(1,8,9,(int)6)"), (float_type)9.0, true);
  iNumErr += EqnTest(_T("max((int)6, 1+2, 4, -9)"), (float_type)6.0, true);
  iNumErr += EqnTest(_T("min((int)6, 1+2, 4, -9)"), (float_type)-9.0, true);

  Assessment(iNumErr);
  return iNumErr;
}

//---------------------------------------------------------------------------
int ParserTester::TestVector()
{
  int  iNumErr = 0;
  *m_stream << _T("testing vector operations...");

  // Vector operations
  iNumErr += ThrowTest(_T("10+2*va"),    ecTYPE_CONFLICT_FUN);   // fail: number + vector
  iNumErr += ThrowTest(_T("10+va*2"),    ecTYPE_CONFLICT_FUN);   // fail: number + vector
  iNumErr += ThrowTest(_T("va+vc"),      ecARRAY_SIZE_MISMATCH); // fail: vectors of different size
  iNumErr += ThrowTest(_T("va-vc"),      ecARRAY_SIZE_MISMATCH); // fail: vectors of different size
  iNumErr += ThrowTest(_T("va*vc"),      ecARRAY_SIZE_MISMATCH); // fail: vectors of different size
  iNumErr += ThrowTest(_T("va[-1]"),     ecTYPE_CONFLICT_IDX);   // fail: negative value used as an index
  iNumErr += ThrowTest(_T("va[1.23]"),   ecTYPE_CONFLICT_IDX);   // fail: float value used as index
  iNumErr += ThrowTest(_T("va[sin(8)]"), ecTYPE_CONFLICT_IDX);   // fail: float value used as index
  iNumErr += ThrowTest(_T("va[c]"),      ecINDEX_OUT_OF_BOUNDS);
  iNumErr += ThrowTest(_T("va[(3)]"),    ecINDEX_OUT_OF_BOUNDS);
  iNumErr += ThrowTest(_T("a[1]"),       ecNOT_AN_ARRAY);   // fail: number + vector

  iNumErr += EqnTest(_T("va*vb"), 16, true);
  iNumErr += EqnTest(_T("va*va"), 14, true);
  iNumErr += EqnTest(_T("(va*vb)*b"), 32, true);
  iNumErr += EqnTest(_T("vb[va[0]]"), 3, true);
  iNumErr += EqnTest(_T("m1[0][0]+m1[1][1]+m1[2][2]"), 3, true);
  iNumErr += EqnTest(_T("vb[m1[0][0]]"), 3, true);

  iNumErr += EqnTest(_T("m1[0][0]=2"), 2, true);
  iNumErr += EqnTest(_T("m1[1][1]=2"), 2, true);
  iNumErr += EqnTest(_T("m1[2][2]=2"), 2, true);
  iNumErr += EqnTest(_T("va[0]=12.3"), (float_type)12.3, true);
  iNumErr += EqnTest(_T("va[1]=12.3"), (float_type)12.3, true);
  iNumErr += EqnTest(_T("va[2]=12.3"), (float_type)12.3, true);

  iNumErr += EqnTest(_T("va[0]"), 1, true);
  iNumErr += EqnTest(_T("va[1]"), 2, true);
  iNumErr += EqnTest(_T("va[2]"), 3, true);
  iNumErr += EqnTest(_T("(va[2])"), 3, true);
  iNumErr += EqnTest(_T("va[a]"), 2, true);
  iNumErr += EqnTest(_T("(va[a])"), 2, true);
  iNumErr += EqnTest(_T("va[b]"), 3, true);
  iNumErr += EqnTest(_T("va[(2)]"), 3, true);
  iNumErr += EqnTest(_T("va[-(-2)]"), 3, true);
  iNumErr += EqnTest(_T("(va[(2)])"), 3, true);
  iNumErr += EqnTest(_T("(va[-(-2)])"), 3, true);
  iNumErr += EqnTest(_T("va[1+1]"), 3, true);
  iNumErr += EqnTest(_T("va[(int)sin(8)+1]"), 2, true);

  iNumErr += EqnTest(_T("va[2]+4"), 7, true);
  iNumErr += EqnTest(_T("4+va[2]"), 7, true);
  iNumErr += EqnTest(_T("va[2]*4"), 12, true);
  iNumErr += EqnTest(_T("4*va[2]"), 12, true);
  iNumErr += EqnTest(_T("va[2]+a"), 4, true);
  iNumErr += EqnTest(_T("a+va[2]"), 4, true);
  iNumErr += EqnTest(_T("va[2]*b"), 6, true);
  iNumErr += EqnTest(_T("b*va[2]"), 6, true);

  //Value buf(3,0);
  //buf[0] = 3;
  //buf[1] = 4;
  //buf[2] = 5;
  //iNumErr += EqnTest(_T("vx+vy"), buf, true);

  Assessment(iNumErr);
  return iNumErr;
}

//---------------------------------------------------------------------------
int ParserTester::TestBinOp()
{
  int  iNumErr = 0;
  *m_stream << _T("testing binary operators...");

  // standard aperators
  iNumErr += EqnTest(_T("1+7"),   (float_type)8.0, true);
  iNumErr += EqnTest(_T("10-1"),  (float_type)9.0, true);
  iNumErr += EqnTest(_T("3*4"),  (float_type)12.0, true);
  iNumErr += EqnTest(_T("10/2"),  (float_type)5.0, true);
  // operator associativity
  iNumErr += EqnTest(_T("2^2^3"),  (float_type)256.0, true);
  iNumErr += EqnTest(_T("3+4*2/(1-5)^2^3"), (float_type)3.0001220703125, true); 
  iNumErr += EqnTest(_T("1/2/3"), (float_type)1.0/(float_type)6.0, true); 

  // operator precedencs
  iNumErr += EqnTest(_T("1+2-3*4/5^6"), (float_type)2.99923, true);
  iNumErr += EqnTest(_T("a+b-c*4/5^6"), (float_type)2.99923, true);
  iNumErr += EqnTest(_T("1^2/3*4-5+6"), (float_type)2.3333, true);
  iNumErr += EqnTest(_T("a^b/c*4-5+6"), (float_type)2.3333, true);
  iNumErr += EqnTest(_T("1+2*3"), (float_type)7.0, true);
  iNumErr += EqnTest(_T("a+b*c"), (float_type)7.0, true);
  iNumErr += EqnTest(_T("(1+2)*3"), (float_type)9.0, true);
  iNumErr += EqnTest(_T("(a+b)*c"), (float_type)9.0, true);
  iNumErr += EqnTest(_T("(1+2)*(-3)"), (float_type)-9.0, true);
  iNumErr += EqnTest(_T("(a+b)*(-c)"), (float_type)-9.0, true);
  iNumErr += EqnTest(_T("2/4"), (float_type)0.5, true);
  iNumErr += EqnTest(_T("4&4"), 4, true);
  iNumErr += EqnTest(_T("2+2&(a+b+c)"), 4, true);
  iNumErr += EqnTest(_T("3&3"), 3, true);
  iNumErr += EqnTest(_T("c&3"), 3, true);
  iNumErr += EqnTest(_T("(c)&3"), 3, true);
  iNumErr += EqnTest(_T("(a+b)&3"), 3, true);
  iNumErr += EqnTest(_T("(a+b+c)&6"), 6, true);
  iNumErr += EqnTest(_T("(1+2+3)&6"), 6, true);
  iNumErr += EqnTest(_T("3&c"), 3, true);
  iNumErr += EqnTest(_T("(a<<1)+2"), 4, true);
  iNumErr += EqnTest(_T("(a<<2)+2"), 6, true);
  iNumErr += EqnTest(_T("(a<<3)+2"), 10, true);
  iNumErr += EqnTest(_T("(a<<4)+2"), 18, true);
  iNumErr += EqnTest(_T("(a<<5)+2"), 34, true);

  // bool operators for comparing values
  iNumErr += EqnTest(_T("a<b"), true, true);
  iNumErr += EqnTest(_T("b>a"), true, true);
  iNumErr += EqnTest(_T("a>a"), false, true);
  iNumErr += EqnTest(_T("a<a"), false, true);
  iNumErr += EqnTest(_T("a>a"), false, true);
  iNumErr += EqnTest(_T("a<=a"), true, true);
  iNumErr += EqnTest(_T("a<=b"), true, true);
  iNumErr += EqnTest(_T("b<=a"), false, true);
  iNumErr += EqnTest(_T("a>=a"), true, true);
  iNumErr += EqnTest(_T("b>=a"), true, true);
  iNumErr += EqnTest(_T("a>=b"), false, true);

  // interaction with sign operator
  iNumErr += EqnTest( _T("3-(-a)"), 4, true);
  iNumErr += EqnTest( _T("3--a"), 4, true);

  Assessment(iNumErr);
  return iNumErr;
}

//---------------------------------------------------------------------------
int ParserTester::TestIfElse()
{
  int  iNumErr = 0;
  *m_stream << _T("testing if-else conditional...");

  // Test error detection
  iNumErr += ThrowTest(_T("(a<b) ? (b<c) ? 1 : 2"), ecMISSING_ELSE_CLAUSE); 
  iNumErr += ThrowTest(_T("(a<b) ? 1"), ecMISSING_ELSE_CLAUSE); 
  iNumErr += ThrowTest(_T("(a<b) ? a"), ecMISSING_ELSE_CLAUSE); 
  iNumErr += ThrowTest(_T("(a<b) ? a+b"), ecMISSING_ELSE_CLAUSE); 
  iNumErr += ThrowTest(_T("a : b"), ecMISPLACED_COLON); 
  iNumErr += ThrowTest(_T("1 : 2"), ecMISPLACED_COLON); 
  iNumErr += ThrowTest(_T("(true) ? 1 : 2 : 3"), ecMISPLACED_COLON); 

  iNumErr += EqnTest(_T("(true) ? 10 : 11"), 10, true);
  iNumErr += EqnTest(_T("(false) ? 10 : 11"), 11, true);
  iNumErr += EqnTest(_T("(true) ? a+b : c+d"), 3, true);
  iNumErr += EqnTest(_T("(false) ? a+b : c+d"), 1, true);
  iNumErr += EqnTest(_T("(true) ? false : true"), false, true);
  iNumErr += EqnTest(_T("(false) ? false : true"), true, true);
  iNumErr += EqnTest(_T("(a<b) ? 10 : 11"), 10, true);
  iNumErr += EqnTest(_T("(a>b) ? 10 : 11"), 11, true);
  iNumErr += EqnTest(_T("(a<b) ? c : d"), 3, true);
  iNumErr += EqnTest(_T("(a>b) ? c : d"), -2, true);
  iNumErr += EqnTest(_T("sum((a>b) ? 1 : 2)"), 2, true);
  iNumErr += EqnTest(_T("sum((true) ? 1 : 2)"), 1, true);
  iNumErr += EqnTest(_T("sum((a>b) ? 1 : 2, 100)"), 102, true);
  iNumErr += EqnTest(_T("sum((true) ? 1 : 2, 100)"), 101, true);
  iNumErr += EqnTest(_T("sum(3, (a>b) ? 3 : 10)"), 13, true);
  iNumErr += EqnTest(_T("sum(3, (a<b) ? 3 : 10)"), 6, true);
  iNumErr += EqnTest(_T("10*sum(3, (a>b) ? 3 : 10)"), 130, true);
  iNumErr += EqnTest(_T("10*sum(3, (a<b) ? 3 : 10)"), 60, true);
  iNumErr += EqnTest(_T("sum(3, (a>b) ? 3 : 10)*10"), 130, true);
  iNumErr += EqnTest(_T("sum(3, (a<b) ? 3 : 10)*10"), 60, true);
  iNumErr += EqnTest(_T("(a<b) ? sum(3, (a<b) ? 3 : 10)*10 : 99"), 60, true);
  iNumErr += EqnTest(_T("(a>b) ? sum(3, (a<b) ? 3 : 10)*10 : 99"), 99, true);
  iNumErr += EqnTest(_T("(a<b) ? sum(3, (a<b) ? 3 : 10,10,20)*10 : 99"), 360, true);
  iNumErr += EqnTest(_T("(a>b) ? sum(3, (a<b) ? 3 : 10,10,20)*10 : 99"), 99, true);

#ifdef MUP_NICE_TO_HAVE
  iNumErr += EqnTest(_T("(true) ? a=10:a=20,a"), 10, true);
  iNumErr += EqnTest(_T("(false) ? a=10:a=20,a"), 20, true);
  iNumErr += EqnTest(_T("(true) ? a=a+1:a=a+2,a"), 2, true);
  iNumErr += EqnTest(_T("(false) ? a=a+1:a=a+2,a"), 3, true);
#endif

  // nested conditionals with brackets
  iNumErr += EqnTest(_T("(a<b) ? ((b<c) ? 2*(a+b) : 2) : 3"), 6, true);
  iNumErr += EqnTest(_T("(a<b) ? 3 : ((b<c) ? 2*(a+b) : 2)"), 3, true);
  iNumErr += EqnTest(_T("(a<b) ? ((b>c) ? 1 : 2) : 3"), 2, true);
  iNumErr += EqnTest(_T("(a>b) ? ((b<c) ? 1 : 2) : 3"), 3, true);
  iNumErr += EqnTest(_T("(a>b) ? ((b>c) ? 1 : 2) : 3"), 3, true);

  // nested conditionals without brackets
  iNumErr += EqnTest(_T("(a<b) ? (b<c) ? 1 : 2 : 3"), 1, true);
  iNumErr += EqnTest(_T("(a<b) ? (b>c) ? 1 : 2 : 3"), 2, true);
  iNumErr += EqnTest(_T("(a>b) ? (b<c) ? 1 : 2 : 3"), 3, true);
  iNumErr += EqnTest(_T("(a>b) ? (b>c) ? 1 : 2 : 3"), 3, true);

  Assessment(iNumErr);
  return iNumErr;
}

//---------------------------------------------------------------------------
int ParserTester::TestEqn()
{
  int  iNumErr = 0;
  *m_stream << _T("testing sample equations...");

  iNumErr += EqnTest(_T("-2--8"), (float_type)6.0, true);
  iNumErr += EqnTest(_T("2*(a=9)*3"), 54, true);

  // Functions
  iNumErr += EqnTest(_T("10*strlen(toupper(\"12345\"))"), 50, true);

  // complex numbers
  iNumErr += EqnTest(_T("i*i"), -1, true, 0);
  iNumErr += EqnTest(_T("norm(3+4i)"), 25, true, 0);
  iNumErr += EqnTest(_T("norm(4i+3)"), 25, true, 0);
  iNumErr += EqnTest(_T("norm(3i+4)"), 25, true, 0);
  iNumErr += EqnTest(_T("real(4.1i+3.1)"), (float_type)3.1, true, 0);
  iNumErr += EqnTest(_T("imag(3.1i+4.1)"), (float_type)3.1, true, 0);
  iNumErr += EqnTest(_T("real(3.1)"),  (float_type)3.1, true, 0);
  iNumErr += EqnTest(_T("imag(2.1i)"), (float_type)2.1, true, 0);
  iNumErr += EqnTest(_T("-(4i+5)"), cmplx_type(-5, -4), true, 0);
  iNumErr += EqnTest(_T("sqrt(-1)"), cmplx_type(0, 1), true, 0);
  iNumErr += EqnTest(_T("sqrt(i*i)"), cmplx_type(0, 1), true, 0);
  iNumErr += EqnTest(_T("sqrt(f)"), cmplx_type(0, 1), true, 1);
  iNumErr += EqnTest(_T("sqrt(2-3)"), cmplx_type(0, 1), true, 0);
  iNumErr += EqnTest(_T("sqrt(a-b)"), cmplx_type(0, 1), true, 2);
  iNumErr += EqnTest(_T("sqrt((2-3))"), cmplx_type(0, 1), true, 0);
  iNumErr += EqnTest(_T("sqrt((a-b))"), cmplx_type(0, 1), true, 2);
  iNumErr += EqnTest(_T("sqrt(-(1))"), cmplx_type(0, 1), true, 0);
  iNumErr += EqnTest(_T("sqrt((-1))"), cmplx_type(0, 1), true, 0);
  iNumErr += EqnTest(_T("sqrt(-(-1))"), cmplx_type(1, 0), true, 0);
  iNumErr += EqnTest(_T("sqrt(1)"), cmplx_type(1, 0), true, 0);
  iNumErr += EqnTest(_T("a=1+2i"), cmplx_type(1, 2), true, 1);
  iNumErr += EqnTest(_T("-(1+2i)"), cmplx_type(-1, -2), true, 0);
  iNumErr += EqnTest(_T("-(-1-2i)"), cmplx_type(1, 2), true, 0);
  iNumErr += EqnTest(_T("-(a+b*i)"), cmplx_type(-1, -2), true, 2);
  iNumErr += EqnTest(_T("-(-a-b*i)"), cmplx_type(1, 2), true, 2);

  // Test assignment operator with complex numbers. This needs to be written
  // in a bit of an obfuscated way in order to make the test work. EqnTest
  // computes the expression multiple times which would assign the variable
  // multiple times with different values. This would cause false positives
  // in the fail count. The result of the test expression is assigned to the
  // variable c, the original variable is then reset and finally the value
  // in c is used as the expression result...
  iNumErr += EqnTest(_T("c=(a+=1+2i), a=1, c"), cmplx_type(2, 2), true, 2);
  iNumErr += EqnTest(_T("c=(a-=1+2i), a=1, c"), cmplx_type(0, -2), true, 2);
  iNumErr += EqnTest(_T("c=(b*=1+2i), b=2, c"), cmplx_type(2, 4), true, 2);
  iNumErr += EqnTest(_T("c=(b/=1+2i), b=2, c"), cmplx_type((float_type)0.4, (float_type)-0.8), true, 2);

  // hex value recognition
  iNumErr += EqnTest(_T("0xff"), 255, true);
  iNumErr += EqnTest(_T("10+0xff"), 265, true);
  iNumErr += EqnTest(_T("0xff+10"), 265, true);
  iNumErr += EqnTest(_T("10*0xff"), 2550, true);
  iNumErr += EqnTest(_T("0xff*10"), 2550, true);
  iNumErr += EqnTest(_T("10+0xff+1"), 266, true);
  iNumErr += EqnTest(_T("1+0xff+10"), 266, true);

  // ...
  iNumErr += EqnTest(_T("exp(ln(7))"), (float_type)7.0, true);
  iNumErr += EqnTest(_T("e^ln(7)"), (float_type)7.0, true);
  iNumErr += EqnTest(_T("e^(ln(7))"), (float_type)7.0, true);
  iNumErr += EqnTest(_T("(e^(ln(7)))"), (float_type)7.0, true);
  iNumErr += EqnTest(_T("1-(e^(ln(7)))"), (float_type)-6.0, true);
  iNumErr += EqnTest(_T("2*(e^(ln(7)))"), (float_type)14.0, true);
  iNumErr += EqnTest(_T("10^log(5)"), (float_type)5.0, true);
  iNumErr += EqnTest(_T("10^log10(5)"), (float_type)5.0, true);
  iNumErr += EqnTest(_T("2^log2(4)"), (float_type)4.0, true);
  iNumErr += EqnTest(_T("-(sin(0)+1)"), (float_type)-1.0, true);
  iNumErr += EqnTest(_T("-(2^1.1)"), (float_type)-2.14354692, true);

  // infix + postfix operator in arguments for binary operators (Reference: Matlab)
  iNumErr += EqnTest(_T("-sin(8){m}*6"), (float_type)-0.00593615, true);
  iNumErr += EqnTest(_T("-sin(8){m}/6"), (float_type)-1.6489e-4, true);
  iNumErr += EqnTest(_T("-sin(8){m}+6"), (float_type)5.99901, true);
  iNumErr += EqnTest(_T("-sin(8){m}-6"), (float_type)-6.000989, true);

  iNumErr += EqnTest(_T("(cos(2.41)/b)"), (float_type)-0.372056, true);

  // long formula (Reference: Matlab)
  iNumErr += EqnTest(
    _T("(((-9))-e/(((((((pi-(((-7)+(-3)/4/e))))/(((-5))-2)-((pi+(-0))*(sqrt((e+e))*(-8))*(((-pi)+(-pi)-(-9)*(6*5))")
    _T("/(-e)-e))/2)/((((sqrt(2/(-e)+6)-(4-2))+((5/(-2))/(1*(-pi)+3))/8)*pi*((pi/((-2)/(-6)*1*(-1))*(-6)+(-e)))))/")
    _T("((e+(-2)+(-e)*((((-3)*9+(-e)))+(-9)))))))-((((e-7+(((5/pi-(3/1+pi)))))/e)/(-5))/(sqrt((((((1+(-7))))+((((-")
    _T("e)*(-e)))-8))*(-5)/((-e)))*(-6)-((((((-2)-(-9)-(-e)-1)/3))))/(sqrt((8+(e-((-6))+(9*(-9))))*(((3+2-8))*(7+6")
    _T("+(-5))+((0/(-e)*(-pi))+7)))+(((((-e)/e/e)+((-6)*5)*e+(3+(-5)/pi))))+pi))/sqrt((((9))+((((pi))-8+2))+pi))/e")
    _T("*4)*((-5)/(((-pi))*(sqrt(e)))))-(((((((-e)*(e)-pi))/4+(pi)*(-9)))))))+(-pi)"), (float_type)-12.23016549, true);

  // long formula (Reference: Matlab)
  iNumErr += EqnTest(_T("1+2-3*4/5^6*(2*(1-5+(3*7^9)*(4+6*7-3)))+12"), (float_type)-7995810.09926, true);

  /* <ibg 20100321 atan currently unsupported/>

  // long formula (Reference: Matlab)
  iNumErr += EqnTest(
    "(atan(sin((((((((((((((((pi/cos((a/((((0.53-b)-pi)*e)/b))))+2.51)+a)-0.54)/0.98)+b)*b)+e)/a)+b)+a)+b)+pi)/e"
    ")+a)))*2.77)", -2.16995656, true);
  */

  Assessment(iNumErr);
  return iNumErr;
}


  //---------------------------------------------------------------------------
  int ParserTester::TestNames()
  {
    int  iStat= 0;

    *m_stream << "testing name restriction enforcement...";
    
    ParserX p;

#define PARSER_THROWCHECK(DOMAIN, FAIL, EXPR, ARG) \
    iErr = 0;                                    \
    ParserTester::c_iCount++;                    \
    try                                          \
    {                                            \
      p.Define##DOMAIN(EXPR, ARG);               \
    }                                            \
    catch(Parser::exception_type&)               \
    {                                            \
      iErr = (FAIL==false) ? 0 : 1;              \
    }                                            \
    iStat += iErr;      
    
    // constant names
//      PARSER_THROWCHECK(Const, false, "0a", 1)
//      PARSER_THROWCHECK(Const, false, "9a", 1)
//      PARSER_THROWCHECK(Const, false, "+a", 1)
//      PARSER_THROWCHECK(Const, false, "-a", 1)
//      PARSER_THROWCHECK(Const, false, "a-", 1)
//      PARSER_THROWCHECK(Const, false, "a*", 1)
//      PARSER_THROWCHECK(Const, false, "a?", 1)
//      PARSER_THROWCHECK(Const, false, "a?", 1)
//      PARSER_THROWCHECK(Const, false, "a?", 1)
//      PARSER_THROWCHECK(Const, true, "a", 1)
//      PARSER_THROWCHECK(Const, true, "a_min", 1)
//      PARSER_THROWCHECK(Const, true, "a_min0", 1)
//      PARSER_THROWCHECK(Const, true, "a_min9", 1)
//      // variable names
//      Parser::value_type a;
//      p.ClearConst();
//      PARSER_THROWCHECK(Var, false, "9a", &a)
//      PARSER_THROWCHECK(Var, false, "0a", &a)
//      PARSER_THROWCHECK(Var, false, "+a", &a)
//      PARSER_THROWCHECK(Var, false, "-a", &a)
//      PARSER_THROWCHECK(Var, false, "?a", &a)
//      PARSER_THROWCHECK(Var, false, "!a", &a)
//      PARSER_THROWCHECK(Var, false, "a+", &a)
//      PARSER_THROWCHECK(Var, false, "a-", &a)
//      PARSER_THROWCHECK(Var, false, "a*", &a)
//      PARSER_THROWCHECK(Var, false, "a?", &a)
//      PARSER_THROWCHECK(Var, true, "a", &a)
//      PARSER_THROWCHECK(Var, true, "a_min", &a)
//      PARSER_THROWCHECK(Var, true, "a_min0", &a)
//      PARSER_THROWCHECK(Var, true, "a_min9", &a)
//      PARSER_THROWCHECK(Var, false, "a_min9", 0)
//      // Postfix operators
//      // fail
//      PARSER_THROWCHECK(PostfixOprt, false, "(k", f1of1)
//      PARSER_THROWCHECK(PostfixOprt, false, "9+", f1of1)
//      PARSER_THROWCHECK(PostfixOprt, false, "+", 0)
//      // pass
//      PARSER_THROWCHECK(PostfixOprt, true, "-a",  f1of1)
//      PARSER_THROWCHECK(PostfixOprt, true, "?a",  f1of1)
//      PARSER_THROWCHECK(PostfixOprt, true, "_",   f1of1)
//      PARSER_THROWCHECK(PostfixOprt, true, "#",   f1of1)
//      PARSER_THROWCHECK(PostfixOprt, true, "&&",  f1of1)
//      PARSER_THROWCHECK(PostfixOprt, true, "||",  f1of1)
//      PARSER_THROWCHECK(PostfixOprt, true, "&",   f1of1)
//      PARSER_THROWCHECK(PostfixOprt, true, "|",   f1of1)
//      PARSER_THROWCHECK(PostfixOprt, true, "++",  f1of1)
//      PARSER_THROWCHECK(PostfixOprt, true, "--",  f1of1)
//      PARSER_THROWCHECK(PostfixOprt, true, "?>",  f1of1)
//      PARSER_THROWCHECK(PostfixOprt, true, "?<",  f1of1)
//      PARSER_THROWCHECK(PostfixOprt, true, "**",  f1of1)
//      PARSER_THROWCHECK(PostfixOprt, true, "xor", f1of1)
//      PARSER_THROWCHECK(PostfixOprt, true, "and", f1of1)
//      PARSER_THROWCHECK(PostfixOprt, true, "or",  f1of1)
//      PARSER_THROWCHECK(PostfixOprt, true, "not", f1of1)
//      PARSER_THROWCHECK(PostfixOprt, true, "!",   f1of1)
//      // Binary operator
//      // The following must fail with builtin operators activated
//      // p.EnableBuiltInOp(true); -> this is the default
//      PARSER_THROWCHECK(Oprt, false, "+",  f1of2)
//      PARSER_THROWCHECK(Oprt, false, "-",  f1of2)
//      PARSER_THROWCHECK(Oprt, false, "*",  f1of2)
//      PARSER_THROWCHECK(Oprt, false, "/",  f1of2)
//      // without activated built in operators it should work
//      p.EnableBuiltInOprt(false);
//      PARSER_THROWCHECK(Oprt, true, "+",  f1of2)
//      PARSER_THROWCHECK(Oprt, true, "-",  f1of2)
//      PARSER_THROWCHECK(Oprt, true, "*",  f1of2)
//      PARSER_THROWCHECK(Oprt, true, "/",  f1of2)
//
#undef PARSER_THROWCHECK

    if (iStat==0) 
      *m_stream << "passed" << endl;
    else 
      *m_stream << "\n  failed with " << iStat << " errors" << endl;

    return iStat;
  }

  //---------------------------------------------------------------------------
  void ParserTester::AddTest(testfun_type a_pFun)
  {
    m_vTestFun.push_back(a_pFun);
  }

  //---------------------------------------------------------------------------
  void ParserTester::Run()
  {
    int iStat = 0;
    try
    {
      for (int i=0; i<(int)m_vTestFun.size(); ++i)
        iStat += (this->*m_vTestFun[i])();
    }
    catch(ParserError &e)
    {
      *m_stream << e.GetMsg() << endl;
      *m_stream << e.GetToken() << endl;
      Abort();
    }
    catch(std::exception &e)
    {
      *m_stream << e.what() << endl;
      Abort();
    }
    catch(...)
    {
      *m_stream << _T("Internal error");
      Abort();
    }

    if (iStat==0) 
    {
      *m_stream << _T("Test passed (") <<  ParserTester::c_iCount << _T(" expressions)") << endl;
    }
    else 
    {
      *m_stream << _T("Test failed with ") << iStat 
                << _T(" errors (") <<  ParserTester::c_iCount 
                << _T(" expressions)") << endl;
    }
    ParserTester::c_iCount = 0;

#ifdef MUP_LEAKAGE_REPORT
    // All tokens must have been destroyed by now, verify this
    IToken::LeakageReport();  
#endif
  }


  //---------------------------------------------------------------------------
  int ParserTester::ThrowTest(const string_type &a_sExpr, int a_iErrc, bool a_bFail)
  {
    ParserTester::c_iCount++;
    
    try
    {
      ParserX p;

      // Add variables
      Value vVarVal[] = { 1, 2, 3, -2};
      p.DefineVar( _T("a"), Variable(&vVarVal[0]) );
      p.DefineVar( _T("b"), Variable(&vVarVal[1]) );
      p.DefineVar( _T("c"), Variable(&vVarVal[2]) );
      p.DefineVar( _T("d"), Variable(&vVarVal[3]) );

      // some vector variables
      Value aVal1(3, 0);
      aVal1[0] = (float_type)1.0;
      aVal1[1] = (float_type)2.0;
      aVal1[2] = (float_type)3.0;
  
      Value aVal2(3, 0);
      aVal2[0] = (float_type)4.0;
      aVal2[1] = (float_type)3.0;
      aVal2[2] = (float_type)2.0;

      Value aVal3(4, 0);
      aVal3[0] = (float_type)4.0;
      aVal3[1] = (float_type)3.0;
      aVal3[2] = (float_type)2.0;
      aVal3[3] = (float_type)5.0;

      Value aVal4(4, 0);
      aVal4[0] = (float_type)4.0;
      aVal4[1] = false;
      aVal4[2] = _T("hallo");

      p.DefineVar(_T("va"), Variable(&aVal1));
      p.DefineVar(_T("vb"), Variable(&aVal2)); 
      p.DefineVar(_T("vc"), Variable(&aVal3)); 
      p.DefineVar(_T("vd"), Variable(&aVal4)); 

      p.SetExpr(a_sExpr);
      Value fRes = p.Eval();
    }
    catch(ParserError &e)
    {
      // output the formula in case of an failed test
      if (a_bFail==true && a_iErrc!=e.GetCode() )
      {
        *m_stream << _T("\n  ") 
                  << _T("Expression: ") << a_sExpr 
                  << _T("  Code:") << e.GetCode() 
                  << _T("  Expected:") << a_iErrc;
      }
      return (a_iErrc==e.GetCode()) ? 0 : 1;
    }

    if (a_bFail==true)
    {
      *m_stream << _T("\n  ") 
                << _T("Expression failed: ") 
                << a_sExpr 
                << _T("  (reason: no exception raised).");
    }

    // if a_bFail==false no exception is expected
    return (a_bFail==false) ? 0 : 1; 
  }

  //---------------------------------------------------------------------------
  int ParserTester::EqnTest(const string_type &a_str, Value a_fRes, bool a_fPass, int nExprVar)
  {
    ParserTester::c_iCount++;
    int iRet(1);
    Value fVal[5];

    try
    {
      // p1 is a pointer since I'm going to delete it in order to test if
      // parsers after copy construction still refer to members of the deleted object.
      // !! If this is the case this function will crash !!
      std::auto_ptr<ParserX> p1(new ParserX()); 

      // Add variables
      Value vVarVal[] = { 1, 2, 3, -2, -1};

      Value m1(3,3,0);
      m1[0][0] = 1;
      m1[1][1] = 1;
      m1[2][2] = 1;

      p1->DefineOprt(new DbgSillyAdd);

      p1->DefineVar( _T("a"),  Variable(&vVarVal[0]) );
      p1->DefineVar( _T("b"),  Variable(&vVarVal[1]) );
      p1->DefineVar( _T("c"),  Variable(&vVarVal[2]) );
      p1->DefineVar( _T("d"),  Variable(&vVarVal[3]) );
      p1->DefineVar( _T("f"),  Variable(&vVarVal[4]) );
      p1->DefineVar( _T("m1"), Variable(&m1) );

      // Add constants
      p1->DefineConst(_T("pi"), (float_type)MUP_CONST_PI);
      p1->DefineConst(_T("e"),  (float_type)MUP_CONST_E);
      p1->DefineConst(_T("const"),  1);
      p1->DefineConst(_T("const1"), 2);
      p1->DefineConst(_T("const2"), 3);

      // some vector variables
      Value aVal1(3, 0);
      aVal1[0] = (float_type)1.0;
      aVal1[1] = (float_type)2.0;
      aVal1[2] = (float_type)3.0;
      
      Value aVal2(3, 0);
      aVal2[0] = (float_type)4.0;
      aVal2[1] = (float_type)3.0;
      aVal2[2] = (float_type)2.0;
      p1->DefineVar(_T("va"), Variable(&aVal1));
      p1->DefineVar(_T("vb"), Variable(&aVal2)); 

      p1->SetExpr(a_str);
      
      fVal[0] = p1->Eval();

      // Test copy and assignement operators
      std::vector<ParserX> vParser;
      vParser.push_back(*p1);   // Push p1 into the vector
      ParserX p2 = vParser[0];   // take parser from vector
      
      // destroy the originals from p2
      vParser.clear();              // delete the vector
      p1.reset(0);                  // delete the original

      fVal[1] = p2.Eval();          // If copy constructions does not work
                                    // we may see a crash here

      // Test assignement operator
      // additionally  disable Optimizer this time
      ParserX p3;
      p3 = p2;
      fVal[2] = p3.Eval();          // If assignment does not work
                                    // we may see a crash here

      // Calculating a second time will parse from rpn rather than from
      // string. The result must be the same...
      fVal[3] = p3.Eval();

      // Calculate yet another time. There is the possibility of
      // changing variables as a side effect of expression
      // evaluation. So there are really bugs that could make this fail...
      fVal[4] = p3.Eval(); 

      // Check i number of used variables is correct
      if (nExprVar!=-1)
      {
        int n2 = p2.GetExprVar().size();
        int n3 = p3.GetExprVar().size();

        if (n2+n3!=2*n2 || n2!=nExprVar)       
        {
          *m_stream << _T("  Number of expression variables is incorrect. (expected: ") 
                    << nExprVar << _T("; detected: ") << n2 << _T(")");
        }
      }

      // Check the three results
      // 1.) Types must be identic
      bool bStat = fVal[0].GetType()==fVal[1].GetType() &&
                   fVal[0].GetType()==fVal[2].GetType() &&
                   fVal[0].GetType()==fVal[3].GetType() &&
                   fVal[0].GetType()==fVal[4].GetType();
      char_type cType = fVal[1].GetType();
      if (!bStat) 
      {
       *m_stream << _T("\n  ") << a_str << _T(" :  inconsistent result type (") 
                 << fVal[0].GetType() << _T(", ") 
                 << fVal[1].GetType() << _T(", ") 
                 << fVal[2].GetType() << _T(", ") 
                 << fVal[3].GetType() << _T(", ") 
                 << fVal[4].GetType() << _T(")");
        return 1;
      }

      // Compare the results
      switch(cType)
      {
      case  'i': 
      case  'b': 
      case  's': bStat = (a_fRes==fVal[0] && 
                          a_fRes==fVal[1] && 
                          a_fRes==fVal[2] && 
                          a_fRes==fVal[3] && 
                          a_fRes==fVal[4]); break;

      // We need more attention for comaring float values due to floating point
      // inaccuracies.
      case  'f': 
            {
              bStat = true;
  	          int num = sizeof(fVal)/sizeof(Value);
              for (int i=0; i<num; ++i)
	              bStat &= (fabs(a_fRes.GetFloat()-fVal[i].GetFloat()) <= fabs(fVal[i].GetFloat()*0.0001));
            }
            break;

      case 'c':
            {
              bStat = true;
  	          int num = sizeof(fVal) / sizeof(Value);
              for (int i=0; i<num; ++i)
              {
			    //VC++ does not understand this!
                //bStat &= (fabs(a_fRes.GetFloat() - fVal[i].GetFloat()) <= std::max((float_type)1e-15, fabs(fVal[i].GetFloat() * (float_type)0.0000001)));
                //bStat &= (fabs(a_fRes.GetImag() - fVal[i].GetImag())  <= std::max((float_type)1e-15, fabs(fVal[i].GetImag() * (float_type)0.0000001)));
				float_type f_a = fabs(a_fRes.GetFloat() - fVal[i].GetFloat());
				float_type f_b = std::max((float_type)1e-15, fabs(fVal[i].GetFloat() * (float_type)0.0000001));
                bStat = bStat && (f_a <= f_b);
				float_type f_c = fabs(a_fRes.GetImag() - fVal[i].GetImag());
				float_type f_d = std::max((float_type)1e-15, fabs(fVal[i].GetImag() * (float_type)0.0000001));
                bStat = bStat && (f_c  <= f_d);
              }
            }
            break;

      default: 
            throw std::runtime_error("Parser return value has an unexpected typecode.");
      }

      iRet = (bStat==a_fPass) ? 0 : 1;
   }
   catch(ParserError &e)
   {
     *m_stream << _T("\n  ") << a_str << _T(" : ") << e.GetMsg();	
     return 1;
   }
   catch(std::exception &e)
   {
     *m_stream << _T("\n  ") << a_str << _T(" : ") << e.what() << _T("\n");
     return 1;
   }
   catch(...)
   {
     *m_stream << _T("\n  \"") << a_str << _T("\" : ") << _T("Unexpected Eception");
     return 1;
   }

   if (iRet)
   {
     *m_stream << _T("\n  ") << a_str << _T(" : ") << _T("(at least one incorrect result ") 
               << fVal[0] << _T(", ") 
               << fVal[1] << _T(", ") 
               << fVal[2] << _T(", ") 
               << fVal[3] << _T(", ") 
               << fVal[4] << _T("; expected=") << a_fRes << _T(")");	
   }

   return iRet; 
  }

  //---------------------------------------------------------------------------
  /** \brief Internal error in test class Test is going to be aborted. */
  void ParserTester::Abort() const
  {
    *m_stream << _T("\nTest failed (internal error in test class)") << endl;
    while (!getchar());
    exit(-1);
  }

  //---------------------------------------------------------------------------
  void ParserTester::Assessment(int a_iNumErr) const
  {
    if (a_iNumErr==0) 
      *m_stream << _T("passed") << endl;
    else 
      *m_stream << _T("\n  failed with ") << a_iNumErr << _T(" errors") << endl;
  }
MUP_NAMESPACE_END
