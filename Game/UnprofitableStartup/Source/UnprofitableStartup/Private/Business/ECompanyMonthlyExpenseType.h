// Alvaro Antonio Chavez Mixco (w18367131), 6CCGD011W Computer Games Development Final Project 

#pragma once

#include "CoreMinimal.h"
#include "ECompanyMonthlyExpenseType.generated.h"

UENUM(BlueprintType, meta = (
	ToolTip = "Enum to represent a monthly expense in the company"))
enum class ECompanyMonthyExpenseType : uint8
{
	Marketing,
	ResearchAndDevelopment,
	Count UMETA(Hidden) // Hidden to avoid the enum to be used as a value in editor
};
ENUM_RANGE_BY_COUNT(ECompanyMonthyExpenseType, ECompanyMonthyExpenseType::Count); // Indicate the last value in the enum to use for iteration