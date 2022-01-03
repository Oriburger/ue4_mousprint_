// Fill out your copyright notice in the Description page of Project Settings.

#include "TileGenerator.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ATileGenerator::ATileGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATileGenerator::BeginPlay()
{
	Super::BeginPlay();
	SpawnedTileArr.Push(SpawnTile(true, 0, false)); //게임 시작 시 하나는 스폰
}

// Called every frame
void ATileGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//스폰 된 타일의 수가 MaxSpawnTileCnt보다 작고, 스폰 작업 중이 아니라면
	if (SpawnedTileArr.Num() <= MaxSpawnTileCnt && !bIsSpawningTile)
	{
		bIsSpawningTile = true; //중복 스폰 방지
		bool bIsCurve = FMath::RandBool();
		if(bIsCurve) SpawnedTileArr.Push(SpawnTile(false, FMath::RandRange(1, CurveTileClassArray.Num() - 1), bIsCurve)); //Spawn 된 타일을 Arr에 넣음
		else SpawnedTileArr.Push(SpawnTile(false, FMath::RandRange(1, StraightTileClassArray.Num() - 1), bIsCurve));
		 
		bIsSpawningTile = false;
	}
	if (SpawnedTileArr.IsValidIndex(10) //플레이어가 2번째 타일의 오버랩 볼륨에 닿았다면
		&& SpawnedTileArr[10]->IsOverlapped())
	{
		DestroyTile(SpawnedTileArr[0]); //0번째 타일을 Destroy
		SpawnedTileArr.RemoveAt(0); //배열로부터 제거
	}
}

FTransform ATileGenerator::GetNextSpawnTransform() const
{
	if (SpawnedTileArr.Num() == 0) return FTransform(); //배열이 비었다면 기본 Transform 반환
	return SpawnedTileArr.Last()->GetNextSpawnPoint(); //맨 앞의 타일의 Arrow Transform 반환
}

ATileBasic* ATileGenerator::SpawnTile(const bool _bIsInit, int TileIdx, bool bIsCurve)
{
	if (!GetWorld()) return nullptr; //Idx 정보가 Invalid 라면 nullptr 반환
	if (bIsCurve)
	{
		if (prevCurveTileType && TileIdx % 2 == 1) TileIdx -= 1;
		else if (!prevCurveTileType && TileIdx % 2 == 0) TileIdx += 1;
		prevCurveTileType = (TileIdx % 2 == 1);
	}

	TSubclassOf<class ATileBasic>& SpawnTarget = (bIsCurve ? CurveTileClassArray[TileIdx] : StraightTileClassArray[TileIdx]);
	if (SpawnTarget == nullptr) return nullptr;


	//UE_LOG(LogTemp, Warning, TEXT("InitTile")); //디버그 Log

	FRotator BeginRotation = GetNextSpawnTransform().GetRotation().Rotator();
	FVector BeginLocation;	
	if (_bIsInit) BeginLocation = FVector(0, 0, 0); //최초 스폰 시 초기 위치는 {0,0,0}
	else BeginLocation = GetNextSpawnTransform().GetLocation(); 

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this; //타일의 소유자는 Generator
	SpawnParams.Instigator = GetInstigator(); 

	//타일을 스폰하고 반환
	ATileBasic* NewTile = GetWorld()->SpawnActor<ATileBasic>(SpawnTarget, BeginLocation, BeginRotation);
	
	return NewTile;
}

void ATileGenerator::DestroyTile(ATileBasic * target)
{
	if (target == nullptr) return; //target이 null 이라면 Destroy 하지 않음
	target->Destroy();
}