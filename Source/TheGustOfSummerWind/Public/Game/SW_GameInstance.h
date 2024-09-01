// Fill out your copyright notice in the Description page of Project Settings.

#pragma once 

#include "CoreMinimal.h"
#include "GlobalVariables/GlobalVariablesManager.h"


#include "SW_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class THEGUSTOFSUMMERWIND_API USW_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UGlobalVariablesManager>GlobalVariablesManger;

	void Init() override;
};
