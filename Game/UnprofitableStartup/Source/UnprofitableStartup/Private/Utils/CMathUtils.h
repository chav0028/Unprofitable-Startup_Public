// Alvaro Antonio Chavez Mixco (w18367131), 6CCGD011W Computer Games Development Final Project 

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CMathUtils.generated.h"

UCLASS(ClassGroup = (Utils), meta = (
	ToolTip = "Library class with multiple math utility functions"))
class UCMathUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:	
	/**
	* Scales a value to a 0 to 1 range, based on the whole dataset minimum and maximum values
	* @param aUnnormalizedValue The unnormalized value
	* @param aMinValue The minimum, unnormalized, value of the whole dataset
	* @param aMaxValue The maximum, unnormalized, value of the whole dataset
	* @return the normalized value in  a 0 to 1 range
	*/
	UFUNCTION(BlueprintCallable, Category = "Math Util")
	static float MinMaxNormalization0To1(float aUnnormalizedValue, float aMinValue, float aMaxValue);

	/**
	* Unscales a value from a 0 to 1 range, to its real unnormalized values
	* @param aNormalizedValue The normalized value that is scaled to a 0 to 1 range
	* @param aMinValue The minimum, unnormalized, value of the whole dataset
	* @param aMaxValue The maximum, unnormalized, value of the whole dataset
	* @return the unnormalized value in its real values range (not restricted to 0 to 1)
	*/
	UFUNCTION(BlueprintCallable, Category = "Math Util")
	static float MinMaxUnnormalization0To1(float aNormalizedValue, float aMinValue, float aMaxValue);
};
