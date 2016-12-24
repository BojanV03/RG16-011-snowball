// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "RG_Project.h"
#include "RollingCodeGameMode.h"
#include "RollingCodeBall.h"

ARollingCodeGameMode::ARollingCodeGameMode()
{
	// set default pawn class to our ball
	DefaultPawnClass = ARollingCodeBall::StaticClass();
	PrimaryActorTick.bCanEverTick = true;
}


void ARollingCodeGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FPS = (int) 1.0f / DeltaTime;
	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Cyan, FString::Printf(TEXT("FPS: %d"), FPS));
}