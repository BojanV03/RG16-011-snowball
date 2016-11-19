// Fill out your copyright notice in the Description page of Project Settings.

#include "RG_Project.h"
#include "BasicCube.h"


// Sets default values
ABasicCube::ABasicCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
//	static ConstructorHelpers::FObjectFinder<UStaticMesh>StaticMeshOb_torus(TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube.1M_Cube'"));
//	if (StaticMeshOb_torus.Object)
//		Cube->SetStaticMesh(StaticMeshOb_torus.Object);
	RootComponent = Root;
}

// Called when the game starts or when spawned
void ABasicCube::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasicCube::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

