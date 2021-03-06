// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellBookComponent.h"
#include "Spell.h"

// Sets default values for this component's properties
USpellBookComponent::USpellBookComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USpellBookComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!BasicSpellClass)
	{
		FString Name = GetName();
		UE_LOG(LogTemp, Warning, TEXT("Spell class not found on %s"), *Name);
		return;
	}
	BasicSpell =  Cast<ASpell>(GetWorld()->SpawnActor(BasicSpellClass));
	
	// SET SPELL OWNER TO PLAYER
	BasicSpell->SetOwner(GetOwner());


	if (!BasicSpell)
	{
		FString Name = GetName();
		UE_LOG(LogTemp, Warning, TEXT("Failed Creating Basic Spell instance on %s"), *Name);
		return;
	}

	SelectedSpell = BasicSpell;
	// ...
	
}


// Called every frame
void USpellBookComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USpellBookComponent::StartCast()
{
	SelectedSpell->StartCast();
}

void USpellBookComponent::EndCast()
{
	SelectedSpell->EndCast();
}

