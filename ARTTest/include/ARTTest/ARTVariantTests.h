#ifndef ARTVARIANT_TESTS_H

#define ARTVARIANT_TESTS_H

#ifndef ARTinterfaceTest //Test the ARTinterface functions
#  error "This header file should not be included for any release code."
#else


#if defined (_MSC_VER) && _MSC_VER <= 1200
	#include "common_vs6.hpp"
#else
	#include "common.hpp"
#endif
#ifdef WIN32
#include "windows.h" //just for setting color on the windows console... remove or replace when working on a different system
#else
// cbg: no SetConsoleTextAttribute() function on linux systems
#define SetConsoleTextAttribute(a,b)
// cbg: workaround for unix time measurements
#define GetTickCount() 0
#endif

#include "ART.h"
#include "ARTlink.h"
#include "Cell.h"

#include "gmock.h"
#include "gtest.h"

using ::testing::AtLeast;
using ::testing::Return;

/*
 * Definitions for mock class of ARTvariant
 */

class MockARTvariant : public ARTvariant

{
public:

  MOCK_METHOD2(SetVal, void(const int i, const int ind));
  MOCK_METHOD2(SetVal, void(const double d, const int ind));
  MOCK_METHOD2(SetVal, void(const float f, const int ind));
  MOCK_METHOD2(SetVal, void(std::complex<double>, const int ind));
  MOCK_METHOD3(SetVal, void(const double re, const double im, const int ind));
  MOCK_METHOD1(SetVal, void(const char* s));
  MOCK_METHOD2(SetVal, void(const char* s, int ind));

  MOCK_METHOD2(SetType, void(T_ART_Type t, int l));

  MOCK_METHOD0(GetTypeString, std::string());

  MOCK_METHOD1(IsEqual, bool(ARTvariant* other));
  MOCK_METHOD1(IsEqual, bool(const char* s));

  MOCK_METHOD1(SetArrayLength, void(int l));

  MOCK_METHOD0(clone, ARTvariant*(void));

};


//TEST(ARTVariantChecks, SetVal1);
//TEST(ARTCellChecks, CellChecks1);

#endif /*ARTinterfaceTest*/

#endif /* ARTVARIANT_TESTS_H */
