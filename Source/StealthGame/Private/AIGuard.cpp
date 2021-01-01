#include "AIGuard.h"
#include "StealthGameMode.h"
#include "NetworkingHelper.h"

#include "DrawDebugHelpers.h"
#include "GeneratedCodeHelpers.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
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

	if (HasAuthority())
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AAIGuard::OnSeePawn);
		PawnSensingComponent->OnHearNoise.AddDynamic(this, &AAIGuard::OnHearNoise);

		OriginalRotation = GetActorRotation();
	}
	else
	{
		SetActorTickEnabled(false);
	}
}

void AAIGuard::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
		TryStartPatrolling();
}

void AAIGuard::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GuardState == EGuardState::Patrolling)
	{
		StartMovingToOtherTargetPointIfNeeded();
	}
}

void AAIGuard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAIGuard, GuardState);
}

void AAIGuard::ChangeState(EGuardState NewState)
{
	check(HasAuthority())
	
	if (GuardState == NewState)
		return;

	if (GuardState == EGuardState::Patrolling)
		GetController()->StopMovement();

	GuardState = NewState;

	OnRep_GuardState();
}

void AAIGuard::OnSeePawn(APawn* Pawn)
{
	check(HasAuthority());
	
	if (!Pawn)
		return;

	ChangeState(EGuardState::Alerted);
	
	if (auto StealthGameMode = Cast<AStealthGameMode>(GetWorld()->GetAuthGameMode()))
		StealthGameMode->CompleteMission(false);
	
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
	check(HasAuthority());
	
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

void AAIGuard::StartMovingToOtherTargetPointIfNeeded()
{
	check(HasAuthority());
	
	if (GetHorizontalDistanceTo(CurrentTargetPoint) <= AcceptanceRadius2D)
	{
		UpdateTargetPoint();
		UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentTargetPoint);
	}
}

void AAIGuard::TryStartPatrolling()
{
	check(HasAuthority());
	
	if (PatrollingTargetPoints.Num() > 0)
	{
		ChangeState(EGuardState::Patrolling);
		if (CurrentTargetPoint == nullptr)
			UpdateTargetPoint();
		UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentTargetPoint);
	}
	else
	{
		SetActorRotation(OriginalRotation);
		ChangeState(EGuardState::Idle);
	}
}

void AAIGuard::UpdateTargetPoint()
{
	check(HasAuthority());
	
	CurrentTargetPointIndex = FMath::Max(0, CurrentTargetPointIndex+1);
	
	if (CurrentTargetPointIndex > PatrollingTargetPoints.Num() - 1)
		CurrentTargetPointIndex = 0;
	
	CurrentTargetPoint = PatrollingTargetPoints[CurrentTargetPointIndex];
}

void AAIGuard::RotateTowardsLocation(const FVector& Location)
{
	check(HasAuthority());
	
	const FVector TargetDirection = Location - GetActorLocation();
	const float DesiredYaw = FRotationMatrix::MakeFromX(TargetDirection).Rotator().Yaw;
	const FRotator NewRotation { GetActorRotation().Pitch, DesiredYaw, GetActorRotation().Pitch };
	SetActorRotation(NewRotation);
}

void AAIGuard::LookAtNoiseDistraction(const FVector& Location)
{
	check(HasAuthority());
	
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

void AAIGuard::OnRep_GuardState()
{
	if (FNetworkingHelper::HasCosmetics(this))
		OnStateChange(GuardState);
}
