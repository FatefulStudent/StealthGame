// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Interfaces/Interactor.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "StealthCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class AStealthProjectile;
class USoundBase;
class UAnimSequence;

DECLARE_LOG_CATEGORY_EXTERN(LogStealthCharacter, Log, All);

UCLASS()
class AStealthCharacter : public ACharacter, public IInteractorInterface
{
	GENERATED_BODY()

protected:

	// Pawn mesh: 1st person view.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Mesh")
	USkeletalMeshComponent* CharacterMeshComponent;

	// Gun mesh: 1st person view (seen only by self).
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* GunMeshComponent;

	// First person camera.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComponent;
	
	// Whether our character carries an objective.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ObjectiveBehaviour")
	bool bCarriesObjective = false;

	// Projectile class to spawn.
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	TSubclassOf<AStealthProjectile> ProjectileClass;

	// Sound to play each time we fire.
	UPROPERTY(EditDefaultsOnly, Category="Gun|Cocmetic")
	USoundBase* FireSound;

	// AnimMontage to play each time we fire.
	UPROPERTY(EditDefaultsOnly, Category = "Gun|Cocmetic")
	UAnimSequence* FireAnimation;

public:
	// Returns character mesh.
	USkeletalMeshComponent* GetCharacterMesh() const { return CharacterMeshComponent; }

	// Returns FirstPersonCameraComponent subobject.
	UCameraComponent* GetFirstPersonCameraComponent() const { return CameraComponent; }
	
	AStealthCharacter();

	
protected:

	// AActor overrides
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	void TryInteractingWith(AActor* OtherActor);
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	// ~AActor overrides
	
	// IInteractorInterface overrides
	virtual bool WantToInteract(IInteractiveInterface* Interactive) const override;
	virtual void Interact(IInteractiveInterface* Interactive) override;
	// ~IInteractorInterface overrides
	
	// Called when objective is picked up.
	void OnPickUpObjective();
	
	// Fires a projectile.
	void Fire();

	// Handles moving forward/backward.
	void MoveForward(float Val);

	// Handles strafing movement, left and right.
	void MoveRight(float Val);
};

