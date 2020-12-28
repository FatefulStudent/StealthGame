#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlackHole.generated.h"

class USphereComponent;
class URadialForceComponent;

UCLASS()
class STEALTHGAME_API ABlackHole : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Mesh)
	UStaticMeshComponent* StaticMeshComponent;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Collision)
	USphereComponent* DestructingSphereComponent;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Collision)
	USphereComponent* AttractingSphereComponent;

	float DestructingSphereDefaultRadius = 100.0f;
	float AttractingSphereDefaultRadius = 10000.0f;
	float AttractingForce = -1000000.0f;
	
public:	
	ABlackHole();
	virtual void Tick(float DeltaSeconds) override;
	
private:

	void ApplyRadialForceOnOverlappedComponents() const;
	
	UFUNCTION()
	void OnDestructionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult & SweepResult);

	
};
