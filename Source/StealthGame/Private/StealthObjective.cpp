#include "StealthObjective.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


AStealthObjective::AStealthObjective()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = StaticMeshComponent;
	
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollisionComp"));
	CollisionComponent->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CollisionComponent->SetupAttachment(StaticMeshComponent);
}

void AStealthObjective::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	OnPickUp();
}

void AStealthObjective::StartPlayingStationaryEffects()
{
	CachedStationaryEffects = UGameplayStatics::SpawnEmitterAtLocation(this, StationaryEffect, GetActorLocation());
}

void AStealthObjective::OnPickUp() const
{
	CachedStationaryEffects->Deactivate();
	
	UGameplayStatics::SpawnEmitterAtLocation(this, PickupEffect, GetActorLocation());
}

// Called when the game starts or when spawned
void AStealthObjective::BeginPlay()
{
	Super::BeginPlay();
	
	StartPlayingStationaryEffects();
}


