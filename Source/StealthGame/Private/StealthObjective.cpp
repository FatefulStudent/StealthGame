#include "StealthObjective.h"
#include "NetworkingHelper.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


AStealthObjective::AStealthObjective()
{
	PrimaryActorTick.bCanEverTick = false;

	SetReplicates(true);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = StaticMeshComponent;
	
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollisionComp"));
	CollisionComponent->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CollisionComponent->SetGenerateOverlapEvents(true);
	CollisionComponent->SetupAttachment(StaticMeshComponent);
}

void AStealthObjective::BeginPlay()
{
	Super::BeginPlay();

	if (FNetworkingHelper::HasCosmetics(this))
		StartPlayingStationaryEffects();
}

void AStealthObjective::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (FNetworkingHelper::HasCosmetics(this) && EndPlayReason == EEndPlayReason::Destroyed)
		CachedStationaryEffects->DestroyComponent();
}

bool AStealthObjective::IsAvailableForInteraction() const
{
	return !IsPendingKill();
}

void AStealthObjective::OnSuccessfulInteraction()
{
	check(HasAuthority());

	Destroy();
}

void AStealthObjective::StartPlayingStationaryEffects()
{
	check(FNetworkingHelper::HasCosmetics(this));
	
	CachedStationaryEffects = UGameplayStatics::SpawnEmitterAtLocation(this, StationaryEffect, GetActorLocation());
}
