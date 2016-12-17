// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ButtonActivated.h"
#include "Piston.generated.h"

UCLASS()
class RG_PROJECT_API APiston : public AActor, public IButtonActivated
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APiston();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piston")
		int direction = 1;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piston")
		USkeletalMeshComponent *PistonMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Piston")
		USceneComponent *Root;
	// This function is called when the button is Activated
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Piston")

		void Activate();
	virtual void Activate_Implementation() override;
	// ~||~										  Deactivated
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Piston")
		void Deactivate();
	virtual void Deactivate_Implementation() override;

	
};
