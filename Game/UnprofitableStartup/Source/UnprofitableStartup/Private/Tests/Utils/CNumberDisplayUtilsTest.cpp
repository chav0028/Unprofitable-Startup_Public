// Alvaro Antonio Chavez Mixco (w18367131), 6CCGD011W Computer Games Development Final Project 


#include "Tests/Utils/CNumberDisplayUtilsTest.h"

#include "Utils/CNumberDisplayUtils.h"
#include "Tests/TestingConstants.h"

// Possible step sizes
static constexpr float S_MILLION = 1000000.0f;
static constexpr float S_HUNDRED_THOUSAND = 100000.0f;
static constexpr float S_TEN_THOUSAND = 10000.0f;
static constexpr float S_THOUSAND = 1000.0f;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TruncateDisplayNumber_1, "UnprofitableStartup.UnitTesting.CNumberDisplayUtils.TruncateDisplayNumber_1", S_TEST_FLAGS)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(TruncateDisplayNumber_2, "UnprofitableStartup.UnitTesting.CNumberDisplayUtils.TruncateDisplayNumber_2", S_TEST_FLAGS)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(TruncateDisplayNumber_3, "UnprofitableStartup.UnitTesting.CNumberDisplayUtils.TruncateDisplayNumber_3", S_TEST_FLAGS)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(TruncateDisplayNumber_4, "UnprofitableStartup.UnitTesting.CNumberDisplayUtils.TruncateDisplayNumber_4", S_TEST_FLAGS)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(TruncateDisplayNumber_5, "UnprofitableStartup.UnitTesting.CNumberDisplayUtils.TruncateDisplayNumber_5", S_TEST_FLAGS)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(TruncateDisplayNumber_6, "UnprofitableStartup.UnitTesting.CNumberDisplayUtils.TruncateDisplayNumber_6", S_TEST_FLAGS)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(TruncateDisplayNumber_7, "UnprofitableStartup.UnitTesting.CNumberDisplayUtils.TruncateDisplayNumber_7", S_TEST_FLAGS)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(TruncateDisplayNumber_8, "UnprofitableStartup.UnitTesting.CNumberDisplayUtils.TruncateDisplayNumber_8", S_TEST_FLAGS)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(TruncateDisplayNumber_9, "UnprofitableStartup.UnitTesting.CNumberDisplayUtils.TruncateDisplayNumber_9", S_TEST_FLAGS)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(TruncateDisplayNumber_10, "UnprofitableStartup.UnitTesting.CNumberDisplayUtils.TruncateDisplayNumber_10", S_TEST_FLAGS)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GetStepSize_1, "UnprofitableStartup.UnitTesting.CNumberDisplayUtils.GetStepSize_1", S_TEST_FLAGS)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(GetStepSize_2, "UnprofitableStartup.UnitTesting.CNumberDisplayUtils.GetStepSize_2", S_TEST_FLAGS)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(GetStepSize_3, "UnprofitableStartup.UnitTesting.CNumberDisplayUtils.GetStepSize_3", S_TEST_FLAGS)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(GetStepSize_4, "UnprofitableStartup.UnitTesting.CNumberDisplayUtils.GetStepSize_4", S_TEST_FLAGS)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(GetStepSize_5, "UnprofitableStartup.UnitTesting.CNumberDisplayUtils.GetStepSize_5", S_TEST_FLAGS)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(GetStepSize_6, "UnprofitableStartup.UnitTesting.CNumberDisplayUtils.GetStepSize_6", S_TEST_FLAGS)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(GetStepSize_7, "UnprofitableStartup.UnitTesting.CNumberDisplayUtils.GetStepSize_7", S_TEST_FLAGS)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(GetStepSize_8, "UnprofitableStartup.UnitTesting.CNumberDisplayUtils.GetStepSize_8", S_TEST_FLAGS)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(GetStepSize_9, "UnprofitableStartup.UnitTesting.CNumberDisplayUtils.GetStepSize_9", S_TEST_FLAGS)


#pragma region TruncateDisplayNumberTest
bool TruncateDisplayNumber_1::RunTest(const FString& Parameters)
{
    FString expectedResult = "100";

    FString result = UCNumberDisplayUtils::TruncateDisplayNumber(100, "");

    return TestEqual(TEXT("Expected result of '100'"), result, expectedResult);
}

bool TruncateDisplayNumber_2::RunTest(const FString& Parameters)
{
    FString expectedResult = "1,000";

    FString result = UCNumberDisplayUtils::TruncateDisplayNumber(1000, "");

    return TestEqual(TEXT("Expected result of '1,000'"), result, expectedResult);
}

bool TruncateDisplayNumber_3::RunTest(const FString& Parameters)
{
    FString expectedResult = "$5,000";

    FString result = UCNumberDisplayUtils::TruncateDisplayNumber(5000, "$");

    return TestEqual(TEXT("Expected result of '$5,000'"), result, expectedResult);
}

bool TruncateDisplayNumber_4::RunTest(const FString& Parameters)
{
    FString expectedResult = "$500k";

    FString result = UCNumberDisplayUtils::TruncateDisplayNumber(500000, "$");

    return TestEqual(TEXT("Expected result of '$500k'"), result, expectedResult);
}

bool TruncateDisplayNumber_5::RunTest(const FString& Parameters)
{
    FString expectedResult = "$150m";

    FString result = UCNumberDisplayUtils::TruncateDisplayNumber(150000000, "$");

    return TestEqual(TEXT("Expected result of '$150m'"), result, expectedResult);
}

