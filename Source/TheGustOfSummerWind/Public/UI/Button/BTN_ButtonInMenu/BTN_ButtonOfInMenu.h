// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTN_Album.h"
#include "BTN_Continue.h"
#include "BTN_NewGame.h"
#include "BTN_Setting.h"
#include "UI/Button/BTN_ButtonBase.h"
#include "BTN_ButtonOfInMenu.generated.h"

/**
 * 
 */
UCLASS()
class THEGUSTOFSUMMERWIND_API UBTN_ButtonOfInMenu : public UBTN_ButtonBase
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UBTN_Album>BP_Album;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UBTN_NewGame>BP_NewGame;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UBTN_Setting>BP_Setting;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UBTN_Continue>BP_Continue;
};
