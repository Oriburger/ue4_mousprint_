// Fill out your copyright notice in the Description page of Project Settings.


#include "TileBasic.h"
#include "MainCharacter.h"

// Sets default values
ATileBasic::ATileBasic()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT_COMPONENT"));
	DefaultSceneRoot->SetupAttachment(RootComponent);
	SetRootComponent(DefaultSceneRoot);

	EdgeArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("EDGE_ARROW"));
	EdgeArrowComponent->SetRelativeLocationAndRotation(FVector(2000.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
	EdgeArrowComponent->ArrowSize = 2.0f;

	SpawnTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("SPAWN_BOX_TRIGGER"));
	SpawnTrigger->SetRelativeScale3D(FVector(1.0f, 7.0f, 3.0f));

	DestroyTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("DESTROY_BOX_TRIGGER"));
	DestroyTrigger->SetRelativeScale3D(FVector(1.0f, 7.0f, 3.0f));
	DestroyTrigger->SetRelativeLocation(FVector(2500.0f, 0.0f, 0.0f));


	static ConstructorHelpers::FObjectFinder<UBlueprint> NextTile(TEXT("Blueprint'/Game/Maps/Tiles/MyTileBasic.MyTileBasic'"));
	if (NextTile.Object != NULL)
		NextTileBP = (UClass*)NextTile.Object->GeneratedClass;
}

// Called when the game starts or when spawned
void ATileBasic::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnTrigger->OnComponentEndOverlap.AddDynamic(this, &ATileBasic::OnEndOverlapToSpawn);
	DestroyTrigger->OnComponentBeginOverlap.AddDynamic(this, &ATileBasic::OnBeginOverlapToDestroy);
}

// Called every frame
void ATileBasic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FTransform ATileBasic::GetNextSpawnPoint() const
{
	return EdgeArrowComponent->GetComponentTransform();
}

void ATileBasic::SpawnNextTile() const
{
	//if (SpawnFlag) return;
	//SpawnFlag = true;

	FTransform SpawnPoint = GetNextSpawnPoint();
	GetWorld()->SpawnActor<ATileBasic>(NextTileBP, SpawnPoint);
	UE_LOG(LogTemp, Warning, TEXT("SpawnActor!!"));
}

void ATileBasic::OnEndOverlapToSpawn(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
	, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor->ActorHasTag(TEXT("Player"))) return;
	SpawnNextTile();
}

void ATileBasic::OnBeginOverlapToDestroy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
	, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep
	, const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag(TEXT("Player"))) return;
	UE_LOG(LogTemp, Warning, TEXT("DestroyActor!!"));
	Destroy();
}