// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Pawn.h"
#include "RollingCodeGameMode.h"
#include "RollingCodeBall.generated.h"

UCLASS(config=Game)
class ARollingCodeBall : public APawn
{
	GENERATED_BODY()

	/** StaticMesh used for the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Ball;

	/** Spring arm for positioning the camera above the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	/** Camera to view the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

public:
	ARollingCodeBall();

	/** Vertical impulse to apply when pressing jump */
	UPROPERTY(EditAnywhere, Category=Ball)
	float JumpImpulse;

	/** Torque to apply when trying to roll ball */
	UPROPERTY(EditAnywhere, Category=Ball)
	float RollTorque;

	/** */
	UPROPERTY(EditAnywhere, Category = Ball)
		float Radius;
	/** */
//	UPROPERTY(EditAnywhere, Category = Ball)
//		RollingCodeGameMode GameMode;
	/** */
	UPROPERTY(EditAnywhere, Category = Ball)
		float MouseSensitivity;
	/** */
	UPROPERTY(EditAnywhere, Category = Ball)
		float ScrollSensitivity;
	/** */
	UPROPERTY(EditAnywhere, Category = Ball)
		float MassScaleMultiplier;
	/** */
	UPROPERTY(EditAnywhere, Category = Ball)
		bool bIsCameraAttached;
	/** */
	UPROPERTY(EditAnywhere, Category = Ball)
		bool bCanMerge;
	/** */
	UPROPERTY(EditAnywhere, Category = Ball)
		bool bOnFire;
	/** Indicates whether we can currently jump, use to prevent double jumping */
	UPROPERTY(EditAnywhere, Category = Ball)
		bool bCanJump;

	

protected:
	/** Called for side to side input */
	void MoveRight(float Val);

	/** Called to move ball forwards and backwards */
	void MoveForward(float Val);

	/** Handle jump action. */
	void Jump();

	/** Handle reset camera action. */
	void CamReset();

	/** Handle mouse X axis . */
	void CamRotateRight(float Val);

	/** Handle mouse Y axis . */
	void CamRotateUp(float Val); 

	/** Handle mouse scroll. */
	void CamZoomIn(float Val);

	// AActor interface
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	// End of AActor interface

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	/** Handler for when a touch input begins. */
	void JumpStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void JumpStopped(ETouchIndex::Type FingerIndex, FVector Location);

public:
	/** Returns Ball subobject **/
	FORCEINLINE class UStaticMeshComponent* GetBall() const { return Ball; }
	/** Returns SpringArm subobject **/
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }
};
