// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "UI/ViewModel/MVVM_System.h"
#include "SW_SystemData.generated.h"




/**
 * 
 */
UCLASS()
class THEGUSTOFSUMMERWIND_API USW_SystemData : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TMap<int32,FCGSlotData>CGSlotData;

	UPROPERTY()
	float TextDisplaySpeed = 0.1f;

	UPROPERTY()
	float MasterVolume = 0.5f;

	UPROPERTY()
	float SoundEffectVolume = 0.5f;
	
	
};
