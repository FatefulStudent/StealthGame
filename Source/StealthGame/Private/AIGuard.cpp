#include "AIGuard.h"


#include "DrawDebugHelpers.h"
#include "Perception/PawnSensingComponent.h"

AAIGuard::AAIGuard()
{
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	
}

void AAIGuard::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AAIGuard::OnSeePawn);
	PawnSensingComponent->OnHearNoise.AddDynamic(this, &AAIGuard::OnHearNoise);

	OriginalRotation = GetActorRotation();
}

void AAIGuard::OnSeePawn(APawn* Pawn)
{
	if (!Pawn)
		return;
	
	DrawDebugSphere(GetWorld(),
		Pawn->GetActorLocation(), 
		32.0f,
		10,
		FColor::Red,
		false,
		5.0f);
}

void AAIGuard::LookAtNoiseDistraction(const FVector& Location)
{
	const FVector TargetDirection = Location - GetActorLocation();
	const float DesiredYaw = FRotationMatrix::MakeFromX(TargetDirection).Rotator().Yaw;
	const FRotator NewRotation { GetActorRotation().Pitch, DesiredYaw, GetActorRotation().Pitch };
	SetActorRotation(NewRotation);

	auto RevertToOriginalRotation = [this]() { SetActorRotation(OriginalRotation); };
	
	GetWorldTimerManager().SetTimer(
		RevertToOriginalRotationTimerHandle,
		RevertToOriginalRotation,
		RevertToOriginalRotationTimer,
		false);
}

void AAIGuard::OnHearNoise(APawn* NoiseInstigator, const FVector& NoiseLocation, float Volume)
{
	if (!PawnSensingComponent->bSeePawns)
	LookAtNoiseDistraction(NoiseLocation);
	
	DrawDebugSphere(GetWorld(),
	    NoiseLocation,
	    32.0f,
	    10,
	    FColor::Green,
	    false,
	    5.0f);
}

void AAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
