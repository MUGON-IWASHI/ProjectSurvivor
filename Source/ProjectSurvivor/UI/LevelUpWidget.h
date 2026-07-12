// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelUpWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackDamageSelected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMoveSpeedSelected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLaserWeaponSelected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFireRateSelected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProjectileCountSelected);

UCLASS()
class PROJECTSURVIVOR_API ULevelUpWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Level Up")
	FOnAttackDamageSelected OnAttackDamageSelected;

	UPROPERTY(BlueprintAssignable, Category = "Level Up")
	FOnMoveSpeedSelected OnMoveSpeedSelected;

	UPROPERTY(BlueprintAssignable, Category = "Level Up")
	FOnLaserWeaponSelected OnLaserWeaponSelected;

	UPROPERTY(BlueprintAssignable, Category = "Level Up")
	FOnFireRateSelected OnFireRateSelected;

	UPROPERTY(BlueprintAssignable, Category = "Level Up")
	FOnProjectileCountSelected OnProjectileCountSelected;

	UFUNCTION(BlueprintCallable, Category = "Level Up")
	void SelectFireRate();

	UFUNCTION(BlueprintCallable, Category = "Level Up")
	void SelectProjectileCount();

	UFUNCTION(BlueprintCallable, Category = "Level Up")
	void SelectAttackDamage();

	UFUNCTION(BlueprintCallable, Category = "Level Up")
	void SelectMoveSpeed();

	UFUNCTION(BlueprintCallable, Category = "Level Up")
	void SelectLaserWeapon();
};