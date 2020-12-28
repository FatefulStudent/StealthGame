// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "StealthCharacter.h"
#include "StealthObjective.h"
#include "StealthProjectile.h"

#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimSequence.h"

DEFINE_LOG_CATEGORY(LogStealthCharacter);

AStealthCharacter::AStealthCharacter()
{
	// Create a CameraComponent	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->SetRelativeLocation(FVector(0, 0, BaseEyeHeight)); // Position the camera
	CameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	CharacterMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	CharacterMeshComponent->SetupAttachment(CameraComponent);
	CharacterMeshComponent->CastShadow = false;
	CharacterMeshComponent->SetRelativeRotation(FRotator(2.0f, -15.0f, 5.0f));
	CharacterMeshComponent->SetRelativeLocation(FVector(0, 0, -160.0f));

	// Create a gun mesh component
	GunMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	GunMeshComponent->CastShadow = false;
	GunMeshComponent->SetupAttachment(CharacterMeshComponent, "GripPoint");
}


void AStealthCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AStealthCharacter::Fire);

	PlayerInputComponent->BindAxis("MoveForward", this, &AStealthCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AStealthCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void AStealthCharacter::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	TryInteractingWith(OtherActor);
}

void AStealthCharacter::TryInteractingWith(AActor* OtherActor)
{
	if (IInteractiveInterface* OverlappedInteractive = Cast<IInteractiveInterface>(OtherActor))
	{
		if (WantToInteract(OverlappedInteractive))
		{
			Interact(OverlappedInteractive);
		}
		else
		{
			UE_LOG(LogStealthCharacter,
	            Log,
	            TEXT("%s: Tried to interact with %s, but character didn't want to interact with it."),
	            *GetName(),
	            *OtherActor->GetName())
		}
	}
	else
	{
		UE_LOG(LogStealthCharacter,
            Log,
            TEXT("%s: Overlapped with %s, but it was not an interactable."),
            *GetName(),
            *OtherActor->GetName())		
	}
	
}

bool AStealthCharacter::WantToInteract(IInteractiveInterface* Interactive) const
{
	return !bCarriesObjective;
}

void AStealthCharacter::Interact(IInteractiveInterface* Interactive)
{
	if (!ensureAlways(WantToInteract(Interactive)))
		return;
	
	if (Interactive->IsAvailableForInteraction())
	{
		if (Cast<AStealthObjective>(Interactive))
		{
			OnPickUpObjective();
			Interactive->OnSuccessfulInteraction();
		}
		else
		{
			UE_LOG(LogStealthCharacter,
                Log,
                TEXT("%s: Tried to interact with %s, but it was not an objective."),
                *GetName(),
                *Cast<UObject>(Interactive)->GetName())
		}
	}
	else
	{
		UE_LOG(LogStealthCharacter,
			Log,
			TEXT("%s: Tried to interact with %s, but it was not available for interaction."),
			*GetName(),
			*Cast<UObject>(Interactive)->GetName())
	}
}

void AStealthCharacter::OnPickUpObjective()
{
	bCarriesObjective = true;
}

void AStealthCharacter::Fire()
{
	// try and fire a projectile
	if (ProjectileClass)
	{
		// Grabs location from the mesh that must have a socket called "Muzzle" in his skeleton
		const FVector MuzzleLocation = GunMeshComponent->GetSocketLocation("Muzzle");
		// Use controller rotation which is our view direction in first person
		const FRotator& MuzzleRotation = GetControlRotation();

		//Set Spawn Collision Handling Override
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		SpawnParameters.Instigator = this;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		// spawn the projectile at the muzzle
		GetWorld()->SpawnActor<AStealthProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParameters);
	}

	// try and play the sound if specified
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = CharacterMeshComponent->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->PlaySlotAnimationAsDynamicMontage(FireAnimation, "Arms", 0.0f);
		}
	}
}


void AStealthCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}


void AStealthCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}
