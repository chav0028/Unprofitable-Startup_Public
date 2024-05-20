// Alvaro Antonio Chavez Mixco (w18367131), 6CCGD011W Computer Games Development Final Project 

#pragma once

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h" // Needed to implement FAutomationTestBase

class CDateUtilsTest : public FAutomationTestBase
{
public:
    // Constructor to initialise the FAutomationTestBase
    CDateUtilsTest(const FString& aInName, const bool abInComplexTask) : FAutomationTestBase(aInName, abInComplexTask) {}
};
