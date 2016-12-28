// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ButtonActivated.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UButtonActivated : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * Interface implemented by classes that can be activated by buttons
 */
class RG_PROJECT_API IButtonActivated
{
	GENERATED_IINTERFACE_BODY()
public:
	// these functions can be overridden and called in Blueprints
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ButtonActivated")
		void Activate();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ButtonActivated")
		void Deactivate();
	
};
