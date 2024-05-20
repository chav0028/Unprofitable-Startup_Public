// Alvaro Antonio Chavez Mixco (w18367131), 6CCGD011W Computer Games Development Final Project 


#include "Tests/Utils/CMathUtilsTest.h"

#include "Utils/CMathUtils.h"
#include "Tests/TestingConstants.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MinMaxNormalization0To1Test_1,     "UnprofitableStartup.UnitTesting.CMathUtils.MinMaxNormalization0To1_1", S_TEST_FLAGS)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(MinMaxNormalization0To1Test_2,     "UnprofitableStartup.UnitTesting.CMathUtils.MinMaxNormalization0To1_2", S_TEST_FLAGS)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(MinMaxNormalization0To1Test_3,     "UnprofitableStartup.UnitTesting.CMathUtils.MinMaxNormalization0To1_3", S_TEST_FLAGS)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(MinMaxNormalization0To1Test_4,     "UnprofitableStartup.UnitTesting.CMathUtils.MinMaxNormalization0To1_4", S_TEST_FLAGS)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(MinMaxNormalization0To1Test_5,     "UnprofitableStartup.UnitTesting.CMathUtils.MinMaxNormalization0To1_5", S_TEST_FLAGS)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(MinMaxUnnormalization0To1Test_1,   "UnprofitableStartup.UnitTesting.CMathUtils.MinMaxUnnormalization0To1_1", S_TEST_FLAGS)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(MinMaxUnnormalization0To1Test_2,   "UnprofitableStartup.UnitTesting.CMathUtils.MinMaxUnnormalization0To1_2", S_TEST_FLAGS)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(MinMaxUnnormalization0To1Test_3,   "UnprofitableStartup.UnitTesting.CMathUtils.MinMaxUnnormalization0To1_3", S_TEST_FLAGS)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(MinMaxUnnormalization0To1Test_4,   "UnprofitableStartup.UnitTesting.CMathUtils.MinMaxUnnormalization0To1_4", S_TEST_FLAGS)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(MinMaxUnnormalization0To1Test_5,   "UnprofitableStartup.UnitTesting.CMathUtils.MinMaxUnnormalization0To1_5", S_TEST_FLAGS)


#pragma region MinMaxNormalization0To1Test
bool MinMaxNormalization0To1Test_1::RunTest(const FString& Parameters)
{
    float unnormalizedValue = 50.0f;
    float minValue = 0.0f;
    float maxValue = 100.0f;
    float expectedResult = 0.5f;

    float result = UCMathUtils::MinMaxNormalization0To1(unnormalizedValue, minValue, maxValue);

    return TestEqual(TEXT("Expected result of 0.5"), result, expectedResult);
}

bool MinMaxNormalization0To1Test_2::RunTest(const FString& Parameters)
{
    float unnormalizedValue = 0.0f;
    float minValue = 0.0f;
    float maxValue = 100.0f;
    float expectedResult = 0.0f;

    float result = UCMathUtils::MinMaxNormalization0To1(unnormalizedValue, minValue, maxValue);

    return TestEqual(TEXT("Expected result of 0.0"), result, expectedResult);
}

bool MinMaxNormalization0To1Test_3::RunTest(const FString& Parameters)
{
    float unnormalizedValue = 100.0f;
    float minValue = 0.0f;
    float maxValue = 100.0f;
    float expectedResult = 1.0f;

    float result = UCMathUtils::MinMaxNormalization0To1(unnormalizedValue, minValue, maxValue);

    return TestEqual(TEXT("Expected result of 1.0"), result, expectedResult);
}

bool MinMaxNormalization0To1Test_4::RunTest(const FString& Parameters)
{
    float unnormalizedValue = 100.0f;
    float minValue = 50.0f;
    float maxValue = 150.0f;
    float expectedResult = 0.5f;

    float result = UCMathUtils::MinMaxNormalization0To1(unnormalizedValue, minValue, maxValue);

    return TestEqual(TEXT("Expected result of 0.5"), result, expectedResult);
}

bool MinMaxNormalization0To1Test_5::RunTest(const FString& Parameters)
{
    float unnormalizedValue = 800.0f;
    float minValue = 200.0f;
    float maxValue = 1200.0f;
    float expectedResult = 0.6f;

    float result = UCMathUtils::MinMaxNormalization0To1(unnormalizedValue, minValue, maxValue);

    return TestEqual(TEXT("Expected result of 0.6"), result, expectedResult);
}
#pragma endregion MinMaxNormalization0To1Test



#pragma region MinMaxUnnormalization0To1Test
bool MinMaxUnnormalization0To1Test_1::RunTest(const FString& Parameters)
{
    float normalizedValue = 0.5f;
    float minValue = 0.0f;
    float maxValue = 100.0f;
    float expectedResult = 50.0f;

    float result = UCMathUtils::MinMaxUnnormalization0To1(normalizedValue, minValue, maxValue);

    return TestEqual(TEXT("Result should be 50.0"), result, expectedResult);
}

bool MinMaxUnnormalization0To1Test_2::RunTest(const FString& Parameters)
{
    float normalizedValue = 0.0f;
    float minValue = 0.0f;
    float maxValue = 100.0f;
    float expectedResult = 0.0f;

    float result = UCMathUtils::MinMaxUnnormalization0To1(normalizedValue, minValue, maxValue);

    return TestEqual(TEXT("Result should be 0.0"), result, expectedResult);
}

bool MinMaxUnnormalization0To1Test_3::RunTest(const FString& Parameters)
{
    float normalizedValue = 1.0f;
    float minValue = 0.0f;
    float maxValue = 100.0f;
    float expectedResult = 100.0f;

    float result = UCMathUtils::MinMaxUnnormalization0To1(normalizedValue, minValue, maxValue);

    return TestEqual(TEXT("Result should be 100.0"), result, expectedResult);
}

bool MinMaxUnnormalization0To1Test_4::RunTest(const FString& Parameters)
{
    float normalizedValue = 0.5f;
    float minValue = 50.0f;
    float maxValue = 150.0f;
    float expectedResult = 100.0f;

    float result = UCMathUtils::MinMaxUnnormalization0To1(normalizedValue, minValue, maxValue);

    return TestEqual(TEXT("Result should be 100.0"), result, expectedResult);
}

bool MinMaxUnnormalization0To1Test_5::RunTest(const FString& Parameters)
{
    float normalizedValue = 0.6f;
    float minValue = 200.0f;
    float maxValue = 1200.0f;
    float expectedResult = 800.0f;

    float result = UCMathUtils::MinMaxUnnormalization0To1(normalizedValue, minValue, maxValue);

    return TestEqual(TEXT("Result should be 800.0"), result, expectedResult);
}
#pragma endregion MinMaxUnnormalization0To1Test