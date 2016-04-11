#ifndef MU_PACKAGE_STR_H
#define MU_PACKAGE_STR_H

//see mpParserBase.h for details on this:
#if defined(_MSC_VER) && (_MSC_VER <= 1200)
	#include "memory.h"
#else
	#include <memory>
#endif

#include "mpIPackage.h"
#include "mpIOprt.h"


MUP_NAMESPACE_START

//------------------------------------------------------------------------------
/** \brief Package for installing unit postfix operators into muParserX. */
class PackageStr : public IPackage
{
friend class std::auto_ptr<PackageStr>;

public:
  
  static IPackage* Instance();
  virtual void AddToParser(ParserXBase *pParser);
  virtual string_type GetDesc() const;
  virtual string_type GetPrefix() const;

private:

  static std::auto_ptr<PackageStr> s_pInstance;
};

MUP_NAMESPACE_END

#endif
