// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"
#include "TileGenerator.h"
#include "MainCharacter.h"
#include "FollowingGasBase.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MOUSPRINT_API AMainGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
private:
	bool bIsGameStarted = false;
	bool bIsGameOver = false;
	bool bIsTutorialEnd = false; 
	float DeltaTimeSum = 0;
	float StageEndTime = 0;

	ATileGenerator* TileGenerator;
	AMainCharacter* MainCharacter;
	AFollowingGasBase* FollowingGas;

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
		int32 Stage = 0;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
		float Score = 0.0f;

	UPROPERTY(EditAnywhere, Category = Gameplay)
		class UDataTable* StageInfoTable;

	UPROPERTY(EditAnywhere, Category = Gameplay)
		TSubclassOf<class ATileGenerator> TileGeneratorClass;

	UPROPERTY(EditAnywhere, Category = Gameplay)
		TSubclassOf<class AFollowingGasBase> FollowingGasClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	AMainGameModeBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void UpdateStageInfo(const float DeltaTime);

	UFUNCTION()
		void CheckGameOver();

	UFUNCTION()
		float SetStage(const int32 stage_);

	UFUNCTION(BlueprintCallable)
		bool InitGame();

	UFUNCTION(BlueprintCallable)
		bool GameStart();

	UFUNCTION(BlueprintCallable)
		float GetDistanceGasToPlayer() const;

	UFUNCTION(BlueprintCallable)
		bool GetTutorialIsEnd() const;

	UFUNCTION(BlueprintCallable)
		void SetIsTutorialEnd(const bool flag);

	UFUNCTION(BlueprintCallable)
		bool GetGameIsOver() const;
};
