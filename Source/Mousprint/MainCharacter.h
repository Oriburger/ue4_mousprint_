// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Bullet.h"
#include "PathBase.h"
#include "Sound/SoundWave.h"
#include "Camera/CameraShakeBase.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UCLASS()
class MOUSPRINT_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	bool bIsDizzy = false;
	bool bIsDead = false;
	bool bIsRagdoll = false;
	bool bIsAimed = false;
	bool bIsInGame = true; //= false;

	float PathSpawnTime = 0; //PathActor 스폰위한 DeltaTime 계산 값 
	float DisableRagdollDelay = 0; //Ragdoll 상태에서 다시 풀릴때까지 걸리는 시간
	float GettingUpTimeDelay = 0; //플레이어가 넘어진 상태에서 다 일어나기까지 걸리는 시간
	float DizzyTime = 0; //플레이어의 하트가 0이 될 경우, 정신을 차리기까지의 시간
	float CrouchingTime = 0; //슬라이딩 시간 

public:
	UPROPERTY()
		UGameplayStatics* GameStatic;

	UPROPERTY(EditAnywhere, Category = Effect)
		USoundWave* HitSound; //데미지 사운드

	UPROPERTY(EditAnywhere, Category = Effect)
		USoundWave* ShootSound; //총기 발사 사운드

	UPROPERTY(EditAnywhere, Category = Effect)
		TSubclassOf<class UCameraShakeBase> DamageCameraShake;

	UPROPERTY(EditAnywhere, Category = Effect)
		TSubclassOf<class UCameraShakeBase> GasCameraShake;

public:
	// Sets default values for this character's properties
	AMainCharacter();

	/* ----- Charater State + Tutorial ----- */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterStat)
		float CharacterMaxHP = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterStat)
		float CharacterCurrHP = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterStat)
		float CharacterMinWalkSpeed = 1000; //플레이어의 최소 이동 속도 제한 (이 값 이상으로 줄어들지 않음)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterStat)
		float CharacterMaxWalkSpeed = 1400; //플레이어의 최대 이동 속도 제한 (이 값 이상으로 늘어나지 않음)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterStat)
		float CharacterWalkSpeed = 1000; //플레이어의 이동 속도

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TutorialOnly)
		bool bCanMove = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TutorialOnly)
		bool bCanJump = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TutorialOnly)
		bool bCanSlide = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TutorialOnly)
		bool bCanAim = true;


	/* ----- Basic Component ----- */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
		UChildActorComponent* Weapon; //무기 메시

	UPROPERTY(BlueprintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoomNormal; //Aiming을 하지 않은 CameraBoom

	UPROPERTY(BlueprintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoomAiming; //Aiming을 한 Zoom된 CameraBoom

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
		UCameraComponent* FollowingCamera; //Following Camera

	/* ------ Anim Montages ------ */
	UPROPERTY(EditAnywhere, Category = AnimMontage)
		UAnimMontage* FireAnimMontage;

	UPROPERTY(EditAnywhere, Category = AnimMontage)
		UAnimMontage* DizzyAnimMontage;

	UPROPERTY(EditAnywhere, Category = AnimMontage)
		UAnimMontage* GettingUpAnimMontage;

	UPROPERTY(EditAnywhere, Category = AnimMontage)
		UAnimMontage* HitAnimMontage;

	UPROPERTY(EditAnywhere, Category = AnimMontage)
		UAnimMontage* RushAnimMontage;

	/* ----- Spawn Class ----- */
	UPROPERTY(EditAnywhere, Category = Gameplay)
		TSubclassOf<class ABullet> ProjectileClass; //무기에 딸려있는 Projectile Class -> Weapon 클래스로 옮겨질 예정

	UPROPERTY(EditAnywhere, Category = Gameplay)
		TSubclassOf<class APathBase> PathClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* ---플레이어의 상태---- */
	UFUNCTION(BlueprintCallable)
		bool GetPlayerIsAiming() const; //플레이어가 Aiming 중인지 반환

	UFUNCTION(BlueprintCallable)
		bool GetPlayerIsDead() const; 

	UFUNCTION(BlueprintCallable)
		bool GetPlayerIsGettingUp() const;

	UFUNCTION(BlueprintCallable)
		bool GetPlayerIsDizzy() const;

	/* --- Axis & Action Input --- */
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

	UFUNCTION(BlueprintCallable)
		void StartSlide();

	UFUNCTION()
		void TryStopSlide(const float DeltaTime, const bool force = false);

	UFUNCTION()
		void StartRush();

	UFUNCTION()
		void StartJump();

	UFUNCTION()
		void StopJump();

	/* --- Etc ---- */

	UFUNCTION()
		void UpdateDizzyState(const float DeltaTime);

	UFUNCTION()
		void UpdateRagdollState(const float DeltaTime);

	UFUNCTION()
		void UpdateCharacterSpeed(const float DeltaTime);

	UFUNCTION()
		void UpdateGasCamShakeEffect() const;

	UFUNCTION()
		virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent
							, AController* EventInstigator, AActor * DamageCauser) override;

	UFUNCTION(BlueprintCallable)
		bool SetWalkSpeedLimit(const float MinValue, const float MaxValue);

	UFUNCTION(BlueprintCallable)
		void Die();

	UFUNCTION(BlueprintCallable)
		void SetPlayerRagdoll(const bool flag);

	UFUNCTION()
		bool SpawnPathActor(const float DeltaTime);

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp
			, FVector NormalImpulse, const FHitResult& Hit);

};