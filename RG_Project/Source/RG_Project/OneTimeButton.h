// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ButtonActivated.h"
#include "OneTimeButton.generated.h"

UCLASS()
class RG_PROJECT_API AOneTimeButton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOneTimeButton();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	bool activated = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OneTimeButton")
		USceneComponent *Root;
	/** SkeletalMesh subobject */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OneTimeButton")
		USkeletalMeshComponent *Mesh;
	/** Box Component used for detecting collisions with the ARollingCodeBall */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OneTimeButton")
		UBoxComponent *CollisionDetection;
	/** Target to be activated */	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OneTimeButton")
		AActor *Target;
	
	UFUNCTION()
		void BeginOverlap(class UPrimitiveComponent* OverlapedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp2, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION()
		void EndOverlap(class UPrimitiveComponent* OverlapedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp2, int32 OtherBodyIndex);

	
};
