// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerStatsComponent.h"
#include "PlakePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PLAKE_API APlakePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
		UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateUI(UPlayerStatsComponent* PlayerStats);
};
