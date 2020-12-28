// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "StealthGameMode.h"
#include "StealthHUD.h"
#include "StealthCharacter.h"
#include "UObject/ConstructorHelpers.h"

AStealthGameMode::AStealthGameMode()
{
	// use our custom HUD class
	HUDClass = AStealthHUD::StaticClass();
}

void AStealthGameMode::CompleteMission(APawn* InstigatorPawn)
{
	if (IsValid(InstigatorPawn))
	{
		InstigatorPawn->DisableInput(nullptr);
		OnMissionCompleted(InstigatorPawn);
	}
}
