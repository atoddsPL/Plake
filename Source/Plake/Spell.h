// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpellProjectileBase.h"
#include "Spell.generated.h"

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

public:	
		
	UFUNCTION()
	void StartCast();
	UFUNCTION()
	void EndCast();
	UFUNCTION()
	void InstantCast();
	
	
	// GETTERS AND SETTERS
	UFUNCTION()
	float GetManaCost() { return ManaCost; };
};
