// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTN_Auto.h"
#include "BTN_Read.h"
#include "BTN_Skip.h"
#include "UI/Button/BTN_ButtonBase.h"
#include "BTN_Button0fInGameUI.generated.h"

/**
 * 
 */
UCLASS()
class THEGUSTOFSUMMERWIND_API UBTN_Button0fInGameUI : public UBTN_ButtonBase
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UBTN_Auto>BP_Auto;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UBTN_Skip>BP_Skip;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UBTN_Read>BP_Read;
};
