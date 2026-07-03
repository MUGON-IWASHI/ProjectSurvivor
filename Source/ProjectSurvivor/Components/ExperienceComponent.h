#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ExperienceComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelUp, int32, NewLevel);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTSURVIVOR_API UExperienceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UExperienceComponent();

	UFUNCTION(BlueprintCallable, Category = "Experience")
	void AddExperience(int32 Amount);

	UFUNCTION(BlueprintPure, Category = "Experience")
	int32 GetCurrentExperience() const;

	UFUNCTION(BlueprintPure, Category = "Experience")
	int32 GetCurrentLevel() const;

	UFUNCTION(BlueprintPure, Category = "Experience")
	int32 GetRequiredExperience() const;

	UPROPERTY(BlueprintAssignable, Category = "Experience")
	FOnLevelUp OnLevelUp;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Experience")
	int32 CurrentExperience = 0;

	UPROPERTY(VisibleAnywhere, Category = "Experience")
	int32 CurrentLevel = 1;

	UPROPERTY(EditAnywhere, Category = "Experience")
	int32 RequiredExperience = 100;

	void LevelUp();
};