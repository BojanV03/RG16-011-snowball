// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ButtonActivated.h"
#include "ComplexDoor.generated.h"

UCLASS()
class RG_PROJECT_API AComplexDoor : public AActor, public IButtonActivated
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AComplexDoor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Ammount of buttons that are needed to be pressed in order to activate the door*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ComplexDoor")
		int numberOfActivators;
	/** SkeletalMesh subobject*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ComplexDoor")
		USkeletalMeshComponent *DoorMesh;
	/** Root of the object*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ComplexDoor")
		USceneComponent *Root;
	/** Number of activated buttons */
	int count;

	// This function is called when the button is Activated
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ComplexDoor")
		void Activate();
	virtual void Activate_Implementation() override;
	// ~||~										  Deactivated
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ComplexDoor")
		void Deactivate();
	virtual void Deactivate_Implementation() override;
	
};
