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

	SpawnTile(true, 0);
}

// Called every frame
void ATileGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (curr != nullptr && curr->IsOverlapped() && !bIsSpawningTile)
	{
		bIsSpawningTile = true;
		SpawnTile(false, FMath::RandRange(0, ATileBasic::GetTileTypeCount()-1));
	}
}

FTransform ATileGenerator::GetNextSpawnTransform() const
{
	if (curr == nullptr) return FTransform();

	return curr->GetNextSpawnPoint();
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

	ATileBasic* Tile = GetWorld()->SpawnActor<ATileBasic>(TileClassArray[TileIdx], BeginLocation, BeginRotation);

	if (Tile) SetCurrTile(Tile);
	bIsSpawningTile = false;
	
	return Tile;
}

void ATileGenerator::DestroyPrevTile()
{
	if (prev == nullptr) return;

	prev->Destroy();
	prev = nullptr;
}