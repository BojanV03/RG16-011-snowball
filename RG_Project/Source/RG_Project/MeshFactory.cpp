// Fill out your copyright notice in the Description page of Project Settings.

#include "RG_Project.h"
#include "MeshFactory.h"

#define CUSTOM_SKIN_COLLISIONS 9

MeshFactory::MeshFactory()
{
	
	// Crashes the engine on compilation ("FObjectFinder must be used inside of a constructor"...?)
	// but still compiles successfully and works fine after restarting?;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BasicBallCollision(TEXT("StaticMesh'/Game/Rolling/Meshes/BallMesh.BallMesh'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SnowBallSkin(TEXT("StaticMesh'/Game/Geometry/Meshes/Skins/Snowball/Snowball.Snowball'"));	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WireSphere(TEXT("StaticMesh'/Game/Geometry/Meshes/Skins/WireSphere/WireSphere.WireSphere'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphericalHelix(TEXT("StaticMesh'/Game/Geometry/Meshes/Skins/SphericalHelix/Spherical_Helix.Spherical_Helix'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Ball8(TEXT("StaticMesh'/Game/Geometry/Meshes/Skins/8Ball/8Ball.8Ball'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> AwesomeBall(TEXT("StaticMesh'/Game/Geometry/Meshes/Skins/AwesomeBall/AwesomeBall.AwesomeBall'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Eyeball(TEXT("StaticMesh'/Game/Geometry/Meshes/Skins/EyeBall/Eyeball.Eyeball'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GolfBall(TEXT("StaticMesh'/Game/Geometry/Meshes/Skins/GolfBall/GolfBall.GolfBall'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ReflectionSphere(TEXT("StaticMesh'/Game/Geometry/Meshes/Skins/ReflectionSphere/ReflectionSphere.ReflectionSphere'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Fireball(TEXT("StaticMesh'/Game/Geometry/Meshes/Skins/MarioFireball/Fireball.Fireball'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> F1Tire(TEXT("StaticMesh'/Game/Geometry/Meshes/Skins/F1Tire/F1Tire.F1Tire'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> HolyHandGranade(TEXT("StaticMesh'/Game/Geometry/Meshes/Skins/HolyHandGranade/HolyHandGranade.HolyHandGranade'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> RubixCube(TEXT("StaticMesh'/Game/Geometry/Meshes/Skins/RubixCube/Rubix.Rubix'"));	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> KoopaShell(TEXT("StaticMesh'/Game/Geometry/Meshes/Skins/KoopaShell/KoopaShell.KoopaShell'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> InterlockedMobiusStrips(TEXT("StaticMesh'/Game/Geometry/Meshes/Skins/MobiusStrips/InterlockedMobiusStrips.InterlockedMobiusStrips'"));
	

	Meshes[0] = SnowBallSkin.Object;
	Meshes[1] = WireSphere.Object;
	Meshes[2] = SphericalHelix.Object;
	Meshes[3] = Ball8.Object;
	Meshes[4] = AwesomeBall.Object;
	Meshes[5] = Eyeball.Object;
	Meshes[6] = GolfBall.Object;
	Meshes[7] = ReflectionSphere.Object;
	Meshes[8] = Fireball.Object;
	Meshes[CUSTOM_SKIN_COLLISIONS    ] = F1Tire.Object;
	Meshes[CUSTOM_SKIN_COLLISIONS + 1] = HolyHandGranade.Object;
	Meshes[CUSTOM_SKIN_COLLISIONS + 2] = KoopaShell.Object;
	Meshes[CUSTOM_SKIN_COLLISIONS + 3] = RubixCube.Object;
	Meshes[CUSTOM_SKIN_COLLISIONS + 4] = InterlockedMobiusStrips.Object;

	SphereMesh = BasicBallCollision.Object;
}

UStaticMesh* MeshFactory::getMeshFromID(int n)
{
	if (n >= CUSTOM_SKIN_COLLISIONS)
		return Meshes[n];
	else
		return SphereMesh;
}

UStaticMesh* MeshFactory::getSkinFromID(int n)
{
	return Meshes[n];
	return SphereMesh;
}
	
	
MeshFactory::~MeshFactory()
{
	
}