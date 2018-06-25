// Fill out your copyright notice in the Description page of Project Settings.



#include "PickUpBase.h"

#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"

// Sets default values
APickUpBase::APickUpBase()
{

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//All pickups start active
	bIsActive = true;

	//Create the static mesh component
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));


	RootComponent = PickupMesh;
}

// Called when the game starts or when spawned
void APickUpBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APickUpBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//Returns active state
bool APickUpBase::IsActive() {
	return bIsActive;
}

//Changes active state
void APickUpBase::SetActive(bool NewPickupState) {
	bIsActive = NewPickupState;
}