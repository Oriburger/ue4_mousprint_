// Copyright Epic Games, Inc. All Rights Reserved.


#include "MainGameModeBase.h"
#include "StageInfoTable.h"
#include "MainCharacter.h"

ATileGenerator* TileGenerator;
AMainCharacter* MainCharacter;

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

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this; //타일의 소유자는 Generator
	SpawnParams.Instigator = GetInstigator();

	TileGenerator = GetWorld()->SpawnActor<ATileGenerator>(TileGeneratorClass, SpawnParams);
	if (TileGenerator == nullptr) return;

	if (StageInfoTable != nullptr)
	{
		FStageInfoTableRow* StageInfoRow = StageInfoTable->FindRow<FStageInfoTableRow>
				(FName(*(FString::FormatAsNumber(Stage))), FString(""));
		
		TileGenerator->SetSpawnTileIdxRange((*StageInfoRow).MinTileIdx_Straight, (*StageInfoRow).MaxTileIdx_Straight
											,(*StageInfoRow).MinTileIdx_Curve, (*StageInfoRow).MaxTileIdx_Curve);

		MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		MainCharacter->SetWalkSpeedLimit((*StageInfoRow).MinPlayerSpeed, (*StageInfoRow).MaxPlayerSpeed);
	}
}

// Called every frame
void AMainGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!MainCharacter->GetPlayerIsDead())
	{
		Score = Score + DeltaTime * 10;
	}
}