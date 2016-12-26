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

	/** StaticMesh used for the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BallSkin;

	/** Spring arm for positioning the camera above the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	/** Camera to view the ball */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

public:
	ARollingCodeBall();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the ball is destroyed
	virtual void Destroyed() override;

	/** Vertical impulse to apply when pressing jump */
	UPROPERTY(EditAnywhere, Category=Ball)
	float JumpImpulse;

	/** Torque to apply when trying to roll ball */
	UPROPERTY(EditAnywhere, Category=Ball)
	float RollTorque;

	/** Radius of the ball */
	UPROPERTY(EditAnywhere, Category = Ball)
		float Radius;

	/** Reference to the currently used game mode */
	UPROPERTY(EditAnywhere, Category = Ball)
		ARollingCodeGameMode *GameMode;

	/** Mouse X/Y axis movement sensitivity multiplier */
	UPROPERTY(EditAnywhere, Category = Ball)
		float MouseSensitivity;

	/** Mouse scroll sensitivity multiplier */
	UPROPERTY(EditAnywhere, Category = Ball)
		float ScrollSensitivity;

	/** Multiply snowballs scale with this to calculate it's mass */
	UPROPERTY(EditAnywhere, Category = Ball)
		float MassScaleMultiplier;

	/** Is camera attached to the spring arm? If false, camera will remain static in the world */
	UPROPERTY(EditAnywhere, Category = Ball)
		bool bIsCameraAttached;

	/** Should this ball merge with another one upon impact*/
	UPROPERTY(EditAnywhere, Category = Ball)
		bool bCanMerge;

	//TODO
	/** Setting this to true disables the Snow particle system and enables the Fire particle system*/
	UPROPERTY(EditAnywhere, Category = Ball)
		bool bOnFire;

	/** Indicates whether we can currently jump, use to prevent double jumping */
	UPROPERTY(EditAnywhere, Category = Ball)
		bool bCanJump;

	UPROPERTY(EditAnywhere, Category = Ball)
		int BallSkinID;

	/** Handle ball resizing */
	UFUNCTION(BlueprintCallable, Category = Ball)
		void Resize(float Val);

protected:

	/** Called for side to side input */
	void MoveRight(float Val);

	/** Called to move ball forwards and backwards */
	void MoveForward(float Val);

	/** Handle jump action */
	void Jump();

	/** Handle reset camera action */
	void CamReset();

	/** Handle reset camera detachment */
	void CamDetach();

	/** Handle mouse X axis */
	void CamRotateRight(float Val);

	/** Handle mouse Y axis */
	void CamRotateUp(float Val); 

	/** Handle mouse scroll */
	void CamZoomIn(float Val);

	/** Increase ball when key is pressed */
	void IncreaseSize();

	/** Decrease ball when key is pressed */
	void DecreaseSize();

	/** Split a ball in half */
	void Split();

	/** bCanMerge = !bCanMerge */
	void ToggleMerge();

	/** Possess next Ball */
	void ChangeBallForward();

	/** Possess previous Ball */
	void ChangeBallBackward();

	// Called each time this collides with something
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	// Input Axis/Action setup
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

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
