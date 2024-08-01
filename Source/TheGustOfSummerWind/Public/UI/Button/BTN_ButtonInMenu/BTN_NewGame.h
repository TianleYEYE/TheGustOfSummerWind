// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "UI/Button/BTN_ButtonBase.h"
#include "BTN_NewGame.generated.h"

DECLARE_MULTICAST_DELEGATE(FEntrustNewGame)


UCLASS()
class THEGUSTOFSUMMERWIND_API UBTN_NewGame : public UBTN_ButtonBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UButton>BTN_NewGame;

	UPROPERTY(Transient,EditAnywhere,BlueprintReadWrite,meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation>flashing;
	
	FEntrustNewGame NewGame;

	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void DelegatedAgent();
};
