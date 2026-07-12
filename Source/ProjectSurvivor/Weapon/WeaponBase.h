// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "WeaponBase.generated.h"

class AProjectileBase;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponEvolutionReady);

UCLASS()
class PROJECTSURVIVOR_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Fire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void IncreaseAttackDamage(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void LevelUpWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void IncreaseProjectileCount();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ReduceFireInterval(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	float GetFireInterval() const;

	UPROPERTY(BlueprintAssignable,Category = "Weapon")
	FOnWeaponEvolutionReady OnWeaponEvolutionReady;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float AttackDamage = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float FireInterval = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AProjectileBase> ProjectileClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USceneComponent* MuzzlePoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	int32 WeaponLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	int32 ProjectileCount = 1;

};
