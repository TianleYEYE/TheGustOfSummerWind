// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "UI/Button/BTN_ButtonBase.h"
#include "BTN_No.generated.h"

DECLARE_MULTICAST_DELEGATE(FEntrustNo)


UCLASS()
class THEGUSTOFSUMMERWIND_API UBTN_No : public UBTN_ButtonBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UButton>BTN_No;


	virtual void NativeConstruct() override;

	//创建委托实例
	FEntrustNo No;
	//多播委托代理
	UFUNCTION()
	void DelegatedAgent();
};
