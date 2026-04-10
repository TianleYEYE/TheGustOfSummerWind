// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SW_UIBase.h"

#include "Kismet/GameplayStatics.h"

ASW_ScriptManager* USW_UIBase::GetScriptManager()
{
	if (ScriptManager)
	{
		return ScriptManager;
	}

	AActor* TempActor = UGameplayStatics::GetActorOfClass(GetWorld(), ASW_ScriptManager::StaticClass());
	if (TempActor)
	{
		ScriptManager= Cast<ASW_ScriptManager>(TempActor);
	}
	return ScriptManager;
}
