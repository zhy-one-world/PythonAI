// Copyright Epic Games, Inc. All Rights Reserved.

#include "PythonAIGameMode.h"
#include "PythonAICharacter.h"
#include "UObject/ConstructorHelpers.h"

APythonAIGameMode::APythonAIGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
