// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SW_UIBase.h"
#include "SW_SettingUI.generated.h"


UCLASS()
class THEGUSTOFSUMMERWIND_API USW_SettingUI : public USW_UIBase
{
	GENERATED_BODY()
public:
	UPROPERTY(Transient,EditAnywhere,BlueprintReadWrite,meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation>Fade;

};
