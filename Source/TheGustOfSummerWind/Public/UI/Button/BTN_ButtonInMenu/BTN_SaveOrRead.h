// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Button/BTN_ButtonBase.h"
#include "BTN_SaveOrRead.generated.h"

/**
 * 
 */
UCLASS()
class THEGUSTOFSUMMERWIND_API UBTN_SaveOrRead : public UBTN_ButtonBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<USW_UIBase>BP_SaveGame;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<USW_UIBase>BP_LoadGame;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<USW_UIBase>BP_InGameSetting;
};
