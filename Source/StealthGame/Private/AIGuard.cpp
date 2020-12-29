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

void AAIGuard::OnHearNoise(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	DrawDebugSphere(GetWorld(),
	    Location, 
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
