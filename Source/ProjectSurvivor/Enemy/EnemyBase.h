// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/HealthComponent.h"
#include "EnemyBase.generated.h"

class AExperienceOrb;

class USoundBase;

UCLASS()
class PROJECTSURVIVOR_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Die();

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float MoveSpeed = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float StopDistance = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float RotationSpeed = 8.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Attack")
	float AttackDamage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Attack")
	float AttackInterval = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Drop")
	TSubclassOf<AExperienceOrb> ExperienceOrbClass;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundBase* DeathSound = nullptr;

	float LastAttackTime = 0.0f;

	void TryAttackPlayer(ACharacter* PlayerCharacter);

	void MoveToPlayer(float DeltaTime);

	void DropExperienceOrb();
};
