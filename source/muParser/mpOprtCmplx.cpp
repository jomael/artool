/*
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
*/
#include "mpOprtCmplx.h"
#include <iomanip>

MUP_NAMESPACE_START

//Sadjad
#if defined(_MSC_VER) && (_MSC_VER <= 1200)
	//see mpFuncCmplx.cpp for details on this:
	using std::log;
	using ::log;
	using std::exp;
	using ::exp;

#endif

//------------------------------------------------------------------------------
//
//  class  OprtSignCmplx
//
//------------------------------------------------------------------------------

  OprtSignCmplx::OprtSignCmplx()
    :IOprtInfix( _T("-"), _T("c:c") )
  {}

  //------------------------------------------------------------------------------
  void OprtSignCmplx::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc)  
  { 
    assert(a_iArgc==1);
    float_type re = a_pArg[0]->GetFloat();
    float_type im = a_pArg[0]->GetImag();

		// Do not omit the test for zero! Multiplying 0 with -1 
    // will yield -0 on IEEE754 compliant implementations!
    // This would change the result of complex calculations:
    // 
    // i.e. sqrt(-1 + (-0)i) !=  sqrt(-1 + 0i)
    //                   -i  !=  i  
    cmplx_type v((re==0) ? 0 : -re, (im==0) ? 0 : -im);
    *ret = v; 
  }

  //------------------------------------------------------------------------------
  const char_type* OprtSignCmplx::GetDesc() const 
  { 
    return _T("unit multiplicator 1e-9"); 
  }

  //------------------------------------------------------------------------------
  IToken* OprtSignCmplx::Clone() const 
  { 
    return new OprtSignCmplx(*this); 
  }

//-----------------------------------------------------------
//
// class OprtAddCmplx
//
//-----------------------------------------------------------

  OprtAddCmplx::OprtAddCmplx() 
    :IOprtBin(_T("+"), _T("c:.."), 3, oaLEFT) 
  {}

  //-----------------------------------------------------------
  void OprtAddCmplx::Eval(ptr_val_type& ret, const ptr_val_type *a_pArg, int num)
  { 
    assert(num==2);
    std::size_t i;
    const IValue *arg1 = a_pArg[0].Get();
    const IValue *arg2 = a_pArg[1].Get();
    if (arg1->IsNonComplexScalar() && arg2->IsNonComplexScalar())
    {
      *ret = arg1->GetFloat() + arg2->GetFloat(); 
    }
    else if (arg1->GetType()=='a' && arg2->GetType()=='a')
    {
      // Vector + Vector
      const array_type &a1 = arg1->GetArray(),
                       &a2 = arg2->GetArray();
      if (a1.size()!=a2.size())
        throw ParserError(ErrorContext(ecARRAY_SIZE_MISMATCH, -1, GetIdent(), 'a', 'a', 2));
      
      array_type rv(a1.size());
      for (i=0; i<a1.size(); ++i)
      {
        if (!a1[i]->IsScalar())
          throw ParserError( ErrorContext(ecTYPE_CONFLICT_FUN, -1, GetIdent(), a1[i]->GetType(), 'c', 1));

        if (!a2[i]->IsScalar())
          throw ParserError( ErrorContext(ecTYPE_CONFLICT_FUN, -1, GetIdent(), a2[i]->GetType(), 'c', 1));

        rv[i] = new Value(cmplx_type(a1[i]->GetFloat() + a2[i]->GetFloat(),
                           a1[i]->GetImag()  + a2[i]->GetImag()));
      }

      *ret = rv; 
      for (i = 0; i < rv.size(); ++i)
      {
        delete dynamic_cast<Value*>(rv[i]);
      }
    }
    else
    {
      if (!arg1->IsScalar())
        throw ParserError( ErrorContext(ecTYPE_CONFLICT_FUN, -1, GetIdent(), arg1->GetType(), 'c', 1)); 

      if (!arg2->IsScalar())
        throw ParserError( ErrorContext(ecTYPE_CONFLICT_FUN, -1, GetIdent(), arg2->GetType(), 'c', 2)); 
      
      *ret = cmplx_type(arg1->GetFloat() + arg2->GetFloat(),
                        arg1->GetImag()  + arg2->GetImag()); 
    }
  }

  //-----------------------------------------------------------
  const char_type* OprtAddCmplx::GetDesc() const 
  { 
    return _T("addition"); 
  }
  
  //-----------------------------------------------------------
  IToken* OprtAddCmplx::Clone() const
  { 
    return new OprtAddCmplx(*this); 
  }

