// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Border.h"
#include "UI/SW_UIBase.h"
#include "UI/Button/BTN_ButtonInMenu/BTN_ButtonOfInMenu.h"
#include "SW_InMenuUI.generated.h"

class USW_InGameUI;
class USoundBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMenuTransitionComplete);

UCLASS()
class THEGUSTOFSUMMERWIND_API USW_InMenuUI : public USW_UIBase
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UBTN_ButtonOfInMenu> BP_ButtonOfInMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UBorder> ScreenBlack;

	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FullScreenBlack;

	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Fade;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	TObjectPtr<USoundBase> ClickSound;

	UPROPERTY(BlueprintAssignable, Category = "Menu")
	FOnMenuTransitionComplete OnNewGameTransition;

	UPROPERTY(BlueprintAssignable, Category = "Menu")
	FOnMenuTransitionComplete OnContinueTransition;

	UPROPERTY(BlueprintAssignable, Category = "Menu")
	FOnMenuTransitionComplete OnAlbumTransition;

	UPROPERTY(BlueprintAssignable, Category = "Menu")
	FOnMenuTransitionComplete OnSettingTransition;

	FWidgetAnimationDynamicEvent TurnBlack;

	UFUNCTION()
	void PressBTN_NewGame();

	UFUNCTION()
	void PressBTN_Album();

	UFUNCTION()
	void PressBTN_Continue();

	UFUNCTION()
	void PressBTN_Setting();

	UFUNCTION()
	void FlashingOver();

	UFUNCTION()
	void OnFullScreenBlackFinished();

	void PlayATapSound();

	void SetScreenBlackZOrder(int32 NewZOrder);

protected:
	virtual UWidgetAnimation* GetFadeAnimation() const override { return Fade; }

private:
	enum class ETransitionTarget : uint8
	{
		None,
		NewGame,
		Continue,
		Album,
		Setting
	};

	ETransitionTarget CurrentTransition = ETransitionTarget::None;
};
