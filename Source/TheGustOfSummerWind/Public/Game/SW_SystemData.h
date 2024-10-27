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
	
};
