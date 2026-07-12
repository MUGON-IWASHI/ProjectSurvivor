// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class AEnemyBase;

UCLASS()
class PROJECTSURVIVOR_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	TSubclassOf<AEnemyBase> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	float SpawnInterval = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	float SpawnRadius = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	TSubclassOf<AEnemyBase> FastEnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	float FastEnemyUnlockTime = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	float FastEnemySpawnChance = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	TSubclassOf<AEnemyBase> TankEnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	float TankEnemyUnlockTime = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	float TankEnemySpawnChance = 0.3f;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	float MidGameSpawnInterval = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	float LateGameSpawnInterval = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	float MidGameTime = 30.0f;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	float LateGameTime = 60.0f;

	bool bMidGameSpawnApplied = false;
	bool bLateGameSpawnApplied = false;

	void UpdateSpawnInterval(float NewInterval);

	float ElapsedTime = 0.0f;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	FTimerHandle SpawnTimerHandle;
	void SpawnEnemy();



	
};
