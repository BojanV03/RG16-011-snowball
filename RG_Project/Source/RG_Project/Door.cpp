// Fill out your copyright notice in the Description page of Project Settings.

#include "RG_Project.h"
#include "Door.h"


// Sets default values
ADoor::ADoor()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void ADoor::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}
/*
void ADoor::Activate()
{
	return;
}

void ADoor::Deactivate()
{
	return;
}*/

