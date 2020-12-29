#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "AIGuard.generated.h"

class ATargetPoint;
class UNavigationPath;
class UPawnSensingComponent;

UENUM()
enum class EGuardState : uint8
{
	Idle,
	Patrolling,
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

	UPROPERTY(EditInstanceOnly, Category=Patrolling)
	TArray<ATargetPoint*> PatrollingTargetPoints;

	UPROPERTY(EditDefaultsOnly, Category=Patrolling)
	float PatrollingSpeed = 300.0f;
	
	UPROPERTY(EditDefaultsOnly, Category=Patrolling)
	float AcceptanceRadius2D = 100.0f;

public:
	AAIGuard();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintImplementableEvent, Category="StateChange")
    void OnStateChange(EGuardState NewState);
	
	UFUNCTION()
    void OnSeePawn(APawn* Pawn);
	
	UFUNCTION()
    void OnHearNoise(APawn* NoiseInstigator, const FVector& NoiseLocation, float Volume);

private:
	EGuardState GuardState = EGuardState::Idle;
	FRotator OriginalRotation = FRotator::ZeroRotator;
	FTimerHandle RevertToOriginalRotationTimerHandle;

	UPROPERTY() ATargetPoint* CurrentTargetPoint;
	int32 CurrentTargetPointIndex = INDEX_NONE;

	void TryStartPatrolling();
	void UpdateTargetPoint();
	void TickPatrolling(float DeltaSeconds);
	void RotateTowardsLocation(const FVector& Location);
	void MoveTowardsLocation(const FVector& Location, float DeltaSeconds);
	void LookAtNoiseDistraction(const FVector& Location);
	void ChangeState(EGuardState NewState);
	void RevertToOriginalRotation();

};
