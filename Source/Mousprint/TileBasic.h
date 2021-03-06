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

public:	
	// Sets default values for this actor's properties
	ATileBasic();

	UPROPERTY(EditAnywhere, Category = Gameplay)
		int32 MaxObstacleCount = 5;
	
	UPROPERTY(VisibleDefaultsOnly)
		USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleDefaultsOnly)
		UArrowComponent* EdgeArrowComponent; //타일의 스폰 방향/위치를 나타내는 Arrow

	UPROPERTY(VisibleDefaultsOnly)
		UBoxComponent* BoxTrigger; //플레이어의 Overlapped를 감지하는 볼륨

	UPROPERTY(EditAnywhere, Category = Obstacle)
		TArray<TSubclassOf<class AActor> > ObstacleClassArray;

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
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
								, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
