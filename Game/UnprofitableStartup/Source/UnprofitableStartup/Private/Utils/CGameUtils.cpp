// Alvaro Antonio Chavez Mixco (w18367131), 6CCGD011W Computer Games Development Final Project 


#include "Utils/CGameUtils.h"
#include <Kismet/GameplayStatics.h>

void UCGameUtils::ReloadCurrentLevel(const UObject* aWorldContextObject)
{
	if (IsValid(aWorldContextObject))
	{
		// Get the current level name path as "Game/Levels/LevelName"
		FName levelName = FName(UGameplayStatics::GetCurrentLevelName(aWorldContextObject, true));

		// Reload the current level
		UGameplayStatics::OpenLevel(aWorldContextObject, levelName, true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot reload the current level, the world context object is invalid"));
	}
}