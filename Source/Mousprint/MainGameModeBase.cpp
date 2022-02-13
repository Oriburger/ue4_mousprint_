// Copyright Epic Games, Inc. All Rights Reserved.


#include "MainGameModeBase.h"
#include "StageInfoTable.h"

// Sets default values
AMainGameModeBase::AMainGameModeBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMainGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AMainGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateStageInfo(DeltaTime);
	CheckGameOver();
}

void AMainGameModeBase::UpdateStageInfo(const float DeltaTime)
{
	if (!bIsGameStarted) return;
	if (StageEndTime != 0 && !MainCharacter->GetPlayerIsDead())
	{
		Score = Score + DeltaTime * 10;
		DeltaTimeSum += DeltaTime;

		if (DeltaTimeSum >= StageEndTime)
		{
			StageEndTime = SetStage(++Stage);
		}
	}
}

void AMainGameModeBase::CheckGameOver()
{
	if (!bIsGameStarted || MainCharacter == nullptr) return;

	if (MainCharacter->GetPlayerIsDead())
	{
		bIsGameOver = true;
		bIsGameStarted = false;
	}
}

float AMainGameModeBase::SetStage(const int32 stage_)
{
	if (StageInfoTable == nullptr) return -1; 

	FStageInfoTableRow* StageInfoRow = StageInfoTable->FindRow<FStageInfoTableRow>
		(FName(*(FString::FormatAsNumber(stage_))), FString(""));

	if (StageInfoRow == nullptr) return -1;

	TileGenerator->SetSpawnTileIdxRange((*StageInfoRow).MinTileIdx_Straight, (*StageInfoRow).MaxTileIdx_Straight
		, (*StageInfoRow).MinTileIdx_Curve, (*StageInfoRow).MaxTileIdx_Curve);

	MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	MainCharacter->SetWalkSpeedLimit((*StageInfoRow).MinPlayerSpeed, (*StageInfoRow).MaxPlayerSpeed);
	FollowingGas->SetMoveSpeedLimit((*StageInfoRow).MinPlayerSpeed * 0.9f, (*StageInfoRow).MaxPlayerSpeed * 0.9f);
	
	return (*StageInfoRow).EndTime;
}

bool AMainGameModeBase::GameStart()
{
	if (!GetWorld()) return false;

	bIsGameStarted = true;
	FActorSpawnParameters SpawnParams;
	FTransform SpawnTransform;
	SpawnParams.Owner = this; //타일의 소유자는 Generator
	SpawnParams.Instigator = GetInstigator();

	SpawnTransform.SetLocation({ -7500.0f, 0.0f, 1000.0f });
	TileGenerator = GetWorld()->SpawnActor<ATileGenerator>(TileGeneratorClass, SpawnParams);
	FollowingGas = GetWorld()->SpawnActor<AFollowingGasBase>(FollowingGasClass, SpawnTransform, SpawnParams);
	if (TileGenerator == nullptr || FollowingGas == nullptr) return false;

	StageEndTime = SetStage(Stage);
	return true;
}

float AMainGameModeBase::GetDistanceGasToPlayer() const
{
	if (MainCharacter == nullptr || FollowingGas == nullptr) return -1.0f;

	return FVector::Distance(MainCharacter->GetActorLocation(), FollowingGas->GetActorLocation()) - 1450.0f;
}

bool AMainGameModeBase::GetGameIsOver() const { return bIsGameOver;  }