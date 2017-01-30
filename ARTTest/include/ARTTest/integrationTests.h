#ifndef INTEGRATION_TESTS_H

#define INTEGRATION_TESTS_H

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

#include "gmock.h"
#include "gtest.h"

#include "art.clp"
#include "ARTSimulator.h"
#include "ART.h"
#include "ARTmethod.h"
#include "WaveObject.h"
#include "ARTmodel.h"

#include "ARTtimeModule.h"
#include "timePrototypes.h"
#include "AcousticResearchTool.h"

#define TEST_DEF_START(test_id, suite) \
        class test_id : public TestClass \
                {       \
                        public: \
                                test_id() : TestClass(#test_id, suite) {}

#define CUSTOM_TEST_DEF_START(test_id, suite, superclass) \
        class test_id : public superclass \
                {       \
                        public: \
                                test_id() : superclass(#test_id, suite) {}

#define TEST_DEF_END(test_id) \
        };      \
        test_id * test_id##Object = new test_id ();

class TestClass
{
        private:
                vector<TestClass*> testList;
                string prefix;
                TestClass* suite;

        protected:

                //this is protected, because it must not be run without calling prepare and unprepare
                virtual bool run(); //is overriden by derived tests
        public:

                string description;
                bool success;
                TestClass(string desc = "", TestClass* in_suite = NULL);
                virtual void prepare() { }


                virtual bool runTests();
                virtual void printTree();
                virtual void printSummary(bool top = true);

                virtual void add(TestClass* test);
                virtual void addMessagePrefix(string s);

                virtual void unprepare() { }
};

extern TestClass* AllMyTests;

#endif /*ARTinterfaceTest*/

#endif /* INTEGRATION_TESTS_H */
