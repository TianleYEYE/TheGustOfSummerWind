// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SW_UIBase.h"
#include "AlbumUI/SW_AlbumUI.h"
#include "Components/CanvasPanel.h"
#include "Components/WidgetSwitcher.h"
#include "ContinueUI/SW_ContinueUI.h"
#include "InGameUI/SW_InGameUI.h"
#include "InMenuUI/SW_InMenuUI.h"
#include "SW_UICollection.generated.h"

/**
 * 
 */
UCLASS()
class THEGUSTOFSUMMERWIND_API USW_UICollection : public USW_UIBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher>UISwitcher;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UCanvasPanel>MenuPage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UCanvasPanel>InGamePage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UCanvasPanel>ContinuePage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UCanvasPanel>AlbumPage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UCanvasPanel>SettingPage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<USW_InGameUI>BP_InGameUI;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<USW_InMenuUI>BP_MenuUI;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<USW_AlbumUI>BP_AlbumUI;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<USW_UIBase>BP_ContinueUI;
	
	
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void SetupWidgets();
	
	FTimerHandle TimerHandle;
};
