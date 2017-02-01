/*
 * integrationTests.cpp
 *
 *  Created on: 13 Jun 2016
 *      Author: clgeyer
 */

#include "ARTTest/integrationTests.h"
using namespace ART;

//#define _TIMEDEBUG


//this is protected, because it must not be run without calling prepare and unprepare
bool TestClass::run() //is overriden by derived tests
{
  success = true;
  std::cout <<  "\n" << prefix << "SUITE " << description << ":\n";
  //prepare(); //NOT called! ((un)prepare methods are called in loop below, from where run() is called. calling run from here would mean we call it twice...)
  if (testList.size()==0) std::cout << prefix << "\t(empty)\n";
  for (::size_t i = 0; i < testList.size(); i++)
  {
    testList[i]->prepare();
    //make sure that sub-suites (if there are any, prefix this)
    testList[i]->addMessagePrefix(prefix + "\t");
    testList[i]->success = testList[i]->run();
    if (!testList[i]->success)
    {
      success = false;
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
#     ifdef __linux__
        std::cout << "\033[1;31m";
#     endif
        std::cout << prefix << "\t" << testList[i]->description << " FAILED.\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
#     ifdef __linux__
        std::cout << "\033[0m";
#     endif
    }
    else
    {
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN );
#     ifdef __linux__
        std::cout << "\033[1;32m";
#     endif
        std::cout << prefix << "\t" <<  testList[i]->description << " succeded.\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
#     ifdef __linux__
        std::cout << "\033[0m";
#     endif
    }
    testList[i]->unprepare();
  }
  //unprepare();
  return success;
}

TestClass::TestClass(string desc, TestClass* in_suite)
: description(desc)
{
  //add this test to a suite
  if (in_suite)
  {
    in_suite->add(this);
    suite = in_suite;
  }
  else suite = NULL;
}



bool TestClass::runTests() //the public function to run all tests (and (un)prepare them)!
{
  prepare();
  bool success = run();
  unprepare();
  return success;
}

void TestClass::printTree()
{
  std::cout << prefix << "SUITE " << description << ":\n";
  for (::size_t i = 0; i < testList.size(); i++)
  {
    testList[i]->addMessagePrefix(prefix + "\t");
    testList[i]->printTree();
  }
}

void TestClass::printSummary(bool top)
{
  bool overallsuccess = true;
  if (testList.size()!=0) std::cout <<  "\n" << prefix << "SUITE " << description << ":\n";
  for (::size_t i = 0; i < testList.size(); i++)
  {
    testList[i]->addMessagePrefix(prefix + "\t");
    testList[i]->printSummary(false); //false means the test is not top level
    if (!testList[i]->success)
    {
      overallsuccess = false;
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
#     ifdef __linux__
        std::cout << "\033[1;31m";
#     endif
        std::cout << prefix << "\t" << testList[i]->description << " FAILED.\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
#     ifdef __linux__
        std::cout << "\033[0m";
#     endif
    }
    else
    {
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN );
#     ifdef __linux__
        std::cout << "\033[1;32m";
#     endif
        std::cout << prefix << "\t" <<  testList[i]->description << " succeded.\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
#     ifdef __linux__
        std::cout << "\033[0m";
#     endif
    }
  }
  if (top) //if this is the top level, print overall summary
  {
    if (overallsuccess)
    {
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN );
#     ifdef __linux__
        std::cout << "\n\n\033[1;32mAll tests successful!\033[0m\n\n";
#     else
        std::cout << "\n\nAll tests successful!\n\n";
#     endif
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
    }
    else
    {
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
#     ifdef __linux__
        std::cout << "\n\n\033[1;31mSome tests FAILED!\033[0m\n\n";
#     else
        std::cout << "\n\nSome tests FAILED!\n\n";
#     endif
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
    }
  }
}

void TestClass::add(TestClass* test)
{
  testList.push_back(test);
}

void TestClass::addMessagePrefix(string s)
{
  prefix = s;
}



//******************************************************************************************************************************************
//******************************************************************************************************************************************
/*m�glichkeiten tests 2u schreiben:
        unabh�ngige tests
        eine klasse mit prepare und unprepare, testf�lle stammen von dieser klasse ab
        eine klasse mit prepare und unprepare, testf�lle liegen in objekt dieser klasse (prep und unprep auch nur einmal aufgerufen) -> CUSTOM_TEST_DEF_START
*/
//******************************************************************************************************************************************
//******************************************************************************************************************************************

//I pack all test suites into this object
TestClass* AllMyTests = new TestClass("AllMyTests");

#ifndef _TIMEDEBUG
//******************************************************************************************************************************************
TestClass* DataContainerTests = new TestClass("DataContainerTests", AllMyTests);

TEST_DEF_START(initDataContainers, DataContainerTests)
        virtual bool run()
        {
                try
                {
                        ARTvariant* av_int = new ARTvariant(1);
                        if (av_int->typ != C_ART_int) return false;
                        if (av_int->val->i != 1) return false;
                        if (av_int->len != -1) return false;

                        ARTvariant* av_dbl = new ARTvariant(1.1);
                        if (av_dbl->typ != C_ART_dbl) return false;
                        if (av_dbl->val->d != 1.1) return false;
                        if (av_dbl->len != -1) return false;

                        ARTvariant* av_flo = new ARTvariant(1.1f);
                        if (av_flo->typ != C_ART_flo) return false;
                        if (av_flo->val->f != 1.1f) return false;
                        if (av_flo->len != -1) return false;

                        ARTvariant* av_str = new ARTvariant("Ich bin ein Berliner");
                        if (av_str->typ != C_ART_str) return false;
                        if (strcmp(av_str->val->s,"Ich bin ein Berliner")) return false;
                        if (av_str->len != -1) return false;

                        ARTvariant* av_nstr1 = new ARTvariant("Ich bin ein Wiener", "Ich bin ein Berliner");
                        if (av_nstr1->typ != C_ART_nstr) return false;
                        if (strcmp(av_nstr1->val->ns[0],"Ich bin ein Wiener")) return false;
                        if (strcmp(av_nstr1->val->ns[1],"Ich bin ein Berliner")) return false;
                        if (av_nstr1->len != 2) return false;

                        ARTvariant* av_nstr2 = new ARTvariant("Ich bin ein String", "Ich auch", "Ich nicht");
                        if (av_nstr2->typ != C_ART_nstr) return false;
                        if (strcmp(av_nstr2->val->ns[0],"Ich bin ein String")) return false;
                        if (strcmp(av_nstr2->val->ns[1],"Ich auch")) return false;
                        if (strcmp(av_nstr2->val->ns[2],"Ich nicht")) return false;
                        if (av_nstr2->len != 3) return false;

                        ARTvariant* av_nstr3 = new ARTvariant("yksi", "kaksi", "kolme", "nelj�");
                        if (av_nstr3->typ != C_ART_nstr) return false;
                        if (strcmp(av_nstr3->val->ns[0],"yksi")) return false;
                        if (strcmp(av_nstr3->val->ns[1],"kaksi")) return false;
                        if (strcmp(av_nstr3->val->ns[2],"kolme")) return false;
                        if (strcmp(av_nstr3->val->ns[3],"nelj�")) return false;
                        if (av_nstr3->len != 4) return false;

                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage() << "\n";
                        return false;
                }
                return true;
        };
TEST_DEF_END(initDataContainers)

TEST_DEF_START(copyDataContainers, DataContainerTests)
        virtual bool run()
        {
                try
                {
                        ARTvariant* av_int = new ARTvariant(1);
                        ARTvariant* av_copy_int = new ARTvariant(*av_int);
                        if (av_copy_int->typ != C_ART_int) return false;
                        if (av_copy_int->val->i != 1) return false;
                        if (av_copy_int->len != -1) return false;

                        ARTvariant* av_dbl = new ARTvariant(1.1);
                        ARTvariant* av_copy_dbl = new ARTvariant(*av_dbl);
                        if (av_copy_dbl->typ != C_ART_dbl) return false;
                        if (av_copy_dbl->val->d != 1.1) return false;
                        if (av_copy_dbl->len != -1) return false;

                        ARTvariant* av_flo = new ARTvariant(1.1f);
                        ARTvariant* av_copy_flo = new ARTvariant(*av_flo);
                        if (av_copy_flo->typ != C_ART_flo) return false;
                        if (av_copy_flo->val->f != 1.1f) return false;
                        if (av_copy_flo->len != -1) return false;

                        ARTvariant* av_str = new ARTvariant("Ich bin ein Berliner");
                        ARTvariant* av_copy_str = new ARTvariant(*av_str);
                        if (av_copy_str->typ != C_ART_str) return false;
                        if (strcmp(av_copy_str->val->s,"Ich bin ein Berliner")) return false;
                        if (av_copy_str->len != -1) return false;

                        ARTvariant* av_nstr1 = new ARTvariant("Ich bin ein Wiener", "Ich bin ein Berliner");
                        ARTvariant* av_copy_nstr1 = new ARTvariant(*av_nstr1);
                        if (av_copy_nstr1->typ != C_ART_nstr) return false;
                        if (strcmp(av_copy_nstr1->val->ns[0],"Ich bin ein Wiener")) return false;
                        if (strcmp(av_copy_nstr1->val->ns[1],"Ich bin ein Berliner")) return false;
                        if (av_copy_nstr1->len != 2) return false;

                        ARTvariant* av_nstr2 = new ARTvariant("Ich bin ein String", "Ich auch", "Ich nicht");
                        ARTvariant* av_copy_nstr2 = new ARTvariant(*av_nstr2);
                        if (av_copy_nstr2->typ != C_ART_nstr) return false;
                        if (strcmp(av_copy_nstr2->val->ns[0],"Ich bin ein String")) return false;
                        if (strcmp(av_copy_nstr2->val->ns[1],"Ich auch")) return false;
                        if (strcmp(av_copy_nstr2->val->ns[2],"Ich nicht")) return false;
                        if (av_copy_nstr2->len != 3) return false;

                        ARTvariant* av_nstr3 = new ARTvariant("yksi", "kaksi", "kolme", "nelj�");
                        ARTvariant* av_copy_nstr3 = new ARTvariant(*av_nstr3);
                        if (av_copy_nstr3->typ != C_ART_nstr) return false;
                        if (strcmp(av_copy_nstr3->val->ns[0],"yksi")) return false;
                        if (strcmp(av_copy_nstr3->val->ns[1],"kaksi")) return false;
                        if (strcmp(av_copy_nstr3->val->ns[2],"kolme")) return false;
                        if (strcmp(av_copy_nstr3->val->ns[3],"nelj�")) return false;
                        if (av_copy_nstr3->len != 4) return false;

                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage() << "\n";
                        return false;
                }
                return true;
        };
TEST_DEF_END(copyDataContainers)

TEST_DEF_START(compareDataContainers, DataContainerTests)
        virtual bool run()
        {
                try
                {
                        ARTvariant* av_int = new ARTvariant(1);
                        ARTvariant* av_copy_int = new ARTvariant(*av_int);
                        if ( !av_copy_int->IsEqual(av_int) ) return false;
                        if ( !av_int->IsEqual(av_copy_int) ) return false;

                        ARTvariant* av_dbl = new ARTvariant(1.1);
                        ARTvariant* av_copy_dbl = new ARTvariant(*av_dbl);
                        if ( !av_copy_dbl->IsEqual(av_dbl) ) return false;

                        ARTvariant* av_flo = new ARTvariant(1.1f);
                        ARTvariant* av_copy_flo = new ARTvariant(*av_flo);
                        if ( !av_flo->IsEqual(av_copy_flo) ) return false;

                        ARTvariant* av_str = new ARTvariant("Ich bin ein Berliner");
                        ARTvariant* av_copy_str = new ARTvariant(*av_str);
                        if ( !av_copy_str->IsEqual(av_str) ) return false;

                        ARTvariant* av_nstr1 = new ARTvariant("Ich bin ein Wiener", "Ich bin ein Berliner");
                        ARTvariant* av_copy_nstr1 = new ARTvariant(*av_nstr1);
                        if ( !av_nstr1->IsEqual(av_copy_nstr1) ) return false;

                        ARTvariant* av_nstr2 = new ARTvariant("Ich bin ein String", "Ich auch", "Ich nicht");
                        ARTvariant* av_copy_nstr2 = new ARTvariant(*av_nstr2);
                        if ( !av_nstr2->IsEqual(av_copy_nstr2) ) return false;

                        ARTvariant* av_nstr3 = new ARTvariant("yksi", "kaksi", "kolme", "nelj�");
                        ARTvariant* av_copy_nstr3 = new ARTvariant(*av_nstr3);
                        if ( !av_copy_nstr3->IsEqual(av_nstr3) ) return false;

                        //if it is the same it should be equal:
                        if ( !av_copy_int->IsEqual(av_copy_int) ) return false;
                        if ( !av_copy_dbl->IsEqual(av_copy_dbl) ) return false;
                        if ( !av_copy_flo->IsEqual(av_copy_flo) ) return false;
                        if ( !av_copy_str->IsEqual(av_copy_str) ) return false;
                        if ( !av_copy_nstr1->IsEqual(av_copy_nstr1) ) return false;
                        if ( !av_copy_nstr2->IsEqual(av_copy_nstr2) ) return false;
                        if ( !av_copy_nstr3->IsEqual(av_copy_nstr3) ) return false;
                        if ( !av_int->IsEqual(av_int) ) return false;
                        if ( !av_dbl->IsEqual(av_dbl) ) return false;
                        if ( !av_flo->IsEqual(av_flo) ) return false;
                        if ( !av_str->IsEqual(av_str) ) return false;
                        if ( !av_nstr1->IsEqual(av_nstr1) ) return false;
                        if ( !av_nstr2->IsEqual(av_nstr2) ) return false;
                        if ( !av_nstr3->IsEqual(av_nstr3) ) return false;

                        //and these should not be equal:
                        if ( av_int->IsEqual(av_dbl) ) return false;
                        if ( av_dbl->IsEqual(av_flo) ) return false;
                        if ( av_flo->IsEqual(av_str) ) return false;
                        if ( av_str->IsEqual(av_int) ) return false;
                        if ( av_nstr1->IsEqual(av_nstr2) ) return false;
                        if ( av_nstr2->IsEqual(av_str) ) return false;
                        if ( av_nstr3->IsEqual(av_dbl) ) return false;

                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage() << "\n";
                        return false;
                }
                return true;
        };
TEST_DEF_END(compareDataContainers)


TEST_DEF_START(createDataContainerParserVars, DataContainerTests)
        virtual bool run()
        {
                try
                {
                        DataContainer* testDC = new DataContainer(C_ART_dbl, -1, "meineTestVar");
                        ParserX* parser = new ParserX(mup::pckCOMPLEX_NO_STRING);
                        testDC->SetParser(parser);
                        testDC->SetParserVar("meineTestVar");

                        //Check if variable exists
                        //cout << "testDC value = " << testDC->GetValueAsDouble() << endl;
                        parser->SetExpr("meineTestVar = 2 + 2 ");
                        parser->Eval(); //will throw an error if the variable does not exist

                        //cout << "testDC value (should be 4) = " << testDC->GetValueAsDouble() << endl;

                        delete testDC;

                        try
                        {
                                parser->SetExpr("meineTestVar = 2 + 2 ");
                                parser->Eval(); //will throw an error if the variable does not exist -> it should NOT exist anymore
                        }
                        catch(mup::ParserError e)
                        {
                                return true;
                        }

                }
                catch(mup::ParserError e)
                {
                        std::cout << "\n\n" << e.GetMsg() << "\n";
                        return false;
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage() << "\n";
                        return false;
                }
                return false; //should not reach this point! exception is expected!!
        };
TEST_DEF_END(createDataContainerParserVars)

//******************************************************************************************************************************************

TestClass* ARTpreperationFunctions = new TestClass("ARTpreperationFunctions", AllMyTests);

TEST_DEF_START(initRootObject, ARTpreperationFunctions)
        virtual bool run()
        {
                try
                {
                        ARTRootObject();
                        ARTRootDestroy();
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage() << "\n";
                        return false;
                }
                return true;
        };
TEST_DEF_END(initRootObject)


TEST_DEF_START(addTopLevelProps, ARTpreperationFunctions)

        P_ART_Object art;

        virtual void prepare()
        {
                art = ARTRootObject();
        }

        virtual bool run()
        {
                try
                {
                        ARTvariant* numbers = new ARTvariant(C_ART_nstr, 6);
                        ARTSetString((ARTdataProp*)numbers,0,"ek");
                        ARTSetString((ARTdataProp*)numbers,1,"do");
                        ARTSetString((ARTdataProp*)numbers,2,"tiin");
                        ARTSetString((ARTdataProp*)numbers,3,"chaar");
                        ARTSetString((ARTdataProp*)numbers,4,"paanch");
                        ARTSetString((ARTdataProp*)numbers,5,"che");
                        art->AppendDataProp("ginti", numbers);
                        //should also work like this
                        ARTdataProp* numbers2 = new ARTdataProp(C_ART_nstr, 6, "laskea");
                        ARTSetString(numbers2,0,"yksi");
                        ARTSetString(numbers2,1,"kaksi");
                        ARTSetString(numbers2,2,"kolme");
                        ARTSetString(numbers2,3,"nelj�");
                        ARTSetString(numbers2,4,"viisi");
                        ARTSetString(numbers2,5,"kuusi");
                        art->AppendDataProp(numbers2);
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage() << "\n";
                        return false;
                }
                return true;
        };

        virtual void unprepare()
        {
                ARTRootDestroy();
        };
TEST_DEF_END(addTopLevelProps)


TEST_DEF_START(addTopLevelPropsTwice, ARTpreperationFunctions)

        P_ART_Object art;

        virtual void prepare()
        {
                art = ARTRootObject();
        }

        virtual bool run()
        {
                try
                {
                        ARTvariant* authors = new ARTvariant(C_ART_nstr, 2);
                        ARTSetString((ARTdataProp*)authors,0,"Dick");
                        ARTSetString((ARTdataProp*)authors,1,"Doof");
                        art->AppendDataProp("Authors", authors); //should fail, we already have a prop "Authors"
                }
                catch(ARTerror e)
                {
                        return true;
                }
                return false;
        };

        virtual void unprepare()
        {
                ARTRootDestroy();
        };
TEST_DEF_END(addTopLevelPropsTwice)


TEST_DEF_START(initSimulator, ARTpreperationFunctions)
        virtual void prepare()
        {
                        ARTRootObject();
        }
        virtual bool run() //test creating and destroying of simulator
        {
                try
                {
                        ARTRootObject();
                        P_ART_Simulator mySim = ARTCreateSimulator("MeinSimulator", "FrequencyDomain", "MultiModal");
                        ARTDestroySimulator(mySim);
                        ARTRootDestroy();
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage() << "\n";
                        return false;
                }
                return true;
        }
        virtual void unprepare()
        {
                        ARTRootDestroy();
        }
TEST_DEF_END(initSimulator)


TEST_DEF_START(initWrongSimulator, ARTpreperationFunctions)
        virtual bool run() //test creating and destroying of simulator //test creating simulator with wrong properties
        {
                try
                {
                        ARTRootObject();
                        P_ART_Simulator mySim = ARTCreateSimulator("MeinSimulator", "SomethingNonexistant", "MultiModal");
                        ARTDestroySimulator(mySim);
                        ARTRootDestroy();
                }
                catch(ARTerror e)
                {
                        return true;
                }
                return false; //should not be successfull!
        }
TEST_DEF_END(initWrongSimulator)

TEST_DEF_START(initSimulatorTwice, ARTpreperationFunctions)

        P_ART_Simulator mySim;

        virtual void prepare()
        {
                ARTRootObject();
                mySim = ARTCreateSimulator("MeinSimulator", "FrequencyDomain", "MultiModal");
        }

        virtual bool run() //test creating and destroying simulator with same name twice
        {
                try
                {
                        P_ART_Simulator mySim2 = ARTCreateSimulator("MeinSimulator", "FrequencyDomain", "MultiModal");
                        ARTDestroySimulator(mySim2);
                }
                catch(ARTerror e)
                {
                        return true;
                }
                return false; //should not be successfull!
        }

        virtual void unprepare()
        {
                ARTDestroySimulator(mySim);
                ARTRootDestroy();
        }
TEST_DEF_END(initSimulatorTwice)


