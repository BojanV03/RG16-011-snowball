// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "RG_Project.h"
#include "RollingCodeBall.h"

#define PI 3.1415926f
#define CalculateSphereVolume(r) (4.0f*r*r*r/3.0f)*PI
ARollingCodeBall::ARollingCodeBall()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BallMesh(TEXT("/Game/Rolling/Meshes/BallMesh.BallMesh"));

	// Create mesh component for the ball
	Ball = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball0"));
	Ball->SetStaticMesh(BallMesh.Object);
	Ball->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	Ball->SetSimulatePhysics(true);
	Ball->SetAngularDamping(0.1f);
	Ball->SetLinearDamping(0.1f);
	Ball->BodyInstance.MassScale = 3.5f;
	Ball->BodyInstance.MaxAngularVelocity = 800.0f;
	Ball->SetNotifyRigidBodyCollision(true);
	Ball->SetMassScale("None", Ball->RelativeScale3D.GetAbsMax() * MassScaleMultiplier);
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


	// Create a camera and attach to boom
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Set up forces
	RollTorque = 50000000.0f;
	JumpImpulse = 700.0f;
	// Other variables
	bCanJump = true; 
	MassScaleMultiplier = 3.5f;
	bIsCameraAttached = true;
	bCanMerge = false;
	bOnFire = false;
	Radius = 100.0f;
	MouseSensitivity = 5.0f;
	ScrollSensitivity = 150.0f;

	Resize(0.0f);
}


void ARollingCodeBall::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAxis("MoveRight", this, &ARollingCodeBall::MoveRight);
	PlayerInputComponent->BindAxis("MoveForward", this, &ARollingCodeBall::MoveForward);
	PlayerInputComponent->BindAxis("CamRotateUp", this, &ARollingCodeBall::CamRotateUp);
	PlayerInputComponent->BindAxis("CamRotateRight", this, &ARollingCodeBall::CamRotateRight);
	PlayerInputComponent->BindAxis("CamZoomIn", this, &ARollingCodeBall::CamZoomIn);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ARollingCodeBall::Jump);
	PlayerInputComponent->BindAction("CamReset", IE_Pressed, this, &ARollingCodeBall::CamReset);
	PlayerInputComponent->BindAction("CamDetach", IE_Pressed, this, &ARollingCodeBall::CamDetach);
	PlayerInputComponent->BindAction("IncreaseSize", IE_Pressed, this, &ARollingCodeBall::IncreaseSize);
	PlayerInputComponent->BindAction("DecreaseSize", IE_Pressed, this, &ARollingCodeBall::DecreaseSize);
	PlayerInputComponent->BindAction("Split", IE_Pressed, this, &ARollingCodeBall::Split);
	PlayerInputComponent->BindAction("ToggleMerge", IE_Pressed, this, &ARollingCodeBall::ToggleMerge);
	PlayerInputComponent->BindAction("ChangeBallForward", IE_Pressed, this, &ARollingCodeBall::ChangeBallForward);
	PlayerInputComponent->BindAction("ChangeBallBackward", IE_Pressed, this, &ARollingCodeBall::ChangeBallBackward);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ARollingCodeBall::JumpStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ARollingCodeBall::JumpStopped);
}

void ARollingCodeBall::MoveRight(float Val)
{
	float radiusTimesMass = Ball->GetMass() * Ball->RelativeScale3D.GetMax();
	float intensity = -1.0f * (Val * RollTorque * radiusTimesMass) / 1000.0f;

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
		// We want a linear decrease all the way to 0in the subtrahend of the zImpulse equation
		subtrahend *= 170; // therefore we scale it to fit the 0-700 region
		float zImpulse = Ball->GetMass() * (JumpImpulse - subtrahend);
		// Add impulse to the ball
		Ball->AddImpulse(FVector(0.f, 0.f, zImpulse));
bCanJump = false;	// And reenable jumping
	}
}

void ARollingCodeBall::CamReset()
{
	SpringArm->TargetArmLength = 1200;
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
	if (FMath::Abs(Val * MouseSensitivity + currentRotation.Pitch) < 89.99)
	{
		SpringArm->SetWorldRotation(FRotator(Val * MouseSensitivity + currentRotation.Pitch, currentRotation.Yaw, currentRotation.Roll));
	}
}

