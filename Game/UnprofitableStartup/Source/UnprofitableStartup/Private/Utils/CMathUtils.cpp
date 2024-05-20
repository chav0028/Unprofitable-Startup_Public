// Alvaro Antonio Chavez Mixco (w18367131), 6CCGD011W Computer Games Development Final Project 


#include "Utils/CMathUtils.h"

float UCMathUtils::MinMaxNormalization0To1(float aUnnormalizedValue, float aMinValue, float aMaxValue)
{
	if (aMinValue >= aMaxValue)
	{
		UE_LOG(LogTemp, Warning, TEXT("MinMaxNormalization0To1 min value of `%f` is bigger than max value `%f`"), aMinValue, aMaxValue);
	}
	return (aUnnormalizedValue - aMinValue) / (aMaxValue - aMinValue);
}

float UCMathUtils::MinMaxUnnormalization0To1(float aNormalizedValue, float aMinValue, float aMaxValue)
{
	if (aMinValue >= aMaxValue)
	{
		UE_LOG(LogTemp, Warning, TEXT("MinMaxUnnormalization0To1 min value of `%f` is bigger than max value `%f`"), aMinValue, aMaxValue);
	}
	return (aMaxValue - aMinValue) * aNormalizedValue + aMinValue;
}
