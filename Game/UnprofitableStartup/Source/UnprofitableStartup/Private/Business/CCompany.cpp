// Alvaro Antonio Chavez Mixco (w18367131), 6CCGD011W Computer Games Development Final Project 


#include "Business/CCompany.h"

#include <NeuralNetwork.h>
#include "Engine/World.h"
#include "CFinancialNeuralNetwork.h"
#include "Utils/CDateUtils.h"
#include "Utils/CNumberDisplayUtils.h"

#pragma region Initialisation
UCCompany::UCCompany() :
	m_financialNN(nullptr),
	m_companyName(""),
	m_currentFinancialData(),
	m_cashAvailable(0),
	m_monthlyBudget(0),
	m_monthlyBudgetPercentage(0.1f),
	m_minimumMonthlyBudget(0),
	m_overallProfits(0),
	m_monthlyOperationExpenses(0),
	m_isNetIncomeChangeCapped(true),
	m_cappedNetIncomeMinimumChange(100000),
	m_maxNetIncomePercentChange(0.1f),
	m_netIncomeHandicapAmount(0),
	m_netIncomeChangePerMonth(0),
	m_maxStockValue(650.0f), // Default value being the max stock value per scaling data
	m_negativeNetIncomeMarketingBonusRate(0.5f)
{
	m_financialNN = CreateDefaultSubobject<UCFinancialNeuralNetwork>(TEXT("Net Income NN"));

	// Initialise all spending types to 0
	for (ECompanyMonthyExpenseType expenseType : TEnumRange<ECompanyMonthyExpenseType>())
	{
		FString enumName = TEXT("ECompanyMonthyExpenseType_") + UEnum::GetValueAsString(expenseType);
		UCMonthlyExpenseInvestment* newExpense = CreateDefaultSubobject<UCMonthlyExpenseInvestment>(*enumName);
		m_monthlyExpenses.Add(expenseType, newExpense);
	}
}

void UCCompany::Initialise(UNeuralNetwork* aNeuralNetworkModel, const FString& aScalingDataFilePath, 
	int64 aAvailableCash, TMap<ECompanyMonthyExpenseType, UCMonthlyExpenseInvestment*> aMonthlyExpenses, 
	float aMonthlyBudgetPercentage, int64 aMinimumMonthlyBudget, FSFinancialData aStartingFinancialData)
{
	// Setup the Neural Network
	m_financialNN->SetModel(aNeuralNetworkModel, aScalingDataFilePath);

	m_cashAvailable = aAvailableCash;

	// Set the monthly expenses, at start assigning the unverified budgets
	m_monthlyExpenses = aMonthlyExpenses;

	// Calculate the total monthly expenses, for all expenses type
	int64 totalMonthlyExpenses = 0;
	for (ECompanyMonthyExpenseType expenseType : TEnumRange<ECompanyMonthyExpenseType>())
	{
		totalMonthlyExpenses += aMonthlyExpenses[expenseType]->GetCurrentMonthlySpending();
	}

	// Calculating budget to verify they are valid values within budget
	if (totalMonthlyExpenses > aMinimumMonthlyBudget)
	{
		UE_LOG(LogTemp, Warning, TEXT("The total expenses %lld are bigger than the monthly budget %lld, capping the values"),
			totalMonthlyExpenses, aMinimumMonthlyBudget);

		// Cap the monthly expenses evenly accross all types
		int64 expensePerType = aMinimumMonthlyBudget / static_cast<int64>(ECompanyMonthyExpenseType::Count);
		for (ECompanyMonthyExpenseType expenseType : TEnumRange<ECompanyMonthyExpenseType>())
		{
			// Set the monthly expenses
			m_monthlyExpenses[expenseType]->SetCurrentMonthlySpending(expensePerType);
		}
	}

	// Check that is is a valid percentage
	if (aMonthlyBudgetPercentage > 1.0f || aMonthlyBudgetPercentage < 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("The initialising monthly budget percentage of %f, is not within a 0 to 1 range, clamping the value"), aMonthlyBudgetPercentage);

		aMonthlyBudgetPercentage = FMath::Clamp(aMonthlyBudgetPercentage, 0.0f, 1.0f);
	}

	m_monthlyBudgetPercentage = aMonthlyBudgetPercentage;
	m_minimumMonthlyBudget = aMinimumMonthlyBudget;

	m_currentFinancialData = aStartingFinancialData;

	// Initialise multiple stats
	CalculateMonthlyBudget();
	CalculateMonthlyOperationExpenses();
}

