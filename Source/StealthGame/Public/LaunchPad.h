#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaunchPad.generated.h"

class UBoxComponent;
class UArrowComponent;
class UStaticMeshComponent;

UCLASS()
class STEALTHGAME_API ALaunchPad : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, Category=Base)
	UStaticMeshComponent* LaunchPadBase;

	UPROPERTY(VisibleDefaultsOnly, Category=Arrow)
	UStaticMeshComponent* LaunchPadArrow;

	UPROPERTY(VisibleDefaultsOnly, Category=OverlapDetection)
	UBoxComponent* OverlapBoxComponent;

	UPROPERTY(EditAnywhere, Category=LaunchParams)
	float CharacterLaunchForce = 1500.0f;

	UPROPERTY(EditAnywhere, Category=LaunchParams)
	float PhysicsBodyLaunchForce = 100000.0f;

	UPROPERTY(EditAnywhere, Category=LaunchParams)
	float LaunchPitchAngle = 35.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Effects)
	UParticleSystem* LaunchEffect;
	
public:	
	ALaunchPad();

protected:
	UFUNCTION()
    void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult & SweepResult);

};
