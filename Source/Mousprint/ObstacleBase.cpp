// Fill out your copyright notice in the Description page of Project Settings.


#include "ObstacleBase.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AObstacleBase::AObstacleBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT_COMPONENT")); //RootComponent 지정
	DefaultSceneRoot->SetupAttachment(RootComponent);
	SetRootComponent(DefaultSceneRoot); //루트 컴포넌트화

	//Obstacle Child Actor Component 초기화
	for (int idx = 0; idx < MaxObstacleCnt; idx++)
	{
		FName ObstacleName = FName("OBSTACLE_" + FString::FromInt(idx));
		ObstacleComponentArray.Add(CreateDefaultSubobject<UChildActorComponent>(ObstacleName));
		ObstacleComponentArray[idx]->SetupAttachment(RootComponent);

		bIsArrived.Add(false);
	}
	//Obstacle Path Component 초기화
	for (int idx = 0; idx < MaxObstacleCnt * 2; idx++)
	{
		FName ObstacleName = FName("Path_" + FString::FromInt(idx/2) + "_" + FString::FromInt(idx % 2));
		PathComponentArray.Add(CreateDefaultSubobject<USphereComponent>(ObstacleName));
		PathComponentArray[idx]->SetupAttachment(RootComponent);
	}
}

// Called when the game starts or when spawned
void AObstacleBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AObstacleBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateObstacleLocation();
}

void AObstacleBase::UpdateObstacleLocation()
{
	for (int idx = 0; idx < UseObstacleCount; idx++)
	{
		if (!bIsMovingObstacle.IsValidIndex(idx) || !bIsMovingObstacle[idx]) continue;
		//UE_LOG(LogTemp, Warning, TEXT("ObstacleBase : UpdateObstacle ID#%d to %d"), idx, bIsArrived[idx] ? idx * 2 + 1 : idx * 2);
		MoveObstacle(idx, bIsArrived[idx] ? idx * 2 + 1 : idx * 2);
	}
}

void AObstacleBase::MoveObstacle(const int idx, const int target_idx)
{
	FVector CurrLocation = ObstacleComponentArray[idx]->GetComponentLocation();
	FVector TargetLocation = PathComponentArray[target_idx]->GetComponentLocation();
	FVector MoveDirection = UKismetMathLibrary::FindLookAtRotation(CurrLocation, TargetLocation).Vector();
	float MoveSpeed = ObstacleMoveSpeed.IsValidIndex(idx) ? ObstacleMoveSpeed[idx] : DefaultObstacleMoveSpeed;
	FVector NewLocation = CurrLocation + MoveDirection * MoveSpeed;

	if (TargetLocation.Equals(NewLocation, 3.0f)) bIsArrived[idx] = !bIsArrived[idx];
	
	ObstacleComponentArray[idx]->SetWorldLocation(NewLocation);
}