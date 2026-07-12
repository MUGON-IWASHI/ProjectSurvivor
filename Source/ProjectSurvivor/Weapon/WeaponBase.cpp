// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBase.h"
#include "Weapon/ProjectileBase.h"
#include "Components/PrimitiveComponent.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(Root);

	MuzzlePoint = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzlePoint"));
	MuzzlePoint->SetupAttachment(WeaponMesh);

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::Fire() {

	if (ProjectileClass == nullptr)
	{
		return;
	}

	AActor* WeaponOwner = GetOwner();

	if (WeaponOwner == nullptr)
	{
		return;
	}

	const FVector FireDirection =
		WeaponOwner->GetActorForwardVector();

	const FVector ForwardOffset =
		FireDirection * 100.0f;

	const FVector SpawnLocation =
		MuzzlePoint->GetComponentLocation() + ForwardOffset;

	const FRotator BaseRotation =
		FireDirection.Rotation();

	for (int32 i = 0; i < ProjectileCount; ++i)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = GetInstigator();
		SpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		const float AngleBetweenProjectiles = 10.0f;
		const float SideSpacing = 20.0f;

		const float AngleOffset =
			(i - (ProjectileCount - 1) / 2.0f) * AngleBetweenProjectiles;

		const float SideOffset =
			(i - (ProjectileCount - 1) / 2.0f) * SideSpacing;

		FRotator SpawnRotation = BaseRotation;
		SpawnRotation.Yaw += AngleOffset;

		const FVector ProjectileSpawnLocation =
			SpawnLocation + MuzzlePoint->GetRightVector() * SideOffset;

		AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(
			ProjectileClass,
			ProjectileSpawnLocation,
			SpawnRotation,
			SpawnParams
		);

		if (Projectile != nullptr)
		{
			Projectile->SetDamage(AttackDamage);

			

			if (WeaponOwner != nullptr)
			{
				if (UPrimitiveComponent* ProjectileRoot =
					Cast<UPrimitiveComponent>(Projectile->GetRootComponent()))
				{
					ProjectileRoot->IgnoreActorWhenMoving(
						WeaponOwner,
						true
					);
				}

				if (UPrimitiveComponent* OwnerRoot =
					Cast<UPrimitiveComponent>(WeaponOwner->GetRootComponent()))
				{
					OwnerRoot->IgnoreActorWhenMoving(
						Projectile,
						true
					);
				}
			}
		}
	}

}

void AWeaponBase::IncreaseAttackDamage(float Amount) {

	if (Amount <= 0.0f) {
		return;
	}

	AttackDamage += Amount;

}

void AWeaponBase::LevelUpWeapon()
{
	++WeaponLevel;

	if (WeaponLevel == 2)
	{
		IncreaseAttackDamage(5.0f);
	}
	else if (WeaponLevel == 3)
	{
		IncreaseProjectileCount();
	}
	else if (WeaponLevel == 4)
	{
		IncreaseProjectileCount();
	}
	else if (WeaponLevel == 5)
	{
		ReduceFireInterval(0.1f);

		UE_LOG(LogTemp, Warning, TEXT("Evolution Broadcast!"));

		OnWeaponEvolutionReady.Broadcast();
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Weapon Level : %d"), WeaponLevel);
}

void AWeaponBase::IncreaseProjectileCount()
{
	++ProjectileCount;

	UE_LOG(LogTemp, Warning, TEXT("Projectile Count : %d"), ProjectileCount);
}

void AWeaponBase::ReduceFireInterval(float Amount)
{
	if (Amount <= 0.0f)
	{
		return;
	}

	FireInterval -= Amount;

	if (FireInterval < 0.1f)
	{
		FireInterval = 0.1f;
	}

	UE_LOG(LogTemp, Warning, TEXT("Fire Interval : %.2f"), FireInterval);
}

float AWeaponBase::GetFireInterval() const
{
	return FireInterval;
}