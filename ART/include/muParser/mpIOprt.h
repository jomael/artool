/** \file mpIOprt.h
    \brief Definition of base classes needed for parser operator definitions.

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
#ifndef MUP_IPARSER_OPERATOR_H
#define MUP_IPARSER_OPERATOR_H

#include "mpICallback.h"


MUP_NAMESPACE_START

//------------------------------------------------------------------------------
    /** \brief Interface for binary operators.
        \ingroup binop

      All classes representing binary operator callbacks must be derived from
      this base class.
    */
    class IOprtBin : public ICallback
    {
    public:

      IOprtBin(const char_type *a_szIdent, const char_type *a_szArgDef, int nPrec, EOprtAsct eAsc);
      virtual ~IOprtBin();
      virtual string_type AsciiDump() const;

      int GetPri() const;
      EOprtAsct GetAssociativity() const;

    private:

      void CheckPrototype(const string_type &a_szProt);
      int m_nPrec;
      EOprtAsct m_eAsc;
    }; // class IOperator


    //------------------------------------------------------------------------------
    /** \brief Interface for unary postfix operators.
        \ingroup postfix
    */
    class IOprtPostfix : public ICallback
    {
    public:
        IOprtPostfix(const char_type *a_szIdent, const char_type *a_szArgDef);
        virtual ~IOprtPostfix();
        virtual string_type AsciiDump() const;
    }; // class IOperator


    //------------------------------------------------------------------------------
    /** \brief Interface for unary infix operators.
        \ingroup infix
    */
    class IOprtInfix : public ICallback
    {
    public:
        IOprtInfix(const char_type *a_szIdent, const char_type *a_szArgDef);
        virtual ~IOprtInfix();
        virtual string_type AsciiDump() const;
    }; // class IOperator
}  // namespace mu

#endif

