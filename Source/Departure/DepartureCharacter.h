// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DepartureCharacter.generated.h"

UCLASS(config=Game)
class ADepartureCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* CollectionSphere;
public:
	ADepartureCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/**Accessor function for initial stamina power*/
	UFUNCTION(BlueprintPure, Category = "Stats")
		float GetInitialStamina();

	/**Accessor function for current stamina power*/
	UFUNCTION(BlueprintPure, Category = "Stats")
		float GetCurrentStamina();

	/**Function to update the character's stamina*/
	UFUNCTION(BlueprintCallable, Category = "Stats")
		void UpdateStamina(float StaminaUpdate);

	/**Accessor function for initial health level*/
	UFUNCTION(BlueprintPure, Category = "Stats")
		float GetInitialHealth();

	/**Accessor function for current health level*/
	UFUNCTION(BlueprintPure, Category = "Stats")
		float GetCurrentHealth();

	/**Function to update the character's health*/
	UFUNCTION(BlueprintCallable, Category = "Stats")
		void UpdateHealth(float HealthUpdate);

	//Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	//Whether or not the character is sprinting
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		bool IsSprinting;

	//Called when we press a key to have the character sprint
	UFUNCTION(BlueprintCallable, Category = "Stats")
		void StopCharacterSprint();

	//Called when we press a key to have the character sprint
	UFUNCTION(BlueprintCallable, Category = "Stats")
		void CharacterSprint();

	//The increase of speed when sprinting
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float SpeedFactor;

	//The original speed variable when not sprinting
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float BaseSpeed;

	//The initial stamina level of our character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float InitialStamina;

	//The initial health level of our character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float InitialHealth;

	private:

		//Current stamina level of our character
		UPROPERTY(VisibleAnywhere, Category = "Stats")
			float CharacterStamina;

		//Current health level of our character
		UPROPERTY(VisibleAnywhere, Category = "Stats")
			float CharacterHealth;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	/** Returns CollectionSphere subobject **/
	FORCEINLINE class USphereComponent* GetCollectionSphere() const { return CollectionSphere; }
};

