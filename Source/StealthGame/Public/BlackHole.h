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
	URadialForceComponent* AttractingRadialForceComponent;
	
public:	
	ABlackHole();

protected:	
	virtual void Tick(float DeltaTime) override;

private:
	void DestroyObjectsInDestructionSphere() const;
};
