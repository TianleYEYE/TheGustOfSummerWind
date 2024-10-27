// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

#include "SW_SaveGame.generated.h"

UENUM(BlueprintType)
enum ESaveSlotStatus
{
	Save,
	Load
};

/**
 * 
 */
UCLASS()
class THEGUSTOFSUMMERWIND_API USW_SaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FString SlotName=FString();

	UPROPERTY()
	TObjectPtr<USoundBase> BackgroundMusic;
	
	UPROPERTY()
	TObjectPtr<UDataTable>DataTable;

	UPROPERTY()
	int32 SlotIndex=0;

	UPROPERTY()
	int32 RowDialog =0;

	UPROPERTY()
	FString ChapterName=FString("Default Chapter");
	
	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SaveSlotStatus =Save;
};
