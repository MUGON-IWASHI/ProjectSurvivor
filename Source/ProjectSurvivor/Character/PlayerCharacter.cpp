// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"
#include "Weapon/WeaponBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/Engine.h"
#include "Components/ExperienceComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/UserWidget.h"
#include "UI/LevelUpWidget.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

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

			CurrentWeapon->OnWeaponEvolutionReady.AddDynamic(
				this,
				&APlayerCharacter::HandleWeaponEvolutionReady
			);

			CurrentWeapon->AttachToComponent(
				GetMesh(),
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				TEXT("WeaponSocket")
			);

			StartAutoFire();
		}
	}

	if (GameHUDWidgetClass != nullptr)
	{
		GameHUDWidget = CreateWidget<UUserWidget>(
			GetWorld(),
			GameHUDWidgetClass
		);

		if (GameHUDWidget != nullptr)
		{
			GameHUDWidget->AddToViewport();

			UE_LOG(LogTemp, Warning, TEXT("Game HUD Added"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create Game HUD"));
		}
	}

	else
	{
		UE_LOG(LogTemp, Error, TEXT("GameHUDWidgetClass is nullptr"));
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

	}

	if (ExperienceComponent != nullptr && GEngine != nullptr) {

		const FString ExpText = FString::Printf(
			TEXT("Level: %d, EXP: %d / %d"),
			ExperienceComponent->GetCurrentLevel(),
			ExperienceComponent->GetCurrentExperience(),
			ExperienceComponent->GetRequiredExperience()
		);

	}

	if (!bIsGameOver &&
		HealthComponent != nullptr &&
		HealthComponent->IsDead())
	{
		bIsGameOver = true;

		ShowGameOver();
	}

	if (!bIsGameOver && !bIsGameClear)
	{
		ElapsedGameTime += DeltaTime;

		if (ElapsedGameTime >= ClearTime)
		{
			bIsGameClear = true;
			ShowGameClear();
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
		//EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &APlayerCharacter::Fire);
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

	if (LevelUpSound != nullptr)
	{
		UGameplayStatics::PlaySound2D(
			GetWorld(),
			LevelUpSound
		);
	}

	if (GEngine != nullptr) {

		const FString LevelUpText = FString::Printf(
			TEXT("LEVEL UP! lv.%d"),
			NewLevel
		);

	}

	

	if (LevelUpWidgetClass != nullptr) {

		LevelUpWidget = CreateWidget<ULevelUpWidget>(GetWorld(), LevelUpWidgetClass);

		if (LevelUpWidget != nullptr) {

			LevelUpWidget->OnAttackDamageSelected.AddDynamic(
				this,
				&APlayerCharacter::ApplyAttackDamageUpgrade
			);

			LevelUpWidget->OnMoveSpeedSelected.AddDynamic(
				this,
				&APlayerCharacter::ApplyMoveSpeedUpgrade
			);

			LevelUpWidget->OnLaserWeaponSelected.AddDynamic(
				this,
				&APlayerCharacter::HandleLaserWeaponSelected
			);

			LevelUpWidget->OnFireRateSelected.AddDynamic(
				this,
				&APlayerCharacter::ApplyFireRateUpgrade
			);

			LevelUpWidget->OnProjectileCountSelected.AddDynamic(
				this,
				&APlayerCharacter::ApplyProjectileCountUpgrade
			);

			LevelUpWidget->AddToViewport();
			
			APlayerController* PlayerController = GetController<APlayerController>();

			if (PlayerController != nullptr) {

				PlayerController->SetPause(true);

				PlayerController->SetShowMouseCursor(true);

				FInputModeUIOnly InputMode;
				PlayerController->SetInputMode(InputMode);
			}
		}
	}
}

void APlayerCharacter::ApplyAttackDamageUpgrade() {

	if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->LevelUpWeapon();

		StartAutoFire();
	}

	CloseLevelUpWidget();
}

void APlayerCharacter::ApplyMoveSpeedUpgrade()
{
	if (GetCharacterMovement() == nullptr)
	{
		return;
	}

	GetCharacterMovement()->MaxWalkSpeed += MoveSpeedUpgradeAmount;

	CloseLevelUpWidget();
}

void APlayerCharacter::ApplyFireRateUpgrade()
{
	if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->ReduceFireInterval(0.05f);
		StartAutoFire();
	}

	CloseLevelUpWidget();
}

void APlayerCharacter::ApplyProjectileCountUpgrade()
{
	if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->IncreaseProjectileCount();
	}

	CloseLevelUpWidget();
}

void APlayerCharacter::CloseLevelUpWidget()
{
	if (LevelUpWidget != nullptr)
	{
		LevelUpWidget->RemoveFromParent();
		LevelUpWidget = nullptr;
	}

	APlayerController* PlayerController = GetController<APlayerController>();

	if (PlayerController != nullptr)
	{
		PlayerController->SetPause(false);
		PlayerController->SetShowMouseCursor(false);

		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
	}
}

