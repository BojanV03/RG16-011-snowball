// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "RG_Project.h"
#include "RollingCodeBall.h"

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
	SpringArm->RelativeRotation = FRotator(-45.f, 0.f, 0.f);
	SpringArm->TargetArmLength = 1200.f;
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 3.f;


	// Create a camera and attach to boom
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Set up forces
	RollTorque = 50000000.0f;
	JumpImpulse = 700.0f;
	// Other variables
	bCanJump = true; 
	MassScaleMultiplier = 3.5;
	bIsCameraAttached = true;
	bCanMerge = false;
	bOnFire = false;
	Radius = 100;
	MouseSensitivity = 5;
	ScrollSensitivity = 150;
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

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ARollingCodeBall::JumpStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ARollingCodeBall::JumpStopped);
}

void ARollingCodeBall::MoveRight(float Val)
{
	float radiusTimesMass = Ball->GetMass() * Ball->RelativeScale3D.GetMax();
	float a = -1 * (Val * RollTorque * radiusTimesMass) / 1000;

	FVector Torque = SpringArm->GetForwardVector() * a;
	Ball->AddTorque(Torque);
}

void ARollingCodeBall::MoveForward(float Val)
{
	float radiusTimesMass = Ball->GetMass() * Ball->RelativeScale3D.GetMax();
	float a = (Val * RollTorque * radiusTimesMass) / 1000;

	FVector ForwardVector = SpringArm->GetForwardVector();
	ForwardVector = FVector(ForwardVector.Y*-1, ForwardVector.X, 0);

	FVector Torque = ForwardVector * a;
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
	if (newLength > 1)
	{
		SpringArm->TargetArmLength = newLength;
	}
	else
	{
		SpringArm->TargetArmLength = 1.0f;
	}
}
void ARollingCodeBall::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	bCanJump = true;
}

void ARollingCodeBall::JumpStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (bCanJump)
	{
		const FVector Impulse = FVector::FVector::FVector(0.f, 0.f, JumpImpulse);
		Ball->AddImpulse(Impulse);
		bCanJump = false;
	}

}

void ARollingCodeBall::JumpStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (bCanJump)
	{
		const FVector Impulse = FVector(0.f, 0.f, JumpImpulse);
		Ball->AddImpulse(Impulse);
		bCanJump = false;
	}
}
