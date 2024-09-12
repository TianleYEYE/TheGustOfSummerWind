// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "UI/Button/BTN_ButtonBase.h"
#include "BTN_AlbumReturn.generated.h"

DECLARE_MULTICAST_DELEGATE(FEntrustAlbumReturn)
/**
 * 
 */
UCLASS()
class THEGUSTOFSUMMERWIND_API UBTN_AlbumReturn : public UBTN_ButtonBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UButton>BTN_AlbumReturn;

	UPROPERTY()
	TObjectPtr<UWidgetSwitcher> ParentSwitcher;

	// Function to set the reference to the parent widget
	void SetParentSwitcher(UWidgetSwitcher* InSwitcher);
	
	virtual void NativeConstruct() override;

	//创建委托实例
	FEntrustAlbumReturn AlbumReturn;
	//多播委托代理
	UFUNCTION()
	void DelegatedAgent();
};
