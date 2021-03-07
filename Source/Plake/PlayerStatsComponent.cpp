// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatsComponent.h"

// Sets default values for this component's properties
UPlayerStatsComponent::UPlayerStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);
	// ...
}


// Called when the game starts
void UPlayerStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPlayerStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UPlayerStatsComponent::ChangeHealth(float Delta)
{
	CurrentHealth += Delta;
	if (CurrentHealth < 0) CurrentHealth = 0;
	if (CurrentHealth > MaxHealth) CurrentHealth = MaxHealth;
	return CurrentHealth;
}

