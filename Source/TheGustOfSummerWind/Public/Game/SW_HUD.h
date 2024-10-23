// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/SW_UICollection.h"
#include "UI/SettingUI/SW_SettingUI.h"
#include "UI/ViewModel/MVVM_System.h"
#include "SW_HUD.generated.h"


class UMVVM_LoadScreen;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUICollectionInitialized);
/**
 * 
 */
UCLASS()
class THEGUSTOFSUMMERWIND_API ASW_HUD : public AHUD
{
	GENERATED_BODY()
public:
	
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
	TObjectPtr<USW_AlbumUI>AlbumUI;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Widget")
	TObjectPtr<USW_SettingUI>SettingUI;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<USW_InMenuUI>MenuClass;

	
};
