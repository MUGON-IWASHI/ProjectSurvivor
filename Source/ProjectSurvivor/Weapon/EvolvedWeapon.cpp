// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/EvolvedWeapon.h"
#include "Weapon/ProjectileBase.h"

void AEvolvedWeapon::Fire()
{
	if (ProjectileClass == nullptr || MuzzlePoint == nullptr)
	{
		return;
	}

	AActor* WeaponOwner = GetOwner();

	if (WeaponOwner == nullptr)
	{
		return;
	}

	const int32 ShotCount = FMath::Max(ProjectileCount, 1);
	const float AngleStep = 360.0f / static_cast<float>(ShotCount);

	for (int32 i = 0; i < ShotCount; ++i)
	{
		const float YawAngle = AngleStep * i;
		const FRotator SpawnRotation(0.0f, YawAngle, 0.0f);
		const FVector FireDirection = SpawnRotation.Vector();

		// Characterの内側で弾が生成されないよう、方向ごとに前へずらす
		const FVector SpawnLocation =
			MuzzlePoint->GetComponentLocation() + FireDirection * 100.0f;

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = WeaponOwner;
		SpawnParams.Instigator = GetInstigator();
		SpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AProjectileBase* Projectile =
			GetWorld()->SpawnActor<AProjectileBase>(
				ProjectileClass,
				SpawnLocation,
				SpawnRotation,
				SpawnParams
			);

		if (Projectile != nullptr)
		{
			Projectile->SetDamage(AttackDamage);
		}
	}
}