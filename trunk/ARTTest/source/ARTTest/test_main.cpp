#include "ARTTest/test_main.h"

#include "ARTTest/integrationTests.h"

#include "gmock.h"
#include "gtest.h"


int main(int argc, char **argv)
{
  ::testing::InitGoogleMock(&argc, argv);

  try
  {

    // run all implementation tests
    bool success = AllMyTests->runTests();

    if (success)
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
  catch(ARTerror e)
  {
    string err = e.GetErrorMessage();
    std::cout << "\n\n" << err;
  }

  // run all gmock unit tests
  return RUN_ALL_TESTS();
}

