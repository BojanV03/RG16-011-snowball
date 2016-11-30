// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Portal.generated.h"
UCLASS()
class RG_PROJECT_API APortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortal();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
		USceneComponent *Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
		UStaticMeshComponent *PortalPlane;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
		UStaticMeshComponent *PortalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
		UArrowComponent *Arrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
		APortal *TargetPortal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
		USceneCaptureComponent2D *PortalView;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
		UBoxComponent *Activator;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
		bool CanTeleport;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
		bool isPortalActivated = true;
	
	UFUNCTION()
		void PortalBeginOverlap(class UPrimitiveComponent* OverlapedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp2, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION()
		void ActivatorBeginOverlap(class UPrimitiveComponent* OverlapedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp2, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

};
