// Alvaro Antonio Chavez Mixco (w18367131), 6CCGD011W Computer Games Development Final Project 


#include "Business/CMonthlyExpenseInvestment.h"
#include "SInvestmentLevel.h"

UCMonthlyExpenseInvestment::UCMonthlyExpenseInvestment()
	: m_currentMonthlySpending(0)
	, m_historicalSpent(0)
	, m_currentInvestmentLevel(0)
{
}

void UCMonthlyExpenseInvestment::Initialise(int64 aMonthlySpending, TArray<FSInvestmentLevel> aInvestmentLevels)
{
	// Set the current monthly spending
	SetCurrentMonthlySpending(aMonthlySpending);

	// Add the investment levels
	AddInvestmentLevels(aInvestmentLevels);
}

void UCMonthlyExpenseInvestment::AddInvestmentLevels(TArray<FSInvestmentLevel> aInvestmentLevels)
{
	// Add the new investment levels to the existing ones
	m_investmentLevels.Append(aInvestmentLevels);

	// Sort the investment levels in ascending order, per required spending
	m_investmentLevels.Sort();

	// Recalculate the current investment level
	m_currentInvestmentLevel = 0;
	CalculateInvestmentLevel();
}



#pragma region GettersAndSetters
void UCMonthlyExpenseInvestment::SetCurrentMonthlySpending(int64 aCurrentMonthlySpending)
{
	m_currentMonthlySpending = aCurrentMonthlySpending;
}

int64 UCMonthlyExpenseInvestment::GetCurrentMonthlySpending() const
{
	return m_currentMonthlySpending;
}

float UCMonthlyExpenseInvestment::GetCurrentBonus() const
{
	// The bonus corresponds to the investment level that has been passed, not
	// the  one that is currently being calculated.
	// So if none has been passed, return no bonus
	if (m_currentInvestmentLevel == 0 || m_investmentLevels.Num() == 0)
	{
		return 0.0f;
	}
	// If at the max investment level, return the max bonus
	else if (m_currentInvestmentLevel - 1 > m_investmentLevels.Num())
	{
		return m_investmentLevels[m_investmentLevels.Num() - 1].m_levelBonus;
	}
	// Return the bonus of the investment level for which the spending has been surpassed
	else
	{
		return m_investmentLevels[m_currentInvestmentLevel - 1].m_levelBonus;
	}
}

uint8 UCMonthlyExpenseInvestment::GetCurrentInvestmentLevel() const
{
	return m_currentInvestmentLevel;
}

int64 UCMonthlyExpenseInvestment::GetHistoricalSpent() const
{
	return m_historicalSpent;
}

int64 UCMonthlyExpenseInvestment::GetSpendingLeftForNextInvestmentLevel() const
{
	// The current investment level is the one that
	// has currently been reached, so the next one is the current + 1
	int64 requiredSpendingNextLevel = 0;
	if (m_currentInvestmentLevel < m_investmentLevels.Num())
	{
		requiredSpendingNextLevel = m_investmentLevels[m_currentInvestmentLevel].m_requiredSpending;
		return requiredSpendingNextLevel - m_historicalSpent;
	}

	// If all the investment levels have been reached, 
	// or there are no investment levels, return 0
	return requiredSpendingNextLevel;
}
#pragma endregion GettersAndSetters



void UCMonthlyExpenseInvestment::AdvanceMonth()
{
	// Increase the historical spending
	m_historicalSpent += m_currentMonthlySpending;

	// With the new spending, calculate the investment level
	CalculateInvestmentLevel();
}

void UCMonthlyExpenseInvestment::CalculateInvestmentLevel()
{	
	// If there are more investment levels to go through
	if (m_currentInvestmentLevel < m_investmentLevels.Num())
	{
		// This works with the expectation that the investment levels are sorted in ascending order
		if (m_historicalSpent >= m_investmentLevels[m_currentInvestmentLevel].m_requiredSpending)
		{
			// May need actual UI improvement, but for now, just log the level reached
			UE_LOG(LogTemp, Log, TEXT("The investment level %d has been reached with a spending of %lld to fulfill the required spending of %lld"),
								m_currentInvestmentLevel, m_historicalSpent, m_investmentLevels[m_currentInvestmentLevel].m_requiredSpending);

			// Increase the investment level
			m_currentInvestmentLevel++;

			// Recursively calculate the investment level, in case that with the current spending multiple levels may be reached
			CalculateInvestmentLevel();
		}
	}
}