TEST_DEF_START(changeSimulatorParams, ARTpreperationFunctions)

        P_ART_Simulator mySim;

        virtual void prepare()
        {
                ARTRootObject();
                mySim = ARTCreateSimulator("MeinSimulator", "FrequencyDomain", "MultiModal");
        }

        virtual bool run()
        {
                try
                {
                        // set parameters
                        ARTSetParameter(mySim, "LowerFrequencyLimit = 1");
                        ARTSetParameter(mySim, "HigherFrequencyLimit = 1000");
                        ARTSetParameter(mySim, "FrequencyStep = 11; NumberOfModes = 2");

                        // check their values
                        ARTdataProp* p = ARTFindDataProperty(mySim,"LowerFrequencyLimit");
                        if (p->GetFloat() != 1) return false;
                        p = ARTFindDataProperty(mySim,"HigherFrequencyLimit");
                        if (p->GetFloat() != 1000) return false;
                        p = ARTFindDataProperty(mySim,"FrequencyStep");
                        if (p->GetFloat() != 11) return false;
                        p = ARTFindDataProperty(mySim,"NumberOfModes");
                        if (p->GetFloat() != 2) return false;
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }

        virtual void unprepare()
        {
                ARTDestroySimulator(mySim);
                ARTRootDestroy();
        }
TEST_DEF_END(changeSimulatorParams)


TEST_DEF_START(getSimulatorParamsDef, ARTpreperationFunctions)

        P_ART_Simulator mySim;

        virtual void prepare()
        {
                ARTRootObject();
                mySim = ARTCreateSimulator("MeinSimulator", "FrequencyDomain", "MultiModal");
        }

        virtual bool run()
        {
                try
                {
                        // get all parameters as strings
                        ARTdataProp* prop = NULL;
                        string allParams = "";
                        while ( (prop = ARTGetDataProperties(mySim, prop)) )
                        {
                                allParams += string(ARTGetDefinitionString(prop)) + ",";
                        }

                        // check if parameter strings are as expected
                        //Of course this test will fail when the properties of the simulator are changed... but for now it checks that the receiving of parameters works.
                        //std::cout << allParams;
                        if ("NumberOfModes = 1,LowerFrequencyLimit = 50,HigherFrequencyLimit = 1800,FrequencyStep = 5," != allParams) return false;

                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }

        virtual void unprepare()
        {
                ARTDestroySimulator(mySim);
                ARTRootDestroy();
        }
TEST_DEF_END(getSimulatorParamsDef)


TEST_DEF_START(getSimulatorParamsVal, ARTpreperationFunctions)

        P_ART_Simulator mySim;

        virtual void prepare()
        {
                ARTRootObject();
                mySim = ARTCreateSimulator("MeinSimulator", "FrequencyDomain", "MultiModal");
        }

        virtual bool run()
        {
                try
                {
                        ARTdataProp* dp = ARTFindDataProperty(mySim,"LowerFrequencyLimit");
                        ARTvariant* av = ARTGetValue( dp );
                        double f_min = ARTGetDouble( av , 0 );

                        dp = ARTFindDataProperty(mySim,"HigherFrequencyLimit");
                        av = ARTGetValue( dp );
                        double f_max = ARTGetDouble( av , 0 );

                        // Standard values are f_min = 50 and f_max = 1800. Check!
                        if (f_min != 50) return false;
                        if (f_max != 1800) return false;
                        //std::cout << f_min << " " << f_max << "\n";;

                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }

        virtual void unprepare()
        {
                ARTDestroySimulator(mySim);
                ARTRootDestroy();
        }
TEST_DEF_END(getSimulatorParamsVal)


TEST_DEF_START(getDataPropFromObjectWhereNoDataProp, ARTpreperationFunctions)

        P_ART_Simulator mySim;
        P_ART_Object o;

        virtual void prepare()
        {
                ARTRootObject();
                mySim = ARTCreateSimulator("MeinSimulator", "FrequencyDomain", "MultiModal");
                o = new ARTobject("TestObject", "A test object that does not have data properties");
                o->AppendMethod("A method");
                o->AppendProperty("A property");
                o->AppendProperty("Another property");
        }

        virtual bool run() //test creating and destroying simulator with same name twice
        {
                try
                {
                        // try to get a data property, this should simply return NULL
                        ARTdataProp* prop = ARTGetDataProperties(o, NULL);
                        if (prop) return false;
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }

        virtual void unprepare()
        {
                delete o;
                ARTDestroySimulator(mySim);
                ARTRootDestroy();
        }
TEST_DEF_END(getDataPropFromObjectWhereNoDataProp)


TEST_DEF_START(createElementTwice, ARTpreperationFunctions)

        P_ART_Simulator mySim;
        P_ART_Element El1;
        P_ART_Element El2;

        virtual void prepare()
        {
                        ARTRootObject();
                        mySim = ARTCreateSimulator("MeinSimulator", "FrequencyDomain", "MultiModal");
                        El1 = NULL;
                        El2 = NULL;
        }

        virtual bool run() //test using the same element name twice
        {
                try
                {
                        El1 = ARTCreateElement(mySim, "Twice", "BentCylinder");
                        El2 = ARTCreateElement(mySim, "Twice", "Cylinder");
                }
                catch(ARTerror e)
                {
                        return true;
                }
                return false; //should not be successfull!
        }


        virtual void unprepare()
        {
                if (El1 != NULL)
                {
                        ARTDestroyElement(mySim, El1);
                }
                if (El2 != NULL)
                {
                        ARTDestroyElement(mySim, El2);
                }
                ARTDestroySimulator(mySim);
                ARTRootDestroy();
        }

TEST_DEF_END(createElementTwice)


TEST_DEF_START(createElements, ARTpreperationFunctions)

        P_ART_Simulator mySim;
        P_ART_Element El1;
        P_ART_Element El2;

        virtual void prepare()
        {
                        ARTRootObject();
                        mySim = ARTCreateSimulator("MeinSimulator", "FrequencyDomain", "MultiModal");

        }

        virtual bool run() //test creating elements
        {
                try
                {
                        El1 = ARTCreateElement(mySim, "BC", "BentCylinder");
                        El2 = ARTCreateElement(mySim, "C", "Cylinder");
                }
                catch(ARTerror e)
                {
                        return false;
                }
                return true;
        }


        virtual void unprepare()
        {
                if (El1 != NULL)
                {
                        ARTDestroyElement(mySim, El1);
                }
                if (El2 != NULL)
                {
                        ARTDestroyElement(mySim, El2);
                }
                ARTDestroySimulator(mySim);
                ARTRootDestroy();
        }

TEST_DEF_END(createElements)

TEST_DEF_START(createAndDestroyElements, ARTpreperationFunctions)

        P_ART_Simulator mySim;

        virtual void prepare()
        {
                        ARTRootObject();
                        mySim = ARTCreateSimulator("MeinSimulator", "FrequencyDomain", "MultiModal");
        }

        virtual bool run() //test creating elements
        {
                ParserX* parser = mySim->GetParser();
                try
                {
                        P_ART_Element El2 = ARTCreateElement(mySim, "C", "Cylinder");
                        //check if parser variables exist
                        parser->SetExpr("C.length = 2 + 2");
                        parser->Eval(); //will throw an error if the variable does not exist

                        ARTDestroyElement(mySim,El2);
                }
                catch(mup::ParserError e)
                {
                        return false;
                }
                catch(ARTerror e)
                {
                        return false;
                }
                return true;

                //check if the parser vars are destroyed
                try
                {
                        parser->SetExpr("C.length = 2");
                        parser->Eval(); //will throw an error if the variable does not exist

                }
                catch(mup::ParserError e)
                {
                        return true; //error expected!
                }
                return false;
        }


        virtual void unprepare()
        {
                        ARTDestroySimulator(mySim);
                        ARTRootDestroy();
        }

TEST_DEF_END(createAndDestroyElements)



TEST_DEF_START(changeElementModelType, ARTpreperationFunctions)

        P_ART_Simulator mySim;

        virtual void prepare()
        {
                        ARTRootObject();
                        mySim = ARTCreateSimulator("MeinSimulator", "FrequencyDomain", "MultiModal");
        }

        virtual bool run() //test creating elements
        {
                int exceptionsCaught = 0;
                int exceptionsExpected = 0;
                ParserX* parser = mySim->GetParser();
                try
                {
                        P_ART_Element El2 = ARTCreateElement(mySim, "Wandler", "BoreJump");

                        //check if parser variables exist
                        parser->SetExpr("Wandler.r1 = 2 + 2"); //BoreJump has R1, Cyl. does not
                        parser->Eval(); //will throw an error if the variable does not exist

                        //check that parser variable does not exist
                        try
                        {
                                exceptionsExpected++;
                                parser->SetExpr("Wandler.length = 2 + 2"); //Cyl has length, BoreJump does not
                                parser->Eval(); //will throw an error if the variable does not exist
                        } catch (mup::ParserError e) {exceptionsCaught++;}

                        ARTChangeElementModel(mySim, El2, "Cylinder");

                        //check if parser variables exist
                        parser->SetExpr("Wandler.length = 2 + 2");
                        parser->Eval(); //will throw an error if the variable does not exist

                        //check that parser variable does not exist
                        try
                        {
                                exceptionsExpected++;
                                parser->SetExpr("Wandler.r1 = 2 + 2");
                                parser->Eval(); //will throw an error if the variable does not exist
                        } catch (mup::ParserError e) {exceptionsCaught++;}


                        ARTDestroyElement(mySim,El2);

                        //check that parser variable does not exist
                        try
                        {
                                exceptionsExpected++;
                                parser->SetExpr("Wandler.r1 = 2 + 2");
                                parser->Eval(); //will throw an error if the variable does not exist
                        } catch (mup::ParserError e) {exceptionsCaught++;}

                                                //check that parser variable does not exist
                        try
                        {
                                exceptionsExpected++;
                                parser->SetExpr("Wandler.length = 2 + 2");
                                parser->Eval(); //will throw an error if the variable does not exist
                        } catch (mup::ParserError e) {exceptionsCaught++;}

                        if (exceptionsCaught != exceptionsExpected) return false;
                }
                catch(mup::ParserError e)
                {
                        std::cout << "\n\n" << e.GetMsg() << "\n";
                        return false;
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage() << "\n";
                        return false;
                }
                return true;

        }


        virtual void unprepare()
        {
                        ARTDestroySimulator(mySim);
                        ARTRootDestroy();
        }

TEST_DEF_END(changeElementModelType)


TEST_DEF_START(changeElementName, ARTpreperationFunctions)

        P_ART_Simulator mySim;

        virtual void prepare()
        {
                        ARTRootObject();
                        mySim = ARTCreateSimulator("MeinSimulator", "FrequencyDomain", "MultiModal");
        }

        virtual bool run()
        {
                int exceptionsCaught = 0;
                int exceptionsExpected = 0;
                ParserX* parser = mySim->GetParser();
                try
                {
                        P_ART_Element El2 = ARTCreateElement(mySim, "Wandler", "Cylinder");
                        //change a parameter, so that a definition string is created internally
                        ARTSetParameter(mySim, "Wandler.temp = 20");

                        ARTChangeName( El2, "Cyl");

                        //check that a parser variable (with "Wandler") does not exist after name change
                        try
                        {
                                exceptionsExpected++;
                                parser->SetExpr("Wandler.length = 2 + 2"); //Wandler does not exist anymore -> error expected
                                parser->Eval(); //will throw an error if the variable does not exist
                        } catch (mup::ParserError e) {exceptionsCaught++;}

                        //check that variable name in definition string was also changed
                        const char* def = ARTGetDefinitionString( ARTFindDataProperty( ARTGetModel(El2), "temp") );
                        if (strcmp(def, "Cyl.temp = 20") != 0) return false;

                        ARTChangeName( El2, "Cylin");

                        //check if parser variable with "Cylin" exist
                        parser->SetExpr("Cylin.r = 2 + 2");
                        parser->Eval(); //will throw an error if the variable does not exist

                        //check that parser variable with "Cyl" does not exist anymore
                        try
                        {
                                exceptionsExpected++;
                                parser->SetExpr("Cyl.r = 2 + 2");
                                parser->Eval(); //will throw an error if the variable does not exist
                        } catch (mup::ParserError e) {exceptionsCaught++;}

                        //check that variable name in definition string was also changed
                        def = ARTGetDefinitionString( ARTFindDataProperty( ARTGetModel(El2), "temp") );
                        if (strcmp(def, "Cylin.temp = 20") != 0) return false;

                        ARTDestroyElement(mySim,El2);

                        //check that no old parser variable exists anymore
                        try
                        {
                                exceptionsExpected++;
                                parser->SetExpr("Wandler.r = 2 + 2");
                                parser->Eval(); //will throw an error if the variable does not exist
                        } catch (mup::ParserError e) {exceptionsCaught++;}

                        try
                        {
                                exceptionsExpected++;
                                parser->SetExpr("Cyl.length = 2 + 2");
                                parser->Eval(); //will throw an error if the variable does not exist
                        } catch (mup::ParserError e) {exceptionsCaught++;}

                        try
                        {
                                exceptionsExpected++;
                                parser->SetExpr("Cylin.length = 2 + 2");
                                parser->Eval(); //will throw an error if the variable does not exist
                        } catch (mup::ParserError e) {exceptionsCaught++;}

                        if (exceptionsCaught != exceptionsExpected) return false;
                }
                catch(mup::ParserError e)
                {
                        std::cout << "\n\n" << e.GetMsg() << "\n";
                        return false;
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage() << "\n";
                        return false;
                }
                return true;

        }


        virtual void unprepare()
        {
                        ARTDestroySimulator(mySim);
                        ARTRootDestroy();
        }

TEST_DEF_END(changeElementName)


//******************************************************************************************************************************************

//If Tests need preperation (and unpreperation), define the suite to be a derived class ...
class ARTsettingElementParametersClass : public TestClass
{
        public:
                P_ART_Simulator mySim;
                P_ART_Element Con;
                P_ART_Element Zyl1;
                P_ART_Element BZyl1;

                ARTsettingElementParametersClass() : TestClass("ARTsettingElementParameters", AllMyTests) {}

                virtual void prepare()
                {
                        ARTRootObject();
                        mySim = ARTCreateSimulator("MeinSimulator", "FrequencyDomain", "MultiModal");
                        Con = ARTCreateElement(mySim, "Con", "Cone");
                        Zyl1 = ARTCreateElement(mySim, "Zyl", "Cylinder");
                        BZyl1 = ARTCreateElement(mySim, "BZyl", "BentCylinder");
                }
                virtual void unprepare()
                {
                        ARTDestroyElement(mySim, Con);
                        ARTDestroyElement(mySim, Zyl1);
                        ARTDestroyElement(mySim, BZyl1);
                        ARTDestroySimulator(mySim);
                        ARTRootDestroy();
                }
};
// ... and instantiate one object of that class
ARTsettingElementParametersClass* ARTsettingElementParameters = new ARTsettingElementParametersClass();

//Now add all the tests to the object, as usual
TEST_DEF_START(setValidParameters, ARTsettingElementParameters)
        P_ART_Simulator mySim;
        virtual void prepare()
        {
                 mySim = ARTsettingElementParameters->mySim;
        }
        virtual bool run() //test calculating impedance twice
        {
                try
                {
                        ARTSetParameter(mySim, "Con.length = 11.123; ");
                        ARTSetParameter(mySim, "Con.r1 = Con.r2*0.34;");
                        ARTSetParameter(mySim, "Con.r2 = 4");
                        ARTSetParameter(mySim, "Zyl.r = 4;Zyl.temp=20");
                        ARTSetParameter(mySim, "  BZyl.bendradius= 13124;BZyl.temp=Zyl.temp;");
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }
TEST_DEF_END(setValidParameters)




TEST_DEF_START(getDefString, ARTsettingElementParameters)
        P_ART_Simulator mySim;
        virtual void prepare()
        {
                 mySim = ARTsettingElementParameters->mySim;
        }
        virtual bool run() //test calculating impedance twice
        {
                try
                {
                        string s1 = "Con.r1 = Con.r2*0.34";
                        P_ART_DataProp dp = ARTSetParameter(mySim, "Con.r1 = Con.r2*0.34");
                        string s2 = string(ARTGetDefinitionString(dp));
                        if (s1 != s2) return false;
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }
TEST_DEF_END(getDefString)




TEST_DEF_START(getAllDefStrings, ARTsettingElementParameters)

        P_ART_Simulator mySim;
        P_ART_Element TestCon;

        virtual void prepare()
        {
                 mySim = ARTsettingElementParameters->mySim;
                 TestCon = ARTCreateElement(mySim,"TestCon","Cone");
        }
        virtual bool run() //test calculating impedance twice
        {
                try
                {
                        ARTSetParameter(mySim, "TestCon.r1 = TestCon.r2*0.34");

                        P_ART_Object model = ARTGetModel(TestCon);

                        P_ART_DataProp dp = ARTGetDataProperties(model, NULL);

                        string s = "";
                        do
                        {
                                s += string(ARTGetDefinitionString(dp)) + ";";
                                dp = ARTGetDataProperties(model, dp);
                        }
                        while (dp);

                        if (s != "TestCon.length = 50;TestCon.r1 = TestCon.r2*0.34;TestCon.r2 = 5;TestCon.temp = 21;TestCon.loss = 1;TestCon.humidity = 0.8;TestCon.xc = 0.000382;")
                                return false;

                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }
        virtual void unprepare()
        {
                 ARTDestroyElement(mySim,TestCon);
        }
TEST_DEF_END(getAllDefStrings)



TEST_DEF_START(getModelDataProps, ARTsettingElementParameters)

        P_ART_Simulator mySim;
        P_ART_Element Con;

        virtual void prepare()
        {
                 mySim = ARTsettingElementParameters->mySim;
                 Con = ARTsettingElementParameters->Con;
        }
        virtual bool run() //test calculating impedance twice
        {
                try
                {
                        P_ART_Object model = ARTGetModel(Con);
                        P_ART_DataProp dp = ARTGetDataProperties(model, NULL);
                        string s = "";
                        do
                        {
                                s +=  string(ARTGetName(dp)) + ",";
                                dp = ARTGetDataProperties(model, dp);
                        }
                        while (dp);
                        //compare with what we expect (this is a fragile test... the order and number of properties could change, then this test would fail...)
                        if (s=="length,r1,r2,temp,loss,humidity,xc,") return true;

                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }
TEST_DEF_END(getModelDataProps)


TEST_DEF_START(setStringParameters, ARTsettingElementParameters)
        P_ART_Simulator mySim;
        P_ART_Element Term;

        virtual void prepare()
        {
                 mySim = ARTsettingElementParameters->mySim;
                 Term = ARTCreateElement(mySim, "Term", "TerminationModel");
        }

        virtual bool run() //test calculating impedance twice
        {
                try
                {
                        ARTSetParameter(mySim, "Term.radiation = Zorumski;");
                        ARTSetParameter(mySim, "Term.radiation = Reflecting;");
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }

        virtual void unprepare()
        {
                 ARTDestroyElement(mySim, Term);
        }

TEST_DEF_END(setStringParameters)


TEST_DEF_START(setInvalidParameters, ARTsettingElementParameters)
        P_ART_Simulator mySim;
        virtual void prepare()
        {
                 mySim = ARTsettingElementParameters->mySim;
        }
        virtual bool run() //test calculating impedance twice
        {
                //all parameter strings contain errors and should generate exceptions
                int exceptionsExpected = 0;
                int exceptionsCaught = 0;
                try
                {
                        exceptionsExpected++;
                        ARTSetParameter(mySim, "Con.LENGT = 11.123; ");
                }
                catch(ARTerror e)
                {
                        exceptionsCaught++;
                }
                try
                {
                        exceptionsExpected++;
                        ARTSetParameter(mySim, "Con.r2  4");
                }
                catch(ARTerror e)
                {
                        exceptionsCaught++;
                }
                try
                {
                        exceptionsExpected++;
                        ARTSetParameter(mySim, "Zyl.r = 4 Zyl.temp=20");
                }
                catch(ARTerror e)
                {
                        exceptionsCaught++;
                }
                try
                {
                        exceptionsExpected++;
                        ARTSetParameter(mySim, "Con.r1 = Ccon.r2*0.34;");
                }
                catch(ARTerror e)
                {
                        exceptionsCaught++;
                }
                try
                {
                        exceptionsExpected++;
                        ARTSetParameter(mySim, "  BZyl.bendradius= ;BZyl.temp=Zyl.temp;");
                }
                catch(ARTerror e)
                {
                        exceptionsCaught++;
                }

                if (exceptionsCaught==exceptionsExpected) return true;
                        else return false;
        }
TEST_DEF_END(setInvalidParameters)

//******************************************************************************************************************************************

//This is the suite for the tests
TestClass* ARTreferencesInCircuits = new TestClass("ARTreferencesInCircuitsClass", AllMyTests);

//This is a "template" for the tests... what they all have to prepare and unprepare (same as writing the same prepare and unprepare stuff in every test class)
class ARTreferencesInCircuitsSuperClass : public TestClass
{
        public:
                P_ART_Simulator mySim;
                P_ART_Element Zyl1;
                P_ART_Element Kon1;
                P_ART_Element Zyl2;
                P_ART_Element Kon2;
                P_ART_Circuit meinIns;

                ARTreferencesInCircuitsSuperClass(string desc = "ARTreferencesInCircuits", TestClass* in_suite = AllMyTests)
                : TestClass(desc, in_suite) {}

                virtual void prepare()
                {
                        ARTRootObject();
                        mySim = ARTCreateSimulator("MeinSimulator", "FrequencyDomain", "MultiModal");
                        ARTSetFrequencyRange    (mySim, 10, 20, 5);
                        ARTSetNModes    (mySim, 1);
                        Zyl1 = ARTCreateElement(mySim, "Zyl1", "Cylinder");
                        Kon1 = ARTCreateElement(mySim, "Kon1", "Cone");
                        Zyl2 = ARTCreateElement(mySim, "Zyl2", "Cylinder");
                        Kon2 = ARTCreateElement(mySim, "Kon2", "Cone");
                        meinIns = ARTCreateCircuit(mySim, "MeinInstrument");
                        ARTAppendReference( meinIns, Zyl1);
                        ARTAppendReference( meinIns, Zyl2);
                        ARTAppendReference( meinIns, Kon1);
                        ARTAppendReference( meinIns, Kon2);
                }
                virtual void unprepare()
                {
                        ARTDestroyCircuit(mySim, meinIns);
                        ARTDestroyElement(mySim, Zyl1);
                        ARTDestroyElement(mySim, Kon1);
                        ARTDestroyElement(mySim, Zyl2);
                        ARTDestroyElement(mySim, Kon2);
                        ARTDestroySimulator(mySim);
                        ARTRootDestroy();
                }
};

CUSTOM_TEST_DEF_START(getReferencePosition, ARTreferencesInCircuits, ARTreferencesInCircuitsSuperClass)
        virtual bool run() //test destroying a referenced element, then evaluating the circuits impedance -> should generate error
        {
                try
                {
                        if (ARTGetReferencePosition( meinIns,Zyl1) != 0) return false;
                        if (ARTGetReferencePosition( meinIns,Zyl2) != 1) return false;
                        if (ARTGetReferencePosition( meinIns,Kon1) != 2) return false;
                        if (ARTGetReferencePosition( meinIns,Kon2) != 3) return false;
                }
                catch(ARTerror e)
                {
                        return false;
                }
                return true;
        }
TEST_DEF_END(getReferencePosition)

CUSTOM_TEST_DEF_START(getReferencePosition2, ARTreferencesInCircuits, ARTreferencesInCircuitsSuperClass)
        virtual bool run() //test destroying a referenced element, then evaluating the circuits impedance -> should generate error
        {
                try
                {
                        ARTRemoveReference( meinIns,Zyl2);
                        if (ARTGetReferencePosition( meinIns,Zyl1) != 0) return false;
                        if (ARTGetReferencePosition( meinIns,Zyl2) != -1) return false;
                        if (ARTGetReferencePosition( meinIns,Kon1) != 1) return false;
                        if (ARTGetReferencePosition( meinIns,Kon2) != 2) return false;
                }
                catch(ARTerror e)
                {
                        return false;
                }
                return true;
        }
TEST_DEF_END(getReferencePosition2)

CUSTOM_TEST_DEF_START(replaceReference, ARTreferencesInCircuits, ARTreferencesInCircuitsSuperClass)
        virtual bool run() //test destroying a referenced element, then evaluating the circuits impedance -> should generate error
        {
                try
                {
                        ARTRemoveReference( meinIns,Kon1);
                        ARTReplaceReference( meinIns,Zyl1,Kon1);
                        if (ARTGetReferencePosition( meinIns,Kon1) != 0) return false;
                        if (ARTGetReferencePosition( meinIns,Zyl1) != -1) return false;
                        if (ARTGetReferencePosition( meinIns,Zyl2) != 1) return false;
                        if (ARTGetReferencePosition( meinIns,Kon2) != 2) return false;
                }
                catch(ARTerror e)
                {
                        return false;
                }
                return true;
        }
TEST_DEF_END(replaceReference)

CUSTOM_TEST_DEF_START(appendReferenceBeforeAndAfter, ARTreferencesInCircuits, ARTreferencesInCircuitsSuperClass)
        virtual bool run() //test destroying a referenced element, then evaluating the circuits impedance -> should generate error
        {
                try
                {
                        ARTRemoveReference( meinIns,Kon1);
                        ARTRemoveReference( meinIns,Kon2);
                        ARTAppendReferenceBefore( meinIns,Zyl2,Kon2);
                        ARTAppendReferenceAfter( meinIns,Zyl1,Kon1);
                        if (ARTGetReferencePosition( meinIns,Zyl1) != 0) return false;
                        if (ARTGetReferencePosition( meinIns,Kon1) != 1) return false;
                        if (ARTGetReferencePosition( meinIns,Kon2) != 2) return false;
                        if (ARTGetReferencePosition( meinIns,Zyl2) != 3) return false;
                }
                catch(ARTerror e)
                {
                        return false;
                }
                return true;
        }
TEST_DEF_END(appendReferenceBeforeAndAfter)

CUSTOM_TEST_DEF_START(destroyElementThenEvaluate, ARTreferencesInCircuits, ARTreferencesInCircuitsSuperClass)
        virtual bool run() //test destroying a referenced element, then evaluating the circuits impedance -> should generate error
        {
                try
                {
                        ARTDestroyElement(mySim,Kon1);
                        ARTvariant* meineImpKurve = ARTInputImpedance(meinIns);
                }
                catch(ARTerror e)
                {
                        Kon1 = ARTCreateElement(mySim, "Replacement_for_unprepare_method", "Cone");
                        return true;
                }
                Kon1 = ARTCreateElement(mySim, "Replacement_for_unprepare_method", "Cone");
                return false;
        }
TEST_DEF_END(destroyElementThenEvaluate)

CUSTOM_TEST_DEF_START(destroyElementRecreateThenEvaluate, ARTreferencesInCircuits, ARTreferencesInCircuitsSuperClass)
        virtual bool run() //test destroying a referenced element, then recreating an element with the same name, then evaluating the circuits impedance -> should generate error
        {
                try
                {
                        ARTDestroyElement(mySim,Kon1);
                        //recreate element with the same name
                        Kon1 = ARTCreateElement(mySim, "Kon1", "Cone");
                        ARTvariant* meineImpKurve = ARTInputImpedance(meinIns);
                }
                catch(ARTerror e)
                {
                        return true;
                }
                return false;
        }
TEST_DEF_END(destroyElementRecreateThenEvaluate)

CUSTOM_TEST_DEF_START(addAndReplace, ARTreferencesInCircuits, ARTreferencesInCircuitsSuperClass)
        virtual bool run() //test adding and replacing references
        {
                try
                {

                        //replace Kon2 with Kon1 -> there should be one occurence
                        if (1 != ARTReplaceReference( meinIns, Kon2,Kon1))
                                return false;

                        //replace Kon1 with Kon2 -> there should be two occurences now
                        if (2 != ARTReplaceReference( meinIns, Kon1,Kon2))
                                return false;

                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }
TEST_DEF_END(addAndReplace)


CUSTOM_TEST_DEF_START(addAndReplaceNonExisting, ARTreferencesInCircuits, ARTreferencesInCircuitsSuperClass)
        virtual bool run() //test adding and replacing references
        {
                try
                {
                        //replace null reference with Kon1 -> there should be an error
                        ARTReplaceReference( meinIns, NULL, Kon1);
                        return false;
                }
                catch(ARTerror e)
                {
                        return true;
                }

                try
                {
                        //replace Kon1 with null reference -> there should be an error thrown
                        ARTReplaceReference( meinIns, Kon1, NULL);
                        return false;
                }
                catch(ARTerror e)
                {
                        return true;
                }
        }
TEST_DEF_END(addAndReplaceNonExisting)

CUSTOM_TEST_DEF_START(removeElement, ARTreferencesInCircuits, ARTreferencesInCircuitsSuperClass)
        virtual bool run() //test adding and replacing references
        {
                try
                {
                        //remove Kon1 -> there should be one occurence
                        if (1 != ARTRemoveReference( meinIns, Kon1))
                                return false;
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }
TEST_DEF_END(removeElement)

CUSTOM_TEST_DEF_START(removeAllElements, ARTreferencesInCircuits, ARTreferencesInCircuitsSuperClass)
        virtual bool run() //test adding and replacing references
        {
                try
                {
                        //remove all -> there should be one occurence
                        if (4 != ARTRemoveAllReferences(meinIns))
                                return false;
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }
TEST_DEF_END(removeAllElements)

//******************************************************************************************************************************************

class ARTdependencyTreeClass : public TestClass
{
        public:
                P_ART_Simulator mySim;

                ARTdependencyTreeClass() : TestClass("ARTdependencyTree", AllMyTests) {}

                virtual void prepare()
                {
                        ARTRootObject();
                        mySim = ARTCreateSimulator("MeinSimulator", "FrequencyDomain", "MultiModal");
                        ARTSetFrequencyRange    (mySim, 10, 25, 5);
                        ARTSetNModes    (mySim, 1);

                }
                virtual void unprepare()
                {
                        ARTDestroySimulator(mySim);
                        ARTRootDestroy();
                }
};
ARTdependencyTreeClass* ARTdependencyTree = new ARTdependencyTreeClass();

TEST_DEF_START(changeElementAndCalculateImpedance2, ARTdependencyTree) //vielleicht in andereklasse "dependency tree" oder sowas
//Change type of element Bes1 to Cylinder, so that equation "Cyl1.length = Bes1.r1 * 2;" becomes invalid -> error expected
        P_ART_Circuit meinIns;
        P_ART_Element Cyl;
        P_ART_Element Bes;
        virtual void prepare()
        {
                meinIns = ARTCreateCircuit(ARTdependencyTree->mySim, "MeinInstrument");
                Cyl = ARTCreateElement(ARTdependencyTree->mySim, "Cyl1", "Cylinder");
                Bes = ARTCreateElement(ARTdependencyTree->mySim, "Bes1", "Besselhorn");
        }

        virtual bool run() //test calculating impedance
        {
                try
                {
                        ARTAppendReference( meinIns, Cyl);
                        ARTAppendReference(meinIns, Bes);

                        ARTSetParameter(ARTdependencyTree->mySim, "Cyl1.length = Bes1.r1 * 2;");

                        ARTvariant* meineImpKurve = ARTInputImpedance(meinIns);

                        //Now change element to cylinder.
                        ARTChangeElementModel(ARTdependencyTree->mySim, Bes, "Cylinder");

                        //this should throw an exception, because "Cyl.length = Bes.r1 * 2;" is not valid anymore (Cylinders don't have R1)
                        try {
                        meineImpKurve = ARTInputImpedance(meinIns);
                        } catch (ARTerror e) {return true;} //exception expected!
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();
                        return false;
                }
                return false;
        }

        virtual void unprepare()
        {
                ARTDestroyElement(ARTdependencyTree->mySim, Cyl);
                ARTDestroyElement(ARTdependencyTree->mySim, Bes);
                ARTDestroyCircuit(ARTdependencyTree->mySim, meinIns);
        }

TEST_DEF_END(changeElementAndCalculateImpedance2)


TEST_DEF_START(deleteElementAndCalculateImpedance, ARTdependencyTree) //vielleicht in andereklasse "dependency tree" oder sowas
//element of circuit is deleted, then impedance calculated for second time -> should give error
        P_ART_Circuit meinIns;
        P_ART_Element Cyl;

        virtual void prepare()
        {
                meinIns = ARTCreateCircuit(ARTdependencyTree->mySim, "MeinInstrument");
                Cyl = ARTCreateElement(ARTdependencyTree->mySim, "Cyl1", "Cylinder");
        }

        virtual bool run()
        {
                try
                {
                        P_ART_Element Bes = ARTCreateElement(ARTdependencyTree->mySim, "Bes1", "Besselhorn");
                        ARTAppendReference(meinIns, Cyl);
                        ARTAppendReference( meinIns, Bes);

                        ARTvariant* meineImpKurve = ARTInputImpedance(meinIns);
                        //Now change element to cylinder.
                        ARTDestroyElement(ARTdependencyTree->mySim, Bes);
                        //this should throw an exception, because "Cyl.length = Bes.r1 * 2;" is not valid anymore (Cylinders don't have R1)
                        try {
                        meineImpKurve = ARTInputImpedance(meinIns);
                        } catch (ARTerror e) {return true;} //exception expected!
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();
                        return false;
                }
                return false;
        }

        virtual void unprepare()
        {
                ARTDestroyElement(ARTdependencyTree->mySim, Cyl);
                ARTDestroyCircuit(ARTdependencyTree->mySim, meinIns);
        }

TEST_DEF_END(deleteElementAndCalculateImpedance)


TEST_DEF_START(deleteElementAndCalculateImpedance2, ARTdependencyTree) //vielleicht in andereklasse "dependency tree" oder sowas
//same as above with additional inter-parameter dependencies
        P_ART_Circuit meinIns;
        P_ART_Element Cyl;

        virtual void prepare()
        {
                meinIns = ARTCreateCircuit(ARTdependencyTree->mySim, "MeinInstrument");
                Cyl = ARTCreateElement(ARTdependencyTree->mySim, "Cyl1", "Cylinder");
        }

        virtual bool run() //test calculating impedance
        {
                try
                {
                        P_ART_Element Bes = ARTCreateElement(ARTdependencyTree->mySim, "Bes1", "Besselhorn");
                        ARTAppendReference(meinIns, Cyl);
                        ARTAppendReference(meinIns, Bes);

                        ARTSetParameter(ARTdependencyTree->mySim, "Cyl1.length = Bes1.r1 * 2;");
                        ARTvariant* meineImpKurve = ARTInputImpedance(meinIns);

                        //Now change element to cylinder.
                        ARTDestroyElement(ARTdependencyTree->mySim, Bes);
                        //this should throw an exception, because "Cyl.length = Bes.r1 * 2;" is not valid anymore (Cylinders don't have R1)
                        try {
                        meineImpKurve = ARTInputImpedance(meinIns);
                        } catch (ARTerror e) {return true;} //exception expected!
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();
                        return false;
                }
                return false;
        }

        virtual void unprepare()
        {
                ARTDestroyElement(ARTdependencyTree->mySim, Cyl);
                ARTDestroyCircuit(ARTdependencyTree->mySim, meinIns);
        }

TEST_DEF_END(deleteElementAndCalculateImpedance2)


TEST_DEF_START(changeElementAndCalculateImpedance3, ARTdependencyTree) //vielleicht in andereklasse "dependency tree" oder sowas
//Change type of element Bes1 to Cone, so that equation "Cyl1.length = Bes1.r1 * 2;" stays valid (Cone also has property R1)-> no error expected, dependencies must be re-considered!
        P_ART_Circuit meinIns;
        P_ART_Element Cyl;
        P_ART_Element Bes;
        virtual void prepare()
        {
                meinIns = ARTCreateCircuit(ARTdependencyTree->mySim, "MeinInstrument");
                Cyl = ARTCreateElement(ARTdependencyTree->mySim, "Cyl1", "Cylinder");
                Bes = ARTCreateElement(ARTdependencyTree->mySim, "Bes1", "Besselhorn");
        }

        virtual bool run() //test calculating impedance
        {
                try
                {
                        ARTAppendReference(meinIns, Cyl);
                        ARTAppendReference(meinIns, Bes);

                        ARTSetParameter(ARTdependencyTree->mySim, "Cyl1.length = Bes1.r1 * 2;");
                        ARTvariant* meineImpKurve = ARTInputImpedance(meinIns);

                        //Now change element to cylinder.
                        ARTChangeElementModel(ARTdependencyTree->mySim, Bes, "Cone");
                        //this should throw NO exception, because "Cyl.length = Bes.r1 * 2;" is still valid (Cones also have R1)
                        meineImpKurve = ARTInputImpedance(meinIns);
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();
                        return false;
                }
                return true;
        }

        virtual void unprepare()
        {
                ARTDestroyElement(ARTdependencyTree->mySim, Cyl);
                ARTDestroyElement(ARTdependencyTree->mySim, Bes);
                ARTDestroyCircuit(ARTdependencyTree->mySim, meinIns);
        }

TEST_DEF_END(changeElementAndCalculateImpedance3)


TEST_DEF_START(testEvaluationCost, ARTdependencyTree)

        P_ART_Circuit meinIns;
        P_ART_Element Cyl;

        virtual void prepare()
        {
                 meinIns = ARTCreateCircuit(ARTdependencyTree->mySim, "MeinInstrument2");
                 Cyl = ARTCreateElement(ARTdependencyTree->mySim, "Cyl", "Cylinder");
        }

        virtual bool run() //test evaluation cost
        {
                try
                {
                        ARTSetFrequencyRange    (ARTdependencyTree->mySim, 12, 202, 5);

                        ARTAppendReference(meinIns, Cyl);

                        meinIns->PrepareCalculation();
                        DataContainer* impCurveProp = (ARTdataProp*)(meinIns->FindProperty("InputImpedanceCurve"));

                        if (impCurveProp == NULL) throw ARTerror("ARTInputImpedance", "Invalid impedance curve");

                        //now the cost should be greater than zero (depending on the properties of the model cylinder)
                        int ec_begin = impCurveProp->GetEvaluationCost();
                        if (ec_begin==0) return false;
                        //asking for the evaluation cost a second time should return exactly the same
                        if (ec_begin!=impCurveProp->GetEvaluationCost()) return false;

                        //evaluate
                        ARTvariant* meineImpKurve = ARTInputImpedance(meinIns);

                        //now the cost should be zero
                        if (0!=impCurveProp->GetEvaluationCost()) return false;

                        //after changing one parameter the ev. cost should be non zero...
                        ARTSetParameter(ARTdependencyTree->mySim,"Cyl.length = 123;");
                        meinIns->PrepareCalculation();
                        int ec = impCurveProp->GetEvaluationCost();
                        if (0==ec) return false;
                        //...but smaller than the first value (where also frequency grid etc. had to be evaluated)
                        if (ec_begin<=ec) return false; //is this true??? I have introduced the complexity memory,
                        //so the datacontainers try to remember how complex the calculation was... --> complexity turned off
                        //and do we still get exactly the same when asking twice?
                        if (ec != impCurveProp->GetEvaluationCost()) return false;
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }

        virtual void unprepare()
        {
                ARTDestroyElement(ARTdependencyTree->mySim, Cyl);
                ARTDestroyCircuit(ARTdependencyTree->mySim, meinIns);
        }

TEST_DEF_END(testEvaluationCost)

TEST_DEF_START(testEvaluationCounter, ARTdependencyTree)

        P_ART_Circuit meinIns;
        P_ART_Element Cyl;
        static bool reached_one;

        virtual void prepare()
        {
                 meinIns = ARTCreateCircuit(ARTdependencyTree->mySim, "MeinInstrument2");
                 Cyl = ARTCreateElement(ARTdependencyTree->mySim, "Cyl", "Cylinder");
        }

        static bool testEvalCounter_callback(double p, const char* msg)
        {
                if (p == 1) reached_one = true;
                if (p > 1) reached_one = false; //it should reach *exactly* one
                //std::cout << " " << p << " (" << msg << ")\n";
                return true;
        }

        virtual bool run() //Check if the estimated evaluation cost is exactly as the real evaluation cost
        {
                try
                {
                        ARTSetFrequencyRange    (ARTdependencyTree->mySim, 10, 200, 5);
                        reached_one = false;
                        ARTSetProgressFunction(&testEvaluationCounter::testEvalCounter_callback);
                        ARTAppendReference( meinIns, Cyl);
                        ARTvariant* meineImpKurve = ARTInputImpedance(meinIns);

                        //the counter should have reached 1
                        if (!reached_one) return false;
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }

        virtual void unprepare()
        {
                ARTDestroyElement(ARTdependencyTree->mySim, Cyl);
                ARTDestroyCircuit(ARTdependencyTree->mySim, meinIns);
        }

TEST_DEF_END(testEvaluationCounter)

bool testEvaluationCounter::reached_one = false;

TEST_DEF_START(testAbortAndEvaluate, ARTdependencyTree)

        P_ART_Circuit meinIns;
        P_ART_Element Cyl;
        static bool reached_one;

        virtual void prepare()
        {
                 meinIns = ARTCreateCircuit(ARTdependencyTree->mySim, "MeinInstrument2");
                 Cyl = ARTCreateElement(ARTdependencyTree->mySim, "Cyl", "Cylinder");
        }

        static bool abortAtFifty_callback(double p, const char* msg)
        {
                if (p > 0.5) return false;
                return true;
        }

        virtual bool run()
        {
                try
                {
                        ARTSetFrequencyRange    (ARTdependencyTree->mySim, 10, 200, 5);
                        ARTSetProgressFunction(&testAbortAndEvaluate::abortAtFifty_callback);
                        ARTAppendReference( meinIns, Cyl);
                        ARTvariant* meineImpKurve = ARTInputImpedance(meinIns);
                        //remove the aborting callback
                        ARTSetProgressFunction(NULL);
                        //reevaluate
                        meineImpKurve = ARTInputImpedance(meinIns);
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }

        virtual void unprepare()
        {
                ARTDestroyElement(ARTdependencyTree->mySim, Cyl);
                ARTDestroyCircuit(ARTdependencyTree->mySim, meinIns);
        }

TEST_DEF_END(testAbortAndEvaluate)


TEST_DEF_START(testARTReEvaluation, ARTdependencyTree)
//Test evaluate and (re)evaluate using the DLL functions
        //P_ART_Object psim
        P_ART_Simulator mySim;
        P_ART_Circuit myIns;
        P_ART_Element Bes;
        P_ART_Element Cyl;
        P_ART_Element Con;

        virtual void prepare()
        {
                // Create Root Object
                //psim = ARTRootObject(); created in test suite
                mySim = ARTCreateSimulator("MySimulator", "FrequencyDomain", "MultiModal");
                Cyl = ARTCreateElement(mySim, "Cyl", "Cylinder");
                Con = ARTCreateElement(mySim, "Con", "Cone");
                Bes = ARTCreateElement(mySim, "Bes", "Besselhorn");
                myIns = ARTCreateCircuit(mySim, "MyInstrument");
                ARTSetFrequencyRange(mySim, 10, 1800, 10);
                ARTSetNModes(mySim, 1);
        }

        virtual bool run() //Check if the estimated evaluation cost is exactly as the real evaluation cost
        {
                try
                {
                        ARTAppendReference(myIns, Cyl);
                        ARTAppendReference( myIns, Con);
                        ARTAppendReference( myIns, Bes);

                        int time1 = GetTickCount();
                        P_ART_DataProp myImpCurve = ARTInputImpedance(myIns);
                        time1 = GetTickCount() - time1; //this is something like 2000ms on my laptop

                        int time2 = GetTickCount();
                        myImpCurve = ARTInputImpedance(myIns);
                        time2 = GetTickCount() - time2; //this should be much smaller than *time*, something like 0-10ms on my laptop

                        //should take slightly longer than the calculation before, because only the cylinder needs to be reevaluated
                        ARTSetParameter(mySim, "Cyl.r = 1.1212; Cyl.length = 101;");
                        int time3 = GetTickCount();
                        myImpCurve = ARTInputImpedance(myIns);
                        time3 = GetTickCount() - time3;

                        //should take nearly as long as first calculation, because all elements (but not frq grids) need to be reevaluated
                        ARTSetParameter(mySim, "Bes.r1 = 2.324; Bes.length = 112;");
                        int time4 = GetTickCount();
                        myImpCurve = ARTInputImpedance(myIns);
                        time4 = GetTickCount() - time4;
                        //std::cout << time1 << " " << time2 << " " << time3 << " " << time4 << endl;

                        //some checks... note that these might not be valid on all machines, depending on processor speed. But for now they do what they are supposed to accomplish...
                        if ((time1 / 10) < time2) return false; // in 2 nothing had to be eval, should be ~0
                        if ((time1 / 5) < time3) return false; //in 3 only cyl had to be eval -> time3 << time1
                        if (time2 > time3) return false; //cyl had to be reeval.

                        if (time4 > time1) return false; //in 4 frq grid was not eval.
                        if ((time1 / 2) > time4) return false; //but everything else, should not be much smaller than time1

                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }

        virtual void unprepare()
        {
                ARTDestroyElement(mySim, Cyl);
                ARTDestroyElement(mySim, Con);
                ARTDestroyElement(mySim, Bes);
                ARTDestroyCircuit(mySim, myIns);
                ARTDestroySimulator(mySim);
        }

TEST_DEF_END(testARTReEvaluation)


TEST_DEF_START(testARTRemoveElement, ARTdependencyTree)
//Test evaluate and (re)evaluate using the DLL functions
        //P_ART_Object psim
        P_ART_Simulator mySim;
        P_ART_Circuit myIns;
        P_ART_Element Bes;
        P_ART_Element Cyl;
        P_ART_Element Con;

        virtual void prepare()
        {
                mySim = ARTCreateSimulator("MySimulator", "FrequencyDomain", "MultiModal");
                Cyl = ARTCreateElement(mySim, "Cyl", "Cylinder");
                Con = ARTCreateElement(mySim, "Con", "Cone");
                Bes = ARTCreateElement(mySim, "Bes", "Besselhorn");
                myIns = ARTCreateCircuit(mySim, "MyInstrument");
                ARTSetFrequencyRange(mySim, 10, 1800, 10);
                ARTSetNModes(mySim, 1);
        }

        virtual bool run() //Calculate impedance, remove element at end, recalculate
        {
                try
                {
                        ARTAppendReference( myIns, Cyl);
                        ARTAppendReference( myIns, Con);

                        //int time1 = GetTickCount();
                        P_ART_DataProp myImpCurve = ARTInputImpedance(myIns);
                        int f = int(myImpCurve->val->nt[0].f);
                        int re = int(myImpCurve->val->nt[0].re);
                        //time1 = GetTickCount() - time1; //this is something like 2000ms on my laptop
                        //myImpCurve->DebugDepTree(""); std::cout << "***\n\n";
                        //std::cout << int(myImpCurve->val->nt[0].re) << "\n";

                        ARTRemoveReference(myIns,Con);
                        //int time2 = GetTickCount();
                        myImpCurve = ARTInputImpedance(myIns);
                        //time2 = GetTickCount() - time2;
                        //myImpCurve->DebugDepTree(""); std::cout << "***\n\n";

                        if (f != int(myImpCurve->val->nt[0].f)) return false;
                        if (re == int(myImpCurve->val->nt[0].re)) return false;

                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }

        virtual void unprepare()
        {
                ARTDestroyElement(mySim, Cyl);
                ARTDestroyElement(mySim, Con);
                ARTDestroyElement(mySim, Bes);
                ARTDestroyCircuit(mySim, myIns);
                ARTDestroySimulator(mySim);
        }

TEST_DEF_END(testARTRemoveElement)


TEST_DEF_START(testARTDepTreeStruct, ARTdependencyTree)
//Check if the structure of the dependency tree is as we expect it.
        P_ART_Simulator mySim;
        P_ART_Circuit myIns;
        P_ART_Element Bes;
        P_ART_Element Cyl;
        P_ART_Element Con;

        virtual void prepare()
        {
                // Create Root Object
                //psim = ARTRootObject(); created in test suite
                mySim = ARTCreateSimulator("MySimulator", "FrequencyDomain", "MultiModal");
                Cyl = ARTCreateElement(mySim, "Cyl", "Cylinder");
                Con = ARTCreateElement(mySim, "Con", "Cone");
                Bes = ARTCreateElement(mySim, "Bes", "Besselhorn");
                myIns = ARTCreateCircuit(mySim, "MyInstrument");
                ARTSetFrequencyRange(mySim, 10, 1800, 10);
                ARTSetNModes(mySim, 1);
        }

        virtual bool run()
        {
                try
                {
                        //If the format of the Dependency Tree output is changed or the naming convention, this test will fail
                        string test = "";
                        string test2 = "";


                        ARTAppendReference(myIns, Cyl);
                        P_ART_DataProp myImpCurve = ARTInputImpedance(myIns);
                        test = myImpCurve->WriteDepTree("","*");
                        test2 = "InputImpedanceCurve*. Z_Cyl*. . wfrqGrid*. . . frqGrid*. . . . LowerFrequencyLimit*. . . . HigherFrequencyLimit*. . . . FrequencyStep*. . ZR_Cyl*. . . wfrqGrid*. . . . frqGrid*. . . . . LowerFrequencyLimit*. . . . . HigherFrequencyLimit*. . . . . FrequencyStep*. . . NumberOfModes*. . . Cyl.length*. . . Cyl.r*. . . Cyl.temp*. . . Cyl.loss*. . . Cyl.humidity*. . . Cyl.xc*. . Cyl.length*. . Cyl.r*. . Cyl.temp*. . Cyl.loss*. . Cyl.humidity*. . Cyl.xc*. . NumberOfModes*";
                        //std::cout << test << "\n\n";
                        if (test.compare(test2) != 0) return false;

                        ARTAppendReference( myIns, Con);
                        myImpCurve = ARTInputImpedance(myIns);
                        test = myImpCurve->WriteDepTree("","*");
                        //std::cout << test << "\n\n";
                        test2 = "InputImpedanceCurve*. Z_Cyl*. . wfrqGrid*. . . frqGrid*. . . . LowerFrequencyLimit*. . . . HigherFrequencyLimit*. . . . FrequencyStep*. . Z_Con*. . . wfrqGrid*. . . . frqGrid*. . . . . LowerFrequencyLimit*. . . . . HigherFrequencyLimit*. . . . . FrequencyStep*. . . ZR_Con*. . . . wfrqGrid*. . . . . frqGrid*. . . . . . LowerFrequencyLimit*. . . . . . HigherFrequencyLimit*. . . . . . FrequencyStep*. . . . NumberOfModes*. . . . Con.length*. . . . Con.r1*. . . . Con.r2*. . . . Con.temp*. . . . Con.loss*. . . . Con.humidity*. . . . Con.xc*. . . Con.length*. . . Con.r1*. . . Con.r2*. . . Con.temp*. . . Con.loss*. . . Con.humidity*. . . Con.xc*. . . NumberOfModes*. . Cyl.length*. . Cyl.r*. . Cyl.temp*. . Cyl.loss*. . Cyl.humidity*. . Cyl.xc*. . NumberOfModes*";
                        if (test.compare(test2) != 0) return false;

                        ARTAppendReference(myIns, Bes);
                        myImpCurve = ARTInputImpedance(myIns);
                        test = myImpCurve->WriteDepTree("","*");
                        //std::cout << test << "\n\n";
                        test2 = "InputImpedanceCurve*. Z_Cyl*. . wfrqGrid*. . . frqGrid*. . . . LowerFrequencyLimit*. . . . HigherFrequencyLimit*. . . . FrequencyStep*. . Z_Con*. . . wfrqGrid*. . . . frqGrid*. . . . . LowerFrequencyLimit*. . . . . HigherFrequencyLimit*. . . . . FrequencyStep*. . . Z_Bes*. . . . wfrqGrid*. . . . . frqGrid*. . . . . . LowerFrequencyLimit*. . . . . . HigherFrequencyLimit*. . . . . . FrequencyStep*. . . . ZR_Bes*. . . . . wfrqGrid*. . . . . . frqGrid*. . . . . . . LowerFrequencyLimit*. . . . . . . HigherFrequencyLimit*. . . . . . . FrequencyStep*. . . . . NumberOfModes*. . . . . Bes.length*. . . . . Bes.r1*. . . . . Bes.r2*. . . . . Bes.flare*. . . . . Bes.temp*. . . . . Bes.loss*. . . . . Bes.humidity*. . . . . Bes.xc*. . . . Bes.length*. . . . Bes.r1*. . . . Bes.r2*. . . . Bes.flare*. . . . Bes.temp*. . . . Bes.loss*. . . . Bes.humidity*. . . . Bes.xc*. . . . NumberOfModes*. . . Con.length*. . . Con.r1*. . . Con.r2*. . . Con.temp*. . . Con.loss*. . . Con.humidity*. . . Con.xc*. . . NumberOfModes*. . Cyl.length*. . Cyl.r*. . Cyl.temp*. . Cyl.loss*. . Cyl.humidity*. . Cyl.xc*. . NumberOfModes*";
                        if (test.compare(test2) != 0) return false;

                        ARTSetParameter(mySim, "Bes.r1 = 2.324; Bes.length = 112;");
                        myImpCurve = ARTInputImpedance(myIns);
                        test = myImpCurve->WriteDepTree("","*");
                        test2 = "InputImpedanceCurve*. Z_Cyl*. . wfrqGrid*. . . frqGrid*. . . . LowerFrequencyLimit*. . . . HigherFrequencyLimit*. . . . FrequencyStep*. . Z_Con*. . . wfrqGrid*. . . . frqGrid*. . . . . LowerFrequencyLimit*. . . . . HigherFrequencyLimit*. . . . . FrequencyStep*. . . Z_Bes*. . . . wfrqGrid*. . . . . frqGrid*. . . . . . LowerFrequencyLimit*. . . . . . HigherFrequencyLimit*. . . . . . FrequencyStep*. . . . ZR_Bes*. . . . . wfrqGrid*. . . . . . frqGrid*. . . . . . . LowerFrequencyLimit*. . . . . . . HigherFrequencyLimit*. . . . . . . FrequencyStep*. . . . . NumberOfModes*. . . . . Bes.length*. . . . . Bes.r1*. . . . . Bes.r2*. . . . . Bes.flare*. . . . . Bes.temp*. . . . . Bes.loss*. . . . . Bes.humidity*. . . . . Bes.xc*. . . . Bes.length*. . . . Bes.r1*. . . . Bes.r2*. . . . Bes.flare*. . . . Bes.temp*. . . . Bes.loss*. . . . Bes.humidity*. . . . Bes.xc*. . . . NumberOfModes*. . . Con.length*. . . Con.r1*. . . Con.r2*. . . Con.temp*. . . Con.loss*. . . Con.humidity*. . . Con.xc*. . . NumberOfModes*. . Cyl.length*. . Cyl.r*. . Cyl.temp*. . Cyl.loss*. . Cyl.humidity*. . Cyl.xc*. . NumberOfModes*";
                        //std::cout << test << "\n\n";
                        if (test.compare(test2) != 0) return false;

                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }

        virtual void unprepare()
        {
                ARTDestroyElement(mySim, Cyl);
                ARTDestroyElement(mySim, Con);
                ARTDestroyElement(mySim, Bes);
                ARTDestroyCircuit(mySim, myIns);
                ARTDestroySimulator(mySim);
        }

TEST_DEF_END(testARTDepTreeStruct)


TEST_DEF_START(testReCreateCircuit, ARTdependencyTree)
//Check if the recreation and reevaluation works
        P_ART_Simulator mySim;
        P_ART_Circuit Cir1;
        P_ART_Element Cyl1;
        P_ART_Element Cyl2;
        P_ART_Element Cyl3;

        virtual void prepare()
        {
                // Create Root Object
                //psim = ARTRootObject(); created in test suite
                mySim = ARTCreateSimulator("MySimulator", "FrequencyDomain", "MultiModal");
                Cyl1 = ARTCreateElement(mySim, "Cyl1", "Cylinder");
                Cyl2 = ARTCreateElement(mySim, "Cyl2", "Cylinder");
                Cyl3 = ARTCreateElement(mySim, "Cyl3", "Cylinder");
                ARTSetFrequencyRange(mySim, 10, 1800, 10);
                ARTSetNModes(mySim, 1);
        }

        virtual bool run()
        {
                try
                {
                        ARTdataProp* impCurveProp;

                        Cir1 = ARTCreateCircuit(mySim, "MyInstrument");
                        ARTAppendReference(Cir1, Cyl1);
                        impCurveProp = static_cast<ARTdataProp*>(Cir1->FindProperty("InputImpedanceCurve"));
                        Cir1->PrepareCalculation();
                        int ec1 =  impCurveProp->GetEvaluationCost();
                        impCurveProp->GetValue();
                        if (impCurveProp->GetEvaluationCost() != 0) return false; //no changes -> no need to reevaluate -> must be 0!
                        ARTDestroyCircuit(mySim, Cir1);

                        // ----------------------------------------------
                        // After rebuilding the same circuit, with the same elements, the evaluation cost of its input impedance
                        // must be lower than the first evaluation (but it won't be null, since so far the impedance converted to
                        // a curve is saved in the circuit and must be recalculated when the circuit is fresh
                        Cir1 = ARTCreateCircuit(mySim, "Cir1trument");
                        ARTAppendReference(Cir1, Cyl1);

                        impCurveProp = static_cast<ARTdataProp*>(Cir1->FindProperty("InputImpedanceCurve"));
                        Cir1->PrepareCalculation();
                        int ec2 =  impCurveProp->GetEvaluationCost();
                        if (ec2 >= ec1) return false;
                        impCurveProp->GetValue();
                        if (impCurveProp->GetEvaluationCost() != 0) return false; //no changes -> no need to reevaluate -> must be 0!

                        ARTDestroyCircuit(mySim, Cir1);
                        // ----------------------------------------------
                        Cir1 = ARTCreateCircuit(mySim, "Cir1trument");
                        ARTAppendReference(Cir1, Cyl2);
                        ARTAppendReference(Cir1, Cyl1);

                        impCurveProp = static_cast<ARTdataProp*>(Cir1->FindProperty("InputImpedanceCurve"));
                        Cir1->PrepareCalculation();
                        int ec3 = impCurveProp->GetEvaluationCost();
                        //must be smaller than ec1, because Cyl1 (outer element) does not have to be recalculated...
                        if (ec3 >= ec1) return false;
                        //should be bigger than ec2, because Cyl2 has to be calculated, and not just the conversion...
                        if (ec3 <= ec2) return false;
                        impCurveProp->GetValue();
                        if (impCurveProp->GetEvaluationCost() != 0) return false; //no changes -> no need to reevaluate -> must be 0!
                        ARTDestroyCircuit(mySim, Cir1);
                        // ----------------------------------------------
                        Cir1 = ARTCreateCircuit(mySim, "Cir1trument");
                        ARTAppendReference(Cir1, Cyl2);
                        ARTAppendReference(Cir1, Cyl1);

                        impCurveProp = static_cast<ARTdataProp*>(Cir1->FindProperty("InputImpedanceCurve"));
                        Cir1->PrepareCalculation();
                        int ec4 =  impCurveProp->GetEvaluationCost();
                        // When recalculating the same circuit, only the same conversion as for ec2 has to be carried out, so:
                        if (ec2 != ec4) return false;
                        impCurveProp->GetValue();
                        if (impCurveProp->GetEvaluationCost() != 0) return false; //no changes -> no need to reevaluate -> must be 0!
                        ARTDestroyCircuit(mySim, Cir1);
                        // ----------------------------------------------
                        Cir1 = ARTCreateCircuit(mySim, "Cir1trument");
                        ARTAppendReference(Cir1, Cyl1);
                        ARTAppendReference(Cir1, Cyl2);

                        impCurveProp = static_cast<ARTdataProp*>(Cir1->FindProperty("InputImpedanceCurve"));
                        Cir1->PrepareCalculation();
                        int ec5 =  impCurveProp->GetEvaluationCost();
                        //When adding the elements in reverse order, they both need to be recalculated, so the cost should be more than ec1 (which was only one element)
                        if (ec5 <= ec1) return false;
                        impCurveProp->GetValue();
                        if (impCurveProp->GetEvaluationCost() != 0) return false; //no changes -> no need to reevaluate -> must be 0!
                        ARTDestroyCircuit(mySim, Cir1);
                        // ----------------------------------------------
                        Cir1 = ARTCreateCircuit(mySim, "Cir1trument");
                        ARTAppendReference(Cir1, Cyl1);
                        ARTAppendReference(Cir1, Cyl2);
                        impCurveProp = static_cast<ARTdataProp*>(Cir1->FindProperty("InputImpedanceCurve"));
                        Cir1->PrepareCalculation();
                        int ec6 = impCurveProp->GetEvaluationCost();
                        if (ec6 != ec2) return false;
                        impCurveProp->GetValue();
                        if (impCurveProp->GetEvaluationCost() != 0) return false; //no changes -> no need to reevaluate -> must be 0!
                        ARTDestroyCircuit(mySim, Cir1);


                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }

        virtual void unprepare()
        {
                ARTDestroyElement(mySim, Cyl1);
                ARTDestroyElement(mySim, Cyl2);
                ARTDestroyElement(mySim, Cyl3);
        }

TEST_DEF_END(testReCreateCircuit)

//******************************************************************************************************************************************

class ARTcalculatingImpedanceClass : public TestClass
{
        public:
                P_ART_Simulator mySim;
                P_ART_Element Zyl1;
                P_ART_Element Kon1;
                P_ART_Element BZyl1;

                ARTcalculatingImpedanceClass() : TestClass("ARTcalculatingImpedance", AllMyTests) {}

                virtual void prepare()
                {
                        ARTRootObject();
                        mySim = ARTCreateSimulator("MeinSimulator", "FrequencyDomain", "MultiModal");
                        ARTSetFrequencyRange    (mySim, 10, 25, 5);
                        ARTSetNModes    (mySim, 1);
                        Kon1 = ARTCreateElement(mySim, "Kon", "Cone");
                        Zyl1 = ARTCreateElement(mySim, "Zyl", "Cylinder");
                        BZyl1 = ARTCreateElement(mySim, "BZyl", "BentCylinder");
                }
                virtual void unprepare()
                {
                        ARTDestroyElement(mySim, Kon1);
                        ARTDestroyElement(mySim, Zyl1);
                        ARTDestroyElement(mySim, BZyl1);
                        ARTDestroySimulator(mySim);
                        ARTRootDestroy();
                }
};
ARTcalculatingImpedanceClass* ARTcalculatingImpedance = new ARTcalculatingImpedanceClass();

TEST_DEF_START(calculateImpedance, ARTcalculatingImpedance)

        P_ART_Circuit meinIns;
        P_ART_Circuit meinIns2;

        virtual void prepare()
        {
                 meinIns = ARTCreateCircuit(ARTcalculatingImpedance->mySim, "MeinInstrument");
                 meinIns2 = ARTCreateCircuit(ARTcalculatingImpedance->mySim, "MeinInstrument2");
        }

        virtual bool run() //test calculating impedance
        {
                try
                {
                        ARTAppendReference(meinIns, ARTcalculatingImpedance->BZyl1);
                        ARTAppendReference(meinIns, ARTcalculatingImpedance->Zyl1);
                        ARTvariant* meineImpKurve = ARTInputImpedance(meinIns);

                        //std::cout << "meineImpKurve " << meineImpKurve << "\n";
                        //std::cout << "meineImpKurve->val " << meineImpKurve->val << "\n";
                        //std::cout << "meineImpKurve->val->typ " << meineImpKurve->GetTypeString() << "\n";
                        //std::cout << "meineImpKurve->val->nt " << meineImpKurve->val->nt << "\n";

                        //check if results are as we expect them (at least approx. in int, because of rounding errors...)
                        if (int(meineImpKurve->val->nt[0].f) != 10) return false;
                        if (int(meineImpKurve->val->nt[0].re) != 2546) return false;
                        if (int(meineImpKurve->val->nt[0].im) != 513489) return false;

                        if (int(meineImpKurve->val->nt[1].f) != 15) return false;
                        if (int(meineImpKurve->val->nt[1].re) != 8225) return false;
                        if (int(meineImpKurve->val->nt[1].im) != 823605) return false;
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }

        virtual void unprepare()
        {
                ARTDestroyCircuit(ARTcalculatingImpedance->mySim, meinIns);
                ARTDestroyCircuit(ARTcalculatingImpedance->mySim, meinIns2);
        }

TEST_DEF_END(calculateImpedance)



TEST_DEF_START(changeElementAndCalculateImpedance, ARTcalculatingImpedance)

        P_ART_Circuit meinIns;

        virtual void prepare()
        {
                 meinIns = ARTCreateCircuit(ARTcalculatingImpedance->mySim, "MeinInstrument");
        }

        virtual bool run() //test calculating impedance
        {
                try
                {
                        P_ART_Element El = ARTCreateElement(ARTcalculatingImpedance->mySim, "Wandler", "Cylinder");
                        ARTAppendReference(meinIns, El);

                        ARTvariant* meineImpKurve = ARTInputImpedance(meinIns);

                        //check if results are as we expect them (at least approx. in int, because of rounding errors...)
                        if (int(meineImpKurve->val->nt[0].f) != 10) return false;
                        if (int(meineImpKurve->val->nt[0].re) != 296) return false;
                        if (int(meineImpKurve->val->nt[0].im) != 248106) return false;

                        //Now change element to cone. The Reference in the circuit should still be valid, the results different!
                        ARTChangeElementModel(ARTcalculatingImpedance->mySim, El, "Cone");

                        meineImpKurve = ARTInputImpedance(meinIns);

                        if (int(meineImpKurve->val->nt[0].f) != 10) return false;
                        if (int(meineImpKurve->val->nt[0].re) != 3) return false;
                        if (int(meineImpKurve->val->nt[0].im) != 24732) return false;
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }

        virtual void unprepare()
        {
                ARTDestroyCircuit(ARTcalculatingImpedance->mySim, meinIns);
        }

TEST_DEF_END(changeElementAndCalculateImpedance)

TEST_DEF_START(calculateImpedance2, ARTcalculatingImpedance)

        P_ART_Circuit meinIns;
        P_ART_Circuit meinIns2;

        virtual void prepare()
        {
                 meinIns = ARTCreateCircuit(ARTcalculatingImpedance->mySim, "MeinInstrument");
                 meinIns2 = ARTCreateCircuit(ARTcalculatingImpedance->mySim, "MeinInstrument2");
        }

        virtual bool run() //test calculating impedance
        {
                try
                {
                        ARTAppendReference( meinIns, ARTcalculatingImpedance->Zyl1);
                        ARTAppendReference( meinIns, ARTcalculatingImpedance->Kon1);
                        ARTSetParameter(ARTcalculatingImpedance->mySim, "Kon.length = 11.123; ");
                        ARTvariant* meineImpKurve = ARTInputImpedance(meinIns);
                        meineImpKurve = ARTInputImpedance(meinIns);

                        //std::cout << int(meineImpKurve->val->nt[0].f) << "\n";
                        //std::cout << int(meineImpKurve->val->nt[0].re) << "\n";
                        //std::cout << int(meineImpKurve->val->nt[0].im) << "\n";

                        //std::cout << int(meineImpKurve->val->nt[1].f) << "\n";
                        //std::cout << int(meineImpKurve->val->nt[1].re) << "\n";
                        //std::cout << int(meineImpKurve->val->nt[1].im) << "\n";

                        //check if results are as we expect them (at least approx. in int, because of rounding errors...)
                        if (int(meineImpKurve->val->nt[0].f) != 10) return false;
                        if (int(meineImpKurve->val->nt[0].re) != 310) return false;
                        if (int(meineImpKurve->val->nt[0].im) != 252015) return false;

                        if (int(meineImpKurve->val->nt[1].f) != 15) return false;
                        if (int(meineImpKurve->val->nt[1].re) != 886) return false;
                        if (int(meineImpKurve->val->nt[1].im) != 384065) return false;
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }

        virtual void unprepare()
        {
                ARTDestroyCircuit(ARTcalculatingImpedance->mySim, meinIns);
                ARTDestroyCircuit(ARTcalculatingImpedance->mySim, meinIns2);
        }

TEST_DEF_END(calculateImpedance2)


TEST_DEF_START(changeNumberOfModes, ARTcalculatingImpedance)

        P_ART_Circuit meinIns;
        P_ART_Circuit meinIns2;
        P_ART_Element BZyl2;

        virtual bool run()
        {
                meinIns = ARTCreateCircuit(ARTcalculatingImpedance->mySim, "MeinInstrument");
                meinIns2 = ARTCreateCircuit(ARTcalculatingImpedance->mySim, "MeinInstrument2");
                BZyl2 = ARTCreateElement(ARTcalculatingImpedance->mySim, "BZyl2", "BentCylinder");

                try
                {

                        ARTSetFrequencyRange    (ARTcalculatingImpedance->mySim, 599, 601, 1);
                        ARTSetNModes    (ARTcalculatingImpedance->mySim, 32);
                        ARTSetParameter(ARTcalculatingImpedance->mySim, "BZyl2.length = 100; ");
                        ARTSetParameter(ARTcalculatingImpedance->mySim, "BZyl2.r = 1; ");
                        ARTSetParameter(ARTcalculatingImpedance->mySim, "BZyl2.bendradius = 5; ");
                        ARTAppendReference(meinIns, BZyl2);

                        ARTvariant* meineImpKurve = ARTInputImpedance(meinIns); //nur wenn ich hier ARTInputImpedance aufrufe

                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }

                ARTDestroyCircuit(ARTcalculatingImpedance->mySim, meinIns);
                ARTDestroyCircuit(ARTcalculatingImpedance->mySim, meinIns2);

                //reset to original values
                ARTSetFrequencyRange    (ARTcalculatingImpedance->mySim, 10, 200, 5);
                ARTSetNModes    (ARTcalculatingImpedance->mySim, 1);
                // Teil 2
                meinIns = ARTCreateCircuit(ARTcalculatingImpedance->mySim, "MeinInstrument");
                meinIns2 = ARTCreateCircuit(ARTcalculatingImpedance->mySim, "MeinInstrument2");

                try
                {
                        ARTAppendReference(meinIns, ARTcalculatingImpedance->BZyl1);
                        ARTAppendReference(meinIns, BZyl2);
                        ARTAppendReference(meinIns, ARTcalculatingImpedance->Zyl1);
                        ARTvariant* meineImpKurve = ARTInputImpedance(meinIns);
                        meineImpKurve = ARTInputImpedance(meinIns);

                        ARTAppendReference(meinIns2, ARTcalculatingImpedance->Zyl1);
                        ARTAppendReference(meinIns2, ARTcalculatingImpedance->BZyl1);
                        ARTAppendReference(meinIns2, BZyl2);
                        ARTvariant* meineImpKurve2 = ARTInputImpedance(meinIns2);
                        meineImpKurve = ARTInputImpedance(meinIns2);
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }

                ARTDestroyElement(ARTcalculatingImpedance->mySim, BZyl2);
                ARTDestroyCircuit(ARTcalculatingImpedance->mySim, meinIns);
                ARTDestroyCircuit(ARTcalculatingImpedance->mySim, meinIns2);
                BZyl2 = NULL;
                meinIns = NULL;
                meinIns2 = NULL;
                return true;
        }

        virtual void unprepare()
        {
                //clear up if there was an error
                if (BZyl2) ARTDestroyElement(ARTcalculatingImpedance->mySim, BZyl2);
                if (meinIns) ARTDestroyCircuit(ARTcalculatingImpedance->mySim, meinIns);
                if (meinIns2) ARTDestroyCircuit(ARTcalculatingImpedance->mySim, meinIns2);
        }

TEST_DEF_END(changeNumberOfModes)


TEST_DEF_START(calculateImpedanceTwice, ARTcalculatingImpedance)

        P_ART_Circuit meinIns;
        P_ART_Circuit meinIns2;

        virtual void prepare()
        {
                 meinIns = ARTCreateCircuit(ARTcalculatingImpedance->mySim, "MeinInstrument");
                 meinIns2 = ARTCreateCircuit(ARTcalculatingImpedance->mySim, "MeinInstrument2");
        }

        virtual bool run() //test calculating impedance twice
        {
                try
                {
                        ARTAppendReference(meinIns, ARTcalculatingImpedance->BZyl1);
                        ARTAppendReference( meinIns, ARTcalculatingImpedance->Zyl1);
                        ARTvariant* meineImpKurve = ARTInputImpedance(meinIns);
                        meineImpKurve = ARTInputImpedance(meinIns);

                        ARTAppendReference(meinIns2, ARTcalculatingImpedance->Zyl1);
                        ARTAppendReference(meinIns2, ARTcalculatingImpedance->BZyl1);
                        ARTvariant* meineImpKurve2 = ARTInputImpedance(meinIns2);
                        meineImpKurve = ARTInputImpedance(meinIns2);
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }

        virtual void unprepare()
        {
                ARTDestroyCircuit(ARTcalculatingImpedance->mySim, meinIns);
                ARTDestroyCircuit(ARTcalculatingImpedance->mySim, meinIns2);
        }

TEST_DEF_END(calculateImpedanceTwice)

TEST_DEF_START(calculateBentImpedance, ARTcalculatingImpedance)

        P_ART_Simulator mySim2;
        P_ART_Circuit meinIns;
        P_ART_Element BZyl2;

        virtual void prepare()
        {
                mySim2 = ARTCreateSimulator("MeinSimulator2", "FrequencyDomain", "MultiModal");
                meinIns = ARTCreateCircuit(mySim2, "MeinIns");
                BZyl2 = ARTCreateElement(mySim2, "BZyl2", "BentCylinder");
        }

        virtual bool run() //test calculating impedance
        {
                try
                {
                        ARTSetFrequencyRange    (mySim2, 598, 599, 1);
                        ARTSetNModes    (mySim2, 32);
                        ARTSetParameter(mySim2, "BZyl2.length = 100; ");
                        ARTSetParameter(mySim2, "BZyl2.r = 1; ");
                        ARTSetParameter(mySim2, "BZyl2.bendradius = 5; ");
                        ARTAppendReference(meinIns, BZyl2);

                        ARTvariant* meineImpKurve = ARTInputImpedance(meinIns);
                        meineImpKurve = ARTInputImpedance(meinIns);
                        int i;


                        //for (i = 0; i < meineImpKurve->len; i++)
                        //{
                        //      dcomp c = dcomp(meineImpKurve->val->nt[i].re,meineImpKurve->val->nt[i].im);
                        //      std::cout << (meineImpKurve->val->nt[i].f) << "H ";
                        //      std::cout << int(abs(c)) << " ";
                        //      std::cout << int(arg(c)) << "\n";
                        //}


                        if (meineImpKurve->len != 2) return false;

                        //there should be a peak at 598 Hz
                        i = 0;
                        dcomp c = dcomp(meineImpKurve->val->nt[i].re,meineImpKurve->val->nt[i].im);
                        if (int(meineImpKurve->val->nt[i].f) != 598) return false;
                        if (int(abs(c)) != 17844915) return false;
                        if (int(arg(c)) != 0) return false;

                        i = 1;
                        c = dcomp(meineImpKurve->val->nt[i].re,meineImpKurve->val->nt[i].im);
                        if (int(meineImpKurve->val->nt[i].f) != 599) return false;
                        if (int(abs(c)) != 17431868) return false;
                        if (int(arg(c)) != 0) return false;
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }

        virtual void unprepare()
        {
                ARTDestroyCircuit(mySim2, meinIns);
                ARTDestroyElement(mySim2, BZyl2);
                ARTDestroySimulator(mySim2);
        }

TEST_DEF_END(calculateBentImpedance)


TEST_DEF_START(calculateBentImpedanceAndChangeModes, ARTcalculatingImpedance)

        P_ART_Simulator mySim2;
        P_ART_Circuit meinIns;
        P_ART_Element BZyl2;

        virtual void prepare()
        {
                mySim2 = ARTCreateSimulator("MeinSimulator2", "FrequencyDomain", "MultiModal");
                meinIns = ARTCreateCircuit(mySim2, "MeinIns");
                BZyl2 = ARTCreateElement(mySim2, "BZyl2", "BentCylinder");
        }

        virtual bool run() //test calculating impedance
        {
                try
                {
                        ARTSetFrequencyRange    (mySim2, 598, 599, 1); //nur 600 H2
                        ARTSetNModes    (mySim2, 1);
                        ARTSetParameter(mySim2, "BZyl2.length = 100; ");
                        ARTSetParameter(mySim2, "BZyl2.r = 1; ");
                        ARTSetParameter(mySim2, "BZyl2.bendradius = 5; ");
                        ARTAppendReference(meinIns, BZyl2);

                        ARTvariant* meineImpKurve = ARTInputImpedance(meinIns);
                        meineImpKurve = ARTInputImpedance(meinIns);
                        int i;
                        //for (i = 0; i < meineImpKurve->len; i++)
                        //{
                        //      dcomp c = dcomp(meineImpKurve->val->nt[i].re,meineImpKurve->val->nt[i].im);
                        //      std::cout << (meineImpKurve->val->nt[i].f) << "H ";
                        //      std::cout << int(abs(c)) << " ";
                        //      std::cout << int(arg(c)) << "\n";
                        //}

                        if (meineImpKurve->len != 2) return false;

                        //compare with expected values
                        i = 0;
                        dcomp c = dcomp(meineImpKurve->val->nt[i].re,meineImpKurve->val->nt[i].im);
                        if (int(meineImpKurve->val->nt[i].f) != 598) return false;
                        if (int(abs(c)) != 13094287) return false;
                        if (int(arg(c)) != 0) return false;

                        i = 1;
                        c = dcomp(meineImpKurve->val->nt[i].re,meineImpKurve->val->nt[i].im);
                        if (int(meineImpKurve->val->nt[i].f) != 599) return false;
                        if (int(abs(c)) != 11647690) return false;
                        if (int(arg(c)) != 0) return false;

                        //Now change the number of modes to 32
                        ARTSetNModes    (mySim2, 32);
                        meineImpKurve = ARTInputImpedance(meinIns);
                        meineImpKurve = ARTInputImpedance(meinIns);

                        if (meineImpKurve->len != 2) return false;

                        //there should be a peak at 598 Hz
                        i = 0;
                        c = dcomp(meineImpKurve->val->nt[i].re,meineImpKurve->val->nt[i].im);
                        if (int(meineImpKurve->val->nt[i].f) != 598) return false;
                        if (int(abs(c)) != 17844915) return false;
                        if (int(arg(c)) != 0) return false;

                        i = 1;
                        c = dcomp(meineImpKurve->val->nt[i].re,meineImpKurve->val->nt[i].im);
                        if (int(meineImpKurve->val->nt[i].f) != 599) return false;
                        if (int(abs(c)) != 17431868) return false;
                        if (int(arg(c)) != 0) return false;

                        //Now change the number of modes to 1
                        ARTSetNModes    (mySim2, 1);

                        meineImpKurve = ARTInputImpedance(meinIns);
                        meineImpKurve = ARTInputImpedance(meinIns);

                        if (meineImpKurve->len != 2) return false;

                        //compare with expected values
                        i = 0;
                        c = dcomp(meineImpKurve->val->nt[i].re,meineImpKurve->val->nt[i].im);
                        if (int(meineImpKurve->val->nt[i].f) != 598) return false;
                        if (int(abs(c)) != 13094287) return false;
                        if (int(arg(c)) != 0) return false;

                        i = 1;
                        c = dcomp(meineImpKurve->val->nt[i].re,meineImpKurve->val->nt[i].im);
                        if (int(meineImpKurve->val->nt[i].f) != 599) return false;
                        if (int(abs(c)) != 11647690) return false;
                        if (int(arg(c)) != 0) return false;
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }

        virtual void unprepare()
        {
                ARTDestroyCircuit(mySim2, meinIns);
                ARTDestroyElement(mySim2, BZyl2);
                ARTDestroySimulator(mySim2);
        }

TEST_DEF_END(calculateBentImpedanceAndChangeModes)




TEST_DEF_START(calculateTerminationModelImpedance, ARTcalculatingImpedance)

        P_ART_Simulator mySim;
        P_ART_Circuit myIns;
        P_ART_Element Cyl;
        P_ART_Element Term;

        virtual void prepare()
        {
                mySim = ARTCreateSimulator("Sim", "FrequencyDomain", "MultiModal");
                myIns = ARTCreateCircuit(mySim, "Ins");
                Cyl = ARTCreateElement(mySim, "Cyl", "Cylinder");
                Term = ARTCreateElement(mySim, "Term", "TerminationModel");
        }

        virtual bool run() //test calculating impedance
        {
                try
                {
                        ARTSetFrequencyRange    (mySim, 598, 599, 1); //nur 600 H2
                        ARTSetNModes    (mySim, 1);

                        ARTAppendReference(myIns, Cyl);
                        ARTAppendReference(myIns, Term);

                        ARTvariant* meineImpKurve = ARTInputImpedance(myIns);

                        int i;
                        if (meineImpKurve->len != 2) return false;
                        i = 0;
                        dcomp c = dcomp(meineImpKurve->val->nt[i].re,meineImpKurve->val->nt[i].im);
                        if (int(meineImpKurve->val->nt[i].f) != 598) return false;
                        if (int(abs(c)) != 13094287) return false;
                        if (int(arg(c)) != 0) return false;

                        i = 1;
                        c = dcomp(meineImpKurve->val->nt[i].re,meineImpKurve->val->nt[i].im);
                        if (int(meineImpKurve->val->nt[i].f) != 599) return false;
                        if (int(abs(c)) != 11647690) return false;
                        if (int(arg(c)) != 0) return false;

                        ARTSetParameter(mySim, "Term.radiation = Reflecting;");
                        meineImpKurve = ARTInputImpedance(myIns);

                        if (meineImpKurve->len != 2) return false;
                        i = 0;
                        c = dcomp(meineImpKurve->val->nt[i].re,meineImpKurve->val->nt[i].im);
                        if (int(meineImpKurve->val->nt[i].f) != 598) return false;
                        if (int(abs(c)) != 104559) return false;
                        if (int(arg(c)) != 0) return false;

                        i = 1;
                        c = dcomp(meineImpKurve->val->nt[i].re,meineImpKurve->val->nt[i].im);
                        if (int(meineImpKurve->val->nt[i].f) != 599) return false;
                        if (int(abs(c)) != 97470) return false;
                        if (int(arg(c)) != 0) return false;


                        ARTSetParameter(mySim, "Term.radiation = Zorumski;"); //default -> same results as in first calculation expected!
                        meineImpKurve = ARTInputImpedance(myIns);

                        if (meineImpKurve->len != 2) return false;
                        i = 0;
                        c = dcomp(meineImpKurve->val->nt[i].re,meineImpKurve->val->nt[i].im);
                        if (int(meineImpKurve->val->nt[i].f) != 598) return false;
                        if (int(abs(c)) != 13094287) return false;
                        if (int(arg(c)) != 0) return false;

                        i = 1;
                        c = dcomp(meineImpKurve->val->nt[i].re,meineImpKurve->val->nt[i].im);
                        if (int(meineImpKurve->val->nt[i].f) != 599) return false;
                        if (int(abs(c)) != 11647690) return false;
                        if (int(arg(c)) != 0) return false;

                        //for (i = 0; i < meineImpKurve->len; i++)
                        //{
                        //      dcomp c = dcomp(meineImpKurve->val->nt[i].re,meineImpKurve->val->nt[i].im);
                        //      std::cout << (meineImpKurve->val->nt[i].f) << "H ";
                        //      std::cout << int(abs(c)) << " ";
                        //      std::cout << int(arg(c)) << "\n";
                        //}


                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }

        virtual void unprepare()
        {
                ARTDestroyCircuit(mySim, myIns);
                ARTDestroyElement(mySim, Cyl);
                ARTDestroyElement(mySim, Term);
                ARTDestroySimulator(mySim);
        }

TEST_DEF_END(calculateTerminationModelImpedance)



TEST_DEF_START(calculateTerminationModelImpedanceModes2, ARTcalculatingImpedance)

        P_ART_Simulator mySim;
        P_ART_Circuit myIns;
        P_ART_Element Cyl;
        P_ART_Element Term;

        virtual void prepare()
        {
                mySim = ARTCreateSimulator("Sim", "FrequencyDomain", "MultiModal");
                myIns = ARTCreateCircuit(mySim, "Ins");
                Cyl = ARTCreateElement(mySim, "Cyl", "Cylinder");
                Term = ARTCreateElement(mySim, "Term", "TerminationModel");
        }

        virtual bool run() //test calculating impedance
        {
                try
                {
                        ARTSetFrequencyRange    (mySim, 598, 599, 1); //nur 600 H2
                        ARTSetNModes    (mySim, 2);

                        ARTAppendReference(myIns, Cyl);
                        ARTAppendReference(myIns, Term);

                        ARTvariant* meineImpKurve = ARTInputImpedance(myIns);

                        int i;

                        if (meineImpKurve->len != 2) return false;
                        i = 0;
                        dcomp c = dcomp(meineImpKurve->val->nt[i].re,meineImpKurve->val->nt[i].im);
                        if (int(meineImpKurve->val->nt[i].f) != 598) return false;
                        if (int(abs(c)) != 13210376) return false;
                        if (int(arg(c)) != 0) return false;

                        i = 1;
                        c = dcomp(meineImpKurve->val->nt[i].re,meineImpKurve->val->nt[i].im);
                        if (int(meineImpKurve->val->nt[i].f) != 599) return false;
                        if (int(abs(c)) != 11756012) return false;
                        if (int(arg(c)) != 0) return false;

                        ARTSetParameter(mySim, "Term.radiation = Reflecting;");
                        meineImpKurve = ARTInputImpedance(myIns);

                        if (meineImpKurve->len != 2) return false;
                        i = 0;
                        c = dcomp(meineImpKurve->val->nt[i].re,meineImpKurve->val->nt[i].im);
                        if (int(meineImpKurve->val->nt[i].f) != 598) return false;
                        if (int(abs(c)) != 104559) return false;
                        if (int(arg(c)) != 0) return false;

                        i = 1;
                        c = dcomp(meineImpKurve->val->nt[i].re,meineImpKurve->val->nt[i].im);
                        if (int(meineImpKurve->val->nt[i].f) != 599) return false;
                        if (int(abs(c)) != 97470) return false;
                        if (int(arg(c)) != 0) return false;


                        ARTSetParameter(mySim, "Term.radiation = Zorumski;"); //default -> same results as in first calculation expected!
                        meineImpKurve = ARTInputImpedance(myIns);
                        if (meineImpKurve->len != 2) return false;
                        i = 0;
                        c = dcomp(meineImpKurve->val->nt[i].re,meineImpKurve->val->nt[i].im);
                        if (int(meineImpKurve->val->nt[i].f) != 598) return false;
                        if (int(abs(c)) != 13210376) return false;
                        if (int(arg(c)) != 0) return false;

                        i = 1;
                        c = dcomp(meineImpKurve->val->nt[i].re,meineImpKurve->val->nt[i].im);
                        if (int(meineImpKurve->val->nt[i].f) != 599) return false;
                        if (int(abs(c)) != 11756012) return false;
                        if (int(arg(c)) != 0) return false;

                        ARTSetNModes    (mySim, 1);

                        ARTSetParameter(mySim, "Term.radiation = Zorumski;");

                        meineImpKurve = ARTInputImpedance(myIns);

                        if (meineImpKurve->len != 2) return false;
                        i = 0;
                        c = dcomp(meineImpKurve->val->nt[i].re,meineImpKurve->val->nt[i].im);
                        if (int(meineImpKurve->val->nt[i].f) != 598) return false;
                        if (int(abs(c)) != 13094287) return false;
                        if (int(arg(c)) != 0) return false;

                        i = 1;
                        c = dcomp(meineImpKurve->val->nt[i].re,meineImpKurve->val->nt[i].im);
                        if (int(meineImpKurve->val->nt[i].f) != 599) return false;
                        if (int(abs(c)) != 11647690) return false;
                        if (int(arg(c)) != 0) return false;

                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }

        virtual void unprepare()
        {
                ARTDestroyCircuit(mySim, myIns);
                ARTDestroyElement(mySim, Cyl);
                ARTDestroyElement(mySim, Term);
                ARTDestroySimulator(mySim);
        }

TEST_DEF_END(calculateTerminationModelImpedanceModes2)



TEST_DEF_START(changeFrqStep, ARTcalculatingImpedance)

        P_ART_Simulator mySim2;
        P_ART_Circuit meinIns;
        P_ART_Element BZyl2;

        virtual void prepare()
        {
                mySim2 = ARTCreateSimulator("MeinSimulator2", "FrequencyDomain", "MultiModal");
                meinIns = ARTCreateCircuit(mySim2, "MeinIns");
                BZyl2 = ARTCreateElement(mySim2, "BZyl2", "Cylinder");
        }

        virtual bool run() //test calculating impedance
        {
                try
                {
                        ARTSetFrequencyRange    (mySim2, 10, 1800, 100);
                        ARTSetNModes    (mySim2, 1);
                        ARTAppendReference(meinIns, BZyl2);

                        ARTvariant* meineImpKurve = ARTInputImpedance(meinIns);

                        //Now change the frq step
                        ARTSetFrequencyRange    (mySim2, 10, 1800, 10);
                        meineImpKurve = ARTInputImpedance(meinIns);
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }

        virtual void unprepare()
        {
                ARTDestroyCircuit(mySim2, meinIns);
                ARTDestroyElement(mySim2, BZyl2);
                ARTDestroySimulator(mySim2);
        }

TEST_DEF_END(changeFrqStep)

//********** SUB-Suite **************

class addAndRemoveElementsCalculateImpedanceClass : public TestClass
{
        public:
                P_ART_Simulator mySim;
                P_ART_Element Cyl;
                P_ART_Element Cyl2;
                P_ART_Element Con;
                P_ART_Element Bes;

                addAndRemoveElementsCalculateImpedanceClass() : TestClass("addAndRemoveElementsCalculateImpedance", ARTcalculatingImpedance) {}

                virtual void prepare()
                {
                        mySim = ARTCreateSimulator("MySimulator", "FrequencyDomain", "MultiModal");
                        Cyl = ARTCreateElement(mySim, "Cyl", "Cylinder");
                        Cyl2 = ARTCreateElement(mySim, "Cyl2", "Cylinder");
                        Con = ARTCreateElement(mySim, "Con", "Cone");
                        Bes = ARTCreateElement(mySim, "Bes", "Besselhorn");

                        ARTSetParameter(mySim, "Cyl.r = 1; Cyl.length = 50;");
                        ARTSetParameter(mySim, "Con.length = 50;Con.r1 = 1; Con.r2 = 1.5;");
                        ARTSetParameter(mySim, "Bes.length = 20; Bes.r1 = 1.5; Bes.r2 = 6;");

                        ARTSetFrequencyRange(mySim, 900, 901, 1);
                        ARTSetNModes(mySim, 1);
                }
                virtual void unprepare()
                {
                        ARTDestroyElement(mySim, Cyl);
                        ARTDestroyElement(mySim, Cyl2);
                        ARTDestroyElement(mySim, Con);
                        ARTDestroyElement(mySim, Bes);
                        ARTDestroySimulator(mySim);
                        //ARTRootDestroy();
                }
};
addAndRemoveElementsCalculateImpedanceClass* addAndRemoveElementsCalculateImpedance = new addAndRemoveElementsCalculateImpedanceClass();

TEST_DEF_START(remElementFromBeginning, addAndRemoveElementsCalculateImpedance)

        P_ART_Simulator mySim;
        P_ART_Circuit myIns;

        virtual void prepare()
        {
                mySim = addAndRemoveElementsCalculateImpedance->mySim;
                myIns = ARTCreateCircuit(mySim, "MyInstrument");
        }
        virtual bool run()
        {
                try
                {
                        ARTSetParameter(mySim, "Cyl2.r = 1; Cyl2.length = 50;");

                        ARTAppendReference(myIns, addAndRemoveElementsCalculateImpedance->Cyl2);
                        ARTAppendReference(myIns, addAndRemoveElementsCalculateImpedance->Cyl);
                        ARTAppendReference( myIns, addAndRemoveElementsCalculateImpedance->Con);
                        ARTAppendReference(myIns, addAndRemoveElementsCalculateImpedance->Bes);

                        P_ART_DataProp myImpCurve = ARTInputImpedance(myIns);

                        if (int(myImpCurve->val->nt[0].f) != 900) return false;
                        if (int(myImpCurve->val->nt[0].re) != 554184) return false;
                        if (int(myImpCurve->val->nt[0].im) != -822264) return false;

                        ARTRemoveReference( myIns, addAndRemoveElementsCalculateImpedance->Cyl2);
                        myImpCurve = ARTInputImpedance(myIns);

                        if (int(myImpCurve->val->nt[0].f) != 900) return false;
                        if (int(myImpCurve->val->nt[0].re) != 445944) return false;
                        if (int(myImpCurve->val->nt[0].im) != 780980) return false;
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }
        virtual void unprepare()
        {
                ARTDestroyCircuit(mySim,myIns);
        }
TEST_DEF_END(remElementFromBeginning)

TEST_DEF_START(remElementFromEnd, addAndRemoveElementsCalculateImpedance)

        P_ART_Simulator mySim;
        P_ART_Circuit myIns;

        virtual void prepare()
        {
                mySim = addAndRemoveElementsCalculateImpedance->mySim;
                myIns = ARTCreateCircuit(mySim, "MyInstrument");
        }
        virtual bool run()
        {
                try
                {
                        ARTSetParameter(mySim, "Cyl2.r = 6; Cyl2.length = 10;");

                        ARTAppendReference(myIns, addAndRemoveElementsCalculateImpedance->Cyl);
                        ARTAppendReference( myIns, addAndRemoveElementsCalculateImpedance->Con);
                        ARTAppendReference(myIns, addAndRemoveElementsCalculateImpedance->Bes);
                        ARTAppendReference(myIns, addAndRemoveElementsCalculateImpedance->Cyl2);

                        P_ART_DataProp myImpCurve = ARTInputImpedance(myIns);

                        if (int(myImpCurve->val->nt[0].f) != 900) return false;
                        if (int(myImpCurve->val->nt[0].re) != 247368) return false;
                        if (int(myImpCurve->val->nt[0].im) != 207467) return false;

                        ARTRemoveReference( myIns, addAndRemoveElementsCalculateImpedance->Cyl2);
                        myImpCurve = ARTInputImpedance(myIns);

                        if (int(myImpCurve->val->nt[0].f) != 900) return false;
                        if (int(myImpCurve->val->nt[0].re) != 445944) return false;
                        if (int(myImpCurve->val->nt[0].im) != 780980) return false;
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }
        virtual void unprepare()
        {
                ARTDestroyCircuit(mySim,myIns);
        }

TEST_DEF_END(remElementFromEnd)

TEST_DEF_START(remElementFromMiddle, addAndRemoveElementsCalculateImpedance)

        P_ART_Simulator mySim;
        P_ART_Circuit myIns;

        virtual void prepare()
        {
                mySim = addAndRemoveElementsCalculateImpedance->mySim;
                myIns = ARTCreateCircuit(mySim, "MyInstrument");
        }
        virtual bool run()
        {
                try
                {
                        ARTSetParameter(mySim, "Cyl2.r = 1; Cyl2.length = 50;");

                        ARTAppendReference(myIns, addAndRemoveElementsCalculateImpedance->Cyl);
                        ARTAppendReference(myIns, addAndRemoveElementsCalculateImpedance->Cyl2);
                        ARTAppendReference( myIns, addAndRemoveElementsCalculateImpedance->Con);
                        ARTAppendReference(myIns, addAndRemoveElementsCalculateImpedance->Bes);

                        P_ART_DataProp myImpCurve = ARTInputImpedance(myIns);

                        if (int(myImpCurve->val->nt[0].f) != 900) return false;
                        if (int(myImpCurve->val->nt[0].re) != 554184) return false;
                        if (int(myImpCurve->val->nt[0].im) != -822264) return false;

                        ARTRemoveReference( myIns, addAndRemoveElementsCalculateImpedance->Cyl2);
                        myImpCurve = ARTInputImpedance(myIns);

                        if (int(myImpCurve->val->nt[0].f) != 900) return false;
                        if (int(myImpCurve->val->nt[0].re) != 445944) return false;
                        if (int(myImpCurve->val->nt[0].im) != 780980) return false;
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }
        virtual void unprepare()
        {
                ARTDestroyCircuit(mySim,myIns);
        }

TEST_DEF_END(remElementFromMiddle)


TEST_DEF_START(remElementFromMiddle2, addAndRemoveElementsCalculateImpedance)

        P_ART_Simulator mySim;
        P_ART_Circuit myIns;

        virtual void prepare()
        {
                mySim = addAndRemoveElementsCalculateImpedance->mySim;
                myIns = ARTCreateCircuit(mySim, "MyInstrument");
        }
        virtual bool run()
        {
                try
                {
                        ARTSetParameter(mySim, "Cyl2.r = 1.5; Cyl2.length = 50;");

                        ARTAppendReference(myIns, addAndRemoveElementsCalculateImpedance->Cyl);
                        ARTAppendReference( myIns, addAndRemoveElementsCalculateImpedance->Con);
                        ARTAppendReference(myIns, addAndRemoveElementsCalculateImpedance->Cyl2);
                        ARTAppendReference(myIns, addAndRemoveElementsCalculateImpedance->Bes);

                        P_ART_DataProp myImpCurve = ARTInputImpedance(myIns);

                        if (int(myImpCurve->val->nt[0].f) != 900) return false;
                        if (int(myImpCurve->val->nt[0].re) != 458066) return false;
                        if (int(myImpCurve->val->nt[0].im) != -741964) return false;

                        ARTRemoveReference( myIns, addAndRemoveElementsCalculateImpedance->Cyl2);
                        myImpCurve = ARTInputImpedance(myIns);

                        if (int(myImpCurve->val->nt[0].f) != 900) return false;
                        if (int(myImpCurve->val->nt[0].re) != 445944) return false;
                        if (int(myImpCurve->val->nt[0].im) != 780980) return false;
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }
        virtual void unprepare()
        {
                ARTDestroyCircuit(mySim,myIns);
        }

TEST_DEF_END(remElementFromMiddle2)

//Add elements
TEST_DEF_START(addElementToBeginning, addAndRemoveElementsCalculateImpedance)

        P_ART_Simulator mySim;
        P_ART_Circuit myIns;

        virtual void prepare()
        {
                mySim = addAndRemoveElementsCalculateImpedance->mySim;
                myIns = ARTCreateCircuit(mySim, "MyInstrument");
        }
        virtual bool run()
        {
                try
                {
                        ARTSetParameter(mySim, "Cyl2.r = 1; Cyl2.length = 50;");

                        ARTAppendReference(myIns, addAndRemoveElementsCalculateImpedance->Cyl);
                        ARTAppendReference( myIns, addAndRemoveElementsCalculateImpedance->Con);
                        ARTAppendReference(myIns, addAndRemoveElementsCalculateImpedance->Bes);

                        P_ART_DataProp myImpCurve = ARTInputImpedance(myIns);

                        if (int(myImpCurve->val->nt[0].f) != 900) return false;
                        if (int(myImpCurve->val->nt[0].re) != 445944) return false;
                        if (int(myImpCurve->val->nt[0].im) != 780980) return false;

                        ARTAppendReferenceBefore( myIns, addAndRemoveElementsCalculateImpedance->Cyl, addAndRemoveElementsCalculateImpedance->Cyl2);
                        myImpCurve = ARTInputImpedance(myIns);

                        if (int(myImpCurve->val->nt[0].f) != 900) return false;
                        if (int(myImpCurve->val->nt[0].re) != 554184) return false;
                        if (int(myImpCurve->val->nt[0].im) != -822264) return false;
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }
        virtual void unprepare()
        {
                ARTDestroyCircuit(mySim,myIns);
        }
TEST_DEF_END(addElementToBeginning)

TEST_DEF_START(addElementToEnd, addAndRemoveElementsCalculateImpedance)

        P_ART_Simulator mySim;
        P_ART_Circuit myIns;

        virtual void prepare()
        {
                mySim = addAndRemoveElementsCalculateImpedance->mySim;
                myIns = ARTCreateCircuit(mySim, "MyInstrument");
        }
        virtual bool run()
        {
                try
                {
                        ARTSetParameter(mySim, "Cyl2.r = 6; Cyl2.length = 10;");

                        ARTAppendReference(myIns, addAndRemoveElementsCalculateImpedance->Cyl);
                        ARTAppendReference( myIns, addAndRemoveElementsCalculateImpedance->Con);
                        ARTAppendReference(myIns, addAndRemoveElementsCalculateImpedance->Bes);

                        P_ART_DataProp myImpCurve = ARTInputImpedance(myIns);

                        if (int(myImpCurve->val->nt[0].f) != 900) return false;
                        if (int(myImpCurve->val->nt[0].re) != 445944) return false;
                        if (int(myImpCurve->val->nt[0].im) != 780980) return false;

                        ARTAppendReferenceAfter( myIns, addAndRemoveElementsCalculateImpedance->Bes, addAndRemoveElementsCalculateImpedance->Cyl2);
                        myImpCurve = ARTInputImpedance(myIns);

                        if (int(myImpCurve->val->nt[0].f) != 900) return false;
                        if (int(myImpCurve->val->nt[0].re) != 247368) return false;
                        if (int(myImpCurve->val->nt[0].im) != 207467) return false;
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }
        virtual void unprepare()
        {
                ARTDestroyCircuit(mySim,myIns);
        }

TEST_DEF_END(addElementToEnd)

TEST_DEF_START(addElementToMiddle, addAndRemoveElementsCalculateImpedance)

        P_ART_Simulator mySim;
        P_ART_Circuit myIns;

        virtual void prepare()
        {
                mySim = addAndRemoveElementsCalculateImpedance->mySim;
                myIns = ARTCreateCircuit(mySim, "MyInstrument");
        }
        virtual bool run()
        {
                try
                {
                        ARTSetParameter(mySim, "Cyl2.r = 1; Cyl2.length = 50;");

                        ARTAppendReference(myIns, addAndRemoveElementsCalculateImpedance->Cyl);
                        ARTAppendReference( myIns, addAndRemoveElementsCalculateImpedance->Con);
                        ARTAppendReference(myIns, addAndRemoveElementsCalculateImpedance->Bes);

                        P_ART_DataProp myImpCurve = ARTInputImpedance(myIns);

                        if (int(myImpCurve->val->nt[0].f) != 900) return false;
                        if (int(myImpCurve->val->nt[0].re) != 445944) return false;
                        if (int(myImpCurve->val->nt[0].im) != 780980) return false;


                        ARTAppendReferenceAfter( myIns, addAndRemoveElementsCalculateImpedance->Cyl, addAndRemoveElementsCalculateImpedance->Cyl2);
                        myImpCurve = ARTInputImpedance(myIns);

                        if (int(myImpCurve->val->nt[0].f) != 900) return false;
                        if (int(myImpCurve->val->nt[0].re) != 554184) return false;
                        if (int(myImpCurve->val->nt[0].im) != -822264) return false;
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();
                        return false;
                }
                return true;
        }
        virtual void unprepare()
        {
                ARTDestroyCircuit(mySim,myIns);
        }

TEST_DEF_END(addElementToMiddle)


TEST_DEF_START(addElementToMiddle2, addAndRemoveElementsCalculateImpedance)

        P_ART_Simulator mySim;
        P_ART_Circuit myIns;

        virtual void prepare()
        {
                mySim = addAndRemoveElementsCalculateImpedance->mySim;
                myIns = ARTCreateCircuit(mySim, "MyInstrument");
        }
        virtual bool run()
        {
                try
                {
                        ARTSetParameter(mySim, "Cyl2.r = 1.5; Cyl2.length = 50;");

                        ARTAppendReference(myIns, addAndRemoveElementsCalculateImpedance->Cyl);
                        ARTAppendReference( myIns, addAndRemoveElementsCalculateImpedance->Con);
                        ARTAppendReference(myIns, addAndRemoveElementsCalculateImpedance->Bes);

                        P_ART_DataProp myImpCurve = ARTInputImpedance(myIns);


                        if (int(myImpCurve->val->nt[0].f) != 900) return false;
                        if (int(myImpCurve->val->nt[0].re) != 445944) return false;
                        if (int(myImpCurve->val->nt[0].im) != 780980) return false;

                        ARTAppendReferenceAfter( myIns, addAndRemoveElementsCalculateImpedance->Con, addAndRemoveElementsCalculateImpedance->Cyl2);
                        myImpCurve = ARTInputImpedance(myIns);

                        if (int(myImpCurve->val->nt[0].f) != 900) return false;
                        if (int(myImpCurve->val->nt[0].re) != 458066) return false;
                        if (int(myImpCurve->val->nt[0].im) != -741964) return false;
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }
        virtual void unprepare()
        {
                ARTDestroyCircuit(mySim,myIns);
        }

TEST_DEF_END(addElementToMiddle2)





//******************************************************************************************************************************************

TestClass* ARTobjectsTests = new TestClass("ARTobjectsTests", AllMyTests);

TEST_DEF_START(ARTobjectCopyConstr, ARTobjectsTests)
        virtual bool run()
        {
                try
                {
                        ARTobject* obj = new ARTobject("ARTobjectCopyConstr-Testobject","This is just a test object.");
                        obj->AppendDataProp("length", 100.0,"axial length [cm]"); //is this mm??
                        obj->AppendDataProp("r", 1.0,"duct radius [cm]"); //is this mm???
                        obj->AppendDataProp("temp", 21.0,"local temperature [degC]");
                        obj->AppendDataProp("loss", 1.0,"loss factor (defaults to 1)");
                        obj->AppendMethod("SphericalWave", "MapesRiordan Model");
                        obj->AppendMethod("PlaneWave", "first mode only");
                        obj->AppendMethod("MultiModal", "modal decomposition up to n modes (Kemp02)");
                        obj->AppendMethod("FrequencyDomain", "simulation in frq domain");

                        ARTobject* obj2 = (ARTobject*) obj->clone();

                        //Check if all the properties are there. This check will assume an order... anyone who has time to implement it in a more sophisticated way is welcome.
                        string obj_prp = "";
                        Property* prp;
                        ARTmethod* mtd;
                        for (prp = obj->GetProperties(NULL); prp != NULL; prp = obj->GetProperties(prp))
                                obj_prp += prp->GetName() + ", ";

                        for (mtd = obj->GetMethods(NULL); mtd != NULL; mtd = obj->GetMethods(mtd))
                                obj_prp += mtd->GetName() + ", ";

                        string obj2_prp = "";
                        for (prp = obj2->GetProperties(NULL); prp != NULL; prp = obj2->GetProperties(prp))
                                obj2_prp += prp->GetName() + ", ";

                        for (mtd = obj2->GetMethods(NULL); mtd != NULL; mtd = obj2->GetMethods(mtd))
                                obj2_prp += mtd->GetName() + ", ";

                        //they must be similar
                        if (obj_prp != obj2_prp) return false;
                        //and look like this
                        if (obj_prp !=  "length, r, temp, loss, SphericalWave, PlaneWave, MultiModal, FrequencyDomain, ") return false;

                        delete obj;
                }
                catch(ARTerror e)
                {
                        return false;
                }
                return true;
        }
TEST_DEF_END(ARTobjectCopyConstr)


TEST_DEF_START(ARTdataPropRange, ARTobjectsTests)
        virtual bool run()
        {
                try
                {
                        ARTobject* obj = new ARTobject("ARTobjectCopyConstr-Testobject","This is just a test object.");

                        ARTdataProp* dp = obj->AppendDataProp("radiation", "Zorumski","The radiation impedance. Can be either 'Reflecting' or 'Zorumski'.");
                        dp->SetRange(new ARTvariant("Reflecting", "Zorumski", "Levine"));

                        dp = obj->AppendDataProp("length", 100.0,"axial length [cm]");
                        ARTvariant* av = new ARTvariant(C_ART_ndbl, 2);
                        av->val->nd[0] = 0;
                        av->val->nd[1] = 999;
                        dp->SetRange(av);

                        dp = obj->AppendDataProp("int", 1.0,"some integer parameter"); //is this mm???
                        av = new ARTvariant(C_ART_nint, 2);
                        av->val->ni[0] = 0;
                        av->val->ni[1] = 999;
                        dp->SetRange(av);

                        dp = obj->AppendDataProp("temp", 21.0,"local temperature [degC]");
                        av = new ARTvariant(C_ART_ndbl, 2);
                        av->val->nd[0] = -273.15; //�C = 0 K
                        av->val->nd[1] = 1000; //�C
                        dp->SetRange(av);

                        dp = obj->AppendDataProp("loss", 1.0f,"loss factor as floating point number");
                        av = new ARTvariant(C_ART_nflo, 2);
                        av->val->nf[0] = 0.0f;
                        av->val->nf[1] = 10.0f; //just some dummy value
                        dp->SetRange(av);

                        dp = obj->AppendDataProp("noRangeProp", 1.0f,"some property without any range specified");

                        obj->AppendMethod("SphericalWave", "MapesRiordan Model");
                        obj->AppendMethod("PlaneWave", "first mode only");
                        obj->AppendMethod("MultiModal", "modal decomposition up to n modes (Kemp02)");
                        obj->AppendMethod("FrequencyDomain", "simulation in frq domain");

                        //Check if all the ranges are set
                        dp = static_cast<ARTdataProp*>(obj->FindProperty("radiation"));
                        av = dp->GetRange();
                        if (0 != strcmp(av->val->ns[0], "Reflecting")) return false;
                        if (0 != strcmp(av->val->ns[1], "Zorumski")) return false;
                        if (0 != strcmp(av->val->ns[2], "Levine")) return false;
                        if (C_ART_nstr != av->typ) return false;
                        if (3 != av->len) return false;

                        dp = static_cast<ARTdataProp*>(obj->FindProperty("length"));
                        av = dp->GetRange();
                        if (0 != av->val->nd[0]) return false;
                        if (999 != av->val->nd[1]) return false;
                        if (C_ART_ndbl != av->typ) return false;
                        if (2 != av->len) return false;

                        dp = static_cast<ARTdataProp*>(obj->FindProperty("int"));
                        av = dp->GetRange();
                        if (0 != av->val->ni[0]) return false;
                        if (999 != av->val->ni[1]) return false;
                        if (C_ART_nint != av->typ) return false;
                        if (2 != av->len) return false;

                        dp = static_cast<ARTdataProp*>(obj->FindProperty("temp"));
                        av = dp->GetRange();
                        if (-273.15 != av->val->nd[0]) return false;
                        if (1000 != av->val->nd[1]) return false;
                        if (C_ART_ndbl != av->typ) return false;
                        if (2 != av->len) return false;

                        dp = static_cast<ARTdataProp*>(obj->FindProperty("loss"));
                        av = dp->GetRange();
                        if (0 != av->val->nf[0]) return false;
                        if (10 != av->val->nf[1]) return false;
                        if (C_ART_nflo != av->typ) return false;
                        if (2 != av->len) return false;

                        dp = static_cast<ARTdataProp*>(obj->FindProperty("noRangeProp"));
                        av = dp->GetRange();
                        if (av) return false;  // there's no range specified: range should be NULL!

                        delete obj;
                }
                catch(ARTerror e)
                {
                        return false;
                }
                return true;
        }
TEST_DEF_END(ARTdataPropRange)
//******************************************************************************************************************************************

TestClass* ARTmodelTests = new TestClass("ARTmodelTests", AllMyTests);

TEST_DEF_START(createUnknownHElements, ARTmodelTests)

        P_ART_Simulator mySim;

        virtual void prepare()
        {
                        ARTRootObject();
                        mySim = ARTCreateSimulator("MeinSimulator", "FrequencyDomain", "MultiModal");
        }

        virtual bool run() //test creating unexisting element -> should fail
        {
                try
                {
                        P_ART_Element El1 = ARTCreateElement(mySim, "error", "UnknownZylinder");
                }
                catch(ARTerror e)
                {
                        return true;
                }
                return false;
        }


        virtual void unprepare()
        {
                        ARTDestroySimulator(mySim);
                        ARTRootDestroy();
        }

TEST_DEF_END(createUnknownHElements)

TEST_DEF_START(createHElements, ARTmodelTests)

        P_ART_Simulator mySim;

        virtual void prepare()
        {
                        ARTRootObject();
                        mySim = ARTCreateSimulator("MeinSimulator", "FrequencyDomain", "MultiModal");
        }

        virtual bool run() //test creating elements
        {
                try
                {
                        P_ART_Element El1 = ARTCreateElement(mySim, "SZ1", "Cylinder");
                        P_ART_Element El2 = ARTCreateElement(mySim, "SZ2", "Cylinder");
                }
                catch(ARTerror e)
                {
                        return false;
                }
                return true;
        }


        virtual void unprepare()
        {
                        ARTDestroySimulator(mySim);
                        ARTRootDestroy();
        }

TEST_DEF_END(createHElements)


TEST_DEF_START(createHElementTwice, ARTmodelTests)

        P_ART_Simulator mySim;

        virtual void prepare()
        {
                        ARTRootObject();
                        mySim = ARTCreateSimulator("MeinSimulator", "FrequencyDomain", "MultiModal");
        }

        virtual bool run() //test using the same element name twice
        {
                try
                {
                        P_ART_Element El1 = ARTCreateElement(mySim, "Twice", "Cylinder");
                        P_ART_Element El2 = ARTCreateElement(mySim, "Twice", "Cylinder");
                }
                catch(ARTerror e)
                {
                        return true;
                }
                return false; //should not be successfull!
        }


        virtual void unprepare()
        {
                        ARTDestroySimulator(mySim);
                        ARTRootDestroy();
        }

TEST_DEF_END(createHElementTwice)


TEST_DEF_START(setHElementParam, ARTmodelTests)

        P_ART_Simulator mySim;
        P_ART_Element El1;

        virtual void prepare()
        {
                        ARTRootObject();
                        mySim = ARTCreateSimulator("MeinSimulator", "FrequencyDomain", "MultiModal");
                        El1 = ARTCreateElement(mySim, "SZ", "Cylinder");
        }

        virtual bool run()
        {
                try
                {
                        ARTSetParameter(mySim, "SZ.length = 1000; ");
                        ARTSetParameter(mySim, "SZ.r = SZ.length * 0.25;");
                        DataContainer* p1 = dynamic_cast<DataContainer*>(((Element*)El1)->model->FindProperty("length"));
                        DataContainer* p2 = dynamic_cast<DataContainer*>(((Element*)El1)->model->FindProperty("r"));
                        //std::cout << p1->GetValue()->d << "\n";
                        //std::cout << p2->GetValue()->d << "\n";
                        if (p1->GetFloat() != 1000) return false;
                        if (p2->GetFloat() != 250) return false;
                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true; //should be successfull!
        }


        virtual void unprepare()
        {
                        ARTDestroyElement(mySim,El1);
                        ARTDestroySimulator(mySim);
                        ARTRootDestroy();
        }

TEST_DEF_END(setHElementParam)

TEST_DEF_START(getHElementImpedance, ARTmodelTests)

        P_ART_Simulator mySim;
        P_ART_Element El1;
        P_ART_Element El2;

        P_ART_Circuit Cir;

        virtual void prepare()
        {
                ARTRootObject();
                mySim = ARTCreateSimulator("MeinSimulator", "FrequencyDomain", "MultiModal");
                ARTSetFrequencyRange    (mySim, 10, 200, 5);
                ARTSetNModes    (mySim, 1);
                Cir = ARTCreateCircuit(mySim, "MeinInstrument");
                El1 = ARTCreateElement(mySim, "Zyl", "Cylinder");
                El2 = ARTCreateElement(mySim, "Kon", "Cone");
        }

        virtual bool run()
        {
                try
                {
                        //Elements must be in circuit, which will check for bends!
                        ARTAppendReference(Cir,El1);
                        ARTAppendReference(Cir,El2);
                        ((Element*)El1)->model->SetCircuit(Cir);
                        ((Element*)El2)->model->SetCircuit(Cir);

                        ARTSetParameter(mySim, "Zyl.length = 100; ");
                        WaveObjectInterface* Z_R = NULL;
                        WaveObjectInterface* Z_EL1 = NULL;
                        WaveObjectInterface* Z_I = NULL;

                        ((Element*)El1)->RadiationImpedance(Z_R);
                        ((Element*)El1)->InputImpedance(Z_R, Z_EL1);
                        ((Element*)El2)->InputImpedance(Z_EL1, Z_I);

                        DataContainer* z = NULL;
                        Z_I->ImpedanceMatrix(z);

                        T_ART_Var* var = z->GetValue();

                        //Check some values
                        if (int(var->nt[0].f) != 10) return false;
                        if (int(var->nt[0].re) != 2449) return false;
                        if (int(var->nt[0].im) != 328242) return false;
                        if (int(var->nt[1].f) != 15) return false;
                        if (int(var->nt[1].re) != 13481) return false;
                        if (int(var->nt[1].im) != 683379) return false;

                        //for (int i = 0; i < z->len; i++)
                        //{
                        //      std::cout << int( z->GetValue()->nt[i].f )<< "Hz: ";
                        //      std::cout << int( z->GetValue()->nt[i].re )<< " + ";
                        //      std::cout << int( z->GetValue()->nt[i].im )<< " * i\n";
                        //}

                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }


        virtual void unprepare()
        {
                ARTDestroyCircuit(mySim,Cir);
                ARTDestroyElement(mySim,El1);
                ARTDestroySimulator(mySim);
                ARTRootDestroy();
        }

TEST_DEF_END(getHElementImpedance)


//******************************************************************************************************************************************

TestClass* customModelTests = new TestClass("customModelTests", AllMyTests);

TEST_DEF_START(testToneHole, customModelTests)

        P_ART_Simulator mySim;

        virtual void prepare()
        {
                        ARTRootObject();
                        mySim = ARTCreateSimulator("MeinSimulator", "FrequencyDomain", "MultiModal");
        }

        virtual bool run()
        {
                P_ART_Element El1;
                try
                {
                        El1 = ARTCreateElement(mySim, "myToneHole", "ToneHole");
                }
                catch(ARTerror e)
                {
                        return false;
                }

                ARTDestroyElement(mySim,El1);
                return true;
        }


        virtual void unprepare()
        {
                        ARTDestroySimulator(mySim);
                        ARTRootDestroy();
        }

TEST_DEF_END(testToneHole)

TEST_DEF_START(testToneHoleParam, customModelTests)

        P_ART_Simulator mySim;
        P_ART_Element Th;

        virtual void prepare()
        {
                        ARTRootObject();
                        mySim = ARTCreateSimulator("MeinSimulator", "FrequencyDomain", "MultiModal");
                        Th = ARTCreateElement(mySim, "myToneHole", "ToneHole");
        }

        virtual bool run()
        {
                try
                {
                        ARTSetParameter(mySim, "myToneHole.length = 100; ");
                        ARTSetParameter(mySim, "myToneHole.r = myToneHole.length * 0.25;");
                        DataContainer* p1 = dynamic_cast<DataContainer*>(((Element*)Th)->model->FindProperty("length"));
                        DataContainer* p2 = dynamic_cast<DataContainer*>(((Element*)Th)->model->FindProperty("r"));
                        if (p1->GetFloat() != 100) return false;
                        if (p2->GetFloat() != 25) return false;
                }
                catch(ARTerror e)
                {
                        return false;
                }

                return true;
        }


        virtual void unprepare()
        {
                ARTDestroyElement(mySim,Th);
                ARTDestroySimulator(mySim);
                ARTRootDestroy();
        }

TEST_DEF_END(testToneHoleParam)


TEST_DEF_START(testToneHoleImp, customModelTests)

        P_ART_Simulator mySim;
        P_ART_Element Cyl1;
        P_ART_Element Cyl2;
        P_ART_Element Th;
        P_ART_Circuit meinIns;

        virtual void prepare()
        {
                ARTRootObject();
                mySim = ARTCreateSimulator("MeinSimulator", "FrequencyDomain", "MultiModal");
                Cyl1 = ARTCreateElement(mySim, "Cyl1", "Cylinder");
                Th = ARTCreateElement(mySim, "myToneHole", "ToneHole");
                Cyl2 = ARTCreateElement(mySim, "Cyl2", "Cylinder");
                ARTSetParameter(mySim, "Cyl1.length = 50; ");
                ARTSetParameter(mySim, "Cyl1.r = 1;");
                ARTSetParameter(mySim, "myToneHole.length = 2; ");
                ARTSetParameter(mySim, "myToneHole.r = 2;");
                ARTSetParameter(mySim, "myToneHole.radiation = Zorumski;");
                ARTSetParameter(mySim, "Cyl2.length = 25; ");
                ARTSetParameter(mySim, "Cyl2.r = 1;");
                meinIns = ARTCreateCircuit(mySim, "MeinInstrument");
                ARTSetFrequencyRange(mySim, 10, 200, 5);
                ARTSetNModes(mySim, 1);
        }

        virtual bool run()
        {
                try
                {

                        ARTAppendReference(meinIns, Cyl1);
                        ARTAppendReference(meinIns, Th);
                        ARTAppendReference(meinIns, Cyl2);
                        ARTvariant* meineImpKurve = ARTInputImpedance(meinIns);
                        meineImpKurve = ARTInputImpedance(meinIns);

                        //Check some values
                        if (int(meineImpKurve->val->nt[0].f) != 10) return false;
                        if (int(meineImpKurve->val->nt[0].re) != 32) return false;
                        if (int(meineImpKurve->val->nt[0].im) != 115272) return false;
                        if (int(meineImpKurve->val->nt[1].f) != 15) return false;
                        if (int(meineImpKurve->val->nt[1].re) != 95) return false;
                        if (int(meineImpKurve->val->nt[1].im) != 178563) return false;

                        //for (int i = 0; i < meineImpKurve->len; i++)
                        //{
                        //      std::cout << int( meineImpKurve->val->nt[i].f )<< "Hz: ";
                        //      std::cout << int( meineImpKurve->val->nt[i].re )<< " + ";
                        //      std::cout << int( meineImpKurve->val->nt[i].im )<< " * i\n";
                        //}

                }
                catch(ARTerror e)
                {
                        string err = e.GetErrorMessage();
                        std::cout << "\n\n" << err;
                        return false;
                }

                return true;
        }


        virtual void unprepare()
        {
                ARTDestroyCircuit(mySim, meinIns);
                ARTDestroyElement(mySim,Cyl2);
                ARTDestroyElement(mySim,Th);
                ARTDestroyElement(mySim,Cyl1);
                ARTDestroySimulator(mySim);
                ARTRootDestroy();
        }

TEST_DEF_END(testToneHoleImp)


TEST_DEF_START(testToneHoleImpModes2, customModelTests)

        P_ART_Simulator mySim;
        P_ART_Element Cyl1;
        P_ART_Element Cyl2;
        P_ART_Element Th;
        P_ART_Circuit meinIns;

        virtual void prepare()
        {
                ARTRootObject();
                mySim = ARTCreateSimulator("MeinSimulator", "FrequencyDomain", "MultiModal");
                Cyl1 = ARTCreateElement(mySim, "Cyl1", "Cylinder");
                Th = ARTCreateElement(mySim, "myToneHole", "ToneHole");
                Cyl2 = ARTCreateElement(mySim, "Cyl2", "Cylinder");
                ARTSetParameter(mySim, "Cyl1.length = 50; ");
                ARTSetParameter(mySim, "Cyl1.r = 1;");
                ARTSetParameter(mySim, "myToneHole.length = 2; ");
                ARTSetParameter(mySim, "myToneHole.r = 2;");
                ARTSetParameter(mySim, "myToneHole.radiation = Zorumski;");
                ARTSetParameter(mySim, "Cyl2.length = 25; ");
                ARTSetParameter(mySim, "Cyl2.r = 1;");
                meinIns = ARTCreateCircuit(mySim, "MeinInstrument");
                ARTSetFrequencyRange(mySim, 10, 200, 5);
                ARTSetNModes(mySim, 2);
        }

        virtual bool run()
        {
                try
                {
                        ARTAppendReference(meinIns, Cyl1);
                        ARTAppendReference(meinIns, Th);
                        ARTAppendReference(meinIns, Cyl2);
                        ARTvariant* meineImpKurve = ARTInputImpedance(meinIns);
                        meineImpKurve = ARTInputImpedance(meinIns);

                        //Check some values
                        if (int(meineImpKurve->val->nt[0].f) != 10) return false;
                        if (int(meineImpKurve->val->nt[0].re) != 32) return false;
                        if (int(meineImpKurve->val->nt[0].im) != 115235) return false;
                        if (int(meineImpKurve->val->nt[1].f) != 15) return false;
                        if (int(meineImpKurve->val->nt[1].re) != 95) return false;
                        if (int(meineImpKurve->val->nt[1].im) != 178539) return false;


                }
                catch(ARTerror e)
                {
                        string err = e.GetErrorMessage();
                        std::cout << "\n\n" << err;
                        return false;
                }

                return true;
        }


        virtual void unprepare()
        {
                ARTDestroyCircuit(mySim, meinIns);
                ARTDestroyElement(mySim,Cyl2);
                ARTDestroyElement(mySim,Th);
                ARTDestroyElement(mySim,Cyl1);
                ARTDestroySimulator(mySim);
                ARTRootDestroy();
        }

TEST_DEF_END(testToneHoleImpModes2)


TEST_DEF_START(testClosedToneHoleImp, customModelTests)

        P_ART_Simulator mySim;
        P_ART_Element Cyl1;
        P_ART_Element Cyl2;
        P_ART_Element Th;
        P_ART_Circuit meinIns;

        virtual void prepare()
        {
                ARTRootObject();
                mySim = ARTCreateSimulator("MeinSimulator", "FrequencyDomain", "MultiModal");
                Cyl1 = ARTCreateElement(mySim, "Cyl1", "Cylinder");
                Th = ARTCreateElement(mySim, "myToneHole", "ToneHole");
                Cyl2 = ARTCreateElement(mySim, "Cyl2", "Cylinder");
                ARTSetParameter(mySim, "Cyl1.length = 50; ");
                ARTSetParameter(mySim, "Cyl1.r = 1;");
                ARTSetParameter(mySim, "myToneHole.length = 2; ");
                ARTSetParameter(mySim, "myToneHole.r = 2;");
                ARTSetParameter(mySim, "myToneHole.radiation = Reflecting;");
                ARTSetParameter(mySim, "Cyl2.length = 25; ");
                ARTSetParameter(mySim, "Cyl2.r = 1;");
                meinIns = ARTCreateCircuit(mySim, "MeinInstrument");
                ARTSetFrequencyRange(mySim, 10, 200, 5);
                ARTSetNModes(mySim, 1);
        }

        virtual bool run()
        {
                try
                {
                        ARTAppendReference(meinIns, Cyl1);
                        ARTAppendReference(meinIns, Th);
                        ARTAppendReference(meinIns, Cyl2);
                        ARTvariant* meineImpKurve = ARTInputImpedance(meinIns);
                        meineImpKurve = ARTInputImpedance(meinIns);

                        //std::cout << int(meineImpKurve->val->nt[0].re) << "\n";
                        //std::cout << int(meineImpKurve->val->nt[0].im) << "\n";
                        //std::cout << int(meineImpKurve->val->nt[1].re) << "\n";
                        //std::cout << int(meineImpKurve->val->nt[1].im) << "\n";

                        //Check some values (these values are slightly different from the values of the open tonehole)
                        if (int(meineImpKurve->val->nt[0].f) != 10) return false;
                        if (int(meineImpKurve->val->nt[0].re) != 116) return false;
                        if (int(meineImpKurve->val->nt[0].im) != 179168) return false;
                        if (int(meineImpKurve->val->nt[1].f) != 15) return false;
                        if (int(meineImpKurve->val->nt[1].re) != 338) return false;
                        if (int(meineImpKurve->val->nt[1].im) != 274463) return false;



                }
                catch(ARTerror e)
                {
                        string err = e.GetErrorMessage();
                        std::cout << "\n\n" << err;
                        return false;
                }

                return true;
        }


        virtual void unprepare()
        {
                ARTDestroyCircuit(mySim, meinIns);
                ARTDestroyElement(mySim,Cyl2);
                ARTDestroyElement(mySim,Th);
                ARTDestroyElement(mySim,Cyl1);
                ARTDestroySimulator(mySim);
                ARTRootDestroy();
        }

TEST_DEF_END(testClosedToneHoleImp)

//******************************************************************************************************************************************

TestClass* BranchTests = new TestClass("BranchTests", AllMyTests);

TEST_DEF_START(addCircuitAsReference, BranchTests)

        P_ART_Simulator mySim;
        P_ART_Element Branch1El1;
        P_ART_Element Branch1El2;
        P_ART_Element Branch2El1;
        P_ART_Element Branch2El2;
        P_ART_Element Branch1El3;
        P_ART_Circuit Branch1;
        P_ART_Circuit Branch2;

        virtual void prepare()
        {
                        ARTRootObject();
                        mySim = ARTCreateSimulator("MeinSimulator", "FrequencyDomain", "MultiModal");
                        Branch1El1 = ARTCreateElement(mySim, "Branch1Element1", "Cylinder");
                        Branch1El2 = ARTCreateElement(mySim, "Branch1Element2", "Cylinder");
                        Branch2El1 = ARTCreateElement(mySim, "Branch2Element1", "Cylinder");
                        Branch2El2 = ARTCreateElement(mySim, "Branch2Element2", "Cylinder");
                        Branch1El3 = ARTCreateElement(mySim, "Branch1Element3", "Cylinder");
                        Branch1 = ARTCreateCircuit(mySim, "Branch1");
                        Branch2 = ARTCreateCircuit(mySim, "Branch2");
                        //create side branch
                        ARTAppendReference(Branch2,Branch2El1);
                        ARTAppendReference(Branch2,Branch2El2);
                        //create first part of main branch
                        ARTAppendReference(Branch1,Branch1El1);
                        ARTAppendReference(Branch1,Branch1El2);
                        //all is ready for the test :)
        }

        virtual bool run()
        {
                try
                {
                        //add a reference to a circuit - ie. a branch
                        ARTAppendReference(Branch1,Branch2);
                        //add one more cylinder to main branch
                        ARTAppendReference(Branch1,Branch1El3);
                        //get input impedance
                        ARTSetFrequencyRange(mySim, 10, 200, 5);
                        ARTSetNModes(mySim, 1);
                        ARTvariant* meineImpKurve = ARTInputImpedance(Branch1);

                        //for (int i = 0; i < meineImpKurve->len; i++)
                        //{
                        //      std::cout << int( meineImpKurve->val->nt[i].f )<< "Hz: ";
                        //      std::cout << int( meineImpKurve->val->nt[i].re )<< " + ";
                        //      std::cout << int( meineImpKurve->val->nt[i].im )<< " * i\n";
                        //}

                }
                catch(ARTerror e)
                {
                        std::cout << "\n\n" << e.GetErrorMessage();;
                        return false;
                }
                return true;
        }


        virtual void unprepare()
        {
                        ARTDestroyCircuit(mySim, Branch1);
                        ARTDestroyCircuit(mySim, Branch2);
                        ARTDestroyElement(mySim, Branch1El1);
                        ARTDestroyElement(mySim, Branch1El2);
                        ARTDestroyElement(mySim, Branch2El1);
                        ARTDestroyElement(mySim, Branch2El2);
                        ARTDestroyElement(mySim, Branch1El3);
                        ARTDestroySimulator(mySim);
                        ARTRootDestroy();
        }

TEST_DEF_END(addCircuitAsReference)


TEST_DEF_START(testCircuitAsBranchImpedance, BranchTests)

        P_ART_Simulator mySim;
        P_ART_Element Cyl1;
        P_ART_Element Cyl2;
        P_ART_Element Tunnel;
        P_ART_Circuit mainInst;
        P_ART_Circuit branch;

        virtual void prepare()
        {
                ARTRootObject();
                mySim = ARTCreateSimulator("MeinSimulator", "FrequencyDomain", "MultiModal");
                Cyl1 = ARTCreateElement(mySim, "Cyl1", "Cylinder");
                Tunnel = ARTCreateElement(mySim, "ToneHoleCyl", "Cylinder");
                Cyl2 = ARTCreateElement(mySim, "Cyl2", "Cylinder");
                ARTSetParameter(mySim, "Cyl1.length = 50; ");
                ARTSetParameter(mySim, "Cyl1.r = 1;");
                ARTSetParameter(mySim, "ToneHoleCyl.length = 2; ");
                ARTSetParameter(mySim, "ToneHoleCyl.r = 2;");
                ARTSetParameter(mySim, "Cyl2.length = 25; ");
                ARTSetParameter(mySim, "Cyl2.r = 1;");
                mainInst = ARTCreateCircuit(mySim, "mainInstrument");
                branch = ARTCreateCircuit(mySim, "branch");
                ARTSetFrequencyRange(mySim, 10, 200, 5);
                ARTSetNModes(mySim, 1);
        }

        virtual bool run()
        {
                try
                {
                        //add tonhole to circuit
                        ARTAppendReference( branch, Tunnel);

                        ARTAppendReference( mainInst, Cyl1);
                        ARTAppendReference( mainInst, branch);
                        ARTAppendReference( mainInst, Cyl2);
                        ARTvariant* meineImpKurve = ARTInputImpedance(mainInst);

                        //for (int i = 0; i < meineImpKurve->len; i++)
                        //{
                        //      std::cout << int( meineImpKurve->val->nt[i].f )<< "Hz: ";
                        //      std::cout << int( meineImpKurve->val->nt[i].re) << " " << int( meineImpKurve->val->nt[i].im)<< "\n";
                        //}

                        if (int(meineImpKurve->val->nt[0].f) != 10) return false;
                        if (int(meineImpKurve->val->nt[0].re) != 39) return false;
                        if (int(meineImpKurve->val->nt[0].im) != 124019) return false;
                        if (int(meineImpKurve->val->nt[1].f) != 15) return false;
                        if (int(meineImpKurve->val->nt[1].re) != 107) return false;
                        if (int(meineImpKurve->val->nt[1].im) != 186803) return false;
                }
                catch(ARTerror e)
                {
                        string err = e.GetErrorMessage();
                        std::cout << "\n\n" << err;
                        return false;
                }

                return true;
        }


        virtual void unprepare()
        {
                ARTDestroyCircuit(mySim, branch);
                ARTDestroyCircuit(mySim, mainInst);
                ARTDestroyElement(mySim,Cyl2);
                ARTDestroyElement(mySim,Tunnel);
                ARTDestroyElement(mySim,Cyl1);
                ARTDestroySimulator(mySim);
                ARTRootDestroy();
        }

TEST_DEF_END(testCircuitAsBranchImpedance)


TEST_DEF_START(testCircuitAsBranchImpedanceModes2, BranchTests)

        P_ART_Simulator mySim;
        P_ART_Element Cyl1;
        P_ART_Element Cyl2;
        P_ART_Element Tunnel;
        P_ART_Circuit mainInst;
        P_ART_Circuit branch;

        virtual void prepare()
        {
                ARTRootObject();
                mySim = ARTCreateSimulator("MeinSimulator", "FrequencyDomain", "MultiModal");
                Cyl1 = ARTCreateElement(mySim, "Cyl1", "Cylinder");
                Tunnel = ARTCreateElement(mySim, "ToneHoleCyl", "Cylinder");
                Cyl2 = ARTCreateElement(mySim, "Cyl2", "Cylinder");
                mainInst = ARTCreateCircuit(mySim, "mainInstrument");
                branch = ARTCreateCircuit(mySim, "branch");
                ARTSetFrequencyRange(mySim, 10, 200, 5);
                ARTSetNModes(mySim, 2);
        }

        virtual bool run()
        {
                try
                {
                        //add tonhole to circuit
                        ARTAppendReference( branch, Tunnel);

                        ARTAppendReference( mainInst, Cyl1);
                        ARTAppendReference( mainInst, branch);
                        ARTAppendReference( mainInst, Cyl2);
                        ARTvariant* meineImpKurve = ARTInputImpedance(mainInst);


                        //for (int i = 0; i < meineImpKurve->len; i++)
                        //{
                        //      std::cout << int( meineImpKurve->val->nt[i].f )<< "Hz: ";
                        //      std::cout << int( meineImpKurve->val->nt[i].re) << " " << int( meineImpKurve->val->nt[i].im)<< "\n";
                        //}


                        if (int(meineImpKurve->val->nt[0].f) != 10) return false;
                        if (int(meineImpKurve->val->nt[0].re) != 1142) return false;
                        if (int(meineImpKurve->val->nt[0].im) != 377127) return false;
                        if (int(meineImpKurve->val->nt[1].f) != 15) return false;
                        if (int(meineImpKurve->val->nt[1].re) != 3483) return false;
                        if (int(meineImpKurve->val->nt[1].im) != 588976) return false;

                }
                catch(ARTerror e)
                {
                        string err = e.GetErrorMessage();
                        std::cout << "\n\n" << err;
                        return false;
                }

                return true;
        }


        virtual void unprepare()
        {
                ARTDestroyCircuit(mySim, branch);
                ARTDestroyCircuit(mySim, mainInst);
                ARTDestroyElement(mySim,Cyl2);
                ARTDestroyElement(mySim,Tunnel);
                ARTDestroyElement(mySim,Cyl1);
                ARTDestroySimulator(mySim);
                ARTRootDestroy();
        }

TEST_DEF_END(testCircuitAsBranchImpedanceModes2)
//******************************************************************************************************************************************
#endif
TestClass* ARTtimeSimulatorTests = new TestClass("ARTtimeSimulatorTests", AllMyTests);
//#ifdef _TIMEDEBUG
TEST_DEF_START(DoublePortInitialization, ARTtimeSimulatorTests)

        TimeModule* timeModule;

        virtual void prepare()
        {
                timeModule = new TimeModule("myModule");
        }

        virtual bool run()
        {

                try
                {
                        timeModule->addOPort("fib", "fib[t] = fib[t-1] + fib[t-2]");
                        timeModule->addOPort("fib", "fib[t] = t");

                        return false;
                }
                catch (ARTerror& e)
                {
//                      string err = e.GetErrorMessage();
//                      std::cout << "\n\n" << err;
                        return true;
                }

        }

        virtual void unprepare()
        {
                delete timeModule;
        }

TEST_DEF_END(DoublePortInitialization)


TEST_DEF_START(DoubleModuleCreation, ARTtimeSimulatorTests)

        TimeSimulator* myTimeSimulator;
        TimeModule* timeModule1;
        TimeModule* timeModule2;


        virtual void prepare()
        {
                myTimeSimulator = new TimeSimulator("TestSim");
                myTimeSimulator->userElements = new ListProp("testList");
                timeModule1 = new TimeModule("myModule");
                timeModule2 = new TimeModule("myModule");
        }

        virtual bool run()
        {

                try
                {
                        myTimeSimulator->AddTimeModule(timeModule1);
                        myTimeSimulator->AddTimeModule(timeModule2);

                        return false;
                }
                catch (ARTerror& e)
                {
//                      string err = e.GetErrorMessage();
//                      std::cout << "\n\n" << err;
                        return true;
                }

        }

        virtual void unprepare()
        {
                delete (myTimeSimulator->userElements);
                delete myTimeSimulator;
        }

TEST_DEF_END(DoubleModuleCreation)

TEST_DEF_START(CreateAndChangeLocalParameter, ARTtimeSimulatorTests)

        TimeSimulator* timeSimulator;
        TimeModule* timeModule;

        virtual void prepare()
        {
                timeSimulator = new TimeSimulator("mySimulator");
                timeSimulator->userElements = new ListProp("testList");
                timeModule = new TimeModule("myTimeModule");
        }

        virtual bool run()
        {

                try
                {

                        timeSimulator->AddTimeModule(timeModule);

                        // setting a new global parameter should raise an error
                        timeModule->addLocalParameter("x", 2.0);
                        timeModule->addOPort("test", "test[t] = x * y");
                        timeModule->addLocalParameter("y", "y = 3.0");

                        ITimeModule::OPortType& outputPort = dynamic_cast<ITimeModule::OPortType&>(*(timeModule->getPort("test")));

                        timeSimulator->SimulateTimeStep(0);
                        if (outputPort[0].GetFloat() != 6.0)
                        {
                                return false;
                        }

                        timeModule->setLocalParameter("y", 0.5);
                        timeModule->setLocalParameter("x", "x = 44100");

                        timeSimulator->SimulateTimeStep(1);
                        if (outputPort[1].GetFloat() != 44100.0/2)
                        {
                                return false;
                        }

                        return true;
                }
                catch (ARTerror& e)
                {
                        string err = e.GetErrorMessage();
                        std::cout << "\n\n" << err;
                        return false;
                }

        }

        virtual void unprepare()
        {
                delete (timeSimulator->userElements);
                delete timeSimulator;
//              delete timeModule;
        }

TEST_DEF_END(CreateAndChangeLocalParameter)

TEST_DEF_START(ChangeGlobalParameter, ARTtimeSimulatorTests)

        TimeSimulator* myTimeSimulator;
        TimeModule* timeModule;

        virtual void prepare()
        {
                myTimeSimulator = new TimeSimulator("TestSim");
                myTimeSimulator->userElements = new ListProp("testList");
                timeModule = new TimeModule("myModule");
        }

        virtual bool run()
        {

                try
                {

                        timeModule->addOPort("test", "test[t] = T");
                        myTimeSimulator->AddTimeModule(timeModule);

                        ITimeModule::OPortType& testPort = dynamic_cast<ITimeModule::OPortType&>(*(timeModule->getPort("test")));

                        myTimeSimulator->SetSimulationParameter("T", 0.1);

                        myTimeSimulator->SimulateTimeStep(0);
                        if (testPort[0].GetFloat() != 0.1)
                        {
                                return false;
                        }

                        myTimeSimulator->SetSimulationParameter("T", 0.2);

                        myTimeSimulator->SimulateTimeStep(1);
                        if (testPort[1].GetFloat() != 0.2)
                        {
                                return false;
                        }

                        return true;
                }
                catch (ARTerror& e)
                {
                        string err = e.GetErrorMessage();
                        std::cout << "\n\n" << err;
                        return false;
                }


        }

        virtual void unprepare()
        {
                delete (myTimeSimulator->userElements);
                delete myTimeSimulator;
        }

TEST_DEF_END(ChangeGlobalParameter)


TEST_DEF_START(CreateGlobalParameter, ARTtimeSimulatorTests)

        TimeSimulator* myTimeSimulator;
        TimeModule* timeModule;

        virtual void prepare()
        {
                myTimeSimulator = new TimeSimulator("TestSim");
                myTimeSimulator->userElements = new ListProp("testList");
                timeModule = new TimeModule("myModule");
        }

        virtual bool run()
        {

                try
                {

                        timeModule->addOPort("test", "test[t] = TEMP");
                        myTimeSimulator->AddTimeModule(timeModule);

                        ITimeModule::OPortType& testPort = dynamic_cast<ITimeModule::OPortType&>(*(timeModule->getPort("test")));

                        myTimeSimulator->AddSimulationParameter("TEMP", 40.0);

                        myTimeSimulator->SimulateTimeStep(0);
                        if (testPort[0].GetFloat() != 40.0)
                        {
                                return false;
                        }

                        myTimeSimulator->SetSimulationParameter("TEMP", "TEMP = sqrt(2)");

                        myTimeSimulator->SimulateTimeStep(1);
                        if (testPort[1].GetFloat() != std::sqrt(std::complex<double>(2.0,0)).real())
                        {
                                return false;
                        }

                        // adding a new global parameter with same name should raise an error
                        try
                        {
                                myTimeSimulator->AddSimulationParameter("TEMP", 0.0);
                        }
                        catch (ARTerror& e)
                        {
                                return true;
                        }

                        return false;
                }
                catch (ARTerror& e)
                {
                        string err = e.GetErrorMessage();
                        std::cout << "\n\n" << err;
                        return false;
                }

        }

        virtual void unprepare()
        {
                delete (myTimeSimulator->userElements);
                delete myTimeSimulator;
        }

TEST_DEF_END(CreateGlobalParameter)

TEST_DEF_START(SetNonExistingGlobalParameter, ARTtimeSimulatorTests)

        TimeSimulator* myTimeSimulator;

        virtual void prepare()
        {
                myTimeSimulator = new TimeSimulator("TestSim");
        }

        virtual bool run()
        {

                try
                {

                        // setting a new global parameter should raise an error
                        myTimeSimulator->SetSimulationParameter("unknown", 0.0);

                        return false;
                }
                catch (ARTerror& e)
                {
//                      string err = e.GetErrorMessage();
//                      std::cout << "\n\n" << err;
                        return true;
                }

        }

        virtual void unprepare()
        {
//              delete (myTimeSimulator->userElements);
                delete myTimeSimulator;
        }

TEST_DEF_END(SetNonExistingGlobalParameter)

TEST_DEF_START(FibonacciNumbers1, ARTtimeSimulatorTests)

        TimeSimulator* myTimeSimulator;

        virtual void prepare()
        {
                myTimeSimulator = new TimeSimulator("TestSim");
                myTimeSimulator->userElements = new ListProp("testList");

        }

        virtual bool run()
        {

                try
                {

                        TimeModule* timeModule = new TimeModule("myModule");

                        timeModule->addOPort("fib", "fib[t] = fib[t-1] + fib[t-2]", 7);
                        timeModule->addOPort("test", "test[t] = fib[t]");

                        myTimeSimulator->AddTimeModule(timeModule);

                        ITimeModule::OPortType& fibPort = dynamic_cast<ITimeModule::OPortType&>(*(timeModule->getPort("fib")));
                        ITimeModule::OPortType& outputPort = dynamic_cast<ITimeModule::OPortType&>(*(timeModule->getPort("test")));

                        fibPort.initPortValue("fib[-1] = 1, fib[-2] = 0");

                        for (int i = 0; i < 50; ++i)
                        {
//                              outputPort.GetPortValue(i).GetFloat();
                                myTimeSimulator->SimulateTimeStep(i);
//                              cout << "Fib[" << i << "] = " << outputPort.GetPortValue(i).GetFloat() << endl;
                        }

                        // test 51st fibonacci number
                        if (outputPort[49].GetFloat() == (2.0*1597.0*6376021.0))
                        {
                                return true;
                        }
                        else
                        {
                                return false;
                        }
                }
                catch (ARTerror& e)
                {
                        string err = e.GetErrorMessage();
                        std::cout << "\n\n" << err;
                        return false;
                }

        }

        virtual void unprepare()
        {
                delete (myTimeSimulator->userElements);
                delete myTimeSimulator;
        }

TEST_DEF_END(FibonacciNumbers1)


TEST_DEF_START(FibonacciNumbers2, ARTtimeSimulatorTests)

        TimeSimulator* myTimeSimulator;

        virtual void prepare()
        {
                myTimeSimulator = new TimeSimulator("TestSim");
                myTimeSimulator->userElements = new ListProp("testList");

        }

        virtual bool run()
        {

                try
                {

                        TimeModule* timeModule = new TimeModule("myModule");
                        TimeModule* timeModule2 = new TimeModule("myModule2");

                        timeModule->addOPort("fib", "fib[t] = fib[t-1] + fib[t-2]");
//                      timeModule2->addOPort("test", "test[t] = (fib[t] + fib[t-1]) / fib[t] + 1");
                        timeModule2->addOPort("test", "test[t] = fib[t]");

                        myTimeSimulator->AddTimeModule(timeModule);
                        myTimeSimulator->AddTimeModule(timeModule2);

                        ITimeModule::OPortType& testPort = dynamic_cast<ITimeModule::OPortType&>(*(timeModule->getPort("fib")));
                        timeModule2->addIPort("fib",&testPort);

                        ITimeModule::OPortType& outputPort = dynamic_cast<ITimeModule::OPortType&>(*(timeModule2->getPort("test")));


                        testPort.initPortValue("fib[-1] = 1, fib[-2] = 0");

                        for (int i = 0; i < 50; ++i)
                        {
//                              std::cout << "Fibonacci[" << i << "] = " << outputPort[i].real() << std::endl;
                                myTimeSimulator->SimulateTimeStep(i);
//                              outputPort.GetPortValue(i).GetFloat();
                        }

                        // test 51st fibonacci number
                        if (outputPort[49].GetFloat() == (2.0*1597.0*6376021.0))
                        {
                                return true;
                        }
                        else
                        {
                                return false;
                        }
                }
                catch (ARTerror& e)
                {
                        string err = e.GetErrorMessage();
                        std::cout << "\n\n" << err;
                        return false;
                }

        }

        virtual void unprepare()
        {
                delete (myTimeSimulator->userElements);
                delete myTimeSimulator;
        }

TEST_DEF_END(FibonacciNumbers2)

TEST_DEF_START(FibonacciNumbers3, ARTtimeSimulatorTests)

        Simulator* myTimeSimulator;
        ITimeModule* timeModule;
        ITimeModule* timeModule2;

        virtual void prepare()
        {
                myTimeSimulator = ARTCreateSimulator("TestSim", "TimeDomain", "PlaneWave");
        }

        virtual bool run()
        {
                try
                {

                        timeModule = ARTCreateTModule(myTimeSimulator, "myModule", "TimeModule");
                        timeModule2 = ARTCreateTModule(myTimeSimulator, "myModule2", "TimeModule");

                        ARTAddOPortToTModule(timeModule, "fib", "fib[t] = fib[t-1] + fib[t-2]");
                        ARTAddOPortToTModule(timeModule2, "test", "test[t] = fib[t] * l * T");

                        ARTAddLocalParamToTModule(timeModule2, "l", "l = 0.0");

                        ARTSetParameter(myTimeSimulator, "myModule2.l = 2.0/T");

                        ARTConnectPorts(myTimeSimulator, "myModule2.fib = myModule.fib");

                        ARTSetParameter(myTimeSimulator, "myModule.fib[-1] = 1; myModule.fib[-2] = 0");

                        P_ART_DataProp testPort = ARTGetPortFromTModule(timeModule2, "test");

                        T_ART_Cmplx outVal;

                        for (int i = 0; i < 50; ++i)
                        {
                                outVal = ARTGetComplexFromPort(testPort, i);
//                              std::cout << "Fibonacci[" << i << "] = " << outVal.re << std::endl;
                        }

                        // test 51st fibonacci number
                        if (outVal.re == (2.0*2.0*1597.0*6376021.0))
                        {
                                return true;
                        }
                        else
                        {
                                return false;
                        }
                }
                catch (ARTerror& e)
                {
                        string err = e.GetErrorMessage();
                        std::cout << "\n\n" << err;
                        return false;
                }

        }

        virtual void unprepare()
        {
                ARTDestroyTModule(myTimeSimulator, timeModule);
                ARTDestroyTModule(myTimeSimulator, timeModule2);
                ARTDestroySimulator(myTimeSimulator);
                ARTRootDestroy();

        }

TEST_DEF_END(FibonacciNumbers3)

TEST_DEF_START(ConvolutionTest1, ARTtimeSimulatorTests)

        TimeSimulator* myTimeSimulator;

        virtual void prepare()
        {
                myTimeSimulator = new TimeSimulator("TestSim");
                myTimeSimulator->userElements = new ListProp("testList");

        }

        virtual bool run()
        {

                try
                {

                        TimeModule* timeModule = new TimeModule("myModule");

                        timeModule->addOPort("p", "p[t] = conv(U,G,t)");
                        timeModule->addOPort("U", "U[t] = sin(2*pi*30*t*T)");
                        timeModule->addOPort("G", "G[t] = (t == 0) ? 1 : 0");
                        ITimeModule::OPortType& pPort = dynamic_cast<ITimeModule::OPortType&>(*(timeModule->getPort("p")));
//                      ITimeModule::OPortType* gPort = dynamic_cast<ITimeModule::OPortType*>(timeModule->getPort("G"));
//                      ITimeModule::OPortType* uPort = dynamic_cast<ITimeModule::OPortType*>(timeModule->getPort("U"));

                        myTimeSimulator->AddTimeModule(timeModule);

                        for (int i = 0; i < 50; ++i)
                        {
                                myTimeSimulator->SimulateTimeStep(i);
//                              pPort->GetPortValue(i).GetFloat();
//                              std::cout << "p[" << i << "] = " << pPort->GetPortValue(i).GetFloat() << std::endl;
//                              std::cout << "G[" << i << "] = " << gPort->GetPortValue(i).GetFloat() << std::endl;
//                              std::cout << "U[" << i << "] = " << uPort->GetPortValue(i).GetFloat() << std::endl;
//                              std::cout << "p[" << i << "] = " << pPort->GetPortValue(i).GetFloat() << std::endl;
                        }

                        if (std::abs(pPort[49].GetFloat() - (std::sin(2.0 * PI * 30.0 * 49.0 / 44100))) < 1.0e-7)
//                      if (std::abs((pPort->GetPortValue(3).GetFloat() - (std::sin(2.0 * PI * 30.0 * 3.0 / 44100)))) < 1.0e-7)
                        {
                                return true;
                        }
                        else
                        {
                                std::cout << "\n\nsin(2*pi*30*15/44100) = " << pPort[49].GetFloat() << " != " << std::sin(2.0 * PI * 30.0 * 15.0 / 44100) << std::endl;
//                              std::cout << "sin(2*pi*30*3/44100) = " << pPort->GetPortValue(3).GetFloat() << " != " << std::sin(2.0 * PI * 30.0 * 3.0 / 44100) << std::endl;
                                return false;
                        }
                        return true;
                }
                catch (ARTerror& e)
                {
                        string err = e.GetErrorMessage();
                        std::cout << "\n\n" << err;
                        return false;
                }

        }

        virtual void unprepare()
        {
                delete (myTimeSimulator->userElements);
                delete myTimeSimulator;
        }

TEST_DEF_END(ConvolutionTest1)

//TEST_DEF_START(ConvolutionTest2, ARTtimeSimulatorTests)
//
//      ARTtimeSimulator* myTimeSimulator;
//
//      virtual void prepare()
//      {
//              myTimeSimulator = new ARTtimeSimulator("TestSim");
//              myTimeSimulator->userElements = new ListProp("testList");
//
//      }
//
//      virtual bool run()
//      {
//
//              try
//              {
//
//                      TimeModule* timeModule = new TimeModule("lip_module");
//
////                    timeModule->addOPort("x", "x[t] = (-sqrt(k/m) * (1/Q) * ((x[t-1] - x[t-2])/T) + ((b*d)/m)* p_lip[t] - (k/m)*x[t-2])*T^2 - x[t-2] + 2*x[t-1] ");
////                    timeModule->addOPort("S_lip", "S_lip[t] = max(b*(2*x[t] + x_0), 0.00001)", 1);
////                    timeModule->addOPort("U", "U[t] = ((p_0 - p[t-1])*(S_lip[t]/rho) + ((U[t - 1])^2) * (-1/(2*S_lip[t]) + 1/S_cup + S_lip[t]/(S_cup^2)) ) * (T/d) + U[t-1]");
////                    timeModule->addOPort("p", "p[t] = conv(U, G, t)");
////                    timeModule->addOPort("p_lip", "p_lip[t] = -rho * U[t]^2 * (1/(S_cup * S_lip[t]) - 1/(S_cup^2)) + p[t]", 1);
////                    timeModule->addOPort("G", "G[t] = 0.005", 20);
//
//                      timeModule->addOPort("x", "x[t] = (-sqrt(k/m) * (1/Q) * ((x[t-1] - x[t-2])/T) + ((b*d)/m)* p_lip[t-1] - (k/m)*x[t-2])*T^2 - x[t-2] + 2*x[t-1]", 3);
//                      timeModule->addOPort("S_lip", "S_lip[t] = max(b*(2*x[t] + x_0), 0.00001)", 1);
//                      timeModule->addOPort("p_lip", "p_lip[t] = -rho * U[t]^2 * (1/(S_cup * S_lip[t]) - 1/(S_cup^2)) + p[t]", 1);
//                      timeModule->addOPort("U", "U[t] = ((p_0 - p[t-1])*(S_lip[t-1]/rho) + ((U[t-1])^2) * (-1/(2*S_lip[t-1]) + 1/S_cup + S_lip[t-1]/(S_cup^2)) ) * (T/d) + U[t-1]", 5);
////                    timeModule->addOPort("U", "U[t] = S_lip[t-1]", 5);
//                      timeModule->addOPort("p", "p[t] = conv(U, G, t)", 5);
//                      timeModule->addOPort("G", "G[t] = 0.005", 20);
////
//                      ITimeModule::OPortType* pPort = dynamic_cast<ITimeModule::OPortType*>(timeModule->getPort("p"));
//                      ITimeModule::OPortType* xPort = dynamic_cast<ITimeModule::OPortType*>(timeModule->getPort("x"));
////                    ITimeModule::OPortType* gPort = dynamic_cast<ITimeModule::OPortType*>(timeModule->getPort("G"));
//                      ITimeModule::OPortType* uPort = dynamic_cast<ITimeModule::OPortType*>(timeModule->getPort("U"));
//                      xPort->initPortValue("x[-1] = 0.00002, x[-2] = 0.00001");
//                      uPort->initPortValue(0, 0);
////                    uPort->SetVal(0.0, 0);
////                    std::cerr << "HERE" << std::endl;
//                      pPort->SetVal(0.0, 0);
////                    std::cerr << "got it!" << std::endl;
//
////                    ITimeModule::OPortType* xPort = dynamic_cast<ITimeModule::OPortType*>(timeModule->getPort("x"));
//                      xPort->initPortValue("x[-1] = 0.00002, x[-2] = 0.00001");
//
//
////                    for (int i = 0; i < 20; ++i)
////                    {
////                            gPort->SetVal(0.005, i);
////                    }
//
//                      myTimeSimulator->AddTimeModule(timeModule);
//
//                      // speed of sound
//                      timeModule->addLocalParameter("c", 3.4E2);
//                      // average air density
//                      timeModule->addLocalParameter("rho", 1.2);
//                      // area of mouthpiece entryway
//                      timeModule->addLocalParameter("S_cup", 2.3E-3);
//                      // breadth of lip orifice
//                      timeModule->addLocalParameter("b", 8.0E-3);
//                      // thickness of lips
//                      timeModule->addLocalParameter("d", 2.0E-3);
//                      // equilibrium lip opening length
//                      timeModule->addLocalParameter("x_0", 2.1E-4);
//                      // lip quality factor
//                      timeModule->addLocalParameter("Q", 5.0);
//                      // lip resonance frequency
//                      timeModule->addLocalParameter("f_lip", 100);
//                      // lip mass define 1.5/(((2*PI)^2)/f_lip
//                      timeModule->addLocalParameter("m", 3.799544386E-4);
//                      // stiffness of lips
//                      timeModule->addLocalParameter("k", 1.5E2);
//                      // blowing pressure
//                      timeModule->addLocalParameter("p_0", 1.0);
//
//
////                    for (int i = 0; i < 44100*10; ++i)
//                      for (int i = 1; i < 100; ++i)
//                      {
////                            pPort->GetPortValue(i).GetFloat();
//                              std::cout << "p[" << i << "] = " << pPort->GetPortValue(i).GetFloat() << std::endl;
//                      }
//                      return true;
//              }
//              catch (ARTerror& e)
//              {
//                      string err = e.GetErrorMessage();
//                      std::cout << "\n\n" << err;
//                      return false;
//              }
//
//      }
//
//      virtual void unprepare()
//      {
//              delete (myTimeSimulator->userElements);
//              delete myTimeSimulator;
//      }
//
//TEST_DEF_END(ConvolutionTest2)

//TEST_DEF_START(ConvolutionTest3, ARTtimeSimulatorTests)
//
//      ARTtimeSimulator* myTimeSimulator;
//
//      virtual void prepare()
//      {
//              myTimeSimulator = new ARTtimeSimulator("TestSim");
//              myTimeSimulator->userElements = new ListProp("testList");
//
//      }
//
//      virtual bool run()
//      {
//
//              try
//              {
//                      int i;
//
//                      ITimeModule* timeModule = new inputFunctionModule("myModule");
//                      TimeModule* timeModule2 = new TimeModule("ConvolutionModule");
//
//                      dynamic_cast<inputFunctionModule*>(timeModule)->defineOPort(30, "");
//
//                      ITimeModule::FPortType* functionOutPort = dynamic_cast<ITimeModule::FPortType*>(timeModule->getPort("out"));
//
//                      for (i = 0; i < 30; ++i)
//                      {
//                              functionOutPort->initPortValue(0.5, i);
//                      }
//
//                      timeModule2->addOPort("local", "local[t] = t");
//                      timeModule2->addOPort("testout", "testout[t] = conv(local, function, t)");
//                      timeModule2->addIPort("function", functionOutPort);
//
//                      ITimeModule::OPortType& simulationOutPort = dynamic_cast<ITimeModule::OPortType&>(*(timeModule2->getPort("testout")));
//
//                      myTimeSimulator->AddTimeModule(timeModule);
//                      myTimeSimulator->AddTimeModule(timeModule2);
//
//                      for (i = 0; i < 100; ++i)
//                      {
////                            cout << "Conv[" << i << "] = " << simulationOutPort->GetPortValue(i).GetFloat() << endl;
////                            simulationOutPort->GetPortValue(i).GetFloat();
//                              myTimeSimulator->SimulateTimeStep(i);
//                      }
//
//                      if (simulationOutPort[99].GetFloat() != 0.25*(99.0 - 70.0 + 1.0)*(99.0 + 70.0))
//                      {
//                              return false;
//                      }
//
//                      return true;
//              }
//              catch (ARTerror& e)
//              {
//                      string err = e.GetErrorMessage();
//                      std::cout << "\n\n" << err;
//                      return false;
//              }
//
//      }
//
//      virtual void unprepare()
//      {
//              delete (myTimeSimulator->userElements);
//              delete myTimeSimulator;
//      }
//
//TEST_DEF_END(ConvolutionTest3)

TEST_DEF_START(HiddenTimeDelay, ARTtimeSimulatorTests)

        TimeSimulator* myTimeSimulator;

        virtual void prepare()
        {
                myTimeSimulator = new TimeSimulator("TestSim");
                myTimeSimulator->userElements = new ListProp("testList");

        }

        virtual bool run()
        {

                try
                {

                        TimeModule* timeModule = new TimeModule("myModule");
                        TimeModule* timeModule2 = new TimeModule("myModule2");

                        timeModule->addOPort("x", "x[t] = y[t-1]");
                        timeModule2->addOPort("y", "y[t] = x[t] + 2");
//                      timeModule->addOPort("x", "x[t] = (t == 0) ? 9 : 0");
//                      timeModule2->addOPort("y", "y[t] = x[t - ((round) (d/T))]");

                        ITimeModule::OPortType& xPort = dynamic_cast<ITimeModule::OPortType&>(*(timeModule->getPort("x")));
                        ITimeModule::OPortType& yPort = dynamic_cast<ITimeModule::OPortType&>(*(timeModule2->getPort("y")));

                        timeModule->addIPort("y", &yPort);
                        timeModule2->addIPort("x", &xPort);

                        myTimeSimulator->AddTimeModule(timeModule);
                        myTimeSimulator->AddTimeModule(timeModule2);

                        myTimeSimulator->SetSimulationParameter("T", 0.1);

                        timeModule2->addLocalParameter("d", 0.45);

                        yPort.initPortValue(-2, -1);

                        for (int i = 0; i < 100; ++i)
                        {
                                myTimeSimulator->SimulateTimeStep(i);
//                              cout << "x[" << i << "] = " << xPort[i].GetFloat() << endl;
//                              cout << "y[" << i << "] = " << yPort[i].GetFloat() << endl;
                        }

                        if (xPort[99].GetFloat() != 98.0*2.0)
                        {
//                          cout << "x[" << 99 << "] = " << xPort[99].GetFloat() << endl;
                                return false;
                        }

                        return true;
                }
                catch (ARTerror& e)
                {
                        string err = e.GetErrorMessage();
                        std::cout << "\n\n" << err;
                        return false;
                }

        }

        virtual void unprepare()
        {
                delete (myTimeSimulator->userElements);
                delete myTimeSimulator;
        }

TEST_DEF_END(HiddenTimeDelay)

TEST_DEF_START(RectangularFunction, ARTtimeSimulatorTests)

        TimeSimulator* myTimeSimulator;

        virtual void prepare()
        {
                myTimeSimulator = new TimeSimulator("TestSim");
                myTimeSimulator->userElements = new ListProp("testList");

        }

        virtual bool run()
        {

                try
                {

                        TimeModule* timeModule = new TimeModule("myModule");

                        timeModule->addOPort("x", "x[t] = (((t*T) >= S)  and ((t*T) <= E)) ? 1 : 0");

                        ITimeModule::OPortType& xPort = dynamic_cast<ITimeModule::OPortType&>(*(timeModule->getPort("x")));

                        myTimeSimulator->AddTimeModule(timeModule);

                        timeModule->addLocalParameter("S", 0.3);
                        timeModule->addLocalParameter("E", 2.7);

                        myTimeSimulator->SetSimulationParameter("T", 0.1);

                        for (int i = 0; i < 100; ++i)
                        {
                                myTimeSimulator->SimulateTimeStep(i);
                                if (i < 3 && xPort[i].GetFloat() != 0)
                                {
                                        cout << "x[" << i << "] = " << xPort[i].GetFloat() << " != 0" << endl;
                                        return false;
                                }
                                else if (i > 3 && i <= 27 && xPort[i].GetFloat() != 1)
                                {
                                        cout << "x[" << i << "] = " << xPort[i].GetFloat() << " != 1" << endl;
                                        return false;
                                }
                                else if (i > 27 && xPort[i].GetFloat() != 0)
                                {
                                        cout << "x[" << i << "] = " << xPort[i].GetFloat() << " != 0" << endl;
                                        return false;
                                }
                        }

                        return true;
                }
                catch (ARTerror& e)
                {
                        string err = e.GetErrorMessage();
                        std::cout << "\n\n" << err;
                        return false;
                }

        }

        virtual void unprepare()
        {
                delete (myTimeSimulator->userElements);
                delete myTimeSimulator;
        }

TEST_DEF_END(RectangularFunction)

TEST_DEF_START(SineFunction, ARTtimeSimulatorTests)

        TimeSimulator* myTimeSimulator;

        virtual void prepare()
        {
                myTimeSimulator = new TimeSimulator("TestSim");
                myTimeSimulator->userElements = new ListProp("testList");

        }

        virtual bool run()
        {

                try
                {

                        TimeModule* timeModule = new TimeModule("myModule");

                        timeModule->addOPort("x", "x[t] = A*sin(2*pi*(t*T*f - Delta))");
//                      timeModule->addOPort("x", "x[t] = A*sin(t)");

                        ITimeModule::OPortType& xPort = dynamic_cast<ITimeModule::OPortType&>(*(timeModule->getPort("x")));

                        myTimeSimulator->AddTimeModule(timeModule);

                        timeModule->addLocalParameter("f", 11025.0);
                        timeModule->addLocalParameter("Delta", 0.25);
                        timeModule->addLocalParameter("A", 2.3);

//                      myTimeSimulator->SetSimulationParameter("T", 0.1);

                        for (int i = 0; i < 100; ++i)
                        {
                                myTimeSimulator->SimulateTimeStep(i);
//                              cout << "x[" << i << "] = " << xPort[i].GetFloat() << endl;
                        }

                        if (std::abs(xPort[99].GetFloat() - 2.3*std::sin(2*PI*(99.0/44100.0*11025.0 - 0.25))) > 1e-6)
                        {
                                cout << "\n\nx[" << 99 << "] = " << xPort[99].GetFloat() << " != " << std::sin(2*PI*(99.0/44100.0*11025.0 - 0.25)) << endl;
                                return false;
                        }

                        return true;
                }
                catch (ARTerror& e)
                {
                        string err = e.GetErrorMessage();
                        std::cout << "\n\n" << err;
                        return false;
                }

        }

        virtual void unprepare()
        {
                delete (myTimeSimulator->userElements);
                delete myTimeSimulator;
        }

TEST_DEF_END(SineFunction)

TEST_DEF_START(FractionalDelay, ARTtimeSimulatorTests)

        TimeSimulator* myTimeSimulator;

        virtual void prepare()
        {
                myTimeSimulator = new TimeSimulator("TestSim");
                myTimeSimulator->userElements = new ListProp("testList");

        }

        virtual bool run()
        {

                try
                {

                        TimeModule* pulseModule = new TimeModule("pulseModule");
                        ITimeModule* delayModule1 = new fractionalDelayModule("testDelay1");
                        ITimeModule* delayModule2 = new fractionalDelayModule("testDelay2");

                        pulseModule->addOPort("x", "x[t] = (t == 0) ? 1 : 0");
//                      pulseModule->addOPort("x", "x[t] = A*sin(t)");

                        ITimeModule::OPortType& xPort = dynamic_cast<ITimeModule::OPortType&>(*(pulseModule->getPort("x")));
                        ITimeModule::OPortType& simulationPort1 = dynamic_cast<ITimeModule::OPortType&>(*(delayModule1->getPort("out")));
                        ITimeModule::OPortType& simulationPort2 = dynamic_cast<ITimeModule::OPortType&>(*(delayModule2->getPort("out")));

                        delayModule1->addIPort("in", &xPort);
                        delayModule2->addIPort("in", &xPort);

                        myTimeSimulator->AddTimeModule(pulseModule);
                        myTimeSimulator->AddTimeModule(delayModule1);
                        myTimeSimulator->AddTimeModule(delayModule2);

                        myTimeSimulator->SetSimulationParameter("T", 0.1);

                        delayModule1->setLocalParameter("type", "thiran");
                        delayModule1->setLocalParameter("Delay", 0.51);
//                      delayModule1->setLocalParameter("Delay", "Delay = 0.3");
                        delayModule1->setLocalParameter("order", 5);
//                      delayModule1->setLocalParameter("order", 3);

                        delayModule2->setLocalParameter("type", "lagrange");
                        delayModule2->setLocalParameter("Delay", 0.51);
//                      delayModule2->setLocalParameter("Delay", "Delay = 0.9");
                        delayModule2->setLocalParameter("order", 12);

                        for (int i = 0; i < 100; ++i)
                        {
                                myTimeSimulator->SimulateTimeStep(i);
//                              cout << "out1[" << i << "] = " << simulationPort1[i].GetFloat() << endl;
//                              cout << "out2[" << i << "] = " << simulationPort2[i].GetFloat() << endl;
                                if (i == 5)
                                {
                                        if (std::abs(simulationPort1[i].GetFloat() - 1.0) > 0.1)
                                        {
                                                return false;
                                        }
                                        if (std::abs(simulationPort2[i].GetFloat() - 1.0) > 0.1)
                                        {
                                                return false;
                                        }
                                }
                        }

                        if ((std::abs(simulationPort1[99].GetFloat()) > 1e-10) || (simulationPort2[99].GetFloat() != 0))
                        {
                                return false;
                        }

                        return true;
                }
                catch (ARTerror& e)
                {
                        string err = e.GetErrorMessage();
                        std::cout << "\n\n" << err;
                        return false;
                }

        }

        virtual void unprepare()
        {
                delete (myTimeSimulator->userElements);
                delete myTimeSimulator;
        }

TEST_DEF_END(FractionalDelay)

TEST_DEF_START(FindNames, ARTtimeSimulatorTests)

        AcousticResearchTool* ART;
        TimeSimulator* myTimeSimulator1;
        TimeSimulator* myTimeSimulator2;
        TimeModule* timeModule1;
//        TimeModule* timeModule2;


        virtual void prepare()
        {
//                ART = new AcousticResearchTool();
//                myTimeSimulator1 = new ARTtimeSimulator("TestSim2");
//                myTimeSimulator2 = new ARTtimeSimulator("TestSim2");
//                myTimeSimulator1->userElements = new ListProp("testList");
//                timeModule1 = new TimeModule("myModule");
        }

        virtual bool run()
        {

                try
                {
//                        std::cout << "# of simulators: " << ART->simulators->Size() << std::endl;



//                        ART->simulators->AppendObject(myTimeSimulator1);
//                        myTimeSimulator->AddTimeModule(timeModule1);

//                        std::cout << "# of simulators: " << ART->simulators->Size() << std::endl;

//                        ART->simulators->AppendObject(myTimeSimulator2);

//                        std::cout << "# of simulators: " << ART->simulators->Size() << std::endl;

                        ARTCreateSimulator("myTimeSimulator1", "TimeDomain", "");

                        return true;
                }
                catch (ARTerror& e)
                {
                      string err = e.GetErrorMessage();
                      std::cout << "\n\n" << err;
                        return false;
                }

        }

        virtual void unprepare()
        {
//                delete (myTimeSimulator1->userElements);
                //delete myTimeSimulator;
//                delete ART;
                 ARTRootDestroy();
        }

TEST_DEF_END(FindNames)


//TEST_DEF_START(DWGCylinder, ARTtimeSimulatorTests)
//
//      ARTtimeSimulator* myTimeSimulator;
//
//      virtual void prepare()
//      {
//              myTimeSimulator = new ARTtimeSimulator("TestSim");
//              myTimeSimulator->userElements = new ListProp("testList");
//
//      }
//
//      virtual bool run()
//      {
//
//              try
//              {
//
//                      TimeModule* pulseModule = new TimeModule("pulseModule");
//                      ITimeModule* cylModule1 = new DWGcylinderModule("cyl1");
////                    ITimeModule* cylModule1 = new DWGconeJunctionModule("cyl1");
//
//                      pulseModule->addOPort("x", "x[t] = (t == 0) ? 1 : 0", 50);
//
//                      ITimeModule::OPortType& xPort = dynamic_cast<ITimeModule::OPortType&>(*(pulseModule->getPort("x")));
//                      ITimeModule::OPortType& simulationPort1 = dynamic_cast<ITimeModule::OPortType&>(*(cylModule1->getPort("p2p")));
//                      ITimeModule::OPortType& simulationPort2 = dynamic_cast<ITimeModule::OPortType&>(*(cylModule1->getPort("p1m")));
//
//                      cylModule1->addIPort("p1p", &xPort);
//                      cylModule1->addIPort("p2m", &xPort);
//
//                      myTimeSimulator->AddTimeModule(pulseModule);
//                      myTimeSimulator->AddTimeModule(cylModule1);
//                      myTimeSimulator->AddSimulationParameter("c", 343.36);
//
//                      myTimeSimulator->SetSimulationParameter("T", 1.0/44100.0);
//
//                      cylModule1->setLocalParameter("type", "thiran");
//                      cylModule1->setLocalParameter("length", 320);
////                    cylModule1->setLocalParameter("r2", 70);
//
//                      for (int i = 0; i < 100; ++i)
//                      {
//                              cout << "BLA" << endl;
//                              myTimeSimulator->SimulateTimeStep(i);
//                              if (simulationPort1[i].GetFloat() != simulationPort2[i].GetFloat())
//                              {
//                                      return false;
//                              }
//                              if (i == 41 && simulationPort1[i].GetFloat() < 0.95)
//                              {
//                                      return false;
//                              }
////                            cout << "p2p[" << i << "] = " << simulationPort1[i].GetFloat() << endl;
////                            cout << "p1m[" << i << "] = " << simulationPort2[i].GetFloat() << endl;
//                      }
//
//                      if (std::abs(simulationPort1[99].GetFloat()) > 1e-10)
//                      {
//                              return false;
//                      }
//
//                      return true;
//              }
//              catch (ARTerror& e)
//              {
//                      string err = e.GetErrorMessage();
//                      std::cout << "\n\n" << err;
//                      return false;
//              }
//
//      }
//
//      virtual void unprepare()
//      {
//              delete (myTimeSimulator->userElements);
//              delete myTimeSimulator;
//      }
//
//TEST_DEF_END(DWGCylinder)
//#endif

//TEST_DEF_START(ResizeTest, ARTtimeSimulatorTests)
//
//      ARTsimulator* myTimeSimulator;
//
//      virtual void prepare()
//      {
//              myTimeSimulator = ARTCreateSimulator("TestSim", "TimeDomain", "");
//      }
//
//      virtual bool run()
//      {
//
//              try
//              {
//                      int i;
//
//                      ITimeModule* impulseModule = ARTCreateTModule(myTimeSimulator, "Impulse", "ImpulseModule");
//                      ITimeModule* rightCylinder = ARTCreateTModule(myTimeSimulator, "RightCylinder", "DWGCylinderModule");
//                      ITimeModule* gain2 = ARTCreateTModule(myTimeSimulator, "Gain2", "AmplificationModule");
//                      std::stringstream tmp;
//
////                    ARTAddOPortToTModule(resizeModule, "x", "x[t] = t + x[t-35]");
//
//                      ARTAddGlobalParamToTSimulator(myTimeSimulator, "c", "c = 331");
//
//                      ARTConnectPorts(myTimeSimulator, "Gain2.in = RightCylinder.p2p; RightCylinder.p2m = Gain2.out");
//                      ARTConnectPorts(myTimeSimulator, "RightCylinder.p1p = Impulse.out");
//                      ARTSetParameter(myTimeSimulator, "RightCylinder.length = 102; RightCylinder.type = 'thiran'");
//                      ARTSetParameter(myTimeSimulator, "Gain2.A = -1");
//
//                      P_ART_DataProp outPort = ARTGetPortFromTModule(rightCylinder, "p1m");
//
////                    ITimeModule::OPortType& xPort = dynamic_cast<ITimeModule::OPortType&>(*(resizeModule->getPort("x")));
//
//
//
//                      for (i = -1; i >= -27; --i)
//                      {
//                              std::stringstream tmp;
//                              tmp << "RightCylinder.p1m[" << i << "] = 0;";
//                              tmp << "Impulse.out[" << i << "] = 0;";
//                              tmp << "Gain2.out[" << i << "] = 0;";
//                              ARTSetParameter(myTimeSimulator, tmp.str().c_str());
//                      }
//
//                      for (i = 0; i < 100; ++i)
//                      {
////                            myTimeSimulator->SimulateTimeStep(i);
//                              T_ART_Cmplx outVal = ARTGetComplexFromPort(outPort, i);
//                              cout << "x[" << i << "] = " << outVal.re << endl;
//                      }
//
//                      return true;
//              }
//              catch (ARTerror& e)
//              {
//                      string err = e.GetErrorMessage();
//                      std::cout << "\n\n" << err;
//                      return false;
//              }
//
//      }
//
//      virtual void unprepare()
//      {
////            delete (myTimeSimulator->userElements);
////            delete myTimeSimulator;
//              ARTDestroySimulator(myTimeSimulator);
//      }
//
//TEST_DEF_END(ResizeTest)

//******************************************************************************************************************************************


obinary   binary_out;

bool percent(double p, const char* msg)
{
        static int pr;
        int     i = p*100;
        if (pr != i) pr = i;
//      cout << int(p*100) << "%" << "\r";
        std::cout  << msg << " " << pr << "%                           " << "\r";
        return true;
}

