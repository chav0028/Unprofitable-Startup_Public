// Alvaro Antonio Chavez Mixco (w18367131), 6CCGD011W Computer Games Development Final Project 


#include "Utils/CNumberDisplayUtils.h"

// How much in excess an amount has to be, before it is abbrevaited.
// e.g. if this is 100, then the thousands will not be abbreviated
// until there is 100 thousands (100,000)
static constexpr float S_AMOUNT_MULTIPLIER_EXCESS = 100.0f;

// Numerical amounts normally used to handle values
static constexpr int64 S_BILLION_AMOUNT = 1000000000;
static constexpr int64 S_MILLION_AMOUNT = 1000000;
static constexpr int64 S_THOUSAND_AMOUNT = 1000;

// Signs used to indicate the abbreviate amounts
static const FString S_BILLION_SIGN = "b";
static const FString S_MILLION_SIGN = "m";
static const FString S_THOUSAND_SIGN = "k";

// Sign used to indicate the positive/negative amounts
static const FString S_POSITIVE_SIGN = "";
static const FString S_NEGATIVE_SIGN = "-";

// Possible step sizes (floats)
static constexpr float S_MILLION = 1000000.0f;
static constexpr float S_HUNDRED_THOUSAND = 100000.0f;
static constexpr float S_TEN_THOUSAND = 10000.0f;
static constexpr float S_THOUSAND = 1000.0f;

// Amounts required for each step size
static constexpr float S_MILLION_STEP_REQUIRED = S_MILLION * 100.0f;
static constexpr float S_HUNDRED_THOUSAND_STEP_REQUIRED = S_MILLION;
static constexpr float S_TEN_THOUSAND_STEP_REQUIRED = S_HUNDRED_THOUSAND;


FString UCNumberDisplayUtils::TruncateDisplayNumber(int64 aNumber, const FString& aCurrencyPrefix)
{
	FString suffixCharacter = "";

	// Set positive or negative sign
	const FString numberValueSign = aNumber >= 0 ? S_POSITIVE_SIGN : S_NEGATIVE_SIGN;


	// Use absolute version of number for it to also truncate negative values
	// Also omits the `-` negative sign, so that it can be prefixed, before 
	// the currency, if needed
	int64 absoluteNumber = FMath::Abs(aNumber);


	// Check billions
	if (absoluteNumber > S_BILLION_AMOUNT * S_AMOUNT_MULTIPLIER_EXCESS)
	{
		suffixCharacter = S_BILLION_SIGN;
		absoluteNumber /= S_BILLION_AMOUNT;
	}
	// Check millions
	else if (absoluteNumber > S_MILLION_AMOUNT * S_AMOUNT_MULTIPLIER_EXCESS)
	{
		suffixCharacter = S_MILLION_SIGN;
		absoluteNumber /= S_MILLION_AMOUNT;
	}
	// Check thousands
	else if (absoluteNumber > S_THOUSAND_AMOUNT * S_AMOUNT_MULTIPLIER_EXCESS)
	{
		suffixCharacter = S_THOUSAND_SIGN;
		absoluteNumber /= S_THOUSAND_AMOUNT;
	}

	// FormatAsNumber takes an int32, so if the number was not divided
	// before hand this could cause issues with large numbers
	// No decimal point is shown
	const FString textDisplay = numberValueSign + aCurrencyPrefix +
		FString::FormatAsNumber(absoluteNumber) + suffixCharacter;

	return textDisplay;
}

float UCNumberDisplayUtils::GetStepSize(int64 aAmount)
{
	// Cast to float for easier use
	float amount = static_cast<float>(aAmount);

	// To avoid issues with negative values, use the absoluve value
	amount = FMath::Abs(amount);

	// Check millions
	if (amount > S_MILLION_STEP_REQUIRED)
	{
		return S_MILLION;
	}
	// Check hundred thousands
	else if (amount > S_HUNDRED_THOUSAND_STEP_REQUIRED)
	{
		return S_HUNDRED_THOUSAND;
	}
	// Check tens thousands
	else if (amount > S_TEN_THOUSAND_STEP_REQUIRED)
	{
		return S_TEN_THOUSAND;
	}
	// Check thousands
	else if (amount > S_THOUSAND)
	{
		return S_THOUSAND;
	}
	// This should not happen with such small values but just in case
	else if (amount > 1.0f)
	{
		return 1.0f;
	}
	else
	{
		return 0.0f;
	}
}