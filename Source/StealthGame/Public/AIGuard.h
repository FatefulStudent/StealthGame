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
	float AcceptanceRadius2D = 50.0f;

public:
	AAIGuard();

protected:
	// AActor overrides
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void GetLifetimeReplicatedProps (TArray <FLifetimeProperty> & OutLifetimeProps) const override;
	// ~AActor overrides
		
	UFUNCTION(BlueprintImplementableEvent, Category="StateChange")
    void OnStateChange(EGuardState NewState);
	
	UFUNCTION()
    void OnSeePawn(APawn* Pawn);
	
	UFUNCTION()
    void OnHearNoise(APawn* NoiseInstigator, const FVector& NoiseLocation, float Volume);

private:
	UPROPERTY(ReplicatedUsing=OnRep_GuardState)
	EGuardState GuardState = EGuardState::Idle;
	
	FRotator OriginalRotation = FRotator::ZeroRotator;
	FTimerHandle RevertToOriginalRotationTimerHandle;

	UPROPERTY() ATargetPoint* CurrentTargetPoint;
	int32 CurrentTargetPointIndex = INDEX_NONE;
	
	void StartMovingToOtherTargetPointIfNeeded();
	void TryStartPatrolling();
	void UpdateTargetPoint();
	void RotateTowardsLocation(const FVector& Location);
	void LookAtNoiseDistraction(const FVector& Location);
	void ChangeState(EGuardState NewState);
	void RevertToOriginalRotation();

	UFUNCTION()
	void OnRep_GuardState();
};
