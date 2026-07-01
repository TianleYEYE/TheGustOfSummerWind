// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SW_UIBase.h"
#include "UI/Button/BTN_ButtonInMenu/BTN_ButtonOfInMenu.h"
#include "SW_InMenuUI.generated.h"

UCLASS()
class THEGUSTOFSUMMERWIND_API USW_InMenuUI : public USW_UIBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UBTN_ButtonOfInMenu> BP_ButtonOfInMenu;

	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Fade;

protected:
	virtual UWidgetAnimation* GetFadeAnimation() const override { return Fade; }
};
