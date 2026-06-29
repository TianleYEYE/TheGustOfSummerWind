// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SW_UIBase.h"
#include "SW_SettingUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSettingFadeComplete);

UCLASS()
class THEGUSTOFSUMMERWIND_API USW_SettingUI : public USW_UIBase
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Fade;

	UPROPERTY(BlueprintAssignable, Category = "Setting")
	FOnSettingFadeComplete OnSettingFadeComplete;

	UFUNCTION(BlueprintCallable, Category = "Setting")
	void PlayFadeOut();

	UFUNCTION(BlueprintCallable, Category = "Setting")
	void PlayFadeIn();

protected:
	UFUNCTION()
	void OnFadeFinished();

	virtual UWidgetAnimation* GetFadeAnimation() const override { return Fade; }
};
