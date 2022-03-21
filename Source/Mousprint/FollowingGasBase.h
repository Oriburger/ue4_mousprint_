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
	bool bCanMove = false; 
	bool bChaingMode = false;

public:
	// Sets default values for this character's properties
	AFollowingGasBase();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Collision)
		USphereComponent* PathFindingCollision;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Collision)
		USphereComponent* DamageCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
		float CharacterMoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
		float CharacterMaxMoveSpeed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void CheckPathAndUpdateLocation();

	UFUNCTION()
		void UpdateCharacterSpeed(const float DeltaTime);

	UFUNCTION()
		void SetMoveSpeedLimit(const float MinValue, const float MaxValue);

	UFUNCTION(BlueprintCallable)
		float GetCurrentSpeed() const;

	UFUNCTION()
		void PathBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp
			, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void PathEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
			, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void PlayerBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp
			, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
		void SetGasCanMove(const bool flag);

	UFUNCTION()
		void SetChaingMode(const bool flag);
};
