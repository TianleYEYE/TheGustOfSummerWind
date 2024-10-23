// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CanvasPanel.h"
#include "Components/WidgetSwitcher.h"
#include "UI/SW_UIBase.h"
#include "UI/Button/BTN_Return.h"
#include "UI/Button/BTN_ButtonInAlbum/BTN_AlbumReturn.h"
#include "UI/Button/BTN_ButtonInAlbum/BTN_Next.h"
#include "UI/Button/BTN_ButtonInAlbum/BTN_Previous.h"
#include "SW_AlbumUI.generated.h"


class UMVVM_System;

UCLASS()
class THEGUSTOFSUMMERWIND_API USW_AlbumUI : public USW_UIBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UBTN_Next>BP_Next;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UBTN_Previous>BP_Previous;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher>PageSwitcher;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UCanvasPanel>Page_1;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UCanvasPanel>Page_2;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UBTN_AlbumReturn>BP_Return;

	UPROPERTY(Transient,EditAnywhere,BlueprintReadWrite,meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation>Fade;

	// Function to set the reference to the parent widget
	void SetParentSwitcher(UWidgetSwitcher* InSwitcher);
	UFUNCTION()
	void OnReturnButtonClicked();
	
	
	UPROPERTY()
	TObjectPtr<UWidgetSwitcher> ParentSwitcher;
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void SwitchToNextPage_2();
	UFUNCTION()
	void SwitchToNextPage_1();
	
};
