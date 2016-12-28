// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "RG_Project.h"
#include "MySaveGame.h"
#include "MeshFactory.h"
#include "RollingCodeBall.h"

#define PI 3.1415926f
#define CalculateSphereVolume(r) (4.0f*r*r*r/3.0f)*PI
#define FIREBALL_SKIN 8

ARollingCodeBall::ARollingCodeBall()
{
	// Preparing standard Meshes
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BallMesh(TEXT("/Game/Rolling/Meshes/BallMesh.BallMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BallSkinMesh(TEXT("StaticMesh'/Game/Geometry/Meshes/Skins/Snowball/Snowball.Snowball'"));
	// Preparing particle systems
	static ConstructorHelpers::FObjectFinder<UParticleSystem> SnowParticles(TEXT("ParticleSystem'/Game/Particles/Snow/Snow.Snow'"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> FireParticles(TEXT("ParticleSystem'/Game/Particles/Fire/P_Fire.P_Fire'"));

	// Create mesh component for the ball
	Ball = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball0"));
	Ball->SetStaticMesh(BallMesh.Object);
	Ball->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	Ball->SetSimulatePhysics(true);
	Ball->SetAngularDamping(0.5f);
	Ball->SetLinearDamping(0.5f);
	Ball->SetEnableGravity(true);
	Ball->BodyInstance.MassScale = 2.666f;
	Ball->SetNotifyRigidBodyCollision(true);
	Ball->SetMassScale("None", Ball->RelativeScale3D.GetAbsMax() * MassScaleMultiplier);
	Ball->SetVisibility(false);
	
	RootComponent = Ball;

	// Create a camera boom attached to the root (ball)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bAbsoluteRotation = true; // Rotation of the ball should not affect rotation of boom
	SpringArm->RelativeRotation = FRotator(-45.0f, 0.0f, 0.0f);
	SpringArm->TargetArmLength = 1200.0f;
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 3.0f;
	SpringArm->bDoCollisionTest = true;
	
	// Create a Skin; the visible part of the ball
	BallSkin = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallSkin"));
	BallSkin->SetupAttachment(RootComponent);
	BallSkin->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BallSkin->SetSimulatePhysics(false);
	BallSkin->SetStaticMesh(BallSkinMesh.Object);

	// Create a camera and attach to boom
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	CameraBoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	CameraBoxCollision->RelativeScale3D = FVector(0.1f, 0.1f, 0.1f);
	CameraBoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CameraBoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	CameraBoxCollision->SetupAttachment(Camera);

	// Create the Snow particle system
	SnowParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SnowParticle"));
	SnowParticle->SetupAttachment(Camera, "None");
	SnowParticle->RelativeLocation = FVector(SnowParticle->RelativeLocation.X, SnowParticle->RelativeLocation.Y, SnowParticle->RelativeLocation.Z + 250);
	SnowParticle->Deactivate();

	SnowParticle->Template = SnowParticles.Object;

	// Create the Fire particle system
	FireParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireParticle"));
	FireParticle->SetupAttachment(Ball, "None");
	FireParticle->Deactivate();
	FireParticle->SetRelativeScale3D(FVector(2.5f, 2.5f, 2.5f));
	FireParticle->Template = FireParticles.Object;

	// Set up forces
	RollTorque = 50000000.0f;
	JumpImpulse = 800.0f;
	// Other variables
	bCanJump = true; 
	MassScaleMultiplier = 2.666f;
	bIsCameraAttached = true;
	bCanMerge = false;
	bOnFire = false;
	MouseSensitivity = 5.0f;
	ScrollSensitivity = 150.0f;

	Resize(0.0f);
}

void ARollingCodeBall::activateSnow()
{
	if(!bOnFire)
		SnowParticle->Activate(false);
}

void ARollingCodeBall::deactivateSnow()
{
	SnowParticle->Deactivate();
}

void ARollingCodeBall::activateFire()
{
	FireParticle->Activate(false);
	bOnFire = true;
}

void ARollingCodeBall::deactivateFire()
{
	FireParticle->Deactivate();
	bOnFire = false;
}


void ARollingCodeBall::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key axis bindings
	PlayerInputComponent->BindAxis("MoveRight", this, &ARollingCodeBall::MoveRight);
	PlayerInputComponent->BindAxis("MoveForward", this, &ARollingCodeBall::MoveForward);
	PlayerInputComponent->BindAxis("CamRotateUp", this, &ARollingCodeBall::CamRotateUp);
	PlayerInputComponent->BindAxis("CamRotateRight", this, &ARollingCodeBall::CamRotateRight);
	PlayerInputComponent->BindAxis("CamZoomIn", this, &ARollingCodeBall::CamZoomIn);

	// set up gameplay key action bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ARollingCodeBall::Jump);
	PlayerInputComponent->BindAction("CamReset", IE_Pressed, this, &ARollingCodeBall::CamReset);
	PlayerInputComponent->BindAction("CamDetach", IE_Pressed, this, &ARollingCodeBall::CamDetach);
	PlayerInputComponent->BindAction("IncreaseSize", IE_Pressed, this, &ARollingCodeBall::IncreaseSize);
	PlayerInputComponent->BindAction("DecreaseSize", IE_Pressed, this, &ARollingCodeBall::DecreaseSize);
	PlayerInputComponent->BindAction("Split", IE_Pressed, this, &ARollingCodeBall::Split);
	PlayerInputComponent->BindAction("ToggleMerge", IE_Pressed, this, &ARollingCodeBall::ToggleMerge);
	PlayerInputComponent->BindAction("ChangeBallForward", IE_Pressed, this, &ARollingCodeBall::ChangeBallForward);
	PlayerInputComponent->BindAction("ChangeBallBackward", IE_Pressed, this, &ARollingCodeBall::ChangeBallBackward);
}

void ARollingCodeBall::MoveRight(float Val)
{
	// Torque = Radius * Force;
	// Force = Mass * Acceleration;
	// => Torque = Radius * Mass * Acceleration
	float radiusTimesMass = Ball->GetMass() * Ball->RelativeScale3D.GetMax();
	float intensity = -1.0f * (Val * RollTorque * radiusTimesMass) / 1000.0f;

	GEngine->AddOnScreenDebugMessage(0, 0.0f, FColor::Black, FString::Printf(TEXT("Right: %f"), intensity));

	FVector RightVector = SpringArm->GetForwardVector();
	RightVector = FVector(RightVector.X, RightVector.Y, 0.0f);
	RightVector.Normalize();

	FVector Torque = RightVector * intensity;
	Ball->AddTorque(Torque);
}

void ARollingCodeBall::MoveForward(float Val)
{
	float radiusTimesMass = Ball->GetMass() * Ball->RelativeScale3D.GetMax();
	float intensity = (Val * RollTorque * radiusTimesMass) / 1000.0f;

	GEngine->AddOnScreenDebugMessage(0, 0.0f, FColor::Black, FString::Printf(TEXT("Forward: %f"), intensity));

	FVector ForwardVector = SpringArm->GetForwardVector();
	ForwardVector = FVector(ForwardVector.Y*-1, ForwardVector.X, 0.0f);
	ForwardVector.Normalize();

	FVector Torque = ForwardVector * intensity;
	Ball->AddTorque(Torque);
}

void ARollingCodeBall::Jump()
{
	if(bCanJump)
	{
		// We want to reach the minimum JumpImpulse when Scale of the ball reaches 5 Unreal Units
		// but still maintain the same impulse between 0 and 1 UUnits
		float subtrahend = FMath::Clamp(Ball->RelativeScale3D.GetAbsMax(), 1.f, 5.f);		
		subtrahend -= 1; // This is now in a range between 0-4
		// We want a linear decrease all the way to 0 in the subtrahend of the zImpulse equation
		subtrahend *= 170; // therefore we scale it to fit the 0-700 region
		float zImpulse = Ball->GetMass() * (JumpImpulse - subtrahend);
		// Add impulse to the ball
		Ball->AddImpulse(FVector(0.f, 0.f, zImpulse));
		bCanJump = false;	// And reenable jumping
	}
}

void ARollingCodeBall::CamReset()
{
	// Set distance to ball to 1200 Unreal Units
	SpringArm->TargetArmLength = 1200;
	// Set the Pitch to -45 degrees, roll to 0 degrees
	SpringArm->SetWorldRotation(FRotator(-45.0f, SpringArm->GetComponentRotation().Yaw, 0.0f));
}

void ARollingCodeBall::CamDetach()
{
	if (bIsCameraAttached)
	{
		Camera->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}
	else
	{
		Camera->AttachToComponent(SpringArm, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
	bIsCameraAttached = !bIsCameraAttached;
}

void ARollingCodeBall::CamRotateRight(float Val)
{
	FRotator currentRotation = SpringArm->GetComponentRotation();
	SpringArm->SetWorldRotation(FRotator(currentRotation.Pitch, Val * MouseSensitivity + currentRotation.Yaw, currentRotation.Roll));
}

void ARollingCodeBall::CamRotateUp(float Val)
{
	FRotator currentRotation = SpringArm->GetComponentRotation();
	if (FMath::Abs(Val * MouseSensitivity + currentRotation.Pitch) < 89.5)
	{
		SpringArm->SetWorldRotation(FRotator(Val * MouseSensitivity + currentRotation.Pitch, currentRotation.Yaw, currentRotation.Roll));
		GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Orange, FString::Printf(TEXT("CameraAngle: %.2f"), FMath::Abs(Val * MouseSensitivity + currentRotation.Pitch)));
	}
}

void ARollingCodeBall::CamZoomIn(float Val)
{
	float newLength = (-1.0f*Val*ScrollSensitivity) + SpringArm->TargetArmLength;

	// We don't want negative lengths
	if (newLength > 1.0f)
	{
		SpringArm->TargetArmLength = newLength;
	}
	else
	{
		SpringArm->TargetArmLength = 1.0f;
	}
}

void ARollingCodeBall::Resize(float Val)
{
	// Refresh overlap detections after resizing(for example when resizing while on a button that measures weight
	// we want the new weight to be updated as soon as resizing happens and not force the player to leave the button
	// and come back. Achieved this by simply turning off Query, resizing and then turning Query back on
	Ball->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	// minScale and maxScale should be always the same, but maybe i'll add some other objects like eggs
	// that don't have the same scale values... Just making sure the function always works
	float maxScale = Ball->RelativeScale3D.GetMax();
	float minScale = Ball->RelativeScale3D.GetMin();

	minScale += Val;
	maxScale += Val;

	// Kill it
	if (minScale < 0.05)
	{
		Destroy();
	}
	else if (maxScale < 5)
	{
		Ball->SetRelativeScale3D(Ball->RelativeScale3D + Val);
		Ball->SetMassScale("None", maxScale * MassScaleMultiplier);
	}
	else
	{
		Ball->SetRelativeScale3D(FVector(4.99f, 4.99f, 4.99f));
		Ball->SetMassScale("None", 4.99f * MassScaleMultiplier);
	}

	Ball->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}
// Activates when pressing +, only for development, remove before shipping
void ARollingCodeBall::IncreaseSize()
{
	Resize(0.1f);
}

// Activates when pressing -, only for development, remove before shipping
void ARollingCodeBall::DecreaseSize()
{
	Resize(-0.1f);
}

void ARollingCodeBall::ToggleMerge()
{
	bCanMerge = !bCanMerge;
}

void ARollingCodeBall::ChangeBallForward()
{
	int newIndex = (GameMode->CurrentBallIndex + 1) % (GameMode->BallArray.Num());
	GEngine->AddOnScreenDebugMessage(-1, 6, FColor::Orange, FString::Printf(TEXT("NewIndex: %d"), newIndex));

	// We want to disable merging so that, when the player is controlling another ball, it should have all the
	// control over whether merging occurs or not
	bCanMerge = false;
	deactivateFire();
	deactivateSnow();

	// Enable particle systems
	GetWorld()->GetFirstPlayerController()->Possess((ARollingCodeBall*)GameMode->BallArray[newIndex]);
	if (bOnFire)
		((ARollingCodeBall*)GameMode->BallArray[newIndex])->activateFire();
	else
		((ARollingCodeBall*)GameMode->BallArray[newIndex])->activateSnow();

	GameMode->CurrentBallIndex = newIndex;
}

void ARollingCodeBall::ChangeBallBackward()
{
	int newIndex = (GameMode->CurrentBallIndex - 1 + GameMode->BallArray.Num()) % (GameMode->BallArray.Num());
	GEngine->AddOnScreenDebugMessage(-1, 6, FColor::Orange, FString::Printf(TEXT("NewIndex: %d"), newIndex));

	bCanMerge = false;

	// Enable particle systems
	GetWorld()->GetFirstPlayerController()->Possess((ARollingCodeBall*)GameMode->BallArray[newIndex]);
	if (bOnFire)
		((ARollingCodeBall*)GameMode->BallArray[newIndex])->activateFire();
	else
		((ARollingCodeBall*)GameMode->BallArray[newIndex])->activateSnow();

	GameMode->CurrentBallIndex = newIndex;
}

void ARollingCodeBall::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	bCanJump = true;
	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Emerald, FString::Printf(TEXT("CanMerge: %d"), bCanMerge ? 1 : 0));

	if (Other == nullptr)	//leave if the Other object is not an AActor
		return;

	// Try to cast it to a RollingCodeBall
	ARollingCodeBall *otherBall = Cast<ARollingCodeBall>(Other);

	//	if it's successful, merge 2 balls
	if (otherBall != nullptr && bCanMerge)
	{
		bCanMerge = false;
		//Disableing/Enabling Query collisions in order to retrigger any volumes the balls happen to be in
		Ball->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		otherBall->GetBall()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

		// We want to double the Volume, not the Radius, therefore some maths:
		FVector newScale;
		float ballVolume = CalculateSphereVolume(Ball->RelativeScale3D.GetMax());
		float otherBallVolume = CalculateSphereVolume(otherBall->GetBall()->RelativeScale3D.GetMax());
		float newRadius = FMath::Pow((3.0f * (ballVolume + otherBallVolume) / 4.0f) / PI, 0.333333f);
		newRadius = FMath::Clamp(newRadius, 0.0f, 5.0f);
		newScale = FVector(newRadius, newRadius, newRadius);
		Ball->SetRelativeScale3D(newScale);
		Resize(0.0f);
		Other->Destroy();
		Ball->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	} // Increase size
	else if (Other->GetName().StartsWith("Landscape", ESearchCase::IgnoreCase) || 
			(OtherComp != nullptr && OtherComp->GetMaterial(0) != nullptr && 
				OtherComp->GetMaterial(0)->GetName().StartsWith("Snow")))
	{
		float resizeValue = (1.0f / Ball->GetMass()) * (120.0f / (GameMode->FPS + 1));
		resizeValue = FMath::Clamp(resizeValue, 0.0f, 0.003f);
		Resize(resizeValue);
	} // Decrease size
	else if (OtherComp!= nullptr && OtherComp->GetMaterial(0) != nullptr && OtherComp->GetMaterial(0)->GetName().StartsWith("Hot"))
	{
		float resizeValue = (1.0f / Ball->GetMass()) * 5.0f * (120.0f / (GameMode->FPS + 1));
		resizeValue = FMath::Clamp(resizeValue, 0.0f, 0.01f);
		Resize(-resizeValue);
	}
}

void ARollingCodeBall::Split()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::FromInt(GameMode->BallArray.Num()));
	Ball->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	bCanMerge = false;

	float newScale = Ball->RelativeScale3D.GetMax();
	// We want to halve the Volume of the sphere, not the Radius, therefore some math:
	newScale /= FMath::Pow(2.0f, 1.0f / 3.0f);
	Ball->SetRelativeScale3D(FVector(newScale, newScale, newScale));
	Ball->SetMassScale("None", newScale*MassScaleMultiplier);

	FVector spawnLocation;
	spawnLocation.X = Ball->GetComponentLocation().X;
	spawnLocation.Y = Ball->GetComponentLocation().Y;
	spawnLocation.Z = Ball->GetComponentLocation().Z + newScale*110;
	// Make a transfor using the calculated Location/Scale and inherited Rotation
	FTransform spawnTransform = FTransform(Ball->GetComponentRotation(), spawnLocation, Ball->RelativeScale3D);
	
	FActorSpawnParameters F;
	F.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	GetWorld()->SpawnActor(ARollingCodeBall::GetClass(), &spawnTransform, F);

	Ball->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ARollingCodeBall::BeginPlay()
{
	Super::BeginPlay();
	// Get GameMode reference
	// Trigger camera overlap (if splitting while insideof a camera trigger volume, we want to activate it)
	// it is done at the end of this function
	CameraBoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	// Set GameMode
	if (GetWorld() != nullptr)
	{
		if (GetWorld()->GetAuthGameMode() != nullptr)
		{
			GameMode = (ARollingCodeGameMode*)GetWorld()->GetAuthGameMode();
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::FromInt(GameMode->FPS));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "GameModeNull");
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "WorldNull");
	}
	GameMode->BallArray.AddUnique(this);

	GEngine->AddOnScreenDebugMessage(-1, 250, FColor::Orange, FString::FromInt(GameMode->BallArray.Num()));

	// Load BallSkin from SaveGame	
	UMySaveGame* LoadGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	LoadGameInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot("0", 0));
	if (LoadGameInstance != nullptr)
	{
		BallSkinID = LoadGameInstance->BallSkinID;
		Ball->SetStaticMesh(GameMode->BallMeshFactory.getMeshFromID(BallSkinID));
		BallSkin->SetStaticMesh(GameMode->BallMeshFactory.getSkinFromID(BallSkinID));

		// if it is the first ball added to the array we want the particle effects
		if (GameMode->BallArray.Num() == 1)
		{
			if (BallSkinID == FIREBALL_SKIN)
			{
				bOnFire = true;
				SnowParticle->Deactivate();
				FireParticle->Activate();
			}
			else
			{
				FireParticle->Deactivate();
				SnowParticle->Activate();
			}
		} // otherwise turn them off
		else
		{
			SnowParticle->Deactivate();
			FireParticle->Deactivate();
		}
	}
	CameraBoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	Resize(0.0f);
}

void ARollingCodeBall::Tick(float DeltaSeconds)
{
	// if it is a fireball, we don't want it to inherit the Balls rotation
	if (bOnFire)
		FireParticle->SetWorldRotation(FRotator(0.0f, 0.0f, 0.0f));
	// If it's snow, we don't want it to inherit Camera's Scale
	else
		SnowParticle->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
}

void ARollingCodeBall::Destroyed()
{
	if(GameMode != nullptr && GameMode->BallArray.Contains(this))
		GameMode->BallArray.Remove(this);
}
