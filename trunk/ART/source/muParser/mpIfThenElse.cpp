#include "mpIfThenElse.h"

MUP_NAMESPACE_START

  //---------------------------------------------------------------------------
  //
  // A local jump in the RPN
  //
  //---------------------------------------------------------------------------

  TokenIf::TokenIf()
    :IToken(cmIF, g_sCmdCode[ cmIF ])
    ,m_nOffset()
  {}

  //---------------------------------------------------------------------------
  IToken* TokenIf::Clone() const
  {
    return new TokenIf(*this);
  }

  //---------------------------------------------------------------------------
  void TokenIf::SetOffset(int nOffset)
  {
    m_nOffset = nOffset;
  }

  //---------------------------------------------------------------------------
  int TokenIf::GetOffset() const
  {
    return m_nOffset;
  }

  //---------------------------------------------------------------------------
  string_type TokenIf::AsciiDump() const
  {
    stringstream_type ss;

    ss << GetIdent();
    ss << _T(" [addr=0x") << std::hex << this << std::dec;
    ss << _T("; offset=") << m_nOffset;
    ss << _T("]");
    return ss.str();
  }

  //---------------------------------------------------------------------------
  //
  // A local jump in the RPN
  //
  //---------------------------------------------------------------------------

  TokenElse::TokenElse()
    :IToken(cmELSE, g_sCmdCode[ cmELSE ])
    ,m_nOffset()
  {}

  //---------------------------------------------------------------------------
  IToken* TokenElse::Clone() const
  {
    return new TokenElse(*this);
  }

  //---------------------------------------------------------------------------
  void TokenElse::SetOffset(int nOffset)
  {
    m_nOffset = nOffset;
  }

  //---------------------------------------------------------------------------
  int TokenElse::GetOffset() const
  {
    return m_nOffset;
  }

  //---------------------------------------------------------------------------
  string_type TokenElse::AsciiDump() const
  {
    stringstream_type ss;

    ss << GetIdent();
    ss << _T(" [addr=0x") << std::hex << this << std::dec;
    ss << _T("; offset=") << m_nOffset;
    ss << _T("]");
    return ss.str();
  }

MUP_NAMESPACE_END