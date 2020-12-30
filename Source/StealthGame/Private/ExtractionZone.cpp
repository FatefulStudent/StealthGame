#include "ExtractionZone.h"
#include "NetworkingHelper.h"
#include "StealthCharacter.h"
#include "StealthGameMode.h"

#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"

AExtractionZone::AExtractionZone()
{
	PrimaryActorTick.bCanEverTick = false;
	SetReplicates(true);
	
	OverlapBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBoxContent"));
	OverlapBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapBoxComponent->SetCollisionObjectType(ECC_WorldDynamic);
	OverlapBoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapBoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapBoxComponent->SetGenerateOverlapEvents(true);
	OverlapBoxComponent->SetBoxExtent(FVector(300.0f));
	OverlapBoxComponent->SetHiddenInGame(false);

	RootComponent = OverlapBoxComponent;

	OverlapDecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	OverlapDecalComponent->DecalSize = OverlapBoxComponent->GetScaledBoxExtent();
	OverlapDecalComponent->SetupAttachment(OverlapBoxComponent);
}

void AExtractionZone::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!HasAuthority())
		OverlapBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AExtractionZone::NotifyActorBeginOverlap(AActor* OtherActor)
{
	check(HasAuthority())
	
	if (auto StealthCharacter = Cast<AStealthCharacter>(OtherActor))
	{
		if (StealthCharacter->IsCarryingObjective())
		{
			OnExtractionComplete(StealthCharacter);
		}
		else
		{
			OnExtractionFailed(StealthCharacter);
		}
	}
}

void AExtractionZone::OnExtractionComplete(AStealthCharacter* StealthCharacter)
{
	check(HasAuthority())
	
	UE_LOG(LogStealthCharacter, Warning, TEXT("%s: Extraction Successful"), *StealthCharacter->GetName());
	if (auto StealthGameMode = Cast<AStealthGameMode>(GetWorld()->GetAuthGameMode()))
		StealthGameMode->CompleteMission(StealthCharacter, true);
}

void AExtractionZone::OnExtractionFailed(AStealthCharacter* StealthCharacter)
{
	check(HasAuthority())
	
	UE_LOG(LogStealthCharacter, Warning, TEXT("Overlapped with extraction, but no objective"));

	StealthCharacter->PlaySoundOnAutonomousClients(ExtractionFailedSound);
}
