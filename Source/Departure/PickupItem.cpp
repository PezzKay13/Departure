// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupItem.h"




//Set default values
APickupItem::APickupItem() {

	//The base power of the pickup
	PickupPower = 150.f;

	//Initialize the pickup type name
	PickupType = TEXT("");


}

float APickupItem::GetPower() {
	return PickupPower;
}



//Return the pickup type
FString APickupItem::GetType() {
	return PickupType;
}
