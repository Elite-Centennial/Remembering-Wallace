// Copyright Epic Games, Inc. All Rights Reserved.

#include "RememberingWallaceGameMode.h"
#include "RememberingWallaceCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARememberingWallaceGameMode::ARememberingWallaceGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
