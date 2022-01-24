// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "TileSpawnInfoTable.generated.h"

USTRUCT(BlueprintType)
struct FTileSpawnInfoTableRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 Stage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float SpawnPercentage_Straight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float SpawnPercentage_Curve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float SpawnPercentage_Obstacle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString Description;
};


UCLASS()
class MOUSPRINT_API ATileSpawnInfoTable : public AActor
{
	GENERATED_BODY()

};
