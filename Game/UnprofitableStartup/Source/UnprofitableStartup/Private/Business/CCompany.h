// Alvaro Antonio Chavez Mixco (w18367131), 6CCGD011W Computer Games Development Final Project 

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../FinancialData/SFinancialData.h"
#include "ECompanyMonthlyExpenseType.h"
#include "CMonthlyExpenseInvestment.h"
#include "CCompany.generated.h"

class UNeuralNetwork;
class UCFinancialNeuralNetwork;

UCLASS(BlueprintType,
	meta = (ToolTip = "Representation of a company, including its name and financial data"))
class UCCompany : public UObject
{
	GENERATED_BODY()
	
public:
	UCCompany();

	/**
	* Parses a JSOn file with the Minimum and Maximum value of the financial data, and setups
	* the scaler per these values
	* @param aNeuralNetworkModel The Neural Network model asset, used to predict the Net Income
	* @param aScalingDataFilePath The path of the JSON file (relative to Content folder) that contains the MinMax scaling data
	* @param aAvailableCash The starting cash the company will start with
	* @param aMarketingExpenses The starting amount of cash spend by the company in Marketing
	* @param aResearchExpenses The starting amount of cash spend by the company in Research
	* @param aMonthlyBudgetPercentage The percentage of the cash available that the company can spend per month
	* @param aMinimumMonthlyBudget The starting minimum amount of budget the company can get per month
	* @param aStartingFinancialData The starting financial data, to be set as current
	*/
	UFUNCTION(BlueprintCallable, Category = "Business")
	void Initialise(UNeuralNetwork* aNeuralNetworkModel, const FString& aScalingDataFilePath,
		int64 aAvailableCash, TMap<ECompanyMonthyExpenseType, UCMonthlyExpenseInvestment*> aMonthlyExpenses,
		float aMonthlyBudgetPercentage, int64 aMinimumMonthlyBudget,
		FSFinancialData aStartingFinancialData);

	/**
	* Parses a JSOn file with the Minimum and Maximum value of the financial data, and setups
	* the scaler per these values. This function for ease of reading, has a scaling factor for which
	* the input parameters are multiplied for.
	* @param aParametersMultiplier The amount all the input parameters will be multiplied by, this does not affect the company subsequent operations
	* @param aNeuralNetworkModel The Neural Network model asset, used to predict the Net Income
	* @param aScalingDataFilePath The path of the JSON file (relative to Content folder) that contains the MinMax scaling data
	* @param aAvailableCash The starting cash the company will start with
	* @param aMarketingExpenses The starting amount of cash spend by the company in Marketing
	* @param aResearchExpenses The starting amount of cash spend by the company in Research
	* @param aMonthlyBudgetPercentage The percentage of the cash available that the company can spend per month
	* @param aMinimumMonthlyBudget The starting minimum amount of budget the company can get per month
	* @param aStartingFinancialData The starting financial data, to be set as current
	*/
	UFUNCTION(BlueprintCallable, Category = "Business")
	void InitialiseWithMultiplier(int64 aParametersMultiplier, UNeuralNetwork* aNeuralNetworkModel, const FString& aScalingDataFilePath,
		int64 aAvailableCash, TMap<ECompanyMonthyExpenseType, UCMonthlyExpenseInvestment*> aMonthlyExpenses,
		float aMonthlyBudgetPercentage, int64 aMinimumMonthlyBudget,
		FSFinancialData aStartingFinancialData);

	/**
	* Set a cap to the amount the net income can change each month
	* This is capped before calculating any potential bonus
	* @param aChangePercent The percentage amount the net income can change, in comparison to the previous month
	* @param aMinimumChangeAmount The minimum amount the net income can change, in comparison to the previous month, even when capped
	* @param aIsChangeCapped Control if the net income change is capped or not
	*/
	UFUNCTION(BlueprintCallable, Category = "Business",
		meta = (ToolTip = "Set how much the net income can change each month from its previous value"))
	void SetNetIncomeMonthlyChangeCap(float aChangePercent, float aMinimumChangeAmount, bool aIsChangeCapped = true);



	/**
	* Run a simulation of the month using the neural network, updating the Net Income and other related financial statistics
	*/
	UFUNCTION(BlueprintCallable, Category = "Business")
	void SimulateMonth();

	/**
	* Allocate the budget for the desired expense, capping the amount allocated to
	* the maximum possible
	* @param aBudget The budget amount to try to be allocated for marketing
	* @param aStepSize The step size to round the amount allocated to
	* @return The amount of budget that was successfully allocated for marketing
	*/
	UFUNCTION(BlueprintCallable, Category = "Business")
	int64 TryAllocateMonthlyExpenseBudget(ECompanyMonthyExpenseType aMonthlyExpenseType, int64 aBudget, float aStepSize);



