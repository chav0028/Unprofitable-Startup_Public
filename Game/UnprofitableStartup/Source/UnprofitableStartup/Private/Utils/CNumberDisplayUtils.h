// Alvaro Antonio Chavez Mixco (w18367131), 6CCGD011W Computer Games Development Final Project 

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CNumberDisplayUtils.generated.h"

UCLASS(ClassGroup = (Utils), meta = (
	ToolTip = "Library class with multiple utility functions to display different number data"))
class UCNumberDisplayUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	* Truncate a number, rounding it by units of thousands, millions, or billions
	* @param aNumber The number being rounded
	* @param aCurrencyPrefix The currency prefix to add before the number being shown, keep in mind character restrictions in engine
	* @return the truncated rounded number to the nearest thousand, million, or billion, with no decimal precision
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Number Display Util")
	static FString TruncateDisplayNumber(int64 aNumber, const FString& aCurrencyPrefix = "$");
	
	/**
	* Get the step (numerical units) a numerical value should have when being handled
	* @param aAmount The value for which the step size will be calculated
	* @return The step size for the unit
	*/
	UFUNCTION(BlueprintCallable, Category = "Number Display Util")
	static float GetStepSize(int64 aAmount);
};