void APlayerCharacter::StartAutoFire()
{
	if (CurrentWeapon == nullptr)
	{
		return;
	}

	GetWorldTimerManager().ClearTimer(FireTimerHandle);

	GetWorldTimerManager().SetTimer(
		FireTimerHandle,
		this,
		&APlayerCharacter::AutoFire,
		CurrentWeapon->GetFireInterval(),
		true
	);
}

void APlayerCharacter::AutoFire()
{
	Fire();
}

void APlayerCharacter::EvolveWeapon() {

	if (CurrentWeapon == nullptr || EvolvedWeaponClass == nullptr) {

		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("EvolveWeapon"));

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AWeaponBase* NewWeapon = GetWorld()->SpawnActor<AWeaponBase>(
		EvolvedWeaponClass,
		CurrentWeapon->GetActorTransform(),
		SpawnParams
	);

	if (NewWeapon == nullptr) {
		return;
	}

	NewWeapon->AttachToComponent(
		GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		TEXT("WeaponSocket")
	);

	CurrentWeapon->Destroy();
	CurrentWeapon = NewWeapon;

	CurrentWeapon->OnWeaponEvolutionReady.AddDynamic(
		this,
		&APlayerCharacter::HandleWeaponEvolutionReady
	);

	StartAutoFire();
}

void APlayerCharacter::HandleWeaponEvolutionReady() {

	UE_LOG(LogTemp, Warning, TEXT("Evolution Event Received!"));

	EvolveWeapon();
}

void APlayerCharacter::HandleLaserWeaponSelected()
{
	AcquireSecondaryWeapon();
	CloseLevelUpWidget();
}

void APlayerCharacter::SecondaryAutoFire() {

	if (SecondaryWeapon != nullptr)
	{
		SecondaryWeapon->Fire();
	}
}

void APlayerCharacter::StartSecondaryAutoFire()
{
	if (SecondaryWeapon == nullptr)
	{
		return;
	}

	GetWorldTimerManager().ClearTimer(SecondaryFireTimerHandle);

	GetWorldTimerManager().SetTimer(
		SecondaryFireTimerHandle,
		this,
		&APlayerCharacter::SecondaryAutoFire,
		SecondaryWeapon->GetFireInterval(),
		true
	);
}

void APlayerCharacter::AcquireSecondaryWeapon()
{
	if (SecondaryWeapon != nullptr)
	{
		return;
	}

	if (SecondaryWeaponClass == nullptr)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	SecondaryWeapon = GetWorld()->SpawnActor<AWeaponBase>(
		SecondaryWeaponClass,
		GetActorLocation(),
		GetActorRotation(),
		SpawnParams
	);

	if (SecondaryWeapon == nullptr)
	{
		return;
	}

	SecondaryWeapon->AttachToComponent(
		GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		TEXT("WeaponSocket")
	);

	StartSecondaryAutoFire();
}

void APlayerCharacter::ShowGameOver()
{
	if (GameOverSound != nullptr)
	{
		UGameplayStatics::PlaySound2D(
			GetWorld(),
			GameOverSound
		);
	}

	if (GameOverWidgetClass == nullptr)
	{
		return;
	}

	GameOverWidget = CreateWidget<UUserWidget>(
		GetWorld(),
		GameOverWidgetClass
	);

	if (GameOverWidget == nullptr)
	{
		return;
	}

	GameOverWidget->AddToViewport();

	APlayerController* PC =
		Cast<APlayerController>(GetController());

	if (PC != nullptr)
	{
		PC->SetPause(true);
		PC->SetShowMouseCursor(true);

		FInputModeUIOnly InputMode;
		PC->SetInputMode(InputMode);
	}
}

void APlayerCharacter::ShowGameClear()
{
	if (GameClearSound != nullptr)
	{
		UGameplayStatics::PlaySound2D(
			GetWorld(),
			GameClearSound
		);
	}

	APlayerController* PC =
		Cast<APlayerController>(GetController());

	if (PC != nullptr)
	{
		PC->SetPause(true);
		PC->SetShowMouseCursor(true);

		FInputModeUIOnly InputMode;
		PC->SetInputMode(InputMode);
	}

	if (GameClearWidgetClass == nullptr)
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("GameClearWidgetClass is nullptr")
		);
		return;
	}

	GameClearWidget = CreateWidget<UUserWidget>(
		GetWorld(),
		GameClearWidgetClass
	);

	if (GameClearWidget == nullptr)
	{
		return;
	}

	GameClearWidget->AddToViewport();
}

float APlayerCharacter::GetRemainingTime() const
{
	return FMath::Max(ClearTime - ElapsedGameTime, 0.0f);
}

float APlayerCharacter::GetCurrentHealth() const
{
	return HealthComponent != nullptr
		? HealthComponent->GetCurrentHealth()
		: 0.0f;
}

float APlayerCharacter::GetMaxHealth() const
{
	return HealthComponent != nullptr
		? HealthComponent->GetMaxHealth()
		: 0.0f;
}

int32 APlayerCharacter::GetCurrentLevel() const
{
	return ExperienceComponent != nullptr
		? ExperienceComponent->GetCurrentLevel()
		: 1;
}