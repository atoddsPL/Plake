// Fill out your copyright notice in the Description page of Project Settings.


#include "Spell.h"
// GAME
#include "PlakeCharacter.h"

// ENGINE
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/DamageType.h"



// Sets default values
ASpell::ASpell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

// Called when the game starts or when spawned
void ASpell::BeginPlay()
{
	Super::BeginPlay();
	
}





void ASpell::StartCast()
{
	if (MaxCastTime > 0)
	{
		// tu dorobic obsluge castowanych spelli
	}
	else
	{ // rzucamy instant casta
		InstantCast();
	}
}

void ASpell::EndCast()
{
}


void ASpell::InstantCast()
{
	
	if (bSpawnsProjectile && ProjectileClass != nullptr)
	{
		FVector Location;
		FRotator Rotation;

		GetProjectileSpawnPoint(Location, Rotation);
		
		if (GetNetMode() != ENetMode::NM_Client)
		{
			SpawnProjectile(Location, Rotation);
			UE_LOG(LogTemp, Warning, TEXT("HasAuthority"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("NoAuthority"));
			Server_ProjectileSpellCast(Location, Rotation);
		}
		
	}
}

void ASpell::Server_ProjectileSpellCast_Implementation(const FVector& Location, const FRotator& Rotation)
{
	UE_LOG(LogTemp, Warning, TEXT("Server_"));
		SpawnProjectile(Location, Rotation);		
}

//bool ASpell::Server_ProjectileSpellCast_Validate(const FVector& Location, const FRotator& Rotation)
//{
//	return true;
//}

void ASpell::SpawnProjectile(const FVector& Location, const FRotator& Rotation)
{
	APlakeCharacter* Ownz = Cast<APlakeCharacter>(GetOwner());
	if (!Ownz) return;
	if (!Ownz->ManaCheck(ManaCost)) return;
	UE_LOG(LogTemp, Warning, TEXT("Spawn"));
	ASpellProjectileBase* SpellProjectile = Cast<ASpellProjectileBase>(GetWorld()->SpawnActor(ProjectileClass, &Location, &Rotation));
	if (SpellProjectile)
	{
		SpellProjectile->InitialLifeSpan = 3.0f;
		SpellProjectile->SetOwner(this);
	}
	
}

void ASpell::GetProjectileSpawnPoint(FVector& Location, FRotator& Rotation)
{
	AActor* Ownz = GetOwner();

	if (!Ownz) {
		UE_LOG(LogTemp, Warning, TEXT("ASOIDN"));
		return;
	}
	APlakeCharacter* ActorOwner = Cast<APlakeCharacter>(Ownz);
	if (!Ownz) return;
	
	Location = ActorOwner->GetActorLocation() + ActorOwner->GetActorForwardVector().GetSafeNormal() * 100.f + FVector(0, 20.f, 0);
	Rotation = ActorOwner->GetActorRotation();
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FVector PCLoc;
	FRotator PCRot;
	PC->GetPlayerViewPoint(PCLoc, PCRot);
	FVector EndLocation = PCLoc + PCRot.Vector() * 10000;
	FHitResult Hit;
	FCollisionQueryParams Param;

	GetWorld()->LineTraceSingleByChannel(Hit, PCLoc, EndLocation, ECollisionChannel::ECC_PhysicsBody, Param);
	//DrawDebugLine(GetWorld(), PCLoc, EndLocation, FColor::Red, true, 2.0f, SDPG_MAX, 2.0f);
	if (Hit.IsValidBlockingHit())
	{
		EndLocation = Hit.Location;
		UE_LOG(LogTemp, Warning, TEXT("Hit!"));
	}

	Rotation = (EndLocation - Location).GetSafeNormal().Rotation();
}



void ASpell::SpellHit(AActor* OtherActor, const FHitResult& SweepResult)
{
	switch (OnHitBehaviour)
	{
	case EOnHitBehaviour::DIRECTDAMAGE:
		DirectDamage(OtherActor, SweepResult);
	case EOnHitBehaviour::DOT:
	case EOnHitBehaviour::SPAWNEFFECT:
	default:
		break;
	}
}

void ASpell::DirectDamage(AActor* OtherActor, const FHitResult& SweepResult)
{
	if (!OtherActor) return;
	if (OtherActor->ActorHasTag(TEXT("Player")))
	{
		TSubclassOf<UDamageType> DamageType = UDamageType::StaticClass();
		UGameplayStatics::ApplyDamage(OtherActor, BaseDamage, nullptr, this, DamageType);
	
	}
}