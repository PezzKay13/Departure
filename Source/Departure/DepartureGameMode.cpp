// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "DepartureGameMode.h"
#include "DepartureCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

ADepartureGameMode::ADepartureGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ADepartureGameMode::BeginPlay() {
	Super::BeginPlay();

	//Initialize the HUD when game starts
	if (HUDWidgetClass != nullptr) {

		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);


		if (CurrentWidget != nullptr) {

			CurrentWidget->AddToViewport();
		}
	}
}
