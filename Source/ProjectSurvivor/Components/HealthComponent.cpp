// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::ApplyDamage(float Damage)
{
	if (Damage <= 0.0f) {
		return;
	}

	if (IsDead()) {
		return;
	}

	CurrentHealth -= Damage;

	if (CurrentHealth < 0.0f) {
		CurrentHealth = 0.0f;
	}

}

bool UHealthComponent::IsDead() const
{
	return CurrentHealth <= 0.0f;
}

float UHealthComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

