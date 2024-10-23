// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SW_UIBase.h"
#include "BTN_ButtonBase.generated.h"

/**
 * 
 */
UCLASS()
class THEGUSTOFSUMMERWIND_API UBTN_ButtonBase : public USW_UIBase
{
	GENERATED_BODY()
public:
	virtual void DelegatedAgentAndPlayAnimation();

	
};
