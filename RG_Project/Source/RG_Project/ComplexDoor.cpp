// Fill out your copyright notice in the Description page of Project Settings.

#include "RG_Project.h"
#include "ComplexDoor.h"


// Sets default values
AComplexDoor::AComplexDoor()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> DoorMeshContainer(TEXT("SkeletalMesh'/Game/Geometry/Skeletals/ComplexDoor/ComplexDoor.ComplexDoor'"));

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	DoorMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));

	if (DoorMeshContainer.Object != nullptr)
	{
		DoorMesh->SkeletalMesh = DoorMeshContainer.Object;
		DoorMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		DoorMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		DoorMesh->AnimationData.bSavedLooping = false;
		DoorMesh->AnimationData.bSavedPlaying = false;
		static ConstructorHelpers::FObjectFinder<UAnimationAsset> AnimContainer(TEXT("AnimSequence'/Game/Geometry/Skeletals/ComplexDoor/ComplexDoor_Anim.ComplexDoor_Anim'"));
		if (AnimContainer.Succeeded())
		{
			DoorMesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
			DoorMesh->AnimationData.AnimToPlay = AnimContainer.Object;
		}

	}

	RootComponent = Root;
	DoorMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AComplexDoor::BeginPlay()
{
	Super::BeginPlay();

	count = 0;
}

void AComplexDoor::Activate_Implementation()
{
	count++;	// If we activated enough buttons, play the animation
	if (count == numberOfActivators)
	{
		DoorMesh->SetPlayRate(2);
		DoorMesh->Play(false);
	}

	FString strDebug = "";
	strDebug.AppendInt(count);
	strDebug.Append(" out of ");
	strDebug.AppendInt(numberOfActivators);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, strDebug);
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
