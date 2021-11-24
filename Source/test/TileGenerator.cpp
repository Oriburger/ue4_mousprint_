// Fill out your copyright notice in the Description page of Project Settings.


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

	//Init Spawn은 파생 BP 클래스에서 진행
}

// Called every frame
void ATileGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (curr != nullptr && curr->IsOverlapped())
	{
		bCanSpawnTile = true;
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

void ATileGenerator::DestroyPrevTile()
{
	if (prev == nullptr) return;

	prev->Destroy();
	prev = nullptr;
}