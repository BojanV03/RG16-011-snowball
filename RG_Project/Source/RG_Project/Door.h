// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "ButtonActivated.h"
#include "Door.generated.h"

UCLASS()
class RG_PROJECT_API ADoor : public APawn//, public IButtonActivated
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADoor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

//	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ButtonActivated")
//		void Activate();
//	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ButtonActivated")
//		void Deactivate();
};
