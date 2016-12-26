// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Pawn.h"
#include "RG_Project.h"


/**
 * 
 */
class RG_PROJECT_API MeshFactory
{
public:

	MeshFactory();

	UPROPERTY(EditAnywhere, Category = RollingCodeGamemode)
		UStaticMesh* Meshes[12];

	UPROPERTY(EditAnywhere, Category = RollingCodeGamemode)
		UStaticMesh* SphereMesh;

	UStaticMesh* getMeshFromID(int n);
	UStaticMesh* getSkinFromID(int n);
	~MeshFactory();
};
