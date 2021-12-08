// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Weapon.h"
#include "Bullet.h"
#include "Sound/SoundWave.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UCLASS()
class TEST_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	bool bIsAimed;

public:
	UPROPERTY()
		UGameplayStatics* GameStatic;

	UPROPERTY(EditAnywhere, Category = Sound)
		USoundWave* ShootSound;

public:
	// Sets default values for this character's properties
	AMainCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
		UChildActorComponent * Weapon;

	//UPROPERTY(VisibleAnywhere, Category = Camera)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoomNormal;

	//UPROPERTY(VisibleAnywhere, Category = Camera)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoomAiming;

	//UPROPERTY(VisibleAnywhere, Category = Camera)
	UPROPERTY(BlueprintReadWrite, Category = Camera)
		UCameraComponent* FollowingCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
		TSubclassOf<class ABullet> ProjectileClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable)
		bool GetPlayerIsAiming() const;

	UFUNCTION()
		void MoveForward(float Value);

	UFUNCTION()
		void MoveRight(float Value);

	UFUNCTION()
		void Fire();

	UFUNCTION()
		void Aim();

	UFUNCTION()
		void StopAim();

	UFUNCTION()
		void StartCrouch();

	UFUNCTION()
		void StopCrouch();

	UFUNCTION()
		void StartJump();
};