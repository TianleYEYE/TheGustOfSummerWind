// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Game/SW_ScriptManager.h"
#include "SW_UIBase.generated.h"

class UMVVM_System;
class UWidget;

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
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void BlueprintInializeWidget();

	UPROPERTY(EditAnywhere,Category="AudioPlayer")
	TObjectPtr<ASW_ScriptManager>ScriptManager;

	UPROPERTY(BlueprintAssignable)
	FPlayAnimation PlayWidgetFade;

	UPROPERTY(BlueprintAssignable)
	FPlayAnimation PlayingWidgetFade;

	UPROPERTY(Transient, BlueprintReadWrite, Category = "Backlog")
	bool bJumpToBacklogOnMouseDown = false;

	UPROPERTY(Transient, BlueprintReadWrite, Category = "Backlog")
	int32 BacklogIndex = INDEX_NONE;
	
	ASW_ScriptManager* GetScriptManager();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "System Settings")
	UMVVM_System* GetSystemViewModel() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	static bool AddWidgetToViewportOnce(UUserWidget* Widget, int32 ZOrder = 0);

	UFUNCTION(BlueprintCallable, Category = "UI")
	static bool RemoveWidgetFromParentIfValid(UWidget* Widget);
};
