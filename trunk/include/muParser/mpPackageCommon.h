#ifndef MU_PACKAGE_COMMON_H
#define MU_PACKAGE_COMMON_H

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
/** \brief Package for installing operators and functions which
           are always present. 
*/
class PackageCommon: public IPackage
{
friend class std::auto_ptr<PackageCommon>;

public:
  
  static IPackage* Instance();
  virtual void AddToParser(ParserXBase *pParser);
  virtual string_type GetDesc() const;
  virtual string_type GetPrefix() const;

private:

  static std::auto_ptr<PackageCommon> s_pInstance;
};

MUP_NAMESPACE_END

#endif
