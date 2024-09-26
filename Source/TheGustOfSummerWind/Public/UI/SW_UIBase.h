// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SW_UIBase.generated.h"

/**
 * 
 */
UCLASS()
class THEGUSTOFSUMMERWIND_API USW_UIBase : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void BlueprintInializeWidget();
};
