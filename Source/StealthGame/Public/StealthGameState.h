// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "StealthGameState.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHGAME_API AStealthGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastCompleteMission(APawn* InstigatorPawn, bool bSuccess);
};
