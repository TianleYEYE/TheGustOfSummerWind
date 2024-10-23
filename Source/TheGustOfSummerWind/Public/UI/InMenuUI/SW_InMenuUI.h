// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Border.h"
//#include "Components/CanvasPanel.h"
#include "UI/SW_UIBase.h"
#include "UI/Button/BTN_ButtonInMenu/BTN_ButtonOfInMenu.h"
#include "SW_InMenuUI.generated.h"

class USW_InGameUI;
/**
 * 
 */
UCLASS()
class THEGUSTOFSUMMERWIND_API USW_InMenuUI : public USW_UIBase
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UBTN_ButtonOfInMenu>BP_ButtonOfInMenu;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UBorder>ScreenBlack;
	
	UPROPERTY(Transient,EditAnywhere,BlueprintReadWrite,meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation>FullScreenBlack;

	

	FWidgetAnimationDynamicEvent TurnBlack;
	
	virtual void NativeConstruct() override;
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

	void PlayATapSound();
	
	void SetScreenBlackZOrder(int32 NewZOrder);
};
