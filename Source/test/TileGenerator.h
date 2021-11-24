// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileBasic.h"
#include "TileGenerator.generated.h"

UCLASS()
class TEST_API ATileGenerator : public AActor
{
	GENERATED_BODY()

private:
	ATileBasic* curr;
	ATileBasic* prev;

public:	
	// Sets default values for this actor's properties
	ATileGenerator();

	UPROPERTY(BlueprintReadWrite)
		bool bCanSpawnTile = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		FTransform GetNextSpawnTransform() const;

	UFUNCTION(BlueprintCallable)
		void SetCurrTile(ATileBasic * tmp);

	UFUNCTION(BlueprintCallable)
		void DestroyPrevTile();
};
