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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ButtonActivated")
		USkeletalMeshComponent *DoorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ButtonActivated")
		USceneComponent *Root;

	// This function is called when the button is Activated
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ButtonActivated")
		void Activate();
		virtual void Activate_Implementation() override;
	// ~||~										  Deactivated
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ButtonActivated")
		void Deactivate();
		virtual void Deactivate_Implementation() override;
	
};
