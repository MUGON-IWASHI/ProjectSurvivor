// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ProjectileBase.h"
#include "Components/HealthComponent.h"
#include "Enemy/EnemyBase.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	SetRootComponent(CollisionComponent);
	CollisionComponent->InitSphereRadius(15.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionComponent->SetGenerateOverlapEvents(true);
	CollisionComponent->CanCharacterStepUpOn =
		ECanBeCharacterBase::ECB_No;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(CollisionComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetGenerateOverlapEvents(false);
	MeshComponent->CanCharacterStepUpOn =
		ECanBeCharacterBase::ECB_No;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->UpdatedComponent = CollisionComponent;
	ProjectileMovementComponent->InitialSpeed = 3000.0f;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f; // No gravity

	InitialLifeSpan = 3.0f; // Destroy after 3 seconds

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnOverlap);

}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileBase::OnOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{

	if (OtherActor == nullptr || OtherActor == this || OtherActor == GetOwner() || OtherActor == GetInstigator())
	{
		return;
	}

	UHealthComponent* HealthComponent = OtherActor->FindComponentByClass<UHealthComponent>();

	if (HealthComponent == nullptr)
	{
		return;
	}

	HealthComponent->ApplyDamage(Damage);

	if (HealthComponent->IsDead())
	{
		if (AEnemyBase* Enemy = Cast<AEnemyBase>(OtherActor))
		{
			Enemy->Die();
		}
		else {
			OtherActor->Destroy();
		}
	}

	Destroy();
}

void AProjectileBase::SetDamage(float NewDamage)
{
	Damage = NewDamage;
}
