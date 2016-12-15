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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stair")
		USceneComponent *Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OneTimeButton")
		USkeletalMeshComponent *Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OneTimeButton")
		UBoxComponent *CollisionDetection;
	
	//	Error: UPROPERTY Pointers cannot be interfaces, did you mean "TScriptInterface<IButtonActivated>"
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OneTimeButton")
		AActor *Target;
	
	UFUNCTION()
		void BeginOverlap(class UPrimitiveComponent* OverlapedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp2, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION()
		void EndOverlap(class UPrimitiveComponent* OverlapedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp2, int32 OtherBodyIndex);

	
};
