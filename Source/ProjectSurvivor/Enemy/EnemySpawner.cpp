// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemySpawner.h"
#include "Enemy/EnemyBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&AEnemySpawner::SpawnEnemy,
		SpawnInterval,
		true
	);
	
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedTime += DeltaTime;

	if (!bLateGameSpawnApplied &&
		ElapsedTime >= LateGameTime)
	{
		bLateGameSpawnApplied = true;
		UpdateSpawnInterval(LateGameSpawnInterval);
	}
	else if (!bMidGameSpawnApplied &&
		ElapsedTime >= MidGameTime)
	{
		bMidGameSpawnApplied = true;
		UpdateSpawnInterval(MidGameSpawnInterval);
	}

}

void AEnemySpawner::SpawnEnemy() {

	TSubclassOf<AEnemyBase> ClassToSpawn = EnemyClass;

	const float RandomValue = FMath::FRand();

	if (ElapsedTime >= TankEnemyUnlockTime &&
		TankEnemyClass != nullptr &&
		RandomValue < TankEnemySpawnChance)
	{
		ClassToSpawn = TankEnemyClass;
	}
	else if (ElapsedTime >= FastEnemyUnlockTime &&
		FastEnemyClass != nullptr &&
		RandomValue < TankEnemySpawnChance + FastEnemySpawnChance)
	{
		ClassToSpawn = FastEnemyClass;
	}

	if (ClassToSpawn == nullptr)
	{
		return;
	}

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (PlayerCharacter == nullptr) {
		return;
	}

	const FVector PlayerLocation = PlayerCharacter->GetActorLocation();

	const FVector RandomOffset = FVector(
		FMath::RandRange(-SpawnRadius, SpawnRadius),
		FMath::RandRange(-SpawnRadius, SpawnRadius),
		0.0f
	);

	const FVector SpawnLocation = PlayerLocation + RandomOffset;
	const FRotator SpawnRotation = FRotator::ZeroRotator;

	GetWorld()->SpawnActor<AEnemyBase>(
		ClassToSpawn,
		SpawnLocation,
		SpawnRotation
	);
}

void AEnemySpawner::UpdateSpawnInterval(float NewInterval)
{
	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);

	GetWorldTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&AEnemySpawner::SpawnEnemy,
		NewInterval,
		true
	);
}

