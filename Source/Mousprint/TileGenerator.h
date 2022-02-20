// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "TileBasic.h"
#include "Containers/Array.h"
#include "TileGenerator.generated.h"

UCLASS()
class MOUSPRINT_API ATileGenerator : public AActor
{
	GENERATED_BODY()

private:
	TArray<ATileBasic*> SpawnedTileArr; //스폰된 타일의 주소를 저장하는 배열
	int32 Stage = 0;
	int32 StraightTileMinIdx = 0; //스폰할 타일 액터의 시작 Idx
	int32 StraightTileMaxIdx = 0; //스폰할 타일 액터의 끝 Idx
	int32 CurveTileMinIdx = 0; //스폰할 타일 액터의 시작 Idx
	int32 CurveTileMaxIdx = 0; //스폰할 타일 액터의 끝 Idx

	bool bIsStageChanged = false;
	bool prevCurveTileType = 0; //0 왼쪽 1 오른쪽
	bool prevTileType; //이전에 스폰된 타일 유형 - 0 직선, 1 커브
	int32 prevTileIdx = -1; //이전에 스폰된 타일의 Idx

	bool bIsSpawningTile = false; //타일이 겹치게 스폰되는 것을 방지하는 변수


public:	
	// Sets default values for this actor's properties
	ATileGenerator();
	
	UPROPERTY(EditAnywhere, Category = SpawnInfo)
		float CurveTileSpawnPercentage = 10.0f;

	UPROPERTY(EditAnywhere, Category = SpawnInfo)
		int32 MaxSpawnTileCnt = 5; //맵에 스폰될 최대 타일 개수, SpawnedTileArr의 최대 크기

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpawnInfo)
		TArray<TSubclassOf<class ATileBasic> > TileClassArray; //실제로 스폰될 타일의 BP들이 원소로 들어있음

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpawnInfo)
		TArray<TSubclassOf<class ATileBasic> > BeginTileClassArray;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void SpawnNextTile();

	UFUNCTION()
		void DestroyHeadTile();

	UFUNCTION()
		FTransform GetNextSpawnTransform() const; //다음 스폰될 타일의 위치를 반환

	UFUNCTION()
		int32 GetNextSpawnTileIdx();

	UFUNCTION()
		ATileBasic* SpawnTile(TSubclassOf<class ATileBasic>& SpawnTarget); //타일을 스폰
																 //_bIsInit : 최초 스폰 시 true로 전달 
																 //TileIdx : 스폰할 타일 클래스의 idx를 전달
	UFUNCTION(BlueprintCallable)
		bool SetSpawnTileIdxRange(const int32 stage_, const int32 start_straight, const int32 finish_straight
								, const int32 start_curve, const int32 finish_curve);

	UFUNCTION()
		void DestroyTile(ATileBasic* target); //Target 타일을 제거
};
