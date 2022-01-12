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
	PathFindingCollision->SetRelativeScale3D(FVector(10.0f, 10.0f, 10.0f));

	GetCharacterMovement()->DefaultLandMovementMode = MOVE_Flying;
	GetCharacterMovement()->MaxFlySpeed = 750;
	GetCharacterMovement()->MaxWalkSpeed = 2000;
}

// Called when the game starts or when spawned
void AFollowingGasBase::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnDefaultController();
	PathFindingCollision->SetRelativeScale3D(FVector(50.0f, 50.0f, 50.0f));

	PathFindingCollision->OnComponentEndOverlap.AddDynamic(this, &AFollowingGasBase::OnEndOverlap);
	PathFindingCollision->OnComponentBeginOverlap.AddDynamic(this, &AFollowingGasBase::OnBeginOverlap);
}

// Called every frame
void AFollowingGasBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PathList.Num() > 0)
	{
		AActor* Target = PathList[PathList.Num()-1];
		FRotator ToTargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation()
									, Target->GetActorLocation());		
		this->SetActorRotation(UKismetMathLibrary::RLerp(GetActorRotation(), ToTargetRotation, 0.1f, true).Quaternion());
		AddMovementInput(ToTargetRotation.Vector(), 1.0f);

		UE_LOG(LogTemp, Warning, TEXT("Move"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Empty"));
	}

	GetCharacterMovement()->MaxFlySpeed = 750 + DeltaTime*3.0f;
}

// Called to bind functionality to input
void AFollowingGasBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFollowingGasBase::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp
									, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr || !OtherActor->ActorHasTag("Path")) return;
	if (Cast<APathBase>(OtherActor)->bIsChecked) return;

	PathList.Push(OtherActor);
	Cast<APathBase>(OtherActor)->bIsChecked = true;
}

void AFollowingGasBase::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
									, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == nullptr || !OtherActor->ActorHasTag("Path")) return;
	OtherActor->Destroy();

	if(PathList.Num() > 0) PathList.RemoveAt(0);
}