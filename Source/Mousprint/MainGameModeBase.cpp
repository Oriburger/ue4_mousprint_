// Copyright Epic Games, Inc. All Rights Reserved.


#include "MainGameModeBase.h"
#include "SaveInfo.h"
#include "Kismet/GameplayStatics.h"
#include "StageInfoTable.h"

// Sets default values
AMainGameModeBase::AMainGameModeBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USaveInfo* LoadGameInstance = Cast<USaveInfo>(UGameplayStatics::CreateSaveGameObject(USaveInfo::StaticClass()));
	LoadGameInstance = Cast<USaveInfo>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, LoadGameInstance->UserIndex));
	if (LoadGameInstance != nullptr)
	{
		bIsTutorialEnd = LoadGameInstance->bIsTutorialEnd;
		HighScore = LoadGameInstance->HighScore;
		UE_LOG(LogTemp, Warning, TEXT("Game Load"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Save Data Not Found"));
	}
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
	MaintainDistance();
}

void AMainGameModeBase::UpdateStageInfo(const float DeltaTime)
{
	if (!bIsGameStarted) return;
	if (StageEndTime != 0 && !MainCharacter->GetPlayerIsDead())
	{
		Score = Score + DeltaTime * 10;
		DeltaTimeSum += DeltaTime;

		if (DeltaTimeSum >= StageEndTime) StageEndTime = SetStage(++Stage);
	}
}

void AMainGameModeBase::MaintainDistance()
{
	if (bIsGameOver || !bIsGameStarted) return;
	if (GetDistanceGasToPlayer() >= 20000.0f)
		FollowingGas->SetMoveSpeedLimit(10000.0f, 10000.0f); //거리가 20000이상 벌어지면 빠르게 이동
	else
		SetStage(Stage); //그렇지 않으면, 속도를 원래대로
}

void AMainGameModeBase::CheckGameOver()
{
	if (!bIsGameStarted || MainCharacter == nullptr) return;

	if (MainCharacter->GetPlayerIsDead())
	{
		bIsGameOver = true;
		bIsGameStarted = false;

		SaveGameInfo(true, FMath::Max(HighScore, Score));
	}
}

float AMainGameModeBase::SetStage(const int32 stage_)
{
	if (StageInfoTable == nullptr || bIsGameOver) return -1;

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

bool AMainGameModeBase::GameInit()
{
	if (!GetWorld()) return false;

	SaveGameInfo(true, HighScore);

	bIsGameStarted = true;
	FActorSpawnParameters SpawnParams;
	FTransform SpawnTransform;
	SpawnParams.Owner = this; //타일의 소유자는 Generator
	SpawnParams.Instigator = GetInstigator();

	SpawnTransform.SetLocation({ -7600.0f, 0.0f, 2500.0f });
	TileGenerator = GetWorld()->SpawnActor<ATileGenerator>(TileGeneratorClass, SpawnParams);
	FollowingGas = GetWorld()->SpawnActor<AFollowingGasBase>(FollowingGasClass, SpawnTransform, SpawnParams);
	if (TileGenerator == nullptr || FollowingGas == nullptr) return false;

	FollowingGas->SetGasCanMove(true);

	StageEndTime = SetStage(Stage);
	return true;
}

bool AMainGameModeBase::GameStart()
{
	if (!GetWorld() || MainCharacter == nullptr || FollowingGas == nullptr) return false;

	MainCharacter->bCanMove = true;
	GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(MainCharacter);

	return true;
}

float AMainGameModeBase::GetDistanceGasToPlayer() const
{
	if (MainCharacter == nullptr || FollowingGas == nullptr) return -1.0f;

	return FVector::Distance(MainCharacter->GetActorLocation(), FollowingGas->GetActorLocation()) - 1450.0f;
}

bool AMainGameModeBase::GetTutorialIsEnd() const { return bIsTutorialEnd;  }

void AMainGameModeBase::SetIsTutorialEnd(const bool flag) { bIsTutorialEnd = flag; }

bool AMainGameModeBase::GetGameIsOver() const { return bIsGameOver;  }

bool AMainGameModeBase::SaveGameInfo(bool bIsTutorialEnd_, float score_)
{
	USaveInfo* SaveGameInstance = Cast<USaveInfo>(UGameplayStatics::CreateSaveGameObject(USaveInfo::StaticClass()));
	SaveGameInstance->bIsTutorialEnd = bIsTutorialEnd_;
	SaveGameInstance->HighScore = FMath::Max(score_, HighScore);
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex);
	//UE_LOG(LogTemp, Warning, TEXT("Data Saved"));
	return true;
}