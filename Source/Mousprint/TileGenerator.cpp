// Fill out your copyright notice in the Description page of Project Settings.

#include "TileGenerator.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ATileGenerator::ATileGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->Tags = { "Tile Generator" };
}

// Called when the game starts or when spawned
void ATileGenerator::BeginPlay()
{
	Super::BeginPlay();
	
	ATileBasic* InitialTile = SpawnTile(BeginTileClassArray[Stage - 1]);
	if (InitialTile) SpawnedTileArr.Push(InitialTile); //게임 시작 시 하나는 스폰
}

// Called every frame
void ATileGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//스폰 된 타일의 수가 MaxSpawnTileCnt보다 작고, 스폰 작업 중이 아니라면
	if (SpawnedTileArr.Num() <= MaxSpawnTileCnt && !bIsSpawningTile)
	{
		//UE_LOG(LogTemp, Warning, TEXT("TileGenerator : Tick!"));
		//bIsSpawningTile = true; //중복 스폰 방지

		ATileBasic* SpawnedTile = nullptr;
		int32 nextTileIdx = GetNextSpawnTileIdx();  
	
		UE_LOG(LogTemp, Warning, TEXT("TileGenerator : next tile idx is %d"), nextTileIdx);

		if (nextTileIdx != -1 && TileClassArray.IsValidIndex(nextTileIdx))
		{
			SpawnedTile = SpawnTile(TileClassArray[nextTileIdx]);
			if(SpawnedTile != nullptr)	SpawnedTileArr.Push(SpawnedTile);//Spawn 된 타일을 Arr에 넣음
		}
		//bIsSpawningTile = false;
	}

	if ((SpawnedTileArr.IsValidIndex(10) && SpawnedTileArr[10]->IsOverlapped()) //플레이어가 10번째 타일의 오버랩 볼륨에 닿았다면
		|| (SpawnedTileArr.IsValidIndex(11) && SpawnedTileArr[11]->IsOverlapped()))
	{
		ATileBasic* DestoyTarget = SpawnedTileArr[0];
		if (DestoyTarget != nullptr && IsValid(DestoyTarget))
		{
			UE_LOG(LogTemp, Warning, TEXT("TileGenerator : Tile Removed"));
			DestoyTarget->DestroyObstacle();
			DestoyTarget->Destroy();  //0번째 타일을 Destroy
		}
		SpawnedTileArr.RemoveAt(0); //배열로부터 제거
	}
}

FTransform ATileGenerator::GetNextSpawnTransform() const
{
	if (SpawnedTileArr.Num() == 0) return FTransform(); //배열이 비었다면 기본 Transform 반환
	return SpawnedTileArr.Last()->GetNextSpawnPoint(); //맨 앞의 타일의 Arrow Transform 반환
}

int32 ATileGenerator::GetNextSpawnTileIdx() 
{
	int32 nextTileIdx = -1;

	/*--- 다음 스폰할 타일의 Idx와 유형을 무작위로 선택 ---*/
	bool nextTileType = (FMath::RandRange(0, 100) < CurveTileSpawnPercentage);

	if (nextTileType)
	{
		do
		{
			nextTileIdx = FMath::RandRange(CurveTileMinIdx, CurveTileMaxIdx); //맨 시작 타일은 제외
		} while (prevTileType == nextTileType && prevTileIdx == nextTileIdx);
	}
	else
	{
		do
		{
			nextTileIdx = FMath::RandRange(StraightTileMinIdx, StraightTileMaxIdx); //맨 시작 타일은 제외
		} while (prevTileType == nextTileType && prevTileIdx == nextTileIdx);
	}

	/*--- 곡선 타일로 인해 타일들이 겹치는 현상 방지 ---*/
	if (nextTileType)
	{
		if (prevCurveTileType && nextTileIdx % 2 == 1) nextTileIdx -= 1;
		else if (!prevCurveTileType && nextTileIdx % 2 == 0) nextTileIdx += 1;
		prevCurveTileType = (nextTileIdx % 2 == 1);
	}

	prevTileType = nextTileType;
	prevTileIdx = nextTileIdx; 

	return nextTileIdx;
}

ATileBasic* ATileGenerator::SpawnTile(TSubclassOf<class ATileBasic>& SpawnTarget)
{
	if (!GetWorld()) return nullptr; 
	if (SpawnTarget == nullptr || !IsValid(SpawnTarget)) return nullptr;	

	//SpawnTransform 및 인자 지정
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(GetNextSpawnTransform().GetLocation());
	SpawnTransform.SetRotation(GetNextSpawnTransform().GetRotation());

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this; //타일의 소유자는 Generator
	SpawnParams.Instigator = GetInstigator(); 
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//타일을 스폰하고 반환 (SpawnActorDeferred 함수 : BeginPlay 호출을 지연시킴)
	ATileBasic* NewTile = GetWorld()->SpawnActor<ATileBasic>(SpawnTarget, SpawnTransform, SpawnParams);
	
	return NewTile;
}

bool ATileGenerator::SetSpawnTileIdxRange(const int32 start_straight, const int32 finish_straight
										, const int32 start_curve, const int32 finish_curve)
{
	if (start_straight > finish_straight || start_curve > finish_curve) return false;

	StraightTileMinIdx = start_straight;
	StraightTileMaxIdx = finish_straight;
	CurveTileMinIdx = start_curve;
	CurveTileMaxIdx = finish_curve;

	return true;
}

void ATileGenerator::DestroyTile(ATileBasic * target)
{
	if (target == nullptr) return; //target이 null 이라면 Destroy 하지 않음
	target->Destroy();
}