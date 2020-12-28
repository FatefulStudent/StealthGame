#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "StealthObjective.generated.h"

class USphereComponent;

UCLASS()
class STEALTHGAME_API AStealthObjective : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Mesh)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Collision)
	USphereComponent* CollisionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Effects)
	UParticleSystem* PickupEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Effects)
	UParticleSystem* StationaryEffect;
	
public:	
	AStealthObjective();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	void StartPlayingStationaryEffects();
	void OnPickUp() const;

protected:
	virtual void BeginPlay() override;

private:	
	UPROPERTY()
	UParticleSystemComponent* CachedStationaryEffects;

};
