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
	bool bIsExploding = false; 
	float DyingOpacity = 0;
	float ExplodeTime = 0;
	AActor * target = nullptr;

public:
	// Sets default values for this character's properties
	AMobBase();

	UPROPERTY(EditAnywhere, Category = EffectSetting)
		bool bCanExplode = false;

	UPROPERTY(EditAnywhere, Category = CharacterStat)
		float CharacterMaxHP = 100;

	UPROPERTY(EditAnywhere, Category = CharacterStat)
		float CharacterCurrHP = 100;

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
		void OnBeginDetect(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp
			, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapAtkRange(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp
			, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp
			, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void Die();

	UFUNCTION()
		void SetRagdollMode(const bool flag);
	
	UFUNCTION(BlueprintCallable)
		void SetExplode(bool flag);
};
