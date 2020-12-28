#include "ExtractionZone.h"
#include "StealthCharacter.h"

#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"

AExtractionZone::AExtractionZone()
{
	PrimaryActorTick.bCanEverTick = true;
	
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

void AExtractionZone::BeginPlay()
{
	Super::BeginPlay();
	
	OverlapBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AExtractionZone::OnOverlapWithPawn);
}

void AExtractionZone::OnOverlapWithPawn(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (auto StealthCharacter = Cast<AStealthCharacter>(OtherActor))
	{
		if (StealthCharacter->IsCarryingObjective())
		{
			OnExtractionComplete(StealthCharacter);
		}
		else
		{
			UE_LOG(LogStealthCharacter, Warning, TEXT("%s: Overlapped with extraction, but no objective"),
				*StealthCharacter->GetName());
		}
	}
}

void AExtractionZone::OnExtractionComplete(AStealthCharacter* StealthCharacter)
{
	UE_LOG(LogStealthCharacter, Warning, TEXT("%s: Extraction Successful"), *StealthCharacter->GetName());
}
