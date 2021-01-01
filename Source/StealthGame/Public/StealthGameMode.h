// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StealthGameMode.generated.h"

UCLASS()
class AStealthGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category=Extraction)
	TSubclassOf<AActor> ViewTargetActorClass;
	
public:
	AStealthGameMode();
	
	void CompleteMission(bool bSuccess);

	void ChangeViewTargetOnMissionComplete() const;

private:
	AActor* GetViewTarget() const;
	void SetNewViewTargetForAllPlayerControllers(AActor* NewViewTarget) const;
};
