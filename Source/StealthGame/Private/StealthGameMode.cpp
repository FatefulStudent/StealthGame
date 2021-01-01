// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "StealthGameMode.h"
#include "StealthGameState.h"
#include "StealthHUD.h"

#include "Kismet/GameplayStatics.h"

AStealthGameMode::AStealthGameMode()
{
	HUDClass = AStealthHUD::StaticClass();
	GameStateClass = AStealthGameState::StaticClass();
}

void AStealthGameMode::CompleteMission(APawn* InstigatorPawn, bool bSuccess)
{
	if (IsValid(InstigatorPawn))
	{
		ChangeViewTargetOnMissionComplete(InstigatorPawn);
		
		OnMissionCompleted(InstigatorPawn, bSuccess);
	}

	AStealthGameState* GS = GetGameState<AStealthGameState>();
	if (GS)
	{
		GS->NetMulticastCompleteMission(InstigatorPawn, bSuccess);
	}
	
}

void AStealthGameMode::ChangeViewTargetOnMissionComplete(APawn* InstigatorPawn) const
{
	if (!ViewTargetActorClass)
		return;
	
	TArray<AActor*> ViewTargets;
	UGameplayStatics::GetAllActorsOfClass(this, ViewTargetActorClass, ViewTargets);

	if (ViewTargets.Num() > 0)
	{
		AActor* NewViewTarget = ViewTargets[0];
		if (auto PC = Cast<APlayerController>(InstigatorPawn->Controller))
		{
			PC->SetViewTargetWithBlend(NewViewTarget, 0.5, EViewTargetBlendFunction::VTBlend_Cubic);
		}
	}
}

