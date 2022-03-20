// Fill out your copyright notice in the Description page of Project Settings.


#include "PathBase.h"

// Sets default values
APathBase::APathBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->Tags = { "Path" };
}

// Called when the game starts or when spawned
void APathBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APathBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

