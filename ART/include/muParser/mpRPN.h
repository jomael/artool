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
#ifndef MUP_RPN_H
#define MUP_RPN_H

#include "mpFwdDecl.h"
#include "mpTypes.h"

//Sadjad: Those stupid macros max and min!!! UNDEFINE!!!!!!!!!!!!!!!!!!!!!!!!
#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

//Sadjad
#if defined(_MSC_VER) && (_MSC_VER <= 1200)
	namespace std
	{
		typedef size_t size_t;
		template <class T> const T& max ( const T& a, const T& b ) {
			return (b<a)?a:b;     // or: return comp(b,a)?a:b; for the comp version
		}

		template <class T> const T& min ( const T& a, const T& b ) {
			return (a<b)?a:b;     // or: return comp(a,b)?a:b; for the comp version
		}
	}
#endif

MUP_NAMESPACE_START

  //---------------------------------------------------------------------------
  class RPN
  {
  public:
    
    RPN();
   ~RPN();
    
    void Add(ptr_tok_type tok);
    void Pop(int num);
    void Reset();
    void AsciiDump() const;

    const token_vec_type& GetData() const;
    std::size_t GetSize() const;

    int GetRequiredStackSize() const;

  private:

    token_vec_type m_vRPN;
    int m_nStackPos;
    int m_nMaxStackPos;
  };

MUP_NAMESPACE_END

#endif