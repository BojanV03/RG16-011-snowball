// Fill out your copyright notice in the Description page of Project Settings.

#include "RG_Project.h"
#include "BasicDoor.h"


// Sets default values
ABasicDoor::ABasicDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Allow these objects to be set/modified from within UE4
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	DoorMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));


	RootComponent = Root;
	DoorMesh->AttachToComponent(Root, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

// Called when the game starts or when spawned
void ABasicDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasicDoor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ABasicDoor::Activate_Implementation()
{
//	DoorMesh->PlayAnimation(DoorMesh, true);
	DoorMesh->SetPlayRate(2);
	DoorMesh->Play(false);
	return;
}

void ABasicDoor::Deactivate_Implementation()
{
	DoorMesh->SetPlayRate(-2);
	DoorMesh->Play(false);
	return;
}