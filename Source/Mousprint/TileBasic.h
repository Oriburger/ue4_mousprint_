// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Containers/Array.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "TileBasic.generated.h"

UCLASS()
class MOUSPRINT_API ATileBasic : public AActor
{
	GENERATED_BODY()

private:
	TArray<AActor*> SpawnedObstacleArr; //스폰된 장애물의 주소를 저장하는 배열

	bool OverlapFlag = false; //플레이어가 BoxTrigger에 Overlapped 되었는가?
	static const int32 MaxObstacleCount = 16; 
	int SpawnedObstacleCnt = 0; //현재까지 스폰된 장애물의 수
	int32 TileIdx = -1; //타일의 Idx 정보

public:	
	// Sets default values for this actor's properties
	ATileBasic();

	UPROPERTY(EditAnywhere, Category = Gameplay)
		int32 UseObstacleCount = 10;  //배치되어있는 장애물 스폰 볼륨의 수

	UPROPERTY(EditAnywhere, Category = Gameplay)
		int32 MaxSpawnObstacleCount = 10; //배치된 스폰 볼륨 중에서 실제로 스폰할 장애물의 수 

	UPROPERTY(EditAnywhere, Category = Gameplay)
		float ObstacleSpawnPercentage = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float TileSpawnPercentage = 100.0f;

	UPROPERTY(VisibleDefaultsOnly)
		USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleDefaultsOnly)
		UArrowComponent* EdgeArrowComponent; //타일의 스폰 방향/위치를 나타내는 Arrow

	UPROPERTY(VisibleDefaultsOnly)
		UBoxComponent* BoxTrigger; //플레이어의 Overlapped를 감지하는 볼륨

	UPROPERTY(EditAnywhere, Category = Obstacle)
		TSubclassOf<class AActor> ObstacleClass;

	UPROPERTY(VisibleDefaultsOnly)
		TArray<USphereComponent*> ObstacleSpawnPointArray;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		FTransform GetNextSpawnPoint() const; //Arrow의 Transform 반환
	
	UFUNCTION()
		bool InitObstacle();

	UFUNCTION()
		void DestroyObstacle();

	UFUNCTION()
		bool IsOverlapped() const; //플레이어가 BoxTrigger에 Overlapped 되었는가?

	UFUNCTION()
		int32 GetTileIdx() const;

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
								, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
