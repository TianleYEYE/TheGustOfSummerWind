// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/InMenuUI/SW_InMenuUI.h"
#include "UI/SettingUI/SW_SettingUI.h"
#include "UI/ViewModel/MVVM_System.h"
#include "SW_HUD.generated.h"


class UMVVM_LoadScreen;
class USW_InGameUI;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUICollectionInitialized);
/**
 * 
 */
UCLASS()
class THEGUSTOFSUMMERWIND_API ASW_HUD : public AHUD
{
	GENERATED_BODY()
public:
	ASW_HUD();
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadScreen>LoadScreenViewModelClass;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMVVM_LoadScreen>LoadScreenViewModel;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_System>SystemViewModelClass;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMVVM_System>SystemViewModel;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Widget")
	TObjectPtr<USW_InMenuUI>Menu;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Widget")
	TObjectPtr<USW_InGameUI>InGameUI;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Widget")
	TObjectPtr<USW_UIBase>ContinueUI;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Widget")
	TObjectPtr<USW_UIBase>AlbumUI;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Widget")
	TObjectPtr<USW_UIBase>SettingUI;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<USW_InMenuUI>MenuClass;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<USW_InGameUI> InGameUIClass;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<USW_UIBase> ContinueUIClass;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<USW_UIBase> AlbumUIClass;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<USW_UIBase> SettingUIClass;

	UFUNCTION()
	void HandleNewGameTransition();

	UFUNCTION()
	void HandleContinueTransition();

	UFUNCTION()
	void HandleAlbumTransition();

	UFUNCTION()
	void HandleSettingTransition();

	UFUNCTION()
	void HandleOverlayFadeComplete(bool bIsPlayingAnimation);

private:
	void InitializeViewModels();
	void CreateMenu();
	void BindMenuTransitions();
	void SetOverlayWidget(TObjectPtr<USW_UIBase>& Storage, TSubclassOf<USW_UIBase> WidgetClass, EWidgetStatus NewWidgetState, int32 ZOrder = 1);
	ASW_ScriptManager* GetScriptManager() const;
};
