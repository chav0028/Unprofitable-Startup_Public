// Alvaro Antonio Chavez Mixco (w18367131), 6CCGD011W Computer Games Development Final Project 

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CGameUtils.generated.h"

UCLASS(ClassGroup = (Utils), meta = (
	ToolTip = "Library class with utility functions for multiple game related tasks"))
	class UCGameUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	* Reloads the current Unreal Engine level'
	* @param aWorldContextObject The object that is used to get the current world context
	*/
	UFUNCTION(BlueprintCallable, Category = "Game Util")
	static void ReloadCurrentLevel(const UObject* aWorldContextObject);
};
