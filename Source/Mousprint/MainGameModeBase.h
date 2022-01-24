// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"
#include "TileGenerator.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MOUSPRINT_API AMainGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		int32 Stage = 0;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		float Score = 0.0f;

	UPROPERTY(EditAnywhere)
		class UDataTable* StageInfoTable;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ATileGenerator> TileGeneratorClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	AMainGameModeBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
