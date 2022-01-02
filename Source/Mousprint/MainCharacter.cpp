// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
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
	CameraBoomNormal->TargetArmLength = 250.0f;
	CameraBoomNormal->bEnableCameraLag = true;
	CameraBoomNormal->CameraLagSpeed = 10.0f;
	CameraBoomNormal->SocketOffset.Y = 50;

	CameraBoomAiming = CreateDefaultSubobject<USpringArmComponent>(TEXT("CAMERA_BOOM_AIMING"));
	CameraBoomAiming->SetupAttachment(RootComponent);
	CameraBoomAiming->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(-15.0f, 0.0f, 0.0f));
	CameraBoomAiming->bUsePawnControlRotation = true;
	CameraBoomAiming->TargetArmLength = 150.0f;
	CameraBoomAiming->bEnableCameraLag = true;
	CameraBoomAiming->CameraLagSpeed = 10.0f;
	CameraBoomAiming->SocketOffset.Y = 50;
	CameraBoomAiming->SocketOffset.Z = 60;

	FollowingCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FOLLOWING_CAMERA"));
	FollowingCamera->SetupAttachment(CameraBoomNormal);
	GetCharacterMovement()->MaxWalkSpeed = CharacterMaxWalkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CharacterMaxWalkSpeed;
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
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsInGame)
	{
		if (GetCharacterMovement()->IsCrouching())
		{
			CrouchingTime += DeltaTime;
			if (CrouchingTime > 0.5f) StopSlide();
		}

		MoveForward(1);

		CharacterMaxWalkSpeed += DeltaTime*5;
		CharacterAimingWalkSpeed += DeltaTime*5;
		GetCharacterMovement()->MaxWalkSpeed = CharacterMaxWalkSpeed;
		GetCharacterMovement()->MaxWalkSpeedCrouched = CharacterMaxWalkSpeed;
	}
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//�Է��� �Լ��� ���ε�
	//PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AMainCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AMainCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMainCharacter::Fire);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AMainCharacter::Aim);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &AMainCharacter::StopAim);

	PlayerInputComponent->BindAction("Slide", IE_Pressed, this, &AMainCharacter::StartSlide);
	//PlayerInputComponent->BindAction("Slide", IE_Released, this, &AMainCharacter::StopSlide);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharacter::StartJump);
}

bool AMainCharacter::GetPlayerIsAiming() const { return bIsAimed;  }

void AMainCharacter::MoveForward(float Value)
{
	//���� Controller�� X �������� Value ��ŭ �̵�
	FVector Direction = FRotationMatrix(this->GetActorRotation()).GetScaledAxis(EAxis::X);
	if (abs(Value) > 0) this->SetActorRotation({0, Controller->GetControlRotation().Yaw, 0});
	AddMovementInput(Direction, Value);
}

void AMainCharacter::MoveRight(float Value)
{
	//���� Controller�� Y �������� Value ��ŭ �̵�
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	if (abs(Value) > 0) this->SetActorRotation({ 0, Controller->GetControlRotation().Yaw, 0 });
	AddMovementInput(Direction, Value*10);
	MoveForward(abs(Value*10));
}

void AMainCharacter::Fire()
{
	if (!GetWorld() || !ProjectileClass || !bIsAimed || bIsDead) return;
	if (GetCharacterMovement()->IsFalling() || GetCharacterMovement()->IsCrouching()) return;

	//UE_LOG(LogTemp, Warning, TEXT("Fire!!"));

	FHitResult LineTraceHitResult; //LineTracing�� ����� ��� ����
	FVector TraceBeginLocation = FollowingCamera->GetComponentLocation(); //Trace�� ī�޶󿡼� ����
	FVector TraceEndLocation = TraceBeginLocation + (FollowingCamera->GetComponentRotation()).Vector() * 200000.0f;
								//End�� Camera�κ��� 20000.0f ������ ��������


	GetWorld()->LineTraceSingleByChannel(LineTraceHitResult, TraceBeginLocation, TraceEndLocation
										, ECollisionChannel::ECC_Visibility); //LineTrace

	FVector BeginLocation = Weapon->GetSocketLocation(TEXT("Muzzle")); //�߻� ������ Muzzle ������ ��ġ 
	FRotator BeginRotation = UKismetMathLibrary::FindLookAtRotation(BeginLocation, LineTraceHitResult.ImpactPoint);
							 //Muzzle ���Ͽ��� LineTrace�� Hit�� ��ġ������ Rotataion�� �߻簢


	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator(); 
	ABullet* Projectile = GetWorld()->SpawnActor<ABullet>(ProjectileClass, BeginLocation, BeginRotation);
	
	if (Projectile)
	{
		FVector LaunchDirection = BeginRotation.Vector();
		Projectile->FireInDirection(LaunchDirection);
	}

	GameStatic->PlaySoundAtLocation(GetWorld(), ShootSound, this->GetActorLocation(), this->GetActorRotation(), 1.0f);
}	

void AMainCharacter::Aim()
{
	if (bIsDead) return;
	//UE_LOG(LogTemp, Warning, TEXT("Aim"));

	bIsAimed = true;
	GetCharacterMovement()->MaxWalkSpeed = CharacterAimingWalkSpeed;

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
	GetCharacterMovement()->MaxWalkSpeed = CharacterMaxWalkSpeed;

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	FollowingCamera->AttachToComponent(CameraBoomNormal, FAttachmentTransformRules::KeepWorldTransform);
	UKismetSystemLibrary::MoveComponentTo(FollowingCamera, FVector(0, 0, 0), FRotator(0, 0, 0)
							, true, true, 0.2, false, EMoveComponentAction::Type::Move, LatentInfo);
} 

void AMainCharacter::StartSlide()
{
	if (GetCharacterMovement()->IsFalling() || bIsDead) return;
	UE_LOG(LogTemp, Warning, TEXT("Crouch"));
	ACharacter::Crouch();
}

void AMainCharacter::StopSlide()
{
	UE_LOG(LogTemp, Warning, TEXT("UnCrouch"));
	ACharacter::UnCrouch();
	CrouchingTime = 0;
}

void AMainCharacter::StartJump()
{
	if (GetCharacterMovement()->IsFalling() || bIsDead) return;
	StopSlide();
	ACharacter::Jump();
}

float AMainCharacter::TakeDamage(const float damage)
{
	CharacterCurrHP = (CharacterCurrHP - damage < 0 ? 0 : CharacterCurrHP - damage);

	if (CharacterCurrHP == 0) Die();

	return CharacterCurrHP;
}

void AMainCharacter::Die()
{
	bIsDead = true;
	UE_LOG(LogTemp, Warning, TEXT("DEAD"));
	SetPlayerRagdoll(true);
}

void AMainCharacter::SetPlayerRagdoll(const bool flag)
{
	GetMesh()->SetSimulatePhysics(flag);
	if (flag) GetCharacterMovement()->DisableMovement();
	else GetCharacterMovement()->Activate();
}