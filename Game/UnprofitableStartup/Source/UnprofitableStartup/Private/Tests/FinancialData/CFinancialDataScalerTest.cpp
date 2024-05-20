// Alvaro Antonio Chavez Mixco (w18367131), 6CCGD011W Computer Games Development Final Project 


#include "Tests/FinancialData/CFinancialDataScalerTest.h"

#include "FinancialData/CFinancialDataScaler.h"
#include "Tests/TestingConstants.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FileParsingTest_1,         "UnprofitableStartup.FinancialDataScaler.FileParsingTest_1", S_TEST_FLAGS)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FileParsingTest_2,         "UnprofitableStartup.FinancialDataScaler.FileParsingTest_2", S_TEST_FLAGS)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FileParsingTest_3,         "UnprofitableStartup.FinancialDataScaler.FileParsingTest_3", S_TEST_FLAGS)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(NetIncomeUnscalingTest_1,  "UnprofitableStartup.FinancialDataScaler.NetIncomeUnscalingTest_1", S_TEST_FLAGS)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(NetIncomeUnscalingTest_2,  "UnprofitableStartup.FinancialDataScaler.NetIncomeUnscalingTest_2", S_TEST_FLAGS)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(NetIncomeUnscalingTest_3,  "UnprofitableStartup.FinancialDataScaler.NetIncomeUnscalingTest_3", S_TEST_FLAGS)


#pragma region FileParsingTest
// Parsing empty file
bool FileParsingTest_1::RunTest(const FString& Parameters)
{
    // Allow logging error messages and passing the test
    // Per https://forums.unrealengine.com/t/using-automation-testing-for-catching-expected-errors/400360
    AddExpectedError(TEXT("No JSON file for scaling data has been specified"), EAutomationExpectedErrorFlags::Exact, 1);

    bool expectedResult = false;

    bool result = false;
    UCFinancialDataScaler* dataScaler = NewObject<UCFinancialDataScaler>();
    result = dataScaler->ParseMinMaxValues("");

    return TestEqual(TEXT("Expected result of false with empty file"), result, expectedResult);
}

// Parsing invalid file
bool FileParsingTest_2::RunTest(const FString& Parameters)
{
    // Allow logging error messages and passing the test
    AddExpectedError(TEXT("Error reading JSON file"), EAutomationExpectedErrorFlags::Contains, 1);

    bool expectedResult = false;

    bool result = false;
    UCFinancialDataScaler* dataScaler = NewObject<UCFinancialDataScaler>();
    result = dataScaler->ParseMinMaxValues("non-existent.json");

    return TestEqual(TEXT("Expected result of false with non-existent file name"), result, expectedResult);
}

// Parsing correct file
bool FileParsingTest_3::RunTest(const FString& Parameters)
{
    bool expectedResult = true;

    bool result = false;
    UCFinancialDataScaler* dataScaler = NewObject<UCFinancialDataScaler>();
    result = dataScaler->ParseMinMaxValues(S_SCALING_DATA_JSON_PATH);

    return TestEqual(TEXT("Expected result of true, check that 'Content/Data/scaling_data_information.json' exists"), result, expectedResult);
}
#pragma endregion FileParsingTest

#pragma region NetIncomeScalingTest
// Scale 0
bool NetIncomeUnscalingTest_1::RunTest(const FString& Parameters)
{
    // Min value from JSON
    float expectedResult = 2689000.0f;
    float actualResult = FLT_MAX;

    bool fileRead = false;
    UCFinancialDataScaler* dataScaler = NewObject<UCFinancialDataScaler>();
    fileRead = dataScaler->ParseMinMaxValues(S_SCALING_DATA_JSON_PATH);

    actualResult = dataScaler->UnscaleNetIncome(0.0f);

    return TestTrue(TEXT("Scaling data JSON file could not be read, check that 'Content/Data/scaling_data_information.json' exists"), fileRead)
        && TestEqual(TEXT("Result should be 2,689,000.0f"), expectedResult, actualResult);
}

// Scale 1.0
bool NetIncomeUnscalingTest_2::RunTest(const FString& Parameters)
{
    // Max value from JSON
    float expectedResult = 1706715000.0f;
    float actualResult = FLT_MAX;

    bool fileRead = false;
    UCFinancialDataScaler* dataScaler = NewObject<UCFinancialDataScaler>();
    fileRead = dataScaler->ParseMinMaxValues(S_SCALING_DATA_JSON_PATH);

    actualResult = dataScaler->UnscaleNetIncome(1.0f);

    return TestTrue(TEXT("Scaling data JSON file could not be read, check that 'Content/Data/scaling_data_information.json' exists"), fileRead)
        && TestEqual(TEXT("Result should be 1,706,715,000.0f"), expectedResult, actualResult);
}

// Scale 2.0f
bool NetIncomeUnscalingTest_3::RunTest(const FString& Parameters)
{
    float expectedResult = 3410741000.0f;
    float actualResult = FLT_MAX;

    bool fileRead = false;
    UCFinancialDataScaler* dataScaler = NewObject<UCFinancialDataScaler>();
    fileRead = dataScaler->ParseMinMaxValues(S_SCALING_DATA_JSON_PATH);

    actualResult = dataScaler->UnscaleNetIncome(2.0f);
    return TestTrue(TEXT("Scaling data JSON file could not be read, check that 'Content/Data/scaling_data_information.json' exists"), fileRead)
        && TestEqual(TEXT("Result should be 3,410,741,000.0f"), expectedResult, actualResult);
}
#pragma region NetIncomeScalingTest