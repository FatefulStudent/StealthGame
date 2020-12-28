#include "BlackHole.h"

#include "Components/SphereComponent.h"

ABlackHole::ABlackHole()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = StaticMeshComponent;
	
	DestructingSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("DestructionSphere"));
	DestructingSphereComponent->SetSphereRadius(DestructingSphereDefaultRadius);
	DestructingSphereComponent->SetCollisionObjectType(ECC_WorldDynamic);
	DestructingSphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	DestructingSphereComponent->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	DestructingSphereComponent->SetGenerateOverlapEvents(true);
	DestructingSphereComponent->SetupAttachment(StaticMeshComponent);

	DestructingSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABlackHole::OnDestructionSphereBeginOverlap);
	
	AttractingSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("AttractingSphereComponent"));
	AttractingSphereComponent->SetSphereRadius(AttractingSphereDefaultRadius);
	AttractingSphereComponent->SetCollisionObjectType(ECC_WorldDynamic);
	AttractingSphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttractingSphereComponent->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	AttractingSphereComponent->SetGenerateOverlapEvents(true);
	AttractingSphereComponent->SetupAttachment(StaticMeshComponent);
}

void ABlackHole::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	ApplyRadialForceOnOverlappedComponents();
}

void ABlackHole::ApplyRadialForceOnOverlappedComponents() const
{
	TArray<UPrimitiveComponent*> AttractedComponents;
	AttractingSphereComponent->GetOverlappingComponents(AttractedComponents);
	
	for (UPrimitiveComponent* AttractedComponent : AttractedComponents)
	{
		AttractedComponent->AddRadialForce(
			GetActorLocation(),
			AttractingSphereComponent->GetScaledSphereRadius(),
			AttractingForce,
			RIF_Linear);
	}
}

void ABlackHole::OnDestructionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
		OtherActor->Destroy();
}