bool TruncateDisplayNumber_6::RunTest(const FString& Parameters)
{
    FString expectedResult = "$3,612m";

    FString result = UCNumberDisplayUtils::TruncateDisplayNumber(3612000000, "$");

    return TestEqual(TEXT("Expected result of '$3,612m'"), result, expectedResult);
}

// Test a number above the int32 range of 2,147,483,647
bool TruncateDisplayNumber_7::RunTest(const FString& Parameters)
{
    FString expectedResult = "$3,147m";

    FString result = UCNumberDisplayUtils::TruncateDisplayNumber(3147483647, "$");

    return TestEqual(TEXT("Expected result of '$3,147m'"), result, expectedResult);
}

// Test a number above the int32 range of 2,147,483,647
bool TruncateDisplayNumber_8::RunTest(const FString& Parameters)
{
    FString expectedResult = "$500b";

    FString result = UCNumberDisplayUtils::TruncateDisplayNumber(500000000000, "$");

    return TestEqual(TEXT("Expected result of '$500b'"), result, expectedResult);
}

// Test a negative number
bool TruncateDisplayNumber_9::RunTest(const FString& Parameters)
{
    FString expectedResult = "-$3,147m";

    // Specify int64 type to avoid compile errors
    int64 numberToTruncate = 3147483647;
    FString result = UCNumberDisplayUtils::TruncateDisplayNumber(-numberToTruncate, "$");

    return TestEqual(TEXT("Expected result of '-$3,147m'"), result, expectedResult);
}

// Test a negative number below the int32 range of -2,147,483,647
bool TruncateDisplayNumber_10::RunTest(const FString& Parameters)
{
    FString expectedResult = "-$500b";

    int64 numberToTruncate = 500000000000;
    FString result = UCNumberDisplayUtils::TruncateDisplayNumber(-numberToTruncate, "$");

    return TestEqual(TEXT("Expected result of '$-500b'"), result, expectedResult);
}
#pragma endregion AdvanceMonthTest


#pragma region GetStepSizeTest
bool GetStepSize_1::RunTest(const FString& Parameters)
{
    int64 moneyAmount = S_MILLION * 2000;
    float expectedResult = S_MILLION;

    float result = UCNumberDisplayUtils::GetStepSize(moneyAmount);

    return TestEqual(TEXT("UCNumberDisplayUtils::GetStepSize(S_MILLION  * 2000)"), result, expectedResult);
}

bool GetStepSize_2::RunTest(const FString& Parameters)
{
    // Check negative value
    int64 moneyAmount = S_MILLION * -2000;
    float expectedResult = S_MILLION;

    float result = UCNumberDisplayUtils::GetStepSize(moneyAmount);

    return TestEqual(TEXT("UCNumberDisplayUtils::GetStepSize(S_MILLION  * -2000)"), result, expectedResult);
}

bool GetStepSize_3::RunTest(const FString& Parameters)
{
    int64 moneyAmount = S_MILLION + 1;
    float expectedResult = S_HUNDRED_THOUSAND;

    float result = UCNumberDisplayUtils::GetStepSize(moneyAmount);

    return TestEqual(TEXT("UCNumberDisplayUtils::GetStepSize(S_MILLION + 1)"), result, expectedResult);
}

bool GetStepSize_4::RunTest(const FString& Parameters)
{
    int64 moneyAmount = S_HUNDRED_THOUSAND * 15;
    float expectedResult = S_HUNDRED_THOUSAND;

    float result = UCNumberDisplayUtils::GetStepSize(moneyAmount);

    return TestEqual(TEXT("UCNumberDisplayUtils::GetStepSize(S_HUNDRED_THOUSAND * 15)"), result, expectedResult);
}

bool GetStepSize_5::RunTest(const FString& Parameters)
{
    int64 moneyAmount = S_HUNDRED_THOUSAND + 1;
    float expectedResult = S_TEN_THOUSAND;

    float result = UCNumberDisplayUtils::GetStepSize(moneyAmount);

    return TestEqual(TEXT("UCNumberDisplayUtils::GetStepSize(S_HUNDRED_THOUSAND + 1)"), result, expectedResult);
}

bool GetStepSize_6::RunTest(const FString& Parameters)
{
    int64 moneyAmount = S_TEN_THOUSAND * 13;
    float expectedResult = S_TEN_THOUSAND;

    float result = UCNumberDisplayUtils::GetStepSize(moneyAmount);

    return TestEqual(TEXT("UCNumberDisplayUtils::GetStepSize(S_TEN_THOUSAND * 13)"), result, expectedResult);
}

bool GetStepSize_7::RunTest(const FString& Parameters)
{
    int64 moneyAmount = S_THOUSAND * 2;
    float expectedResult = S_THOUSAND;

    float result = UCNumberDisplayUtils::GetStepSize(moneyAmount);

    return TestEqual(TEXT("UCNumberDisplayUtils::GetStepSize(S_THOUSAND * 2)"), result, expectedResult);
}

bool GetStepSize_8::RunTest(const FString& Parameters)
{
    int64 moneyAmount = 500;
    float expectedResult = 1.0f;

    float result = UCNumberDisplayUtils::GetStepSize(moneyAmount);

    return TestEqual(TEXT("UCNumberDisplayUtils::GetStepSize(500)"), result, expectedResult);
}

bool GetStepSize_9::RunTest(const FString& Parameters)
{
    // Check 0
    int64 moneyAmount = 0;
    float expectedResult = 0;

    float result = UCNumberDisplayUtils::GetStepSize(moneyAmount);

    return TestEqual(TEXT("UCNumberDisplayUtils::GetStepSize(0)"), result, expectedResult);
}
#pragma endregion GetStepSize