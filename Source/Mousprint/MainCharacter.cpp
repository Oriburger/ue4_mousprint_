// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "MobBase.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->Tags = {"Player"};

	CameraBoomNormal = CreateDefaultSubobject<USpringArmComponent>(TEXT("CAMERA_BOOM_NORMAL"));
	CameraBoomNormal->SetupAttachment(RootComponent);
	CameraBoomNormal->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(-15.0f, 0.0f, 0.0f));
	CameraBoomNormal->bUsePawnControlRotation = true;
	CameraBoomNormal->ProbeChannel = ECollisionChannel::ECC_Visibility;
	CameraBoomNormal->TargetArmLength = 250.0f;
	CameraBoomNormal->bEnableCameraLag = true;
	CameraBoomNormal->CameraLagSpeed = 10.0f;
	CameraBoomNormal->SocketOffset.Y = 30;
	CameraBoomNormal->SocketOffset.Z = 40;

	CameraBoomAiming = CreateDefaultSubobject<USpringArmComponent>(TEXT("CAMERA_BOOM_AIMING"));
	CameraBoomAiming->SetupAttachment(RootComponent);
	CameraBoomAiming->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(-15.0f, 0.0f, 0.0f));
	CameraBoomAiming->bUsePawnControlRotation = true;
	CameraBoomAiming->ProbeChannel = ECollisionChannel::ECC_Visibility;
	CameraBoomAiming->TargetArmLength = 150.0f;
	CameraBoomAiming->bEnableCameraLag = true;
	CameraBoomAiming->CameraLagSpeed = 5.0f;
	CameraBoomAiming->SocketOffset.Y = 80;
	CameraBoomAiming->SocketOffset.Z = 35;

	FollowingCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FOLLOWING_CAMERA"));
	FollowingCamera->SetupAttachment(CameraBoomNormal);
	GetCharacterMovement()->MaxWalkSpeed = CharacterMinWalkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CharacterMinWalkSpeed * 0.75;
	GetCharacterMovement()->JumpZVelocity = 350;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	//this->bUseControllerRotationYaw = false;

	Weapon = CreateDefaultSubobject<UChildActorComponent>(TEXT("WEAPON"));	
	Weapon->SetupAttachment(this->GetMesh(), TEXT("Weapon"));
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AMainCharacter::OnHit); //Hit 이벤트 추가
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsInGame) return;

	TryStopSlide(DeltaTime); //Slide 중이라면, 일정 시간 이후 자동으로 Slide를 멈춤
	SpawnPathActor(DeltaTime); //일정 Tick 간격으로 PathActor를 스폰

	UpdateRagdollState(DeltaTime);
	UpdateCharacterSpeed(DeltaTime);

	//MoveForward(1);
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//입력을 함수와 바인딩
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AMainCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AMainCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMainCharacter::Fire);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AMainCharacter::Aim);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &AMainCharacter::StopAim);

	PlayerInputComponent->BindAction("Slide", IE_Pressed, this, &AMainCharacter::StartSlide);
	PlayerInputComponent->BindAction("Rush", IE_Pressed, this, &AMainCharacter::StartRush);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharacter::StartJump);
}

bool AMainCharacter::GetPlayerIsAiming() const { return bIsAimed;  }

bool AMainCharacter::GetPlayerIsDead() const { return bIsDead;  }

bool AMainCharacter::GetPlayerIsGettingUp() const { return GettingUpTimeDelay > 0; };

void AMainCharacter::MoveForward(float Value)
{
	if (!GetWorld() || bIsDead || bIsRagdoll || !bCanMove) return;
	//현재 Controller의 X 방향으로 Value 만큼 이동
	FVector Direction = FRotationMatrix(this->GetActorRotation()).GetScaledAxis(EAxis::X);
	if (abs(Value) > 0) this->SetActorRotation({0, Controller->GetControlRotation().Yaw, 0});
	AddMovementInput(Direction, Value);
}

void AMainCharacter::MoveRight(float Value)
{
	if (!GetWorld() || bIsDead || bIsRagdoll || !bCanMove) return;
	//현재 Controller의 Y 방향으로 Value 만큼 이동
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	if (abs(Value) > 0) this->SetActorRotation({ 0, Controller->GetControlRotation().Yaw, 0 });
	AddMovementInput(Direction, Value*10);
	MoveForward(abs(Value*10));
}