void ARollingCodeBall::CamZoomIn(float Val)
{
	float newLength = (-1.0f*Val*ScrollSensitivity) + SpringArm->TargetArmLength;
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

	float maxScale = Ball->RelativeScale3D.GetMax();
	float minScale = Ball->RelativeScale3D.GetMax();

	minScale += Val;
	maxScale += Val;

	if (minScale < 0.05)
	{
		Destroy();
	}
	else if (maxScale < 5)
	{
		Ball->SetRelativeScale3D(Ball->RelativeScale3D + Val);
		Ball->SetMassScale("None", maxScale * MassScaleMultiplier);
	}

	Ball->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ARollingCodeBall::IncreaseSize()
{
	Resize(0.1f);
}

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

	bCanMerge = false;
	
	GetWorld()->GetFirstPlayerController()->Possess((ARollingCodeBall*)GameMode->BallArray[newIndex]);
	GameMode->CurrentBallIndex = newIndex;
}

void ARollingCodeBall::ChangeBallBackward()
{
	int newIndex = (GameMode->CurrentBallIndex - 1 + GameMode->BallArray.Num()) % (GameMode->BallArray.Num());
	GEngine->AddOnScreenDebugMessage(-1, 6, FColor::Orange, FString::Printf(TEXT("NewIndex: %d"), newIndex));
	bCanMerge = false;
	GetWorld()->GetFirstPlayerController()->Possess((ARollingCodeBall*)GameMode->BallArray[newIndex]);
	GameMode->CurrentBallIndex = newIndex;
}

void ARollingCodeBall::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	bCanJump = true;
	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Emerald, FString::Printf(TEXT("CanMerge: %d"), bCanMerge ? 1 : 0));

	if (Other == nullptr)	//leave if the Other object is not an AActor
		return;

	ARollingCodeBall *otherBall = Cast<ARollingCodeBall>(Other);
	
	//Merge 2 balls
	if (otherBall != nullptr && bCanMerge)
	{
		bCanMerge = false;
		Ball->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		otherBall->GetBall()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

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
			 OtherComp->GetMaterial(0)->GetName().StartsWith("Snow"))
	{
		float resizeValue = (1.0f / Ball->GetMass()) * (120.0f / (GameMode->FPS + 1));
		resizeValue = FMath::Clamp(resizeValue, 0.0f, 0.003f);
		Resize(resizeValue);
	} // Decrease size
	else if (OtherComp->GetMaterial(0)->GetName().StartsWith("Hot"))
	{
		float resizeValue = (1.0f / Ball->GetMass()) * 5.0f * (120.0f / (GameMode->FPS + 1));
		resizeValue = FMath::Clamp(resizeValue, 0.0f, 0.01f);
		Resize(-resizeValue);
	}
}
// useless
void ARollingCodeBall::JumpStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (bCanJump)
	{
		const FVector Impulse = FVector::FVector::FVector(0.f, 0.f, JumpImpulse);
		Ball->AddImpulse(Impulse);
		bCanJump = false;
	}

}
// useless
void ARollingCodeBall::JumpStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (bCanJump)
	{
		const FVector Impulse = FVector(0.f, 0.f, JumpImpulse);
		Ball->AddImpulse(Impulse);
		bCanJump = false;
	}
}

void ARollingCodeBall::Split()
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::FromInt(GameMode->BallArray.Num()));
	Ball->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	bCanMerge = false;

	float newScale = Ball->RelativeScale3D.GetMax();
	newScale /= FMath::Pow(2.0f, 1.0f / 3.0f);
	Ball->SetRelativeScale3D(FVector(newScale, newScale, newScale));
	Ball->SetMassScale("None", newScale*MassScaleMultiplier);

	FVector spawnLocation;
	spawnLocation.X = Ball->GetComponentLocation().X;
	spawnLocation.Y = Ball->GetComponentLocation().Y;
	spawnLocation.Z = Ball->GetComponentLocation().Z + Radius;
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
}

void ARollingCodeBall::Destroyed()
{
	GameMode->BallArray.Remove(this);
}
