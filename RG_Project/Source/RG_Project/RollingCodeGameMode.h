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

	UPROPERTY()
		UStaticMesh* Meshes[14];
	// Used for getting the mesh/skin for the selected ball
//	MeshFactory BallMeshFactory;
	UPROPERTY()
		UStaticMesh* SphereMesh;

	/** Returns a mesh that is used for collisions for the ball with index n */
	UStaticMesh* getMeshFromID(int n);
	/** Returns a skin that is used just for aesthetics for the ball with index n */
	UStaticMesh* getSkinFromID(int n);

	virtual void Tick(float DeltaSeconds) override;
};
