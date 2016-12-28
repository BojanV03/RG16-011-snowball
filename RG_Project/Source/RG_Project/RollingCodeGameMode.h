// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameModeBase.h"
#include "MeshFactory.h"
#include "RollingCodeGameMode.generated.h"

UCLASS(minimalapi)
class ARollingCodeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARollingCodeGameMode();
	// Contains and allows us to iterate through all the balls that are currently in the level
	TArray<AActor*> BallArray;
	// Index of the possessed ball
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RollingCodeGamemode)
		int CurrentBallIndex;
	// FPS Counter
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RollingCodeGamemode)
		int FPS;
	// Used for getting the mesh/skin for the selected ball
	MeshFactory BallMeshFactory;

	virtual void Tick(float DeltaSeconds) override;
};
