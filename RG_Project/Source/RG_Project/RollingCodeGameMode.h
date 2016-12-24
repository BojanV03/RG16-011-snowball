// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameModeBase.h"
#include "RollingCodeGameMode.generated.h"

UCLASS(minimalapi)
class ARollingCodeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARollingCodeGameMode();

	TArray<AActor*> BallArray;
	UPROPERTY(EditAnywhere, Category = Gamemode)
		int CurrentBallIndex;
	UPROPERTY(EditAnywhere, Category = Gamemode)
		int FPS;

	virtual void Tick(float DeltaSeconds) override;
};



