// Fill out your copyright notice in the Description page of Project Settings.


#include "StealthGameState.h"

void AStealthGameState::NetMulticastCompleteMission_Implementation(bool bSuccess)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (APlayerController* PC = It->Get())
		{
			PC->DisableInput(nullptr);
			if (APawn* Pawn = PC->GetPawn())
				Pawn->DisableInput(nullptr);
		}	
	}

	OnMissionCompleted(bSuccess);
}
