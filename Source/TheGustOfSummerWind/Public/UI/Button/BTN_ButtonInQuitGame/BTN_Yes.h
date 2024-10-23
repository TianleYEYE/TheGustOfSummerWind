// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "UI/Button/BTN_ButtonBase.h"
#include "BTN_Yes.generated.h"

DECLARE_MULTICAST_DELEGATE(FEntrustYes)

UCLASS()
class THEGUSTOFSUMMERWIND_API UBTN_Yes : public UBTN_ButtonBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UButton>BTN_Yes;


	virtual void NativeConstruct() override;

	//创建委托实例
	FEntrustYes Yes;
	//多播委托代理
	UFUNCTION()
	void DelegatedAgent();
};
