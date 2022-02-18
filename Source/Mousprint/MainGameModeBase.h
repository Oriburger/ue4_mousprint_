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

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
		float HighScore = 0.0f;

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
		void MaintainDistance(); //가스가 플레이어와 일정 거리 이상 멀어지지 않도록 함

	UFUNCTION()
		void CheckGameOver(); //게임오버를 체크

	UFUNCTION()
		float SetStage(const int32 stage_);

	UFUNCTION(BlueprintCallable)
		bool GameInit(); //게임에 필요한 기본적인 요소들을 스폰 (가스, 타일생성기 등)

	UFUNCTION(BlueprintCallable)
		bool GameStart(); //본격적으로 게임을 시작함

	UFUNCTION(BlueprintCallable)
		float GetDistanceGasToPlayer() const; //가스-플레이어 사이의 거리를 구함

	UFUNCTION(BlueprintCallable)
		bool GetTutorialIsEnd() const; //튜토리얼이 끝났는지 여부를 반환

	UFUNCTION(BlueprintCallable)
		void SetIsTutorialEnd(const bool flag);

	UFUNCTION(BlueprintCallable)
		bool GetGameIsOver() const;

	UFUNCTION()
		bool SaveGameInfo(bool bIsTutorialEnd_, float score_);
};
