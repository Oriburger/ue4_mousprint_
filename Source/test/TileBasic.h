// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "TileBasic.generated.h"

UCLASS()
class TEST_API ATileBasic : public AActor
{
	GENERATED_BODY()

private:
	mutable bool SpawnFlag = false;
	TSubclassOf<class AActor> NextTileBP;

public:	
	// Sets default values for this actor's properties
	ATileBasic();
	UPROPERTY(VisibleDefaultsOnly)
		USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleDefaultsOnly)
		UArrowComponent* EdgeArrowComponent;

	UPROPERTY(VisibleDefaultsOnly)
		UBoxComponent* SpawnTrigger;

	UPROPERTY(VisibleDefaultsOnly)
		UBoxComponent* DestroyTrigger;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		FTransform GetNextSpawnPoint() const;

	UFUNCTION()
		void SpawnNextTile() const;

	UFUNCTION()
		void OnEndOverlapToSpawn(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
								, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnBeginOverlapToDestroy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
			, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
