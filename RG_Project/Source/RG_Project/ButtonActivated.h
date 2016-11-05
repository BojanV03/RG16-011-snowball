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
 * 
 */
class RG_PROJECT_API IButtonActivated
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// these functions can be overridden and called in Blueprints
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ButtonActivated")
		void Activate();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ButtonActivated")
		void Deactivate();
	
};
