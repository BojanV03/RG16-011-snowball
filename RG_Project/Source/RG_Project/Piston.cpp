// Fill out your copyright notice in the Description page of Project Settings.

#include "RG_Project.h"
#include "Piston.h"


// Sets default values
APiston::APiston()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	PistonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));

	RootComponent = Root;
	PistonMesh->SetupAttachment(RootComponent);

	// Set up the skeletalMesh, animations and collisions
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshContainer(TEXT("SkeletalMesh'/Game/Geometry/Skeletals/Piston/Piston.Piston'"));
	if (MeshContainer.Succeeded())
	{
		PistonMesh->SkeletalMesh = MeshContainer.Object;
		PistonMesh->AnimationData.bSavedLooping = false;
		PistonMesh->AnimationData.bSavedPlaying = false;
		static ConstructorHelpers::FObjectFinder<UAnimationAsset> AnimContainer(TEXT("AnimationAsset'/Game/Geometry/Skeletals/Piston/Piston_Anim.Piston_Anim'"));
		if (AnimContainer.Succeeded())
		{
			PistonMesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
			PistonMesh->AnimationData.AnimToPlay = AnimContainer.Object;
		}
		PistonMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		PistonMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	}
}

// Called when the game starts or when spawned
void APiston::BeginPlay()
{
	Super::BeginPlay();

	//Set piston in it's starting position
	Deactivate_Implementation();
}

void APiston::Activate_Implementation()
{
	PistonMesh->SetPlayRate(2 * direction);
	PistonMesh->Play(false);
	return;
}

void APiston::Deactivate_Implementation()
{
	PistonMesh->SetPlayRate(-2 * direction);
	PistonMesh->Play(false);
	return;
}
