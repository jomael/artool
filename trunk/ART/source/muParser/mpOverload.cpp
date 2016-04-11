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
#include "mpOverload.h"


MUP_NAMESPACE_START

  //------------------------------------------------------------------------------
  OprtBinOverloadProxy::OprtBinOverloadProxy(const string_type &sIdent) 
    :IToken(cmOPRT_BIN_OVERLOAD, sIdent) 
    ,m_vpOprt()
  {}
  
  //------------------------------------------------------------------------------
  const char_type* OprtBinOverloadProxy::GetDesc() const 
  { 
    return _T("overload proxy"); 
  }

  //------------------------------------------------------------------------------
  IToken* OprtBinOverloadProxy::Clone() const
  { 
    return new OprtBinOverloadProxy(GetIdent()); 
  }

  //------------------------------------------------------------------------------
  string_type OprtBinOverloadProxy::AsciiDump() const
  {
    return string_type(_T(""));
  }

  //---------------------------------------------------------------------------
  ICallback* OprtBinOverloadProxy::AsICallback()
  {
    return NULL;
  }

  //---------------------------------------------------------------------------
  IValue* OprtBinOverloadProxy::AsIValue()
  {
    return NULL;
  }

  //------------------------------------------------------------------------------
  void OprtBinOverloadProxy::AddOverload(ptr_tok_type pOprtBin)
  {
    m_vpOprt.push_back(pOprtBin);
  }
}