void UCCompany::InitialiseWithMultiplier(int64 aParametersMultiplier, 
	UNeuralNetwork* aNeuralNetworkModel, const FString& aScalingDataFilePath, 
	int64 aAvailableCash, TMap<ECompanyMonthyExpenseType, UCMonthlyExpenseInvestment*> aMonthlyExpenses,
	float aMonthlyBudgetPercentage, int64 aMinimumMonthlyBudget, FSFinancialData aStartingFinancialData)
{
	// Multiply each expense by the parameter multiplier
	for (ECompanyMonthyExpenseType expenseType : TEnumRange<ECompanyMonthyExpenseType>())
	{
		// Multiply the monthly expenses by the parameter multiplier
		int64 monthlyExpense = aMonthlyExpenses[expenseType]->GetCurrentMonthlySpending();
		monthlyExpense *= aParametersMultiplier;
		aMonthlyExpenses[expenseType]->SetCurrentMonthlySpending(monthlyExpense);
	}	

	// Initialise the company with the multiplied values
	Initialise(aNeuralNetworkModel, aScalingDataFilePath, aAvailableCash * aParametersMultiplier, 
		aMonthlyExpenses, aMonthlyBudgetPercentage, aMinimumMonthlyBudget * aParametersMultiplier, 
		aStartingFinancialData);
}

void UCCompany::SetNetIncomeMonthlyChangeCap(float aChangePercent, float aMinimumChangeAmount, bool aIsChangeCapped)
{
	m_maxNetIncomePercentChange = aChangePercent;
	m_isNetIncomeChangeCapped = aIsChangeCapped;

	// Ensure that the minimum change amount is not negative
	if (aMinimumChangeAmount < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("The minimum net income change amount %f is negative, using the absolute value"), aMinimumChangeAmount);
	}
	m_cappedNetIncomeMinimumChange = FMath::Abs(aMinimumChangeAmount);
}
#pragma endregion Initialisation



void UCCompany::SimulateMonth()
{
	// Calculate the stock value of the company based on its previous month performance
	CalculateCompanyStockValue();

	// Update the monthly operation expenses
	CalculateMonthlyOperationExpenses();

	// Calcualte the new net income by running the Neural Network
	// and applying any bonus/cap to it
	CalculateNetIncome();

	// Update the date
	m_currentFinancialData.m_date = UCDateUtils::AdvanceMonth(m_currentFinancialData.m_date);

	// Update any financial stat record
	EvaluateEndMonthStats();
}

int64 UCCompany::TryAllocateMonthlyExpenseBudget(ECompanyMonthyExpenseType aMonthlyExpenseType, int64 aBudget, float aStepSize)
{
	// Double check that is is a valid type
	if (aMonthlyExpenseType >= ECompanyMonthyExpenseType::Count)
	{
		UE_LOG(LogTemp, Warning, TEXT("The monthly expense type %d is not valid, please use a valid type"), aMonthlyExpenseType);
		return 0;
	}

	// Allocate the expense
	int64 monthlyExpense = m_monthlyExpenses[aMonthlyExpenseType]->GetCurrentMonthlySpending();
	TryAllocateMonthlyExpense(monthlyExpense, aBudget, aStepSize);
	
	// Update the expense
	m_monthlyExpenses[aMonthlyExpenseType]->SetCurrentMonthlySpending(monthlyExpense);

	// After the expense has been updated, recalculate the monthly operation expenses
	CalculateMonthlyOperationExpenses();

	return monthlyExpense;
}



#pragma region PublicGetters
FString UCCompany::GetCompanyName() const
{
	return m_companyName;
}

int64 UCCompany::GetCashAvailable() const
{
	return m_cashAvailable;
}

