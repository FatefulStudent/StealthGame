#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "AIGuard.generated.h"

class UPawnSensingComponent;

UENUM()
enum class EGuardState : uint8
{
	Idle,
	Suspicious,
	Alerted
};

UCLASS()
class STEALTHGAME_API AAIGuard : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, Category=Perception)
	UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(EditDefaultsOnly, Category=Perception)
	float RevertToOriginalRotationTimer = 3.0f;

	EGuardState GuardState = EGuardState::Idle;

public:
	AAIGuard();

protected:
	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintImplementableEvent, Category="StateChange")
    void OnStateChange(EGuardState NewState);
	
	UFUNCTION()
    void OnSeePawn(APawn* Pawn);
	
	UFUNCTION()
    void OnHearNoise(APawn* NoiseInstigator, const FVector& NoiseLocation, float Volume);
	
	void LookAtNoiseDistraction(const FVector& Location);
	void ChangeState(EGuardState NewState);
	void RevertToOriginalRotation();
	
private:
	FRotator OriginalRotation;
	FTimerHandle RevertToOriginalRotationTimerHandle;
};