	// Getters, for the UI these could be BlueprintReadOnly with AllowPrivateAccess specifiers,
	// but made public functions to support C++ code
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Business")
	FString GetCompanyName() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Business")
	int64 GetCashAvailable() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Business")
	int64 GetMonthlyExpensesFromAllTypes() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Business")
	int64 GetMonthlyBudget() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Business")
	int64 GetOverallProfits() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Business")
	int64 GetMonthlyExpenses() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Business")
	int64 GetNetIncome() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Business")
	FSFinancialData GetCurrentFinancialData() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Business")
	int64 GetMonthlyExpenseByType(ECompanyMonthyExpenseType aMonthlyExpenseType) const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Business")
	uint8 GetMonthlyExpenseCurrentInvestmentLevel(ECompanyMonthyExpenseType aMonthlyExpenseType) const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Business")
	int64 GetMonthlyExpenseHistoricalSpending(ECompanyMonthyExpenseType aMonthlyExpenseType) const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Business")
	int64 GetMonthlyExpenseFundingNextLevel(ECompanyMonthyExpenseType aMonthlyExpenseType) const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Business")
	float GetMonthlyExpenseBonus(ECompanyMonthyExpenseType aMonthlyExpenseType) const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Business")
	float GetNetIncomeChangePerMonth() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Business",
		meta = (ToolTip = "The amount of the monthly budget free, after deducting the monthly expenses"))
	int64 GetMonthlyBudgetAvailable() const;



	// Simple Setters
	UFUNCTION(BlueprintCallable, Category = "Business")
	void SetCompanyName(const FString& aCompanyName);
	UFUNCTION(BlueprintCallable, Category = "Business")
	void SetNetIncomeHandicapAmount(int64 aHandicapAmount);
	UFUNCTION(BlueprintCallable, Category = "Business")
	void SetMaxStockValue(float aMaxStockValue);
	UFUNCTION(BlueprintCallable, Category = "Business")
	void SetNegativeNetIncomeMarketingBonusRate(float aBonusRate);

private:
	// Evaluate and update any financial statistic at the end of the month
	void EvaluateEndMonthStats();
	// Run the neural network simulation to predict the Net Income,
	// and apply any extra calculations to the Net Income
	void CalculateNetIncome();
	// Calculate the new monthly budget to be allocated
	void CalculateMonthlyBudget();
	// Get the overall monthly expenses at the end of the month
	void CalculateMonthlyOperationExpenses();
	/**
	* Rebalance the monthly expenses, so that if the monthly budget is not enough, the expenses are reduced proportionally
	* @param aPrevMonthlyBudget The company budget of the previous month
	*/
	void RebalanceMonthlyExpenses(int64 aPrevMonthlyBudget);
	/**
	* Cap the net income change between months
	* @param aPrevMonthlyBudget The company budget of the previous month
	*/
	void CapNetIncomeChange(float aPrevMonthNetIncome);
	// Calculate the stock value of the company based on the previous month performance
	void CalculateCompanyStockValue();
	
	
	/**
	* Try to allocate a certain amount of budget to a certain expense, capping the amount allocated to
	* @param aMonthlyExpense The expense value being allocated
	* @param aAmountToAllocate The amount of budget to try to allocate
	* @param aStepSize The step size to round the amount allocated to
	*/
	void TryAllocateMonthlyExpense(int64& aMonthlyExpense, int64 aAmountToAllocate, float aStepSize);



	UPROPERTY(EditAnywhere,
		meta = (ToolTip = "The Neural Network used to predict the Net Income"))
	UCFinancialNeuralNetwork* m_financialNN;
	

	UPROPERTY(EditAnywhere,
		meta = (ToolTip = "The name of the company"))
	FString m_companyName;
	UPROPERTY(EditAnywhere,
		meta = (ToolTip = "The finacial data of the current month for the company"))
	FSFinancialData m_currentFinancialData;


	UPROPERTY(EditAnywhere,
		meta = (ToolTip = "The cash reserves currently available for the company"))
	int64 m_cashAvailable;
	UPROPERTY(VisibleInstanceOnly,
		meta = (ToolTip = "Monthly expenses that after certain spending can provide certain bonuses"))
	TMap<ECompanyMonthyExpenseType, UCMonthlyExpenseInvestment*> m_monthlyExpenses;

	UPROPERTY(VisibleInstanceOnly,
		meta = (ToolTip = "The currently set monthly budget"))
	int64 m_monthlyBudget;
	UPROPERTY(EditAnywhere,
		meta = (ToolTip = "The percentage of the cash available that the company can spend per month"))
	float m_monthlyBudgetPercentage;
	UPROPERTY(EditAnywhere,
		meta = (ToolTip = "The minimum amount of budget the company can get per month"))
	int64 m_minimumMonthlyBudget;


	UPROPERTY(meta = (ToolTip = "The profits made by the company throughout its entire history"))
	int64 m_overallProfits;
	UPROPERTY(meta = (ToolTip = "The overall monthly expenses of the company"))
	int64 m_monthlyOperationExpenses;


	// Net income game balance
	UPROPERTY(EditAnywhere,
		meta = (ToolTip = "Whether the change in net income each month from the company is capped"))
	bool m_isNetIncomeChangeCapped;
	UPROPERTY(EditAnywhere,
		meta = (ToolTip = "If net income change is capped, the percentage of change allowed each month"))
	float m_maxNetIncomePercentChange;
	UPROPERTY(EditAnywhere,
		meta = (ClampMin = "0.0", 
			ToolTip = "The minimum amount the net income can change, in comparison to the previous month, even when capped"))
	float m_cappedNetIncomeMinimumChange;
	UPROPERTY(EditAnywhere,
		meta = (ToolTip = "An amount to be deducted to the net income calculated by the neural network"))
	int64 m_netIncomeHandicapAmount;
	UPROPERTY(VisibleAnywhere,
		meta = (ToolTip = "The amount that the net income has changed in this month in comparison to the previous month"))
	int64 m_netIncomeChangePerMonth;


	// Marketing bonus/stock value game balance
	UPROPERTY(EditAnywhere,
		meta = (ClampMin = "0.0", 
			ToolTip = "The max value that the stock price can have"))
	float m_maxStockValue;
	UPROPERTY(EditAnywhere,
		meta = (ClampMin = "0.0", ClampMax = "1.0",
			ToolTip = "The percentage of the marketing bonus applied when the net income is negative"))
	float m_negativeNetIncomeMarketingBonusRate;
};
