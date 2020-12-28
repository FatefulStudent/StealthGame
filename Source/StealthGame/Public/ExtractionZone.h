#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExtractionZone.generated.h"

class UBoxComponent;
class AStealthCharacter;

UCLASS()
class STEALTHGAME_API AExtractionZone : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Collision")
	UBoxComponent* OverlapBoxComponent;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Effects")
	UDecalComponent* OverlapDecalComponent;
	
public:	
	AExtractionZone();
	virtual void BeginPlay() override;
protected:
	
	UFUNCTION()
	void OnOverlapWithPawn(UPrimitiveComponent* OverlappedComponent,
	    AActor* OtherActor,
	    UPrimitiveComponent* OtherComp,
	    int32 OtherBodyIndex,
	    bool bFromSweep,
	    const FHitResult & SweepResult);

	void OnExtractionComplete(AStealthCharacter* StealthCharacter);
};
