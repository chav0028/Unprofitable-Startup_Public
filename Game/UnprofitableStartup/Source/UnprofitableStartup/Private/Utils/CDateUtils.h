// Alvaro Antonio Chavez Mixco (w18367131), 6CCGD011W Computer Games Development Final Project 

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CDateUtils.generated.h"

UCLASS(ClassGroup = (Utils), meta = (
	ToolTip = "Library class with multiple date utility functions"))
class UCDateUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/**
	* Advance the initial datetime by a single month, changing to the next year if necessary
	* @param aInitialDateTime The date being added a month
	* @return The initial date plus a month
	*/
	UFUNCTION(BlueprintCallable, Category = "Date Util")
	static FDateTime AdvanceMonth(FDateTime aInitialDateTime);
};
