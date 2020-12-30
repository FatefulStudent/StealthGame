// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "StealthCharacter.h"
#include "NetworkingHelper.h"
#include "StealthObjective.h"
#include "StealthProjectile.h"

#include "GeneratedCodeHelpers.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimSequence.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Kismet/GameplayStatics.h"


DEFINE_LOG_CATEGORY(LogStealthCharacter);

AStealthCharacter::AStealthCharacter()
{
	SetReplicates(true);
	SetReplicateMovement(true);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	// Create a CameraComponent	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->SetRelativeLocation(FVector(0, 0, BaseEyeHeight)); // Position the camera
	CameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	CharacterMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	CharacterMeshComponent->SetupAttachment(CameraComponent);
	CharacterMeshComponent->CastShadow = false;
	CharacterMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CharacterMeshComponent->SetRelativeRotation(FRotator(2.0f, -15.0f, 5.0f));
	CharacterMeshComponent->SetRelativeLocation(FVector(0, 0, -160.0f));

	// Create a gun mesh component
	GunMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	GunMeshComponent->CastShadow = false;
	GunMeshComponent->SetupAttachment(CharacterMeshComponent, "GripPoint");

	NoiseEmitterComponent = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("NoiseEmitter"));
}

void AStealthCharacter::PlaySoundOnAutonomousClients(USoundBase* SoundToPlay)
{
	if (IsLocallyControlled())
		UGameplayStatics::PlaySound2D(this, SoundToPlay);
	else
		ClientPlaySound(SoundToPlay);
}

void AStealthCharacter::ClientPlaySound_Implementation(USoundBase* SoundToPlay)
{
	UGameplayStatics::PlaySound2D(this, SoundToPlay);
}

void AStealthCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Interaction happens only on server
	if (!HasAuthority())
	{
		GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	}
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

void AStealthCharacter::BeginPlay()
{
	Super::BeginPlay();

	// For non locally controlled we need to use tick
	if (!IsLocallyControlled())
	{
		SetActorTickEnabled(true);
	}
	else
	{
		SetActorTickEnabled(false);
	}
}

void AStealthCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	ensure(!IsLocallyControlled());

	CameraComponent->SetWorldRotation(GetBaseAimRotation());
}

void AStealthCharacter::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (ensureAlways(HasAuthority()))
		TryInteractingWith(OtherActor);
}

void AStealthCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AStealthCharacter, bCarriesObjective);
}

void AStealthCharacter::TryInteractingWith(AActor* OtherActor)
{
	check(HasAuthority());
	
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
	check(HasAuthority());
	
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

void AStealthCharacter::ServerSpawnProjectileAndEffects_Implementation()
{
	SpawnProjectileAndEffects();
}

bool AStealthCharacter::ServerSpawnProjectileAndEffects_Validate()
{
	return true;
}

void AStealthCharacter::SpawnProjectile()
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
}

void AStealthCharacter::SpawnProjectileAndEffects()
{
	check(HasAuthority());
	
	SpawnProjectile();

	NetMulticastPlayEffectsOnProjectileSpawn();
}

void AStealthCharacter::PlayEffectsOnProjectileSpawn() const
{
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

void AStealthCharacter::NetMulticastPlayEffectsOnProjectileSpawn_Implementation()
{
	if (FNetworkingHelper::HasCosmetics(this))
		PlayEffectsOnProjectileSpawn();
}

void AStealthCharacter::Fire()
{
	if (HasAuthority())
		SpawnProjectileAndEffects();
	else if (FNetworkingHelper::IsAutonomousClient(this))
		ServerSpawnProjectileAndEffects();
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
