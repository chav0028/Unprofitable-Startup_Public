// Alvaro Antonio Chavez Mixco (w18367131), 6CCGD011W Computer Games Development Final Project 

#pragma once

#include "CoreMinimal.h"
#include "EEndGameCondition.generated.h"

UENUM(BlueprintType, meta = (
	ToolTip = "Enum to represent the condition in which the game can end"))
enum class EEndGameCondition : uint8
{
	GameActive, // Game has not ended
	OutOfCash, // Player has lost, running out of cash available
	Profitability, // The target profits and cash reserves have been reached
	MaxTurns // The max number of turns has been reached
};