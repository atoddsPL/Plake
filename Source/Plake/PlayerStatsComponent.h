// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "PlayerStatsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLAKE_API UPlayerStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerStatsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

// GETTERS AND SETTERS
	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth() { return CurrentHealth; };

	UFUNCTION(BlueprintCallable)
	float GetCurrentHealthPercent() { return (CurrentHealth/MaxHealth); };

	UFUNCTION(BlueprintCallable)
	float GetCurrentMana() { return CurrentMana; };

	UFUNCTION(BlueprintCallable)
	float GetCurrentManaPercent() { return (CurrentMana / MaxMana); };

	/* Changes Health and returns remaining amount. 0 = Dead*/
	float ChangeHealth(float Delta);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"), Category = "Health")
	float MaxHealth = 100.f;

	float CurrentHealth = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"), Category = "Mana")
		float MaxMana = 100.f;
	float CurrentMana = 100.f;

};
