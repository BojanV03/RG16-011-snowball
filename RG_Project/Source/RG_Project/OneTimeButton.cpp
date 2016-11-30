// Fill out your copyright notice in the Description page of Project Settings.

#include "RG_Project.h"
#include "OneTimeButton.h"


// Sets default values
AOneTimeButton::AOneTimeButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Allow these objects to be set/modified from within UE4
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("StaticMesh"));
	CollisionDetection = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));

	RootComponent = Root;
	Mesh->AttachToComponent(Root, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	CollisionDetection->AttachToComponent(Root, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	CollisionDetection->OnComponentBeginOverlap.AddDynamic(this, &AOneTimeButton::BeginOverlap);
	CollisionDetection->OnComponentEndOverlap.AddDynamic(this, &AOneTimeButton::EndOverlap);
}

void AOneTimeButton::BeginOverlap(UPrimitiveComponent * OverlapedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp2, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	IButtonActivated* iTarget = Cast<IButtonActivated>(Target);
	if (iTarget)
	{
		//Don't call your functions directly, use the 'Execute_' prefix
		//the Execute_ReactToHighNoon and Execute_ReactToMidnight are generated on compile
		//you may need to compile before these functions will appear
//		iTarget->Execute_Activate(OtherActor);
		iTarget->Execute_Activate(Target);
		Mesh->SetPlayRate(1.5);
		Mesh->Play(false);
		return;
	}

}

void AOneTimeButton::EndOverlap(UPrimitiveComponent * OverlapedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp2, int32 OtherBodyIndex)
{
	
}
// Called when the game starts or when spawned
void AOneTimeButton::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOneTimeButton::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

