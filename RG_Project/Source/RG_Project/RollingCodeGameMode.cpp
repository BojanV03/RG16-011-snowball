// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "RG_Project.h"
#include "RollingCodeGameMode.h"
#include "RollingCodeBall.h"

ARollingCodeGameMode::ARollingCodeGameMode()
{
	// set default pawn class to our ball
	DefaultPawnClass = ARollingCodeBall::StaticClass();
}
