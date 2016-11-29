// Fill out your copyright notice in the Description page of Project Settings.

#include "RG_Project.h"
#include "Portal.h"


// Sets default values
APortal::APortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalMesh"));
	PortalPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalPlane"));
//	PortalView = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("PortalPlane"));

	RootComponent = Root;

	PortalMesh->AttachToComponent(Root, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	PortalPlane->AttachToComponent(Root, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
//	Arrow->AttachToComponent(Root, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
//	Arrow->SetRelativeRotation(FRotator(0, 0, -90));
}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APortal::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (isPortalActivated)
	{
		FVector Location;
		FTransform CameraTransform = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetTransform();
		Location = TargetPortal->GetActorLocation() - this->GetActorLocation();
		Location += CameraTransform.GetLocation();

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetTransform().GetLocation().ToString());

		PortalView->SetWorldLocationAndRotation(Location, CameraTransform.GetRotation(), false);
		PortalView->CaptureScene();
	}
}

void APortal::BeginOverlap(UPrimitiveComponent * OverlapedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp2, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
}

void APortal::EndOverlap(UPrimitiveComponent * OverlapedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp2, int32 OtherBodyIndex)
{
}