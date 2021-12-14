// Fill out your copyright notice in the Description page of Project Settings.

#include "Math/UnrealMathUtility.h"
#include "TileGenerator.h"

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

	SpawnedTileArr.Push(SpawnTile(false, 0));
}

// Called every frame
void ATileGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SpawnedTileArr.Num() <= MaxSpawnTileCnt && !bIsSpawningTile)
	{
		bIsSpawningTile = true;
		SpawnedTileArr.Push(SpawnTile(false, 0));
		bIsSpawningTile = false;
	}
	if (SpawnedTileArr.IsValidIndex(1)
		&& SpawnedTileArr[1]->IsOverlapped())
	{
		DestroyTile(SpawnedTileArr[0]);
		SpawnedTileArr.RemoveAt(0);
	}
}

FTransform ATileGenerator::GetNextSpawnTransform() const
{
	if (SpawnedTileArr.Num() == 0) return FTransform();

	return SpawnedTileArr.Last()->GetNextSpawnPoint();

	/*if (curr == nullptr) return FTransform();
	return curr->GetNextSpawnPoint();*/
}

void ATileGenerator::SetCurrTile(ATileBasic* tmp)
{
	if (tmp == nullptr) return;

	prev = curr;
	curr = tmp;
}

ATileBasic* ATileGenerator::SpawnTile(const bool _bIsInit, const int TileIdx)
{
	if (!GetWorld() || TileClassArray.GetAllocatedSize() <= TileIdx) return nullptr;
	if (!TileClassArray[TileIdx] ) return nullptr;

	UE_LOG(LogTemp, Warning, TEXT("InitTile"));

	FRotator BeginRotation = FRotator(0, 0, 0);
	FVector BeginLocation;	
	if (_bIsInit) BeginLocation = FVector(0, 0, 0);
	else BeginLocation = GetNextSpawnTransform().GetLocation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	ATileBasic* NewTile = GetWorld()->SpawnActor<ATileBasic>(TileClassArray[TileIdx], BeginLocation, BeginRotation);

	if (NewTile) SetCurrTile(NewTile);
	
	return NewTile;
}

void ATileGenerator::DestroyPrevTile()
{
	if (prev == nullptr) return;

	prev->Destroy();
	prev = nullptr;
}

void ATileGenerator::DestroyTile(ATileBasic * target)
{
	if (target == nullptr) return;
	target->Destroy();
}