// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DepartureGameMode.generated.h"

UCLASS(minimalapi)
class ADepartureGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADepartureGameMode();

	virtual void BeginPlay() override;

protected:

	//Widget class to use for the HUD screen
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CustomWidget", meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> HUDWidgetClass;

	//The instance of the HUD
	UPROPERTY()
		class UUserWidget* CurrentWidget;
};



