// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "TrainingGrounds.h"
#include "TrainingGroundsGameMode.h"
#include "TrainingGroundsHUD.h"
#include "Player/FirstPersonCharacter.h"

ATrainingGroundsGameMode::ATrainingGroundsGameMode()
	: Super()
{
	// This is standard and not needed. Also, made the editor hang on loading because of apparently some kind of known problem.
	// Commenting it out for now.

	//// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Dynamic/Character/Behaviour/BP_Character"));
	//DefaultPawnClass = PlayerPawnClassFinder.Class;

	//// use our custom HUD class
	//HUDClass = ATrainingGroundsHUD::StaticClass();
}
