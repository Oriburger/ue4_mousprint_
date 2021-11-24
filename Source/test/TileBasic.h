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
	bool OverlapFlag = false;
	int NextTileIdx = -1;

public:	
	// Sets default values for this actor's properties
	ATileBasic();
	UPROPERTY(VisibleDefaultsOnly)
		USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleDefaultsOnly)
		UArrowComponent* EdgeArrowComponent;

	//UPROPERTY(VisibleDefaultsOnly)
	UPROPERTY(BlueprintReadOnly)
		UBoxComponent* BoxTrigger;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		FTransform GetNextSpawnPoint() const;
	
	UFUNCTION(BlueprintCallable)
		bool IsOverlapped() const;

	UFUNCTION(BlueprintCallable)
		int GetNextTileIdx() const;

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
								, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
