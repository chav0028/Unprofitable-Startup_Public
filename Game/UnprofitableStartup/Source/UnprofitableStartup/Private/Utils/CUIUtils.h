// Alvaro Antonio Chavez Mixco (w18367131), 6CCGD011W Computer Games Development Final Project 

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Components/Slider.h"
#include "CUIUtils.generated.h"

UCLASS(ClassGroup = (Utils), meta = (
	ToolTip = "Library class with multiple utility functions to handle UI"))
class UCUIUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/**
	* Get the step a slider should have based on its numerical value
	* @param aSlider The amount of money to be converted to a slider step size
	* @param aSliderStepSize The amount of money to be converted to a slider step size
	* @param aInitialValue The amount of money to be converted to a slider step size
	* @param aMaxValue The amount of money to be converted to a slider step size
	* @param aMinValue The amount of money to be converted to a slider step size
	*/
	UFUNCTION(BlueprintCallable, Category = "UI Util")
	static void SetSliderValues(USlider* aSlider, float aSliderStepSize, float aInitialValue, float aMaxValue, float aMinValue = 0.0f);
};
