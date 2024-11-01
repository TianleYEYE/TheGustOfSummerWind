// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Game/SW_ScriptManager.h"
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

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AudioPlayer")
	TObjectPtr<ASW_ScriptManager>ScriptManager;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UWidgetAnimation>ContinueUIFade;
	
	ASW_ScriptManager* GetScriptManager();
};
