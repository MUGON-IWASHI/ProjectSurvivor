// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/LaserWeapon.h"
#include "Components/HealthComponent.h"
#include "DrawDebugHelpers.h"
#include "Enemy/EnemyBase.h"

void ALaserWeapon::Fire() {

	UE_LOG(LogTemp, Warning, TEXT("LaserWeapon Fire"));

	AActor* WeaponOwner = GetOwner();

	if (WeaponOwner == nullptr || GetWorld() == nullptr)
	{
		return;
	}

	const FVector Start =
		MuzzlePoint->GetComponentLocation();

	const FVector FireDirection =
		WeaponOwner->GetActorForwardVector();

	const float LaserRange = 2000.0f;

	const FVector End =
		Start + FireDirection * LaserRange;

	FHitResult HitResult;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(WeaponOwner);

	if (APawn* InstigatorPawn = GetInstigator())
	{
		QueryParams.AddIgnoredActor(InstigatorPawn);
	}

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	const bool bHit = GetWorld()->LineTraceSingleByObjectType(
		HitResult,
		Start,
		End,
		ObjectQueryParams,
		QueryParams
	);
	const FVector DebugEnd = bHit
		? HitResult.ImpactPoint
		: End;

	DrawDebugLine(
		GetWorld(),
		Start,
		DebugEnd,
		bHit ? FColor::Green : FColor::Red,
		false,
		0.1f,
		0,
		3.0f
	);

	if (bHit && HitResult.GetActor() != nullptr)
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("Laser Hit: %s"),
			*HitResult.GetActor()->GetName()
		);
	}

	if (!bHit || HitResult.GetActor() == nullptr)
	{
		return;
	}

	UHealthComponent* HealthComponent =
		HitResult.GetActor()->FindComponentByClass<UHealthComponent>();

	if (HealthComponent == nullptr)
	{
		return;
	}

	HealthComponent->ApplyDamage(AttackDamage);

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Enemy Health: %.1f"),
		HealthComponent->GetCurrentHealth()
	);

	if (HealthComponent->IsDead())
	{
		if (AEnemyBase* Enemy =
			Cast<AEnemyBase>(HitResult.GetActor()))
		{
			Enemy->Die();
		}
		else
		{
			HitResult.GetActor()->Destroy();
		}
	}
}