// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.h"
#include "MobBase.generated.h"

UCLASS()
class MOUSPRINT_API AMobBase : public ACharacter
{
	GENERATED_BODY()
private:
	bool bIsDead = false;
	bool bIsFlying = false;
	AMainCharacter * target = nullptr;

public:
	// Sets default values for this character's properties
	AMobBase();

	UPROPERTY(EditAnywhere, Category = CharacterStat)
		float CharacterMaxHP = 100;

	UPROPERTY(EditAnywhere, Category = CharacterStat)
		float CharacterCurrHP = 100;

	UPROPERTY(EditAnywhere, Category = Mesh)
		USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = OverlapVolume)
		USphereComponent* EnemyDetectVolume;

	UPROPERTY(EditAnywhere, Category = OverlapVolume)
		USphereComponent* AtkRangeVolume;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void OnBeginDetect(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
			, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnOverlapAtkRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
			, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
