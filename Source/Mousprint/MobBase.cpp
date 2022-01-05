// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/KismetMathLibrary.h"
#include "MobBase.h"

// Sets default values
AMobBase::AMobBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->Tags = { "Mob" };

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	Mesh->CreateDynamicMaterialInstance(0, );
	Mesh->CreateDynamicMaterialInstance(1);

	EnemyDetectVolume = CreateDefaultSubobject<USphereComponent>("EnemyDetectVolume");
	EnemyDetectVolume->SetupAttachment(RootComponent);
	EnemyDetectVolume->SetRelativeLocation(FVector(650, 0, 0));
	EnemyDetectVolume->SetRelativeScale3D(FVector(30, 30, 30));

	AtkRangeVolume = CreateDefaultSubobject<USphereComponent>("AtkRangeVolume");
	AtkRangeVolume->SetupAttachment(RootComponent);
	AtkRangeVolume->SetRelativeScale3D(FVector(6, 6, 6));
	
	GetCharacterMovement()->DefaultLandMovementMode = MOVE_Flying;
	GetCharacterMovement()->MaxFlySpeed = 2000;
	//GetCharacterMovement()-
}

// Called when the game starts or when spawned
void AMobBase::BeginPlay()
{
	Super::BeginPlay();
	
	EnemyDetectVolume->OnComponentEndOverlap.AddDynamic(this, &AMobBase::OnBeginDetect); 
	AtkRangeVolume->OnComponentEndOverlap.AddDynamic(this, &AMobBase::OnOverlapAtkRange);


}

// Called every frame
void AMobBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(target) || !bIsDead)
	{
		FVector MoveDirection = UKismetMathLibrary::FindLookAtRotation(GetActorLocation()
								, target->GetActorLocation()).Vector().ForwardVector;
		AddMovementInput(MoveDirection, 1.0, false);

	}

	Mesh->SetVectorParameterValueOnMaterials(TEXT("Emmisive"),);
}

// Called to bind functionality to input
void AMobBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMobBase::OnBeginDetect(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
	, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!GetWorld() || !OtherActor->ActorHasTag("Player")) return;

	target = Cast<AMainCharacter>(OtherActor);
}

void AMobBase::OnOverlapAtkRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
	, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
		
}

