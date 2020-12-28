// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "StealthGameMode.h"
#include "StealthHUD.h"
#include "Kismet/GameplayStatics.h"

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
		
		ChangeViewTargetOnMissionComplete(InstigatorPawn);
		
		OnMissionCompleted(InstigatorPawn);
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

