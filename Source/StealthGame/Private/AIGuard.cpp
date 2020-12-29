#include "AIGuard.h"
#include "StealthGameMode.h"

#include "DrawDebugHelpers.h"
#include "Engine/TargetPoint.h"
#include "Perception/PawnSensingComponent.h"

AAIGuard::AAIGuard()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	
}

void AAIGuard::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AAIGuard::OnSeePawn);
	PawnSensingComponent->OnHearNoise.AddDynamic(this, &AAIGuard::OnHearNoise);

	OriginalRotation = GetActorRotation();
}

void AAIGuard::BeginPlay()
{
	Super::BeginPlay();

	TryStartPatrolling();
}

void AAIGuard::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GuardState == EGuardState::Patrolling)
	{
		TickPatrolling(DeltaSeconds);
	}
}

void AAIGuard::ChangeState(EGuardState NewState)
{
	if (GuardState == NewState)
		return;

	GuardState = NewState;

	OnStateChange(GuardState);
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

void AAIGuard::TryStartPatrolling()
{
	if (PatrollingTargetPoints.Num() > 0)
	{
		ChangeState(EGuardState::Patrolling);
		if (CurrentTargetPoint == nullptr)
			UpdateTargetPoint();
	}
	else
	{
		SetActorRotation(OriginalRotation);
		ChangeState(EGuardState::Idle);
	}
}

void AAIGuard::UpdateTargetPoint()
{
	CurrentTargetPointIndex = FMath::Max(0, CurrentTargetPointIndex+1);
	
	if (CurrentTargetPointIndex > PatrollingTargetPoints.Num() - 1)
		CurrentTargetPointIndex = 0;
	
	CurrentTargetPoint = PatrollingTargetPoints[CurrentTargetPointIndex];
}

void AAIGuard::TickPatrolling(float DeltaSeconds)
{
	const float DistanceToTarget2D = GetHorizontalDistanceTo(CurrentTargetPoint);
	if (DistanceToTarget2D <= AcceptanceRadius2D)
	{
		UpdateTargetPoint();
		return;
	}

	RotateTowardsLocation(CurrentTargetPoint->GetActorLocation());
	MoveTowardsLocation(CurrentTargetPoint->GetActorLocation(), DeltaSeconds);
}

void AAIGuard::RotateTowardsLocation(const FVector& Location)
{
	const FVector TargetDirection = Location - GetActorLocation();
	const float DesiredYaw = FRotationMatrix::MakeFromX(TargetDirection).Rotator().Yaw;
	const FRotator NewRotation { GetActorRotation().Pitch, DesiredYaw, GetActorRotation().Pitch };
	SetActorRotation(NewRotation);
}

void AAIGuard::MoveTowardsLocation(const FVector& Location, float DeltaSeconds)
{
	const float MaxPossibleDistance = PatrollingSpeed * DeltaSeconds;
	const FVector TargetDirection = Location - GetActorLocation();
	if (TargetDirection.Size() < MaxPossibleDistance)
	{
		SetActorLocation(Location);
		return;
	}

	const FVector TargetDirectionNormalized = TargetDirection.GetSafeNormal();
	FVector LocationDelta = TargetDirectionNormalized * MaxPossibleDistance;
	LocationDelta.Z = 0;
	const FVector FinalLocation = GetActorLocation() + LocationDelta;
	
	SetActorLocation(FinalLocation);
}

void AAIGuard::LookAtNoiseDistraction(const FVector& Location)
{
	RotateTowardsLocation(Location);
	
	GetWorldTimerManager().SetTimer(
		RevertToOriginalRotationTimerHandle,
		this,
		&AAIGuard::RevertToOriginalRotation,
		RevertToOriginalRotationTimer,
		false);
}

void AAIGuard::RevertToOriginalRotation()
{
	if (GuardState == EGuardState::Alerted)
		return;
	
	TryStartPatrolling();
}
