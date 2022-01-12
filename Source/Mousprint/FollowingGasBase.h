// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Containers/Array.h"
#include "GameFramework/Character.h"
#include "FollowingGasBase.generated.h"

UCLASS()
class MOUSPRINT_API AFollowingGasBase : public ACharacter
{
	GENERATED_BODY()

private:
	TArray<AActor*> PathList;

public:
	// Sets default values for this character's properties
	AFollowingGasBase();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Collision)
		USphereComponent* PathFindingCollision;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp
			, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
			, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
