// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellBookComponent.h"
// GAME
#include "Spell.h"
// ENGINE
#include "Net/UnrealNetwork.h"
// Sets default values for this component's properties
USpellBookComponent::USpellBookComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);
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
	
	if (GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("i has auth"));
		if (!BasicSpellClass) return;
		BasicSpell = Cast<ASpell>(GetWorld()->SpawnActor(BasicSpellClass));

		// SET SPELL OWNER TO PLAYER
		BasicSpell->SetOwner(GetOwner());
		if (!BasicSpell)
		{
			FString Name = GetName();
			UE_LOG(LogTemp, Warning, TEXT("Failed Creating Basic Spell instance on %s"), *Name);
			return;
		}

		SelectedSpell = BasicSpell;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("i NOT has auth"));
		Server_SpawnBasicSpell();
	}

	
	// ...
	
}

void USpellBookComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USpellBookComponent, BasicSpell);
}

// Called every frame
void USpellBookComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USpellBookComponent::StartCast()
{
	if (!SelectedSpell) return;
	SelectedSpell->StartCast();
}

void USpellBookComponent::EndCast()
{
	if (!SelectedSpell) return;
	SelectedSpell->EndCast();
}

void USpellBookComponent::OnRep_BasicSpell()
{
	UE_LOG(LogTemp, Warning, TEXT("Replication"));
	if (!BasicSpell) return;
	SelectedSpell = BasicSpell;
}

void USpellBookComponent::Server_SpawnBasicSpell_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("SERVER SPANWBASIC"));
	if (!BasicSpellClass) return;
	BasicSpell = Cast<ASpell>(GetWorld()->SpawnActor(BasicSpellClass));
	
	// SET SPELL OWNER TO PLAYER
	BasicSpell->SetOwner(GetOwner());

	return;
}

