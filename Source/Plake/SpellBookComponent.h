// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Spell.h"
#include "SpellBookComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLAKE_API USpellBookComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpellBookComponent();

	// GETTERS and SETTERS
	float GetActiveManaCost() { if (!SelectedSpell) return 0;  return SelectedSpell->GetManaCost(); };

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StartCast();
	void EndCast();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class ASpell> BasicSpellClass;
	
	UPROPERTY(ReplicatedUsing = OnRep_BasicSpell)
		ASpell* BasicSpell;
	UPROPERTY(Replicated)
		ASpell* SelectedSpell;

	UFUNCTION()
		void OnRep_BasicSpell();

	UFUNCTION(Server, Reliable)
		void Server_SpawnBasicSpell();

		
};
