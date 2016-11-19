// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Stair.generated.h"

UCLASS()
class RG_PROJECT_API AStair : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStair();



	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	FTimerHandle ActivateTimerHandle;
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	// Number of balls in the vicinity of the Stair
	int NearByBalls = 0;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stair")
		float width=100; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stair")
		float height=100;			//how far up should the stairs move
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stair")
		float length = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stair")
		float AnimationLength = 1;	//in seconds!
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stair")
		UStaticMeshComponent *StairMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stair")
		USceneComponent *Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stair")
		UBoxComponent *CollisionDetection;

	UFUNCTION() 
		void BeginOverlap(class UPrimitiveComponent* OverlapedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp2, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION() 
		void EndOverlap(class UPrimitiveComponent* OverlapedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp2, int32 OtherBodyIndex);

	// For some reason using a timer crashes my entire project which is why i'm sticking with the Tick event
	UFUNCTION()
		void ActivateTimer();
};
