// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "UI/Button/BTN_ButtonBase.h"
#include "BTN_Album.generated.h"

DECLARE_MULTICAST_DELEGATE(FEntrustAlbum)


UCLASS()
class THEGUSTOFSUMMERWIND_API UBTN_Album : public UBTN_ButtonBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UButton>BTN_Album;
	
	UPROPERTY(Transient,EditAnywhere,BlueprintReadWrite,meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation>flashing;

	FWidgetAnimationDynamicEvent Flashing;
	FEntrustAlbum Album;
	
	UPROPERTY()
	TObjectPtr<UWidgetSwitcher> ParentSwitcher;

	// Function to set the reference to the parent widget
	void SetParentSwitcher(UWidgetSwitcher* InSwitcher);
	

	UFUNCTION()
	void FlashingOver();
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	virtual void DelegatedAgentAndPlayAnimation()override;
};
