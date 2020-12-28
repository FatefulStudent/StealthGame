#include "BlackHole.h"

#include "Components/SphereComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

ABlackHole::ABlackHole()
{
	PrimaryActorTick.bCanEverTick = true;

	DestructingSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("DestructionSphere"));
	DestructingSphereComponent->SetCollisionObjectType(ECC_WorldDynamic);
	DestructingSphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	DestructingSphereComponent->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	RootComponent = DestructingSphereComponent;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetupAttachment(DestructingSphereComponent);
	
	AttractingRadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("AttractingForceComponnet"));
	AttractingRadialForceComponent->SetupAttachment(DestructingSphereComponent);
}

void ABlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DestroyObjectsInDestructionSphere();
}

void ABlackHole::DestroyObjectsInDestructionSphere() const
{
	TArray<AActor*> ActorsToDestroy;
	DestructingSphereComponent->GetOverlappingActors(ActorsToDestroy);

	for (AActor* ActorToDestroy : ActorsToDestroy)
	{
		if (IsValid(ActorToDestroy))
			ActorToDestroy->Destroy();
	}
}
