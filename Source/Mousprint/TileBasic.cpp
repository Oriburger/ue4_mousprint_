// Fill out your copyright notice in the Description page of Project Settings.


#include "TileBasic.h"
#include "MainCharacter.h"

// Sets default values
ATileBasic::ATileBasic()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	this->Tags = { "Tile" };

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT_COMPONENT")); //RootComponent 지정
	DefaultSceneRoot->SetupAttachment(RootComponent);
	SetRootComponent(DefaultSceneRoot); //루트 컴포넌트화

	EdgeArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("EDGE_ARROW"));
	EdgeArrowComponent->SetupAttachment(RootComponent);
	EdgeArrowComponent->SetRelativeLocationAndRotation(FVector(1000.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
	EdgeArrowComponent->ArrowSize = 2.0f;

	BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BOX_TRIGGER"));
	BoxTrigger->SetupAttachment(RootComponent);
	BoxTrigger->SetRelativeScale3D(FVector(1.0f, 8.75f, 10.0f));
	BoxTrigger->SetRelativeLocation(FVector(500.0f, 0.0f, 300.0f));
	
	for (int idx = 0; idx < MaxObstacleCount; idx++)
	{
		FName ObstacleName = FName("OBSTACLE" + FString::FromInt(idx));
		ObstacleSpawnPointArray.Add(CreateDefaultSubobject<USphereComponent>(ObstacleName));
		ObstacleSpawnPointArray[idx]->SetupAttachment(RootComponent);
	}
}

// Called when the game starts or when spawned
void ATileBasic::BeginPlay()
{
	Super::BeginPlay();
	
	BoxTrigger->OnComponentEndOverlap.AddDynamic(this, &ATileBasic::OnEndOverlap); //오버랩 이벤트를 추가

	InitObstacle();

	if (UseObstacleCount > ObstacleSpawnPointArray.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Variable \"UseObstacle\" is Overflowed!!!"));
	}
}

// Called every frame
void ATileBasic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FTransform ATileBasic::GetNextSpawnPoint() const
{
	return EdgeArrowComponent->GetComponentTransform(); 
}

bool ATileBasic::InitObstacle()
{
	if (!GetWorld() || !ObstacleClass || ObstacleSpawnPointArray.Num() == 0) return false;
	
	const FActorSpawnParameters SpawnParams;

	//SpawnParams.Instigator = this;

	for(int i = FMath::RandRange(0, UseObstacleCount - 1); ;)
	{
		if (SpawnedObstacleCnt >= MaxSpawnObstacleCount) break;
		if (SpawnedObstacleCnt >= UseObstacleCount) break;

		const auto target = ObstacleSpawnPointArray[i];
		if (FMath::RandRange(1, 100) > 100.0f - ObstacleSpawnPercentage)
		{
			AActor* SpawnedObstacle = GetWorld()->SpawnActor<AActor>(ObstacleClass, target->GetComponentLocation(), target->GetComponentRotation());
			if (SpawnedObstacle == nullptr) continue;
			SpawnedObstacleArr.Push(SpawnedObstacle);
			SpawnedObstacleCnt++;
		}
		i = ((i + 1) % UseObstacleCount);
	}
	return true;
}

bool ATileBasic::IsOverlapped() const
{
	return OverlapFlag;
}

void ATileBasic::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
	, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor->ActorHasTag(TEXT("Player"))) return; //오버랩된 액터가 플레이어가 아니면 return;
	OverlapFlag = true;
}

void ATileBasic::DestroyObstacle()
{
	//스폰한 장애물들을 전부 반환
	if (SpawnedObstacleArr.Num() == 0) return;

	for(int idx=SpawnedObstacleArr.Num()-1; idx>=0; idx--)
	{
		if (SpawnedObstacleArr[idx] != nullptr) SpawnedObstacleArr[idx]->Destroy();
		if (SpawnedObstacleArr.IsValidIndex(idx)) SpawnedObstacleArr.RemoveAt(idx);
	}
}
