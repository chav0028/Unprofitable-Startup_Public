// Alvaro Antonio Chavez Mixco (w18367131), 6CCGD011W Computer Games Development Final Project 

#pragma once

#include "CoreMinimal.h"
#include "SInvestmentLevel.generated.h" // Generated file must match name of the file, and not precisely the struct



USTRUCT(BlueprintType,
	meta = (ToolTip = "Representation the financial data of a company"))
struct FSInvestmentLevel
{
	GENERATED_BODY()

public:
	FSInvestmentLevel()
		: m_requiredSpending(0), m_levelBonus(0.0f)
	{
	}

	FSInvestmentLevel(int64 aRequiredSpending, float aLevelBonus)
		: m_requiredSpending(aRequiredSpending), m_levelBonus(aLevelBonus)
	{
	}

	// Override the `<` operator so that when sorting, this struct
	// is sorted by the required spending
	bool operator<(const FSInvestmentLevel& aOther) const
	{
		return m_requiredSpending < aOther.m_requiredSpending;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (ToolTip = "The overall historical spending required to pass this investment level"))
	int64 m_requiredSpending;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (ClampMin = "0.0", ClampMax = "1.0", // Clamp the value between 0 and 1, in the editor
			ToolTip = "An abstract bonus amount that is provided when this investment level is reached"))
	float m_levelBonus;
};
