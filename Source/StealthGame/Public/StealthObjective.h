#pragma once

#include "Interfaces/Interactive.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "StealthObjective.generated.h"

class USphereComponent;

UCLASS()
class STEALTHGAME_API AStealthObjective : public AActor, public IInteractiveInterface
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

	void StartPlayingStationaryEffects();

protected:
	// AActor overrides
	virtual void BeginPlay() override;
	// ~AActor overrides

	// IInteractiveInterface overrides
	virtual bool IsAvailableForInteraction() const override;
	virtual void OnSuccessfulInteraction() override;
	// ~IInteractiveInterface overrides
	
private:	
	UPROPERTY()
	UParticleSystemComponent* CachedStationaryEffects;

};
