// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"
#include "Weapon/WeaponBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/Engine.h"
#include "Components/ExperienceComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	ExperienceComponent = CreateDefaultSubobject<UExperienceComponent>(TEXT("ExperienceComponent"));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = 
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (WeaponClass != nullptr) {

		CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass);
		CurrentWeapon->SetOwner(this);

		if (CurrentWeapon != nullptr) {

			CurrentWeapon->AttachToComponent(
				GetMesh(),
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				TEXT("WeaponSocket")
			);
		}
	}

	if (ExperienceComponent != nullptr) {

		ExperienceComponent->OnLevelUp.AddDynamic(
			this,
			&APlayerCharacter::HandleLevelUp
		);
	}
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HealthComponent != nullptr && GEngine != nullptr)
	{
		const FString HealthText = FString::Printf(
			TEXT("HP: %.0f / %.0f"),
			HealthComponent->GetCurrentHealth(),
			HealthComponent->GetMaxHealth()
		);

		GEngine->AddOnScreenDebugMessage(
			1,
			0.0f,
			FColor::Green,
			HealthText
		);
	}

	if (ExperienceComponent != nullptr && GEngine != nullptr) {

		const FString ExpText = FString::Printf(
			TEXT("Level: %d, EXP: %d / %d"),
			ExperienceComponent->GetCurrentLevel(),
			ExperienceComponent->GetCurrentExperience(),
			ExperienceComponent->GetRequiredExperience()
		);

		GEngine->AddOnScreenDebugMessage(
			3,
			0.0f,
			FColor::Cyan,
			ExpText
		);
	}

	if (!bIsGameOver &&
		HealthComponent != nullptr &&
		HealthComponent->IsDead())
	{
		bIsGameOver = true;

		if (GEngine != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(
				2,
				5.0f,
				FColor::Red,
				TEXT("GAME OVER")
			);
		}

		APlayerController* PC = Cast<APlayerController>(GetController());

		if (PC != nullptr)
		{
			PC->SetPause(true);
		}
	}

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopJumping);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &APlayerCharacter::Fire);
	}

}

void APlayerCharacter::Move(const FInputActionValue& Value) {

	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr) {

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value) {

	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller != nullptr) {
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::Fire() {
	if (CurrentWeapon != nullptr) {
		CurrentWeapon->Fire();
	}
}

void APlayerCharacter::HandleLevelUp(int32 NewLevel) {

	if (GEngine != nullptr) {

		const FString LevelUpText = FString::Printf(
			TEXT("LEVEL UP! lv.%d"),
			NewLevel
		);

		GEngine->AddOnScreenDebugMessage(
			4,
			3.0f,
			FColor::Yellow,
			LevelUpText
		);
	}
}