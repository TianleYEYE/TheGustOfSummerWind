// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CircularThrobber.h"
#include "DataStruct\DialogStruct.h"
#include "UI/SW_UIBase.h"
#include "SW_DialogBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateText,FText,Text);

/**
 * 
 */
UCLASS()
class THEGUSTOFSUMMERWIND_API USW_DialogBox : public USW_UIBase
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	void SetDialogText(FText &Text);

	UPROPERTY(BlueprintAssignable, Category = "MyDelegate")
	FUpdateText UpdateDialogText; 
	
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly)
	FTimerHandle UpdataTextHandle;

};
