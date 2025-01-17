#include "LaunchPad.h"

#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

ALaunchPad::ALaunchPad()
{
	PrimaryActorTick.bCanEverTick = false;

	LaunchPadBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	LaunchPadBase->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = LaunchPadBase;

	LaunchPadArrow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow"));
	LaunchPadArrow->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LaunchPadArrow->SetupAttachment(LaunchPadBase);

	OverlapBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	OverlapBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	OverlapBoxComponent->SetCollisionObjectType(ECC_WorldDynamic);
	OverlapBoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapBoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapBoxComponent->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	OverlapBoxComponent->SetupAttachment(LaunchPadBase);

	OverlapBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ALaunchPad::OnBeginOverlap);
}

void ALaunchPad::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (auto Character = Cast<ACharacter>(OtherActor))
	{
		FRotator LaunchDirection = GetActorRotation();
		LaunchDirection.Pitch += LaunchPitchAngle;
		
		const FVector LaunchVelocity = LaunchDirection.Vector() * LaunchForce; 
		Character->LaunchCharacter(LaunchVelocity, true, true);
		
		UGameplayStatics::SpawnEmitterAtLocation(this, LaunchEffect, GetActorLocation());
	}
	else if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		FRotator LaunchDirection = GetActorRotation();
		LaunchDirection.Pitch += LaunchPitchAngle;
		
		const FVector LaunchImpulse = LaunchDirection.Vector() * LaunchForce;
		OtherComp->AddImpulse(LaunchImpulse, NAME_None, true);
		
		UGameplayStatics::SpawnEmitterAtLocation(this, LaunchEffect, GetActorLocation());
	}
}
