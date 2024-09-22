// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "DataStruct\DialogStruct.h"
#include "UI/Button/BTN_ButtonInGame/BTN_NextDialog.h"
#include "SW_ScriptManager.generated.h"


UCLASS()
class THEGUSTOFSUMMERWIND_API ASW_ScriptManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASW_ScriptManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Audio")
	TObjectPtr<UAudioComponent>AudioPlayer;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Audio")
	TObjectPtr<UAudioComponent>ConversationalVoicePlayer;

	UPROPERTY( BlueprintReadOnly, Category = "MySceneComponent")
	TObjectPtr<USceneComponent>SceneComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Chapter DataTable")
	TObjectPtr<UDataTable>DataTable;

	UPROPERTY()
	TObjectPtr<UBTN_NextDialog> BTN_NextDialog;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="properties")
	int rowDialog=0;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="properties")
	int MaxDialogIndex;
	
	int GetMaxDialogIndex();
	
	void MenuMusicPlay();
	UFUNCTION()
	void ChapterSwitch(int32 InRow);

	UFUNCTION()
	void OnUICollectionInitialized();

	FDialogStruct *SetDialogStruct();
	
	FDialogStruct *DialogStruct;
};
