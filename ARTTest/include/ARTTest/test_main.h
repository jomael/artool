#ifndef TEST_MAIN_H

#define TEST_MAIN_H

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
#include "Interface.h"

#include "gmock.h"
#include "gtest.h"


#endif /*ARTinterfaceTest*/

#endif /* TEST_MAIN_H */
