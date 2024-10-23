// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Game/SW_SystemData.h"
#include "SW_GameMode.generated.h"

class UMVVM_System;
class USW_SaveGame;
class USaveGame;
class UMVVM_LoadSlot;
/**
 * 
 */
UCLASS()
class THEGUSTOFSUMMERWIND_API ASW_GameMode : public AGameMode
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame>LoadScreenSaveGameClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame>SystemSaveGameClass;

	void SaveCGData(UMVVM_System* MVVMSystem);
	void SaveSlotData(UMVVM_LoadSlot* LoadSlot,int32 SlotIndex);
	USW_SaveGame* GetSaveSlotData(const FString& SlotName,int32 SlotIndex) const;
	USW_SystemData* GetCGSlotData() const;
};
