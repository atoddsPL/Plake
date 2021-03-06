// Fill out your copyright notice in the Description page of Project Settings.


#include "Spell.h"
#include "PlakeCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"


// Sets default values
ASpell::ASpell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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
	AActor* Ownz = GetOwner();

	if (!Ownz) {
		UE_LOG(LogTemp, Warning, TEXT("ASOIDN"));
		return;
	}
	APlakeCharacter* ActorOwner = Cast<APlakeCharacter>(Ownz);

	if (bSpawnsProjectile && ProjectileClass != nullptr && ActorOwner!=nullptr)
	{
		UArrowComponent* Arro = ActorOwner->GetSpellArrowComponent();

		FVector Location = Arro->GetComponentLocation() + ActorOwner->GetActorLocation();
		FRotator Rotation = ActorOwner->GetActorRotation();
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		FVector PCLoc;
		FRotator PCRot;
		PC->GetPlayerViewPoint(PCLoc, PCRot);
		FVector EndLocation = PCLoc + PCRot.Vector() * 10000;
		FHitResult Hit;
		FCollisionQueryParams Param;
		
		GetWorld()->LineTraceSingleByChannel(Hit, PCLoc, EndLocation, ECollisionChannel::ECC_Visibility, Param);
		//DrawDebugLine(GetWorld(), PCLoc, EndLocation, FColor::Red, true, 2.0f, SDPG_MAX, 2.0f);
		if (Hit.IsValidBlockingHit())
		{
			EndLocation = Hit.Location;
			UE_LOG(LogTemp, Warning, TEXT("Hit!"));
		}
		
		Rotation = (EndLocation - Location).GetSafeNormal().Rotation();
		UE_LOG(LogTemp, Warning, TEXT("location: %s"), *Location.ToString());
		ASpellProjectileBase* SpellProjectile = Cast<ASpellProjectileBase>(GetWorld()->SpawnActor(ProjectileClass, &Location, &Rotation));
		SpellProjectile->InitialLifeSpan = 3.0f;
		

	}
}
