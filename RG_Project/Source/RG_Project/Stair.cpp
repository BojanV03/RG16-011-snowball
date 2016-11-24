// Fill out your copyright notice in the Description page of Project Settings.

#include "RG_Project.h"
#include "Stair.h"


// Sets default values
AStair::AStair()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	StairMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	CollisionDetection = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = Root;
	StairMesh->AttachToComponent(Root, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	CollisionDetection->AttachToComponent(Root, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	CollisionDetection->RelativeScale3D = FVector(3, 3, 3);
	CollisionDetection->RelativeLocation = FVector(0, 0, 75);
	CollisionDetection->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionDetection->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);

	CollisionDetection->OnComponentBeginOverlap.AddDynamic(this, &AStair::BeginOverlap);
	CollisionDetection->OnComponentEndOverlap.AddDynamic(this, &AStair::EndOverlap);
}


// Called when the game starts or when spawned
void AStair::BeginPlay()
{
	Super::BeginPlay();
	StairMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StairMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	
}

// Called every frame
// For some reason using a timer crashes my entire project which is why i'm putting all the timing logic in here
void AStair::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (NearByBalls > 0)		// If there are 1 or more balls in the vicinity of the Stair
	{
		if(StairMesh->bHiddenInGame == true)
			StairMesh->SetHiddenInGame(false);		// Make sure the Stair is visible to the player

		if (StairMesh->RelativeLocation.Z < height-0.1)	// And move it up
		{
			FVector newPosition = FVector(0, 0, (StairMesh->RelativeLocation.Z + (height*DeltaTime) / AnimationLength));	
			StairMesh->SetRelativeLocation(newPosition);
		}
		else		// Clamp to desired height
		{
			FVector newPosition = FVector(0, 0, height);
			StairMesh->SetRelativeLocation(newPosition);
		}
	}
	else
	{
		if (StairMesh->RelativeLocation.Z > 0.1)
		{
			FVector newPosition = FVector(0, 0, (StairMesh->RelativeLocation.Z - (height*DeltaTime) / AnimationLength));
			StairMesh->SetRelativeLocation(newPosition);
		}
		else
		{
			FVector newPosition = FVector(0, 0, 0);
			StairMesh->SetRelativeLocation(newPosition);
			StairMesh->SetHiddenInGame(true);
		}
	}

}

void AStair::BeginOverlap(UPrimitiveComponent * OverlapedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp2, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	NearByBalls++;

}

void AStair::EndOverlap(UPrimitiveComponent * OverlapedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp2, int32 OtherBodyIndex)
{
	NearByBalls--;
}

void AStair::ActivateTimer(void)
{

}