int64 UCCompany::GetMonthlyExpensesFromAllTypes() const
{
	int64 expensesPerMonth = 0;
	for (ECompanyMonthyExpenseType expenseType : TEnumRange<ECompanyMonthyExpenseType>())
	{
		expensesPerMonth += m_monthlyExpenses[expenseType]->GetCurrentMonthlySpending();
	}

	return expensesPerMonth;
}

int64 UCCompany::GetMonthlyBudget() const
{
	return m_monthlyBudget;
}

int64 UCCompany::GetOverallProfits() const
{
	return m_overallProfits;
}

int64 UCCompany::GetMonthlyExpenses() const
{
	return m_monthlyOperationExpenses;
}

int64 UCCompany::GetNetIncome() const
{
	return m_currentFinancialData.m_netIncome;
}

FSFinancialData UCCompany::GetCurrentFinancialData() const
{
	return m_currentFinancialData;
}

int64 UCCompany::GetMonthlyExpenseByType(ECompanyMonthyExpenseType aMonthlyExpenseType) const
{
	// Return the expenses per month for the specific type
	if (aMonthlyExpenseType < ECompanyMonthyExpenseType::Count)
	{
		return m_monthlyExpenses[aMonthlyExpenseType]->GetCurrentMonthlySpending();
	}
	// Default to returning the sum of all monthly expense types
	else
	{
		return GetMonthlyExpensesFromAllTypes();
	}
}

uint8 UCCompany::GetMonthlyExpenseCurrentInvestmentLevel(ECompanyMonthyExpenseType aMonthlyExpenseType) const
{
	return m_monthlyExpenses[aMonthlyExpenseType]->GetCurrentInvestmentLevel();
}

int64 UCCompany::GetMonthlyExpenseHistoricalSpending(ECompanyMonthyExpenseType aMonthlyExpenseType) const
{
	return m_monthlyExpenses[aMonthlyExpenseType]->GetHistoricalSpent();
}

int64 UCCompany::GetMonthlyExpenseFundingNextLevel(ECompanyMonthyExpenseType aMonthlyExpenseType) const
{
	return m_monthlyExpenses[aMonthlyExpenseType]->GetSpendingLeftForNextInvestmentLevel();
}

float UCCompany::GetMonthlyExpenseBonus(ECompanyMonthyExpenseType aMonthlyExpenseType) const
{
	return m_monthlyExpenses[aMonthlyExpenseType]->GetCurrentBonus();
}

float UCCompany::GetNetIncomeChangePerMonth() const
{
	return m_netIncomeChangePerMonth;
}

int64 UCCompany::GetMonthlyBudgetAvailable() const
{
	// This is the monthly budget, that has not yet been allocated to monthly expenses
	return m_monthlyBudget - m_monthlyOperationExpenses;
}
#pragma endregion PublicGetters



#pragma region SimpleSetters
void UCCompany::SetCompanyName(const FString& aCompanyName)
{
	m_companyName = aCompanyName;
}

void UCCompany::SetNetIncomeHandicapAmount(int64 aHandicapAmount)
{
	m_netIncomeHandicapAmount = aHandicapAmount;
}

void UCCompany::SetMaxStockValue(float aMaxStockValue)
{
	m_maxStockValue = aMaxStockValue;
}

void UCCompany::SetNegativeNetIncomeMarketingBonusRate(float aBonusRate)
{
	if (aBonusRate < 0.0f || aBonusRate > 1.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("The Negative Net Income Marketing Bonus Rate %f is outside of the 0 to 1 range, clamping the value"), aBonusRate);
	}

	aBonusRate = FMath::Clamp(aBonusRate, 0.0f, 1.0f);

	m_negativeNetIncomeMarketingBonusRate = aBonusRate;
}
#pragma endregion SimpleSetters



void UCCompany::EvaluateEndMonthStats()
{
	// Calculate how much money the company has left
	m_cashAvailable += m_currentFinancialData.m_netIncome;

	// Update the monthly budget
	int prevBudget = m_monthlyBudget;
	CalculateMonthlyBudget();

	// Rebalance the monthly expenses, proportionally, so that the budget is respected
	RebalanceMonthlyExpenses(prevBudget);

	// Check if the current investments have unlocked a new bonus
	for (ECompanyMonthyExpenseType expenseType : TEnumRange<ECompanyMonthyExpenseType>())
	{
		m_monthlyExpenses[expenseType]->AdvanceMonth();
	}

	// Update the tracking of the net income
	m_overallProfits += m_currentFinancialData.m_netIncome;

}

