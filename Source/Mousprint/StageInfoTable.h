// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "StageInfoTable.generated.h"

USTRUCT(BlueprintType)
struct FStageInfoTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 Stage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float EndTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 MinTileIdx_Straight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 MaxTileIdx_Straight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 MinTileIdx_Curve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 MaxTileIdx_Curve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MinPlayerSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MaxPlayerSpeed;
};

UCLASS()
class MOUSPRINT_API AStageInfoTable : public AActor
{
	GENERATED_BODY()
};
