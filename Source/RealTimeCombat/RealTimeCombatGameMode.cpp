// Copyright Epic Games, Inc. All Rights Reserved.

#include "RealTimeCombatGameMode.h"
#include "RealTimeCombatCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARealTimeCombatGameMode::ARealTimeCombatGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
