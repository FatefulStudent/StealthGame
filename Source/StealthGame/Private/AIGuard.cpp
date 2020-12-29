#include "AIGuard.h"
#include "StealthGameMode.h"

#include "DrawDebugHelpers.h"
#include "Perception/PawnSensingComponent.h"

AAIGuard::AAIGuard()
{
	PrimaryActorTick.bCanEverTick = false;

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

	ChangeState(EGuardState::Alerted);
	
	if (auto StealthGameMode = Cast<AStealthGameMode>(GetWorld()->GetAuthGameMode()))
		StealthGameMode->CompleteMission(Pawn, false);
	
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
	
	GetWorldTimerManager().SetTimer(
		RevertToOriginalRotationTimerHandle,
		this,
		&AAIGuard::RevertToOriginalRotation,
		RevertToOriginalRotationTimer,
		false);
}

void AAIGuard::OnHearNoise(APawn* NoiseInstigator, const FVector& NoiseLocation, float Volume)
{
	if (GuardState == EGuardState::Alerted)
		return;
	
	ChangeState(EGuardState::Suspicious);
	
	LookAtNoiseDistraction(NoiseLocation);
	
	DrawDebugSphere(GetWorld(),
	    NoiseLocation,
	    32.0f,
	    10,
	    FColor::Green,
	    false,
	    5.0f);
}

void AAIGuard::ChangeState(EGuardState NewState)
{
	if (GuardState == NewState)
		return;

	GuardState = NewState;

	OnStateChange(GuardState);
}

void AAIGuard::RevertToOriginalRotation()
{
	if (GuardState == EGuardState::Alerted)
		return;

	SetActorRotation(OriginalRotation);
	ChangeState(EGuardState::Idle);
}
