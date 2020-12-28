// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StealthGameMode.generated.h"

UCLASS()
class AStealthGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AStealthGameMode();

	void CompleteMission(APawn* InstigatorPawn);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnMissionCompleted(APawn* InstigatorPawn);
};



