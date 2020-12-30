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

	if (FNetworkingHelper::HasCosmetics())
		StartPlayingStationaryEffects();
}

bool AStealthObjective::IsAvailableForInteraction() const
{
	return !IsPendingKill();
}

void AStealthObjective::OnSuccessfulInteraction()
{
	check(HasAuthority());

	CachedStationaryEffects->DestroyComponent();
	
	Destroy();
}

void AStealthObjective::StartPlayingStationaryEffects()
{
	check(FNetworkingHelper::HasCosmetics());
	
	CachedStationaryEffects = UGameplayStatics::SpawnEmitterAtLocation(this, StationaryEffect, GetActorLocation());
}
