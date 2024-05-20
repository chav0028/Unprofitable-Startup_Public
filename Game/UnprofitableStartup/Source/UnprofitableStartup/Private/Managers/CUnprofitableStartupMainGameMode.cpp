// Alvaro Antonio Chavez Mixco (w18367131), 6CCGD011W Computer Games Development Final Project 


#include "Managers/CUnprofitableStartupMainGameMode.h"

#include "Utils/CDateUtils.h"
#include "Business/CCompany.h"

ACUnprofitableStartupMainGameMode::ACUnprofitableStartupMainGameMode() :
	m_playerCompany(nullptr),
	m_maxNumberOfTurns(60),
	m_winNetIncome(500000),
	m_winCashAvailable(5000000),
	m_loseCashAvailable(0),
	m_currentTurnNumber(1),
	m_currentDate(2024, 1, 1),
	m_currentGameCondition(EEndGameCondition::GameActive),
	m_continuePlayingAfterEnd(false)
{
	// Setup the start date
	m_startDate = m_currentDate;
}

void ACUnprofitableStartupMainGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	AGameModeBase::InitGame(MapName, Options, ErrorMessage);

	// Initialise the Player Company in InitGame, since initialising
	// it in constructor does not seem to properly set it up when using
	// in a Blueprint Child class Begin Play
	m_playerCompany = NewObject<UCCompany>();
}

void ACUnprofitableStartupMainGameMode::EndTurn()
{
	// Simulate the company's next month
	if (IsValid(m_playerCompany))
	{
		m_playerCompany->SimulateMonth();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("The Player Company is not set in the main game mode"));
	}

	// Advance current turn
	m_currentTurnNumber++;

	// Advance the date to the next month
	m_currentDate = UCDateUtils::AdvanceMonth(m_currentDate);

	// Raise event that the current turn ended
	OnTurnEnded.Broadcast(this);

	// Check if the game has ended
	CheckEndGameConditions();
}

UCCompany* ACUnprofitableStartupMainGameMode::GetPlayerCompany() const
{
	return m_playerCompany;
}

FDateTime ACUnprofitableStartupMainGameMode::GetStartDate() const
{
	return m_startDate;
}

void ACUnprofitableStartupMainGameMode::CheckEndGameConditions()
{
	if ((m_currentGameCondition == EEndGameCondition::GameActive || m_continuePlayingAfterEnd)
		&& m_playerCompany != nullptr)
	{
		// Check which end condition has been met.
		// If the end of the game has been reached, and the player decides to continue playing,
		// only the lose condition can be retriggered
		EEndGameCondition prevGameCondition = m_currentGameCondition;
		int64 cashAvailable = m_playerCompany->GetCashAvailable();
		if (m_currentTurnNumber >= m_maxNumberOfTurns && m_continuePlayingAfterEnd == false)
		{
			m_currentGameCondition = EEndGameCondition::MaxTurns;
		}
		else if (m_playerCompany->GetNetIncome() >= m_winNetIncome &&
			cashAvailable >= m_winCashAvailable &&
			m_continuePlayingAfterEnd == false)
		{
			m_currentGameCondition = EEndGameCondition::Profitability;
		}
		else if (cashAvailable <= m_loseCashAvailable)
		{
			m_currentGameCondition = EEndGameCondition::OutOfCash;
		}

		if (m_currentGameCondition != prevGameCondition)
		{
			OnEndGameConditionChanged.Broadcast(m_currentGameCondition);

			// Mark the end of the game has been reached
			m_continuePlayingAfterEnd = true;
		}
	}
}