void AMainCharacter::Fire()
{
	if (!GetWorld() || !ProjectileClass || !bIsAimed || bIsDead || bIsRagdoll) return;
	if (GetCharacterMovement()->IsCrouching()) return;

	//UE_LOG(LogTemp, Warning, TEXT("Fire!!"));

	FHitResult LineTraceHitResult; //LineTracing의 결과가 담길 변수
	FVector TraceBeginLocation = FollowingCamera->GetComponentLocation(); //Trace는 카메라에서 시작
	FVector TraceEndLocation = TraceBeginLocation + (FollowingCamera->GetForwardVector()) * 200000.0f; //End는 Camera로부터 20000.0f 떨어진 지점까지
	FCollisionQueryParams TraceCollisionQuery = FCollisionQueryParams::DefaultQueryParam;
	TraceCollisionQuery.AddIgnoredActor(this->GetUniqueID());  //플레이어의 카메라가 Hit되지 않도록 방지


	GetWorld()->LineTraceSingleByChannel(LineTraceHitResult, TraceBeginLocation, TraceEndLocation
		, ECollisionChannel::ECC_Camera, TraceCollisionQuery); //LineTrace 시작

	FVector BeginLocation = Weapon->GetSocketLocation(TEXT("Muzzle")); //발사 시작은 Blaster의 소켓 "Muzzle"의 위치 
	FRotator BeginRotation = UKismetMathLibrary::FindLookAtRotation(BeginLocation, LineTraceHitResult.ImpactPoint); //Muzzle 소켓에서 LineTrace이 Hit된 위치까지의 Rotataion이 발사각

	//만약 LineTrace에 Hit 된 타겟이 없다면, 카메라의 Rotation을 따라감 
	if (LineTraceHitResult.GetActor() == nullptr && LineTraceHitResult.GetComponent() == nullptr)
	{
		BeginRotation = FollowingCamera->GetComponentRotation();
	}
	//UE_LOG(LogTemp, Warning, TEXT("ActorName : %s"), *UKismetSystemLibrary::GetDisplayName(LineTraceHitResult.GetActor()));

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	ABullet* Projectile = GetWorld()->SpawnActor<ABullet>(ProjectileClass, BeginLocation, BeginRotation);

	if (Projectile)
	{
		FVector LaunchDirection = BeginRotation.Vector();
		Projectile->FireInDirection(LaunchDirection);
	}

	if (FireAnimMontage != nullptr && ShootSound != nullptr)
	{
		PlayAnimMontage(FireAnimMontage);
		GameStatic->PlaySoundAtLocation(GetWorld(), ShootSound, this->GetActorLocation(), this->GetActorRotation(), 1.0f);
	}
}

void AMainCharacter::Aim()
{
	if (bIsDead || bIsRagdoll || !bCanAim) return;
	if (GetCharacterMovement()->IsCrouching()) return;
	//UE_LOG(LogTemp, Warning, TEXT("Aim"));

	bIsAimed = true;
	GetCharacterMovement()->MaxWalkSpeed = CharacterWalkSpeed * 0.75f;

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	FollowingCamera->AttachToComponent(CameraBoomAiming, FAttachmentTransformRules::KeepWorldTransform);
	UKismetSystemLibrary::MoveComponentTo(FollowingCamera, FVector(0, 0, 0), FRotator(0, 0, 0)
							, true, true, 0.2, false, EMoveComponentAction::Type::Move, LatentInfo);
}

void AMainCharacter::StopAim()
{
	//UE_LOG(LogTemp, Warning, TEXT("AimStop"));

	bIsAimed = false;
	GetCharacterMovement()->MaxWalkSpeed = CharacterWalkSpeed;

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	FollowingCamera->AttachToComponent(CameraBoomNormal, FAttachmentTransformRules::KeepWorldTransform);
	UKismetSystemLibrary::MoveComponentTo(FollowingCamera, FVector(0, 0, 0), FRotator(0, 0, 0)
							, true, true, 0.2, false, EMoveComponentAction::Type::Move, LatentInfo);
} 

void AMainCharacter::StartSlide()
{
	if (GetCharacterMovement()->IsCrouching() || bIsDead || bIsRagdoll || !bCanSlide) return;

	StopJump();
	GetCharacterMovement()->MaxWalkSpeedCrouched = CharacterWalkSpeed;

	//UE_LOG(LogTemp, Warning, TEXT("Crouch"));
	ACharacter::Crouch();
}

void AMainCharacter::TryStopSlide(const float DeltaTime, const bool force)
{
	if (!force)
	{
		//슬라이딩 자동 해제 로직
		if (!GetCharacterMovement()->IsCrouching()) return;

		CrouchingTime += DeltaTime;
		if (CrouchingTime < 0.5f) return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("UnCrouch"));
	GetCharacterMovement()->MaxWalkSpeedCrouched = CharacterWalkSpeed * 0.75f;
	ACharacter::UnCrouch();
	CrouchingTime = 0;
}

void AMainCharacter::StartRush()
{
	if (bIsDead || bIsAimed || bIsRagdoll || !bIsInGame || GetPlayerIsGettingUp()) return;	
	if (!bCanJump || GetCharacterMovement()->IsFalling()) return;

	GetCharacterMovement()->DefaultLandMovementMode = MOVE_Flying;
	LaunchCharacter({ 0.0f, 0.0f, 350.0f }, false, true);
	if (RushAnimMontage != nullptr)	PlayAnimMontage(RushAnimMontage);

	FTimerHandle WaitHandle;
	float WaitTime = 0.2f;
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{ LaunchCharacter(GetVelocity() * 1.15f, true, false); }), WaitTime, false); 
	
	GetCharacterMovement()->DefaultLandMovementMode = MOVE_Walking;
}

