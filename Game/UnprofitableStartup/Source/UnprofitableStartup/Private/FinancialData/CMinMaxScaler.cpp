// Alvaro Antonio Chavez Mixco (w18367131), 6CCGD011W Computer Games Development Final Project 


#include "FinancialData/CMinMaxScaler.h"
#include "Utils/CMathUtils.h"

UCMinMaxScaler::UCMinMaxScaler() :
	m_minValue(0.0f),
	m_maxValue(0.0f)
{
}

void UCMinMaxScaler::SetMinMaxValues(float aMinValue, float aMaxValue)
{
	m_minValue = aMinValue;
	m_maxValue = aMaxValue;
}

float UCMinMaxScaler::ScaleValue(float aUnnormalizedValue) const
{
	return UCMathUtils::MinMaxNormalization0To1(aUnnormalizedValue, m_minValue, m_maxValue);
}

float UCMinMaxScaler::UnscaleValue(float aNormalizedValue) const
{
	return UCMathUtils::MinMaxUnnormalization0To1(aNormalizedValue, m_minValue, m_maxValue);
}
