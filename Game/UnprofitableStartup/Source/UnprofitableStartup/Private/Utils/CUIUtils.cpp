// Alvaro Antonio Chavez Mixco (w18367131), 6CCGD011W Computer Games Development Final Project 


#include "Utils/CUIUtils.h"

#include <Components/Slider.h>

void UCUIUtils::SetSliderValues(USlider* aSlider, float aSliderStepSize, float aInitialValue, float aMaxValue, float aMinValue)
{
	// Check valid slider
	if (aSlider == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetSliderValues slider is nullptr"));
		return;
	}

	// Check minimum value is smaller than maximum value
	if (aMinValue >= aMaxValue)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetSliderValues min value of `%f` is bigger than max value `%f`"), aMinValue, aMaxValue);
	}

	// For aesthethic purposes, if the max value is 0, set it to 1
	// so that the handle at 0 value is set to the left
	if (aMaxValue == 0.0f)
	{
		aMaxValue = 1.0f;
	}

	aSlider->SetStepSize(aSliderStepSize);
	aSlider->SetMinValue(aMinValue);
	aSlider->SetMaxValue(aMaxValue);

	// Check if the initial value is within the limits
	if (aInitialValue < aMinValue)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetSliderValues value of `%f` is smaller than min value `%f`"), aInitialValue, aMinValue);
		aSlider->SetValue(aMinValue);
	}
	else if (aInitialValue > aMaxValue)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetSliderValues value of `%f` is bigger than max value `%f`"), aInitialValue, aMaxValue);
		aSlider->SetValue(aMaxValue);
	}
	else
	{
		aSlider->SetValue(aInitialValue);
	}
}