void AMainCharacter::StartJump()
{
	if (GetCharacterMovement()->IsFalling() || bIsDead) return;
	if (!bCanJump|| GetPlayerIsGettingUp()) return;
	TryStopSlide(0, true);
	ACharacter::Jump();
}

void AMainCharacter::StopJump()
{
	if (!GetCharacterMovement()->IsFalling()) return;

	StopJumping();
	LaunchCharacter({ 0.0f, 0.0f, -500.0f }, false, true);
}

void AMainCharacter::UpdateRagdollState(const float DeltaTime)
{
	//Ragdoll 관련 로직
	if (DisableRagdollDelay > 0) //Ragdoll 상태가 되고, 다시 풀릴때까지의 딜레이
	{
		DisableRagdollDelay = FMath::Max(0.0f, DisableRagdollDelay - DeltaTime);
		if (DisableRagdollDelay == 0) SetPlayerRagdoll(false);
	}
	else if (GettingUpTimeDelay > 0) //GettingUp AnimMontage이 실행되는 동안, bIsRagdoll 값 갱신을 딜레이
	{
		GettingUpTimeDelay = FMath::Max(0.0f, GettingUpTimeDelay - DeltaTime);
		if (GettingUpTimeDelay == 0)
		{
			bIsRagdoll = false;
			CharacterWalkSpeed = FMath::Max(CharacterMinWalkSpeed, CharacterWalkSpeed * 90 / 100);
		}
	}
}

void AMainCharacter::UpdateCharacterSpeed(const float DeltaTime)
{
	CharacterWalkSpeed = FMath::Max(CharacterMinWalkSpeed, CharacterWalkSpeed);
	CharacterWalkSpeed = FMath::Min(CharacterMaxWalkSpeed, CharacterWalkSpeed + DeltaTime * 10.0f);

	if (!GetPlayerIsAiming() && !GetCharacterMovement()->IsCrouching())
	{
		GetCharacterMovement()->MaxWalkSpeed = CharacterWalkSpeed;
		GetCharacterMovement()->MaxWalkSpeedCrouched = CharacterWalkSpeed * 0.75;
	}
}

float AMainCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent
	, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	//UE_LOG(LogTemp, Warning, TEXT("Take Damage"));
	if(bIsInGame && !GetPlayerIsGettingUp() && !bIsDead) PlayAnimMontage(HitAnimMontage, 1.5f);

	GameStatic->PlaySoundAtLocation(GetWorld(), HitSound, this->GetActorLocation()
									, this->GetActorRotation(), 1.0f);

	CharacterCurrHP = FMath::Max(0.0f, CharacterCurrHP-Damage);
	if (CharacterCurrHP == 0) Die();

	return FinalDamage;
}

bool AMainCharacter::SetWalkSpeedLimit(const float MinValue, const float MaxValue)
{
	if (MinValue > MaxValue) return false;
	CharacterMaxWalkSpeed = MaxValue;
	CharacterMinWalkSpeed = MinValue;
	return true;
}

void AMainCharacter::Die()
{
	bIsDead = true;
	UE_LOG(LogTemp, Warning, TEXT("DEAD"));
	SetPlayerRagdoll(true);
}

void AMainCharacter::SetPlayerRagdoll(const bool flag)
{
	GetMesh()->SetSimulatePhysics(flag); //플레이어의 Ragdoll 상태를 flag로 지정
	if (flag) GetCharacterMovement()->Deactivate(); //true라면, 캐릭터의 Movement를 비활성화 시킨다

	else //Mesh 위치를 초기화 시키며 일어나는 애니메이션을 출력
	{
		FAttachmentTransformRules ResetTransform = FAttachmentTransformRules::KeepRelativeTransform; 

		GetCapsuleComponent()->SetWorldLocation(GetMesh()->GetComponentLocation()+FVector(0,0,100), false, nullptr, ETeleportType::TeleportPhysics);
		GetMesh()->AttachToComponent(GetCapsuleComponent(), ResetTransform);
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -68.0f), false, nullptr, ETeleportType::TeleportPhysics);
		GetMesh()->SetRelativeRotation({ 0.0f, -90.0f, 0.0f });

		if(GettingUpAnimMontage!=nullptr)
			GettingUpTimeDelay = PlayAnimMontage(GettingUpAnimMontage, 1.5f) / 1.5f;

		GetCharacterMovement()->Activate();
	}
}

bool AMainCharacter::SpawnPathActor(const float DeltaTime)
{
	if (PathClass == nullptr) return false;
	if (bIsDead || !bIsInGame) return false;

	PathSpawnTime += DeltaTime;
	if (PathSpawnTime > 0.25f)
	{
		GetWorld()->SpawnActor<AActor>(PathClass, GetActorLocation(), GetActorRotation());
		PathSpawnTime = 0;
	}

	return true;
}

void AMainCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor
	, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!GetWorld() || OtherActor == nullptr || OtherComp == nullptr) return;
	if (OtherComp->GetCollisionProfileName() == FName("Obstacle"))
	{
		SetPlayerRagdoll(true);
		bIsRagdoll = true;
		DisableRagdollDelay = 1.5f;
	}
}