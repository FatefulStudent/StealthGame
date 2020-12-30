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

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	USoundBase* ExtractionFailedSound;
	
public:	
	AExtractionZone();
	virtual void PostInitializeComponents() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
protected:
	void OnExtractionComplete(AStealthCharacter* StealthCharacter);
	void OnExtractionFailed(AStealthCharacter* StealthCharacter);
};
