// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BasicCube.generated.h"

UCLASS()
class RG_PROJECT_API ABasicCube : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasicCube();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MengerSponge")
		USceneComponent *Root;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MengerSponge")
		UStaticMeshComponent *Cube;
	
	
};
