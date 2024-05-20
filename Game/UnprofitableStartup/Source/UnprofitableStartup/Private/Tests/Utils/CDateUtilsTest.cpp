// Alvaro Antonio Chavez Mixco (w18367131), 6CCGD011W Computer Games Development Final Project 


#include "Tests/Utils/CDateUtilsTest.h"

#include "Utils/CDateUtils.h"
#include "Tests/TestingConstants.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(AdvanceMonthTest_1, "UnprofitableStartup.UnitTesting.CDateUtils.AdvanceMonth_1", S_TEST_FLAGS)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(AdvanceMonthTest_2, "UnprofitableStartup.UnitTesting.CDateUtils.AdvanceMonth_2", S_TEST_FLAGS)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(AdvanceMonthTest_3, "UnprofitableStartup.UnitTesting.CDateUtils.AdvanceMonth_3", S_TEST_FLAGS)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(AdvanceMonthTest_4, "UnprofitableStartup.UnitTesting.CDateUtils.AdvanceMonth_4", S_TEST_FLAGS)


#pragma region AdvanceMonthTest
// Standard case
bool AdvanceMonthTest_1::RunTest(const FString & Parameters)
{
    FDateTime initialDate = FDateTime(2000, 01, 01);
    FDateTime expectedResult = FDateTime(2000, 02, 01);

    FDateTime result = UCDateUtils::AdvanceMonth(initialDate);

    return TestEqual(TEXT("Expected result of 2000-02-01"), result, expectedResult);
}

// Case with middle day of month
bool AdvanceMonthTest_2::RunTest(const FString& Parameters)
{
    FDateTime initialDate = FDateTime(2000, 01, 15);
    FDateTime expectedResult = FDateTime(2000, 02, 15);

    FDateTime result = UCDateUtils::AdvanceMonth(initialDate);

    return TestEqual(TEXT("Expected result of 2000-02-15"), result, expectedResult);
}

// Case for end of year, reset month and go to next year
bool AdvanceMonthTest_3::RunTest(const FString& Parameters)
{
    FDateTime initialDate = FDateTime(2000, 12, 01);
    FDateTime expectedResult = FDateTime(2001, 01, 01);

    FDateTime result = UCDateUtils::AdvanceMonth(initialDate);

    return TestEqual(TEXT("Expected result of 2001-01-01"), result, expectedResult);
}

// Case if the day of the month does not exist in the next month
bool AdvanceMonthTest_4::RunTest(const FString& Parameters)
{
    FDateTime initialDate = FDateTime(2004, 01, 31);
    FDateTime expectedResult = FDateTime(2004, 02, 29);

    FDateTime result = UCDateUtils::AdvanceMonth(initialDate);

    return TestEqual(TEXT("Expected result of 2004-02-29"), result, expectedResult);
}
#pragma endregion AdvanceMonthTest