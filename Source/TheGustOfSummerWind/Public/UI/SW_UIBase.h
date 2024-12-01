// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Game/SW_ScriptManager.h"
#include "SW_UIBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayAnimation,bool,bIsPlayingAnimation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayingAnimation,bool,bIsPlayingAnimation);


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

	UPROPERTY(EditAnywhere,Category="AudioPlayer")
	TObjectPtr<ASW_ScriptManager>ScriptManager;

	UPROPERTY(BlueprintAssignable)
	FPlayAnimation PlayWidgetFade;

	UPROPERTY(BlueprintAssignable)
	FPlayAnimation PlayingWidgetFade;
	
	ASW_ScriptManager* GetScriptManager();
};
