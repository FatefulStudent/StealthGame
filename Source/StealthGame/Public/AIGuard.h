#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "AIGuard.generated.h"

class UPawnSensingComponent;

UCLASS()
class STEALTHGAME_API AAIGuard : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, Category=Perception)
	UPawnSensingComponent* PawnSensingComponent;

public:
	AAIGuard();

protected:
	virtual void PostInitializeComponents() override;
	
	UFUNCTION()
    void OnSeePawn(APawn* Pawn);
	
	UFUNCTION()
    void OnHearNoise(APawn* NoiseInstigator, const FVector& Location, float Volume);

public:	
	virtual void Tick(float DeltaTime) override;
};