void UCCompany::CalculateNetIncome()
{
	// Do the Neural Network prediction
	float prevMonthNetIncome = m_currentFinancialData.m_netIncome;
	if (m_financialNN != nullptr)
	{
		m_currentFinancialData.m_netIncome = m_financialNN->RunModel(m_currentFinancialData);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("The Neural Network is not set, cannot calculate the net income"));
	}

	// Apply the handicap to the net income, after it has been run through the neural network
	m_currentFinancialData.m_netIncome -= m_netIncomeHandicapAmount;

	// Cap the net income change
	CapNetIncomeChange(prevMonthNetIncome);

	// Apply the research bonus to the net income
	m_currentFinancialData.m_netIncome += std::abs(m_currentFinancialData.m_netIncome) * m_monthlyExpenses[ECompanyMonthyExpenseType::ResearchAndDevelopment]->GetCurrentBonus();

	// Calculate how much the net income changed, after all other calculations have been done
	m_netIncomeChangePerMonth = m_currentFinancialData.m_netIncome - prevMonthNetIncome;
}

void UCCompany::CalculateMonthlyBudget()
{
	int64 newMonthlyBudget = 0;
	if (m_cashAvailable <= m_minimumMonthlyBudget)
	{
		newMonthlyBudget = m_cashAvailable;
	}
	else
	{
		newMonthlyBudget = FMath::Floor(m_cashAvailable * m_monthlyBudgetPercentage);

		if (newMonthlyBudget < m_minimumMonthlyBudget)
		{
			newMonthlyBudget = m_minimumMonthlyBudget;
		}
	}

	// Ensure that the monthly budget is never negative
	if (newMonthlyBudget < 0)
	{
		newMonthlyBudget = 0;
	}

	m_monthlyBudget = newMonthlyBudget;
}

void UCCompany::CalculateMonthlyOperationExpenses()
{
	// Separate function from GetMonthlyExpensesFromAllTypes,
	// to be able to add one off, or exceptional expenses to a month
	m_monthlyOperationExpenses = GetMonthlyExpensesFromAllTypes();

	m_currentFinancialData.m_operatingExpenses = m_monthlyOperationExpenses;
}

void UCCompany::RebalanceMonthlyExpenses(int64 aPrevMonthlyBudget)
{
	// If the new budget is smaller than the previous budget, and it cannot cover the expenses
	// rebalance the expenses to fit the new budget
	if (m_monthlyBudget < aPrevMonthlyBudget && m_monthlyOperationExpenses > m_monthlyBudget)
	{
		// Rebalance the expenses, proportionally to the new budget
		float stepSize = UCNumberDisplayUtils::GetStepSize(m_monthlyBudget);
		for (ECompanyMonthyExpenseType expenseType : TEnumRange<ECompanyMonthyExpenseType>())
		{
			float expenseProportion = 0.0f;
			expenseProportion = static_cast<float>(m_monthlyExpenses[expenseType]->GetCurrentMonthlySpending()) / aPrevMonthlyBudget;

			int64 newExpense = expenseProportion * m_monthlyBudget;
			TryAllocateMonthlyExpense(newExpense, newExpense, stepSize);
			m_monthlyExpenses[expenseType]->SetCurrentMonthlySpending(newExpense);
		}

		// Recalculate the monthly operation expenses
		CalculateMonthlyOperationExpenses();
	}
}

