// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Array.h"
#include "ObstacleBase.generated.h"

UCLASS()
class MOUSPRINT_API AObstacleBase : public AActor
{
	GENERATED_BODY()

private:
	const float DefaultObstacleMoveSpeed = 3.0f; //장애물의 기본 이동 속도
	const int32 MaxObstacleCnt = 10; //총 장애물 개수
	TArray<bool> bIsArrived; //두번째 지점에 도착을 했는지 여부

public:	
	// Sets default values for this actor's properties
	AObstacleBase();
	
	UPROPERTY(VisibleDefaultsOnly)
		USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere, Category = Obstacle)
		TArray<UChildActorComponent*> ObstacleComponentArray; //장애물 컴포넌트

	UPROPERTY(EditAnywhere, Category = Gameplay)
		int32 UseObstacleCount = 5; //실제로 사용할 장애물의 개수

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TArray<bool> bIsMovingObstacle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TArray<float> ObstacleMoveSpeed;

	UPROPERTY(EditAnywhere, Category = Gameplay)
		TArray<USphereComponent*> PathComponentArray;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void UpdateObstacleLocation();

	UFUNCTION()
		void MoveObstacle(const int idx, const int target_idx);
};
