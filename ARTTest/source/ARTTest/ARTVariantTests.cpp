/*
 * ARTVariantTests.cpp
 *
 *  Created on: 13 Jun 2016
 *      Author: clgeyer
 */


#include "ARTTest/ARTVariantTests.h"

using ::testing::AtLeast;
using ::testing::Return;
using namespace ART;
// This test should pass
TEST(ARTVariantChecks, SetVal1)
{
  MockARTvariant variable;

  EXPECT_CALL(variable, SetVal(testing::Matcher<int>(2), testing::Matcher<int>(0)));
  EXPECT_CALL(variable, IsEqual(static_cast<ARTvariant*>(0)));

  // should pass
  variable.SetVal(2,0);
  variable.IsEqual(static_cast<ARTvariant*>(0));
}

TEST(ARTCellChecks, CellChecks1)
{
  Cell cell("OldName", "Short Description", "Long Description",
                   "Helpfile Name");

  EXPECT_EQ("OldName", cell.GetName());

  cell.SetName("NewName");

  EXPECT_EQ("NewName", cell.GetName());

}

