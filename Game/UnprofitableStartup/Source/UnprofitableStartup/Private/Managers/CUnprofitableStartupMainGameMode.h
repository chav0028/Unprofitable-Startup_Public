// Alvaro Antonio Chavez Mixco (w18367131), 6CCGD011W Computer Games Development Final Project 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EEndGameCondition.h"
#include "CUnprofitableStartupMainGameMode.generated.h"

class UCCompany;

UDELEGATE(meta = (ToolTip = "Event raised when the current turn is ended"))
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FdelegTurnEnded, ACUnprofitableStartupMainGameMode*, aGameManager);

UDELEGATE(meta = (ToolTip = "Event raised when the game ends"))
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FdelegGameEnded, EEndGameCondition, aEndGameCondition);

UCLASS(BlueprintType, Blueprintable, // Blueprintable so that it can be inherited from a Blueprint class, allowing it to customize the Default values
	meta = (ToolTip = "Class for main game"))
class ACUnprofitableStartupMainGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACUnprofitableStartupMainGameMode();
	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	/**
	* Ends the current turn, advancing the company simulation
	*/
	UFUNCTION(BlueprintCallable, Category = "Main Game Mode")
	void EndTurn();

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Main Game Mode")
	UCCompany* GetPlayerCompany() const;

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Main Game Mode")
	FDateTime GetStartDate() const;



	UPROPERTY(BlueprintAssignable, BlueprintCallable,
		meta = (ToolTip = "Event raised when the current turn is ended"))
	FdelegTurnEnded OnTurnEnded;

	UPROPERTY(BlueprintAssignable, BlueprintCallable,
		meta = (ToolTip = "Event raised when the game ends"))
	FdelegGameEnded OnEndGameConditionChanged;



protected:
	UPROPERTY(EditAnywhere, Category = "Company",
		meta = (ToolTip = "The company being controlled by the player"))
	UCCompany* m_playerCompany;


	UPROPERTY(EditAnywhere, Category = "End Game",
		meta = (ClampMin = 1,
			ToolTip = "The number of turns required to win the game"))
	int m_maxNumberOfTurns; // Not unsigned type, since it is not supported by Blueprints

	UPROPERTY(EditAnywhere, Category = "End Game",
		meta = (ToolTip = "The net income required to win the game by profitability"))
	int64 m_winNetIncome;

	UPROPERTY(EditAnywhere, Category = "End Game",
		meta = (ToolTip = "The cash available required to win the game by profitability"))
	int64 m_winCashAvailable;

	UPROPERTY(EditAnywhere, Category = "End Game",
		meta = (ToolTip = "The minimum amount of cash the company can have before losing"))
	int64 m_loseCashAvailable;



private:
	// Check if the end of the game has been reached
	void CheckEndGameConditions();



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turns",
		meta = (AllowPrivateAccess = "true",
		ToolTip = "Current turn number in the game"))
	int m_currentTurnNumber; // Not unsigned type, since it is not supported by Blueprints

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turns",
		meta = (AllowPrivateAccess = "true",
		ToolTip = "The current date in the game"))
	FDateTime m_currentDate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turns",
		meta = (AllowPrivateAccess = "true",
			ToolTip = "The start date of the game"))
	FDateTime m_startDate;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "End Game",
		meta = (AllowPrivateAccess = "true",
			ToolTip = "The current end game condition, used to determine if the game has ended"))
	EEndGameCondition m_currentGameCondition; // Not unsigned type, since it is not supported by Blueprints

	UPROPERTY(VisibleAnywhere, Category = "End Game",
		meta = (ToolTip = "Tracks if the player has decided to continue playing after the end of the game"))
	bool m_continuePlayingAfterEnd;
};
