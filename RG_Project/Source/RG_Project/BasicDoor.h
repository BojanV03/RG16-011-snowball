// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ButtonActivated.h"
#include "BasicDoor.generated.h"

UCLASS()
class RG_PROJECT_API ABasicDoor : public AActor, public IButtonActivated
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasicDoor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ButtonActivated")
		USkeletalMeshComponent *DoorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ButtonActivated")
		USceneComponent *Root;

//	UPROPERTY(EditAnywhere)
//		UAnimationAsset *DoorAnimation;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ButtonActivated")
		void Activate();
		virtual void Activate_Implementation() override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ButtonActivated")
		void Deactivate();
		virtual void Deactivate_Implementation() override;
	
};
