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
#include "mpRPN.h"
#include "mpIToken.h"
#include "mpICallback.h"
#include "mpError.h"

#include <algorithm>
MUP_NAMESPACE_START

  //---------------------------------------------------------------------------
  RPN::RPN()
    :m_vRPN()
    ,m_nStackPos(-1)
    ,m_nMaxStackPos(0)
  {}
  
  //---------------------------------------------------------------------------
  RPN::~RPN()
  {}
    
  //---------------------------------------------------------------------------
  void RPN::Add(ptr_tok_type tok)
  {
    tok->SetRPNIdx(m_vRPN.size());

    m_vRPN.push_back(tok);
    if (tok->AsIValue()!=NULL)
    {
      m_nStackPos++;
    }
    else if (tok->AsICallback())
    {
      ICallback *pFun = tok->AsICallback();
      MUP_ASSERT(pFun);
      m_nStackPos -= pFun->GetArgsPresent() - 1;
    }

    assert(m_nStackPos>=0);
    m_nMaxStackPos = std::max(m_nStackPos, m_nMaxStackPos);
  }
  
  //---------------------------------------------------------------------------
  void RPN::Pop(int num)
  {
    if (m_vRPN.size()==0)
      return;

    for (int i=0; i<num; ++i)
    {
      ptr_tok_type tok = m_vRPN.back();
      
      if (tok->AsIValue()!=0)
        m_nStackPos--;

      m_vRPN.pop_back();
    }
  }

  //---------------------------------------------------------------------------
  void RPN::Reset()
  {
    m_vRPN.clear();
    m_nStackPos = -1;
    m_nMaxStackPos = 0;
  }

  //---------------------------------------------------------------------------
  std::size_t RPN::GetSize() const
  {
    return m_vRPN.size();
  }

  //---------------------------------------------------------------------------
  const token_vec_type& RPN::GetData() const
  {
    return m_vRPN;
  }

  //---------------------------------------------------------------------------
  int RPN::GetRequiredStackSize() const
  {
    return m_nMaxStackPos + 1;
  }

  //---------------------------------------------------------------------------
  void RPN::AsciiDump() const
  {
    console() << "Number of tokens: " << m_vRPN.size() << "\n";
    console() << "MaxStackPos:       " << m_nMaxStackPos << "\n";
    for (std::size_t i=0; i<m_vRPN.size(); ++i)
    {
      ptr_tok_type pTok = m_vRPN[i];
      console() << pTok->GetRPNIdx() << " : " << pTok->AsciiDump() << "\n";
    }
  }

MUP_NAMESPACE_END