void UCCompany::CapNetIncomeChange(float aPrevMonthNetIncome)
{
	if (m_isNetIncomeChangeCapped)
	{
		// Calculate how much the net income changed
		float netIncomeChange = m_currentFinancialData.m_netIncome - aPrevMonthNetIncome;
		float maxChange = FMath::Abs(aPrevMonthNetIncome * m_maxNetIncomePercentChange);

		// Always allow a minimum amount of change, to avoid
		// getting stuck in a loop of small changes
		if (maxChange < m_cappedNetIncomeMinimumChange)
		{
			maxChange = m_cappedNetIncomeMinimumChange;
		}

		// If the change is bigger than the max change, cap it
		if (FMath::Abs(netIncomeChange) >= maxChange)
		{
			// Cap the net income change
			if (netIncomeChange < 0.0f)
			{
				m_currentFinancialData.m_netIncome = aPrevMonthNetIncome - maxChange;
			}
			else if (netIncomeChange > 0.0f)
			{
				m_currentFinancialData.m_netIncome = aPrevMonthNetIncome + maxChange;
			}
		}
	}
}

void UCCompany::CalculateCompanyStockValue()
{
	// Apply the marketing bonus to the stock value
	float marketinBonusToStockValue = m_monthlyExpenses[ECompanyMonthyExpenseType::Marketing]->GetCurrentBonus();
	if (marketinBonusToStockValue > 0.0f)
	{
		// If there is a negative net income in the previous month
		// simulate a reaction from the stock value
		if (m_currentFinancialData.m_netIncome < 0.0f)
		{
			// Reduce the effect of the bonus
			marketinBonusToStockValue *= m_negativeNetIncomeMarketingBonusRate;
		}

		// Apply the marketing bonus
		m_currentFinancialData.m_stockValue += m_currentFinancialData.m_stockValue * marketinBonusToStockValue;

		// Cap the max value the stock can have
		m_currentFinancialData.m_stockValue = FMath::Min(m_currentFinancialData.m_stockValue, m_maxStockValue);
	}
}



void UCCompany::TryAllocateMonthlyExpense(int64& aMonthlyExpense, int64 aAmountToAllocate, float aStepSize)
{
	// Always allow allocating an amount of 0
	if (aAmountToAllocate == 0)
	{
		aMonthlyExpense = 0;
		return;
	}

	// Get how much the budget can be increased
	int64 availableFunds = GetMonthlyBudgetAvailable();

	// If there is no funds available, it means that we reducing the budget allocation
	// so just consider the current allocations
	bool isAllocatingOverBudget = availableFunds < 0;

	// Get how much of the budget can be allocated
	int64 budgetSpare = 0;
	if (isAllocatingOverBudget == false)
	{
		// Deducting the current budget for the already allocated expense, get the overall amount that can be spend
		budgetSpare = availableFunds + aMonthlyExpense;
	}
	else
	{
		// Only consider the need to reduce the current expense
		budgetSpare = aMonthlyExpense;
	}

	// If the next step goes over budget, just assign whatever budget amount is left
	if (aAmountToAllocate + aStepSize > budgetSpare)
	{
		aAmountToAllocate = budgetSpare;
	}
	// Snap the value to the previous or to the next step
	else
	{
		// Snap the value to the nearest step size
		float snappedValue = FMath::GridSnap(static_cast<float>(aAmountToAllocate), aStepSize);
		aAmountToAllocate = static_cast<int64>(snappedValue);
	}

	// Check that the amount to allocate is valid
	if (aAmountToAllocate < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("The expense amount %lld is negative, capping the value to 0"), aAmountToAllocate);
		aAmountToAllocate = 0;
	}

	// Cap at 0
	if (budgetSpare <= 0)
	{
		aMonthlyExpense = 0;
	}
	// Cap the budget to highest amount possible
	else if (aAmountToAllocate > budgetSpare && isAllocatingOverBudget == false)
	{
		aMonthlyExpense = budgetSpare;
	}
	// Cap the budget to an amount lower that the current expense
	else if (aAmountToAllocate > budgetSpare && isAllocatingOverBudget == true)
	{
		// Round the budget spare down to the nearest multiple of aStepSize
		// The rounding down may cause the budget not to be precise, but better to
		// be under budget than over budget
		float roundedDivision = static_cast<float>(budgetSpare) / aStepSize;
		int64 roundedBudgetSpare = static_cast<int64>(FMath::FloorToInt(roundedDivision)) * aStepSize;
		aMonthlyExpense = roundedBudgetSpare;
	}
	// Assign the amount without any restrictions
	else
	{
		aMonthlyExpense = aAmountToAllocate;
	}
}
