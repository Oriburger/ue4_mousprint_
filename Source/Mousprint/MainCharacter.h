// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Weapon.h"
#include "Bullet.h"
#include "Sound/SoundWave.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UCLASS()
class MOUSPRINT_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	bool bIsDead = false;
	bool bIsRagdoll = false;
	bool bIsAimed = false;
	bool bIsInGame = true; //= false;

	float CrouchingTime = 0; //슬라이딩 시간 

public:
	UPROPERTY()
		UGameplayStatics* GameStatic;

	UPROPERTY(EditAnywhere, Category = Sound)
		USoundWave* ShootSound; //총기 발사 사운드

public:
	// Sets default values for this character's properties
	AMainCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterStat)
		float CharacterMaxHP = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterStat)
		float CharacterCurrHP = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterStat)
		float CharacterMaxWalkSpeed = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterStat)
		float CharacterAimingWalkSpeed = 500;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
		UChildActorComponent* Weapon; //무기 메시

	UPROPERTY(BlueprintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoomNormal; //Aiming을 하지 않은 CameraBoom

	UPROPERTY(BlueprintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoomAiming; //Aiming을 한 Zoom된 CameraBoom

	UPROPERTY(BlueprintReadOnly, Category = Camera)
		UCameraComponent* FollowingCamera; //Following Camera

	UPROPERTY(EditAnywhere, Category = AnimMontage)
		UAnimMontage* FireAnimMontage;

	UPROPERTY(EditAnywhere, Category = AnimMontage)
		UAnimMontage* GettingUpAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		TSubclassOf<class ABullet> ProjectileClass; //무기에 딸려있는 Projectile Class -> Weapon 클래스로 옮겨질 예정

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
		bool GetPlayerIsAiming() const; //플레이어가 Aiming 중인지 반환

	UFUNCTION(BlueprintCallable)
		bool GetPlayerIsDead() const; 

	UFUNCTION(BlueprintCallable)
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
		void StartSlide();

	UFUNCTION()
		void StopSlide();

	UFUNCTION()
		void StartJump();

	UFUNCTION()
		float TakeDamage(const float damage);

	UFUNCTION(BlueprintCallable)
		void Die();

	UFUNCTION(BlueprintCallable)
		void SetPlayerRagdoll(const bool flag);

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp
			, FVector NormalImpulse, const FHitResult& Hit);

};