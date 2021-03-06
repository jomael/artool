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
#include "mpOprtNonCmplx.h"

MUP_NAMESPACE_START

  //------------------------------------------------------------------------------
  //
  //  Sign operator
  //
  //------------------------------------------------------------------------------

  OprtSign::OprtSign()
    :IOprtInfix( _T("-"), _T("f:f") )
  {}

  //------------------------------------------------------------------------------
  void OprtSign::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc)  
  { 
    assert(a_iArgc==1);
    *ret = -a_pArg[0]->GetFloat(); 
  }

  //------------------------------------------------------------------------------
  const char_type* OprtSign::GetDesc() const 
  { 
    return _T("unit multiplicator 1e-9"); 
  }

  //------------------------------------------------------------------------------
  IToken* OprtSign::Clone() const 
  { 
    return new OprtSign(*this); 
  }

//-----------------------------------------------------------
//
// class OprtAdd
//
//-----------------------------------------------------------

  OprtAdd::OprtAdd() 
    :IOprtBin(_T("+"), _T("f:.."), 3, oaLEFT) 
  {}

  //-----------------------------------------------------------
  void OprtAdd::Eval(ptr_val_type& ret, const ptr_val_type *a_pArg, int num)
  { 
    assert(num==2);

    const IValue *arg1 = a_pArg[0].Get();
    const IValue *arg2 = a_pArg[1].Get();
    std::size_t i;
    if (arg1->GetType()=='a' && arg2->GetType()=='a')
    {
      // Vector + Vector
      const array_type &a1 = arg1->GetArray(),
                       &a2 = arg2->GetArray();
      if (a1.size()!=a2.size())
        throw ParserError(ErrorContext(ecARRAY_SIZE_MISMATCH, -1, GetIdent(), 'a', 'a', 2));
      
      array_type rv(a1.size());
      for (i=0; i<a1.size(); ++i)
      {
        if (!a1[i]->IsNonComplexScalar())
          throw ParserError( ErrorContext(ecTYPE_CONFLICT_FUN, -1, GetIdent(), a1[i]->GetType(), 'f', 1));

        if (!a2[i]->IsNonComplexScalar())
          throw ParserError( ErrorContext(ecTYPE_CONFLICT_FUN, -1, GetIdent(), a2[i]->GetType(), 'f', 1));

        rv[i] = new Value(a1[i]->GetFloat() + a2[i]->GetFloat());
      }

      // copy result
      *ret = rv; 
      // clean up memory
      for (i = 0; i < rv.size(); ++i)
      {
    	  delete dynamic_cast<Value*>(rv[i]);
      }
    }
    else
    {
      if (!arg1->IsNonComplexScalar())
        throw ParserError( ErrorContext(ecTYPE_CONFLICT_FUN, -1, GetIdent(), arg1->GetType(), 'f', 1)); 

      if (!arg2->IsNonComplexScalar())
        throw ParserError( ErrorContext(ecTYPE_CONFLICT_FUN, -1, GetIdent(), arg2->GetType(), 'f', 2)); 
      
      *ret = arg1->GetFloat() + arg2->GetFloat(); 
    }
  }

  //-----------------------------------------------------------
  const char_type* OprtAdd::GetDesc() const 
  { 
    return _T("x+y - Addition for noncomplex values"); 
  }
  
  //-----------------------------------------------------------
  IToken* OprtAdd::Clone() const
  { 
    return new OprtAdd(*this); 
  }

