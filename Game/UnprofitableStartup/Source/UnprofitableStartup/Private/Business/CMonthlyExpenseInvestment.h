// Alvaro Antonio Chavez Mixco (w18367131), 6CCGD011W Computer Games Development Final Project 

#pragma once

#include "CoreMinimal.h"
#include "SInvestmentLevel.h"
#include "CMonthlyExpenseInvestment.generated.h"

UCLASS(Blueprintable, BlueprintType,
	meta = (ToolTip = "Abstract class to represent the monthly expense investment of a company, and an abstract bonus attached to it"))
	class UCMonthlyExpenseInvestment : public UObject
{
	GENERATED_BODY()

public:
	UCMonthlyExpenseInvestment();

	/**
	* Setup the current monthly spending and the new investment levels
	* @param aMonthlySpending The monthly spending to be set
	* @param aInvestmentLevels The new investment levels to add to the existing ones
	*/
	UFUNCTION(BlueprintCallable, Category = "Business")
	void Initialise(int64 aMonthlySpending, TArray<FSInvestmentLevel> aInvestmentLevels);

	/**
	* Add new investment levels, sort them by their required monthly spending and
	* re-calculate the current investment level
	* @param aInvestmentLevels The new investment levels to add to the existing ones
	*/
	UFUNCTION(BlueprintCallable, Category = "Business")
	void AddInvestmentLevels(TArray<FSInvestmentLevel> aInvestmentLevels);

	/**
	* Add to the historical spend and calculate the monthly spending
	*/
	UFUNCTION(BlueprintCallable, Category = "Business")
	void AdvanceMonth();

	// Getters and setters
	UFUNCTION(BlueprintCallable, Category = "Business")
	void SetCurrentMonthlySpending(int64 aCurrentMonthlySpending);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Business")
	int64 GetCurrentMonthlySpending() const;
	/**
	* This get the bonus of the investment level for which the current spending has
	* been reached, generally one below the current investment level being checked
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Business")
	float GetCurrentBonus() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Business")
	uint8 GetCurrentInvestmentLevel() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Business")
	int64 GetHistoricalSpent() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Business")
	int64 GetSpendingLeftForNextInvestmentLevel() const;
private:
	/**
	* Based on the historical spend and the required spending of each investment level,
	* calculate the current investment level trying to be reached
	*/
	void CalculateInvestmentLevel();

	int64 m_currentMonthlySpending;
	int64 m_historicalSpent;

	
	uint8 m_currentInvestmentLevel;
	TArray<FSInvestmentLevel> m_investmentLevels;
};
