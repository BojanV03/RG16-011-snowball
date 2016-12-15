// Fill out your copyright notice in the Description page of Project Settings.

#include "RG_Project.h"
#include "ComplexDoor.h"


// Sets default values
AComplexDoor::AComplexDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	DoorMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));

	RootComponent = Root;
	DoorMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AComplexDoor::BeginPlay()
{
	Super::BeginPlay();

	count = 0;
}

// Called every frame
void AComplexDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AComplexDoor::Activate_Implementation()
{
	//	DoorMesh->PlayAnimation(DoorMesh, true);
	FString strDebug = "";
	count++;
	strDebug.AppendInt(count);
	strDebug.Append(" out of ");
	strDebug.AppendInt(numberOfActivators);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, strDebug);
	if (count == numberOfActivators)
	{
		DoorMesh->SetPlayRate(2);
		DoorMesh->Play(false);
	}
	return;
}

void AComplexDoor::Deactivate_Implementation()
{
	count--;
	if (count == numberOfActivators - 1)
	{
		DoorMesh->SetPlayRate(-2);
		DoorMesh->Play(false);
	}
	return;
}