//-----------------------------------------------------------
//
// class OprtSubCmplx
//
//-----------------------------------------------------------

  OprtSubCmplx::OprtSubCmplx() 
    :IOprtBin(_T("-"), _T("c:.."), 3, oaLEFT) 
  {}

  //-----------------------------------------------------------
  void OprtSubCmplx::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int num)
  { 
    assert(num==2);
    std::size_t i;
    if ( a_pArg[0]->IsNonComplexScalar() && a_pArg[1]->IsNonComplexScalar())
    {
      *ret = a_pArg[0]->GetFloat() - a_pArg[1]->GetFloat(); 
    }
    else if (a_pArg[0]->GetType()=='a' && a_pArg[1]->GetType()=='a')
    {
      const array_type &a1 = a_pArg[0]->GetArray(),
                       &a2 = a_pArg[1]->GetArray();
      if (a1.size()!=a2.size())
        throw ParserError(ErrorContext(ecARRAY_SIZE_MISMATCH, -1, GetIdent(), 'a', 'a', 2));
      
      array_type rv(a1.size());
      for (i=0; i<a1.size(); ++i)
      {
        if (!a1[i]->IsScalar())
          throw ParserError( ErrorContext(ecTYPE_CONFLICT_FUN, -1, GetIdent(), a1[i]->GetType(), 'c', 1));

        if (!a2[i]->IsScalar())
          throw ParserError( ErrorContext(ecTYPE_CONFLICT_FUN, -1, GetIdent(), a2[i]->GetType(), 'c', 1));

        rv[i] = new Value(cmplx_type(a1[i]->GetFloat() - a2[i]->GetFloat(),
                           a1[i]->GetImag()  - a2[i]->GetImag()));
      }

      *ret = rv;
      for (i = 0; i < rv.size(); ++i)
      {
        delete dynamic_cast<Value*>(rv[i]);
      }
    }
    else
    {
      if (!a_pArg[0]->IsScalar())
        throw ParserError( ErrorContext(ecTYPE_CONFLICT_FUN, -1, GetIdent(), a_pArg[0]->GetType(), 'c', 1)); 

      if (!a_pArg[1]->IsScalar())
        throw ParserError( ErrorContext(ecTYPE_CONFLICT_FUN, -1, GetIdent(), a_pArg[1]->GetType(), 'c', 2)); 
      
      *ret = cmplx_type(a_pArg[0]->GetFloat() - a_pArg[1]->GetFloat(),
                        a_pArg[0]->GetImag() - a_pArg[1]->GetImag()); 
    }
  }

  //-----------------------------------------------------------
  const char_type* OprtSubCmplx::GetDesc() const 
  { 
    return _T("subtraction"); 
  }
  
  //-----------------------------------------------------------
  IToken* OprtSubCmplx::Clone() const
  { 
    return new OprtSubCmplx(*this); 
  }

//-----------------------------------------------------------
//
// class OprtMulCmplx
//
//-----------------------------------------------------------
    
  OprtMulCmplx::OprtMulCmplx() 
    :IOprtBin(_T("*"), _T("c:.."), 4, oaLEFT) 
  {}

  //-----------------------------------------------------------
  void OprtMulCmplx::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int num)
  { 
    assert(num==2);
    IValue *arg1 = a_pArg[0].Get();
    IValue *arg2 = a_pArg[1].Get();
    std::size_t i;
    if (arg1->IsNonComplexScalar() && arg2->IsNonComplexScalar())
    {
      *ret = arg1->GetFloat() * arg2->GetFloat(); 
    }
    else if (arg1->GetType()=='a' && arg2->GetType()=='a')
    {
      // Scalar multiplication
      array_type a1 = arg1->GetArray();
      array_type a2 = arg2->GetArray();

      if (a1.size()!=a2.size())
        throw ParserError(ErrorContext(ecARRAY_SIZE_MISMATCH, -1, GetIdent(), 'a', 'a', 2));

      float_type val(0);
      for (i=0; i<a1.size(); ++i)
        val += (a1[i]->GetFloat())*(a2[i]->GetFloat());

      *ret = val;
    }
    else if (a_pArg[0]->GetType()=='a' && a_pArg[1]->IsScalar())
    {
      // Skalar * Vector
      array_type out(a_pArg[0]->GetArray());
      for (i=0; i<out.size(); ++i)
        out[i] = new Value((out[i]->GetFloat()) * (a_pArg[1]->GetFloat()));

      *ret = out; 
      for (i = 0; i < out.size(); ++i)
      {
        delete dynamic_cast<Value*>(out[i]);
      }
    }
    else if (a_pArg[1]->GetType()=='a' && a_pArg[0]->IsScalar())
    {
      // Vector * Skalar
      array_type out(a_pArg[1]->GetArray());
      for (i=0; i<out.size(); ++i)
        out[i] = new Value((out[i]->GetFloat()) * (a_pArg[0]->GetFloat()));

      *ret = out; 
      for (i = 0; i < out.size(); ++i)
      {
        delete dynamic_cast<Value*>(out[i]);
      }
    }
    else
    {
      if (!a_pArg[0]->IsScalar())
        throw ParserError( ErrorContext(ecTYPE_CONFLICT_FUN, -1, GetIdent(), a_pArg[0]->GetType(), 'c', 1)); 

      if (!a_pArg[1]->IsScalar())
        throw ParserError( ErrorContext(ecTYPE_CONFLICT_FUN, -1, GetIdent(), a_pArg[1]->GetType(), 'c', 2)); 

      // multiplication of two imaginary numbers      
      float_type a = a_pArg[0]->GetFloat(),
                 b = a_pArg[0]->GetImag(),
                 c = a_pArg[1]->GetFloat(),
                 d = a_pArg[1]->GetImag();
      *ret = cmplx_type(a*c-b*d, a*d+b*c);
    }
  }

  //-----------------------------------------------------------
  const char_type* OprtMulCmplx::GetDesc() const 
  { 
    return _T("multiplication"); 
  }
  
  //-----------------------------------------------------------
  IToken* OprtMulCmplx::Clone() const
  { 
    return new OprtMulCmplx(*this); 
  }

