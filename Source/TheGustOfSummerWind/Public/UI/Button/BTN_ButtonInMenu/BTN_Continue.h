// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Button/BTN_ButtonBase.h"
#include "BTN_Continue.generated.h"


DECLARE_MULTICAST_DELEGATE(FEntrustContinue)

UCLASS()
class THEGUSTOFSUMMERWIND_API UBTN_Continue : public UBTN_ButtonBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UButton>BTN_Continue;
	
	UPROPERTY(Transient,EditAnywhere,BlueprintReadWrite,meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation>flashing;

	FEntrustContinue Continue;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void DelegatedAgent();
	
};
