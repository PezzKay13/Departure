// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "DepartureCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Classes/Components/SphereComponent.h"

//////////////////////////////////////////////////////////////////////////
// ADepartureCharacter

ADepartureCharacter::ADepartureCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//Create the collection sphere
	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
	CollectionSphere->SetupAttachment(RootComponent);
	CollectionSphere->SetSphereRadius(114.929367f);

	//Set the dependences of speed on stamina
	InitialStamina = 100.0f;
	InitialHealth = 100.0f;
	CharacterHealth = InitialHealth;
	CharacterStamina = InitialStamina;
	SpeedFactor = 2.0f;
	BaseSpeed = GetCharacterMovement()->MaxWalkSpeed;
	IsSprinting = false;
	PrimaryActorTick.bCanEverTick = true;
	CanHeal = true;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ADepartureCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ADepartureCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADepartureCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ADepartureCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ADepartureCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ADepartureCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ADepartureCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ADepartureCharacter::OnResetVR);

	//Custom Input created using C++ code
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ADepartureCharacter::CharacterSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ADepartureCharacter::StopCharacterSprint);
}


void ADepartureCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ADepartureCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ADepartureCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ADepartureCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ADepartureCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ADepartureCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ADepartureCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ADepartureCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (IsSprinting) {
		if (GetCurrentStamina() <= 0) {
			StopCharacterSprint();
		}
		else {
			UpdateStamina(-0.5f);
		}
	}
	if (GetCurrentStamina() < 100 && !IsSprinting) {
		UpdateStamina(0.25f);
	}
	if (GetCurrentHealth() < 100 && CanHeal) {
		UpdateHealth(0.22f);
	}
}

void ADepartureCharacter::CharacterSprint() {
	GetCharacterMovement()->MaxWalkSpeed *= SpeedFactor;
	IsSprinting = true;
}

void ADepartureCharacter::StopCharacterSprint() {
	GetCharacterMovement()->MaxWalkSpeed = BaseSpeed;
	IsSprinting = false;
}

float ADepartureCharacter::GetInitialStamina() {
	return InitialStamina;
}

float ADepartureCharacter::GetCurrentStamina() {
	return CharacterStamina;
}

void ADepartureCharacter::UpdateStamina(float StaminaUpdate) {
	CharacterStamina = GetCurrentStamina() + StaminaUpdate;
	if (GetCurrentStamina() > 100) {
		CharacterStamina = 100;
	}
	if (GetCurrentStamina() < 0) {
		CharacterStamina = 0;
	}
}

float ADepartureCharacter::GetInitialHealth() {
	return InitialHealth;
}

float ADepartureCharacter::GetCurrentHealth() {
	return CharacterHealth;
}

void ADepartureCharacter::UpdateHealth(float HealthUpdate) {
	CharacterHealth = GetCurrentHealth() + HealthUpdate;
	if (GetCurrentHealth() > 100) {
		CharacterHealth = 100;
	}
	if (GetCurrentHealth() < 0) {
		CharacterHealth = 0;
	}
}
