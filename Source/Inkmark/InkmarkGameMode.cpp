// Copyright Epic Games, Inc. All Rights Reserved.

#include "InkmarkGameMode.h"
#include "InkmarkCharacter.h"
#include "UObject/ConstructorHelpers.h"

AInkmarkGameMode::AInkmarkGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
