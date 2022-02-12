// Fill out your copyright notice in the Description page of Project Settings.


#include "FollowingGasBase.h"
#include "PathBase.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AFollowingGasBase::AFollowingGasBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->Tags = { "ToxicGas" };

	PathFindingCollision = CreateDefaultSubobject<USphereComponent>(TEXT("PATH_FIND_COLLISION"));
	PathFindingCollision->SetupAttachment(RootComponent);
	PathFindingCollision->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));

	DamageCollision = CreateDefaultSubobject<USphereComponent>(TEXT("DAMAGE_COLLISION"));
	DamageCollision->SetupAttachment(RootComponent);
	DamageCollision->SetRelativeScale3D(FVector(20.0f, 20.0f, 20.0f));
	DamageCollision->SetRelativeLocation(FVector(800.0f, 0.0f, 0.0f));

	GetCharacterMovement()->DefaultLandMovementMode = MOVE_Flying;
	GetCharacterMovement()->MaxFlySpeed = 850;
}

// Called when the game starts or when spawned
void AFollowingGasBase::BeginPlay()
{
	Super::BeginPlay();
	
	PathFindingCollision->OnComponentEndOverlap.AddDynamic(this, &AFollowingGasBase::PathEndOverlap);
	PathFindingCollision->OnComponentBeginOverlap.AddDynamic(this, &AFollowingGasBase::PathBeginOverlap);

	DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &AFollowingGasBase::PlayerBeginOverlap);

	SpawnDefaultController();
	PathFindingCollision->SetRelativeScale3D(FVector(100.0f, 100.0f, 100.0f));
}

// Called every frame
void AFollowingGasBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckPathAndUpdateLocation();
	UpdateCharacterSpeed(DeltaTime);
}

// Called to bind functionality to input
void AFollowingGasBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFollowingGasBase::CheckPathAndUpdateLocation()
{
	if (PathList.Num() > 0)
	{
		AActor* Target = PathList[PathList.Num() - 1];
		FRotator ToTargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation()
			, Target->GetActorLocation());
		this->SetActorRotation(UKismetMathLibrary::RLerp(GetActorRotation(), ToTargetRotation, 0.1f, true).Quaternion());
		AddMovementInput(ToTargetRotation.Vector(), 1.0f);
	}
	else AddMovementInput(GetActorRotation().Vector(), 0.25f);
}

void AFollowingGasBase::UpdateCharacterSpeed(const float DeltaTime)
{
	CharacterMoveSpeed = FMath::Min(CharacterMaxMoveSpeed, CharacterMoveSpeed + DeltaTime * 7.5f);
	GetCharacterMovement()->MaxFlySpeed = CharacterMoveSpeed;
}

void AFollowingGasBase::SetMoveSpeedLimit(const float MinValue, const float MaxValue)
{
	CharacterMoveSpeed = MinValue;
	CharacterMaxMoveSpeed = MaxValue;
}

float AFollowingGasBase::GetCurrentSpeed() const { return GetCharacterMovement()->MaxFlySpeed; }

void AFollowingGasBase::PathBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp
									, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr || !OtherActor->ActorHasTag("Path")) return;
	if (Cast<APathBase>(OtherActor)->bIsChecked) return;

	PathList.Push(OtherActor);
	Cast<APathBase>(OtherActor)->bIsChecked = true;
}

void AFollowingGasBase::PathEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
									, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == nullptr || !OtherActor->ActorHasTag("Path")) return;
	OtherActor->Destroy();

	if(PathList.Num() > 0) PathList.RemoveAt(0);
}

void AFollowingGasBase::PlayerBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp
	, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr || !OtherActor->ActorHasTag("Player")) return;

	UE_LOG(LogTemp, Warning, TEXT("Overlap"));

	UGameplayStatics::ApplyPointDamage(OtherActor, 1000.0f, OtherActor->GetActorLocation(),
		SweepResult, nullptr, this, nullptr);
	/*
	UGameplayStatics::ApplyPointDamage(GetWorld(), 1000.0f, DamageCollision->GetComponentLocation(),
			10000.0f, UDamageType::StaticClass(), {}, this, nullptr);*/
}