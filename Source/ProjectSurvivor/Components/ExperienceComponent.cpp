#include "Components/ExperienceComponent.h"

UExperienceComponent::UExperienceComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UExperienceComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UExperienceComponent::AddExperience(int32 Amount)
{
	if (Amount <= 0)
	{
		return;
	}

	CurrentExperience += Amount;

	while (CurrentExperience >= RequiredExperience)
	{
		CurrentExperience -= RequiredExperience;
		LevelUp();
	}
}

void UExperienceComponent::LevelUp()
{
	CurrentLevel++;
	RequiredExperience = CurrentLevel * 100;

	OnLevelUp.Broadcast(CurrentLevel);
}

int32 UExperienceComponent::GetCurrentExperience() const
{
	return CurrentExperience;
}

int32 UExperienceComponent::GetCurrentLevel() const
{
	return CurrentLevel;
}

int32 UExperienceComponent::GetRequiredExperience() const
{
	return RequiredExperience;
}