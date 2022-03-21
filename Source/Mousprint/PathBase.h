// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathBase.generated.h"

UCLASS()
class MOUSPRINT_API APathBase : public AActor
{
	GENERATED_BODY()
	

public:	
	// Sets default values for this actor's properties
	APathBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
		bool bIsChecked = false;
};
