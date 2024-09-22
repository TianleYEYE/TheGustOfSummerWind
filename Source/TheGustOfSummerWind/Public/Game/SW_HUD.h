// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/SW_UICollection.h"
#include "SW_HUD.generated.h"


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
	

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Widget")
	TObjectPtr<USW_UICollection>UICollection;

	UPROPERTY(EditAnywhere,Category="Widget Class")
	TSubclassOf<USW_UICollection>UICollectionClass;

	
};
