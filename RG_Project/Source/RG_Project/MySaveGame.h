// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class RG_PROJECT_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()
	
	public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RollingCodeGamemode)
			int BallSkinID;	
};
