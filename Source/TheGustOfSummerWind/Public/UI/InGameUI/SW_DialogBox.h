// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CircularThrobber.h"
#include "Components/TextBlock.h"
#include "Components/Throbber.h"
#include "UI/SW_UIBase.h"
#include "SW_DialogBox.generated.h"

/**
 * 
 */
UCLASS()
class THEGUSTOFSUMMERWIND_API USW_DialogBox : public USW_UIBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UTextBlock>Textblock_Dialog;

	UPROPERTY(Transient,EditAnywhere,BlueprintReadWrite,meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation>DialogFade;

	UPROPERTY(Transient,EditAnywhere,BlueprintReadWrite,meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation>DialogShallow;

	UPROPERTY(Transient,EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UCircularThrobber>Throbber_1;
	
	virtual void NativeConstruct() override;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	FTimerHandle UpdataTextHandle;

	void UpdateVisibility();
};
