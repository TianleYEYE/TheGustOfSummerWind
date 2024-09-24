// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/SW_UICollection.h"
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

	FOnUICollectionInitialized OnUICollectionInitialized;

	void InitializeUICollection();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadScreen>LoadScreenViewModelClass;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMVVM_LoadScreen>LoadScreenViewModel;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USW_UIBase>ContinueUIClass;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USW_UIBase>ContinueUI;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Widget")
	TObjectPtr<USW_UICollection>UICollection;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USW_UICollection>UICollectionClass;

	
};
