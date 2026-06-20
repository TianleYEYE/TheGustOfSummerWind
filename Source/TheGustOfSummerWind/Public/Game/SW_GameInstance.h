// Fill out your copyright notice in the Description page of Project Settings.

#pragma once 

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "SW_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class THEGUSTOFSUMMERWIND_API USW_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

private:
	void ApplyDefaultWindowSettings() const;
	static bool IsSixteenByNine(const FIntPoint& Resolution);
	
};
