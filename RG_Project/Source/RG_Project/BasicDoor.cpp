// Fill out your copyright notice in the Description page of Project Settings.

#include "RG_Project.h"
#include "BasicDoor.h"


// Sets default values
ABasicDoor::ABasicDoor()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> DoorMeshContainer(TEXT("SkeletalMesh'/Game/Geometry/Skeletals/AnimatedDoor/AnimatedDoor.AnimatedDoor'"));
	// Allow these objects to be set/modified from within UE4
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	DoorMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));

	// Set up the skeletalMesh, animations and collisions
	if (DoorMeshContainer.Object != nullptr)
	{
		DoorMesh->SkeletalMesh = DoorMeshContainer.Object;
		DoorMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		DoorMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		DoorMesh->AnimationData.bSavedLooping = false;
		DoorMesh->AnimationData.bSavedPlaying = false;
		static ConstructorHelpers::FObjectFinder<UAnimationAsset> AnimContainer(TEXT("AnimSequence'/Game/Geometry/Skeletals/AnimatedDoor/AnimatedDoor_Anim.AnimatedDoor_Anim'"));
		if (AnimContainer.Succeeded())
		{
			DoorMesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
			DoorMesh->AnimationData.AnimToPlay = AnimContainer.Object;
		}

	}
	RootComponent = Root;
	DoorMesh->SetupAttachment(RootComponent);
}

void ABasicDoor::Activate_Implementation()
{
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