//-----------------------------------------------------------
//
// class OprtSub
//
//-----------------------------------------------------------

  OprtSub::OprtSub() 
    :IOprtBin(_T("-"), _T("f:.."), 3, oaLEFT) 
  {}

  //-----------------------------------------------------------
  void OprtSub::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int num)
  { 
    assert(num==2);
    std::size_t i;
    if (a_pArg[0]->GetType()=='a' && a_pArg[1]->GetType()=='a')
    {
      const array_type &a1 = a_pArg[0]->GetArray(),
                       &a2 = a_pArg[1]->GetArray();
      if (a1.size()!=a2.size())
        throw ParserError(ErrorContext(ecARRAY_SIZE_MISMATCH, -1, GetIdent(), 'a', 'a', 2));
      
      array_type rv(a1.size());
      for (i=0; i<a1.size(); ++i)
      {
        if (!a1[i]->IsNonComplexScalar())
          throw ParserError( ErrorContext(ecTYPE_CONFLICT_FUN, -1, GetIdent(), a1[i]->GetType(), 'f', 1));

        if (!a2[i]->IsNonComplexScalar())
          throw ParserError( ErrorContext(ecTYPE_CONFLICT_FUN, -1, GetIdent(), a2[i]->GetType(), 'f', 1));

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
      if (!a_pArg[0]->IsNonComplexScalar())
        throw ParserError( ErrorContext(ecTYPE_CONFLICT_FUN, -1, GetIdent(), a_pArg[0]->GetType(), 'f', 1)); 

      if (!a_pArg[1]->IsNonComplexScalar())
        throw ParserError( ErrorContext(ecTYPE_CONFLICT_FUN, -1, GetIdent(), a_pArg[1]->GetType(), 'f', 2)); 
      
      *ret = a_pArg[0]->GetFloat() - a_pArg[1]->GetFloat(); 
    }
  }

  //-----------------------------------------------------------
  const char_type* OprtSub::GetDesc() const 
  { 
    return _T("subtraction"); 
  }
  
  //-----------------------------------------------------------
  IToken* OprtSub::Clone() const
  { 
    return new OprtSub(*this); 
  }

//-----------------------------------------------------------
//
// class OprtMul
//
//-----------------------------------------------------------
    
  OprtMul::OprtMul() 
    :IOprtBin(_T("*"), _T("f:.."), 4, oaLEFT) 
  {}

  //-----------------------------------------------------------
  void OprtMul::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int num)
  { 
    assert(num==2);
    IValue *arg1 = a_pArg[0].Get();
    IValue *arg2 = a_pArg[1].Get();
    std::size_t i;
    if (arg1->GetType()=='a' && arg2->GetType()=='a')
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
    else if (arg1->GetType()=='a' && arg2->IsNonComplexScalar())
    {
      // Skalar * Vector
      array_type out(a_pArg[0]->GetArray());
      for (i=0; i<out.size(); ++i)
        out[i] = new Value((out[i]->GetFloat()) * (arg2->GetFloat()));

      *ret = out;
      for (i = 0; i < out.size(); ++i)
      {
        delete dynamic_cast<Value*>(out[i]);
      }
    }
    else if (arg2->GetType()=='a' && arg1->IsNonComplexScalar())
    {
      // Vector * Skalar
      array_type out(arg2->GetArray());
      for (i=0; i<out.size(); ++i)
        out[i] = new Value((out[i]->GetFloat()) * (arg1->GetFloat()));

      *ret = out; 
      for (i = 0; i < out.size(); ++i)
      {
        delete dynamic_cast<Value*>(out[i]);
      }
    }
    else
    {
      if (!arg1->IsNonComplexScalar())
        throw ParserError( ErrorContext(ecTYPE_CONFLICT_FUN, -1, GetIdent(), arg1->GetType(), 'f', 1)); 

      if (!arg2->IsNonComplexScalar())
        throw ParserError( ErrorContext(ecTYPE_CONFLICT_FUN, -1, GetIdent(), arg2->GetType(), 'f', 2)); 

      *ret = arg1->GetFloat() * arg2->GetFloat(); 
    }
  }

  //-----------------------------------------------------------
  const char_type* OprtMul::GetDesc() const 
  { 
    return _T("multiplication"); 
  }
  
  //-----------------------------------------------------------
  IToken* OprtMul::Clone() const
  { 
    return new OprtMul(*this); 
  }

//-----------------------------------------------------------
//
// class OprtDiv
//
//-----------------------------------------------------------


  OprtDiv::OprtDiv() 
    :IOprtBin(_T("/"), _T("f:ff"), 4, oaLEFT) 
  {}

  //-----------------------------------------------------------
  /** \brief Implements the Division operator. 
      \throw ParserError in case one of the arguments if 
             nonnumeric or an array.
  
  */
  void OprtDiv::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int num)
  { 
    assert(num==2);

    if (!a_pArg[0]->IsNonComplexScalar())
      throw ParserError( ErrorContext(ecTYPE_CONFLICT_FUN, -1, GetIdent(), a_pArg[0]->GetType(), 'f', 1)); 

    if (!a_pArg[1]->IsNonComplexScalar())
      throw ParserError( ErrorContext(ecTYPE_CONFLICT_FUN, -1, GetIdent(), a_pArg[1]->GetType(), 'f', 2)); 
    
    *ret = a_pArg[0]->GetFloat() / a_pArg[1]->GetFloat();
  }

  //-----------------------------------------------------------
  const char_type* OprtDiv::GetDesc() const 
  { 
    return _T("division"); 
  }
  
  //-----------------------------------------------------------
  IToken* OprtDiv::Clone() const
  { 
    return new OprtDiv(*this); 
  }

//-----------------------------------------------------------
//
// class OprtPow
//
//-----------------------------------------------------------

  OprtPow::OprtPow() 
    :IOprtBin(_T("^"), _T("f:ff"), 5, oaRIGHT) 
  {}
                                                                        
  //-----------------------------------------------------------
  void OprtPow::Eval(ptr_val_type& ret, const ptr_val_type *arg, int argc)
  {
    assert(argc==2);
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
      *ret = pow(a, b);
  }

  //-----------------------------------------------------------
  const char_type* OprtPow::GetDesc() const 
  { 
    return _T("x^y - Raises x to the power of y"); 
  }

  //-----------------------------------------------------------
  IToken* OprtPow::Clone() const            
  { 
    return new OprtPow(*this); 
  }
}
