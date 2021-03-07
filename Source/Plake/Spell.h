// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpellProjectileBase.h"
#include "Spell.generated.h"

UENUM(BlueprintType)
enum class EOnHitBehaviour : uint8 {
	DIRECTDAMAGE = 0 UMETA(DisplayName = "Direct Damage"),
	SPAWNEFFECT = 1  UMETA(DisplayName = "Spawn Effect (requires class to be specified)"),
	DOT = 2     UMETA(DisplayName = "Damage Over Time")

};

UCLASS()
class PLAKE_API ASpell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpell();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	// SPELL STATS
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"), Category = "Spell details")
		float MaxCastTime = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"), Category = "Spell details")
		float ManaCost = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"), Category = "Spell details")
		bool bSpawnsProjectile = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"), Category = "Spell details")
		TSubclassOf<ASpellProjectileBase> ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"), Category = "Spell details")
		float BaseDamage = 10.0f;


// HIT

public:
	UFUNCTION()
		void SpellHit(AActor* OtherActor, const FHitResult& SweepResult);
private: 
	UFUNCTION()
		void DirectDamage(AActor* OtherActor, const FHitResult& SweepResult);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"), Category = "On Hit")
		EOnHitBehaviour OnHitBehaviour;

	UFUNCTION(Server, Reliable)
		void Server_ProjectileSpellCast(const FVector& Location, const FRotator& Rotation);

	UFUNCTION()
		void SpawnProjectile(const FVector& Location, const FRotator& Rotation);
public:	
		
	UFUNCTION()
	void StartCast();
	UFUNCTION()
	void EndCast();
	UFUNCTION()
	void InstantCast();

	void GetProjectileSpawnPoint(FVector& Location, FRotator& Rotation);
	
	
	
	// GETTERS AND SETTERS
	UFUNCTION()
	float GetManaCost() { return ManaCost; };
};
