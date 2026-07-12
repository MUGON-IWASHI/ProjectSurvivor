// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "EvolvedWeapon.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVOR_API AEvolvedWeapon : public AWeaponBase
{
	GENERATED_BODY()

public:

	virtual void Fire() override;
	
};
