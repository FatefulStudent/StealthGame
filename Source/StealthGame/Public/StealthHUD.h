// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "StealthHUD.generated.h"

class UTexture2D;

UCLASS()
class AStealthHUD : public AHUD
{
	GENERATED_BODY()

protected:
	/** Crosshair texture */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* CrosshairTex;

public:

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

};

