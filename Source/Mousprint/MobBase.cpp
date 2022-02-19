// Fill out your copyright notice in the Description page of Project Settings.

#include "MobBase.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMobBase::AMobBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->Tags = { "Mob" };

	GetCapsuleComponent()->SetCapsuleHalfHeight(25.0f);
	GetCapsuleComponent()->SetCapsuleRadius(25.0f);
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);

	GetMesh()->SetupAttachment(RootComponent);

	EnemyDetectVolume = CreateDefaultSubobject<USphereComponent>("EnemyDetectVolume");
	EnemyDetectVolume->SetupAttachment(RootComponent);
	EnemyDetectVolume->SetRelativeLocation(FVector(650, 0, 0));
	EnemyDetectVolume->SetRelativeScale3D(FVector(30, 30, 30));

	AtkRangeVolume = CreateDefaultSubobject<USphereComponent>("AtkRangeVolume");
	AtkRangeVolume->SetupAttachment(RootComponent);
	AtkRangeVolume->SetRelativeScale3D(FVector(6, 6, 6));
	
	GetCharacterMovement()->DefaultLandMovementMode = MOVE_Flying;
	GetCharacterMovement()->MaxFlySpeed = 2000;
}

// Called when the game starts or when spawned
void AMobBase::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnDefaultController();

	GetMesh()->OnComponentHit.AddDynamic(this, &AMobBase::OnHit);
	EnemyDetectVolume->OnComponentBeginOverlap.AddDynamic(this, &AMobBase::OnBeginDetect); 
	AtkRangeVolume->OnComponentBeginOverlap.AddDynamic(this, &AMobBase::OnOverlapAtkRange);

	if(bHasExplodeEffect) GetMesh()->SetScalarParameterValueOnMaterials(TEXT("Emmisive"), 0.0f);
	if(bHasDyingEffect) GetMesh()->SetScalarParameterValueOnMaterials(TEXT("Opacity"), 2.0f);
}

// Called every frame
void AMobBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (target != nullptr && !bIsDead)
	{
		FRotator ToTargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation()
									, target->GetActorLocation()-FVector(0, 0, 500));
		FVector MoveDirection = ToTargetRotation.Vector();
		SetActorRotation(FMath::Lerp<FRotator, float>(GetActorRotation(), ToTargetRotation, 0.1f));
		AddMovementInput(MoveDirection, 1.0, false);
	}

	if (bHasExplodeEffect && bIsExploding && !bIsDying) UpdateExplosionEffect(DeltaTime);
	if (bHasDyingEffect && bIsDying && !bIsExploding) UpdateEvaporatingEffect(DeltaTime);
	if (bIsDead) Die();
}

// Called to bind functionality to input
void AMobBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMobBase::OnBeginDetect(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp
							, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!GetWorld() || OtherActor == nullptr || OtherComp == nullptr) return;
	if(OtherActor->ActorHasTag("Player")) target = OtherActor;
}

void AMobBase::OnOverlapAtkRange(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp
								, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!GetWorld() || OtherActor == nullptr || OtherComp == nullptr) return;
	if (OtherActor->ActorHasTag("Player") && !bIsExploding && !bIsDying) bIsExploding = true;

}

void AMobBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp
	, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!GetWorld() || OtherActor == nullptr || OtherComp == nullptr) return;
	if (OtherActor->ActorHasTag("Projectile") && !bIsDying)
	{
		bIsDying = true;
		bIsExploding = false;
		DyingOpacity = 1.0f;
		GetCharacterMovement()->Deactivate();
	}
}

bool AMobBase::GetIsDead() const { return bIsDead; }

bool AMobBase::GetIsFlying() const { return bIsFlying; }

bool AMobBase::GetIsExploding() const { return bIsExploding; }

bool AMobBase::GetIsDying() const { return bIsDying;  }

void AMobBase::Die()
{
	SetRagdollMode(true);
	Destroy();
}

void AMobBase::SetRagdollMode(const bool flag)
{
	GetMesh()->SetSimulatePhysics(flag);
}

void AMobBase::UpdateExplosionEffect(const float DeltaTime)
{
	if (bIsDead) return;
	if (ExplodeTime >= 0.15)
	{
		UGameplayStatics::ApplyRadialDamage(GetWorld(), 25.0f, GetMesh()->GetComponentLocation()
			, 250.0f, UDamageType::StaticClass(), {}, this, nullptr);

		FTransform EmitterTransform;
		EmitterTransform.SetLocation(this->GetActorLocation());
		EmitterTransform.SetScale3D(FVector(2.0f, 2.0f, 2.0f));

		if(ExplosionEmitter != nullptr)
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEmitter, EmitterTransform);
		if (ExplosionSound != nullptr)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, this->GetActorLocation(), 3.0f);

		//UE_LOG(LogTemp, Warning, TEXT("Apply Damage"));

		bIsDead = true;
		return;
	}
	ExplodeTime += DeltaTime;

	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("Emmisive"), ExplodeTime*500);
}

void AMobBase::UpdateEvaporatingEffect(const float DeltaTime)
{
	if (DyingOpacity <= 0 || bIsDead)
	{
		bIsDead = true;
		return;
	}

	DyingOpacity -= DeltaTime*2;
	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("Opacity"), DyingOpacity);
}