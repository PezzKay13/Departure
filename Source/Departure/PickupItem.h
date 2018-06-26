// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUpBase.h"
#include "PickupItem.generated.h"

/**
 * 
 */
UCLASS()
class DEPARTURE_API APickupItem : public APickUpBase
{
	GENERATED_BODY()

public:

	APickupItem();

	UFUNCTION(BlueprintPure, Category = "Pickup")
		float GetPower();

	UFUNCTION(BlueprintPure, Category = "Pickup")
		FString GetType();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (BlueprintProtected = "true"))
		float PickupPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (BlueprintProtected = "true"))
		FString PickupType;

};