// Fill out your copyright notice in the Description page of Project Settings.

#include "RG_Project.h"
#include "MengerSponge.h"



// Sets default values
AMengerSponge::AMengerSponge()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	RootComponent = Root;
}

// Called when the game starts or when spawned
void AMengerSponge::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frames
void AMengerSponge::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

int AMengerSponge::GenerateSponge(FVector TopFrontLeft, FVector BottomRearRight, int depth)
{
	if (depth <= 1)
		return 0;
//	UStaticMeshComponent MS = UStaticMeshComponent();
//	MS->SetStaticMesh(Cube->StaticMesh);
//	MS->AttachToComponent(Root, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
//	MS->SetRelativeLocation(TopFrontLeft);
//	GenerateSponge(FVector(TopFrontLeft.X, TopFrontLeft.Y, TopFrontLeft.Z + 15), TopFrontLeft, depth--);
	return 0;
}

