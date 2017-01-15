// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "RG_Project.h"

class RG_PROJECT_API MeshFactory
{
public:
	MeshFactory();

	UPROPERTY(BlueprintReadWrite)
		UStaticMesh* Meshes[14];

	UPROPERTY()
		UStaticMesh* SphereMesh;
	/** Returns a mesh that is used for collisions for the ball with index n */
	UStaticMesh* getMeshFromID(int n);
	/** Returns a skin that is used just for aesthetics for the ball with index n */
	UStaticMesh* getSkinFromID(int n);
	~MeshFactory();
};
