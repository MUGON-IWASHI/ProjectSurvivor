// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/HealthComponent.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveToPlayer(DeltaTime);
}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyBase::MoveToPlayer(float DeltaTime) {

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (PlayerCharacter == nullptr) {

		return;
	}

	const FVector EnemyLocation = GetActorLocation();
	const FVector PlayerLocation = PlayerCharacter->GetActorLocation();

	const float DistanceToPlayer = FVector::Dist(EnemyLocation, PlayerLocation);

	if (DistanceToPlayer <= StopDistance) {
		TryAttackPlayer(PlayerCharacter);
		return;
	}

	FVector Direction = PlayerLocation - EnemyLocation;
	Direction.Z = 0.0f; // Ignore vertical difference
	Direction.Normalize();

	const FRotator TargetRotation = Direction.Rotation();
	const FRotator NewRotation = FMath::RInterpTo(
		GetActorRotation(), 
		TargetRotation, 
		DeltaTime, 
		RotationSpeed
	);

	SetActorRotation(NewRotation);

	AddMovementInput(Direction, 1.0f);
}

void AEnemyBase::TryAttackPlayer(ACharacter* PlayerCharacter) {

	if (PlayerCharacter == nullptr) {
		return;
	}

	const float CurrentTime = GetWorld()->GetTimeSeconds();

	if (CurrentTime - LastAttackTime < AttackInterval) {
		return;
	}

	LastAttackTime = CurrentTime;

	UHealthComponent* PlayerHealth = PlayerCharacter->FindComponentByClass<UHealthComponent>();

	if (PlayerHealth != nullptr) {

		PlayerHealth->ApplyDamage(AttackDamage);

		if (PlayerHealth->IsDead()) {
			//PlayerCharacter->Destroy();
		}
	}
}

