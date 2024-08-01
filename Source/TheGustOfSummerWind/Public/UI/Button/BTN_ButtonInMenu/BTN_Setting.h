// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Button/BTN_ButtonBase.h"
#include "BTN_Setting.generated.h"

DECLARE_MULTICAST_DELEGATE(FEntrustSetting)

UCLASS()
class THEGUSTOFSUMMERWIND_API UBTN_Setting : public UBTN_ButtonBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UButton>BTN_Setting;
	
	UPROPERTY(Transient,EditAnywhere,BlueprintReadWrite,meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation>flashing;

	FEntrustSetting Setting;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void DelegatedAgent();
};
