// Alvaro Antonio Chavez Mixco (w18367131), 6CCGD011W Computer Games Development Final Project 

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CMinMaxScaler.generated.h"


UCLASS(BlueprintType,
	meta = (ToolTip = "Class to simplify the process of scaling/unscaling data by storing the min and max values of the data"))
class UCMinMaxScaler : public UObject
{
	GENERATED_BODY()

public:
	UCMinMaxScaler();

	/**
	* Set the Unnormalized Min and Max value of the whole dataset
	* used by the scaler.
	* @param aMinValue The unnormalized minimum value of the dataset
	* @param aMaxValue The unnormalized maximum value of the dataset
	*/
	void SetMinMaxValues(float aMinValue, float aMaxValue);

	/**
	* Scales a value to a 0 to 1 range, based on the whole dataset minimum and maximum values
	* @param aUnnormalizedValue The unnormalized value
	* @return the normalized value in  a 0 to 1 range
	*/
	UFUNCTION(BlueprintCallable, Category = "MinMax Scaler")
	float ScaleValue(float aUnnormalizedValue) const;

	/**
	* Unscales a value from a 0 to 1 range, to its real unnormalized values
	* @param aNormalizedValue The normalized value that is scaled to a 0 to 1 range
	* @return the unnormalized value in its real values range (not restricted to 0 to 1)
	*/
	UFUNCTION(BlueprintCallable, Category = "MinMax Scaler")
	float UnscaleValue(float aNormalizedValue) const;

private:
	// Min and max value, unnormalized, of the whole data used for the scaling
	float m_minValue;
	float m_maxValue;
};
