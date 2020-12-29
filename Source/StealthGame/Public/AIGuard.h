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

	UPROPERTY(EditDefaultsOnly, Category=Perception)
	float RevertToOriginalRotationTimer = 3.0f;

public:
	AAIGuard();

protected:
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
    void OnSeePawn(APawn* Pawn);
	void LookAtNoiseDistraction(const FVector& Location);

	UFUNCTION()
    void OnHearNoise(APawn* NoiseInstigator, const FVector& NoiseLocation, float Volume);

private:
	FRotator OriginalRotation;
	FTimerHandle RevertToOriginalRotationTimerHandle;
};
