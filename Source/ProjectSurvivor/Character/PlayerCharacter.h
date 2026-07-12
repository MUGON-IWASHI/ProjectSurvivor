// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "Components/HealthComponent.h"
#include "Components/ExperienceComponent.h"
#include "PlayerCharacter.generated.h"

class AWeaponBase;

class ULevelUpWidget;

class AWeaponBase;

class UUserWidget;

class USoundBase;

UCLASS()
class PROJECTSURVIVOR_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LevelUp")
	float AttackDamageUpgradeAmount = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LevelUp")
	float MoveSpeedUpgradeAmount = 50.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<ULevelUpWidget> LevelUpWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> GameClearWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> GameHUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundBase* LevelUpSound = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundBase* GameClearSound = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundBase* GameOverSound = nullptr;

	UPROPERTY()
	ULevelUpWidget* LevelUpWidget;

	UPROPERTY()
	UUserWidget* GameOverWidget = nullptr;

	UPROPERTY()
	UUserWidget* GameClearWidget = nullptr;

	UPROPERTY()
	UUserWidget* GameHUDWidget = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AWeaponBase> WeaponClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	AWeaponBase* CurrentWeapon;


	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AWeaponBase> EvolvedWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AWeaponBase> SecondaryWeaponClass;

	UPROPERTY()
	AWeaponBase* SecondaryWeapon = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	float ClearTime = 120.0f;

	UFUNCTION(BlueprintPure, Category = "UI")
	float GetCurrentHealth() const;

	UFUNCTION(BlueprintPure, Category = "UI")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintPure, Category = "UI")
	int32 GetCurrentLevel() const;

	UFUNCTION()
	void HandleLevelUp(int32 NewLevel);

	UFUNCTION()
	void EvolveWeapon();

	UFUNCTION()
	void ShowGameOver();

	UFUNCTION()
	void HandleWeaponEvolutionReady();

	UFUNCTION()
	void HandleLaserWeaponSelected();

	UFUNCTION()
	void AcquireSecondaryWeapon();

	UFUNCTION()
	void ShowGameClear();

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void Fire();

	float ElapsedGameTime = 0.0f;

	bool bIsGameClear = false;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure, Category = "Game")
	float GetRemainingTime() const;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera",meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera",meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UExperienceComponent* ExperienceComponent;

	bool bIsGameOver = false;

	UFUNCTION()
	void ApplyAttackDamageUpgrade();

	UFUNCTION()
	void ApplyMoveSpeedUpgrade();

	UFUNCTION()
	void ApplyFireRateUpgrade();

	UFUNCTION()
	void ApplyProjectileCountUpgrade();

	void CloseLevelUpWidget();

	FTimerHandle FireTimerHandle;

	void StartAutoFire();

	void AutoFire();

	FTimerHandle SecondaryFireTimerHandle;

	void StartSecondaryAutoFire();

	void SecondaryAutoFire();

};
