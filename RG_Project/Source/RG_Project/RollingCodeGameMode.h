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

	TArray<AActor*> BallArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RollingCodeGamemode)
		int CurrentBallIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RollingCodeGamemode)
		int FPS;

	MeshFactory BallMeshFactory;

	virtual void Tick(float DeltaSeconds) override;
};
