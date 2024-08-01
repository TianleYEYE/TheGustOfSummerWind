// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "UI/Button/BTN_ButtonBase.h"
#include "BTN_Read.generated.h"



UCLASS()
class THEGUSTOFSUMMERWIND_API UBTN_Read : public UBTN_ButtonBase
{
	GENERATED_BODY()
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton>BTN_Read;
};
