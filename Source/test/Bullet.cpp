// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SetRootComponent(CollisionComponent); //루트 컴포넌트화
	CollisionComponent->InitSphereRadius(20.0f);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 5500.0f; //발사체의 초기 속력
	ProjectileMovementComponent->MaxSpeed = 10000.0f; //발사체의 최대 속력
	ProjectileMovementComponent->bShouldBounce = true;
	
	InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlapBegin); //오버랩 이벤트 추가
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor
	, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*
	 -- 추가 예정 --
	*/
}

void ABullet::FireInDirection(const FVector& ShootDirection) 
{
	//ShootDirection 방향으로 InitialSpeed 만큼 발사
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}