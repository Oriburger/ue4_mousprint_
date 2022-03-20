// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveInfo.generated.h"

/**
 * 
 */
UCLASS()
class MOUSPRINT_API USaveInfo : public USaveGame
{
	GENERATED_BODY()

public:
	
	UPROPERTY(VisibleAnywhere, Category = Gameplay)
		bool bIsTutorialEnd = false;

	UPROPERTY(VisibleAnywhere, Category = Gameplay)
		float HighScore = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = Gameplay)
		FString SaveSlotName = "Default";

	UPROPERTY(VisibleAnywhere, Category = Gameplay)
		uint32 UserIndex = 0; 
};
