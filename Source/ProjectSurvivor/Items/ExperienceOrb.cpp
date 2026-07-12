// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ExperienceOrb.h"
#include "Components/ExperienceComponent.h"

// Sets default values
AExperienceOrb::AExperienceOrb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	SetRootComponent(CollisionComponent);
	CollisionComponent->InitSphereRadius(50.0f);

	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CollisionComponent->SetGenerateOverlapEvents(true);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(CollisionComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(
		this,
		&AExperienceOrb::OnOverlap);

}

// Called when the game starts or when spawned
void AExperienceOrb::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExperienceOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExperienceOrb::OnOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{

	if (OtherActor == nullptr || OtherActor == this)
	{
		return;
	}

	UExperienceComponent* ExperienceComponent =
		OtherActor->FindComponentByClass<UExperienceComponent>();

	if (ExperienceComponent == nullptr)
	{
		return;
	}

	ExperienceComponent->AddExperience(ExperienceValue);

	Destroy();
}
