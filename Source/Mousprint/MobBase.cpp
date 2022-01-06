// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/KismetMathLibrary.h"
#include "MobBase.h"

// Sets default values
AMobBase::AMobBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->Tags = { "Mob" };

	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);
	GetCapsuleComponent()->SetCapsuleRadius(80.0f);
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);

	GetMesh()->SetupAttachment(RootComponent);
	GetMesh()->CreateDynamicMaterialInstance(0);
	GetMesh()->CreateDynamicMaterialInstance(1);
	
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
	
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AMobBase::OnHit);
	EnemyDetectVolume->OnComponentBeginOverlap.AddDynamic(this, &AMobBase::OnBeginDetect); 
	AtkRangeVolume->OnComponentBeginOverlap.AddDynamic(this, &AMobBase::OnOverlapAtkRange);


}

// Called every frame
void AMobBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (target != nullptr && !bIsDead)
	{
		FVector MoveDirection = UKismetMathLibrary::FindLookAtRotation(GetActorLocation()
								, target->GetActorLocation()).Vector();
		AddMovementInput(MoveDirection, 1.0, false);
	}

	if (bCanExplode && bIsExploding)
	{
		ExplodeTime += DeltaTime*500.0f;
		GetMesh()->SetScalarParameterValueOnMaterials(TEXT("Emmisive"), ExplodeTime);

		if (ExplodeTime > 1000) Die();
	}
}

// Called to bind functionality to input
void AMobBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMobBase::OnBeginDetect(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp
							, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!GetWorld() || !OtherActor->ActorHasTag("Player")) return;

	target = OtherActor;
}

void AMobBase::OnOverlapAtkRange(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp
								, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
		
}

void AMobBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp
	, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!GetWorld()) return;
	if (OtherActor->ActorHasTag("Projectile"))
	{
		DyingOpacity = 100;
	}
}

void AMobBase::Die()
{
	bIsDead = true;
	GetCharacterMovement()->SetActive(false);
	Destroy();
}

void AMobBase::SetRagdollMode(const bool flag)
{
	GetMesh()->SetSimulatePhysics(flag);
	if (flag) GetCharacterMovement()->DisableMovement();
	else GetCharacterMovement()->Activate();
}

void AMobBase::SetExplode(bool flag)
{
	bIsExploding = true; 
}