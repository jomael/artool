#include "mpPackageUnit.h"

#include "mpParserBase.h"


MUP_NAMESPACE_START

/** \brief This is a template for defining unary function callback objects.
*/
#define MUP_POSTFIX_IMLP(CLASS, IDENT, MUL, DESC)                   \
  CLASS::CLASS(IPackage *pPackage)                                 \
    :IOprtPostfix(_T(IDENT), _T("f:f"))                            \
  {}                                                               \
                                                                   \
  void CLASS::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int)        \
  {                                                                \
    *ret = a_pArg[0]->GetFloat() * MUL;                            \
  }                                                                \
                                                                   \
  const char_type* CLASS::GetDesc() const                          \
  {                                                                \
    return _T(DESC);                                               \
  }                                                                \
                                                                   \
  IToken* CLASS::Clone() const                                     \
  {                                                                \
    return new CLASS(*this);                                       \
  }

  MUP_POSTFIX_IMLP(OprtNano,   "{n}",   (float_type)1e-9,   "{n} - unit multiplicator 1e-9")
  MUP_POSTFIX_IMLP(OprtMicro,  "{mu}",  (float_type)1e-6,   "{µ} - unit multiplicator 1e-6")
  MUP_POSTFIX_IMLP(OprtMilli,  "{m}",   (float_type)1e-3,   "{m} - unit multiplicator 1e-3")
  MUP_POSTFIX_IMLP(OprtKilo,   "{k}",   (float_type)1e3,    "{k} - unit multiplicator 1e3")
  MUP_POSTFIX_IMLP(OprtMega,   "{M}",   (float_type)1e6,    "{M} - unit multiplicator 1e6")
  MUP_POSTFIX_IMLP(OprtGiga,   "{G}",   (float_type)1e9,    "{G} - unit multiplicator 1e9")

#undef MUP_POSTFIX_IMLP

//------------------------------------------------------------------------------
std::auto_ptr<PackageUnit> PackageUnit::s_pInstance;

//------------------------------------------------------------------------------
IPackage* PackageUnit::Instance()
{
  if (s_pInstance.get()==NULL)
  {
    s_pInstance.reset(new PackageUnit);
  }

  return s_pInstance.get();
}

//------------------------------------------------------------------------------
void PackageUnit::AddToParser(ParserXBase *pParser)
{
  pParser->DefinePostfixOprt(new OprtNano(this));
  pParser->DefinePostfixOprt(new OprtMicro(this));
  pParser->DefinePostfixOprt(new OprtMilli(this));
  pParser->DefinePostfixOprt(new OprtKilo(this));
  pParser->DefinePostfixOprt(new OprtMega(this));
  pParser->DefinePostfixOprt(new OprtGiga(this));
}

//------------------------------------------------------------------------------
string_type PackageUnit::GetDesc() const
{
  return _T("Postfix operators for basic unit conversions.");
}

//------------------------------------------------------------------------------
string_type PackageUnit::GetPrefix() const
{
  return _T("");
}

MUP_NAMESPACE_END