// Alvaro Antonio Chavez Mixco (w18367131), 6CCGD011W Computer Games Development Final Project 

#pragma once

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h" // Needed to implement FAutomationTestBase

/**
 * Automation instructions:
 * https://docs.unrealengine.com/5.3/en-US/automation-technical-guide/
 */
class CNumberDisplayUtilsTest : public FAutomationTestBase
{
public:
	// Constructor to initialise the FAutomationTestBase
	CNumberDisplayUtilsTest(const FString& aInName, const bool abInComplexTask) : FAutomationTestBase(aInName, abInComplexTask) {}
};