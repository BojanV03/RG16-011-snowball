// Fill out your copyright notice in the Description page of Project Settings.

#include "RG_Project.h"
#include "Portal.h"


// Sets default values
APortal::APortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Allow these objects to be set/modified from within UE4
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalMesh"));
	PortalPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalPlane"));
	Activator = CreateDefaultSubobject<UBoxComponent>(TEXT("Activator"));

	RootComponent = Root;

	PortalMesh->SetupAttachment(RootComponent);
	PortalPlane->SetupAttachment(RootComponent);
	Activator->SetupAttachment(RootComponent);
//	PortalMesh->AttachToComponent(Root, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
//	PortalPlane->AttachToComponent(Root, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
//	Activator->AttachToComponent(Root, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	
	PortalPlane->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PortalPlane->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	PortalPlane->OnComponentBeginOverlap.AddDynamic(this, &APortal::PortalBeginOverlap);
	Activator->OnComponentBeginOverlap.AddDynamic(this, &APortal::ActivatorBeginOverlap);
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
		//CamLocation = Portal2 - Portal1 + PlayerCam
		FVector Location;

		FTransform CameraTransform = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetTransform();
		Location = TargetPortal->GetActorLocation() - this->GetActorLocation();
		Location += CameraTransform.GetLocation();

//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetTransform().GetLocation().ToString());

		PortalView->SetWorldLocationAndRotation(Location, CameraTransform.GetRotation(), false);
		PortalView->CaptureScene();
	}
}

void APortal::PortalBeginOverlap(UPrimitiveComponent * OverlapedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp2, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	isPortalActivated = false;
	OtherActor->SetActorLocation(TargetPortal->GetActorLocation());
	
}

void APortal::ActivatorBeginOverlap(UPrimitiveComponent * OverlapedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp2, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	isPortalActivated = true;
}