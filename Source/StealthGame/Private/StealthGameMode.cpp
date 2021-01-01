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

void AStealthGameMode::CompleteMission(bool bSuccess)
{
	ChangeViewTargetOnMissionComplete();

	AStealthGameState* GS = GetGameState<AStealthGameState>();
	if (GS)
	{
		GS->NetMulticastCompleteMission(bSuccess);
	}
	
}

void AStealthGameMode::ChangeViewTargetOnMissionComplete() const
{
	AActor* NewViewTarget = GetViewTarget();
	
	SetNewViewTargetForAllPlayerControllers(NewViewTarget);
}

AActor* AStealthGameMode::GetViewTarget() const
{
	if (!ViewTargetActorClass)
		return nullptr;
	
	TArray<AActor*> ViewTargets;
	UGameplayStatics::GetAllActorsOfClass(this, ViewTargetActorClass, ViewTargets);

	if (ViewTargets.Num() > 0)
		return ViewTargets[0];
	
	
	return nullptr;
}

void AStealthGameMode::SetNewViewTargetForAllPlayerControllers(AActor* NewViewTarget) const
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (APlayerController* PC = It->Get())
		{
			PC->SetViewTargetWithBlend(NewViewTarget, 0.5, EViewTargetBlendFunction::VTBlend_Cubic);
		}	
	}
}