//-----------------------------------------------------------
//
// class OprtDivCmplx
//
//-----------------------------------------------------------


  OprtDivCmplx::OprtDivCmplx() 
    :IOprtBin(_T("/"), _T("c:cc"), 4, oaLEFT) 
  {}

  //-----------------------------------------------------------
  /** \brief Implements the Division operator. 
      \throw ParserError in case one of the arguments if 
             nonnumeric or an array.
  
  */
  void OprtDivCmplx::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int num)
  { 
    assert(num==2);

    if ( a_pArg[0]->IsNonComplexScalar() && a_pArg[1]->IsNonComplexScalar())
    {
      *ret = a_pArg[0]->GetFloat() / a_pArg[1]->GetFloat(); 
    }
    else
    {
      // multiplication of two imaginary numbers      
      float_type a = a_pArg[0]->GetFloat(),
                 b = a_pArg[0]->GetImag(),
                 c = a_pArg[1]->GetFloat(),
                 d = a_pArg[1]->GetImag(),
                 n = c*c + d*d;
      *ret = cmplx_type((a*c+b*d)/n, (b*c-a*d)/n); 
    }
  }

  //-----------------------------------------------------------
  const char_type* OprtDivCmplx::GetDesc() const 
  { 
    return _T("division"); 
  }
  
  //-----------------------------------------------------------
  IToken* OprtDivCmplx::Clone() const
  { 
    return new OprtDivCmplx(*this); 
  }

//-----------------------------------------------------------
//
// class OprtPowCmplx
//
//-----------------------------------------------------------

  OprtPowCmplx::OprtPowCmplx() 
    :IOprtBin(_T("^"), _T("c:cc"), 5, oaRIGHT) 
  {}
                                                                        
  //-----------------------------------------------------------
  void OprtPowCmplx::Eval(ptr_val_type& ret, const ptr_val_type *arg, int argc)
  {
    assert(argc==2);
    if ((arg[0]->GetImag() == 0) && (arg[1]->GetImag() == 0))
    {
      float_type a = arg[0]->GetFloat();
      float_type b = arg[1]->GetFloat();

      int ib = (int)b;
      if (b-ib==0)
      {
        switch (ib)
        {
          case 1: *ret = a; return;
          case 2: *ret = a*a; return;
          case 3: *ret = a*a*a; return;
          case 4: *ret = a*a*a*a; return;
          case 5: *ret = a*a*a*a*a; return;
          default: *ret = pow(a, ib); return;
        }
      }
      else
      {
        *ret = pow(a, b);
      }
    }
    else
    {
      *ret = exp(arg[1]->GetComplex() * log(arg[0]->GetComplex()));
    }
  }

  //-----------------------------------------------------------
  const char_type* OprtPowCmplx::GetDesc() const 
  { 
    return _T("raise x to the power of y"); 
  }

  //-----------------------------------------------------------
  IToken* OprtPowCmplx::Clone() const            
  { 
    return new OprtPowCmplx(*this); 
  }
}
